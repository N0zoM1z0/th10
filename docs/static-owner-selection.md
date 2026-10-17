# Opt-in linked-PE static owner selection

Canonical match units remain public-only unless they explicitly declare:

```toml
owner_visibility = "static"
pdb_source = "src/TransitionEffect.cpp"
```

The existing `symbol` is an exact decorated MAP spelling. `pdb_source` is
mandatory for static selection and must be the primary `source` or an ordered
`support_sources` member. The selector derives `source.ltcg.obj` or
`support-NN-Stem.ltcg.obj` (`normal.obj` for a declared normal support source)
from that producer graph. Source paths must be canonical repository-relative
paths, with no backslashes, empty components, drive prefix, `.` or `..`.
The build manifest loader still verifies source files and build profiles.

Only `public` and `static` are supported; the key is illegal on COFF or archive
units. Static `size` must be a positive integer, `compare_size` must equal it,
and no data spans are permitted. Existing public selection, reports, linkage
symbol resolution, and normalization rules are unchanged. The static symbol
never becomes a public or a linkage referent.

## Evidence and rejection rules

Selection parses the actual MAP `Static symbols` section, which must be unique
and contain only symbol rows and blank lines. It requires exactly one row
matching the decorated symbol and expected object basename. Same spelling in
another object at a different start is allowed; duplicate qualified rows or
any public/static alias at the selected start are rejected.

The MAP section/offset must join to exactly one PDB contribution at that exact
start. Containing-contribution fallback and next-symbol size inference are not
used. Contributions overlapping the selected extent are rejected, including
non-code contributions. MAP function flag, PDB code/execute flags, PE
code/execute flags, MAP VA, full PDB extent, and raw PE bounds must agree.
There must be exactly one PDB module with the derived object basename.

The PDB object must also be the expected file in the linked image's output
directory. Path comparison permits slash/case differences and the maintained
Wine `Z:` root spelling only. Other drive mappings, relative PDB paths,
wrong directories, and ambiguous same-basename modules fail closed. A fully
qualified MAP object must agree in full as well. Moving archived evidence to
another directory does not silently rebind its embedded PDB object identity.

The existing PE/MAP timestamp and image-base checks and PE/PDB GUID and age
checks run before static ownership selection. No source linkage, compiler
profile, retention, translation-unit graph, link anchor, or field-comparison
change is involved.

## Maintained commands

Use the existing manifest-driven commands without additional CLI switches:

```sh
scripts/repo-python scripts/build-match-unit.py --check
scripts/repo-python scripts/build-match-unit.py --unit UNIT
scripts/repo-python scripts/compare-linked-function.py --unit UNIT --json
scripts/repo-python scripts/replay-exact-units.py --unit UNIT --json
```

The selector is validated in build loading, direct comparison, and replay
loading (and again before grouping/building selected replay units). Owner
visibility does not change producer grouping: static and public units sharing
the same fixed graph still share one cold build. The standalone linked-function
inventory remains public-only by default.

## Tests and scope

`python scripts/test-static-owner-selection.py` exercises small synthetic PE32
and MSF7/PDB files and adversarial ownership/identity fixtures without a native
build or target executable. A tiny metadata fixture preserves four real MAP
rows and their PDB contributions with provenance; it is not a binary dump or
an exactness result. CI runs the tests alongside existing LTCG identity tests.

Tool support alone grants no exact credit. No canonical unit or ledger is
added by this change. Native cold replay, complete field/referent review, and
independent unchanged Factory-verifier compatibility and acceptance remain
separate requirements. The tests do not establish runtime correctness or
Factory support for this selector.

## Integration observations

Pinned repository execution passed all50 static-owner tests, nine existing entry
tests and full target-independent CI with unchanged1126 canonical units. The
legacy and new default-public readers returned identical complete dictionaries
for the retained real effect PE/MAP/PDB:997 public-owned functions and79 unowned
contributions, canonical JSON SHA-256
`481cf53afad9fd1e1d976cea4cac0e23c7a5688e26c78237b0699193fb8dc554`.

A separate ignored warm manifest selected the four natural static draw callbacks
from that already built image. The maintained comparator reported full-owner
address-field equality for all four. This is warm-artifact integration evidence,
not a cold canonical replay, independent Factory acceptance or new exact credit.
Production source, existing canonical recipes, ledgers and trusted Factory policy
were unchanged by this tooling checkpoint.
