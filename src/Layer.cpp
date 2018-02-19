#include "Layer.h"
#include "SDL2\SDL_image.h"

Camera2D::Layer::Layer(const char* path, float scrollMultiplier, int sections, bool locked)
	: SECTIONS((sections < 3) ? 3 : sections) //has to be 3 sections at least
	, m_sections(SECTIONS)
	, m_textureSections(sections)
	, m_left(0)
	, m_middle(1)
	, m_right(2)
	, m_scrollMultiplier(scrollMultiplier)
	, m_locked(locked)
	, m_path(path)
{
}

void Camera2D::Layer::init(bool scrollX, SDL_Renderer * renderer, const SDL_Rect& bounds)
{
	m_scrollX = scrollX;
	for (unsigned int i = 0; i < SECTIONS; i++)
	{
		Section& s = m_sections[i];
		if (i >= m_textureSections) //if index is greater than the amount of available textures (index will always go up to at least 3)
		{
			int textureIndex = m_textureSections - 1; //get index of the last section that loaded a texture
			//use the same texture for this section
			s.texture = m_sections[textureIndex].texture;
		}
		else //load the required texture
		{
			s.texture = loadTexture(m_path + std::to_string(i) + ".png", renderer);
		}
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
	SDL_Rect left = m_sections[m_left].dest;
	SDL_Rect middle = m_sections[m_middle].dest;
	SDL_Rect right = m_sections[m_right].dest;

	if (m_shakeOffset.length() > 0.f)
	{
		if (m_scrollX)
		{
			left.x -= m_shakeOffset.x;
			middle.x -= m_shakeOffset.x;
			right.x -= m_shakeOffset.x;
		}
		else
		{
			left.y -= m_shakeOffset.y;
			middle.y -= m_shakeOffset.y;
			right.y -= m_shakeOffset.y;
		}
	}

	SDL_RenderCopy(target, m_sections[m_left].texture, NULL, &left);
	SDL_RenderCopy(target, m_sections[m_middle].texture, NULL, &middle);
	SDL_RenderCopy(target, m_sections[m_right].texture, NULL, &right);
}

void Camera2D::Layer::update(const Vector2& vel, const SDL_Rect& bounds, const Vector2& shakeOffset)
{
	m_shakeOffset = shakeOffset;
	float xChange = vel.x * m_scrollMultiplier;
	float yChange = vel.y * m_scrollMultiplier;
	if (m_scrollX)
	{
		m_sections[m_left].x += xChange;
		m_sections[m_middle].x += xChange;
		m_sections[m_right].x += xChange;

		if (m_locked == false)
		{
			m_sections[m_left].y += yChange ;
			m_sections[m_middle].y += yChange ;
			m_sections[m_right].y += yChange ;
		}
	}
	else
	{
		m_sections[m_left].y += yChange ;
		m_sections[m_middle].y += yChange ;
		m_sections[m_right].y += yChange ;

		if (m_locked == false)
		{
			m_sections[m_left].x += xChange;
			m_sections[m_middle].x += xChange;
			m_sections[m_right].x += xChange;
		}
	}

	m_sections[m_left].dest.x = m_sections[m_left].x;
	m_sections[m_left].dest.y = m_sections[m_left].y;
	m_sections[m_middle].dest.x = m_sections[m_middle].x;
	m_sections[m_middle].dest.y = m_sections[m_middle].y;
	m_sections[m_right].dest.x = m_sections[m_right].x;
	m_sections[m_right].dest.y = m_sections[m_right].y;


	if (m_scrollX)
	{
		if (m_sections[m_middle].dest.x > bounds.x + bounds.w)
		{
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
	else
	{
		if (m_sections[m_middle].dest.y > bounds.y + bounds.h)
		{
			m_right = m_middle;
			m_middle = m_left;
			m_left = clamp(m_left - 1, 0, SECTIONS);
			positionSection(bounds, m_left, 0, -1);
		}
		else if (m_sections[m_middle].dest.y + bounds.h < bounds.y)
		{
			m_left = m_middle;
			m_middle = m_right;
			m_right = clamp(m_right + 1, 0, SECTIONS);
			positionSection(bounds, m_right, 0, 1);
		}
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

void Camera2D::Layer::setScrollMultiplier(float scrollMultiplier)
{
	m_scrollMultiplier = scrollMultiplier;
}

float Camera2D::Layer::getScrollMultiplier() const
{
	return m_scrollMultiplier;
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

	SDL_Surface* surface = IMG_Load(path.c_str());
	if (surface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), SDL_GetError());
	}
	else
	{
		//SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0xFF, 0, 0xFF));
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		if (texture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		SDL_FreeSurface(surface);
	}

	return texture;
}
