/*
 * src/Player.cpp
 *
 * Copyright (C) 2011 Wilson Tang.
 *
 * This file is part of Extravagant Word Game and is free software, distributed,
 * and licensed under the GNU General Public License, version 3.
 *
 * You should have received a copy of the GNU General Public License.
 * If not, see <http://www.gnu.org/licenses/gpl-3.0.html>.
*/

#include "Player.h"

Player::Player()
{
    score = 0;
    guid = "";
}

Player::~Player()
{

}

void Player::AddTile(Tile tile)
{
    playerRack.AddTile(tile);
}

int Player::GetRackSize()
{
    return playerRack.GetSize();
}

void Player::AddScore(int scr)
{
    score += scr;
}

int Player::GetScore()
{
    return score;
}

void Player::SetGuid(string in)
{
    guid = in;
}

string Player::GetGuid()
{
    return guid;
}

void Player::RemoveRackTile(int id)
{
    playerRack.RemoveTile(id);
}

Tile Player::GetTile(int id)
{
    return playerRack.GetTile(id);
}

void Player::SetPlaced(int id, bool state)
{
    playerRack.SetPlaced(id, state);
}

bool Player::IsPlaced(int id)
{
    return playerRack.IsPlaced(id);
}

Tile Player::PlaceTile(int id)
{
    playerRack.SetPlaced(id, true);
    return playerRack.GetTile(id);
}

void Player::ResetRack()
{
    playerRack.ResetRack();
}
