/*
 * src/Board.h
 *
 * Copyright (C) 2011 Wilson Tang.
 *
 * This file is part of Extravagant Word Game and is free software, distributed,
 * and licensed under the GNU General Public License, version 3.
 *
 * You should have received a copy of the GNU General Public License.
 * If not, see <http://www.gnu.org/licenses/gpl-3.0.html>.
*/

#ifndef BOARD_H
#define BOARD_H

#include "Rack.h"
#include "RenderObject.h"

class Board
{
    public:
        Board();
        ~Board();

        Tile GetTile(coordPos pos);
        int GetPositionModifier(coordPos pos);
        void SetPositionModifier(coordPos pos, int in);

        /*void FlagModifier(coordPos pos);
        void UnflagAllModifiers();
        void UnflagModifier(coordPos pos);

        void DeletePositionModifier(coordPos pos);
        void DeleteFlaggedModifiers();*/

        void PlaceTile(Tile inTile, coordPos pos);
        void RemoveTile(coordPos pos);
    private:
        Tile boardTiles[15][15];
        int modifiers[15][15]; //0 = nothing, 1 = 2L, 2 = 3L, 3 = 2W, 4 = 3W
        //bool flaggedModifiers[15][15];
        //Tile temporaryBoard[15][15];
};

#endif
