"""Conditional rational model of TH10 hint proximity arithmetic.

Assumes x87 precision-control PC53 or PC64 and round-to-nearest, ties-to-even.
These finite, normal-range examples model the observed operation/store sequence;
they are not native execution, native control-word attestation, or a general
x87 emulator. Subnormal stores, overflow, NaNs, exception masks, and invalid
float-to-integer conversions are outside this small model.

All checks stay active under Python -O. Run with Python 3; no dependencies.
Target: 419650/571, TH10 Japanese 1.00a, executable SHA256
2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040.
"""
from fractions import Fraction


def round_significand(value, precision):
    """Round a rational to a binary significand, without exponent bounds."""
    if not value:
        return value
    sign = -1 if value < 0 else 1
    value = abs(value)
    exponent = value.numerator.bit_length() - value.denominator.bit_length()
    if value < Fraction(2) ** exponent:
        exponent -= 1
    step = Fraction(2) ** (exponent - precision + 1)
    quotient, remainder = divmod(value, step)
    if remainder > step / 2 or (remainder == step / 2 and quotient % 2):
        quotient += 1
    return sign * quotient * step


def alpha(px, py, cx, cy, width, sprite, scale, baseline=255,
          precision=53, alteration=None):
    """Model each arithmetic operation and observed float store separately."""
    def extended(value):
        return round_significand(value, precision)

    def single(value):
        return round_significand(value, 24)

    x = extended(px + 224)
    y = extended(py + 16)
    if alteration == 'store_adjusted_player':
        x, y = single(x), single(y)
    dx = abs(extended(x - cx))
    dy = abs(extended(y - cy))
    if alteration != 'omit_dy_store':
        dy = single(dy)
    half_width = single(extended(width / 2))
    half_height = extended(extended(sprite * scale) / 2)
    if alteration == 'store_height':
        half_height = single(half_height)
    if dx < half_width and dy < half_height:
        return (baseline & 255) >> 2

    outer_width = extended(half_width + 32)
    if alteration != 'omit_outer_width_store':
        outer_width = single(outer_width)
    # All supplied baselines are 0..255, so native signed 3*A cannot overflow.
    fade_amount = int(Fraction(3 * baseline, 4))

    def faded_alpha(distance):
        scaled = extended(extended(distance * fade_amount) / 32)
        return int(extended(baseline - scaled)) & 255

    if dx < outer_width and dy < half_height:
        return faded_alpha(extended(outer_width - dx))
    outer_height = extended(half_height + 32)
    if dx < half_width and dy < outer_height:
        return faded_alpha(extended(outer_height - dy))
    return baseline & 255


def check_equal(actual, expected, label):
    if actual != expected:
        raise RuntimeError('%s: got %r, expected %r' % (label, actual, expected))


def main():
    cases = [
        ('retained adjusted player',
         dict(px=Fraction(1, 2**20), py=-16, cx=224, cy=0,
              width=Fraction(1, 2**19), sprite=2, scale=1),
         'store_adjusted_player', (64, 63)),
        ('retained height',
         dict(px=-224, py=-15, cx=0, cy=0, width=2,
              sprite=2 + Fraction(1, 2**22), scale=1 - Fraction(1, 2**24)),
         'store_height', (63, 64)),
        ('dy float store',
         dict(px=-224, py=1, cx=0, cy=Fraction(1, 2**25),
              width=2, sprite=34, scale=1),
         'omit_dy_store', (64, 63)),
        ('outer width float store',
         dict(px=-192, py=-16, cx=0, cy=0,
              width=Fraction(1, 2**19), sprite=2, scale=1),
         'omit_outer_width_store', (255, 254)),
    ]
    for precision in (53, 64):
        for name, inputs, alteration, expected in cases:
            inputs = {key: Fraction(value) for key, value in inputs.items()}
            actual = (alpha(**inputs, precision=precision),
                      alpha(**inputs, precision=precision, alteration=alteration))
            check_equal(actual, expected, 'PC%d %s' % (precision, name))
            print('PC%d RN: %s: native-shaped=%d altered=%d' %
                  (precision, name, *actual))

    for baseline, expected in [(0, (0, 0)), (1, (0, 1)), (2, (0, 1)),
                               (3, (0, 1)), (254, (63, 64)), (255, (63, 64))]:
        interior = baseline >> 2
        boundary = baseline - int(Fraction(3 * baseline, 4))
        check_equal((interior, boundary), expected, 'boundary A=%d' % baseline)
        for precision in (53, 64):
            shared = dict(py=Fraction(-16), cx=Fraction(0), cy=Fraction(0),
                          width=Fraction(2), sprite=Fraction(2), scale=Fraction(1),
                          baseline=baseline, precision=precision)
            check_equal(alpha(px=Fraction(-224), **shared), interior,
                        'modeled interior A=%d PC%d' % (baseline, precision))
            check_equal(alpha(px=Fraction(-223), **shared), boundary,
                        'modeled boundary A=%d PC%d' % (baseline, precision))
            check_equal(alpha(px=Fraction(-191), **shared), baseline,
                        'exact outer boundary A=%d PC%d' % (baseline, precision))
        print('A=%d: strict interior=%d, inner fade boundary=%d' %
              (baseline, interior, boundary))

    # This extra witness prevents claiming that selecting double establishes
    # native semantics when the original process's control word is unknown.
    inputs = dict(px=Fraction(-223), py=Fraction(-16), cx=Fraction(1, 2**60),
                  cy=Fraction(0), width=Fraction(2), sprite=Fraction(2),
                  scale=Fraction(1))
    actual = (alpha(**inputs, precision=53), alpha(**inputs, precision=64))
    check_equal(actual, (64, 63), 'precision-control distinction')
    print('Unknown-CW witness: dx=1-2^-60 gives PC53 alpha64 vs PC64 alpha63 (RN)')


if __name__ == '__main__':
    main()
