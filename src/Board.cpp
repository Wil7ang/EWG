/*
 * src/Board.cpp
 *
 * Copyright (C) 2011 Wilson Tang.
 *
 * This file is part of Extravagant Word Game and is free software, distributed,
 * and licensed under the GNU General Public License, version 3.
 *
 * You should have received a copy of the GNU General Public License.
 * If not, see <http://www.gnu.org/licenses/gpl-3.0.html>.
*/

#include "Board.h"

#include <fstream>
using namespace std;

Board::Board()
{
    for(int i = 0; i < 15; i++)
    {
        for(int g = 0; g < 15; g++)
        {
            boardTiles[i][g] = Tile(-1, 0);
        }
    }

    for(int i = 0; i < 15; i++)
    {
        for(int g = 0; g < 15; g++)
        {
            modifiers[i][g] = 0;
        }
    }

    //3x Word
    modifiers[0][0] = 4;
    modifiers[7][0] = 4;
    modifiers[14][0] = 4;
    modifiers[0][7] = 4;
    modifiers[7][7] = 4;
    modifiers[14][7] = 4;
    modifiers[0][14] = 4;
    modifiers[7][14] = 4;
    modifiers[14][14] = 4;

    //2x Word
    modifiers[7][7] = 3;
    for(int i = 0; i < 15; i++)
    {
        if(i!=0 && i < 5)
        {
            modifiers[i][i] = 3;
            modifiers[14-i][i] = 3;
        }

        if(i != 14 && i > 9)
        {
            modifiers[i][i] = 3;
            modifiers[14-i][i] = 3;
        }
    }

    //3x Letter
    modifiers[5][1] = 2;
    modifiers[9][1] = 2;
    modifiers[1][5] = 2;
    modifiers[5][5] = 2;
    modifiers[9][5] = 2;
    modifiers[13][5] = 2;

    modifiers[13][9] = 2;
    modifiers[9][9] = 2;
    modifiers[5][9] = 2;
    modifiers[1][9] = 2;
    modifiers[9][13] = 2;
    modifiers[5][13] = 2;

    //2x Letter
    modifiers[3][0] = 1;
    modifiers[11][0] = 1;
    modifiers[6][2] = 1;
    modifiers[8][2] = 1;
    modifiers[0][3] = 1;
    modifiers[7][3] = 1;
    modifiers[14][3] = 1;
    modifiers[2][6] = 1;
    modifiers[6][6] = 1;
    modifiers[8][6] = 1;
    modifiers[12][6] = 1;

    modifiers[3][7] = 1;
    modifiers[11][7] = 1;

    modifiers[12][8] = 1;
    modifiers[8][8] = 1;
    modifiers[6][8] = 1;
    modifiers[2][8] = 1;
    modifiers[14][11] = 1;
    modifiers[7][11] = 1;
    modifiers[0][11] = 1;
    modifiers[8][12] = 1;
    modifiers[6][12] = 1;
    modifiers[11][14] = 1;
    modifiers[3][14] = 1;
}

Board::~Board()
{

}

Tile Board::GetTile(coordPos pos)
{
    if(pos.x <15 && pos.x >= 0 && pos.y <15 && pos.y >= 0)
        return boardTiles[pos.x][pos.y];
    else
        return Tile(-1, 0);
}

void Board::PlaceTile(Tile inTile, coordPos pos)
{
    boardTiles[pos.x][pos.y] = inTile;
}

int Board::GetPositionModifier(coordPos pos)
{
    if(pos.x <15 && pos.x >= 0 && pos.y <15 && pos.y >= 0)
        return modifiers[pos.x][pos.y];
    else
        return 0;
}

void Board::SetPositionModifier(coordPos pos, int in)
{
    modifiers[pos.x][pos.y] = in;
}
/*
void Board::DeletePositionModifier(coordPos pos)
{
    modifiers[pos.x][pos.y] = 0;
}

void Board::FlagModifier(coordPos pos)
{
    flaggedModifiers[pos.x][pos.y] = true;
}

void Board::UnflagModifier(coordPos pos)
{
    flaggedModifiers[pos.x][pos.y] = false;
}

void Board::UnflagAllModifiers()
{
    for(int i = 0; i < 15; i++)
    {
        for(int g = 0; g < 15; g++)
        {
            UnflagModifier(coordPos(i, g));
        }
    }
}

void Board::DeleteFlaggedModifiers()
{
    for(int i = 0; i < 15; i++)
    {
        for(int g = 0; g < 15; g++)
        {
            if(flaggedModifiers[i][g])
                DeletePositionModifier(coordPos(i, g));
        }
    }
}*/
