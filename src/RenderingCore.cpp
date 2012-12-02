/*
 * src/RenderingCore.cpp
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

#include "SDL/SDL_image.h"
#include "SDL/SDL_rotozoom.h"
//#include "SDL/SDL_ttf.h"

//#include "box2d/Box2D.h"

#include <algorithm>
#include <vector>
#include <iostream>

#include "Cursor.h"

RenderingCore::RenderingCore()
{
	screenSurface = NULL;
	//updateRects = 0;

	startTime = 0.0f;
	endTime = 0.0f;

	fpsStart = 0.0f;
	fpsEnd = 0.0f;
	fps = 0;
	frameCount = -1;
}

RenderingCore::~RenderingCore()
{
	if ( screenSurface != NULL )
	{
		SDL_FreeSurface( screenSurface );
		screenSurface = NULL;
	}
}

int RenderingCore::InitializeScreen( int screenWidth, int screenHeight, int screenBits, bool fullscreen )
{
	printf( "Initializing SDL...\n" );
	if ( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
	{
		printf( "Failed to initialize SDL!\n" );
		return -1;
	}

	printf( "Starting Screen...\n" );
	if ( fullscreen )
		screenSurface = SDL_SetVideoMode( screenWidth, screenHeight, screenBits, SDL_HWSURFACE | SDL_FULLSCREEN);
	else
		screenSurface = SDL_SetVideoMode( screenWidth, screenHeight, screenBits, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_ASYNCBLIT );

	if ( screenSurface == NULL )
	{
		printf( "Failed to initialize screen!\n" );
		return -1;
	}

	/*if ( TTF_Init() == -1 )
	{
		printf( "Failed to initialize font sub-system\n" );
		return -1;
	}*/

	SDL_WM_SetCaption( "EWG-0.0.1", NULL );

	if ( !MOUSECURSOR )
		SDL_ShowCursor( false );

    SDL_SetCursor(init_system_cursor(arrow));

	//
	SDL_FillRect( screenSurface, &screenSurface->clip_rect, SDL_MapRGB( screenSurface->format, 220, 220, 220 ) );
	SDL_Flip( screenSurface );

	return 0;
}

void RenderingCore::AddRenderObject( RenderObject *obj )
{
	renderObjList.push_back( obj );
}

void RenderingCore::Draw()
{
	for ( unsigned int i = 0; i < renderObjList.size(); i++ )
	{
		renderObjList[i]->Render( screenSurface );
	}
}

void RenderingCore::ClearScene()
{
	startTime = SDL_GetTicks();
	if ( frameCount == -1 )
	{
		fpsStart = SDL_GetTicks();
		frameCount = 0;
	}
	SDL_FillRect( screenSurface, &screenSurface->clip_rect, SDL_MapRGB( screenSurface->format, 0, 0, 0 ) );
}

SDL_Surface *RenderingCore::ReturnScreenPointer()
{
	return screenSurface;
}
/*
void RenderingCore::AppendUpdateRect( SDL_Rect rect )
{
	updateRectMgr.AddUpdateRect( rect );
}

void RenderingCore::AppendUpdateRect( int x, int y, int w, int h )
{
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
	updateRectMgr.AddUpdateRect( rect );
}*/

void RenderingCore::Render()
{
	SDL_Flip( screenSurface );

	/*for ( unsigned int i = 0; i < renderObjList.size(); i++ )
	{
		updateRectMgr.AddUpdateRect( renderObjList[i]->ReturnSDLRect() );
	}*/

	endTime = SDL_GetTicks();

#ifdef DEBUGINFO
	printf( "%f\n", endTime - startTime );
#endif
	fpsEnd = SDL_GetTicks();
	if ( fpsEnd - fpsStart >= 1000 )
	{
		fps = frameCount;
		frameCount = 0;
		fpsStart = SDL_GetTicks();
	}
	else
		frameCount++;

	if ( endTime - startTime < 1000 / MAX_FPS )
	{
		//updateRectMgr.CheckUnique();
		/*vector<SDL_Rect> tVec;
		tVec.resize( updateRectMgr.GetPrevUpdateRectListSize() + updateRectMgr.GetUpdateRectListSize() );
		copy ( updateRectMgr.GetPrevUpdateRectList(), updateRectMgr.GetPrevUpdateRectList() + updateRectMgr.GetPrevUpdateRectListSize(), tVec.begin() );
		copy ( updateRectMgr.GetUpdateRectList(), updateRectMgr.GetUpdateRectList() + updateRectMgr.GetUpdateRectListSize(), tVec.begin() + updateRectMgr.GetPrevUpdateRectListSize() );
		SDL_UpdateRects( screenSurface, updateRectMgr.GetPrevUpdateRectListSize() + updateRectMgr.GetUpdateRectListSize(), &tVec[0] );
		updateRectMgr.PushUpdateRectListToPrev();*/
		SDL_Delay( ( 1000 / MAX_FPS ) - ( endTime - startTime ) );
	}
}

int RenderingCore::GetFps()
{
	return fps;
}
