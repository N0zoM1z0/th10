"""Pure scoped operand metadata and bounded immutable scalar reads; never prints."""
READ=0x40000000
WRITE=0x80000000
EXEC=0x20000000
INITIALIZED=0x40
UNINITIALIZED=0x80
X87_READS=frozenset(('fld','fadd','fmul','fsub','fsubr','fdiv','fdivr','fcom','fcomp'))

def need(value):
    if not value:raise ValueError('typed field gate')

def enrich_field_operands(code803,origin,fields,decoder=None):
    need(type(code803) is bytes and len(code803)==803)
    need(type(origin) is int and 0<=origin<=0xffffffff-803)
    if decoder is None:
        import capstone as cs
        decoder=cs.Cs(cs.CS_ARCH_X86,cs.CS_MODE_32);decoder.detail=True
    # Capstone X86_OP_REG/IMM/MEM are stable pinned constants 1/2/3.
    records=[];cursor=0
    for ins in decoder.disasm(code803,origin):
        need(ins.address==origin+cursor and type(ins.size) is int and 1<=ins.size<=15 and cursor+ins.size<=803)
        operands=[]
        for op in ins.operands:
            need(op.type in (1,2,3) and type(op.size) is int and 0<=op.size<=16)
            if op.type==1:shape=(1,op.size,op.reg)
            elif op.type==2:shape=(2,op.size)
            else:shape=(3,op.size,op.mem.segment,op.mem.base,op.mem.index,op.mem.scale)
            operands.append(shape)
        enc=[]
        for label,off,width in (('immediate',ins.imm_offset,ins.imm_size),('displacement',ins.disp_offset,ins.disp_size)):
            need(type(off) is int and type(width) is int and 0<=width<=ins.size)
            if width:need(0<off and off+width<=ins.size);enc.append((label,cursor+off,width))
        records.append((cursor,ins,tuple(operands),enc));cursor+=ins.size
    need(cursor==803)
    result=[];occupied=set()
    for field in fields:
        off,width=field['offset'],field['width']
        need(type(off) is int and type(width) is int and 0<=off<803 and 0<width<=803-off)
        extent=set(range(off,off+width));need(not occupied&extent);occupied.update(extent)
        matches=[(start,ins,shape,label) for start,ins,shape,enc in records for label,pos,n in enc if (pos,n)==(off,width)]
        need(len(matches)==1);start,ins,shape,label=matches[0]
        mem=[op for op in ins.operands if op.type==3]
        absolute=label=='displacement' and len(mem)==1 and mem[0].mem.base==0 and mem[0].mem.index==0 and mem[0].mem.segment==0
        scalar_width=mem[0].size if absolute else 0
        eligible=(field['type']=='DIR32' and width==4 and absolute and scalar_width in (4,8) and ins.mnemonic in X87_READS and len(ins.operands)==1 and not (getattr(mem[0],'access',0)&2))
        kind=field['type'];need(kind in ('DIR32','REL8','REL16','REL32'))
        need(width=={'DIR32':4,'REL8':1,'REL16':2,'REL32':4}[kind])
        if kind=='DIR32':
            destination=int.from_bytes(code803[off:off+width],'little')
            operand_kind='absolute_memory' if absolute else ('absolute_immediate' if label=='immediate' else 'other')
            need(field.get('instruction_end') is None)
        else:
            need(label=='immediate' and (ins.group(1) or ins.group(2)))
            need(bool(ins.operands) and ins.operands[0].type==2)
            destination=origin+start+ins.size+int.from_bytes(code803[off:off+width],'little',signed=True)
            need(0<=destination<=0xffffffff and field.get('instruction_end')==start+ins.size)
            operand_kind='relative_control'
        need(field['candidate_target']==destination)
        row=dict(field)
        row.update(instruction_offset=start,instruction_size=ins.size,instruction_id=ins.id,
          instruction_mnemonic=ins.mnemonic,operand_structure=shape,field_encoding=label,
          operand_kind=operand_kind,operand_width=scalar_width,
          destination=destination,constant_value_type=('binary32' if scalar_width==4 else 'binary64') if eligible else None,
          constant_read_eligible=bool(eligible))
        result.append(row)
    return result

def require_constant_field(field,scalar_width):
    need(type(scalar_width) is int and scalar_width in (4,8))
    need(field.get('constant_read_eligible') is True and field['operand_width']==scalar_width)
    return scalar_width

def require_same_structure(candidate,target):
    keys=('offset','width','type','instruction_offset','instruction_size','instruction_id',
      'instruction_mnemonic','operand_structure','field_encoding','operand_kind','operand_width')
    need(len(candidate)==len(target))
    left=sorted(candidate,key=lambda x:x['offset']);right=sorted(target,key=lambda x:x['offset'])
    for a,b in zip(left,right):need(all(a[k]==b[k] for k in keys))
    return True

class BudgetedScalarReader:
    def __init__(self,image):
        self.image=image;self.used=0;self.calls=0;self.cache={};self.intervals=[]
        self.relocations=tuple(image.base_relocations())
    def read(self,address,width):
        need(type(address) is int and type(width) is int and width in (4,8))
        need(0<=address<=0x100000000-width)
        need(self.calls<43 and self.used+width<=344)
        self.calls+=1;self.used+=width
        for lo,hi in self.intervals:
            if address<hi and lo<address+width:need((lo,hi)==(address,address+width))
        key=(address,width)
        if key in self.cache:return self.cache[key]
        image=self.image;need(type(image.data) is bytes)
        rva=address-image.image_base;end=rva+width;need(rva>=0)
        matches=[]
        for s in image.sections:
            need(all(type(x) is int and x>=0 for x in (s.rva,s.virtual_size,s.raw_size,s.raw_offset,s.characteristics)))
            if s.rva<end and rva<s.rva+max(s.virtual_size,s.raw_size):matches.append(s)
        need(len(matches)==1);s=matches[0]
        need(s.characteristics&(READ|INITIALIZED)==READ|INITIALIZED)
        need(not s.characteristics&(WRITE|EXEC|UNINITIALIZED|0x20))
        delta=rva-s.rva;need(0<=delta and delta+width<=s.virtual_size and delta+width<=s.raw_size)
        offset=s.raw_offset+delta;need(0<=offset and offset+width<=len(image.data))
        # A second section aliasing the same raw storage is ambiguous, too.
        need(sum(x.raw_offset<offset+width and offset<x.raw_offset+x.raw_size for x in image.sections)==1)
        need(not any(a<address+width and address<a+4 for a in self.relocations))
        # Exclude directory-owned records, including loader-mutated metadata.
        for index,(start,size) in enumerate(getattr(image,'directories',())):
            if not size:continue
            if index==4:need(not (start<offset+width and offset<start+size))
            else:need(not (start<end and rva<start+size))
        value=image.data[offset:offset+width];need(len(value)==width)
        self.intervals.append((address,address+width));self.cache[key]=value
        return value

ScalarReader=BudgetedScalarReader

def target_fields(code803,origin,image,decoder=None):
    """Independent supported target-operand census; no relocation-table claim.

    An address-valued immediate is only a proposal for later semantic whitelist
    binding. Numerical image membership alone never validates its identity.
    """
    need(type(code803) is bytes and len(code803)==803)
    need(type(origin) is int and 0<=origin<=0xffffffff-803)
    if decoder is None:
        import capstone as cs
        decoder=cs.Cs(cs.CS_ARCH_X86,cs.CS_MODE_32);decoder.detail=True
    def image_member(address):
        need(type(address) is int and 0<=address<=0xffffffff)
        matches=0
        for section in image.sections:
            start=image.image_base+section.rva;size=section.virtual_size
            need(type(start) is int and type(size) is int and 0<=start<=0xffffffff and 0<=size<=0x100000000-start)
            matches+=bool(start<=address<start+size)
        need(matches<=1)
        return matches==1
    result=[];occupied=set();cursor=0
    def add(offset,width,kind,destination,end):
        need(type(offset) is int and type(width) is int and 0<=offset<803 and 0<width<=803-offset)
        extent=set(range(offset,offset+width));need(not occupied&extent);occupied.update(extent)
        result.append(dict(offset=offset,width=width,type=kind,candidate_target=destination,instruction_end=end))
    for ins in decoder.disasm(code803,origin):
        need(ins.address==origin+cursor and type(ins.size) is int and 1<=ins.size<=15 and cursor+ins.size<=803)
        for offset,width in ((ins.disp_offset,ins.disp_size),(ins.imm_offset,ins.imm_size)):
            need(type(offset) is int and type(width) is int and 0<=width<=ins.size)
            if width:need(0<offset and offset+width<=ins.size)
        control=bool(ins.group(1) or ins.group(2))
        direct=control and bool(ins.operands) and ins.operands[0].type==2
        if direct:
            need(ins.imm_size in (1,2,4) and len([op for op in ins.operands if op.type==2])==1)
            off=cursor+ins.imm_offset;width=ins.imm_size
            destination=origin+cursor+ins.size+int.from_bytes(code803[off:off+width],'little',signed=True)
            need(0<=destination<=0xffffffff and (int(ins.operands[0].imm)&0xffffffff)==destination)
            if not origin<=destination<origin+803:
                add(off,width,'REL%d'%(width*8),destination,cursor+ins.size)
        mem=[op for op in ins.operands if op.type==3]
        if ins.disp_size:
            need(len(mem)==1)
            op=mem[0];off=cursor+ins.disp_offset
            raw=int.from_bytes(code803[off:off+ins.disp_size],'little')
            if op.mem.base==0 and op.mem.index==0:
                # Absolute non-flat or non-32-bit memory forms are unsupported.
                need(op.mem.segment==0 and ins.disp_size==4)
                need((int(op.mem.disp)&0xffffffff)==raw and image_member(raw))
                add(off,4,'DIR32',raw,None)
            elif ins.disp_size==4 and image_member(raw):
                # An image-like displacement plus a runtime register is ambiguous.
                need(False)
        if ins.imm_size and not direct:
            immediate=[op for op in ins.operands if op.type==2]
            need(len(immediate)==1)
            if ins.imm_size==4:
                off=cursor+ins.imm_offset;raw=int.from_bytes(code803[off:off+4],'little')
                need((int(immediate[0].imm)&0xffffffff)==raw)
                if image_member(raw):add(off,4,'DIR32',raw,None)
        cursor+=ins.size
    need(cursor==803)
    return sorted(result,key=lambda row:(row['offset'],row['type']))
