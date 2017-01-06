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
	m_bounds.x = x; //TODO 
	m_bounds.y = y;
}

void Camera2D::Camera::setPosition(const Point& p)
{
	m_bounds.x = p.x; //TODO 
	m_bounds.y = p.y;
}

//Point Camera2D::Camera::worldToScreen(const Point &p)
//{
//	float vpTop = viewportBottomLeft.y + viewportSize.h;
//	float x = (p.x - viewportBottomLeft.x)* windowSize.w / viewportSize.w;
//	float y = (vpTop - p.y)* windowSize.h / viewportSize.h;
//
//	return Point2D(x, y);
//}
//
//Point Camera2D::Camera::screenToWorld(const Point &p)
//{
//	float vpTop = viewportBottomLeft.y + viewportSize.h;
//	float x = viewportBottomLeft.x + p.x*(viewportSize.w / windowSize.w);
//
//	float y = vpTop - p.y*viewportSize.h / windowSize.h;
//
//	return Point2D(x, y);
//}


bool Camera2D::Camera::worldToScreen(SDL_Rect& r) const
{
	float xScale = m_windowWidth / m_bounds.w;
	float yScale = m_windowHeight / m_bounds.h;

	r.x -= m_bounds.x;
	r.x *= xScale;
	r.y -= m_bounds.y;

	r.y *= yScale;
	r.w *= xScale;
	r.h *= yScale;

	bool visible = false;
	if (/*TODO: r is within the camera then draw it*/true)
	{
		visible = true;
	}
	return visible;
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

void Camera2D::Camera::update(float deltaTime)
{
	if (m_direction.x == 1)
	{
		int test = 5;
	}
	cout << "velX: " << m_direction.x << " , " << m_direction.y << endl;
	m_velocity += m_direction * (m_accelerationRate * deltaTime);
	m_position += m_velocity * deltaTime;
	m_bounds.x = (int)m_position.x;
	m_bounds.y = (int)m_position.y;
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