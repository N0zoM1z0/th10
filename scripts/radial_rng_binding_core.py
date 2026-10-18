"""Pure, fixed-scope radial field binder. No I/O, decoding, or execution authority.

The pinned runtime supplies two independently decoded complete 803-byte owners,
verified symbol/alias evidence, and a checked scalar reader. The scalar reader
must validate unique readonly initialized non-executable, non-relocated storage
and all loader/raw/virtual bounds BEFORE reading any bytes. This module performs
its own stricter request budgeting, deduplication, typed-content checks, and
binding/structure checks. It never learns an expected identity from a field.
"""
from __future__ import annotations

from dataclasses import asdict, dataclass
import hashlib
import json
import re
from types import MappingProxyType
from typing import Callable, Mapping, Sequence

OWNER_SIZE = 803
FIELD_COUNT = 43
FIELD_BYTES = 172
MAX_DATA_BYTES = 344
MAX_DATA_READS = 43
X87_READS = frozenset(("fld", "fadd", "fmul", "fsub", "fsubr", "fdiv", "fdivr", "fcom", "fcomp"))
CONFIG_SHA256 = "8811679fb37ab220216a8926a9ce46b8637335e47cea7cd8d5d8a78b6004cc88"
LEDGER_SHA256 = "19d7e4eb1aab50f97afba28cd7a5c6ed529a771f18bd9ccf4997aee6a1954350"
RNG_LAYOUT_SHA256 = "7600d386e72c0cab29cea5e1c50ab08a6b7390b05d641a595c01921af4bcd49c"
RNG_OBJECT_POLICY = "reviewed-rng-source-object-address-v1"
RNG_OBJECT_SCHEMA = 1
RNG_SOURCE_PINS = tuple(sorted({
    "src/Rng.hpp": RNG_LAYOUT_SHA256,
    "src/RandomMath.cpp": "88af248abd6b7e5732ae9c34eadbd3fb524ff35d5740432762c23dd9e35466c4",
    "candidate/src/RandomMath.cpp": "5866b8573e21b45ff421d4e3930a1d58660a4cf7725e39fd9ec319cc04f7fca1",
    "src/AnmManager.cpp": "26bf39d6a7446311f373fd972477207c8ef950d54c59c0b68f85f0ccc64669ab",
}.items()))
RNG_ARTIFACT_PINS = tuple(sorted({
    "source.exe": "29eb29c4d60d0055375180a453a0eaddd5c3ea8d639671a53d9edbe88758a3b0",
    "source.map": "3c0995e1037f0841ffce385a91a3e53718fe74cb94b307f9c4bc2177bc2267be",
    "source.pdb": "d095ee68683f0566e4d29df7cf9a9cfb0456624f0c47cfcba0805f44b9942413",
    "source.ltcg.obj": "cd6a6bbf13fbafd127f846408911a735d48e7b76eac8320753be8301d3988542",
    "support-00-RandomMath.ltcg.obj": "135ccb2621d7e2892765365b5b3e8b486c828b687ee47726d56468f3c7fa6041",
    "support-01-AnmVmCreate.ltcg.obj": "de8f9b508aa7d3936c349448c6726cdcb780eefdc07281f61308a0773f1aa617",
    "support-02-AnmVmId.ltcg.obj": "c27131265363f7edc7015ab9fc0909b19f3858c5d4386baabf5f43636c848636",
    "link-aliases.rsp": "983c8d976de42513986dc6629b1893a5fd458da2b6fbc73b2055cb5b39666874",
    "link-anchors.c": "bcbaf152451e020167642ee978a35c7c80b783bf646d4cc7c244c8163502f088",
    "link-anchors.obj": "ab3ea58e8f61ad8cfcac42d030064033f180ae6b1165ae81b1ef89a679210fd4",
}.items()))
RNG_LAYOUT = (("seed", 0, 2), ("savedSeed", 2, 2), ("generationCount", 4, 4))
RNG_CANONICAL_UNIT = "text-create-buffer"
RNG_CANONICAL_ROWS = tuple((index, offset, "DIR32", "?g_RngView@@3URngView@@A", 0x4918B0, addend, 4)
                           for index, offset, addend in ((4, 45, 4), (5, 52, 0), (6, 83, 0), (9, 144, 4)))
RNG_CALL_CHAIN = (("InitializePulsingRadialTrail", "g_RngView.GetRandomF32Signed"),
                  ("RngView.GetRandomF32Signed", "RngView.GetRandomU32"),
                  ("RngView.GetRandomU32", "RngView.GetRandomU16 twice"),
                  ("RngView.GetRandomU16", "seed read/write; generationCount increment"))
RNG_CALL_CHAIN_SHA = hashlib.sha256(json.dumps({"source_pins": RNG_SOURCE_PINS, "chain": RNG_CALL_CHAIN},
    sort_keys=True, separators=(",", ":")).encode()).hexdigest()
# Values read from the hash-pinned Capstone 5.0.6 x86_const.py, not inferred.
RNG_X86_CONSTANTS_SHA = "00dcf65aad410f95a61dacd1dadacfe11b0f19b2afbeb07d19a484d35c692c38"
RNG_MOV_ID = 460
RNG_GPR32 = frozenset((19, 20, 21, 22, 23, 24, 29, 30))
RNG_BOUND_STATUS = "bound_rng_source_object"


@dataclass(frozen=True, slots=True, repr=False)
class RngObjectAuthority:
    policy: str
    schema_version: int
    source_pins: tuple[tuple[str, str], ...]
    artifact_pins: tuple[tuple[str, str], ...]
    config_sha256: str
    canonical_unit: str
    canonical_rows: tuple[tuple, ...]
    layout: tuple[tuple[str, int, int], ...]
    object_size: int
    call_chain_sha256: str
    alias_mapping: tuple[tuple[str, str], ...]


@dataclass(frozen=True, slots=True, repr=False)
class CanonicalAliasPair:
    source_symbol: str
    anchor_symbol: str
    address: int
    public_names: tuple[str, ...]
    all_names: tuple[str, ...]


@dataclass(frozen=True, slots=True, repr=False)
class RngObjectFieldEvidence:
    authority: RngObjectAuthority
    key: str
    symbol: str
    offset: int
    addend: int
    candidate_destination: int
    target_destination: int
    field_shape: tuple[tuple[str, object], ...]
    base_pair: CanonicalAliasPair
    destination_pair: CanonicalAliasPair


@dataclass(frozen=True)
class BindingSpec:
    key: str
    symbol: str
    role: str
    addends: tuple[int, ...]
    target_address: int | None
    provenance_sha256: str
    allow_typed_constant: bool = False


@dataclass(frozen=True)
class SymbolEvidence:
    symbol: str
    address: int
    storage_kind: str
    unique: bool
    provenance_verified: bool
    alias_symbol: str | None = None
    alias_verified: bool = False
    same_address_symbols: tuple[str, ...] = ()


@dataclass(frozen=True, repr=False)
class BindingResult:
    aggregate: dict
    private_rows: tuple[dict, ...]

    def __repr__(self) -> str:
        return f"BindingResult(aggregate={self.aggregate!r}, private_rows=<remote-only>)"


@dataclass(frozen=True)
class KnownBinding:
    symbol: str
    role: str
    addends: tuple[int, ...]
    target_address: int | None
    constant_type: str | None = None
    constant_bits: str | None = None


_REGISTRY = {
    "direct_free": KnownBinding("_free", "function", (0,), 0x00452422),
    "wrapper_free": KnownBinding("_free", "function", (0,), 0x004524A1),
    "malloc": KnownBinding("_malloc", "function", (0,), 0x00452706),
    "update_callback": KnownBinding("?UpdatePulsingRadialTrail@@YIHPAUAnmVmView@@@Z", "function", (0,), 0x00445620),
    "draw_callback": KnownBinding("?DrawPulsingRadialTrail@@YIHPAUAnmVmView@@@Z", "function", (0,), 0x00445880),
    "rng_global": KnownBinding("?g_RngView@@3URngView@@A", "global", (0, 4), 0x004918B0),
    "vector_helper": KnownBinding("?FromAngleMagnitude@AnmFloat3View@@QAEXMM@Z", "function", (0,), 0x004458B0),
    "rng_f32_signed": KnownBinding("?GetRandomF32Signed@RngView@@QAEMXZ", "function", (0,), 0x0044BB90),
    "rng_u32": KnownBinding("?GetRandomU32@RngView@@QAEIXZ", "function", (0,), 0x0044B9E0),
    "rng_u16": KnownBinding("?GetRandomU16@RngView@@QAEGXZ", "function", (0,), 0x0044B9B0),
    "rng_f32": KnownBinding("?GetRandomF32@RngView@@QAEMXZ", "function", (0,), 0x0044BB20),
}
# These are source/config query identities fixed before any native census. They
# do not assert that the compiler emitted them or that any field references one.
_CONSTANTS = (
    ("00000000", 0x00470B04), ("3f800000", 0x00470AFC),
    ("3f000000", 0x00470B0C), ("c0490fdb", 0x00470B10),
    ("40490fdb", 0x00470B18), ("40c90fdb", 0x00470B14),
    ("3e4f8c3d", 0x00470B6C), ("41000000", None),
    ("42a00000", None), ("42f00000", None), ("41700000", None),
    ("41f00000", 0x00470C30), ("3c088889", None),
    ("3d888889", None), ("3d088889", None), ("bd888889", None),
    ("4f000000", None), ("4f800000", 0x00470B98),
    ("30000000", 0x00470BEC), ("2f800000", 0x00470BF0),
)
for _bits, _address in _CONSTANTS:
    _REGISTRY["const_" + _bits] = KnownBinding("__real@" + _bits, "constant", (0,), _address, "binary32", _bits)
REGISTRY: Mapping[str, KnownBinding] = MappingProxyType(_REGISTRY)
EXPORT_KEYS = tuple(REGISTRY)

STATUS_CODES = (
    "bound_known_identity", "bound_typed_instance", "candidate_identity_unknown",
    "candidate_identity_ambiguous", "candidate_identity_unverified", "candidate_alias_unverified",
    "candidate_addend_collision",
    "candidate_storage_kind_mismatch", "unsupported_operand", "target_identity_mismatch",
    "missing_target_identity", "constant_type_mismatch", "constant_alias_ambiguous",
    "constant_storage_ambiguous", "constant_read_rejected", "constant_content_mismatch",
    "constant_budget_exceeded",
)
BOUND_STATUSES = frozenset(("bound_known_identity", "bound_typed_instance", RNG_BOUND_STATUS))
STRUCTURE_KEYS = (
    "offset", "width", "type", "instruction_end", "instruction_offset", "instruction_size",
    "instruction_id", "instruction_mnemonic", "operand_structure", "field_encoding",
    "operand_kind", "operand_width", "constant_read_eligible", "constant_value_type",
)


class BindingError(ValueError):
    """Fixed code only; no observed address, symbol, native byte, or raw error."""


def _need(value: object, code: str) -> None:
    if not value:
        raise BindingError(code)


def _uint(value: object, maximum: int = 0xFFFFFFFF) -> bool:
    return type(value) is int and 0 <= value <= maximum


def _digest(value: object) -> str:
    return hashlib.sha256(json.dumps(value, sort_keys=True, separators=(",", ":")).encode()).hexdigest()


def freeze_field_shape(field: dict) -> tuple[tuple[str, object], ...]:
    """Private immutable exact correspondence to an already validated field."""
    def freeze(value):
        return tuple(freeze(part) for part in value) if type(value) in (list, tuple) else value
    return tuple((key, freeze(field[key])) for key in STRUCTURE_KEYS)


def _exact_frozen(value, expected):
    if type(value) is not type(expected):
        return False
    if type(expected) is tuple:
        return len(value) == len(expected) and all(_exact_frozen(a, b) for a, b in zip(value, expected))
    return value == expected


def _rng_authority_gate(value: RngObjectAuthority) -> dict[str, str]:
    _need(type(value) is RngObjectAuthority, "invalid-rng-authority-schema")
    _need(value.policy == RNG_OBJECT_POLICY and type(value.schema_version) is int
          and value.schema_version == RNG_OBJECT_SCHEMA, "invalid-rng-policy")
    _need(_exact_frozen(value.source_pins, RNG_SOURCE_PINS), "rng-source-pin-drift")
    _need(_exact_frozen(value.artifact_pins, RNG_ARTIFACT_PINS), "rng-artifact-pin-drift")
    _need(value.config_sha256 == CONFIG_SHA256 and value.canonical_unit == RNG_CANONICAL_UNIT
          and _exact_frozen(value.canonical_rows, RNG_CANONICAL_ROWS),
          "rng-canonical-authority-drift")
    _need(_exact_frozen(value.layout, RNG_LAYOUT)
          and type(value.object_size) is int and value.object_size == 8, "rng-layout-drift")
    _need(value.call_chain_sha256 == RNG_CALL_CHAIN_SHA, "rng-call-chain-drift")
    mapping = value.alias_mapping
    _need(type(mapping) is tuple and 0 < len(mapping) <= 4096
          and all(type(pair) is tuple and len(pair) == 2 and
                  all(type(name) is str and 0 < len(name) <= 512 and name.isascii()
                      and not any(char in name for char in "\r\n\0=") for name in pair)
                  for pair in mapping), "invalid-rng-alias-mapping")
    _need(tuple(sorted(mapping)) == mapping and len({pair[0] for pair in mapping}) == len(mapping),
          "invalid-rng-alias-order")
    _need(all(anchor == "_th10_ltcg_probe_anchor_%04d" % index
              for index, (source, anchor) in enumerate(mapping)), "invalid-rng-canonical-alias")
    aliases = dict(mapping)
    source = '/* Diagnostic-only, non-runnable unresolved-symbol address anchors. */\n' + ''.join(
        'unsigned char %s[4] = {0};\n' % anchor[1:] for anchor in aliases.values())
    rsp = ''.join('/alternatename:%s=%s\n' % pair for pair in mapping)
    pins = dict(RNG_ARTIFACT_PINS)
    _need(hashlib.sha256(source.encode('ascii')).hexdigest() == pins['link-anchors.c']
          and hashlib.sha256(rsp.encode('ascii')).hexdigest() == pins['link-aliases.rsp'],
          "rng-alias-content-pin-drift")
    return aliases


def _rng_pair_gate(pair: CanonicalAliasPair, aliases: dict[str, str]) -> None:
    _need(type(pair) is CanonicalAliasPair and type(pair.source_symbol) is str
          and pair.source_symbol in aliases and aliases[pair.source_symbol] == pair.anchor_symbol
          and pair.source_symbol != pair.anchor_symbol and _uint(pair.address, 0xFFFFFFFC),
          "invalid-rng-canonical-pair")
    expected = tuple(sorted((pair.source_symbol, pair.anchor_symbol)))
    _need(type(pair.all_names) is tuple and pair.all_names == expected
          and type(pair.public_names) is tuple and len(pair.public_names) in (1, 2)
          and all(type(name) is str for name in pair.public_names)
          and pair.public_names == tuple(sorted(set(pair.public_names)))
          and pair.anchor_symbol in pair.public_names and set(pair.public_names) <= set(expected),
          "invalid-rng-complete-pair")


def _rng_evidence_gate(records, candidate, target, specs, evidence):
    _need(type(records) in (tuple, list) and len(records) <= FIELD_COUNT, "invalid-rng-evidence-count")
    result = {}
    authorities = {}
    by_offset = {left['offset']: (left, right) for left, right in zip(candidate, target)}
    for record in records:
        _need(type(record) is RngObjectFieldEvidence and _uint(record.offset, OWNER_SIZE - 4)
              and record.offset in by_offset and record.offset not in result, "invalid-rng-field-record")
        _need(record.key == 'rng_global' and record.key in specs
              and record.symbol == REGISTRY['rng_global'].symbol, "invalid-rng-field-key")
        _need(type(record.addend) is int and record.addend in (0, 4), "invalid-rng-member")
        authority_id = id(record.authority)
        if authority_id not in authorities:
            authorities[authority_id] = _rng_authority_gate(record.authority)
        aliases = authorities[authority_id]
        _rng_pair_gate(record.base_pair, aliases)
        _rng_pair_gate(record.destination_pair, aliases)
        left, right = by_offset[record.offset]
        _need(_exact_frozen(record.field_shape, freeze_field_shape(left))
              and _exact_frozen(record.field_shape, freeze_field_shape(right)), "rng-field-shape-drift")
        _need(_uint(record.candidate_destination) and _uint(record.target_destination)
              and record.candidate_destination == left['destination']
              and record.target_destination == right['destination'], "rng-field-destination-drift")
        items = evidence.get(record.symbol, ())
        _need(len(items) == 1, "rng-base-evidence-ambiguous")
        item = items[0]
        _need(_symbol_status(specs[record.key], item, 1) is None
              and item.storage_kind == 'diagnostic_anchor' and item.alias_verified
              and record.base_pair.source_symbol == item.symbol
              and record.base_pair.anchor_symbol == item.alias_symbol
              and record.base_pair.address == item.address
              and record.base_pair.all_names == item.same_address_symbols,
              "rng-base-provenance-drift")
        _need(item.address <= 0xFFFFFFFF - record.addend
              and left['destination'] == item.address + record.addend
              and record.destination_pair.address == left['destination']
              and tuple(left['destination_symbols']) == record.destination_pair.all_names,
              "rng-member-pair-drift")
        if record.addend == 0:
            _need(record.destination_pair == record.base_pair, "rng-base-pair-drift")
        else:
            _need(record.destination_pair.source_symbol != item.symbol
                  and record.destination_pair.anchor_symbol != item.alias_symbol
                  and not record.destination_pair.source_symbol.startswith('__real@'),
                  "rng-other-pair-required")
        result[record.offset] = record
    return result


def rng_object_form(field, addend):
    """Exact operand predicate only; grants no identity or exception authority."""
    if type(addend) is not int or addend not in (0, 4):
        return False
    if (field['type'], field['width'], field['operand_kind'], field['field_encoding'], field['operand_width'],
        field['instruction_id'], field['instruction_mnemonic'], field['constant_read_eligible'],
        field['constant_value_type']) != ('DIR32', 4, 'absolute_memory', 'displacement', 4,
                                         RNG_MOV_ID, 'mov', False, None):
        return False
    shape = tuple(tuple(part) for part in field['operand_structure'])
    if len(shape) != 2:
        return False
    memory = (3, 4, 0, 0, 0, 1)
    def gpr(part):
        return len(part) == 3 and part[0] == 1 and part[1] == 4 and part[2] in RNG_GPR32
    load = gpr(shape[0]) and shape[1] == memory
    store = shape[0] == memory and gpr(shape[1])
    return load or (addend == 4 and store)


def _rng_policy_allows(record, spec, field, target, addend, prior_status):
    return (record is not None and spec.key == 'rng_global' and addend == record.addend
            and target['destination'] == REGISTRY['rng_global'].target_address + addend
            and rng_object_form(field, addend)
            and prior_status == ('unsupported_operand' if addend == 0 else 'candidate_addend_collision'))


def _field_gate(rows: Sequence[dict]) -> tuple[dict, ...]:
    _need(type(rows) in (list, tuple) and len(rows) == FIELD_COUNT, "invalid-field-count")
    ordered = []
    occupied = set()
    previous = -1
    for row in rows:
        _need(type(row) is dict and all(k in row for k in (*STRUCTURE_KEYS, "destination")), "invalid-field-schema")
        off, width = row["offset"], row["width"]
        _need(_uint(off, OWNER_SIZE - 4) and type(width) is int and width == 4, "invalid-field-extent")
        _need(off > previous, "unordered-or-duplicate-fields")
        previous = off
        extent = set(range(off, off + width))
        _need(not occupied & extent, "overlapping-fields")
        occupied.update(extent)
        kind = row["type"]
        _need(kind in ("DIR32", "REL32"), "invalid-field-type")
        start, size = row["instruction_offset"], row["instruction_size"]
        _need(_uint(start, OWNER_SIZE - 1) and type(size) is int and 1 <= size <= 15
              and start < off and off + width <= start + size <= OWNER_SIZE, "invalid-operand-alignment")
        _need(_uint(row["instruction_id"]) and row["instruction_id"] > 0, "invalid-instruction-metadata")
        _need(type(row["instruction_mnemonic"]) is str and re.fullmatch(r"[a-z0-9_]{1,16}", row["instruction_mnemonic"]),
              "invalid-instruction-metadata")
        shape = row["operand_structure"]
        _need(type(shape) in (tuple, list) and len(shape) <= 8, "invalid-operand-metadata")
        _need(all(type(part) in (tuple, list) and 2 <= len(part) <= 6
                  and all(_uint(x) for x in part) for part in shape), "invalid-operand-metadata")
        _need(row["field_encoding"] in ("immediate", "displacement"), "invalid-operand-metadata")
        _need(row["operand_kind"] in ("absolute_memory", "absolute_immediate", "relative_control", "other"),
              "invalid-operand-metadata")
        _need(_uint(row["operand_width"], 16) and type(row["constant_read_eligible"]) is bool,
              "invalid-operand-metadata")
        _need(row["constant_value_type"] in (None, "binary32", "binary64"), "invalid-operand-metadata")
        _need(_uint(row["destination"]), "invalid-decoded-destination")
        if "candidate_target" in row:
            _need(type(row["candidate_target"]) is int and row["candidate_target"] == row["destination"],
                  "decoded-destination-disagreement")
        if kind == "REL32":
            _need(type(row["instruction_end"]) is int and row["instruction_end"] == start + size
                  and row["operand_kind"] == "relative_control" and row["field_encoding"] == "immediate",
                  "invalid-relative-operand")
        else:
            _need(row["instruction_end"] is None and row["operand_kind"] != "relative_control", "invalid-absolute-operand")
        if row["operand_kind"] == "absolute_memory":
            _need(row["field_encoding"] == "displacement", "invalid-memory-operand")
        if row["operand_kind"] == "absolute_immediate":
            _need(row["field_encoding"] == "immediate", "invalid-immediate-operand")
        for prior in ordered:
            pstart, psize = prior["instruction_offset"], prior["instruction_size"]
            if start < pstart + psize and pstart < start + size:
                same_instruction = (start, size, row["instruction_id"], row["instruction_mnemonic"], row["operand_structure"])
                prior_instruction = (pstart, psize, prior["instruction_id"], prior["instruction_mnemonic"], prior["operand_structure"])
                _need(same_instruction == prior_instruction and row["field_encoding"] != prior["field_encoding"],
                      "inconsistent-instruction-boundaries")
        names = row.get("candidate_symbols", ())
        _need(type(names) in (list, tuple) and len(names) <= 128
              and all(type(name) is str and len(name) <= 512 for name in names), "invalid-destination-symbols")
        normalized = {k: row[k] for k in (*STRUCTURE_KEYS, "destination")}
        normalized["destination_symbols"] = tuple(names)
        ordered.append(normalized)
    _need(len(occupied) == FIELD_BYTES, "invalid-field-byte-count")
    return tuple(ordered)


def _spec_gate(specs: Sequence[BindingSpec]) -> dict[str, BindingSpec]:
    _need(type(specs) in (tuple, list) and 0 < len(specs) <= len(REGISTRY), "invalid-binding-spec-count")
    result = {}
    for spec in specs:
        _need(type(spec) is BindingSpec and type(spec.key) is str and spec.key in REGISTRY and spec.key not in result,
              "unapproved-binding-key")
        known = REGISTRY[spec.key]
        _need(spec.symbol == known.symbol and spec.role == known.role and type(spec.addends) is tuple
              and spec.addends == known.addends and all(type(x) is int for x in spec.addends), "binding-spec-identity-drift")
        _need(spec.target_address == known.target_address and
              (spec.target_address is None or type(spec.target_address) is int), "binding-target-authority-drift")
        _need(type(spec.provenance_sha256) is str and re.fullmatch(r"[0-9a-f]{64}", spec.provenance_sha256),
              "missing-binding-provenance")
        _need(type(spec.allow_typed_constant) is bool and (spec.role == "constant" or not spec.allow_typed_constant),
              "invalid-typed-instance-policy")
        result[spec.key] = spec
    return result


def _evidence_gate(evidence: Sequence[SymbolEvidence]) -> dict[str, tuple[SymbolEvidence, ...]]:
    _need(type(evidence) in (tuple, list) and len(evidence) <= 512, "invalid-symbol-evidence-count")
    result = {}
    for item in evidence:
        _need(type(item) is SymbolEvidence and type(item.symbol) is str and len(item.symbol) <= 512
              and _uint(item.address), "invalid-symbol-evidence")
        _need(item.storage_kind in ("code", "data", "diagnostic_anchor") and type(item.unique) is bool
              and type(item.provenance_verified) is bool and type(item.alias_verified) is bool, "invalid-symbol-evidence")
        _need(item.alias_symbol is None or (type(item.alias_symbol) is str and 0 < len(item.alias_symbol) <= 512),
              "invalid-symbol-evidence")
        _need(type(item.same_address_symbols) is tuple and len(item.same_address_symbols) <= 64
              and all(type(x) is str and len(x) <= 512 for x in item.same_address_symbols), "invalid-symbol-evidence")
        result.setdefault(item.symbol, []).append(item)
    return {key: tuple(value) for key, value in result.items()}


def _symbol_status(spec: BindingSpec, item: SymbolEvidence, count: int) -> str | None:
    if count != 1 or not item.unique:
        return "candidate_identity_ambiguous"
    if not item.provenance_verified:
        return "candidate_identity_unverified"
    if item.alias_symbol is not None or item.storage_kind == "diagnostic_anchor":
        if spec.role == "constant":
            return "constant_alias_ambiguous"
        if not item.alias_symbol or item.alias_symbol == spec.symbol or not item.alias_verified or item.storage_kind != "diagnostic_anchor":
            return "candidate_alias_unverified"
    elif item.storage_kind != ("code" if spec.role == "function" else "data"):
        return "candidate_storage_kind_mismatch"
    if spec.role == "constant" and item.same_address_symbols != (spec.symbol,):
        return "constant_alias_ambiguous"
    expected_names = tuple(sorted((item.alias_symbol, spec.symbol))) if item.alias_symbol else (spec.symbol,)
    if spec.role != "constant" and item.same_address_symbols != expected_names:
        return "candidate_identity_ambiguous"
    return None


def _operand_status(spec: BindingSpec, field: dict, addend: int) -> str | None:
    kind = field["operand_kind"]
    if spec.role != "constant" and (field["constant_read_eligible"] or field["constant_value_type"] is not None):
        return "unsupported_operand"
    if spec.role == "function":
        return None if ((field["type"] == "REL32" and kind == "relative_control") or
                        (field["type"] == "DIR32" and kind == "absolute_immediate")) else "unsupported_operand"
    if spec.role == "global":
        if field["type"] != "DIR32":
            return "unsupported_operand"
        if kind == "absolute_immediate" and addend == 0:
            return None
        if kind == "absolute_memory" and field["operand_width"] == (2 if addend == 0 else 4):
            return None
        return "unsupported_operand"
    known = REGISTRY[spec.key]
    width = 4 if known.constant_type == "binary32" else 8
    shape = field["operand_structure"]
    absolute_scalar = (len(shape) == 1 and tuple(shape[0]) == (3, width, 0, 0, 0, 1))
    return None if (field["type"] == "DIR32" and kind == "absolute_memory"
                    and field["constant_read_eligible"] is True and field["operand_width"] == width
                    and field["constant_value_type"] == known.constant_type
                    and field["instruction_mnemonic"] in X87_READS and absolute_scalar) else "constant_type_mismatch"


class _ReadBudget:
    def __init__(self, callback: Callable[[str, int, int], bytes]):
        self.callback = callback
        self.used = {"candidate": 0, "target": 0}
        self.calls = {"candidate": 0, "target": 0}
        self.cache = {}

    def read(self, side: str, address: int, width: int) -> tuple[bytes | None, str | None]:
        _need(side in self.used and _uint(address) and type(width) is int and width in (4, 8)
              and address + width <= 0x100000000, "invalid-constant-read-request")
        key = (side, address, width)
        if key in self.cache:
            return self.cache[key]
        if self.calls[side] >= MAX_DATA_READS or self.used[side] + width > MAX_DATA_BYTES:
            return None, "constant_budget_exceeded"
        self.calls[side] += 1
        self.used[side] += width
        try:
            value = self.callback(side, address, width)
            if type(value) is not bytes or len(value) != width:
                result = (None, "constant_read_rejected")
            else:
                result = (value, None)
        except Exception:
            # Checked reader errors never become exported messages; no retries.
            result = (None, "constant_read_rejected")
        self.cache[key] = result
        return result


def bind_fields(candidate_fields: Sequence[dict], target_fields: Sequence[dict],
                binding_specs: Sequence[BindingSpec], symbol_evidence: Sequence[SymbolEvidence], *,
                read_constant: Callable[[str, int, int], bytes],
                rng_object_evidence: Sequence[RngObjectFieldEvidence] = ()) -> BindingResult:
    """Return aggregate census plus explicitly private annotations.

    Structural corruption raises a fixed-code BindingError. Unresolved/unknown
    identities remain counted rows and prevent binding_complete. Runtime users
    must export only .aggregate, never this result's private_rows or dataclass
    conversion. This is binding evidence, not canonical replay or adoption.
    """
    candidate = _field_gate(candidate_fields)
    target = _field_gate(target_fields)
    _need(all(all(a[k] == b[k] for k in STRUCTURE_KEYS) for a, b in zip(candidate, target)),
          "candidate-target-field-structure-drift")
    specs = _spec_gate(binding_specs)
    evidence = _evidence_gate(symbol_evidence)
    rng_records = _rng_evidence_gate(rng_object_evidence, candidate, target, specs, evidence)
    rng_seen = set()
    rng_applied = []
    _need(callable(read_constant), "missing-checked-scalar-reader")
    rows = []
    constant_plans = []
    for index, (left, right) in enumerate(zip(candidate, target)):
        row = {"offset": left["offset"], "width": left["width"], "type": left["type"],
               "instruction_end": left["instruction_end"], "candidate_target": left["destination"],
               "target_target": right["destination"], "key": None, "status": "candidate_identity_unknown",
               "symbol": None, "addend": None, "target_address": None, "diagnostic_placeholder": False}
        possible = []
        for spec in specs.values():
            for item in evidence.get(spec.symbol, ()):
                for addend in spec.addends:
                    if item.address + addend <= 0xFFFFFFFF and left["destination"] == item.address + addend:
                        possible.append((spec, item, addend))
        if not possible:
            rows.append(row)
            continue
        if len({(spec.symbol, item.address, addend) for spec, item, addend in possible}) != 1:
            row["status"] = "candidate_identity_ambiguous"
            rows.append(row)
            continue
        if len(evidence[possible[0][0].symbol]) != 1:
            row["status"] = "candidate_identity_ambiguous"
            rows.append(row)
            continue
        # Multiple entries for the same semantic symbol are permitted only for
        # the independently fixed direct-free and wrapper-free identity pair.
        if len({spec.key for spec, _, _ in possible}) > 1:
            if {spec.key for spec, _, _ in possible} != {"direct_free", "wrapper_free"}:
                row["status"] = "candidate_identity_ambiguous"
                rows.append(row)
                continue
            selected = [(spec, item, addend) for spec, item, addend in possible
                        if right["destination"] == spec.target_address + addend]
            if len(selected) != 1:
                row["status"] = "target_identity_mismatch"
                rows.append(row)
                continue
            possible = selected
        spec, item, addend = possible[0]
        row.update(key=spec.key, symbol=spec.symbol, addend=addend, target_address=spec.target_address,
                   candidate_symbol_address=item.address, provenance_sha256=spec.provenance_sha256,
                   diagnostic_placeholder=item.storage_kind == "diagnostic_anchor")
        failure = _symbol_status(spec, item, len(evidence[spec.symbol])) or _operand_status(spec, left, addend)
        if not failure and addend != 0 and left["destination_symbols"]:
            failure = "candidate_addend_collision"
        record = rng_records.get(left['offset'])
        if record is not None:
            _need(spec.key == 'rng_global' and record.addend == addend, 'unused-rng-field-record')
            rng_seen.add(left['offset'])
        if _rng_policy_allows(record, spec, left, right, addend, failure):
            proof = _digest(asdict(record))
            row.update(status=RNG_BOUND_STATUS, rng_object_policy=RNG_OBJECT_POLICY,
                       rng_prior_status=failure, rng_collision_observed=addend == 4,
                       rng_source_span=(addend, 4), rng_field_proof_sha256=proof)
            rng_applied.append(row)
        elif failure:
            row["status"] = failure
        elif spec.role != "constant":
            row["status"] = ("bound_known_identity" if right["destination"] == spec.target_address + addend
                             else "target_identity_mismatch")
        else:
            known = REGISTRY[spec.key]
            expected_target = spec.target_address
            if expected_target is None and not spec.allow_typed_constant:
                row["status"] = "missing_target_identity"
            elif expected_target is not None and right["destination"] != expected_target and not spec.allow_typed_constant:
                row["status"] = "target_identity_mismatch"
            else:
                scalar_width = 4 if known.constant_type == "binary32" else 8
                if left["destination"] + scalar_width > 0x100000000 or right["destination"] + scalar_width > 0x100000000:
                    row["status"] = "constant_read_rejected"
                else:
                    row.update(status="bound_known_identity" if expected_target == right["destination"] else "bound_typed_instance",
                               target_address=right["destination"], constant_type=known.constant_type,
                               scalar_width=scalar_width)
                    constant_plans.append((index, known, left["destination"], right["destination"], scalar_width))
        rows.append(row)

    _need(rng_seen == set(rng_records), 'unused-rng-field-record')

    # Discover overlapping or mixed constant interpretations across the entire
    # plan before any reads. Both sides of a conflict remain unresolved.
    conflicts = set()
    for position, (index, known, ca, ta, width) in enumerate(constant_plans):
        for other_index, other, cb, tb, other_width in constant_plans[:position]:
            for a, b in ((ca, cb), (ta, tb)):
                if a < b + other_width and b < a + width:
                    if (a, width, known.constant_type, known.constant_bits) != (b, other_width, other.constant_type, other.constant_bits):
                        conflicts.update((index, other_index))
    budget = _ReadBudget(read_constant)
    for index, known, candidate_address, target_address, width in constant_plans:
        if index in conflicts:
            rows[index]["status"] = "constant_storage_ambiguous"
            continue
        expected = int(known.constant_bits, 16).to_bytes(width, "little")
        for side, address in (("candidate", candidate_address), ("target", target_address)):
            value, failure = budget.read(side, address, width)
            if failure:
                rows[index]["status"] = failure
                break
            if value != expected:
                rows[index]["status"] = "constant_content_mismatch"
                break

    status_codes = STATUS_CODES + ((RNG_BOUND_STATUS,) if rng_records else ())
    statuses = {status: sum(row["status"] == status for row in rows) for status in status_codes}
    per_key = {}
    for key in EXPORT_KEYS:
        selected = [row for row in rows if row["key"] == key]
        bound = [row for row in selected if row["status"] in BOUND_STATUSES]
        instances = {row["target_address"] for row in bound if "constant_type" in row}
        per_key[key] = {"observed": len(selected), "bound": len(bound), "unresolved": len(selected) - len(bound),
                        "diagnostic_placeholders": sum(row["diagnostic_placeholder"] for row in bound),
                        "duplicate_constant_instances": max(0, len(instances) - 1)}
    bound_count = sum(row["status"] in BOUND_STATUSES for row in rows)
    spec_receipt = [{"key": spec.key, "symbol": spec.symbol, "role": spec.role, "addends": spec.addends,
                     "target_address": spec.target_address, "provenance_sha256": spec.provenance_sha256,
                     "allow_typed_constant": spec.allow_typed_constant} for spec in specs.values()]
    aggregate = {
        "schema_version": 1, "owner_size": OWNER_SIZE, "field_count": FIELD_COUNT, "field_bytes": FIELD_BYTES,
        "field_structure_validated": True, "bound_fields": bound_count, "unresolved_fields": FIELD_COUNT - bound_count,
        "binding_complete": bound_count == FIELD_COUNT, "normalization_complete": False, "acceptance_authority": "none",
        "status_counts": statuses, "known_bindings": per_key,
        "candidate_data_bytes": budget.used["candidate"], "target_data_bytes": budget.used["target"],
        "candidate_data_reads": budget.calls["candidate"], "target_data_reads": budget.calls["target"],
        "binding_spec_sha256": _digest(sorted(spec_receipt, key=lambda x: x["key"])),
        "private_binding_inventory_sha256": _digest(rows),
        "field_structure_sha256": _digest([{k: row[k] for k in STRUCTURE_KEYS} for row in candidate]),
    }
    if rng_records:
        aggregate['rng_source_object_policy'] = {
            'policy': RNG_OBJECT_POLICY, 'scope': 'compile-address-only',
            'evidence_fields': len(rng_records), 'bound_fields': len(rng_applied),
            'base_dword_loads': sum(row['addend'] == 0 for row in rng_applied),
            'generation_collision_bindings': sum(row['addend'] == 4 for row in rng_applied),
            'proof_sha256': _digest(sorted(row['rng_field_proof_sha256'] for row in rng_applied)),
            'physical_eight_byte_anchor_proved': False, 'original_relocation_symbol_proved': False,
            'semantic_equivalence_proved': False,
        }
    _need(sum(statuses.values()) == FIELD_COUNT and sum(v["observed"] for v in per_key.values()) <= FIELD_COUNT,
          "invalid-aggregate-partition")
    return BindingResult(aggregate, tuple(rows))
