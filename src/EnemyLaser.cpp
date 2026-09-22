struct EnemyLaserVectorView
{
    float x;
    float y;
    float z;

    void FromAngleMagnitude(float angle, float magnitude);
};

typedef char EnemyLaserVectorViewSizeIs0C[
    (sizeof(EnemyLaserVectorView) == 0x0c) ? 1 : -1];

void EnemyLaserVectorView::FromAngleMagnitude(float angle, float magnitude)
{
#if defined(_MSC_VER) && defined(_M_IX86)
    __asm
    {
        mov eax, this
        fld angle
        fsincos
        fmul magnitude
        fstp [eax]
        fmul magnitude
        fstp [eax + 4]
    }
#else
    (void)angle;
    (void)magnitude;
    x = 0.0f;
    y = 0.0f;
#endif
}
