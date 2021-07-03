#pragma once
#include<ctime>
#include"Level.h"
#include <cstdlib>
#include <vector>
#include<stdio.h>
#include <string>
#include <stdexcept>
#include <iostream>
#include <windows.h>
#include<conio.h>
#include<ctime>
#include"Level.h"

using namespace std;




class Map {
private:
    int width, height;
    char** map;
    friend class Player;

public:

    Map(int h, int w, level lv) {
        this->width = w;
        this->height = h;

        map = new char* [h];
        for (int i = 0; i < h; i++) {
            map[i] = new char[w];
        }
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                map[i][j] = '.';
                if (j == 0 || j == w - 1) {
                    map[i][j] = '#';
                    continue;
                }
                if (i == 0 || i == h - 1) {
                    map[i][j] = '#';
                    continue;
                }
            }
        }
        srand((unsigned)time(nullptr));
        // Max inner walls: (h-2)(w-2)
//        int number_inner_walls = rand() % ((h - 2) * (w - 2) + 1);

        // create inner wall
        for (int i = 0; i < lv.number_inner_wall; i++) {
            int  y = rand() % (h - 2) + 1; // y: 1->(h-2)
            int x = rand() % (w - 2) + 1; // x: 1->(w-2)
            map[y][x] = '#';
        }

        // create monster
        for (int i = 0; i < lv.number_monster; i++) {
            int y, x;
            do {
                y = rand() % (h - 2) + 1; // y: 1->(h-2)
                x = rand() % (w - 2) + 1; // x: 1->(w-2)
            } while (map[y][x] != '.');
            map[y][x] = 'm';
        }

        // create gem
        for (int i = 0; i < lv.number_gem; i++) {
            int y, x;
            do {
                y = rand() % (h - 2) + 1; // y: 1->(h-2)
                x = rand() % (w - 2) + 1; // x: 1->(w-2)
            } while (map[y][x] != '.');
            map[y][x] = 'G';
        }


        // create apple
        for (int i = 0; i < 1; i++) {
            int y, x;
            do {
                y = rand() % (h - 2) + 1; // y: 1->(h-2)
                x = rand() % (w - 2) + 1; // x: 1->(w-2)
            } while (map[y][x] != '.');
            map[y][x] = 'A';
        }

    }
    void createGate() {
        // create gate

        for (int i = 0; i < 1; i++) {
            int y, x;
            do {
                y = rand() % (height - 2) + 1; // y: 1->(h-2)
                x = rand() % (width - 2) + 1; // x: 1->(w-2)
            } while (map[y][x] != '.');
            map[y][x] = '0';
        }
    }
    int getHeight() {
        return this->height;
    }
    int getWidth() {
        return this->width;
    }

    void set(Position pos, char c) {
        map[pos.y][pos.x] = c;
    }
    char get(int x, int y) {
        return map[y][x];
    }
    void print() {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                cout << map[i][j] << ' ';
            }
            cout << endl;
        }
    }
    ~Map() {
        this->height = 0;
        this->width = 0;
        delete (map);
    }
    //    Map operator + (const Map& v) {
    //
    //        int h = this->height + v.height;
    //        int w = this->width + v.width;
    //        Map v1(h, w);
    //        //v1.print();
    //        return v1;
    //    }
    Map operator=(const Map& v) {
        this->height = v.height;
        this->width = v.width;
        this->map = v.map;

    }

};
// gate signal: 0