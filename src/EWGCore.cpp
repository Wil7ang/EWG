/*
 * src/EWGCore.cpp
 *
 * Copyright (C) 2011 Wilson Tang.
 *
 * This file is part of Extravagant Word Game and is free software, distributed,
 * and licensed under the GNU General Public License, version 3.
 *
 * You should have received a copy of the GNU General Public License.
 * If not, see <http://www.gnu.org/licenses/gpl-3.0.html>.
*/

#include "EWGCore.h"
#include <cmath>
#include <fstream>
#include "SDL/SDL_gfxPrimitives.h"
#include "SDLFont.h"

using namespace std;

coordPos BoardPositionToRealPosition(coordPos in)
{
    if(in.x >= 0 && in.y >= 0 && in.x <= 14 && in.y <= 14)
    {
        return coordPos(256+in.x*49+in.x*2.5+49/2, 16+in.y*49+in.y*2.5+49/2);
    }
    else if(in.x < 0 && in.y < 0 && in.x == in.y)
    {
        return coordPos(1133+49/2, 223+49/2+(-in.y-1)*51);
    }
    else
        return coordPos(-50, -50);
}

coordPos RealPositionToBoardPosition(coordPos in)
{
    float xPos = 0.0f;
    float yPos = 0.0f;

    in.x -= 256;
    in.y -= 16;

    if(in.x < 0)
        in.x = 0;
    if(in.y < 0)
        in.y = 0;

    if(in.y > 768)
        in.y = 767;

    if(in.x >= 768 && in.x < 876)
    {
        in.x = 767;
    }
    else if(in.x >= 876)
    {
        yPos = floor(float(in.y)/51.25);
        yPos -= 3.0f;
        if(yPos < 1.0f)
            yPos = 1.0f;
        if(yPos > 7.0f)
            yPos = 7.0f;

        yPos = -yPos;
        xPos = yPos;
    }

    if(in.x <768)
    {
        xPos = floor(float(in.x)/51.25);
        yPos = floor(float(in.y)/51.25);
    }


    /*if(floor(xPos) > 14.0f || floor(yPos) > 14.0f )
        return coordPos())*/
    return coordPos(xPos, yPos);
}

EWGCore::EWGCore()
{
    currentPlayer = 0;
    turn = 0;
    round = 0;
    gameStarted = false;
    invalidPlacementCounter = 0;
    validPlacementCounter = 0;
    displayKeyboard = false;

    boardImage.ImageLoad("data/Board.png");
    boardImage.SetPosition( 1280 / 2, 800 / 2 );

    string lettersList[27] = { "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "BLANK" };

    for(int i = 0; i < 27; i++)
    {
        string filepath = "data/Tiles/";
        filepath += lettersList[i];
        filepath += ".jpg";
        tilesImages[i].ImageLoad(filepath);
    }

    gameNotStarted.ImageLoad("data/NotStartedOverlay.png");
    gameNotStarted.SetPosition(1280/2, 800/2);
    acceptedHighlight.ImageLoad("data/AcceptedOverlay.png");
    acceptedHighlight.SetPosition(1280/2, 800/2);
    newTileHighlight.ImageLoad("data/TileNew.png");
    blankTileHighlight.ImageLoad("data/TileBlank.png");

    clearButton.ImageLoad("data/Clear.png");
    clearButton.SetPosition(1155, 168);
    passButton.ImageLoad("data/Pass.png");
    passButton.SetPosition(1155, 168-55);
    submitButton.ImageLoad("data/Submit.png");
    submitButton.SetPosition(1155, 660);
    returnButton.ImageLoad("data/Return.png");
    returnButton.SetPosition(1155, 168-2*55);
    playerOnline.ImageLoad("data/Online.png");
    turnHighlight.ImageLoad("data/TurnHighlight.png");

    invalidPlacementOverlay.ImageLoad("data/InvalidOverlay.png");
    invalidPlacementOverlay.SetPosition(1280/2, 800/2);


    for(int i = 0; i<4;i++)
    {
        guids[i] = "";
    }
}

EWGCore::~EWGCore()
{

}

void EWGCore::AddTile(Tile tle, int origID)
{
    if(boardRack.GetSize()<7)
    {
        boardRack.AddTile(tle);
        originalRackIDs.push_back(origID);
    }
}
/*
void EWGCore::MoveTile(int x, int y, int id)
{
    if(board.GetTile(coordPos(x,y)).letter == -1)
    {
        for(int i = 0;i<7;i++)
        {
            if(id != i)
            {
                if(boardRack.GetTilePosition(i).x != x && boardRack.GetTilePosition(i).y != y)
                {
                    boardRack.SetTilePosition(coordPos(x,y), id);
                    break;
                }
            }
        }
    }
}*/

void EWGCore::MoveTile(coordPos pos, int id)
{
    if(board.GetTile(pos).letter == -1 || ( pos.x < 0 && pos.y <0 ) )
    {
        bool onTop = false;
        for(int i = 0;i<7;i++)
        {
            if(id != i)
            {
                if(boardRack.GetTilePosition(i).x == pos.x && boardRack.GetTilePosition(i).y == pos.y)
                {
                    onTop = true;
                    break;
                }
            }
        }
        if(!onTop)
            boardRack.SetTilePosition(pos, id);
    }
}

void EWGCore::DefineBlankTile(int ID, coordPos pos)
{
    int x = pos.x - 5;
    int y = pos.y - 5;
    int letter = y*5+x;
    if(letter>25)
        letter = 26;

    boardRack.SetTile(ID, Tile(letter, 0));
    displayKeyboard = false;
}

int EWGCore::GetTileIDAtPos(coordPos pos)
{
    for(int i = 0; i<7; i++)
    {
        if(boardRack.GetTilePosition(i).x == pos.x && boardRack.GetTilePosition(i).y == pos.y)
        {
            return i;
        }
    }
    return -1;
}

Tile EWGCore::GetRackTile(int id)
{
    return boardRack.GetTile(id);
}

coordPos EWGCore::GetRackTilePosition(int id)
{
    return boardRack.GetTilePosition(id);
}

bool EWGCore::CheckValidPlacement()
{
    //Check if there are any tiles palced
    int newPlacedTiles = 0;
    int row[7] = { -1, -1, -1, -1, -1, -1, -1};
    int column[7] = { -1, -1, -1, -1, -1, -1, -1};
    for(int i = 0;i<7;i++)
    {
        row[i] = boardRack.GetTilePosition(i).y;
        column[i] = boardRack.GetTilePosition(i).x;

        if(row[i] > -1 && column[i] > -1)
            newPlacedTiles++;
    }

    if(newPlacedTiles == 0)
        return false;

    if(newPlacedTiles == 1 && turn < 1)
        return false;

    //Check if there is a tile in the center in the first turn
    if(turn == 0)
    {
        bool onCenter = false;
        for(int i = 0; i<7;i++)
        {
            if((boardRack.GetTilePosition(i).x > -1 && boardRack.GetTilePosition(i).y > -1) && (boardRack.GetTilePosition(i).x == 7 && boardRack.GetTilePosition(i).y == 7))
            {
                onCenter = true;
                break;
            }
        }
        if(!onCenter)
            return false;
    }

    bool valid = false;

    //Check if there is an old tile next to the newly placed tiles
    if(turn > 0)
    {
        for(int i = 0;i<7;i++)
        {
            if(boardRack.GetTilePosition(i).x > -1 && boardRack.GetTilePosition(i).y > -1)
            {
                int xPos = boardRack.GetTilePosition(i).x;
                int yPos = boardRack.GetTilePosition(i).y;

                if(xPos > -1 && yPos > -1)
                {
                    if(board.GetTile(coordPos(xPos-1, yPos)).letter != -1 || board.GetTile(coordPos(xPos+1, yPos)).letter != -1 || board.GetTile(coordPos(xPos, yPos+1)).letter != -1 || board.GetTile(coordPos(xPos, yPos-1)).letter != -1 )
                    {
                        valid = true;
                        break;
                    }
                }
            }
        }
        if(!valid)
            return false;
    }

    valid = true;

    int inRow = 1; //-1 Neither, 1 inRow, 2 inColumn
    int cur = -1;
    int lowest = 15;

    //Check if all tiles are in the same column or row
    for(int i = 0; i < 7; i++)
    {
        if(row[i] > -1)
        {
            if(column[i] < lowest)
                lowest = column[i];
            for(int g = 0; g<7; g++)
            {
                if(row[g] > -1)
                {
                    if(row[i] != row[g])
                    {
                        valid = false;
                        break;
                    }
                }
            }
            cur = row[i];
        }
        if(!valid)
            break;
    }
    if(!valid)
    {
        lowest = 15;
        valid = true;
        for(int i = 0; i < 7; i++)
        {
            if(column[i] > -1)
            {
                if(row[i] < lowest)
                    lowest = row[i];
                for(int g = 0; g<7; g++)
                {
                    if(column[g] > -1)
                    {
                        if(column[i] != column[g])
                        {
                            valid = false;
                            break;
                        }
                    }
                }
                cur = column[i];
            }
            if(!valid)
            {
                break;
            }
            else
                inRow = 2;
        }
    }
    if(!valid)
        return false;

    if(newPlacedTiles == 1)
        inRow = -1;

    if(inRow == 1)
    {
        int newTileCount = 1;
        for(int i = lowest; i < 15; i++)
        {
            bool nextExists = false;
            for(int g = 0;g<7;g++)
            {
                if(column[g] == i+1)
                {
                    nextExists = true;
                    newTileCount++;
                    break;
                }
            }
            if(!nextExists)
            {
                if(board.GetTile(coordPos(i+1, cur)).letter != -1)
                {
                    nextExists = true;
                }
                else if(board.GetTile(coordPos(i+1, cur)).letter == -1)
                {
                    nextExists = false;
                    break;
                }
            }
        }
        if(newTileCount != newPlacedTiles)
        {
            valid = false;
        }
    }
    else if(inRow == 2)
    {
        int newTileCount = 1;
        for(int i = lowest; i < 15; i++)
        {
            bool nextExists = false;
            for(int g = 0;g<7;g++)
            {
                if(row[g] == i+1)
                {
                    nextExists = true;
                    newTileCount++;
                    break;
                }
            }
            if(!nextExists)
            {
                if(board.GetTile(coordPos(cur, i+1)).letter != -1)
                {
                    nextExists = true;
                }
                else if(board.GetTile(coordPos(cur, i+1)).letter == -1)
                {
                    nextExists = false;
                    break;
                }
            }
        }
        if(newTileCount != newPlacedTiles)
        {
            valid = false;
        }


    }

    vector<pair<string, int> > newWordList = FindNewWords(inRow, lowest, cur, newPlacedTiles);

    if(valid)
    {
        for(unsigned int i = 0; i < newWordList.size();i++)
        {
            if(!CheckWord(newWordList[i].first))
            {
                valid = false;
                break;
            }
        }
    }
    if(!valid)
        return false;

    int currentPlayerID = -1;

    for(int i = 0; i<4;i++)
    {
        if(players[i].GetGuid() == guids[currentPlayer])
        {
            currentPlayerID = i;
            break;
        }
    }

    int scoreAdd = 0;

    for(unsigned int i = 0; i < newWordList.size();i++)
    {
        playedWords.push_back(pair<string, int>(newWordList[i].first, newWordList[i].second));
        scoreAdd += newWordList[i].second;
    }

    for(int i = 0; i<7; i++)
    {
        if(boardRack.GetTilePosition(i).x > -1 && boardRack.GetTilePosition(i).y > -1)
        {
            board.PlaceTile(boardRack.GetTile(i), boardRack.GetTilePosition(i));
            players[currentPlayerID].RemoveRackTile(originalRackIDs[i]);
        }
        boardRack.RemoveTile(i);
    }

    players[currentPlayerID].ResetRack();
    players[currentPlayerID].AddScore(scoreAdd);
    if(players[currentPlayerID].GetRackSize() == 0)
    {
        players[currentPlayerID].AddScore(50);
    }

    originalRackIDs.clear();

    if(!valid)
        return false;
    else
        return true;


    return false;
}

int EWGCore::LoadDictionary(string filename)
{
    int words = 0;
    ifstream dictionaryFile(filename.c_str());
    string inWord = "";
    if(dictionaryFile.is_open())
    {
        while(dictionaryFile.good())
        {
            getline(dictionaryFile, inWord);
            dictionaryWordList.push_back(inWord);
            words++;
        }
    }
    dictionaryFile.close();

    return words;
}

void EWGCore::DisplayInvalidPlacement()
{
    invalidPlacementCounter = 1;
}

void EWGCore::DisplayValidPlacement()
{
    validPlacementCounter = 1;
}

vector<pair<string, int> > EWGCore::FindNewWords(int inRow, int lowest, int cur, int placedTiles)
{
    vector<pair<string, int> > newWordList;

    string letterList[26] = { "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z" };

    int row[7] = { -1, -1, -1, -1, -1, -1, -1};
    int column[7] = { -1, -1, -1, -1, -1, -1, -1};

    for(int i = 0;i<7;i++)
    {
        row[i] = boardRack.GetTilePosition(i).y;
        column[i] = boardRack.GetTilePosition(i).x;
    }

    if(inRow == 1)
    {
        string currentWord = "";
        int wordValue = 0;
        int wordModifier = 0; //Reserved for 1x, 2x, 3x word
        if(lowest != 0)
        {
            for(int i = lowest; i >= 0; i--)
            {
                if(board.GetTile(coordPos(i-1, cur)).letter == -1)
                {
                    lowest = i;
                    break;
                }
            }
        }
        for(int i = lowest; i < 15; i++)
        {
            bool foundTile = false;
            for(int g = 0;g<7;g++)
            {
                if(column[g] == i)
                {

                    foundTile = true;
                    currentWord += letterList[boardRack.GetTile(g).letter];

                    //Get the value of the letter plus its modifiers
                    int letterValue = boardRack.GetTile(g).value;
                    if(board.GetPositionModifier(coordPos(i, cur)) >= 3 )
                    {
                        wordModifier = board.GetPositionModifier(coordPos(i, cur));
                        //board.FlagModifier(coordPos(i, cur));
                    }

                    if(board.GetPositionModifier(coordPos(i, cur)) == 2)
                    {
                        letterValue *= 3;
                        //board.FlagModifier(coordPos(i, cur));
                    }
                    if(board.GetPositionModifier(coordPos(i, cur)) == 1)
                    {
                        letterValue *= 2;
                        //board.FlagModifier(coordPos(i, cur));
                    }

                    wordValue += letterValue;

                    break;
                }
            }
            if(!foundTile)
            {
                if(board.GetTile(coordPos(i, cur)).letter != -1)
                {
                    foundTile = true;
                    currentWord += letterList[board.GetTile(coordPos(i, cur)).letter];

                    //Get the value of the letter
                    wordValue += board.GetTile(coordPos(i, cur)).value;
                }
                else if(board.GetTile(coordPos(i, cur)).letter == -1)
                {
                    foundTile = false;
                    break;
                }
            }
        }
        if(currentWord.length()>1)
        {
            if(wordModifier == 4)
                wordValue *= 3;
            else if(wordModifier == 3)
                wordValue *= 2;

            newWordList.push_back(pair<string, int>(currentWord, wordValue));
        }

        for(int i = 0;i<7;i++)
        {
            if(column[i] > -1)
            {
                wordValue = 0;
                wordModifier = 0;
                string currentWordPerp = "";
                int highest = cur;

                if(highest != 0)
                {
                    for(int g = highest; g > 0; g--)
                    {
                        if(board.GetTile(coordPos(column[i], g-1)).letter == -1)
                        {
                            highest = g;
                            break;
                        }
                    }
                }
                for(int g = highest; g < 15; g++)
                {
                    bool foundTile = false;
                    if(g == cur)
                    {
                        foundTile = true;
                        currentWordPerp += letterList[boardRack.GetTile(i).letter];

                        //Get the value of the letter plus its modifiers
                        int letterValue = boardRack.GetTile(i).value;
                        if(board.GetPositionModifier(coordPos(column[i], g)) >= 3 )
                        {
                            wordModifier = board.GetPositionModifier(coordPos(column[i], g));
                            //board.FlagModifier(coordPos(column[i], g));
                        }

                        if(board.GetPositionModifier(coordPos(column[i], g)) == 2)
                        {
                            letterValue *= 3;
                            //board.FlagModifier(coordPos(column[i], g));
                        }
                        if(board.GetPositionModifier(coordPos(column[i], g)) == 1)
                        {
                            letterValue *= 2;
                            //board.FlagModifier(coordPos(column[i], g));
                        }

                        wordValue += letterValue;
                    }
                    if(!foundTile)
                    {
                        if(board.GetTile(coordPos(column[i], g)).letter != -1)
                        {
                            foundTile = true;
                            currentWordPerp += letterList[board.GetTile(coordPos(column[i], g)).letter];

                            //Get the value of the letter plus
                            wordValue += board.GetTile(coordPos(column[i], g)).value;
                        }
                        else if(board.GetTile(coordPos(column[i], g)).letter == -1)
                        {
                            foundTile = false;
                            break;
                        }
                    }
                }
                if(currentWordPerp.length()>1)
                {
                    if(wordModifier == 4)
                        wordValue *= 3;
                    else if(wordModifier == 3)
                        wordValue *= 2;

                    newWordList.push_back(pair<string, int>(currentWordPerp, wordValue));
                }
            }
        }
    }
    else if(inRow == 2)
    {
        int wordValue = 0;
        int wordModifier = 0; //Reserved for 1x, 2x, 3x word
        string currentWord = "";
        if(lowest != 0)
        {
            for(int i = lowest; i >= 0; i--)
            {
                if(board.GetTile(coordPos(cur, i-1)).letter == -1)
                {
                    lowest = i;
                    break;
                }
            }
        }
        for(int i = lowest; i < 15; i++)
        {
            bool foundTile = false;
            for(int g = 0;g<7;g++)
            {
                if(row[g] == i)
                {
                    foundTile = true;
                    currentWord += letterList[boardRack.GetTile(g).letter];

                    //Get the value of the letter plus its modifiers
                    int letterValue = boardRack.GetTile(g).value;
                    if(board.GetPositionModifier(coordPos(cur, i)) >= 3 )
                    {
                        wordModifier = board.GetPositionModifier(coordPos(cur, i));
                        //board.FlagModifier(coordPos(cur, i));
                    }

                    if(board.GetPositionModifier(coordPos(cur, i)) == 2)
                    {
                        letterValue *= 3;
                        //board.FlagModifier(coordPos(cur, i));
                    }
                    if(board.GetPositionModifier(coordPos(cur, i)) == 1)
                    {
                        letterValue *= 2;
                        //board.FlagModifier(coordPos(cur, i));
                    }

                    wordValue += letterValue;

                    break;
                }
            }
            if(!foundTile)
            {
                if(board.GetTile(coordPos(cur, i)).letter != -1)
                {
                    foundTile = true;
                    currentWord += letterList[board.GetTile(coordPos(cur, i)).letter];

                    //Get the value of the letter
                    wordValue += board.GetTile(coordPos(cur, i)).value;
                }
                else if(board.GetTile(coordPos(cur, i)).letter == -1)
                {
                    foundTile = false;
                    break;
                }
            }
        }
        if(currentWord.length()>1)
        {
            if(wordModifier == 4)
                wordValue *= 3;
            else if(wordModifier == 3)
                wordValue *= 2;

            newWordList.push_back(pair<string, int>(currentWord, wordValue));
        }

        for(int i = 0;i<7;i++)
        {
            if(row[i] > -1)
            {
                wordValue = 0;
                wordModifier = 0;
                string currentWordPerp = "";
                int highest = cur;

                if(highest != 0)
                {
                    for(int g = highest; g > 0; g--)
                    {
                        if(board.GetTile(coordPos(g-1, row[i])).letter == -1)
                        {
                            highest = g;
                            break;
                        }
                    }
                }
                for(int g = highest; g < 15; g++)
                {
                    bool foundTile = false;
                    if(g == cur)
                    {
                        foundTile = true;
                        currentWordPerp += letterList[boardRack.GetTile(i).letter];

                        //Get the value of the letter plus its modifiers
                        int letterValue = boardRack.GetTile(i).value;
                        if(board.GetPositionModifier(coordPos(g, row[i])) >= 3 )
                        {
                            wordModifier = board.GetPositionModifier(coordPos(g, row[i]));
                            //board.FlagModifier(coordPos(g, row[i]));
                        }

                        if(board.GetPositionModifier(coordPos(g, row[i])) == 2)
                        {
                            letterValue *= 3;
                            //board.FlagModifier(coordPos(g, row[i]));
                        }
                        if(board.GetPositionModifier(coordPos(g, row[i])) == 1)
                        {
                            letterValue *= 2;
                            //board.FlagModifier(coordPos(g, row[i]));
                        }

                        wordValue += letterValue;
                    }
                    if(!foundTile)
                    {
                        if(board.GetTile(coordPos(g, row[i])).letter != -1)
                        {
                            foundTile = true;
                            currentWordPerp += letterList[board.GetTile(coordPos(g, row[i])).letter];

                            //Get the value of the letter plus
                            wordValue += board.GetTile(coordPos(g, row[i])).value;
                        }
                        else if(board.GetTile(coordPos(g, row[i])).letter == -1)
                        {
                            foundTile = false;
                            break;
                        }
                    }
                }
                if(currentWordPerp.length()>1)
                {
                    if(wordModifier == 4)
                        wordValue *= 3;
                    else if(wordModifier == 3)
                        wordValue *= 2;

                    newWordList.push_back(pair<string, int>(currentWordPerp, wordValue));
                }
            }
        }
    }
    else if(inRow == -1)
    {
        //TODO: Fix me!
        //For lamers who place only a single tile
        //cur is now the row number, lowest is the column number
        //Do horizontal search first
        int wordValue = 0;
        int wordModifier = 0; //Reserved for 1x, 2x, 3x word
        int origColumn = lowest;
        int origRow = cur;
        string currentWord = "";
        if(lowest != 0)
        {
            for(int i = lowest; i >= 0; i--)
            {
                if(board.GetTile(coordPos(i-1, cur)).letter == -1)
                {
                    lowest = i;
                    break;
                }
            }
        }
        for(int i = lowest; i < 15; i++)
        {
            bool foundTile = false;
            for(int g = 0;g<7;g++)
            {
                if(column[g] == i)
                {
                    foundTile = true;
                    currentWord += letterList[boardRack.GetTile(g).letter];

                    //Get the value of the letter plus its modifiers
                    int letterValue = boardRack.GetTile(g).value;
                    if(board.GetPositionModifier(coordPos(i, cur)) >= 3 )
                    {
                        wordModifier = board.GetPositionModifier(coordPos(i, cur));
                        //board.FlagModifier(coordPos(i, cur));
                    }

                    if(board.GetPositionModifier(coordPos(i, cur)) == 2)
                    {
                        letterValue *= 3;
                        //board.FlagModifier(coordPos(i, cur));
                    }
                    if(board.GetPositionModifier(coordPos(i, cur)) == 1)
                    {
                        letterValue *= 2;
                        //board.FlagModifier(coordPos(i, cur));
                    }

                    wordValue += letterValue;

                    break;
                }
            }
            if(!foundTile)
            {
                if(board.GetTile(coordPos(i, cur)).letter != -1)
                {
                    foundTile = true;
                    currentWord += letterList[board.GetTile(coordPos(i, cur)).letter];

                    //Get the value of the letter
                    wordValue += board.GetTile(coordPos(i, cur)).value;
                }
                else if(board.GetTile(coordPos(i, cur)).letter == -1)
                {
                    foundTile = false;
                    break;
                }
            }
        }
        if(currentWord.length()>1)
        {
            if(wordModifier == 4)
                wordValue *= 3;
            else if(wordModifier == 3)
                wordValue *= 2;

            newWordList.push_back(pair<string, int>(currentWord, wordValue));
        }

        currentWord.clear();
        wordValue = 0;
        wordModifier = 0; //Reserved for 1x, 2x, 3x word

        //Now do vertical search
        lowest = origRow;
        cur = origColumn;
        currentWord = "";
        if(lowest != 0)
        {
            for(int i = lowest; i > 0; i--)
            {
                if(board.GetTile(coordPos(cur, i-1)).letter == -1)
                {
                    lowest = i;
                    break;
                }
            }
        }

        for(int i = lowest; i < 15; i++)
        {
            bool foundTile = false;
            for(int g = 0;g<7;g++)
            {
                if(row[g] == i)
                {
                    foundTile = true;
                    currentWord += letterList[boardRack.GetTile(g).letter];

                    //Get the value of the letter plus its modifiers
                    int letterValue = boardRack.GetTile(g).value;
                    if(board.GetPositionModifier(coordPos(cur, i)) >= 3 )
                    {
                        wordModifier = board.GetPositionModifier(coordPos(cur, i));
                        //board.FlagModifier(coordPos(cur, i));
                    }

                    if(board.GetPositionModifier(coordPos(cur, i)) == 2)
                    {
                        letterValue *= 3;
                        //board.FlagModifier(coordPos(cur, i));
                    }
                    if(board.GetPositionModifier(coordPos(cur, i)) == 1)
                    {
                        letterValue *= 2;
                        //board.FlagModifier(coordPos(cur, i));
                    }

                    wordValue += letterValue;


                    break;
                }
            }
            if(!foundTile)
            {
                if(board.GetTile(coordPos(cur, i)).letter != -1)
                {
                    foundTile = true;
                    currentWord += letterList[board.GetTile(coordPos(cur, i)).letter];

                    //Get the value of the letter
                    wordValue += board.GetTile(coordPos(cur, i)).value;

                }
                else if(board.GetTile(coordPos(cur, i)).letter == -1)
                {
                    foundTile = false;
                    break;
                }
            }
        }
        if(currentWord.length()>1)
        {
            if(wordModifier == 4)
                wordValue *= 3;
            else if(wordModifier == 3)
                wordValue *= 2;

            newWordList.push_back(pair<string, int>(currentWord, wordValue));
        }
    }

    return newWordList;
}

bool EWGCore::CheckWord(string word)
{
    for(unsigned int i = 0; i<dictionaryWordList.size();i++)
    {
        if(dictionaryWordList[i] == word)
        {
            return true;
        }
    }
    return false;
}

void EWGCore::SubmitTiles()
{
    for(int i = 0; i<7;i++)
    {
        int x = boardRack.GetTilePosition(i).x;
        int y = boardRack.GetTilePosition(i).y;
        if(x >= 0 && y >= 0)
        {

        }
    }
}

void EWGCore::Render(SDL_Surface *screen)
{
    boardImage.Render(screen);
    clearButton.Render(screen);
    submitButton.Render(screen);
    returnButton.Render(screen);
    passButton.Render(screen);

    if(!displayKeyboard && gameStarted)
    {
        for(int i = 0; i < 7; i++)
        {
            if(boardRack.GetTile(i).letter != -1)
            {
                tilesImages[boardRack.GetTile(i).letter].SetPosition(BoardPositionToRealPosition(boardRack.GetTilePosition(i)).x, BoardPositionToRealPosition(boardRack.GetTilePosition(i)).y);
                tilesImages[boardRack.GetTile(i).letter].Render(screen);
                if(boardRack.GetTilePosition(i).x > -1 && boardRack.GetTilePosition(i).y > -1)
                {
                    newTileHighlight.SetPosition(BoardPositionToRealPosition(boardRack.GetTilePosition(i)).x, BoardPositionToRealPosition(boardRack.GetTilePosition(i)).y);
                    newTileHighlight.Render(screen);
                }
                if(boardRack.GetTilePosition(i).x > -1 && boardRack.GetTilePosition(i).y > -1 && boardRack.GetTile(i).value == 0)
                {
                    blankTileHighlight.SetPosition(BoardPositionToRealPosition(boardRack.GetTilePosition(i)).x, BoardPositionToRealPosition(boardRack.GetTilePosition(i)).y);
                    blankTileHighlight.Render(screen);
                }
            }
        }

        for(int i = 0; i<26; i++)
        {
            for(int g = 0;g<15;g++)
            {
                for(int f = 0;f<15;f++)
                {
                    if(board.GetTile(coordPos(g,f)).letter == i && board.GetTile(coordPos(g,f)).value != 0)
                    {
                        tilesImages[i].SetPosition(BoardPositionToRealPosition(coordPos(g,f)).x, BoardPositionToRealPosition(coordPos(g,f)).y);
                        tilesImages[i].Render(screen);
                    }

                    if(board.GetTile(coordPos(g,f)).letter == i && board.GetTile(coordPos(g,f)).value == 0)
                    {
                        tilesImages[i].SetPosition(BoardPositionToRealPosition(coordPos(g,f)).x, BoardPositionToRealPosition(coordPos(g,f)).y);
                        tilesImages[i].Render(screen);
                        blankTileHighlight.SetPosition(BoardPositionToRealPosition(coordPos(g,f)).x, BoardPositionToRealPosition(coordPos(g,f)).y);
                        blankTileHighlight.Render(screen);
                    }
                }
            }
        }

        if(invalidPlacementCounter > 0)
        {
            invalidPlacementOverlay.Render(screen);
            invalidPlacementCounter++;
        }

        if(validPlacementCounter > 0)
        {
            acceptedHighlight.Render(screen);
            validPlacementCounter++;
        }

        if(invalidPlacementCounter > 30)
        {
            invalidPlacementCounter = 0;
        }

        if(validPlacementCounter > 30)
        {
            validPlacementCounter = 0;
        }

    }
    else if(displayKeyboard)
    {
        int col = 5;
        int row = 5;
        for(int i = 0; i<26;i++)
        {
            if(i<25)
            {
                tilesImages[i].SetPosition(BoardPositionToRealPosition(coordPos(col,row)).x, BoardPositionToRealPosition(coordPos(col,row)).y);
                tilesImages[i].Render(screen);
            }
            else
            {
                tilesImages[i].SetPosition(BoardPositionToRealPosition(coordPos(7,row)).x, BoardPositionToRealPosition(coordPos(7,row)).y);
                tilesImages[i].Render(screen);
            }
            col++;
            if(col>9)
            {
                col = 5;
                row++;
            }
        }
    }
//gfxPrimitivesSetFont(&SDL_gfx_font_6x9_fnt,6,9);
    for(int i = 0; i<4;i++)
    {
        Player *player = GetPlayer(guids[i]);
        if(player!=NULL)
        {
            gfxPrimitivesSetFont(&SDL_gfx_font_10x20_fnt,10,20);

            //Should probably use a switch here.. oh well
            if(i==0)
            {
                stringRGBA(screen, 10, 773, IntToString(player->GetScore()).c_str(),255, 255, 255, 255);
                playerOnline.SetPosition(97, 792);
                playerOnline.Render(screen);
                if(currentPlayer == 0)
                {
                    turnHighlight.SetPosition(97, 661);
                    turnHighlight.Render(screen);
                }
            }
            if(i==1)
            {
                stringRGBA(screen, 175, 773, IntToString(player->GetScore()).c_str(),255, 255, 255, 255);
                playerOnline.SetPosition(149, 792);
                playerOnline.Render(screen);
                if(currentPlayer == 1)
                {
                    turnHighlight.SetPosition(149, 661);
                    turnHighlight.Render(screen);
                }
            }
            if(i==2)
            {
                stringRGBA(screen, 10, 11, IntToString(player->GetScore()).c_str(),255, 255, 255, 255);
                playerOnline.SetPosition(97, 6);
                playerOnline.Render(screen);
                if(currentPlayer == 2)
                {
                    turnHighlight.SetPosition(97, 148);
                    turnHighlight.Render(screen);
                }
            }
            if(i==3)
            {
                stringRGBA(screen, 175, 11, IntToString(player->GetScore()).c_str(),255, 255, 255, 255);
                playerOnline.SetPosition(149, 6);
                playerOnline.Render(screen);
                if(currentPlayer == 3)
                {
                    turnHighlight.SetPosition(149, 148);
                    turnHighlight.Render(screen);
                }
            }
        }
    }

    string messageBuffer = "Tiles Left: " + IntToString(tilePile.GetTilesLeft());
    gfxPrimitivesSetFont(&SDL_gfx_font_6x9_fnt,6,9);
    stringRGBA(screen, 79, 280, messageBuffer.c_str(),255, 255, 255, 255);
    messageBuffer.clear();
    messageBuffer = "Round: " + IntToString(round+1);
    stringRGBA(screen, 79, 280+9+2, messageBuffer.c_str(),255, 255, 255, 255);
    messageBuffer.clear();
    messageBuffer = "Turn: " + IntToString(turn+1);
    stringRGBA(screen, 79, 280+2*9+4, messageBuffer.c_str(),255, 255, 255, 255);
    messageBuffer.clear();
    messageBuffer = "Last words: " + IntToString(playedWords.size());
    stringRGBA(screen, 79, 280+3*9+3*2, messageBuffer.c_str(),255, 255, 255, 255);
    thickLineRGBA(screen, 79, 280+3*9+3*2+9+3, 166, 280+3*9+3*2+9+3, 2, 255, 255, 255, 255);

    gfxPrimitivesSetFont(&SDL_gfx_font_5x8_fnt,5,8);

    //Render the last 20 words
    int sp = 0;
    for(unsigned int i = playedWords.size();i>0;i--)
    {
        messageBuffer.clear();
        messageBuffer = playedWords[i-1].first + " - " + IntToString(playedWords[i-1].second);
        stringRGBA(screen, 73, 12+317+sp*8+sp*2, messageBuffer.c_str(),255, 255, 255, 255);
        sp++;

        if(sp>19)
            break;
    }


    if(!gameStarted)
    gameNotStarted.Render(screen);


}

bool EWGCore::IsStarted()
{
    return gameStarted;
}

void EWGCore::AddPlayer(string guid)
{
    if(!IsStarted())
    {
        players.push_back(Player());
        players[players.size()-1].SetGuid(guid);
    }
}

void EWGCore::SetGuid(string guid, int id)
{
    guids[id] = guid;
}

Player *EWGCore::GetPlayer(string guid)
{
    for(int i = 0;i<GetNumberOfPlayers();i++)
    {
        if(guid == players[i].GetGuid())
        {
            return &players[i];
        }
    }

    return NULL;
}

bool EWGCore::IsTurn(string guid)
{
    int playerNumber = -1;

    for(int i = 0;i<4;i++)
    {
        if(guid == guids[i])
        {
            playerNumber = i;
            break;
        }
    }

    if(playerNumber == currentPlayer)
        return true;
    else
        return false;
}

void EWGCore::StartGame()
{
    if(!IsStarted())
    {
        gameStarted = true;
        for(unsigned int i = 0;i<players.size();i++)
        {
            for(int g = 0;g<7;g++)
            {
                if(players[i].GetRackSize() < 7)
                {
                    players[i].AddTile(tilePile.GetRandomTile());
                }
            }
        }
    }
}

void EWGCore::AdvanceTurn()
{
    int currentPlayerID = -1;

    for(unsigned int i = 0; i<players.size();i++)
    {
        if(players[i].GetGuid() == guids[currentPlayer])
        {
            currentPlayerID = i;
            break;
        }
    }

    while(players[currentPlayerID].GetRackSize() < 7)
    {
        players[currentPlayerID].AddTile(tilePile.GetRandomTile());
    }

    turn++;
    currentPlayer++;
    if(currentPlayer == GetNumberOfPlayers())
    {
        currentPlayer = 0;
        round++;
    }
}

void EWGCore::Pass()
{
    if(turn > 0)
        turn++;

    ReturnTiles();
    currentPlayer++;
    if(currentPlayer == GetNumberOfPlayers())
    {
        currentPlayer = 0;
        round++;
    }
}

void EWGCore::ClearBoard()
{
    for(int i = 0; i<7; i++)
    {
        boardRack.SetTilePosition(coordPos(-(i+1),-(i+1)), i);
        if(boardRack.GetTile(i).letter != 26 && boardRack.GetTile(i).value == 0 && boardRack.GetTile(i).letter != -1 )
        {
            boardRack.SetTile(i, Tile(26, 0));
        }
    }
}

int EWGCore::GetNumberOfPlayers()
{
    return players.size();
}

void EWGCore::ReturnTiles()
{
    int currentPlayerID = -1;

    for(int i = 0; i<4;i++)
    {
        if(players[i].GetGuid() == guids[currentPlayer])
        {
            currentPlayerID = i;
            break;
        }
    }

    for(int i = 0;i<7;i++)
    {
        boardRack.RemoveTile(i);
    }

    players[currentPlayerID].ResetRack();
}

void EWGCore::DisplayKeyboard()
{
    displayKeyboard = true;
}

void EWGCore::ResetBlankTile(int ID)
{
    boardRack.SetTile(ID, Tile(26, 0));
}
