"""Fail-closed PE32, linker-map, and MSF 7/PDB contribution readers."""

from __future__ import annotations

from dataclasses import dataclass
import hashlib
from pathlib import Path
import re
import struct

MSF7_SIGNATURE = b"Microsoft C/C++ MSF 7.00\r\n\x1aDS\0\0\0"
DBI_SECTION_CONTRIB_V60 = 0xEFFE0000 + 19970605
DBI_SECTION_CONTRIB_V2 = 0xEFFE0000 + 20140516
IMAGE_SCN_CNT_CODE = 0x00000020
IMAGE_REL_BASED_HIGHLOW = 3


class LinkedImageError(ValueError):
    """Raised when linked-image evidence cannot be parsed unambiguously."""


def verify_capstone(lock: dict[str, object]) -> dict[str, object]:
    try:
        import capstone
    except ImportError as exc:
        raise LinkedImageError("Capstone is required for linked-image decoding") from exc
    if lock.get("authority") != "linked-image-field-decoder":
        raise LinkedImageError("Capstone lock has an invalid authority")
    if capstone.__version__ != str(lock.get("version", "")):
        raise LinkedImageError("Capstone version does not match the lock")
    root = Path(capstone.__file__).resolve().parent
    files = {
        "python_wrapper": Path(capstone.__file__).resolve(),
        "x86_wrapper": root / "x86.py",
        "x86_constants": root / "x86_const.py",
        "native_library": root / "lib" / "libcapstone.so",
    }
    result = {}
    for name, path in files.items():
        expected = str(lock.get(f"{name}_sha256", ""))
        if not path.is_file():
            raise LinkedImageError(f"Capstone {name} is missing")
        actual = hashlib.sha256(path.read_bytes()).hexdigest()
        if actual != expected:
            raise LinkedImageError(f"Capstone {name} identity does not match the lock")
        result[name] = {"sha256": actual, "size": path.stat().st_size}
    return {"version": capstone.__version__, "components": result}


def _need(data: bytes, offset: int, size: int, what: str) -> None:
    if offset < 0 or size < 0 or offset + size > len(data):
        raise LinkedImageError(f"truncated {what}")


def _c_string(data: bytes, offset: int, limit: int, what: str) -> tuple[str, int]:
    if not offset <= limit <= len(data):
        raise LinkedImageError(f"invalid {what} bounds")
    end = data.find(b"\0", offset, limit)
    if end < 0:
        raise LinkedImageError(f"unterminated {what}")
    return data[offset:end].decode("cp1252", errors="replace"), end + 1


@dataclass(frozen=True)
class PESection:
    number: int
    name: str
    virtual_size: int
    rva: int
    raw_size: int
    raw_offset: int
    characteristics: int


class PEImage:
    """Read only the PE32 fields required for linked-function diagnostics."""

    def __init__(self, path: Path | str):
        self.path = Path(path)
        self.data = self.path.read_bytes()
        _need(self.data, 0, 0x40, "DOS header")
        if self.data[:2] != b"MZ":
            raise LinkedImageError("missing MZ signature")
        pe = struct.unpack_from("<I", self.data, 0x3C)[0]
        _need(self.data, pe, 24, "PE header")
        if self.data[pe : pe + 4] != b"PE\0\0":
            raise LinkedImageError("missing PE signature")
        coff = pe + 4
        machine, section_count, self.timestamp, _, _, optional_size, _ = struct.unpack_from(
            "<HHIIIHH", self.data, coff
        )
        if machine != 0x14C:
            raise LinkedImageError("linked image is not i386")
        optional = coff + 20
        _need(self.data, optional, optional_size, "PE optional header")
        if optional_size < 96 or struct.unpack_from("<H", self.data, optional)[0] != 0x10B:
            raise LinkedImageError("linked image is not PE32")
        self.image_base = struct.unpack_from("<I", self.data, optional + 28)[0]
        directory_count = struct.unpack_from("<I", self.data, optional + 92)[0]
        directory_capacity = max(0, (optional_size - 96) // 8)
        self.directories = []
        for index in range(min(directory_count, directory_capacity)):
            self.directories.append(
                struct.unpack_from("<II", self.data, optional + 96 + index * 8)
            )
        self.sections = []
        section_table = optional + optional_size
        _need(self.data, section_table, section_count * 40, "PE section table")
        for index in range(section_count):
            offset = section_table + index * 40
            raw_name = struct.unpack_from("<8s", self.data, offset)[0]
            name = raw_name.split(b"\0", 1)[0].decode("ascii", errors="replace")
            virtual_size, rva, raw_size, raw_offset = struct.unpack_from(
                "<IIII", self.data, offset + 8
            )
            characteristics = struct.unpack_from("<I", self.data, offset + 36)[0]
            if raw_size:
                _need(self.data, raw_offset, raw_size, f"PE section {name}")
            self.sections.append(
                PESection(
                    index + 1,
                    name,
                    virtual_size,
                    rva,
                    raw_size,
                    raw_offset,
                    characteristics,
                )
            )

    @property
    def sha256(self) -> str:
        return hashlib.sha256(self.data).hexdigest()

    def section(self, number: int) -> PESection:
        if not 1 <= number <= len(self.sections):
            raise LinkedImageError(f"unknown PE section {number}")
        return self.sections[number - 1]

    def read_section(self, number: int, offset: int, size: int) -> bytes:
        section = self.section(number)
        if offset < 0 or size < 0 or offset + size > section.raw_size:
            raise LinkedImageError("linked extent leaves PE raw section data")
        start = section.raw_offset + offset
        return self.data[start : start + size]

    def read_address(self, address: int, size: int) -> bytes:
        rva = address - self.image_base
        for section in self.sections:
            offset = rva - section.rva
            if 0 <= offset and offset + size <= section.raw_size:
                return self.read_section(section.number, offset, size)
        raise LinkedImageError(f"address extent {address:#x}+{size:#x} has no raw data")

    def base_relocations(self) -> list[int]:
        if len(self.directories) <= 5:
            return []
        table_rva, table_size = self.directories[5]
        if not table_rva or not table_size:
            return []
        table = self.read_address(self.image_base + table_rva, table_size)
        cursor = 0
        addresses = []
        while cursor < len(table):
            _need(table, cursor, 8, "base-relocation block")
            page_rva, block_size = struct.unpack_from("<II", table, cursor)
            if block_size < 8 or block_size % 2 or cursor + block_size > len(table):
                raise LinkedImageError("invalid base-relocation block")
            for offset in range(cursor + 8, cursor + block_size, 2):
                entry = struct.unpack_from("<H", table, offset)[0]
                kind, page_offset = entry >> 12, entry & 0xFFF
                if kind == 0:
                    continue
                if kind != IMAGE_REL_BASED_HIGHLOW:
                    raise LinkedImageError(
                        f"unsupported i386 base relocation type {kind}"
                    )
                addresses.append(self.image_base + page_rva + page_offset)
            cursor += block_size
        return sorted(addresses)

    def codeview_record(self) -> dict[str, object]:
        if len(self.directories) <= 6:
            raise LinkedImageError("linked image lacks a debug directory")
        table_rva, table_size = self.directories[6]
        if not table_rva or table_size < 28 or table_size % 28:
            raise LinkedImageError("linked image lacks a complete debug directory")
        table = self.read_address(self.image_base + table_rva, table_size)
        records = []
        for offset in range(0, len(table), 28):
            _, _, _, _, kind, size, _, raw_offset = struct.unpack_from(
                "<IIHHIIII", table, offset
            )
            if kind != 2:
                continue
            _need(self.data, raw_offset, size, "PE CodeView record")
            record = self.data[raw_offset : raw_offset + size]
            if len(record) < 24 or record[:4] != b"RSDS":
                continue
            path, _ = _c_string(record, 24, len(record), "PE CodeView path")
            records.append(
                {
                    "guid": record[4:20].hex(),
                    "age": struct.unpack_from("<I", record, 20)[0],
                    "path": path,
                }
            )
        if len(records) != 1:
            raise LinkedImageError(
                f"expected one PE RSDS CodeView record, found {len(records)}"
            )
        return records[0]


def _msf_streams(path: Path | str) -> list[bytes | None]:
    data = Path(path).read_bytes()
    _need(data, 0, 56, "MSF 7 superblock")
    if data[:32] != MSF7_SIGNATURE:
        raise LinkedImageError("PDB is not an MSF 7.00 file")
    block_size, _, block_count, directory_size, _, block_map = struct.unpack_from(
        "<6I", data, 32
    )
    if block_size < 512 or block_size & (block_size - 1):
        raise LinkedImageError("invalid MSF block size")
    if block_count * block_size > len(data):
        raise LinkedImageError("MSF block count exceeds the PDB file")

    def block(number: int) -> bytes:
        if number >= block_count:
            raise LinkedImageError("MSF references a block outside the PDB")
        start = number * block_size
        return data[start : start + block_size]

    directory_block_count = (directory_size + block_size - 1) // block_size
    if directory_block_count * 4 > block_size:
        raise LinkedImageError("multi-block MSF directory block map is unsupported")
    map_data = block(block_map)
    directory_blocks = list(
        struct.unpack_from(f"<{directory_block_count}I", map_data, 0)
    )
    directory = b"".join(block(number) for number in directory_blocks)[:directory_size]
    _need(directory, 0, 4, "MSF stream directory")
    stream_count = struct.unpack_from("<I", directory, 0)[0]
    _need(directory, 4, stream_count * 4, "MSF stream sizes")
    sizes = struct.unpack_from(f"<{stream_count}I", directory, 4)
    cursor = 4 + stream_count * 4
    streams: list[bytes | None] = []
    for size in sizes:
        if size == 0xFFFFFFFF:
            streams.append(None)
            continue
        count = (size + block_size - 1) // block_size
        _need(directory, cursor, count * 4, "MSF stream block list")
        numbers = struct.unpack_from(f"<{count}I", directory, cursor) if count else ()
        cursor += count * 4
        streams.append(b"".join(block(number) for number in numbers)[:size])
    if cursor != len(directory):
        raise LinkedImageError("MSF stream directory has trailing or missing data")
    return streams


def pdb_section_contributions(path: Path | str) -> dict[str, object]:
    streams = _msf_streams(path)
    if len(streams) <= 3 or streams[1] is None or streams[3] is None:
        raise LinkedImageError("PDB lacks a DBI stream")
    info = streams[1]
    assert info is not None
    _need(info, 0, 28, "PDB info stream")
    info_version, _, info_age = struct.unpack_from("<III", info, 0)
    if info_version not in (19990903, 20000404, 20030901):
        raise LinkedImageError(f"unsupported PDB info version {info_version}")
    info_guid = info[12:28].hex()
    dbi = streams[3]
    assert dbi is not None
    _need(dbi, 0, 64, "DBI header")
    fields = struct.unpack_from("<iII6H5iI2i2HI", dbi, 0)
    version_signature, version_header, age = fields[:3]
    module_size = fields[9]
    contribution_size = fields[10]
    if version_signature != -1 or version_header not in (19990903, 20091201):
        raise LinkedImageError("unsupported DBI stream version")
    if age != info_age:
        raise LinkedImageError("PDB info and DBI ages disagree")
    if module_size < 0 or contribution_size < 4:
        raise LinkedImageError("invalid DBI substream sizes")
    _need(dbi, 64, module_size + contribution_size, "DBI module/contribution data")

    modules = []
    cursor = 64
    module_limit = cursor + module_size
    while cursor < module_limit:
        _need(dbi, cursor, 64, "DBI module record")
        module_name, after_module = _c_string(
            dbi, cursor + 64, module_limit, "DBI module name"
        )
        object_name, after_object = _c_string(
            dbi, after_module, module_limit, "DBI object name"
        )
        next_cursor = (after_object + 3) & ~3
        if next_cursor <= cursor or next_cursor > module_limit:
            raise LinkedImageError("invalid DBI module record alignment")
        modules.append(
            {
                "index": len(modules),
                "module": module_name,
                "object": object_name,
                "debug_stream": struct.unpack_from("<H", dbi, cursor + 34)[0],
            }
        )
        cursor = next_cursor
    if cursor != module_limit:
        raise LinkedImageError("DBI module substream extent mismatch")

    contribution_start = module_limit
    version = struct.unpack_from("<I", dbi, contribution_start)[0]
    if version == DBI_SECTION_CONTRIB_V60:
        record_size = 28
    elif version == DBI_SECTION_CONTRIB_V2:
        record_size = 32
    else:
        raise LinkedImageError(f"unsupported DBI contribution version {version:#x}")
    payload_size = contribution_size - 4
    if payload_size % record_size:
        raise LinkedImageError("DBI contribution records have an invalid extent")
    contributions = []
    for offset in range(
        contribution_start + 4, contribution_start + contribution_size, record_size
    ):
        section, contribution_offset, size, characteristics, module = struct.unpack_from(
            "<H2xiiIH2x", dbi, offset
        )
        if contribution_offset < 0 or size < 0:
            raise LinkedImageError("negative DBI contribution extent")
        if module >= len(modules):
            raise LinkedImageError("DBI contribution references an unknown module")
        contributions.append(
            {
                "section_number": section,
                "section_offset": contribution_offset,
                "size": size,
                "characteristics": characteristics,
                "module_index": module,
                "module": modules[module]["module"],
                "object": modules[module]["object"],
            }
        )
    return {
        "guid": info_guid,
        "pdb_age": info_age,
        "version_header": version_header,
        "age": age,
        "modules": modules,
        "contributions": contributions,
    }


MAP_PUBLIC_RE = re.compile(
    r"^\s*([0-9A-Fa-f]{4}):([0-9A-Fa-f]{8})\s+(\S+)\s+"
    r"([0-9A-Fa-f]{8})\s+(.+?)\s*$"
)

MAP_TIMESTAMP_RE = re.compile(r"^\s*Timestamp is ([0-9A-Fa-f]+)\b")
MAP_BASE_RE = re.compile(r"^\s*Preferred load address is ([0-9A-Fa-f]+)\b")


def map_metadata(path: Path | str) -> dict[str, int]:
    timestamp = None
    image_base = None
    for line in Path(path).read_text(encoding="cp1252", errors="replace").splitlines():
        timestamp_match = MAP_TIMESTAMP_RE.match(line)
        if timestamp_match is not None:
            timestamp = int(timestamp_match.group(1), 16)
        base_match = MAP_BASE_RE.match(line)
        if base_match is not None:
            image_base = int(base_match.group(1), 16)
        if timestamp is not None and image_base is not None:
            break
    if timestamp is None or image_base is None:
        raise LinkedImageError("linker map lacks timestamp or preferred load address")
    return {"timestamp": timestamp, "image_base": image_base}


def map_publics(path: Path | str) -> list[dict[str, object]]:
    lines = Path(path).read_text(encoding="cp1252", errors="replace").splitlines()
    in_publics = False
    result = []
    for line in lines:
        if "Publics by Value" in line:
            in_publics = True
            continue
        if in_publics and line.lstrip().startswith("entry point at"):
            break
        if not in_publics:
            continue
        match = MAP_PUBLIC_RE.match(line)
        if match is None:
            continue
        section, offset, symbol, address, remainder = match.groups()
        tokens = remainder.split()
        if not tokens:
            raise LinkedImageError("linker-map public lacks an object owner")
        result.append(
            {
                "section_number": int(section, 16),
                "section_offset": int(offset, 16),
                "symbol": symbol,
                "address": int(address, 16),
                "flags": tokens[:-1],
                "object": tokens[-1],
            }
        )
    if not result:
        raise LinkedImageError("linker map contains no public symbols")
    return result


def linked_code_fields(
    image: PEImage,
    publics: list[dict[str, object]],
    address: int,
    size: int,
) -> dict[str, object]:
    """Enumerate link-resolved fields in one complete linked code extent."""
    try:
        from capstone import CS_ARCH_X86, CS_GRP_CALL, CS_GRP_JUMP, CS_MODE_32, Cs
        from capstone.x86_const import X86_OP_IMM
    except ImportError as exc:
        raise LinkedImageError("Capstone is required for linked-image decoding") from exc
    code = image.read_address(address, size)
    symbols_by_address: dict[int, list[str]] = {}
    for public in publics:
        symbols_by_address.setdefault(int(public["address"]), []).append(
            str(public["symbol"])
        )
    fields: list[dict[str, object]] = []
    base_relocation_bytes: set[int] = set()
    for relocation_address in image.base_relocations():
        if relocation_address < address + size and relocation_address + 4 > address:
            if not address <= relocation_address or relocation_address + 4 > address + size:
                raise LinkedImageError("base relocation crosses the function boundary")
        if address <= relocation_address and relocation_address + 4 <= address + size:
            offset = relocation_address - address
            destination = struct.unpack_from("<I", code, offset)[0]
            base_relocation_bytes.update(range(offset, offset + 4))
            fields.append(
                {
                    "offset": offset,
                    "width": 4,
                    "type": "DIR32",
                    "candidate_target": destination,
                    "candidate_symbols": sorted(
                        symbols_by_address.get(destination, [])
                    ),
                    "instruction_end": None,
                }
            )

    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    decoded = 0
    for instruction in decoder.disasm(code, address):
        if instruction.address != address + decoded:
            break
        decoded += instruction.size
        is_control = instruction.group(CS_GRP_CALL) or instruction.group(CS_GRP_JUMP)
        if not is_control or not instruction.operands:
            continue
        operand = instruction.operands[0]
        if operand.type != X86_OP_IMM or not instruction.imm_size:
            continue
        destination = int(operand.imm) & 0xFFFFFFFF
        if address <= destination < address + size:
            continue
        relative_offset = instruction.address - address + instruction.imm_offset
        relative_extent = set(range(relative_offset, relative_offset + instruction.imm_size))
        # A PDB contribution may own an inline jump table after its executable
        # body. Capstone necessarily decodes those data bytes as instructions;
        # a PE base relocation is authoritative when such a false control-flow
        # immediate overlaps one of the table's absolute entries.
        if relative_extent & base_relocation_bytes:
            continue
        fields.append(
            {
                "offset": relative_offset,
                "width": instruction.imm_size,
                "type": f"REL{instruction.imm_size * 8}",
                "instruction": instruction.mnemonic,
                "candidate_target": destination,
                "candidate_symbols": sorted(symbols_by_address.get(destination, [])),
                "instruction_end": instruction.address - address + instruction.size,
            }
        )
    fields.sort(key=lambda field: (int(field["offset"]), str(field["type"])))
    occupied: set[int] = set()
    for field in fields:
        extent = set(
            range(
                int(field["offset"]),
                int(field["offset"]) + int(field["width"]),
            )
        )
        if min(extent, default=0) < 0 or max(extent, default=-1) >= size:
            raise LinkedImageError("linked-image field leaves the function extent")
        if occupied & extent:
            raise LinkedImageError("overlapping linked-image fields")
        occupied.update(extent)
    return {
        "decoded_bytes": decoded,
        "normalization_complete": decoded == len(code),
        "fields": fields,
    }


def linked_functions(
    image_path: Path | str, map_path: Path | str, pdb_path: Path | str
) -> dict[str, object]:
    image = PEImage(image_path)
    publics = map_publics(map_path)
    metadata = map_metadata(map_path)
    pdb = pdb_section_contributions(pdb_path)
    codeview = image.codeview_record()
    if metadata["timestamp"] != image.timestamp:
        raise LinkedImageError("linker map timestamp disagrees with the PE")
    if metadata["image_base"] != image.image_base:
        raise LinkedImageError("linker map image base disagrees with the PE")
    if codeview["guid"] != pdb["guid"] or codeview["age"] != pdb["pdb_age"]:
        raise LinkedImageError("PDB identity disagrees with the PE CodeView record")
    by_start: dict[tuple[int, int], list[dict[str, object]]] = {}
    for public in publics:
        by_start.setdefault(
            (int(public["section_number"]), int(public["section_offset"])), []
        ).append(public)
    functions = []
    unowned = []
    for contribution in pdb["contributions"]:
        if not int(contribution["characteristics"]) & IMAGE_SCN_CNT_CODE:
            continue
        section_number = int(contribution["section_number"])
        section_offset = int(contribution["section_offset"])
        size = int(contribution["size"])
        section = image.section(section_number)
        address = image.image_base + section.rva + section_offset
        starts = by_start.get((section_number, section_offset), [])
        if not starts:
            unowned.append(
                {
                    **contribution,
                    "address": f"0x{address:08X}",
                    "section": section.name,
                }
            )
            continue
        code = image.read_section(section_number, section_offset, size)
        for public in starts:
            if int(public["address"]) != address:
                raise LinkedImageError("linker map VA disagrees with PE section layout")
            functions.append(
                {
                    "symbol": public["symbol"],
                    "address": f"0x{address:08X}",
                    "size": size,
                    "extent_source": "pdb-section-contribution",
                    "section": section.name,
                    "section_number": section_number,
                    "section_offset": section_offset,
                    "map_object": public["object"],
                    "pdb_module_index": contribution["module_index"],
                    "pdb_module": contribution["module"],
                    "pdb_object": contribution["object"],
                    "sha256": hashlib.sha256(code).hexdigest(),
                }
            )
    functions.sort(
        key=lambda item: (int(str(item["address"]), 0), str(item["symbol"]))
    )
    return {
        "image": str(Path(image_path)),
        "image_sha256": image.sha256,
        "image_base": f"0x{image.image_base:08X}",
        "timestamp": f"0x{image.timestamp:08X}",
        "pdb_guid": pdb["guid"],
        "pdb_age": pdb["pdb_age"],
        "function_count": len(functions),
        "unowned_code_contribution_count": len(unowned),
        "functions": functions,
        "unowned_code_contributions": unowned,
    }
