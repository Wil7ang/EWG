/*
 * src/RenderingCore.h
 *
 * Copyright (C) 2011 Wilson Tang.
 *
 * This file is part of Extravagant Word Game and is free software, distributed,
 * and licensed under the GNU General Public License, version 3.
 *
 * You should have received a copy of the GNU General Public License.
 * If not, see <http://www.gnu.org/licenses/gpl-3.0.html>.
*/

#ifndef RENDERINGCORE_H
#define RENDERINGCORE_H

#include <vector>

#include "SDL/SDL.h"
#include "SDL/SDL_gfxPrimitives.h"


#include "RenderObject.h"

//#include "UpdateRectManager.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 800
#define SCREEN_BITS 32

#define MAX_FPS 60

#define FULLSCREEN 1
#define MOUSECURSOR 1

using std::vector;

enum
{
	BEFORE = 0,
	AFTER
};

class RenderingCore
{
	public:
		RenderingCore();
		~RenderingCore();

		int InitializeScreen( int screenWidth = SCREEN_WIDTH, int screenHeight = SCREEN_HEIGHT, int screenBits = SCREEN_BITS, bool fullscreen = FULLSCREEN );

		void Draw();
		void Render();
		void ClearScene();
		void AddRenderObject( RenderObject *obj );
		void RemoveRenderObject( RenderObject *obj );
		void InsertRenderObject( RenderObject *obj, int position );
		//void AppendUpdateRect( SDL_Rect rect );
		//void AppendUpdateRect( int x, int y, int w, int h );

		SDL_Surface *ReturnScreenPointer();

		int GetFps();
	private:
		SDL_Surface *screenSurface;
		SDL_Surface *windowIcon;

		vector<RenderObject*> renderObjList;
		//SDL_Rect updateRectsList[50];

		//UpdateRectManager updateRectMgr;

		//int updateRects;

		float startTime;
		float endTime;

		float fpsStart;
		float fpsEnd;
		int frameCount;
		int fps;
};

#endif
