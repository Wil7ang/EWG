/*
 * src/Rack.h
 *
 * Copyright (C) 2011 Wilson Tang.
 *
 * This file is part of Extravagant Word Game and is free software, distributed,
 * and licensed under the GNU General Public License, version 3.
 *
 * You should have received a copy of the GNU General Public License.
 * If not, see <http://www.gnu.org/licenses/gpl-3.0.html>.
*/

#ifndef RACK_H
#define RACK_H

#include <vector>
#include <stdlib.h>
#include <time.h>

#include "SDL/SDL.h"

#include <fstream>

#include "Utility.h"

using namespace std;

struct Tile
{
    public:
        Tile() { letter = -1; value = 0; visible = true; }
        Tile(int in, int val) { letter = in; value = val; visible = true; }
        ~Tile() { }

        int letter;
        int value;
        bool visible;
};

class TilePile
{
    public:
        TilePile()
        {
            //TODO: Read file to set tile counts
            instockLetters.push_back(pair<int, int>(0, 9));
            instockLetters.push_back(pair<int, int>(1, 2));
            instockLetters.push_back(pair<int, int>(2, 2));
            instockLetters.push_back(pair<int, int>(3, 4));
            instockLetters.push_back(pair<int, int>(4, 12));
            instockLetters.push_back(pair<int, int>(5, 2));
            instockLetters.push_back(pair<int, int>(6, 3));
            instockLetters.push_back(pair<int, int>(7, 2));
            instockLetters.push_back(pair<int, int>(8, 9));
            instockLetters.push_back(pair<int, int>(9, 1));
            instockLetters.push_back(pair<int, int>(10, 1));
            instockLetters.push_back(pair<int, int>(11, 4));
            instockLetters.push_back(pair<int, int>(12, 2));
            instockLetters.push_back(pair<int, int>(13, 6));
            instockLetters.push_back(pair<int, int>(14, 8));
            instockLetters.push_back(pair<int, int>(15, 2));
            instockLetters.push_back(pair<int, int>(16, 1));
            instockLetters.push_back(pair<int, int>(17, 6));
            instockLetters.push_back(pair<int, int>(18, 4));
            instockLetters.push_back(pair<int, int>(19, 6));
            instockLetters.push_back(pair<int, int>(20, 4));
            instockLetters.push_back(pair<int, int>(21, 2));
            instockLetters.push_back(pair<int, int>(22, 2));
            instockLetters.push_back(pair<int, int>(23, 1));
            instockLetters.push_back(pair<int, int>(24, 2));
            instockLetters.push_back(pair<int, int>(25, 1));
            instockLetters.push_back(pair<int, int>(26, 2));
            totalTiles = 100;
            values[0] = 1;
            values[1] = 3;
            values[2] = 3;
            values[3] = 2;
            values[4] = 1;
            values[5] = 4;
            values[6] = 2;
            values[7] = 4;
            values[8] = 1;
            values[9] = 8;
            values[10] = 5;
            values[11] = 1;
            values[12] = 3;
            values[13] = 1;
            values[14] = 1;
            values[15] = 3;
            values[16] = 10;
            values[17] = 1;
            values[18] = 1;
            values[19] = 1;
            values[20] = 1;
            values[21] = 4;
            values[22] = 4;
            values[23] = 8;
            values[24] = 4;
            values[25] = 10;
            values[26] = 0;


            srand(time(NULL));

            for(int i = 0; i<100;i++)
            {
                pile.push_back(GenerateRandomTile());
            }
            totalTiles = 100;
        }
        ~TilePile() { }

        Tile GenerateRandomTile()
        {
            while(totalTiles > 0)
            {
                int character = rand() % instockLetters.size() + 0;
                if(instockLetters[character].second > 0)
                {

                    totalTiles--;
                    instockLetters[character].second--;
                    int letter = instockLetters[character].first;
                    if(instockLetters[character].second == 0)
                    {
                        instockLetters.erase(instockLetters.begin()+character);
                    }
                    return Tile(letter, values[letter]);
                }
            }
            return Tile(-1, 0);
        }

        Tile GetRandomTile()
        {
            if(pile.size() > 0)
            {
                int randomNumber = rand() % pile.size() + 0;
                Tile tempTile = pile[randomNumber];
                pile.erase(pile.begin()+randomNumber);
                return tempTile;
            }
            return Tile(-1, 0);
        }

        int GetTilesLeft()
        {
            return pile.size();
        }
    private:
        int values[27];
        int totalTiles;
        int totalLettersLeft;

        vector<Tile> pile;
        vector<pair<int, int> > instockLetters;
};

struct coordPos
{
    coordPos() { x = -1; y = -1; }
    coordPos(int _x, int _y) { x=_x; y=_y;}
    ~coordPos() { }
    int x;
    int y;
};

class Rack
{
    public:
        Rack();
        ~Rack();
        void AddTile(Tile newTile);
        Tile GetTile(int id);
        void RemoveTile(int id);
        void ResetRack();
        int GetSize();

        //BoardRack only
        void SetTilePosition(coordPos position, int id);
        coordPos GetTilePosition(int id);
        void SetTile(int id, Tile newTile);

        bool IsPlaced(int id);
        void SetPlaced(int id, bool state);
    private:
        Tile tiles[7];
        coordPos positions[7];
        bool full;
        int placed[7];

};

#endif
