#ifndef RENDERER_H
#define RENDERER_H
#include "BasicTypes.h"
#include <SDL.h>
#include "Camera2D.h"

class Renderer 
{
public:
	Renderer();
	void cleanUp();
	void setDrawColour(int r, int g, int b, int a) const;
	void setDrawColour(const Colour& c) const;
	bool initialize(const Camera2D::Camera* camera, const char* title, int width, int height, int flags);
	void clear();
	void drawRect(const SDL_Rect& r, const Colour& fillColour) const;
	void drawOutlineRect(const SDL_Rect& r, const Colour& outlineColour) const;
	void present();
	SDL_Renderer* getSDLRenderer();

private:
	SDL_Window* m_window;
	SDL_Renderer* m_renderer;
	const Camera2D::Camera* m_camera;
};

#endif