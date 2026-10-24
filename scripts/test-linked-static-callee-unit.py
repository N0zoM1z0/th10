"""Synthetic same-artifact static-callee proofs; no target/native compiler."""
from pathlib import Path
import sys, importlib.util, unittest, struct, io, tomllib
from copy import deepcopy
from unittest.mock import patch
ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'scripts'))
def module(name):
    path = ROOT / 'scripts' / name
    spec=importlib.util.spec_from_file_location(name.replace('-','_'),path)
    m=importlib.util.module_from_spec(spec); spec.loader.exec_module(m)
    if name in ("build-match-unit.py","compare-linked-function.py"): m.ROOT=ROOT
    return m
context=module('test-linked-context.py')
context.module=module
from linked_image import PEImage, static_callee_unit_reference, validate_static_callee_units
class ReferencedCallee(context.StaticCalls):
    # Keep the existing legacy controls in their original suite, not inherited here.
    def setUp(self):
        super().setUp()
        (self.root/'config/functions.csv').write_text(
            'address,size,source_file,proposed_name\n0x00401020,8,src/Private.cpp,LocalOwner\n')
        self.field['static_callee_unit']='callee'
        self.callee=deepcopy(self.unit)
        self.callee.update(owner_visibility='static',pdb_source='src/Private.cpp',
                           symbol=self.field['symbol'],target_address=0x401020,
                           functions=['LocalOwner'],size=8,
                           linkages=[dict(offset=1,type='DIR32',symbol='_Public',target=0x501000)])
        data=bytearray(self.image.read_bytes())
        data[0x220:0x228]=b'\xa1'+struct.pack('<I',0x401000)+b'\xc3\x90\x90'
        struct.pack_into('<II',data,0x98+96+5*8,0x1380,12)
        struct.pack_into('<IIHH',data,0x580,0x1000,12,0x3021,0)
        self.image.write_bytes(data)
        self.target=self.root/'target.exe'
        struct.pack_into('<I',data,0x221,0x501000)
        self.target.write_bytes(data)
        self.units={'caller':self.unit,'callee':self.callee}
    def compare(self):
        manifest=dict(target_sha256='fixture',units=self.units)
        controls={self.c.UNITS_MANIFEST:manifest,
                  self.c.TARGET_MANIFEST:{'target':{'sha256':'fixture'}},
                  self.c.TOOLS_LOCK:tomllib.loads((ROOT/'config/tools.lock.toml').read_text())}
        original=Path.open
        def opened(path,*a,**kw):
            if path in controls:
                stream=io.BytesIO();stream.fixture=controls[path]; return stream
            return original(path,*a,**kw)
        with patch.object(self.c,'ROOT',self.root),patch.object(Path,'open',opened),\
             patch.object(self.c.tomllib,'load',side_effect=lambda stream:stream.fixture),\
             patch.object(self.c,'verified_target',side_effect=lambda:PEImage(self.target)):
            return self.c.compare_unit('caller')
    def test_reference_exact_nonzero_fields(self):
        r=self.compare()
        self.assertEqual(r['result'],'exact')
        p=r['linkages'][0]['static_owner']['callee_unit_proof']
        self.assertEqual(p['result'],'exact');self.assertFalse(p['raw_equal'])
        self.assertEqual(len(p['linkages']),1)
        self.assertEqual(r['pdb_module_index'],0)
        self.assertEqual(p['pdb_module_index'],1)
    def test_ordinary_callee_byte_mismatch(self):
        d=bytearray(self.image.read_bytes());d[0x225]=0x90;self.image.write_bytes(d)
        with self.assertRaisesRegex(ValueError,'exact whole'):self.compare()
    def test_missing_or_wrong_callee_field(self):
        self.callee['linkages']=[]
        with self.assertRaisesRegex(ValueError,'field|linkage'):self.compare()
    def test_wrong_callee_binding(self):
        self.callee['linkages'][0]['symbol']='_Missing'
        with self.assertRaisesRegex(ValueError,'resolve uniquely'):self.compare()
    def test_callee_wrong_target_binding(self):
        self.callee['linkages'][0]['target']+=4
        with self.assertRaisesRegex(ValueError,'exact whole'):self.compare()
    def test_interior_direct_call_destination(self):
        d=bytearray(self.image.read_bytes());struct.pack_into('<i',d,0x201,0x21-5);self.image.write_bytes(d)
        with self.assertRaisesRegex(ValueError,'enter its qualified'):self.compare()
    def test_reference_graph_and_owner_rejections(self):
        changes=[('output_dir','build/other'),('source','src/Other.cpp'),
                 ('support_sources',[]),('profile',['/GL']),('support_profiles',{'x':['/GL']}),
                 ('link_profile',['/other']),('entry_symbol','other'),('harness','other'),
                 ('owner_visibility','public'),('pdb_source','src/Public.cpp'),
                 ('symbol','_Other'),('size',7),('compare_size',9),('target_address',0x401021)]
        for key,value in changes:
            old=deepcopy(self.callee)
            self.callee[key]=value
            with self.subTest(key=key),self.assertRaises(ValueError):self.compare()
            self.callee.clear();self.callee.update(old)
    def test_unknown_reference(self):
        self.field['static_callee_unit']='missing'
        with self.assertRaisesRegex(ValueError,'unknown'):self.compare()
    def test_reference_requires_direct_call_metadata(self):
        for key,value in [('static_callee_unit',''),('static_tail_jump',True),
                          ('type','DIR32'),('addend',1),('local',True)]:
            old=dict(self.field);self.field[key]=value
            with self.subTest(key=key),self.assertRaises(ValueError):self.compare()
            self.field.clear();self.field.update(old)
    def test_cycle_rejected_before_replay(self):
        f=dict(self.field,static_callee_unit='callee')
        self.callee['linkages']=[f]
        with self.assertRaisesRegex(ValueError,'cycle'):self.compare()
    def test_missing_reference_metadata(self):
        del self.field['static_source']
        with self.assertRaisesRegex(ValueError,'source and whole'):self.compare()
    def test_corrupt_map_identity(self):
        path=self.out/'source.map';path.write_text(path.read_text().replace('12345678','12345679'))
        with self.assertRaises(ValueError):self.compare()

    def test_local_branch_byte_mismatch(self):
        target=bytearray(self.target.read_bytes())
        target[0x225:0x228]=b'\xeb\x00\xc3'
        self.target.write_bytes(target)
        data=bytearray(self.image.read_bytes())
        data[0x225:0x228]=b'\xeb\xff\xc3'
        self.image.write_bytes(data)
        with self.assertRaises(ValueError):self.compare()
    def test_pdb_identity_mismatch(self):
        p=self.out/'source.pdb';data=bytearray(p.read_bytes())
        needle=bytes(range(16));offset=data.find(needle)
        self.assertGreaterEqual(offset,0);data[offset]^=1;p.write_bytes(data)
        with self.assertRaises(ValueError):self.compare()
    def test_static_owner_alias_rejected(self):
        p=self.out/'source.map';p.write_text(p.read_text()+
            ' 0001:00000020 ?Alias@@YAXXZ 00401020 f support-00-Private.ltcg.obj\n')
        with self.assertRaises(ValueError):self.compare()
    def test_source_mapping_mismatch(self):
        p=self.root/'config/functions.csv';p.write_text(p.read_text().replace('LocalOwner','OtherOwner'))
        with self.assertRaisesRegex(ValueError,'source mapping'):self.compare()
    def test_mutated_artifact_during_recursion(self):
        original=self.c.compare_unit
        def compare(name,**kw):
            result=original(name,**kw)
            if name=='callee':
                p=self.out/'source.map';p.write_text(p.read_text()+'\n')
            return result
        with patch.object(self.c,'compare_unit',side_effect=compare):
            with self.assertRaisesRegex(ValueError,'artifact changed'):self.compare()

if __name__=='__main__':
    # Do not inherit legacy methods with assumptions deliberately different in
    # the nonzero-field fixture. Run them separately against the maintained modules.
    selected=[name for name in ReferencedCallee.__dict__ if name.startswith('test_')]
    suite=unittest.TestSuite(ReferencedCallee(name) for name in selected)
    suite.addTests(unittest.defaultTestLoader.loadTestsFromModule(context))
    for name in ['test-linked-local-fields.py','test-linked-data-coverage.py',
                 'test-static-owner-selection.py']:
        m=module(name)
        if hasattr(m,'module'):m.module=module
        suite.addTests(unittest.defaultTestLoader.loadTestsFromModule(m))
    result=unittest.TextTestRunner(verbosity=2).run(suite)
    sys.exit(not result.wasSuccessful())
