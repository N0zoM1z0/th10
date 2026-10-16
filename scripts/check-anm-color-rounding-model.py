#!/usr/bin/env python3
"""Conditional native-x87 Hermite witness. Only stdlib exact rational arithmetic.

RN-even, normal finite values only (all values here are normal or zero).
PC applies to arithmetic; m32 stores round to 24 bits without modifying ST(0).
Target scale verified: load integer channel, multiply m32 coefficient under PC,
then __ftol2 conversion with no intervening binary32 product reload.
Pause-linked, ExecuteScript-entry baseline and order-variant contexts are separate.
IMPORTANT: candidate calls are modeled as binding the actual target __ftol2.
The fresh probe's 0x40E148 is a link-anchors.obj diagnostic external anchor,
not a CRT implementation. This script does NOT model executable probe behavior.
Direct PE reads verify target 0x470AFC/0x470BD4, baseline and order
0x40D1E4/0x40D20C, and pause 0x41B840/0x41B870 constants are 1/3.
"""
from fractions import Fraction as F


def check(condition, label):
    if not condition:
        raise RuntimeError(label)


def pow2(e):
    return F(2 ** e) if e >= 0 else F(1, 2 ** -e)


def rn(x, p):
    """Round exact rational to p significant binary bits, nearest/even."""
    x = F(x)
    if not x:
        return x
    sign = -1 if x < 0 else 1
    x = abs(x)
    e = x.numerator.bit_length() - x.denominator.bit_length()
    if x < pow2(e):
        e -= 1
    step = pow2(e - p + 1)
    y = x / step
    n, rem = divmod(y.numerator, y.denominator)
    if 2 * rem > y.denominator or (2 * rem == y.denominator and n % 2):
        n += 1
    return sign * n * step


def ftol2_witness(x, pc):
    """Relevant finite/range-safe __ftol2 path under RN-even.

    Duplicate ST(0), FST m32 (sign only), FISTP i64 under current RC,
    FILD i64, subtract rounded integer from original value, then correct
    toward zero. The m32 sign store does not replace the original operand.
    """
    n, rem = divmod(x.numerator, x.denominator)
    if 2 * rem > x.denominator or (2 * rem == x.denominator and n % 2):
        n += 1
    if n == 0:
        return 0
    residual = rn(x - n, pc)
    if x >= 0 and residual < 0:
        n -= 1
    elif x < 0 and residual > 0:
        n += 1
    check(n == int(x), "__ftol2 witness must equal truncation")
    return n


def evaluate(pc, context, elapsed=1, duration=3, initial=27):
    op = lambda x: rn(x, pc)
    store = lambda x: rn(x, 24)
    q = op(F(elapsed, duration))  # FDIVR, at evaluation point
    t = store(q)    # FST m32 does NOT replace q in ST(0)
    if context == 'target':
        twice = store(op(q + q))  # 4417AD / 4417AF
        final_basis = store(op(op(op(3 - twice) * t) * t))
        # First scale call then reload t: 4417CD / 4417D2 / 4417E2
        minus_one = store(op(t - 1))
        initial_basis = store(op(op(op(twice + 1) * minus_one) * minus_one))
    elif context == 'pause':
        minus_one = store(op(q - 1))  # 409FD9 / 409FDF, q still extended
        final_basis = store(op(op(op(op(3 - t) - t) * t) * t))
        # After first scale call, reload m32 t; no m32 store of twice.
        twice = op(t + t)
        initial_basis = store(op(op(op(twice + 1) * minus_one) * minus_one))
    elif context == 'baseline':
        # 401EFC onward: q and q-1 stay extended; initial/final coefficients
        # are stored m32 only after all their arithmetic. Scale is inlined.
        t = q
        minus_one = op(q - 1)
        twice = op(q + q)
        initial_basis = store(op(op(op(twice + 1) * minus_one) * minus_one))
        final_basis = store(op(op(op(op(3 - q) - q) * q) * q))
    elif context == 'variant':
        # 401EFF..401F0D: q, twice, final basis remain extended.
        # 401F39/3F stores q-1 as m32 after final-channel conversions.
        # 401F43/49/4D forms initial basis, kept extended for channel FMUL.
        t = q
        twice = op(q + q)
        final_basis = op(op(op(3 - twice) * q) * q)
        minus_one = store(op(q - 1))
        initial_basis = op(op(op(twice + 1) * minus_one) * minus_one)
    else:
        raise ValueError(context)
    # Zero final and tangent channels give zero independently of coefficients.
    product = op(F(initial) * initial_basis)
    return dict(q=q, t=t, twice=twice, minus_one=minus_one,
                initial_basis=initial_basis, final_basis=final_basis,
                product=product, output=ftol2_witness(product, pc))


if __name__ == '__main__':
    # Independent tie-even sanity checks (positive and negative).
    check(rn(F(5, 4), 2) == 1, "positive even tie")
    check(rn(F(7, 4), 2) == 2, "positive odd tie")
    check(rn(F(-5, 4), 2) == -1, "negative tie")
    expected = {24: (19, 19, 19, 19), 53: (19, 20, 19, 20), 64: (19, 20, 19, 20)}
    contexts = ('target', 'pause', 'baseline', 'variant')
    for pc in (24, 53, 64):
        results = [evaluate(pc, context) for context in contexts]
        check(tuple(r['output'] for r in results) == expected[pc], f'1/3 witness PC{pc}')
        for name, r in zip(contexts, results):
            print(f'1/3 initial27 PC{pc} {name}: output={r["output"]}, '
                  f'coefficient={r["initial_basis"]}, product={r["product"]}')

    expected_45 = {24: (12, 12, 12, 12), 53: (12, 13, 13, 13), 64: (12, 13, 13, 13)}
    for pc in (24, 53, 64):
        results = [evaluate(pc, c, 4, 5, 125) for c in contexts]
        check(tuple(r['output'] for r in results) == expected_45[pc], f'4/5 witness PC{pc}')
        for context, r in zip(contexts, results):
            print(f'4/5 initial125 PC{pc} {context}: output={r["output"]}, '
                  f'coefficient={r["initial_basis"]}, product={r["product"]}')

    # Optional bounded search: stop at first differing initial-channel output
    # independently for each PC. No remote execution or native binary needed.
    import sys
    if '--scan' in sys.argv:
        for pc in (53, 64):
            count, found = 0, False
            for duration in range(2, 33):
                for elapsed in range(1, duration):
                    a = evaluate(pc, 'target', elapsed, duration)
                    b = evaluate(pc, 'baseline', elapsed, duration)
                    for initial in range(1, 256):
                        count += 1
                        pa = rn(initial * a['initial_basis'], pc)
                        pb = rn(initial * b['initial_basis'], pc)
                        oa, ob = ftol2_witness(pa, pc), ftol2_witness(pb, pc)
                        if oa != ob:
                            print(f'SCAN PC{pc}: cases={count}, elapsed={elapsed}, '
                                  f'duration={duration}, initial={initial}, '
                                  f'target={oa}, baseline={ob}')
                            print(f'  target coefficient={a["initial_basis"]}, product={pa}')
                            print(f'  baseline coefficient={b["initial_basis"]}, product={pb}')
                            check((count, elapsed, duration, initial, oa, ob) == (2420, 4, 5, 125, 12, 13), f"bounded scan PC{pc}")
                            found = True
                            break
                    if found:
                        break
                if found:
                    break
            check(found, f'bounded scan unexpectedly found no witness at PC{pc}')
