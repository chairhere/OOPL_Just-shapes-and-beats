//
// Created by jerry on 2026/4/10.
//

#ifndef JUST_SHAPES_AND_BEATS_BULLETTYPE_HPP
#define JUST_SHAPES_AND_BEATS_BULLETTYPE_HPP

enum class BulletType {
    RotatingRectangle = 1,
    Laser,
    EasingBall,
    EffectBall,
    ExpendingBall,
    SpikeBall,
    BiggerSpikeBall,
    SpawnerTriangle,
    BiggerLaser,
    WarningBiggerLaser,
    WarningExpendingBall,
    SpawnerRectangle
};

constexpr int  to_int(const BulletType S) {
    switch (S) {
        case BulletType::RotatingRectangle:
            return 1;
        case BulletType::Laser:
            return 2;
        case BulletType::EasingBall:
            return 3;
        case BulletType::EffectBall:
            return 4;
        case BulletType::ExpendingBall:
            return 5;
        case BulletType::SpikeBall:
            return 6;
        case BulletType::BiggerSpikeBall:
            return 7;
        case BulletType::SpawnerTriangle:
            return 8;
        case BulletType::BiggerLaser:
            return 9;
        case BulletType::WarningBiggerLaser:
            return 10;
        case BulletType::WarningExpendingBall:
            return 11;
        case BulletType::SpawnerRectangle:
            return 12;
        default:
            return 0;
    }
}

#endif //JUST_SHAPES_AND_BEATS_BULLETTYPE_HPP