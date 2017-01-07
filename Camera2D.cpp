#include "Camera2D.h"

Camera2D::Camera::Camera()
{
}

void Camera2D::Camera::init(int windowWidth, int windowHeight, SDL_Renderer * renderer)
{
	m_windowWidth = windowWidth;
	m_windowHeight = windowHeight;

	m_bounds = { 0, 0, m_windowWidth, m_windowHeight };
	m_renderer = renderer;

	m_accelerationRate = DEFAULT_ACCEL;
	m_maxVelocity = DEFAULT_MAX_VEL;
	m_drag = DEFAULT_DRAG;
}


void Camera2D::Camera::setPosition(float x, float y)
{
	m_bounds.x = x; 
	m_bounds.y = y;
}

void Camera2D::Camera::setPosition(const Point& p)
{
	m_bounds.x = p.x; 
	m_bounds.y = p.y;
}

bool Camera2D::Camera::worldToScreen(SDL_Rect& r) const
{
	float xScale = m_windowWidth / m_bounds.w;
	float yScale = m_windowHeight / m_bounds.h;

	Point p(r.x, r.y);
	worldToScreen(p);
	r.x = p.x;
	r.y = p.y;
	r.w *= xScale;
	r.h *= yScale;

	bool visible = false;
	if (r.x < m_bounds.x + m_bounds.w && r.x + r.w > m_bounds.x &&
		r.y < m_bounds.y && r.y + r.h > m_bounds.y + m_bounds.h) 
	{
		visible = true;
	}
	return visible;
}

bool Camera2D::Camera::worldToScreen(Point & p) const
{
	float xScale = m_windowWidth / m_bounds.w;
	float yScale = m_windowHeight / m_bounds.h;

	p.x -= m_bounds.x;
	p.y -= m_bounds.y;
	p.x *= xScale;
	p.y *= yScale;

	return (p.x > m_bounds.x && p.x < m_bounds.x + m_bounds.w &&
			p.y > m_bounds.y && p.y < m_bounds.y + m_bounds.y);
}

SDL_Rect Camera2D::Camera::screenToWorld(const SDL_Rect& sr) const
{
	SDL_Rect r = sr;
	return r;
}

Camera2D::Point Camera2D::Camera::screenToWorld(const Point& sp) const
{
	float xScale = m_bounds.w / m_windowWidth;
	float yScale = m_bounds.h / m_windowHeight;
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

void Camera2D::Camera::update(float deltaTime)
{
	if (m_direction.x == 1)
	{
		int test = 5;
	}
	//cout << "velX: " << m_velocity.x << " , " << m_velocity.y << endl;
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

void Camera2D::Camera::render()
{
}

void Camera2D::Camera::moveBy(float x, float y)
{
	m_position.x += x;
	m_position.y += y;
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