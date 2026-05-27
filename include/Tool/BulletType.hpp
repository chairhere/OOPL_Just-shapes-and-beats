//
// Created by jerry on 2026/4/10.
//

#ifndef JUST_SHAPES_AND_BEATS_BULLETTYPE_HPP
#define JUST_SHAPES_AND_BEATS_BULLETTYPE_HPP

enum class BulletType {
    /*
     * RotatingRectangle
     *必要資訊:StartBeat、StartPos
     */
    RotatingRectangle = 1,
    /*
     * Laser
     * 必要資訊:StartBeat、Offset、StartRotation
     */
    Laser,
    /*
     * EasingBall
     * 必要資訊:需透過其他障礙生成，目前無法自行生成
     */
    EasingBall,
    /*
     * EffectBall
     * 此為特效，目前無法自行生成
     */
    EffectBall,
    /*
     * ExpendingBall
     * 必要資訊:StartBeat
     * 可選資訊(預設值):SpawnBeat(StartBeat + 4)、EndBeat(SpawnBeat + 4.5)、StartPos(Random)、Scale(450.0)
     */
    ExpendingBall,
    /*
     * SpikeBall
     * 必要資訊:StartBeat
     * 可選資訊(預設值):SpawnBeat(StarBeat + 2.0)、EndBeat(StarBeat + 2.0)、AngularVelocity(3.14)、Velocity(EasingBall.Velocity = 450.0)
     * 特殊資訊:SpawnPos和EndPos都需寫入才能修改，否則隨機
     */
    SpikeBall,
    /*
     * BiggerSpikeBall
     * 必要資訊:StartBeat
     * 此為特殊障礙，大多數值皆以調整，目前無法修改
     */
    BiggerSpikeBall,
    /*
     * SpawnerTriangle
     * 必要資訊:StartBeat、PausePos
     */
    SpawnerTriangle,
    /*
     * BiggerLaser
     * 必要資訊:StartBeat
     * 可選資訊(預設值):SpawnBeat(StartBeat + 3.0)、EndBeat(SpawnBeat + 2.5)、Scale(150.0)
     * 特殊資訊:Offset和StartRotation都需寫入才能修改，否則隨機
     */
    BiggerLaser,
    /*
     * WarningBiggerLaser
     * 此為特效，目前無法自行生成
     */
    WarningBiggerLaser,
    /*
     * WarningExpendingBall
     * 此為特效，目前無法自行生成
     */
    WarningExpendingBall,
    /*
     * SpawnerRotatingRectangle
     * 必要資訊:StartBeat、EndBeat
     */
    SpawnerRotatingRectangle,
    /*
     * CheckPointLine
     * 必要資訊:StartBeat
     */
    CheckPointLine,
    /*
     * PopRectangle
     * 必要資訊:StartBeat、StartPos、Scale
     */
    PopRectangle,
    /*
     *
     */
    SpawnerRectangle,
    /*
     *
     */
    SpawnerLinearRectangle,
    /*
     *
     */
    SpawnerExpendingBall,
    /*
     *
     */
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
        case BulletType::SpawnerRotatingRectangle:
            return 12;
        case BulletType::CheckPointLine:
            return 13;
        case BulletType::PopRectangle:
            return 14;
        case BulletType::SpawnerRectangle:
            return 15;
        case BulletType::SpawnerLinearRectangle:
            return 16;
        case BulletType::SpawnerExpendingBall:
            return 17;
        default:
            return 0;
    }
}

#endif //JUST_SHAPES_AND_BEATS_BULLETTYPE_HPP