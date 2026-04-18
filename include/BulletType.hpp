//
// Created by jerry on 2026/4/10.
//

#ifndef JUST_SHAPES_AND_BEATS_BULLETTYPE_HPP
#define JUST_SHAPES_AND_BEATS_BULLETTYPE_HPP

enum class BulletType { RotatingRectangle = 1, Laser, EasingBall, EffectBall, SpikeBall, SpawnerTriangle, BiggerLaser};

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
        case BulletType::SpikeBall:
            return 5;
        case BulletType::SpawnerTriangle:
            return 6;
        case BulletType::BiggerLaser:
            return 7;
        default:
            return 0;
    }
}

#endif //JUST_SHAPES_AND_BEATS_BULLETTYPE_HPP