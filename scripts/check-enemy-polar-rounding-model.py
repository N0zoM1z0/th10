#!/usr/bin/env python3
"""Conditional polar-angle storage witness, using exact rational arithmetic.

Models the reviewed 0x44BC70 loop and 0x40F4D9..0x40F58C operations.
RN-even stores, finite normal inputs, and x87 PC53/PC64 are assumptions.
The helper returns a live x87 result after at most 34 updates in either
single-direction example. It does not store a float32 result internally.
This is neither an executable probe nor a proof of game-input reachability.
"""

import argparse
from fractions import Fraction as F
import hashlib
from pathlib import Path
import struct


CONSTANTS = {
    0x470B18: 0x40490FDB,  # pi
    0x470B14: 0x40C90FDB,  # two pi
    0x470B94: 0x3FC90FDB,  # half pi
    0x470B10: 0xC0490FDB,  # negative pi
}
TARGET_SHA256 = "2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040"


def require(condition, message):
    if not condition:
        raise ValueError(message)


def from_bits(bits):
    return F(struct.unpack("<f", struct.pack("<I", bits))[0])


def rn(value, precision):
    """Round a normal binary rational to precision bits, nearest/even."""
    value = F(value)
    if not value:
        return value
    sign = -1 if value < 0 else 1
    value = abs(value)
    exponent = value.numerator.bit_length() - value.denominator.bit_length()
    power = lambda e: F(2) ** e
    if value < power(exponent):
        exponent -= 1
    step = power(exponent - precision + 1)
    scaled = value / step
    integer, remainder = divmod(scaled.numerator, scaled.denominator)
    if 2 * remainder > scaled.denominator or (
        2 * remainder == scaled.denominator and integer % 2
    ):
        integer += 1
    return sign * integer * step


PI = from_bits(CONSTANTS[0x470B18])
TWO_PI = from_bits(CONSTANTS[0x470B14])
HALF_PI = from_bits(CONSTANTS[0x470B94])


def wrap(value, precision):
    updates = 0
    for direction, bound in ((-1, PI), (1, -PI)):
        while value > bound if direction == -1 else value < bound:
            value = rn(value + direction * TWO_PI, precision)
            old_count = updates
            updates += 1
            if old_count > 32:
                break
    return value, updates


def witness(precision):
    angle = from_bits(0x43DF8000)  # 447, deliberately beyond the wrap cap
    initial = from_bits(0xC1575CFE)
    inner_arg = rn(rn(angle - HALF_PI, precision), 24)
    inner, inner_updates = wrap(inner_arg, precision)
    outer_arg = rn(rn(HALF_PI - inner, precision), 24)
    extended, outer_updates = wrap(outer_arg, precision)
    stored = rn(extended, 24)
    target_difference = abs(rn(initial - stored, precision))
    old_difference = abs(rn(initial - extended, precision))
    require((inner_updates, outer_updates) == (34, 34), "wrap-cap witness changed")
    require(extended == F(-17408245, 1048576), "extended final changed")
    require(stored == F(-4352061, 262144), "stored final changed")
    require(target_difference < PI < old_difference, "branch witness disappeared")
    return {
        "precision": precision,
        "extended_final": str(extended),
        "stored_final": str(stored),
        "target_adjusts": target_difference >= PI,
        "old_candidate_adjusts": old_difference >= PI,
    }


def check_target_constants():
    from linked_image import PEImage

    target = Path(__file__).resolve().parents[1] / "resources/th10.exe"
    require(hashlib.sha256(target.read_bytes()).hexdigest() == TARGET_SHA256,
            "wrong TH10 target")
    image = PEImage(target)
    for address, bits in CONSTANTS.items():
        require(image.read_address(address, 4) == struct.pack("<I", bits),
                f"target constant differs at {address:#x}")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--check-target", action="store_true")
    args = parser.parse_args()
    if args.check_target:
        check_target_constants()
    for precision in (53, 64):
        print(witness(precision))
    print("Conditional polar storage witness passes; runtime/reachability remain unknown.")


if __name__ == "__main__":
    main()
