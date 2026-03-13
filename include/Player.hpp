//
// Created by cheese on 2026/3/13.
//

#ifndef JUST_SHAPES_AND_BEATS_PLAYER_HPP
#define JUST_SHAPES_AND_BEATS_PLAYER_HPP
#include "glm/vec2.hpp"
#include "Util/GameObject.hpp"

class Player : public Util::GameObject {
public:
    Player(int index): Player_index(index) {

        //shape setting;
        switch (index) {
            case 1:
                return;
            case 2:
                return;
            case 3:
                return;
            case 4:
                return;
        }
    };

    void Moving();

    void Dash();

private:
    int Player_index = 0;
    int Max_Health = 3;
    int Health = Max_Health;
};

#endif //JUST_SHAPES_AND_BEATS_PLAYER_HPP