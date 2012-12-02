/*
 * src/main.cpp
 *
 * Copyright (C) 2011 Wilson Tang.
 *
 * This file is part of Extravagant Word Game and is free software, distributed,
 * and licensed under the GNU General Public License, version 3.
 *
 * You should have received a copy of the GNU General Public License.
 * If not, see <http://www.gnu.org/licenses/gpl-3.0.html>.
*/

#include "RenderingCore.h"
#include "ImageObject.h"
#include <cstdio>
#include "NetworkCore.h"
#include "EWGCore.h"

void InputProcess( SDL_Event *event, int *x, int *y, int *xrel, int *yrel, int *mState )
{
    if(*mState == 1)
        *mState = 2;
    if(*mState == -1)
        *mState = 0;
	while( SDL_PollEvent( event ) )
		{
			if ( event->type == SDL_MOUSEBUTTONUP && *mState == 2)
				*mState = -1;
			else if( event->type == SDL_MOUSEBUTTONDOWN && *mState == 0)
				*mState = 1;


			if( event->type == SDL_MOUSEMOTION )
				{
					*x = event->motion.x;
					*y = event->motion.y;
				}
		}
}

bool CheckMouseInBox(int uX, int uY, int bX, int bY, int mX, int mY)
{
    if(mX >= uX && mX <= bX && mY >= uY && mY <= bY)
    {
        return true;
    }
    return false;
}

int main( int argc, char *argv[] )
{
	RenderingCore rCore;
	rCore.InitializeScreen();

	EWGCore eCore;

    eCore.LoadDictionary("ospd4.txt");

	rCore.AddRenderObject( &eCore );

	bool quit = false;

	int mouseX = 0;
	int mouseY = 0;
	int Xrel = 0;
	int Yrel = 0;
	int mouseState = 0;

	SDL_Event event;

	NetworkCore nCore( 8080 );

    int ID = -1;

    bool isBlankTile = false;
    bool showKeyboard = false;

	while( !quit )
    {
        nCore.process(&eCore);
        InputProcess( &event, &mouseX, &mouseY, &Xrel, &Yrel, &mouseState );
        if( mouseState == -1 && CheckMouseInBox(0, 0, 50, 50, mouseX, mouseY))
        {
            quit = true;
        }

        if(!showKeyboard)
        {
            if( mouseState == -1 && CheckMouseInBox(1155-139/2, 660-51/2, 1155+139/2, 660+51/2, mouseX, mouseY))
            {
                if(!eCore.CheckValidPlacement())
                    eCore.DisplayInvalidPlacement();
                else
                {
                    eCore.DisplayValidPlacement();
                    eCore.AdvanceTurn();
                }
            }
            if( mouseState == -1 && CheckMouseInBox(1155-139/2, (168-2*55)-51/2, 1155+139/2, (168-2*55)+51/2, mouseX, mouseY))
            {
                eCore.ReturnTiles();
            }
            if( mouseState == -1 && CheckMouseInBox(1155-139/2, (168-55)-51/2, 1155+139/2, (168-55)+51/2, mouseX, mouseY))
            {
                eCore.Pass();
            }
            if( mouseState == -1 && CheckMouseInBox(1155-139/2, (168)-51/2, 1155+139/2, (168)+51/2, mouseX, mouseY))
            {
                eCore.ClearBoard();
            }
        }
        if( mouseState == -1 && showKeyboard && CheckMouseInBox(511, 271, 769, 580, mouseX, mouseY))
        {
            eCore.DefineBlankTile(ID, RealPositionToBoardPosition(coordPos(mouseX, mouseY)));

            showKeyboard = false;
        }
        if( mouseState == -1 && isBlankTile )
        {
            if(eCore.GetRackTilePosition(ID).x > -1 && eCore.GetRackTilePosition(ID).y > -1)
            {
                eCore.DisplayKeyboard();
                showKeyboard = true;
                isBlankTile = false;
            }
        }


        if(!eCore.IsStarted())
        {
            if( mouseState == -1 && CheckMouseInBox(50, 0, 1280, 800, mouseX, mouseY) && eCore.GetNumberOfPlayers() >= 2)
            {
                eCore.StartGame();
            }
        }

       if( mouseState == 1 && !showKeyboard)
            ID = eCore.GetTileIDAtPos(RealPositionToBoardPosition(coordPos(mouseX, mouseY)));

       if(ID != -1 && mouseState == 2 && !showKeyboard)
       {
           eCore.MoveTile( RealPositionToBoardPosition(coordPos(mouseX, mouseY)),ID);

           if(eCore.GetRackTile(ID).letter == 26 && eCore.GetRackTilePosition(ID).x > -1 && eCore.GetRackTilePosition(ID).y > -1)
            {
                isBlankTile = true;
            }

            if(eCore.GetRackTile(ID).letter != 26 && eCore.GetRackTile(ID).value == 0)
            {
                if(eCore.GetRackTilePosition(ID).x <= -1 && eCore.GetRackTilePosition(ID).y <= -1)
                {
                    eCore.ResetBlankTile(ID);
                    isBlankTile = false;
                    ID = -1;
                }
            }
       }



        rCore.ClearScene();
        rCore.Draw();
        rCore.Render();
    }
	//SDL_Delay(2000);
	return 0;
}



