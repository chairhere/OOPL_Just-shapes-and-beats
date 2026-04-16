//
// Created by jerry on 2026/4/10.
//

#ifndef JUST_SHAPES_AND_BEATS_BULLETTYPE_HPP
#define JUST_SHAPES_AND_BEATS_BULLETTYPE_HPP

enum class BulletType { RotatingRectangle = 1, Laser, Circle, Spike, Triangle};

constexpr int  to_int(const BulletType S) {
    switch (S) {
        case BulletType::RotatingRectangle:
            return 1;
        case BulletType::Laser:
            return 2;
        case BulletType::Circle:
            return 3;
        case BulletType::Spike:
            return 4;
        case BulletType::Triangle:
            return 5;
        default:
            return 0;
    }
}

#endif //JUST_SHAPES_AND_BEATS_BULLETTYPE_HPP