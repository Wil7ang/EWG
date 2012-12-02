/*
 * src/Rack.cpp
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

Rack::Rack()
{
    full = false;
    for(int i = 0; i<7; i++)
    {
        tiles[i] = Tile(-1, 0);
        positions[i] = coordPos(-8, -8);
        placed[i] = false;
    }
}

Rack::~Rack()
{

}

void Rack::AddTile(Tile tile)
{
    if(!full)
    {
        for(int i = 0; i < 7; i++)
        {
            if(tiles[i].letter == -1)
            {
                int position = 0;
                for(int g = 0; g<7;g++)
                {
                    bool found = false;
                    for(int f = 0;f<7;f++)
                    {
                        if(positions[f].x == -(g+1))
                        {
                            position++;
                            found=true;
                            break;
                        }
                    }
                    if(!found)
                        break;
                }
                tiles[i] = tile;
                positions[i] = coordPos(-(position+1), -(position+1));


                placed[i] = false;
                break;
            }
        }

        if(GetSize() == 7)
            full = true;
    }
}

Tile Rack::GetTile(int id)
{
    return tiles[id];
}

void Rack::RemoveTile(int id)
{
    tiles[id] = Tile(-1, 0);
    positions[id] = coordPos(-8, -8);
    placed[id] = false;
    full = false;
}

int Rack::GetSize()
{
    int g = 0;
    for(int i = 0; i < 7;i++)
    {
        if(tiles[i].letter != -1)
            g++;
    }
    return g;
}

void Rack::SetTilePosition(coordPos pos, int id)
{
    positions[id] = pos;
}

coordPos Rack::GetTilePosition(int id)
{
    return positions[id];
}

bool Rack::IsPlaced(int id)
{
    return placed[id];
}

void Rack::SetPlaced(int id, bool state)
{
    placed[id] = state;
}

void Rack::ResetRack()
{
    for(int i =0;i<7;i++)
    {
        SetPlaced(i, false);
    }
}

void Rack::SetTile(int id, Tile newTile)
{
    tiles[id] = newTile;
}
