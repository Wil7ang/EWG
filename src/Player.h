/*
 * src/Player.h
 *
 * Copyright (C) 2011 Wilson Tang.
 *
 * This file is part of Extravagant Word Game and is free software, distributed,
 * and licensed under the GNU General Public License, version 3.
 *
 * You should have received a copy of the GNU General Public License.
 * If not, see <http://www.gnu.org/licenses/gpl-3.0.html>.
*/

#include "Rack.h"
#include <string>

using namespace std;

class Player
{
    public:
        Player();
        ~Player();

        void Draw();
        Tile PlaceTile(int id);
        void SetPlaced(int id, bool state);
        bool IsPlaced(int id);
        void ResetRack();

        void AddTile(Tile tile);
        Tile GetTile(int id);
        int GetRackSize();
        void RemoveRackTile(int id);

        void AddScore(int scr);
        int GetScore();

        void SetGuid(string in);
        string GetGuid();
    private:
        int score;
        Rack playerRack;
        string guid;
};
