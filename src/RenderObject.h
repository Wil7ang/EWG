/*
 * src/RenderObject.h
 *
 * Copyright (C) 2011 Wilson Tang.
 *
 * This file is part of Extravagant Word Game and is free software, distributed,
 * and licensed under the GNU General Public License, version 3.
 *
 * You should have received a copy of the GNU General Public License.
 * If not, see <http://www.gnu.org/licenses/gpl-3.0.html>.
*/

#ifndef RENDEROBJECT_H
#define RENDEROBJECT_H

#include "SDL/SDL.h"

//abstract RenderObject class
class RenderObject
{
	public:
		RenderObject()
		{
			visibility = true;
		}
		virtual ~RenderObject() { }

		virtual void SetPosition( int xPos, int yPos )
		{
			position.x = xPos - position.w / 2;
			position.y = yPos - position.h / 2;
		}
		virtual void Render( SDL_Surface *surface ) { }
		virtual void SetVisible( bool in )
		{
			visibility = in;
		}
		virtual bool GetVisible()
		{
			return visibility;
		}

		SDL_Rect ReturnSDLRect()
		{
			return blitRect;
		}

		SDL_Rect position;
		SDL_Rect blitRect;
		bool visibility;
};

#endif

