"""Authored synthetic x86 only: no original/generated target bytes or execution."""
import copy, hashlib, importlib.util, json, pathlib, struct, subprocess, sys, types, unittest
P=pathlib.Path
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
sp=importlib.util.spec_from_file_location('collector', P(__file__).with_name('replay_owner_structure.py'))
c=importlib.util.module_from_spec(sp);sp.loader.exec_module(c)
START=0x1000

class CFG(unittest.TestCase):
    def setUp(self):
        self.md=Cs(CS_ARCH_X86, CS_MODE_32);self.md.detail=True;self.metadata=[]
    def run_code(self, raw):
        code=bytes.fromhex(raw) if isinstance(raw,str) else raw
        def meta(at,size):
            self.metadata.append((at,size));return {'unique_mapped_virtual_section':True,'raw_span_covered':True,'readable_section':True,'executable_section':True,'disjoint_from_other_pdb_contributions':True}
        return c.preflight(self.md,code,START,meta)
    def bad(self,raw,category):
        with self.assertRaisesRegex(c.GateError,category):self.run_code(raw)
        self.assertEqual(self.metadata,[])
    def proposal_bad(self,raw,category):
        out=self.run_code(raw)
        self.assertFalse(out['full_owner_cfg_closed_under_model'])
        self.assertEqual(out['table_range_proposal']['status'],'UNRESOLVED')
        self.assertRegex(out['table_range_proposal']['category'],'^('+category+')$')
        self.assertEqual(self.metadata,[])
    def test_closed_all_code(self):
        out=self.run_code('31c0 85c0 7401 90 c3')
        self.assertEqual(out['status'],'DIRECT_CFG_CLOSED');self.assertEqual(out['reachable_instruction_byte_count'],8)
        self.assertTrue(out['all_owner_bytes_reachable_instructions']);self.assertEqual(self.metadata,[])
    def test_owner_not_cropped(self):
        out=self.run_code('c3 1122334455667788')
        self.assertEqual(out['owner_byte_count'],9);self.assertEqual(out['unclassified_owner_byte_count'],8)
        self.assertTrue(out['full_owner_cfg_closed_under_model']);self.assertFalse(out['all_owner_bytes_reachable_instructions'])
        self.assertTrue(out['unclassified_bytes_are_not_assumed_data_or_fill'])
    def test_backward_loop(self):
        out=self.run_code('90 ebfd');self.assertEqual(out['reachable_instruction_byte_count'],3);self.assertEqual(out['return_instruction_count'],0)
    def test_opaque_call(self):
        out=self.run_code('e800100000 c3');self.assertEqual(out['opaque_returning_call_count'],1)
    def test_indirect_opaque_call(self):
        out=self.run_code('ffd0 c3');self.assertEqual(out['opaque_returning_call_count'],1)
    def test_synthetic_msvc_ordinary(self):
        out=self.run_code('55 8bec 83ec04 66890f dfe0 9e f3ab f3a5 8be5 5d c3')
        self.assertTrue(out['all_owner_bytes_reachable_instructions'])
    def test_direct_table_metadata(self):
        out=self.run_code('83f803 7707 ff248500200000 c3')
        p=out['table_range_proposal'];self.assertEqual(p['required_span_bytes'],16);self.assertEqual(self.metadata,[(0x2000,16)])
        self.assertFalse(p['table_contents_inspected']);self.assertFalse(p['complete_cfg_guard_dominance_proved'])
    def test_remap_metadata(self):
        out=self.run_code('83f87f 770e 0fb68000300000 ff248500200000 c3')
        p=out['table_range_proposal'];self.assertEqual(p['required_span_bytes'],128);self.assertEqual(self.metadata,[(0x3000,128)])
        self.assertFalse(p['pointer_table_extent_known']);self.assertFalse(p['table_contents_sha256_exported'])
    def test_inline_range_remains_unclassified(self):
        out=self.run_code('83f801 7707 ff24850d100000 c3 1234567812345678')
        p=out['table_range_proposal'];self.assertEqual(p['required_span_bytes'],8)
        self.assertTrue(p['span_classification']['wholly_inside_complete_owner'])
        self.assertTrue(p['span_classification']['disjoint_from_known_reachable_instructions'])
        self.assertEqual(out['unclassified_owner_byte_count'],8)
    def test_range_overlap_report_only(self):
        out=self.run_code('83f800 7707 ff248500100000 c3')
        self.assertFalse(out['table_range_proposal']['span_classification']['disjoint_from_known_reachable_instructions'])
    def test_return_cleanup(self):
        self.assertTrue(self.run_code('c20400')['full_owner_cfg_closed_under_model'])
    def test_external_branch(self):self.bad('eb7f','direct_branch_outside_owner_or_width')
    def test_fallthrough(self):self.bad('90','fallthrough_outside_owner')
    def test_internal_call(self):self.bad('e800000000 c3','internal_call_or_target_width')
    def test_instruction_overlap(self):self.bad('7401 b800000000 c3','overlapping_decode')
    def test_truncated_instruction(self):self.bad('0f','reachable_decode_failure')
    def test_oversized_owner(self):self.bad(b'\xc3'*(c.OWNER_SIZE+1),'owner_decode_extent')
    def test_unknown_control(self):self.bad('c7f800000000 c3','unsupported_branch_shape')
    def test_loop_not_assumed_ordinary(self):self.bad('e2fe c3','unsupported_control_group|unsupported_branch_shape|unsupported_normal_instruction')
    def test_jecxz_not_assumed_jcc(self):self.bad('e300 c3','unsupported_branch_shape')
    def test_interrupt(self):self.bad('cd03 c3','unsupported_control_group')
    def test_iret(self):self.bad('cf','unsupported_control_group')
    def test_halt(self):self.bad('f4','unsupported_control_group')
    def test_sysenter(self):self.bad('0f34 c3','unsupported_control_group|unsupported_normal_instruction')
    def test_ud2(self):self.bad('0f0b c3','unsupported_normal_instruction')
    def test_far_return(self):self.bad('cb','return_shape|unsupported_control_group')
    def test_far_call(self):self.bad('9a002000000000 c3','call_shape')
    def test_excess_cleanup(self):self.bad('c22400','return_shape')
    def test_unaligned_cleanup(self):self.bad('c20200','return_shape')
    def test_prefixed_return(self):self.bad('f3c3','control_prefix')
    def test_narrow_branch(self):self.bad('66eb00 c3','control_prefix')
    def test_duplicate_prefix(self):self.bad('6666890f c3','unsupported_raw_prefix_sequence')
    def test_locked_ordinary(self):self.bad('f00100 c3','unsupported_raw_prefix_sequence')
    def test_segment_override(self):self.bad('648b00 c3','unsupported_raw_prefix_sequence')
    def test_address_override(self):self.bad('678b00 c3','unsupported_raw_prefix_sequence')
    def test_unapproved_repeat(self):self.bad('f390 c3','decoder_prefix_disagree|unsupported_repeat_prefix')
    def test_segment_write(self):self.bad('8ed0 c3','unsupported_normal_write_effect|unsupported_control_group')
    def test_two_indirect_dispatches(self):self.bad('7402 ffe0 ffe1','unresolved_dispatch_count_bound')
    def test_unrecognized_indirect(self):self.proposal_bad('ffe0','dispatch_memory_shape')
    def test_signed_guard(self):self.proposal_bad('83f803 7f07 ff248500200000 c3','unsigned_guard_shape_or_effect')
    def test_range_above_cap(self):self.proposal_bad('83f820 7707 ff248500200000 c3','dispatch_range_bound')
    def test_remap_above_cap(self):self.proposal_bad('81f880000000 770e 0fb68000300000 ff248500200000 c3','dispatch_range_bound')
    def test_table_wrap(self):self.proposal_bad('83f803 7707 ff2485f8ffffff c3','proposed_table_span_wrap')
    def test_wrong_index_register(self):self.proposal_bad('83f903 7707 ff248500200000 c3','guard_compare_shape_or_effect')
    def test_alternate_guard_entry(self):self.proposal_bad('83f803 7707 ff248500200000 ebf7','known_direct_prefix_bypass')
    def test_default_into_compare(self):self.proposal_bad('83f803 77fb ff248500200000 c3','default_enters_dispatch_prefix')
    def test_alternate_compare_entry_is_allowed(self):
        out=self.run_code('83f803 7707 ff248500200000 ebf2')
        self.assertEqual(out['table_range_proposal']['status'],'METADATA_ONLY_RANGE_PROPOSAL')
    def test_prefixed_compare(self):self.proposal_bad('6683f803 7707 ff248500200000 c3','noncontiguous_or_prefixed_dispatch_chain')
    def test_noncontiguous_guard(self):self.proposal_bad('83f803 7707 ff248500200000 ebf5','noncontiguous_or_prefixed_dispatch_chain|known_direct_prefix_bypass')
    def test_indexed_base_not_absolute(self):self.proposal_bad('83f803 7703 ff2483 c3','dispatch_memory_shape')
    def test_wrong_table_scale(self):self.proposal_bad('83f803 7707 ff244500200000 c3','dispatch_memory_shape')
    def test_narrow_remap(self):self.proposal_bad('83f803 770f 660fb68000300000 ff248500200000 c3','noncontiguous_or_prefixed_dispatch_chain')
    def test_optimized_refused(self):
        result=subprocess.run([sys.executable,'-O',c.__file__,'--help'],capture_output=True)
        self.assertNotEqual(result.returncode,0)
    def test_no_address_or_instruction_export(self):
        out=self.run_code('83f803 7707 ff248500200000 c3');encoded=json.dumps(out)
        for forbidden in ('0x1000','0x2000','eax','jmp','83f8','address','instruction_sequence'):
            self.assertNotIn(forbidden,encoded)

class Attribution(unittest.TestCase):
    def setUp(self):
        self.spec=c.SPEC;obj=self.spec['object'];self.maps=[dict(symbol=self.spec['symbol'],object=obj,section_number=1,section_offset=0,address=4096,flags='f')]
        self.cs=[dict(section_number=1,section_offset=0,size=1540,module_index=1,module='producer',object='/fake/build/'+obj,characteristics=3)]
        self.modules=[dict(index=1,module='producer',object='/fake/build/'+obj)]
        self.section=types.SimpleNamespace(rva=0,characteristics=3,virtual_size=4096)
        self.read=lambda a,b,n:b'X'*(n-4)+b'TABL'
    def result(self):
        def path(p):
            p=str(p)
            if not p.startswith('/'):raise ValueError('unrelated module path unsupported')
            return p.lower()
        image=types.SimpleNamespace(image_base=4096,section=lambda n:self.section,read_section=lambda *a:self.read(*a))
        l=types.SimpleNamespace(_object_path=path,_object_basename=lambda p:str(p).split('/')[-1].lower(),IMAGE_SCN_CNT_CODE=1,IMAGE_SCN_MEM_EXECUTE=2)
        return c.measure(l,image,self.maps,dict(modules=self.modules,contributions=self.cs),P('/fake/build'),self.spec)
    def test_full_extent_includes_unclassified(self):
        out=self.result();self.assertEqual(out['size'],1540);self.assertEqual(out['sha256'],hashlib.sha256(b'X'*1536+b'TABL').hexdigest())
    def test_size_mismatch_never_cropped(self):
        self.cs[0]['size']=1600;out=self.result();self.assertEqual(out['size'],1600);self.assertFalse(out['size_equal'])
    def test_unrelated_module_paths_do_not_create_ambiguity(self):
        self.modules.extend([dict(index=2,module='linker',object=''),dict(index=3,module='library',object='relative.obj')]);self.assertTrue(self.result()['complete'])
    def test_attribution_mutations(self):
        changes=[lambda:self.maps.append(dict(self.maps[0],symbol='alias')),lambda:self.maps.clear(),lambda:self.cs.append(dict(self.cs[0])),lambda:self.cs[0].update(object='/wrong/'+self.spec['object']),lambda:self.modules[0].update(object='/wrong/'+self.spec['object']),lambda:self.modules.append(dict(self.modules[0])),lambda:self.cs[0].update(module_index=2),lambda:self.cs[0].update(module='wrong'),lambda:self.maps[0].update(object='other.obj'),lambda:self.cs[0].update(characteristics=1),lambda:setattr(self.section,'characteristics',1),lambda:setattr(self.section,'virtual_size',1539),lambda:self.maps[0].update(address=0),lambda:self.maps.append(dict(self.maps[0],symbol='inside',section_offset=100)),lambda:self.cs.append(dict(self.cs[0],section_offset=100)),lambda:setattr(self,'read',lambda *a:b'X')]
        for i,change in enumerate(changes):
            self.setUp();change()
            with self.subTest(i=i),self.assertRaises((AssertionError,ValueError)):self.result()

class SpanMetadata(unittest.TestCase):
    def setUp(self):
        self.sections=[types.SimpleNamespace(rva=0,virtual_size=256,raw_size=256,characteristics=0x60000020)]
        self.owner=dict(section_number=1,section_offset=0,size=64)
        self.pdb={'contributions':[self.owner]}
        self.image=types.SimpleNamespace(image_base=0x1000,sections=self.sections,section=lambda n:self.sections[n-1])
    def meta(self,at=0x1000,size=64):return c.section_metadata(self.image,self.pdb,self.owner,at,size)
    def test_exact_owner(self):self.assertTrue(all(self.meta().values()))
    def test_partial_second_section_rejected(self):
        self.sections.append(types.SimpleNamespace(rva=32,virtual_size=256,raw_size=256,characteristics=0x60000020))
        self.assertFalse(self.meta()['unique_mapped_virtual_section']);self.assertFalse(self.meta()['raw_span_covered'])
    def test_raw_only_second_section_rejected(self):
        self.sections.append(types.SimpleNamespace(rva=32,virtual_size=0,raw_size=256,characteristics=0x60000020))
        self.assertFalse(self.meta()['unique_mapped_virtual_section'])
    def test_virtual_only_span(self):
        self.sections[0].raw_size=32;self.assertTrue(self.meta()['unique_mapped_virtual_section']);self.assertFalse(self.meta()['raw_span_covered'])
    def test_other_contribution_overlap(self):
        self.pdb['contributions'].append(dict(section_number=1,section_offset=32,size=64));self.assertFalse(self.meta()['disjoint_from_other_pdb_contributions'])
    def test_other_contribution_adjacent(self):
        self.pdb['contributions'].append(dict(section_number=1,section_offset=64,size=64));self.assertTrue(self.meta()['disjoint_from_other_pdb_contributions'])
    def test_unreadable_section(self):
        self.sections[0].characteristics=0x20000020;self.assertFalse(self.meta()['readable_section'])
    def test_unmapped(self):self.assertFalse(self.meta(0x2000)['unique_mapped_virtual_section'])
    def test_wrapping_span(self):
        with self.assertRaisesRegex(c.GateError,'section_span_wrap'):self.meta(0xfffffffc,8)

class Closure(unittest.TestCase):
    def setUp(self):
        self.md=Cs(CS_ARCH_X86,CS_MODE_32);self.md.detail=True
        self.code=bytearray(b'\xcc'*256)
        self.code[:20]=bytes.fromhex('83f87f 770e 0fb68080100000 ff248560100000 c3')
        self.code[32:34]=b'\xc3\xc3'
        self.code[128:256]=bytes([0,1])*64
        struct.pack_into('<II',self.code,96,START+32,START+33)
        self.reads=[];self.meta_calls=[]
        self.meta={'unique_mapped_virtual_section':True,'raw_span_covered':True,'readable_section':True,'executable_section':True,'disjoint_from_other_pdb_contributions':True}
        self.read_override=None;self.metadata_override=None
    def run_close(self,strict=False):
        def read(at,span):
            self.reads.append((at,span))
            if self.read_override:return self.read_override(at,span)
            return bytes(self.code[at-START:at-START+span])
        def metadata(at,span):
            self.meta_calls.append((at,span))
            return dict(self.metadata_override(at,span) if self.metadata_override else self.meta)
        return c.close_owner(self.md,bytes(self.code),START,metadata,read,strict)
    def bad(self,category,reads):
        with self.assertRaisesRegex(c.GateError,category):self.run_close()
        self.assertEqual(self.reads,reads)
    def test_two_targets_closed(self):
        out=self.run_close()
        self.assertEqual(self.reads,[(START+128,128),(START+96,8)])
        self.assertEqual(out['status'],'BOUNDED_COMPLETE_OWNER_CFG_CLOSED')
        self.assertTrue(out['complete_cfg_guard_has_no_bypass']);self.assertTrue(out['complete_cfg_code_and_tables_disjoint'])
        self.assertEqual(out['reachable_instruction_byte_count'],22);self.assertEqual(out['classified_table_byte_count'],136)
        self.assertEqual(out['unclassified_owner_byte_count'],98);self.assertEqual(out['fill_bytes_classified'],0)
        self.assertEqual(out['reachable_instruction_byte_count']+out['classified_table_byte_count']+out['unclassified_owner_byte_count'],256)
        self.assertFalse(out['all_owner_bytes_classified'])
    def test_fragmented_layout_predicates(self):
        out=self.run_close()
        self.assertFalse(out['instruction_bytes_form_contiguous_owner_prefix'])
        self.assertFalse(out['table_bytes_form_single_contiguous_trailing_suffix'])
        self.assertTrue(out['pointer_table_precedes_remap']);self.assertTrue(out['tables_end_at_owner_end'])
        self.assertTrue(out['layout_predicates_are_accounting_only'])
    def test_contiguous_prefix_and_trailing_tables(self):
        self.code=bytearray(b'\xcc'*160)
        self.code[:20]=bytes.fromhex('83f87f 770e 0fb68020100000 ff248518100000 c3')
        self.code[20:24]=b'\x90\x90\x90\xc3'
        struct.pack_into('<II',self.code,24,START+20,START+23)
        self.code[32:160]=bytes([0,1])*64
        out=self.run_close()
        for key in ('instruction_bytes_form_contiguous_owner_prefix','table_bytes_form_single_contiguous_trailing_suffix','pointer_table_precedes_remap','tables_end_at_owner_end'):
            self.assertTrue(out[key])
        self.assertEqual(out['reachable_instruction_byte_count'],24);self.assertEqual(out['unclassified_owner_byte_count'],0)
    def test_remap_precedes_pointer_table(self):
        self.code=bytearray(b'\xcc'*160)
        self.code[:20]=bytes.fromhex('83f87f 770e 0fb68018100000 ff248598100000 c3')
        self.code[20:24]=b'\x90\x90\x90\xc3'
        self.code[24:152]=bytes([0,1])*64
        struct.pack_into('<II',self.code,152,START+20,START+23)
        out=self.run_close()
        self.assertTrue(out['instruction_bytes_form_contiguous_owner_prefix']);self.assertTrue(out['table_bytes_form_single_contiguous_trailing_suffix'])
        self.assertFalse(out['pointer_table_precedes_remap']);self.assertTrue(out['tables_end_at_owner_end'])
    def test_tables_not_at_owner_end(self):
        self.code.extend(b'\xcc'*8)
        out=self.run_close();self.assertFalse(out['table_bytes_form_single_contiguous_trailing_suffix']);self.assertFalse(out['tables_end_at_owner_end'])

    def test_one_selected_pointer(self):
        self.code[128:256]=bytes(128)
        out=self.run_close();self.assertEqual(self.reads,[(START+128,128),(START+96,4)]);self.assertEqual(out['selected_dispatch_destination_count'],1)
    def test_five_slots_only_selected_targets(self):
        self.code[128:256]=bytes([0,4])*64
        struct.pack_into('<5I',self.code,96,START+32,0xffffffff,0,0x2000,START+33)
        out=self.run_close();self.assertEqual(self.reads,[(START+128,128),(START+96,20)])
        self.assertEqual(out['classified_table_byte_count'],148);self.assertEqual(out['selected_dispatch_destination_count'],2)
        self.assertTrue(out['unused_pointer_slots_are_not_execution_targets'])
    def test_duplicate_destinations(self):
        struct.pack_into('<I',self.code,100,START+32)
        out=self.run_close();self.assertEqual(out['selected_dispatch_destination_count'],1)
    def test_excess_pointer_extent_stops_before_read(self):
        self.code[128]=5
        out=self.run_close();self.assertEqual(out['status'],'UNRESOLVED_POINTER_SPAN_EXCEEDS_AUTHORIZED_BOUND')
        self.assertEqual(out['required_pointer_table_byte_count'],24);self.assertFalse(out['pointer_table_contents_inspected'])
        self.assertEqual(self.reads,[(START+128,128)])
    def test_maximum_byte_index_bounded_proposal(self):
        self.code[128]=255
        out=self.run_close();self.assertEqual(out['required_pointer_table_byte_count'],1024);self.assertEqual(self.reads,[(START+128,128)])
    def test_remap_must_be_128(self):
        self.code[2]=126;self.bad('authorized_exact_remap_proposal',[])
    def test_remap_outside_owner(self):
        struct.pack_into('<I',self.code,8,0x3000);self.bad('authorized_remap_classification',[])
    def test_remap_crosses_owner_end(self):
        struct.pack_into('<I',self.code,8,START+200);self.bad('authorized_remap_classification',[])
    def test_remap_overlaps_known_code(self):
        struct.pack_into('<I',self.code,8,START);self.bad('authorized_remap_classification',[])
    def test_remap_section_metadata_bad(self):
        for key in self.meta:
            self.setUp();self.meta[key]=False
            with self.subTest(key=key):self.bad('authorized_remap_classification',[])
    def test_remap_mapping_mismatch(self):
        self.read_override=lambda at,size:b'\0'*size;self.bad('remap_read_mapping',[(START+128,128)])
    def test_remap_truncated(self):
        self.read_override=lambda at,size:bytes(self.code[at-START:at-START+size-1]);self.bad('remap_read_mapping',[(START+128,128)])
    def test_pointer_outside_owner(self):
        struct.pack_into('<I',self.code,15,0x3000);self.bad('pointer_owner_remap_or_code_overlap',[(START+128,128)])
    def test_pointer_crosses_owner_end(self):
        struct.pack_into('<I',self.code,15,START+252);self.bad('pointer_owner_remap_or_code_overlap',[(START+128,128)])
    def test_pointer_overlaps_remap(self):
        struct.pack_into('<I',self.code,15,START+128);self.bad('pointer_owner_remap_or_code_overlap',[(START+128,128)])
    def test_pointer_partially_overlaps_remap(self):
        struct.pack_into('<I',self.code,15,START+124);self.bad('pointer_owner_remap_or_code_overlap',[(START+128,128)])
    def test_pointer_overlaps_known_code(self):
        struct.pack_into('<I',self.code,15,START);self.bad('pointer_owner_remap_or_code_overlap',[(START+128,128)])
    def test_pointer_section_metadata_bad(self):
        for key in self.meta:
            self.setUp()
            self.metadata_override=lambda at,size,k=key:dict(self.meta,**{k:False}) if at==START+96 else self.meta
            with self.subTest(key=key):self.bad('pointer_section_or_contribution_ambiguity',[(START+128,128)])
    def test_pointer_mapping_mismatch(self):
        self.read_override=lambda at,size:bytes(self.code[at-START:at-START+size]) if size==128 else bytes(size)
        self.bad('pointer_read_mapping',[(START+128,128),(START+96,8)])
    def test_selected_target_outside_owner(self):
        struct.pack_into('<I',self.code,96,START-1);self.bad('selected_table_target_outside_code_extent',[(START+128,128),(START+96,8)])
    def test_selected_target_at_owner_end(self):
        struct.pack_into('<I',self.code,96,START+256);self.bad('selected_table_target_outside_code_extent',[(START+128,128),(START+96,8)])
    def test_selected_target_in_remap(self):
        struct.pack_into('<I',self.code,96,START+128);self.bad('selected_table_target_outside_code_extent',[(START+128,128),(START+96,8)])
    def test_selected_target_in_pointer_table(self):
        struct.pack_into('<I',self.code,96,START+96);self.bad('selected_table_target_outside_code_extent',[(START+128,128),(START+96,8)])
    def test_selected_target_instruction_interior(self):
        struct.pack_into('<I',self.code,96,START+1);self.bad('overlapping_decode',[(START+128,128),(START+96,8)])
    def test_selected_target_at_compare_allowed(self):
        struct.pack_into('<I',self.code,96,START)
        out=self.run_close();self.assertTrue(out['complete_cfg_guard_has_no_bypass'])
    def test_table_edge_bypasses_compare(self):
        struct.pack_into('<I',self.code,96,START+3);self.bad('known_direct_prefix_bypass',[(START+128,128),(START+96,8)])
    def test_table_edge_bypasses_guard(self):
        struct.pack_into('<I',self.code,96,START+5);self.bad('known_direct_prefix_bypass',[(START+128,128),(START+96,8)])
    def test_table_edge_bypasses_remap(self):
        struct.pack_into('<I',self.code,96,START+12);self.bad('known_direct_prefix_bypass',[(START+128,128),(START+96,8)])
    def test_recovered_direct_edge_bypasses_guard(self):
        self.code[32:37]=b'\xe9'+struct.pack('<i',START+5-(START+37))
        struct.pack_into('<I',self.code,100,START+32)
        self.bad('known_direct_prefix_bypass',[(START+128,128),(START+96,8)])
    def test_unknown_recovered_instruction(self):
        self.code[32]=0xf4;self.bad('unsupported_control_group',[(START+128,128),(START+96,8)])
    def test_second_indirect_rejected(self):
        self.code[32:34]=b'\xff\xe0';struct.pack_into('<I',self.code,100,START+32)
        self.bad('unresolved_dispatch_count_bound|additional_indirect_dispatch',[(START+128,128),(START+96,8)])
    def test_recovered_external_branch(self):
        self.code[32:37]=b'\xe9'+struct.pack('<i',0x3000-(START+37));struct.pack_into('<I',self.code,100,START+32)
        self.bad('direct_branch_outside_owner_or_width',[(START+128,128),(START+96,8)])
    def test_recovered_branch_into_remap(self):
        self.code[32:37]=b'\xe9'+struct.pack('<i',START+128-(START+37));struct.pack_into('<I',self.code,100,START+32)
        self.bad('control_flow_enters_table',[(START+128,128),(START+96,8)])
    def test_recovered_instruction_crosses_pointer_table(self):
        self.code[94:96]=b'\xb8\0';struct.pack_into('<I',self.code,96,START+94);struct.pack_into('<I',self.code,100,START+94)
        self.bad('instruction_overlaps_table',[(START+128,128),(START+96,8)])
    def test_recovered_prefixed_control(self):
        self.code[32:34]=b'\xf3\xc3';struct.pack_into('<I',self.code,100,START+32)
        self.bad('control_prefix',[(START+128,128),(START+96,8)])
    def test_recovered_duplicate_prefix(self):
        self.code[32:37]=bytes.fromhex('6666890fc3');struct.pack_into('<I',self.code,100,START+32)
        self.bad('unsupported_raw_prefix_sequence',[(START+128,128),(START+96,8)])
    def test_native_pin_required(self):
        with self.assertRaisesRegex(c.GateError,'complete_owner_closure_pin'):self.run_close(True)
        self.assertEqual(self.reads,[])
    def test_hashes_match_classified_payloads(self):
        out=self.run_close();self.assertEqual(out['remap_sha256'],hashlib.sha256(bytes(self.code[128:256])).hexdigest())
        self.assertEqual(out['pointer_table_sha256'],hashlib.sha256(bytes(self.code[96:104])).hexdigest())
        self.assertEqual(out['reachable_instruction_bytes_sha256'],hashlib.sha256(bytes(self.code[:20]+self.code[32:34])).hexdigest())
    def test_no_payload_addresses_or_trace_exported(self):
        encoded=json.dumps(self.run_close())
        for forbidden in ('0x1000','0x1080','eax','jmp','83f8','dispatch_site','pointer_address','instruction_sequence'):
            self.assertNotIn(forbidden,encoded)


class HelperMetadata(unittest.TestCase):
    def setUp(self):
        obj=c.SPEC['object']
        self.section=types.SimpleNamespace(number=1,rva=0,virtual_size=4096,raw_size=4096,raw_offset=0,characteristics=0x60000020)
        self.sections=[self.section]
        self.caller=dict(section_number=1,section_offset=0,size=256,module_index=1,module='producer',object='/fake/build/'+obj,characteristics=0x60000020)
        self.helper=dict(self.caller,section_offset=512,size=68)
        self.modules=[dict(index=1,module='producer',object='/fake/build/'+obj)]
        self.pdb={'modules':self.modules,'contributions':[self.caller,self.helper]}
        self.owner_map=dict(symbol=c.SPEC['symbol'],object=obj,section_number=1,section_offset=0,address=START,flags=['f'])
        self.helper_map=dict(self.owner_map,symbol=c.HELPER_SYMBOL,section_offset=512,address=START+512)
        self.maps=[self.owner_map,self.helper_map];self.statics=[self.helper_map]
        self.body_reads=[]
        def no_read(*args):
            self.body_reads.append(args);raise AssertionError('helper body must never be read')
        self.image=types.SimpleNamespace(image_base=START,sections=self.sections,section=lambda n:self.sections[n-1],data=bytes(4096),read_section=no_read,read_address=no_read)
        def path(p):
            p=str(p)
            if not p.startswith('/'):raise ValueError('unsupported object path')
            return p.lower()
        self.module=types.SimpleNamespace(_object_path=path,_object_basename=lambda p:str(p).replace('\\','/').split('/')[-1].lower(),IMAGE_SCN_CNT_CODE=0x20,IMAGE_SCN_MEM_EXECUTE=0x20000000)
    def identity(self):return c.helper_owner_metadata(self.module,self.image,self.maps,self.statics,self.pdb,P('/fake/build'),self.caller)
    def test_metadata_no_body_read(self):
        self.assertEqual(self.identity(),(START+512,68));self.assertEqual(self.body_reads,[])
    def test_unknown_helper_size_reported_whole(self):
        self.helper['size']=96;self.assertEqual(self.identity(),(START+512,96));self.assertEqual(self.body_reads,[])
    def test_ambiguous_or_invalid_identity_fails(self):
        changes=[
            lambda:self.maps.append(dict(self.helper_map)),
            lambda:self.statics.clear(),
            lambda:self.statics.append(dict(self.helper_map)),
            lambda:self.maps.append(dict(self.helper_map,symbol='alias')),
            lambda:self.pdb['contributions'].append(dict(self.helper)),
            lambda:self.helper.update(size=0),lambda:self.helper.update(size=4097),
            lambda:self.modules.append(dict(self.modules[0])),
            lambda:self.modules[0].update(object='/wrong/'+c.SPEC['object']),
            lambda:self.helper.update(object='/wrong/'+c.SPEC['object']),
            lambda:self.caller.update(object='/wrong/'+c.SPEC['object']),
            lambda:self.helper.update(module_index=2),lambda:self.helper.update(module='wrong'),
            lambda:self.helper_map.update(object='wrong.obj'),
            lambda:self.helper_map.update(object='/wrong/'+c.SPEC['object']),
            lambda:self.helper_map.update(flags=[]),
            lambda:self.helper.update(characteristics=0x40000040),
            lambda:setattr(self.section,'characteristics',0x40000040),
            lambda:setattr(self.section,'virtual_size',550),
            lambda:setattr(self.section,'raw_size',550),
            lambda:setattr(self.image,'data',bytes(550)),
            lambda:self.helper_map.update(address=START+513),
            lambda:self.maps.append(dict(self.helper_map,symbol='interior',section_offset=520,address=START+520)),
            lambda:self.pdb['contributions'].append(dict(self.helper,section_offset=520,size=16)),
            lambda:self.sections.append(types.SimpleNamespace(number=2,rva=530,virtual_size=1024,raw_size=1024,raw_offset=0,characteristics=0x60000020)),
        ]
        for index,change in enumerate(changes):
            self.setUp();change()
            with self.subTest(index=index),self.assertRaises((c.GateError,ValueError)):self.identity()
            self.assertEqual(self.body_reads,[])
    def test_full_map_path_allowed_when_exact(self):
        self.helper_map['object']='/fake/build/'+c.SPEC['object'];self.assertEqual(self.identity(),(START+512,68))
    def test_unrelated_path_metadata_is_not_helper(self):
        self.modules.append(dict(index=9,module='linker',object=''));self.assertEqual(self.identity(),(START+512,68))

class HelperCallEdges(unittest.TestCase):
    def setUp(self):
        self.md=Cs(CS_ARCH_X86,CS_MODE_32);self.md.detail=True
        self.helper=START+512;self.helper_size=68
    def call(self,target):return b'\xe8'+struct.pack('<i',target-(START+5))
    def count(self,code):
        capture={}
        out=c.preflight(self.md,code,START,lambda *a:{},capture)
        self.assertTrue(out['full_owner_cfg_closed_under_model'])
        return c.count_direct_helper_calls(self.md,capture['nodes'],START,len(code),self.helper,self.helper_size)
    def test_actual_direct_call_counted(self):
        out=self.count(self.call(self.helper)+b'\xc3')
        self.assertEqual(out['reachable_direct_call_instruction_count'],1)
        self.assertFalse(out['helper_body_sliced_decoded_or_hashed']);self.assertTrue(out['caller_and_helper_sizes_not_combined'])
    def test_two_distinct_call_instructions(self):
        code=self.call(self.helper)+b'\xe8'+struct.pack('<i',self.helper-(START+10))+b'\xc3'
        self.assertEqual(self.count(code)['reachable_direct_call_instruction_count'],2)
    def test_unreachable_call_form_not_counted(self):
        self.assertEqual(self.count(b'\xc3'+self.call(self.helper))['reachable_direct_call_instruction_count'],0)
    def test_call_form_inside_mov_immediate_not_counted(self):
        self.assertEqual(self.count(bytes.fromhex('b8e8000000 c3'))['reachable_direct_call_instruction_count'],0)
    def test_indirect_call_not_identity_evidence(self):
        self.assertEqual(self.count(b'\xff\xd0\xc3')['reachable_direct_call_instruction_count'],0)
    def test_other_direct_call_not_counted(self):
        self.assertEqual(self.count(self.call(0x3000)+b'\xc3')['reachable_direct_call_instruction_count'],0)
    def test_call_helper_interior_rejected(self):
        with self.assertRaisesRegex(c.GateError,'call_into_helper_interior'):self.count(self.call(self.helper+1)+b'\xc3')
    def test_helper_end_is_other_owner(self):
        self.assertEqual(self.count(self.call(self.helper+self.helper_size)+b'\xc3')['reachable_direct_call_instruction_count'],0)
    def test_closed_cfg_capture_contains_only_reached_nodes(self):
        self.assertEqual(self.count(b'\xeb\x05'+self.call(self.helper)+b'\xc3')['reachable_direct_call_instruction_count'],0)
    def test_output_has_no_sites_targets_or_body_hash(self):
        encoded=json.dumps(self.count(self.call(self.helper)+b'\xc3'))
        for forbidden in ('0x1000','0x1200','call_sites','helper_body_sha256','helper_address','instruction_sequence'):
            self.assertNotIn(forbidden,encoded)

if __name__=='__main__':unittest.main()
