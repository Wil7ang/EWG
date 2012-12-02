/*
 * src/EWGCore.h
 *
 * Copyright (C) 2011 Wilson Tang.
 *
 * This file is part of Extravagant Word Game and is free software, distributed,
 * and licensed under the GNU General Public License, version 3.
 *
 * You should have received a copy of the GNU General Public License.
 * If not, see <http://www.gnu.org/licenses/gpl-3.0.html>.
*/

#ifndef EWGCORE_H
#define EWGCORE_H

#include "Player.h"
#include "RenderObject.h"
#include "ImageObject.h"
#include <vector>
#include "Board.h"

using namespace std;

coordPos BoardPositionToRealPosition(coordPos in);
coordPos RealPositionToBoardPosition(coordPos in);

class EWGCore : public RenderObject
{
    public:
        EWGCore();
        ~EWGCore();

        void StartGame();

        //Board rack functions
        void ClearRack();
        void AddTile(Tile tle, int origID);
        void ReturnTiles();
        //void MoveTile(int x, int y, int id);
        void MoveTile(coordPos pos, int id);
        int GetTileIDAtPos(coordPos pos);
        Tile GetRackTile(int id);
        coordPos GetRackTilePosition(int id);

        bool CheckValidPlacement();
        void SubmitTiles();
        void CheckWords();

        void ClearBoard();
        void Pass();

        void Logic();

        void Render(SDL_Surface *screen);

        int LoadDictionary(string filename);
        bool CheckWord(string word);
        vector<pair<string, int> > FindNewWords(int inRow, int lowest, int cur, int placedTiles);

        bool IsStarted();

        void AddPlayer(string guid);
        Player *GetPlayer(string guid);
        bool IsTurn(string guid);
        void SetGuid(string guid, int id);

        void DisplayKeyboard();
        void DefineBlankTile(int ID, coordPos pos);
        void ResetBlankTile(int ID);
        void AdvanceTurn();

        void DisplayInvalidPlacement();
        void DisplayValidPlacement();

        int GetNumberOfPlayers();
    private:
        vector<Player> players;
        vector<string> dictionaryWordList;
        vector<pair<string, int> > playedWords;

        string guids[4];

        int currentPlayer;
        bool gameStarted;
        Rack boardRack;
        TilePile tilePile;

        //Images
        ImageObject boardImage;
        ImageObject tilesImages[27];
        ImageObject newTileHighlight;
        ImageObject blankTileHighlight;
        ImageObject gameNotStarted;
        ImageObject invalidPlacementOverlay;
        ImageObject clearButton;
        ImageObject submitButton;
        ImageObject returnButton;
        ImageObject passButton;
        ImageObject playerOnline;
        ImageObject turnHighlight;
        ImageObject acceptedHighlight;

        Board board;

        int turn;
        int round;

        int invalidPlacementCounter;
        int validPlacementCounter;

        vector<int> originalRackIDs;

        bool displayKeyboard;
};

#endif
