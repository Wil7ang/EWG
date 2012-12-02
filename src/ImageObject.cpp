/*
 * src/ImageObject.cpp
 *
 * Copyright (C) 2011 Wilson Tang.
 *
 * This file is part of Extravagant Word Game and is free software, distributed,
 * and licensed under the GNU General Public License, version 3.
 *
 * You should have received a copy of the GNU General Public License.
 * If not, see <http://www.gnu.org/licenses/gpl-3.0.html>.
*/

#include "ImageObject.h"

ImageObject::ImageObject()
{
	imageSurface = NULL;
	processedSurface = NULL;
	visibility = true;
	ang = 0.0f;
	scale = 1.0f;
}

ImageObject::ImageObject(string FILENAME)
{
	imageSurface = NULL;
	processedSurface = NULL;
	visibility = true;
	ang = 0.0f;
	scale = 1.0f;
	ImageLoad(FILENAME);
}

ImageObject::~ImageObject()
{
	if ( imageSurface != NULL )
	{
		SDL_FreeSurface( imageSurface );
		imageSurface = NULL;
	}
	if ( processedSurface != NULL )
	{
		SDL_FreeSurface( processedSurface );
		processedSurface = NULL;
	}
}

int ImageObject::ImageLoad( string FILENAME, bool alpha )
{
	SDL_Surface *temp = IMG_Load( FILENAME.c_str() );
	if(alpha)
		imageSurface = SDL_DisplayFormatAlpha( temp );
	else
		imageSurface = SDL_DisplayFormat( temp );
	SDL_FreeSurface( temp );

	if ( imageSurface == NULL )
	{
		printf( "Failed to load image: %s\n", FILENAME.c_str() );
		return -1;
	}

	SetAngle(0.0f);

	return -1;
}

void ImageObject::SetAngle(float angle)
{
	ang = angle;
	if(processedSurface != NULL)
	{
		SDL_FreeSurface(processedSurface);
		processedSurface = NULL;
	}
	processedSurface = rotozoomSurface(imageSurface, ang, scale, 1);

	position.w = processedSurface->w;
	position.h = processedSurface->h;
}

void ImageObject::SetScale(float sca)
{
	scale = sca;
	if(processedSurface != NULL)
	{
		SDL_FreeSurface(processedSurface);
		processedSurface = NULL;
	}
	processedSurface = rotozoomSurface(imageSurface, ang, scale, 0);

	position.w = processedSurface->w;
	position.h = processedSurface->h;
}

void ImageObject::Render(SDL_Surface *surface)
{
	if(visibility)
	{
		blitRect = position;
		SDL_BlitSurface( processedSurface, NULL, surface, &blitRect );
	}
}

SDL_Surface *ImageObject::GetImageSurface()
{
	return imageSurface;
}

SDL_Surface *ImageObject::GetProcessedImageSurface()
{
	return processedSurface;
}
