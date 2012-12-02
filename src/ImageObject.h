/*
 * src/ImageObject.h
 *
 * Copyright (C) 2011 Wilson Tang.
 *
 * This file is part of Extravagant Word Game and is free software, distributed,
 * and licensed under the GNU General Public License, version 3.
 *
 * You should have received a copy of the GNU General Public License.
 * If not, see <http://www.gnu.org/licenses/gpl-3.0.html>.
*/

#ifndef IMAGEOBJECT_H
#define IMAGEOBJECT_H

#include <string>

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_rotozoom.h"
#include "RenderObject.h"

using std::string;

class ImageObject : public RenderObject
{
	public:
		ImageObject();
		ImageObject(string FILENAME);
		~ImageObject();

		int ImageLoad(string FILENAME, bool alpha = true);
		void SetAngle(float angle);
		void SetScale(float sca);
		void Render(SDL_Surface *surface);
		SDL_Surface *GetImageSurface();
		SDL_Surface *GetProcessedImageSurface();
	private:
		SDL_Surface *imageSurface;
		SDL_Surface *processedSurface;
		float ang;
		float scale;
};

#endif

