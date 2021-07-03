#include <cstdlib>
#include <vector>
#include<stdio.h>
#include <string>
#include <stdexcept>
#include <iostream>
#include <windows.h>
#include<conio.h>
#include<ctime>
#include"Positions.h"
#include"Monster.h"
#include"MagicApple.h"
#include"Gem.h"
#include"Level.h"
#include"Map.h"
#pragma warning(disable : 4996)
using namespace std;
//class template to save time and movement
template <class T>
class Stack {
private:
    vector<T> elems;    // elements 

public:
    void push(T const&);  // push element 
    void pop();               // pop element 
    T top() const;            // return top element 
    int getsize();
    T printbro(int a);
    bool empty() const {      // return true if empty.
        return elems.empty();
    }
};

template <class T>
void Stack<T>::push(T const& elem) {
    // append copy of passed element 
    elems.push_back(elem);
}

template <class T>
void Stack<T>::pop() {
    if (elems.empty()) {
        throw out_of_range("Stack<>::pop(): empty stack");
    }

    // remove last element 
    elems.pop_back();
}
template <class T>
int Stack<T>::getsize() {
    return elems.size();
}
template <class T>
T Stack<T>::printbro(int a) {
    cout << elems.at(a);
    return elems.at(a);
}

template <class T>
T Stack<T>::top() const {
    if (elems.empty()) {
        throw out_of_range("Stack<>::top(): empty stack");
    }

    // return copy of last element 
    return elems.back();
}
static string getime() {
    time_t curr_time;
    curr_time = time(NULL);
    string tm = ctime(&curr_time);
    return tm;
}

static Stack<string> stringStack;
static Stack<string> stringStack2;

// contain configuration of level



class Player {
    // signal: @
private:
    static const int MAX_ENERGY = 1000;
    bool state; // true: live, false: die
    Position pos;
    int energy;
    int point;
    friend class Map;

public:
    int gemcount;
    bool finish_level = false;
    Player() {
        energy = 1000; // energy is 1000
        point = 0;
        state = true;
        // player appear randomly in map at empty place
        pos.x = 0;
        pos.y = 0;
        gemcount = 0;
    }

    int getEnergy() {
        return this->energy;
    }
    int getPoint() {
        return this->point;
    }
    bool getState() {
        return this->state;
    }
    // get information of player
    void getInfor() {
        cout << "Energy: " << getEnergy() << endl;
        cout << "Point : " << getPoint() << endl;
        cout << endl;
    }
    void getInfor2() {
        cout << "You finish this game with " << getPoint() << " points. ";
        cout << endl;
    }

    // set position of player on map
    void setPos(int x, int y, Map& m) {
        m.set(Position(x, y), '@');
        pos.x = x;
        pos.y = y;
    }
    void addPoint(int add) {
        cout << "Player get more " << add << ((add == 1) ? " point" : "points") << endl;
        point += add;
    }
    void moveDown(Map& m) {
        int x = pos.x, y = pos.y;

        if (y < m.height - 1) {
            // if there is monster: m
            if (m.get(x, y + 1) == 'm') {
                stringStack2.push("You met a monster");
                stringStack.push(getime());
                cout << "There is a monster !!!" << endl;
                int strength = Monster::strength;
                int pt = Monster::point;
                if (energy <= strength) {
                    stringStack2.push("You were killed by a monster");
                    stringStack.push(getime());
                    cout << "The player has died !!!Game Over!!!" << endl;
                    state = false;
                    return;
                }
                else {
                    stringStack2.push("You killed a monster");
                    stringStack.push(getime());
                    energy -= strength;
                    point += pt;
                    m.set(Position(x, y), '.'); // set previous pixel to '.'
                    setPos(x, y + 1, m); // update new pos of player
                }

            }
            // if there is MagicApple: A
            else if (m.get(x, y + 1) == 'A') {
                stringStack2.push("You ate an Apple");
                stringStack.push(getime());
                cout << "There is a maggicApple !!!" << endl;
                int energy = MagicApple::energy;
                this->energy += energy;
                if (this->energy > MAX_ENERGY) this->energy = MAX_ENERGY;
                m.set(Position(x, y), '.');
                setPos(x, y + 1, m);

            }
            // if there is gem: G
            else if (m.get(x, y + 1) == 'G') {
                stringStack2.push("You got a gem!");
                stringStack.push(getime());
                cout << "There is a Gem !!!" << endl;
                gemcount++;
                energy = energy - 2;
                int point = Gem::points;
                this->point += point;
                m.set(Position(x, y), '.');
                setPos(x, y + 1, m);

            }
            // if there is gate: O
            else if (m.get(x, y + 1) == '0') {
                stringStack2.push("You got to the gate and going to the next level");
                stringStack.push(getime());
                cout << "Get gate !!!Prepare to the next level ";
                finish_level = true;
                m.set(Position(x, y), '.');
            }
            // if there is nothing
            else if (m.get(x, y + 1) == '.') {

                cout << "Move down" << endl;
                energy = energy - 5;
                m.set(Position(x, y), '.');
                setPos(x, y + 1, m);
            }

            // if there is inner_wall
            else {
                cout << "There is inner_wall !!!Can't go to this direction" << endl;
            }
        }
        // meet wall
        else {
            cout << "In the edge of the map, can't go !!! " << endl;
        }
    }
    // move up
    void moveUp(Map& m) {
        int x = pos.x, y = pos.y;
        if (y >= 1) {

            // if there is monster: m
            if (m.get(x, y - 1) == 'm') {
                stringStack2.push("You fought a monster");
                stringStack.push(getime());
                cout << "There is a monster !!!" << endl;
                int strength = Monster::strength;
                int pt = Monster::point;
                if (energy <= strength) {
                    stringStack2.push("You were killed by a monster");
                    stringStack.push(getime());
                    cout << "The player has died !!! Game Over!!!";
                    state = false;
                    return;
                }
                else {
                    stringStack2.push("You killed the monster");
                    stringStack.push(getime());
                    energy -= strength;
                    point += pt;
                    m.set(Position(x, y), '.'); // set previous pixel to '.'
                    setPos(x, y - 1, m); // update new pos of player
                }

            }
            // if there is MagicApple: A
            else if (m.get(x, y - 1) == 'A') {
                stringStack2.push("You ate an Apple");
                stringStack.push(getime());
                cout << "There is a maggicApple !!!" << endl;
                int energy = MagicApple::energy;
                this->energy += energy;
                if (this->energy > MAX_ENERGY) this->energy = MAX_ENERGY;
                m.set(Position(x, y), '.'); // set previous pixel to '.'
                setPos(x, y - 1, m);

            }
            // if there is gem: G
            else if (m.get(x, y - 1) == 'G') {
                stringStack2.push("You met a Gem");
                stringStack.push(getime());
                cout << "There is a Gem !!!" << endl;
                gemcount++;
                int point = Gem::points;
                this->point += point;
                m.set(Position(x, y), '.');
                setPos(x, y - 1, m);

            }
            // if there is gate: O
            else if (m.get(x, y - 1) == '0') {
                stringStack2.push("You got to the gate,next level");
                stringStack.push(getime());
                cout << "Get gate !!! Prepare to the next level" ;
                finish_level = true;
                m.set(Position(x, y), '.');
            }
            // if there is nothing
            else if (m.get(x, y - 1) == '.') {

                cout << "Move up" << endl;
                energy = energy - 5;
                m.set(Position(x, y), '.');
                setPos(x, y - 1, m);
            }
            // if there is inner_wall
            else {
                cout << "There is inner_wall !!! Can't go to this direction " << endl;
            }
        }
        // meet wall
        else {
            cout << "In the edge of the map, can't go !!! " << endl;
        }
    }

    // move left
    void moveLeft(Map& m) {
        int x = pos.x, y = pos.y;
        if (x >= 1) {

            // if there is monster: m
            if (m.get(x - 1, y) == 'm') {
                stringStack2.push("You met a monster");
                stringStack.push(getime());
                cout << "There is a monster !!!" << endl;
                int strength = Monster::strength;
                int pt = Monster::point;
                if (energy <= strength) {
                    stringStack2.push("You were killed by a monster");
                    stringStack.push(getime());
                    cout << "The player has died !!!Game Over!!!" << endl;
                    state = false;
                    return;
                }
                else {
                    stringStack2.push("You killed a monster");
                    stringStack.push(getime());
                    energy -= strength;
                    point += pt;
                    m.set(Position(x, y), '.'); // set previous pixel to '.'
                    setPos(x - 1, y, m); // update new pos of player
                }

            }
            // if there is MagicApple: A
            else if (m.get(x - 1, y) == 'A') {
                stringStack2.push("You ate an Apple");
                stringStack.push(getime());
                cout << "There is a maggicApple !!! " << endl;
                int energy = MagicApple::energy;
                this->energy += energy;
                if (this->energy > MAX_ENERGY) this->energy = MAX_ENERGY;
                m.set(Position(x, y), '.'); // set previous pixel to '.'
                setPos(x - 1, y, m);

            }
            // if there is gem: G
            else if (m.get(x - 1, y) == 'G') {
                stringStack2.push("You got a Gem");
                stringStack.push(getime());
                cout << "There is a Gem !!!" << endl;
                gemcount++;
                int point = Gem::points;
                this->point += point;
                m.set(Position(x, y), '.');
                setPos(x - 1, y, m);

            }
            // if there is gate: O
            else if (m.get(x - 1, y) == '0') {
                stringStack2.push("You got to the Gate, you entered the next level ") ;
                stringStack.push(getime());
                cout << "Get gate !!! Prepare to the next level.   " ;
                finish_level = true;
                m.set(Position(x, y), '.');
            }
            // if there is nothing
            else if (m.get(x - 1, y) == '.') {

                cout << "Move left" << endl;
                energy = energy - 5;

                m.set(Position(x, y), '.');
                setPos(x - 1, y, m);
            }

            // if there is inner_wall
            else {
                cout << "There is inner_wall !!! Can't go to this direction " << endl ;
            }
        }
        // meet wall
        else {
            cout << "In the edge of the map, can't go !!! " << endl;
        }
    }

    // move right
    void moveRight(Map& m) {
        int x = pos.x, y = pos.y;
        if (x < m.width - 1) {

            // if there is monster: m
            if (m.get(x + 1, y) == 'm') {
                stringStack2.push("You met a Monster");
                stringStack.push(getime());
                cout << "There is a monster !!!" << endl;
                int strength = Monster::strength;
                int pt = Monster::point;
                if (energy <= strength) {
                    stringStack2.push("You were killed by the monster");
                    stringStack.push(getime());
                    cout << "The player has died !!!Game Over!!!" << endl;
                    state = false;
                    return;
                }
                else {
                    stringStack2.push("You killed a monster");
                    stringStack.push(getime());
                    energy -= strength;
                    point += pt;
                    m.set(Position(x, y), '.'); // set previous pixel to '.'
                    setPos(x + 1, y, m); // update new pos of player
                }

            }
            // if there is MagicApple: A
            else if (m.get(x + 1, y) == 'A') {
                stringStack2.push("You met an Apple");
                stringStack.push(getime());
                cout << "There is a maggicApple !!!" << endl;
                int energy = MagicApple::energy;
                this->energy += energy;
                if (this->energy > MAX_ENERGY) this->energy = MAX_ENERGY;
                m.set(Position(x, y), '.'); // set previous pixel to '.'
                setPos(x + 1, y, m);

            }
            // if there is gem: G
            else if (m.get(x + 1, y) == 'G') {
                stringStack2.push("You met a Gem");
                stringStack.push(getime());
                cout << "There is a Gem !!!" << endl;
                gemcount++;
                int point = Gem::points;
                this->point += point;
                m.set(Position(x, y), '.');
                setPos(x + 1, y, m);

            }
            // if there is gate: O
            else if (m.get(x + 1, y) == '0') {
                stringStack2.push("You met a Gate, went to next level");
                stringStack.push(getime());
                cout << "Get gate !!!Prepare to the next level" ;
                finish_level = true;
                m.set(Position(x, y), '.');
            }
            // if there is nothing
            else if (m.get(x + 1, y) == '.') {

                cout << "Move right" << endl;
                energy = energy - 5;
                m.set(Position(x, y), '.');
                setPos(x + 1, y, m);
            }

            // if there is inner_wall
            else {
                cout << "There is inner_wall !!!Can't go to this direction" << endl;
            }
        }
        // meet wall
        else {
            cout << "In the edge of the map, can't go !!! " << endl;
        }
    }

};

class Game {
public:

    level lv[10]; // 3 level easy , medium, hard
    Game(level lv1, level lv2, level lv3, level lv4, level lv5) {
        lv[0] = lv1;
        lv[1] = lv2;
        lv[2] = lv3;
        lv[3] = lv4;
        lv[4] = lv5;

    }
    void StartGame() {
        cout << "WELCOME TO TREASURE HUNT BRO!!!!!!      ";
        int cnt = 1;

        // we have 3 level
        Player pl;
        for (auto Lv : lv) {
            pl.gemcount = 0;
            pl.finish_level = false;
            cout << "Level " << cnt++ << endl;

            Map m = Map(12, 12, Lv);

            // set random position of player
            int x, y;
            int h = m.getHeight(), w = m.getWidth();
            srand((unsigned)time(nullptr));
            do {
                y = rand() % (h - 2) + 1; // y: 1->(h-2)
                x = rand() % (w - 2) + 1; // x: 1->(w-2)
            } while (m.get(x, y) != '.');
            pl.setPos(x, y, m);
            m.print();
            char action;
            do {
                if (pl.gemcount == Lv.number_gem) {
                    if (system("CLS"))system("clear");
                    
                    cout << "All gems collected!!! Open a pro vip gate" << endl;
                    pl.gemcount = 0;
                    m.createGate();
                    m.print();
                }
                //std::cin >> action;
                action = tolower(getch());
                if (system("CLS"))system("clear");
                fflush(stdin);
                switch (action) {
                case 'a':

                    pl.moveLeft(m);

                    break;
                case 's':

                    pl.moveDown(m);
                    break;
                case 'd':

                    pl.moveRight(m);
                    break;
                case 'w':

                    pl.moveUp(m);
                    break;
                case 'q':
                    cout << "Exit game";
                    break;
                default:
                    cout << "Wrong key " << endl;
                    break;
                }
                // if finish level. move to another level
                if (pl.finish_level) {
                    cout << "Finish level !!! ";

                    if (cnt == 6) {
                        pl.getInfor2();
                       
                        quitGame2();
                       
                        playagain();

                    }
                    break;
                }



                // if died. end game
                if (!pl.getState()) {
                    cout << "Lose !!!";
                    pl.getInfor2();
                    quitGame();
                    break;
                }
                if (pl.getEnergy() < 0) {
                    cout << "Run out of energy bro !!!";
                    pl.getInfor2();
                    quitGame();
                    break;
                }
                if (action != 'q') {
                    m.print();
                }
                else {
                    quitGame();


                }
                pl.getInfor();

            } while (action != 'q');


            // vt vong lap while de tao buoc di chuyen, thoat game, ...

        }

    }
    void quitGame() {
        for (int i = 0; i < stringStack.getsize(); i++) {
            cout << endl;
            stringStack.printbro(i); cout << ": "; stringStack2.printbro(i); cout << endl;
            cout << "---------------------------------------------------------------------" << endl;
        }
        playagain();
        // exit(0);
    }
    void quitGame2() {
        
        stringStack2.push("YOU WON THE WHOLE GAME:");
        stringStack.push(getime());
        cout << "OH HERO!!!! YOU CONQUERED THIS TERRIBLE GAME!!!!" << endl;
            cout << "---------------------------------------------------------------------" << endl;
            
        for (int i = 0; i < stringStack.getsize(); i++) {
            
            stringStack.printbro(i); cout << ": "; stringStack2.printbro(i); cout << endl; 
            cout << "---------------------------------------------------------------------" << endl;
        }
        playagain();
        // exit(0);
    }
    void playagain() {
        char i;
        cout << "Do you want to play again(y/n): "; cin >> i;
        if (i == 'y') {
            if (system("CLS"))system("clear");
            StartGame();
        }
        if (i == 'n') {
            exit(0);
        }

    }
};

int main() {
    //    Map test(7, 8, level(5, 2, 3));
    //    test.print();

    //    cout << endl;
    //    Map test2(3, 2);
    ////    cout << endl;
    ////    test2.print();
    //    Map test3 = test + test2;
    //    test3.print();
    //char t;
    //cout << "Press T to start: "; cin >> t; cout << endl; 
    //if (system("CLS"))system("clear");
    level lv1 = level(5, 2, 2, 2);
    level lv2 = level(5, 4, 4, 2);
    level lv3 = level(5, 7, 5, 4);
    level lv4 = level(5, 7, 6, 4);
    level lv5 = level(5, 7, 7, 4);
    Game g(lv1, lv2, lv3, lv4, lv5);
    //if (t == 't') {
    g.StartGame();
    //}

}