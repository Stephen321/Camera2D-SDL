#include "Layer.h"

Camera2D::Layer::Layer(const std::string& path, float scrollMultiplier, int sections)
	: SECTIONS((sections < 3) ? 3 : sections) //has to be 3 sections
	, m_sections(SECTIONS)
	, m_left(0)
	, m_middle(1)
	, m_right(2)
	, m_scrollMultiplier(scrollMultiplier)
	, m_path(path)
{
}

void Camera2D::Layer::init(bool scrollX, SDL_Renderer * renderer, const SDL_Rect& bounds)
{
	m_scrollX = scrollX;
	for (int i = 0; i < SECTIONS; i++)
	{
		Section& s = m_sections[i];
		s.texture = loadTexture(m_path + std::to_string(i) + ".bmp", renderer);
		int width, height;
		SDL_QueryTexture(s.texture, NULL, NULL, &width, &height);
		width *= ((float)bounds.w / width);
		height *= ((float)bounds.h / height);
		int x = 0;
		int y = 0;
		if (m_scrollX)
			x = (int)(width * (i - 1));
		else
			y = (int)(height * (i - 1));
		s.x = x;
		s.y = y;
		s.dest = { (int)s.x, (int)s.y, width, height };
	}
}

void Camera2D::Layer::draw(SDL_Renderer* target) const
{
	SDL_RenderCopy(target, m_sections[m_left].texture, NULL, &m_sections[m_left].dest);
	SDL_RenderCopy(target, m_sections[m_middle].texture, NULL, &m_sections[m_middle].dest);
	SDL_RenderCopy(target, m_sections[m_right].texture, NULL, &m_sections[m_right].dest);
}

void Camera2D::Layer::update(float vel, const SDL_Rect& bounds)
{
	//if (m_bounds == 0)
	//	return;
	static bool t = true;
	if (m_scrollX)
	{
		//std::cout << "m_bounds->x " << m_bounds->x << "m_sections[m_middle].dest.x " << m_sections[m_middle].dest.x << std::endl;
		if (m_sections[m_middle].dest.x > bounds.x + bounds.w)
		{
			std::cout << "middle.x > bounds.x + bounds.w" << std::endl;
			m_right = m_middle;
			m_middle = m_left;
			m_left = clamp(m_left - 1, 0, SECTIONS);
			positionSection(bounds, m_left, -1);
		}
		else if (m_sections[m_middle].dest.x + bounds.w < bounds.x)
		{
			m_left = m_middle;
			m_middle = m_right;
			m_right = clamp(m_right + 1, 0, SECTIONS);
			positionSection(bounds, m_right, 1);
		}
	}

	if (m_scrollX)
	{
		m_sections[m_left].x += vel * m_scrollMultiplier;
		m_sections[m_middle].x += vel * m_scrollMultiplier;
		m_sections[m_right].x += vel * m_scrollMultiplier;

		m_sections[m_left].dest.x = m_sections[m_left].x;
		m_sections[m_middle].dest.x = m_sections[m_middle].x;
		m_sections[m_right].dest.x = m_sections[m_right].x;

	}
	else
	{
		m_sections[m_left].y += vel * m_scrollMultiplier;
		m_sections[m_middle].y += vel * m_scrollMultiplier;
		m_sections[m_right].y += vel * m_scrollMultiplier;

		m_sections[m_left].dest.y = m_sections[m_left].y;
		m_sections[m_middle].dest.y = m_sections[m_middle].y;
		m_sections[m_right].dest.y = m_sections[m_right].y;
	}
}

Camera2D::Layer::~Layer()
{
	for (Section& s : m_sections)
	{
		SDL_DestroyTexture(s.texture);
		s.texture = NULL;
	}
}

int Camera2D::Layer::clamp(int value, int min, int max)
{
	if (value < 0)
	{
		return max - 1;
	}
	if (value >= max)
	{
		return 0;
	}

	return value;
}

void Camera2D::Layer::positionSection(const SDL_Rect& bounds, int section, int xDir, int yDir)
{
	float offsetX = bounds.w * xDir;
	float offsetY = bounds.h * yDir;
	m_sections[section].x = m_sections[m_middle].x + offsetX;
	m_sections[section].y = m_sections[m_middle].y + offsetY;
}

SDL_Texture * Camera2D::Layer::loadTexture(const std::string & path, SDL_Renderer* renderer)
{
	SDL_Texture* texture = NULL;

	SDL_Surface* surface = SDL_LoadBMP(path.c_str());
	if (surface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), SDL_GetError());
	}
	else
	{
		SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0xFF, 0, 0xFF));
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		if (texture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		SDL_FreeSurface(surface);
	}

	return texture;
}
