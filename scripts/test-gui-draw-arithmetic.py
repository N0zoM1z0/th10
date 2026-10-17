#!/usr/bin/env python3
"""Exact rational arithmetic witnesses, not target x87/control-word proof.

The target instruction order retains integer FILD and multiply/add intermediates
until an FSTP m32 boundary. This script compares that ideal retained model with
specified early binary32 stores. Neither model proves the native executable's
precision control, exceptions, denormals or compiler's expression evaluation.
"""
from fractions import Fraction
import json
import struct

def require(value, message):
    if not value:
        raise RuntimeError(message)

def value(bits):
    sign = -1 if bits >> 31 else 1
    exponent = (bits >> 23) & 255
    mantissa = bits & 0x7fffff
    require(exponent != 255, 'finite rational input required')
    if exponent:
        mantissa += 1 << 23
    shift = exponent - 150 if exponent else -149
    return sign * Fraction(mantissa) * Fraction(2) ** shift

def nearest_integer(x):
    whole, remainder = divmod(x.numerator, x.denominator)
    twice = remainder * 2
    return whole + (twice > x.denominator or
                    (twice == x.denominator and (whole & 1)))

def bits(x):
    x = Fraction(x)
    sign = 0x80000000 if x < 0 else 0
    x = abs(x)
    if not x:
        return sign
    exponent = x.numerator.bit_length() - x.denominator.bit_length()
    if x < Fraction(2) ** exponent:
        exponent -= 1
    require(-126 <= exponent <= 126, 'witness normal finite range')
    significand = nearest_integer(x / Fraction(2) ** (exponent - 23))
    if significand == 1 << 24:
        significand >>= 1
        exponent += 1
    return sign | ((exponent + 127) << 23) | (significand - (1 << 23))

def rounded(x):
    return value(bits(x))

def pair(name, first, second, expected):
    observed = (bits(first), bits(second))
    require(observed == expected, name + ': witness mismatch ' + repr(observed))
    require(observed[0] != observed[1], name + ': witness must distinguish')
    return {'case': name, 'retained_or_stored': '%08X' % observed[0],
            'early_store_or_folded': '%08X' % observed[1]}

def main():
    coefficient = value(0x3e95a95b)
    witnesses = []
    n = -14187
    witnesses.append(pair('horizontal multiply-add boundary',
        Fraction(n) * coefficient + 51,
        rounded(Fraction(n) * coefficient) + 51,
        (0xc57fff83, 0xc57fff82)))
    gauge = value(0x3de4320d)
    witnesses.append(pair('gauge multiply-add boundary',
        gauge * 342 + 41, rounded(gauge * 342) + 41,
        (0x429e36b7, 0x429e36b8)))
    gauge = value(0x3e83a077)
    witnesses.append(pair('stored right minus one versus folded endpoint',
        rounded(gauge * 342 + 41) - 1, gauge * 342 + 40,
        (0x42ffd85e, 0x42ffd85f)))
    # Search only a fixed small witness range to isolate integer conversion,
    # leaving multiply/add retention identical in the two models.
    for n in range(16777217, 16777318):
        retained = Fraction(n) * coefficient + 51
        early_integer = rounded(n) * coefficient + 51
        if bits(retained) != bits(early_integer):
            witnesses.append({'case': 'FILD exact integer versus early f32',
                'integer': n, 'integer_retained': '%08X' % bits(retained),
                'integer_rounded_first': '%08X' % bits(early_integer)})
            break
    else:
        raise RuntimeError('integer-conversion witness missing')
    print(json.dumps({'qualification': __doc__.strip(), 'witnesses': witnesses}, indent=2))

if __name__ == '__main__':
    main()
