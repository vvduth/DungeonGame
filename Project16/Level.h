#pragma once
class level {
public:
    int number_inner_wall;
    int number_monster;
    int number_gem;
    int number_apple;
    level() {
        number_monster = 0;
        number_inner_wall = 0;
        number_gem = 0;
        number_apple = 0;
    }
    level(int number_inner_wall, int number_monster, int number_gem, int number_apple) {
        this->number_inner_wall = number_inner_wall;
        this->number_monster = number_monster;
        this->number_gem = number_gem;
        this->number_apple = number_apple;
    }
};