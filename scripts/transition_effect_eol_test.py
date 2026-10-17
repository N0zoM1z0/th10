#!/usr/bin/env python3
"""LOCAL-ONLY regression tests for exact CRLF-to-LF host dependency pins."""
from pathlib import Path
import argparse, hashlib, json, shutil, tempfile
from transition_effect_host_test import ROOT, DEFAULT_CANON, DEPENDENCIES, fingerprint, normalize_crlf, read, validate_canonical

def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--canonical',type=Path,default=DEFAULT_CANON)
    ap.add_argument('--out',type=Path,default=ROOT/'.analysis/transition-effect-host/eol')
    a=ap.parse_args();a.out.mkdir(parents=True,exist_ok=True)
    pinfile=ROOT/'config/transition-effect-host-inputs.json'
    original=validate_canonical(a.canonical,pinfile)
    outcomes=[]
    def record(name,fn):
        fn();outcomes.append({'name':name,'status':'PASS'});print('PASS:'+name)
    def reject(canonical,pin=pinfile):
        try:validate_canonical(canonical,pin)
        except RuntimeError as exc:
            assert str(exc).startswith(('CANONICAL_PIN_MISMATCH:','HOST_PIN_SCHEMA_MISMATCH:')),str(exc)
        else:raise AssertionError('altered input accepted')
    with tempfile.TemporaryDirectory(prefix='effect-eol-',dir=a.out) as temporary:
        temp=Path(temporary);canonical=temp/'src';canonical.mkdir()
        pristine={name:normalize_crlf((a.canonical/name).read_bytes()) for name in DEPENDENCIES}
        def restore():
            for name,data in pristine.items():(canonical/name).write_bytes(data)
        def accept_style(style):
            for name,data in pristine.items():
                if style=='crlf':data=data.replace(b'\n',b'\r\n')
                elif style=='mixed':
                    lines=data.split(b'\n');data=b''.join(line+(b'\r\n' if i%2 else b'\n') for i,line in enumerate(lines[:-1]))+lines[-1]
                (canonical/name).write_bytes(data)
            observed=validate_canonical(canonical,pinfile)
            for name in DEPENDENCIES:
                assert observed[str(canonical/name)]['sha256_lf']==hashlib.sha256(pristine[name]).hexdigest()
                assert observed[str(canonical/name)]['bare_cr_count']==0
        for style in ['lf','crlf','mixed']:record('accept_'+style,lambda style=style:accept_style(style))
        edits={
            'reject_added_space':lambda d:d+b' ',
            'reject_added_tab':lambda d:d+b'\t',
            'reject_bare_cr':lambda d:d+b'\r',
            'reject_cr_only_line':lambda d:d.replace(b'\n',b'\r',1),
            'reject_double_crlf':lambda d:d.replace(b'\n',b'\r\r\n',1),
            'reject_content':lambda d:d.replace(b'namespace th10',b'namespace th11',1),
            'reject_added_newline':lambda d:d+b'\n',
            'reject_utf8_bom':lambda d:b'\xef\xbb\xbf'+d,
            'reject_empty':lambda d:b'',
        }
        for name,edit in edits.items():
            def test(edit=edit):
                restore();old=pristine['Chain.cpp'];changed=edit(old);assert changed!=old
                (canonical/'Chain.cpp').write_bytes(changed);reject(canonical)
            record(name,test)
        def missing():restore();(canonical/'Chain.cpp').unlink();reject(canonical)
        record('reject_missing_input',missing)
        config=json.loads(read(pinfile));alternate=temp/'pins.json'
        def schema_case(kind):
            restore();changed=dict(config)
            if kind=='legacy':changed={'sha256':config['observed_raw_sha256']}
            elif kind=='raw_fallback':
                changed['sha256_lf']=dict(config['sha256_lf']);changed['sha256_lf']['src/Chain.cpp']='0'*64
                changed['observed_raw_sha256']=dict(config['observed_raw_sha256']);changed['observed_raw_sha256']['src/Chain.cpp']=fingerprint(canonical/'Chain.cpp')['sha256_raw']
            elif kind=='missing_normalized':changed.pop('sha256_lf')
            elif kind=='unknown_normalization':changed['normalization']='universal-newlines'
            alternate.write_text(json.dumps(changed));reject(canonical,alternate)
        for kind in ['legacy','raw_fallback','missing_normalized','unknown_normalization']:
            record('reject_'+kind,lambda kind=kind:schema_case(kind))
        def raw_is_provenance_only():
            restore();changed=dict(config);changed['observed_raw_sha256']={name:'0'*64 for name in config['observed_raw_sha256']}
            alternate.write_text(json.dumps(changed));validate_canonical(canonical,alternate)
        record('raw_provenance_is_not_acceptance_pin',raw_is_provenance_only)
        def strict_reader():
            p=temp/'reader.txt';p.write_bytes(b'A\rB\r\nC\n')
            assert read(p)=='A\rB\nC\n'
            info=fingerprint(p);assert info['crlf_count']==1 and info['bare_cr_count']==1
        record('reader_preserves_bare_cr',strict_reader)
    report={'canonical':str(a.canonical),'pin_fingerprint':fingerprint(pinfile),'observed_inputs':original,'cases':outcomes,'total':len(outcomes)}
    (a.out/'report.json').write_text(json.dumps(report,indent=2)+'\n')
    print('EOL_PIN_TESTS '+str(len(outcomes))+'/'+str(len(outcomes)))
if __name__=='__main__':main()
