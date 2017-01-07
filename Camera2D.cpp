#include "Camera2D.h"

Camera2D::Camera::Camera()
	: m_accelerationRate(DEFAULT_ACCEL),
	m_maxVelocity(DEFAULT_MAX_VEL),
	m_drag(DEFAULT_DRAG),
	m_zoom(1.f),
	m_zoomSpeed(DEFAULT_ZOOM_SPEED),
	m_minZoom(DEFAULT_MIN_ZOOM),
	m_maxZoom(DEFAULT_MAX_ZOOM),
	m_snapBackSpeed(DEFAULT_ZOOM_SNAP)
{
}

void Camera2D::Camera::init(int windowWidth, int windowHeight, SDL_Renderer * renderer)
{
	m_windowWidth = windowWidth;
	m_windowHeight = windowHeight;

	m_bounds = { (int)m_position.x, (int)m_position.y, m_windowWidth, m_windowHeight };
	m_renderer = renderer;
}


void Camera2D::Camera::setPosition(float x, float y)
{
	m_position.x = x;
	m_position.x = x;
	m_bounds.x = x; 
	m_bounds.y = y;
}

void Camera2D::Camera::setPosition(const Point& p)
{
	m_position.x = p.x;
	m_position.y = p.y;
	m_bounds.x = p.x;
	m_bounds.y = p.y;
}

bool Camera2D::Camera::worldToScreen(SDL_Rect& r) const
{
	float xScale = (float)m_windowWidth / m_bounds.w;
	float yScale = (float)m_windowHeight / m_bounds.h;

	Point p(r.x, r.y);
	worldToScreen(p);
	r.x = p.x;
	r.y = p.y;
	r.w *= xScale;
	r.h *= yScale;


	//pixel_x = (TILE_SIZE*zoom * getPosX() * 0.5) + (TILE_SIZE*zoom * getPosY() * 0.5) - cameraoffset_x; //pixel
	//float pos_x = (x + cameraoffset_x + 2.0*(y + cameraoffset_y)) / (TILE_SIZE*zoom) - 1.5; //iso
	//zoom around centre of each rectangle speratlely
	//r.x = (int)r.x - (r.w * m_zoom - r.w) / 2;
	//r.y = (int)r.y - (r.h * m_zoom - r.h) / 2;
	//r.w = (int)r.w * m_zoom;
	//r.h = (int)r.h * m_zoom;

	return true;
	bool visible = false;
	if (r.x <= m_bounds.w && r.x + r.w >= 0.f &&
		r.y <= m_bounds.h && r.y + r.h >= 0.f)
	{
		visible = true;
	}
	return visible;
}

bool Camera2D::Camera::worldToScreen(Point & p) const
{
	float xScale = (float)m_windowWidth / m_bounds.w;
	float yScale = (float)m_windowHeight / m_bounds.h;

	p.x -= m_bounds.x;
	p.y -= m_bounds.y;
	p.x *= xScale;
	p.y *= yScale;

	return (p.x > m_bounds.x && p.x < m_bounds.x + m_bounds.w &&
			p.y > m_bounds.y && p.y < m_bounds.y + m_bounds.y);
}

SDL_Rect Camera2D::Camera::screenToWorld(const SDL_Rect& sr) const
{
	float xScale = (float)m_bounds.w / m_windowWidth;
	float yScale = (float)m_bounds.h / m_windowHeight;
	SDL_Rect r = sr;

	Point p(r.x, r.y);
	screenToWorld(p);
	r.x = p.x;
	r.y = p.y;
	r.w /= xScale;
	r.h /= yScale;

	return r;
}

Camera2D::Point Camera2D::Camera::screenToWorld(const Point& sp) const
{
	float xScale = (float)m_bounds.w / m_windowWidth;
	float yScale = (float)m_bounds.h / m_windowHeight;
	Point p = sp;

	p.x *= xScale;
	p.y *= yScale;
	p.x += m_bounds.x;
	p.y += m_bounds.y;

	return p;
}

void Camera2D::Camera::setMotionProps(float accelerationRate, float maxVelocity, float drag)
{
	m_accelerationRate = accelerationRate;
	m_maxVelocity = maxVelocity;
	m_drag = drag;
}

void Camera2D::Camera::setZoomProps(float zoomSpeed, float minZoom, float maxZoom, float snapBackSpeed)
{
	m_zoomSpeed = zoomSpeed;
	m_minZoom = minZoom;
	m_maxZoom = maxZoom;
	m_snapBackSpeed = snapBackSpeed;
}

void Camera2D::Camera::setZoomMinMax(float min, float max)
{
	m_minZoom = min;
	m_maxZoom = max;
}

void Camera2D::Camera::pan(int xDir, int yDir)
{
	m_direction.x = xDir;
	m_direction.y = yDir;
}

void Camera2D::Camera::panX(int xDir)
{
	m_direction.x = xDir;
}

void Camera2D::Camera::panY(int yDir)
{
	m_direction.y = yDir;
}

void Camera2D::Camera::zoom(int dir)
{
	m_zoomDirection = dir;
	m_zoom += m_zoomSpeed * m_zoomDirection;
	if (m_zoom < m_maxZoom && m_maxZoom != -1) //TODO check if -1 means infinite
	{
		m_zoom = m_maxZoom; //snap back here
	}
	else if (m_zoom > m_minZoom)
	{
		m_zoom = m_minZoom; //snap here
	}
	else if (m_zoom < 0)
	{
		m_zoom = 0;
	}
	Point centre(m_bounds.x + m_bounds.w * 0.5f, m_bounds.y + m_bounds.h * 0.5f); //centre before zoom
	m_bounds.w = (int)m_windowWidth * m_zoom;
	m_bounds.h = (int)m_windowHeight * m_zoom;

	m_bounds.x = (int)(centre.x - m_bounds.w * 0.5f);
	m_bounds.y = (int)(centre.y - m_bounds.h * 0.5f);


	m_position.x = m_bounds.x;
	m_position.y = m_bounds.y;
	cout << "x: " << m_bounds.x << " ,y: " << m_bounds.y << " ,w: " << m_bounds.w << " ,h: " << m_bounds.h << endl;
}

void Camera2D::Camera::update(float deltaTime)
{
	updateMotion(deltaTime);
	updateZoom(deltaTime);
	//cout << "velX: " << m_velocity.x << " , " << m_velocity.y << endl;

}


void Camera2D::Camera::updateMotion(float deltaTime)
{
	m_velocity += m_direction * (m_accelerationRate * deltaTime);

	if (m_velocity.length() > m_maxVelocity) //limit to max velocity
	{
		m_velocity = m_velocity.normalize() * m_maxVelocity;
	}
	if (m_direction.x == 0.f) //if no input to move on x
	{
		if (abs(m_velocity.x) < MIN_VEL) //moving slow enough then just stop 
		{
			m_velocity.x = 0.f;
		}
		else //apply drag
		{
			m_velocity.x -= m_velocity.x *  deltaTime * m_drag;
		}
	}
	if (m_direction.y == 0.f) //no input to move on y
	{
		if (abs(m_velocity.y) < MIN_VEL) //moving slow enough then just stop 
		{
			m_velocity.y = 0.f;
		}
		else //apply drag
		{
			m_velocity.y -= m_velocity.y *  deltaTime * m_drag;
		}
	}

	m_position += m_velocity * deltaTime;
	m_bounds.x = (int)m_position.x;
	m_bounds.y = (int)m_position.y;
}

void Camera2D::Camera::updateZoom(float deltaTime)
{
}

void Camera2D::Camera::render()
{
}

void Camera2D::Camera::moveBy(float x, float y)
{
	m_bounds.x += x;
	m_bounds.y += y;
}

//SDL_Rect Camera2D::Camera::screenToWorld(const SDL_Rect& r)
//{
//
//	Point2D p = screenToWorld(r.pos);
//
//	float ww = r.size.w*(viewportSize.w / windowSize.w);
//	float wh = -r.size.h*(viewportSize.h / windowSize.h);
//
//	return Rect(p, Size2D(ww, wh));
//}