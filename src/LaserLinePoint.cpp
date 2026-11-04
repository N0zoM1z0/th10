#include "EnemyLaserLine.hpp"
#include <math.h>

// Original TH10 JP 1.00a @0x0041E4D0 (226 bytes). The true vtable
// 0x0046DA60 slot +0x20 independently binds this to EnemyLaserLineView.
// Rotate a 3D point into the beam's local XY plane and test the full beam
// length/half-width including the caller-specified circle radius.
int EnemyLaserLineView::CheckPoint(EnemyLaserVectorView *point, float radius)
{
    volatile EnemyLaserVectorView relative;
    relative.x = point->x - position.x;
    relative.y = point->y - position.y;
    relative.z = point->z - position.z;

    const double sine = sin(-angle);
    const double cosine = cos(-angle);
    const float localX = static_cast<float>(
        relative.x * cosine - relative.y * sine);
    const float localY = static_cast<float>(
        relative.x * sine + relative.y * cosine);

    const float forwardNear = localX - radius;
    const float transverseNear = localY - radius;
    const float forwardFar = localX + radius;
    const float transverseFar = localY + radius;
    // Native FCOMP/FNSTSW accepts unordered inputs at each of these four
    // comparisons. Negated strict rejects preserve that native edge case.
    if (!(forwardNear > length) &&
        !(transverseNear > width * 0.5f) &&
        !(forwardFar < 0.0f) &&
        !(transverseFar < width * -0.5f))
        return 2;
    return 0;
}
