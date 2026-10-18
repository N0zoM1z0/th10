"""Authored synthetic x86 only: no original/generated target bytes or execution."""
import copy, hashlib, importlib.util, json, pathlib, struct, subprocess, sys, types, unittest
P=pathlib.Path
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
sp=importlib.util.spec_from_file_location('collector', P(__file__).with_name('replay_a006_structure.py'))
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



class MetadataInputs(unittest.TestCase):
    def setUp(self):
        self.obj=c.SPEC['object'];self.section=types.SimpleNamespace(number=1,rva=0,virtual_size=8192,raw_size=8192,raw_offset=0,characteristics=0x60000020)
        self.data_section=types.SimpleNamespace(number=2,rva=8192,virtual_size=4096,raw_size=4096,raw_offset=8192,characteristics=0x40000040)
        self.sections=[self.section,self.data_section]
        self.caller=dict(section_number=1,section_offset=0,size=1596,module_index=0,module='producer',object='/fake/build/'+self.obj,characteristics=0x60000020)
        self.maps=[dict(symbol=c.SPEC['symbol'],object=self.obj,section_number=1,section_offset=0,address=START,flags=['f'])]
        self.pdb={'modules':[dict(index=0,module='producer',object='/fake/build/'+self.obj)],'contributions':[self.caller]}
        def no_read(*args):raise AssertionError('No callee/anchor code or data reads allowed')
        self.image=types.SimpleNamespace(image_base=START,sections=self.sections,section=lambda n:self.sections[n-1],data=bytes(12288),read_section=no_read,read_address=no_read)
        def path(p):
            p=str(p)
            if not p.startswith('/'):raise ValueError('object path')
            return p.lower()
        self.module=types.SimpleNamespace(_object_path=path,_object_basename=lambda p:str(p).replace('\\','/').split('/')[-1].lower(),IMAGE_SCN_CNT_CODE=0x20,IMAGE_SCN_MEM_EXECUTE=0x20000000)
        self.coff={'sections':[{'size':1596,'raw_offset':60,'flags':0x20001020}],'symbols':[{'name':c.SPEC['symbol'],'value':0,'section':1,'type':0x20,'storage':2}]}
    def absence(self,text='synthetic MAP'):return c.helper_absence_metadata(self.module,self.image,self.maps,self.pdb,P('/fake/build'),self.caller,self.coff,text)
    def test_qualified_absence_no_body_read(self):
        out=self.absence();self.assertEqual(out['exact_helper_map_symbol_count'],0);self.assertEqual(out['exact_helper_normal_coff_symbol_count'],0)
        self.assertFalse(out['full_named_pdb_symbol_index_parsed']);self.assertFalse(out['corresponding_named_callee_absence_proved'])
    def test_helper_raw_map_name_found(self):
        with self.assertRaisesRegex(c.GateError,'helper_map_symbol_present'):self.absence('skipped malformed line '+c.HELPER_SYMBOL)
    def test_helper_map_entry_found(self):
        self.maps.append(dict(self.maps[0],symbol=c.HELPER_SYMBOL,section_offset=2000,address=START+2000))
        with self.assertRaisesRegex(c.GateError,'helper_map_symbol_present'):self.absence()
    def test_helper_coff_reference_found(self):
        self.coff['symbols'].append(dict(name=c.HELPER_SYMBOL,value=0,section=0,type=0,storage=2))
        with self.assertRaisesRegex(c.GateError,'helper_coff_symbol_present'):self.absence()
    def test_other_same_object_pdb_owner_must_be_named(self):
        self.pdb['contributions'].append(dict(self.caller,section_offset=2000,size=68))
        with self.assertRaisesRegex(c.GateError,'absence_unattributed_or_aliased_pdb_owner'):self.absence()
    def test_other_same_object_named_owner_allowed(self):
        self.pdb['contributions'].append(dict(self.caller,section_offset=2000,size=68))
        self.maps.append(dict(self.maps[0],symbol='_Other',section_offset=2000,address=START+2000))
        self.assertEqual(self.absence()['same_normal_object_code_contribution_count'],2)
    def test_pdb_or_coff_ambiguity_rejected(self):
        changes=[lambda:self.pdb['modules'].append(dict(self.pdb['modules'][0])),lambda:self.caller.update(object='/wrong/'+self.obj),lambda:self.caller.update(module_index=2),lambda:self.maps.append(dict(self.maps[0],symbol='alias')),lambda:self.maps[0].update(object='wrong.obj'),lambda:self.maps[0].update(address=0),lambda:self.maps.append(dict(self.maps[0],symbol='interior',section_offset=10,address=START+10)),lambda:self.coff['symbols'].append(dict(self.coff['symbols'][0])),lambda:self.coff['symbols'][0].update(value=1),lambda:self.coff['symbols'][0].update(type=0),lambda:self.coff['sections'][0].update(size=1595),lambda:self.coff['sections'][0].update(flags=0x20),lambda:self.coff['symbols'].append(dict(self.coff['symbols'][0],name='_Other'))]
        for index,change in enumerate(changes):
            self.setUp();change()
            with self.subTest(index=index),self.assertRaises(c.GateError):self.absence()
    def anchors(self):
        bindings={f'_Symbol{i:03d}':f'_th10_ltcg_probe_anchor_{i:04d}' for i in range(203)}
        aliases=''.join(f'/alternatename:{s}={a}\n' for s,a in bindings.items()).encode()
        source=('/* Diagnostic-only, non-runnable unresolved-symbol address anchors. */\n'+''.join('unsigned char '+a[1:]+'[4] = {0};\n' for a in bindings.values())).encode()
        self.maps += [dict(symbol=a,object='link-anchors.obj',section_number=2,section_offset=i*4,address=START+8192+i*4,flags=[]) for i,a in enumerate(bindings.values())]
        return aliases,source
    def scope(self,a,b):return c.alias_scope_proposal(self.module,self.image,self.maps,a,b)
    def test_alias_hash_proposal_no_source_identity_claim(self):
        a,b=self.anchors();out=self.scope(a,b)
        self.assertEqual(out['binding_count'],203);self.assertFalse(out['source_callee_attribution_performed']);self.assertTrue(out['requires_independent_exact_source_hash_pin_before_census'])
        self.assertEqual(out['alias_sha256'],hashlib.sha256(a).hexdigest())
    def test_virtual_only_zero_fill_anchors_allowed(self):
        a,b=self.anchors();self.data_section.raw_size=0
        self.assertTrue(self.scope(a,b)['numbered_anchor_map_identities_validated'])
    def test_anchor_virtual_extent_required(self):
        a,b=self.anchors();self.data_section.virtual_size=811
        with self.assertRaisesRegex(c.GateError,'anchor_metadata_mapping'):self.scope(a,b)
    def test_alias_mutations_fail(self):
        for kind in range(9):
            self.setUp();a,b=self.anchors()
            if kind==0:a=a.replace(b'/alternatename:',b'/other:',1)
            elif kind==1:a=a.replace(b'anchor_0000',b'anchor_0001',1)
            elif kind==2:a=a.replace(b'\n',b'\r\n')
            elif kind==3:a=a.split(b'\n',1)[1]
            elif kind==4:a=a.replace(b'_Symbol000',b'_Symbol001',1)
            elif kind==5:b=b.replace(b'[4]',b'[8]',1)
            elif kind==6:self.maps.append(dict(self.maps[1]))
            elif kind==7:self.maps[1]['object']='wrong.obj'
            elif kind==8:self.maps[1]['flags']=['f']
            with self.subTest(kind=kind),self.assertRaises(c.GateError):self.scope(a,b)
    def test_alias_non_ascii_rejected(self):
        a,b=self.anchors()
        with self.assertRaises(UnicodeDecodeError):self.scope(a+b'\xff',b)
    def test_literal_map_census_keeps_anchors_unresolved(self):
        self.anchors();md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True
        code=b'\xe8'+struct.pack('<i',START+8192-(START+5))+b'\xc3'
        nodes={i.address:i for i in md.disasm(code,START)}
        out=c.raw_callee_metadata(self.module,self.image,nodes,self.maps)
        self.assertEqual(out['generated_anchor_direct_call_count'],1);self.assertFalse(out['anchor_source_names_resolved']);self.assertEqual(out['literal_map_symbol_call_counts_unordered'],{})
    def test_literal_map_real_call_count(self):
        self.maps.append(dict(self.maps[0],symbol='_Real',section_offset=3000,address=START+3000))
        md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True;code=b'\xe8'+struct.pack('<i',START+3000-(START+5))+b'\xc3'
        out=c.raw_callee_metadata(self.module,self.image,{i.address:i for i in md.disasm(code,START)},self.maps)
        self.assertEqual(out['literal_map_symbol_call_counts_unordered'],{'_Real':1})
    def test_callee_alias_ambiguity_is_unresolved(self):
        self.maps.extend([dict(self.maps[0],symbol='_Real',section_offset=3000,address=START+3000),dict(self.maps[0],symbol='_Alias',section_offset=3000,address=START+3000)])
        md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True;code=b'\xe8'+struct.pack('<i',START+3000-(START+5))+b'\xc3'
        out=c.raw_callee_metadata(self.module,self.image,{i.address:i for i in md.disasm(code,START)},self.maps)
        self.assertEqual(out['unresolved_or_aliased_direct_call_count'],1)

class COFFMetadataParser(unittest.TestCase):
    def make(self,names):
        strings=bytearray(b'\0'*4);records=bytearray()
        for name in names:
            offset=len(strings);strings+=name.encode()+b'\0';records+=struct.pack('<8sIhHBB',struct.pack('<II',0,offset),0,1,0x20,2,0)
        struct.pack_into('<I',strings,0,len(strings))
        header=struct.pack('<HHIIIHH',0x14c,1,0,64,len(names),0,0)
        section=struct.pack('<8sIIIIIIHHI',b'.text\0\0\0',0,0,4,60,0,0,0,0,0x20001020)
        return bytearray(header+section+b'XXXX'+records+strings)
    def test_complete_symbols(self):
        out=c.normal_coff_symbols(bytes(self.make([c.SPEC['symbol'],'_Other'])));self.assertEqual([x['name'] for x in out['symbols']],[c.SPEC['symbol'],'_Other'])
    def test_helper_name_found_without_body_read(self):
        out=c.normal_coff_symbols(bytes(self.make([c.SPEC['symbol'],c.HELPER_SYMBOL])));self.assertEqual(out['symbols'][1]['name'],c.HELPER_SYMBOL)
    def test_metadata_truncation_rejected(self):
        for end in (0,19,59,63,81):
            with self.subTest(end=end),self.assertRaises(c.GateError):c.normal_coff_symbols(bytes(self.make(['_X'])[:end]))
    def test_header_and_symbol_mutations_rejected(self):
        for offset,fmt,value in [(0,'H',0x8664),(16,'H',1),(8,'I',1),(12,'I',0xffffffff),(64+17,'B',255),(64+12,'h',2),(64+4,'I',0xffffffff)]:
            raw=self.make(['_X']);struct.pack_into('<'+fmt,raw,offset,value)
            with self.subTest(offset=offset,fmt=fmt),self.assertRaises(c.GateError):c.normal_coff_symbols(bytes(raw))
    def test_uninitialized_data_section_needs_no_body(self):
        raw=self.make(['_X']);struct.pack_into('<I',raw,40,0);struct.pack_into('<I',raw,56,0x40000080)
        self.assertEqual(c.normal_coff_symbols(bytes(raw))['sections'][0]['raw_offset'],0)
    def test_unterminated_name_rejected(self):
        raw=self.make(['_X']);raw[-1]=ord('A')
        with self.assertRaisesRegex(c.GateError,'coff_symbol_string_termination'):c.normal_coff_symbols(bytes(raw))




class PinnedCalleeMetadata(unittest.TestCase):
    def setUp(self):
        MetadataInputs.setUp(self)
        self.aliases, self.anchors_source = MetadataInputs.anchors(self)
        self.anchor = self.maps[1]
        self.maps.append(dict(self.anchor, symbol='_Symbol000'))
        self.alias_row = self.maps[-1]
        self.md = Cs(CS_ARCH_X86, CS_MODE_32);self.md.detail=True
    def nodes(self, targets=(), indirect=False):
        code=bytearray()
        for target in targets:
            code += b'\xe8'+struct.pack('<i',target-(START+len(code)+5))
        if indirect:code += b'\xff\xd0'
        code += b'\xc3'
        return {i.address:i for i in self.md.disasm(bytes(code),START)}
    def census(self, targets=None, indirect=False, aliases=None, source=None):
        from unittest.mock import patch
        pins={'link-aliases.rsp':c.sha_bytes(self.aliases),'link-anchors.c':c.sha_bytes(self.anchors_source)}
        with patch.dict(c.ALIAS_PINS,pins):
            return c.pinned_callee_census(self.module,self.image,self.nodes([self.anchor['address']] if targets is None else targets,indirect),self.maps,self.aliases if aliases is None else aliases,self.anchors_source if source is None else source)
    def test_exact_pair_resolves_without_body_reads(self):
        out=self.census();self.assertEqual(out['generated_alias_source_symbol_call_counts_unordered'],{'_Symbol000':1})
        self.assertTrue(out['all_recovered_calls_direct_and_named']);self.assertTrue(out['exact_named_helper_absent_from_recovered_calls'])
        self.assertFalse(out['generated_anchor_is_callable_implementation']);self.assertFalse(out['anchor_data_bytes_read'])
    def test_virtual_only_pair_resolves(self):
        self.data_section.raw_size=0;self.assertEqual(self.census()['generated_alias_direct_call_count'],1)
    def test_pair_and_literal_unordered_counts(self):
        self.maps.append(dict(self.maps[0],symbol='_Real',section_offset=3000,address=START+3000))
        a=self.anchor['address'];out=self.census([a,START+3000,a])
        self.assertEqual(out['generated_alias_source_symbol_call_counts_unordered'],{'_Symbol000':2})
        self.assertEqual(out['literal_map_symbol_call_counts_unordered'],{'_Real':1})
        self.assertEqual(out['direct_call_instruction_count'],3)
    def test_anchor_only_unresolved(self):
        self.maps.remove(self.alias_row);out=self.census()
        self.assertEqual(out['unresolved_direct_call_instruction_count'],1);self.assertFalse(out['exact_named_helper_absent_from_recovered_calls'])
    def test_third_symbol_unresolved(self):
        self.maps.append(dict(self.anchor,symbol='_Third'))
        self.assertEqual(self.census()['unresolved_direct_call_instruction_count'],1)
    def test_wrong_source_pair_unresolved(self):
        self.alias_row['symbol']='_Unexpected';self.assertEqual(self.census()['unresolved_direct_call_instruction_count'],1)
    def test_inconsistent_pair_metadata_unresolved(self):
        for key,value in [('section_number',1),('section_offset',1),('flags',['f']),('object','other.obj')]:
            self.setUp();self.alias_row[key]=value
            with self.subTest(key=key):self.assertEqual(self.census()['unresolved_direct_call_instruction_count'],1)
    def test_source_symbol_elsewhere_unresolved(self):
        self.maps.append(dict(self.alias_row,address=START+6000,section_offset=6000))
        self.assertEqual(self.census()['unresolved_direct_call_instruction_count'],1)
    def test_literal_symbol_elsewhere_unresolved(self):
        self.maps.extend([dict(self.maps[0],symbol='_Real',section_offset=3000,address=START+3000),dict(self.maps[0],symbol='_Real',section_offset=4000,address=START+4000)])
        self.assertEqual(self.census([START+3000])['unresolved_direct_call_instruction_count'],1)
    def test_unknown_direct_target_unresolved(self):
        out=self.census([START+7000]);self.assertEqual(out['unresolved_direct_call_instruction_count'],1)
        self.assertIsNone(out['exact_named_helper_recovered_direct_call_count'])
    def test_indirect_call_blocks_complete_census(self):
        out=self.census([],True);self.assertEqual(out['indirect_call_instruction_count'],1)
        self.assertFalse(out['all_recovered_calls_direct_and_named']);self.assertFalse(out['exact_named_helper_absent_from_recovered_calls'])
    def test_helper_literal_presence(self):
        self.maps.append(dict(self.maps[0],symbol=c.HELPER_SYMBOL,section_offset=3000,address=START+3000))
        out=self.census([START+3000]);self.assertEqual(out['exact_named_helper_recovered_direct_call_count'],1)
        self.assertFalse(out['exact_named_helper_absent_from_recovered_calls'])
    def test_helper_generated_presence(self):
        self.aliases=self.aliases.replace(b'_Symbol000',c.HELPER_SYMBOL.encode());self.alias_row['symbol']=c.HELPER_SYMBOL
        out=self.census();self.assertEqual(out['exact_named_helper_recovered_direct_call_count'],1)
        self.assertFalse(out['exact_named_helper_absent_from_recovered_calls'])
    def test_alias_pin_mismatch(self):
        with self.assertRaisesRegex(c.GateError,'generated_alias_source_pin'):self.census(aliases=self.aliases+b'\n')
    def test_anchor_source_pin_mismatch(self):
        with self.assertRaisesRegex(c.GateError,'generated_alias_source_pin'):self.census(source=self.anchors_source+b'\n')
    def test_overlapping_anchor_virtual_extent_rejected(self):
        self.maps[2].update(section_offset=1,address=self.anchor['address']+1)
        with self.assertRaisesRegex(c.GateError,'anchor_virtual_extent_overlap'):self.census()
    def test_target_inside_anchor_unresolved(self):
        self.assertEqual(self.census([self.anchor['address']+1])['unresolved_direct_call_instruction_count'],1)
    def test_noncode_literal_unresolved(self):
        self.section.characteristics=0x60000000
        self.maps.append(dict(self.maps[0],symbol='_Real',section_offset=3000,address=START+3000))
        self.assertEqual(self.census([START+3000])['unresolved_direct_call_instruction_count'],1)
    def test_literal_without_raw_extent_unresolved(self):
        self.section.raw_size=2999
        self.maps.append(dict(self.maps[0],symbol='_Real',section_offset=3000,address=START+3000))
        self.assertEqual(self.census([START+3000])['unresolved_direct_call_instruction_count'],1)
    def test_bound_rejected(self):
        with self.assertRaisesRegex(c.GateError,'callee_census_bound'):self.census([self.anchor['address']]*65)
    def test_no_sites_or_order_exported(self):
        out=json.dumps(self.census())
        for word in ('address','section_number','section_offset','callsite','instruction_sequence','0x1000'):
            self.assertNotIn(word,out)


class UnclassifiedGapMetadata(unittest.TestCase):
    def setUp(self):
        self.md=Cs(CS_ARCH_X86,CS_MODE_32);self.md.detail=True
        # Authored RET followed by a synthetic uninterpreted gap and table.
        nodes={i.address:i for i in self.md.disasm(b'\xc3',START)}
        self.closed={'nodes':nodes,'edges':{START:[]},'covered':{START},'remaining':set(range(START+1,START+4)),'tables':frozenset(range(START+4,START+8))}
        self.result={'full_owner_cfg_closed_under_model':True,'complete_cfg_guard_has_no_bypass':True,'unclassified_owner_byte_count':3,'unclassified_owner_bytes_sha256':'a'*64}
    def check(self):return c.gap_metadata(START,8,self.closed,self.result)
    def test_gap_metadata_preserves_unclassified(self):
        out=self.check();self.assertEqual(out['byte_count'],3);self.assertEqual(out['sha256'],'a'*64)
        self.assertTrue(out['single_contiguous_span']);self.assertTrue(out['between_contiguous_instruction_prefix_and_table_suffix'])
        self.assertTrue(out['preceded_by_recovered_unconditional_control_terminator']);self.assertTrue(out['no_recovered_cfg_edge_enters_span'])
        self.assertTrue(out['remains_unclassified']);self.assertFalse(out['contents_interpreted']);self.assertFalse(out['fill_classification_performed'])
    def test_four_byte_gap_rejected(self):
        self.closed['remaining'].add(START+4);self.closed['tables']=frozenset(range(START+5,START+8));self.result['unclassified_owner_byte_count']=4
        with self.assertRaisesRegex(c.GateError,'gap_extent_bound'):self.check()
    def test_partition_overlap_rejected(self):
        self.closed['remaining'].add(START)
        with self.assertRaisesRegex(c.GateError,'gap_partition'):self.check()
    def test_outside_owner_rejected(self):
        self.closed['remaining'].remove(START+1);self.closed['remaining'].add(START+9)
        with self.assertRaisesRegex(c.GateError,'gap_partition'):self.check()
    def test_open_cfg_rejected(self):
        self.result['full_owner_cfg_closed_under_model']=False
        with self.assertRaisesRegex(c.GateError,'gap_requires_closed_cfg'):self.check()
    def test_edge_into_gap_rejected(self):
        self.closed['edges'][START]=[START+1]
        with self.assertRaisesRegex(c.GateError,'gap_closed_successor_set'):self.check()
    def test_noncontiguous_gap_reported(self):
        self.closed['remaining']={START+1,START+3,START+5};self.closed['tables']=frozenset({START+2,START+4,START+6,START+7})
        out=self.check();self.assertFalse(out['single_contiguous_span']);self.assertFalse(out['between_contiguous_instruction_prefix_and_table_suffix'])
    def test_instruction_coverage_mismatch_rejected(self):
        self.closed['covered'].add(START+1);self.closed['remaining'].remove(START+1);self.result['unclassified_owner_byte_count']=2
        with self.assertRaisesRegex(c.GateError,'gap_instruction_coverage'):self.check()
    def test_nonterminating_predecessor_not_claimed(self):
        self.closed['nodes']={i.address:i for i in self.md.disasm(b'\x90',START)}
        self.assertFalse(self.check()['preceded_by_recovered_unconditional_control_terminator'])
    def test_no_offsets_exported(self):
        out=json.dumps(self.check())
        for word in ('address','offset','start','end','0x1000'):
            self.assertNotIn(word,out)

if __name__=='__main__':unittest.main()
