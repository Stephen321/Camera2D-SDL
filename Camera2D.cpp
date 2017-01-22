#include "Camera2D.h"

Camera2D::Camera::Camera()
	: m_accelerationRate(DEFAULT_ACCEL)
	, m_maxVelocity(DEFAULT_MAX_VEL)
	, m_drag(DEFAULT_DRAG)
	, m_zoom(1.f)
	, m_zoomSpeed(DEFAULT_ZOOM_SPEED)
	, m_zoomToSpeed(DEFAULT_ZOOMTO_SPEED)
	, m_minZoom(DEFAULT_MIN_ZOOM)
	, m_maxZoom(DEFAULT_MAX_ZOOM)
	, m_snapBackSpeed(DEFAULT_ZOOM_SNAP_SPEED)
	, m_maxSnapBackAmount(DEFAULT_ZOOM_SNAP_BACK)
	, m_zoomTarget(0.f)
	, m_zoomToActive(false)
	, m_zoomSnapping(false)
{
}

void Camera2D::Camera::init(int windowWidth, int windowHeight, SDL_Renderer * renderer)
{
	m_windowWidth = windowWidth;
	m_windowHeight = windowHeight;

	m_bounds = { 0, 0, m_windowWidth, m_windowHeight };
	m_renderer = renderer;
}


void Camera2D::Camera::setCentre(float x, float y)
{
	m_centre.x = x;
	m_centre.x = x;
	m_bounds.x = x - m_bounds.w * 0.5f;
	m_bounds.y = y - m_bounds.h * 0.5f;
}

void Camera2D::Camera::setCentre(const Point& p)
{
	m_centre.x = p.x;
	m_centre.y = p.y;
	m_bounds.x = m_centre.x - m_bounds.w * 0.5f;
	m_bounds.y = m_centre.y - m_bounds.h * 0.5f;
}

Camera2D::Vector2 Camera2D::Camera::getPosition() const
{
	return m_centre;
}

Camera2D::Vector2 Camera2D::Camera::getSize() const
{
	return Vector2(m_bounds.w, m_bounds.h);
}

SDL_Rect Camera2D::Camera::getBounds() const
{
	return m_bounds;
}

SDL_Rect Camera2D::Camera::worldToScreen(const SDL_Rect& r) const
{
	SDL_Rect screenR = r;

	float xScale = (float)m_windowWidth / m_bounds.w;
	float yScale = (float)m_windowHeight / m_bounds.h;

	Point screenP(screenR.x, screenR.y);
	screenP = worldToScreen(screenP);
	screenR.x = screenP.x;
	screenR.y = screenP.y;
	screenR.w *= xScale;
	screenR.h *= yScale;

	return screenR;
}

Camera2D::Point Camera2D::Camera::worldToScreen(const Point & p) const
{
	Point screenP = p;

	float xScale = (float)m_windowWidth / m_bounds.w;
	float yScale = (float)m_windowHeight / m_bounds.h;

	screenP.x -= m_bounds.x;
	screenP.y -= m_bounds.y;
	screenP.x *= xScale;
	screenP.y *= yScale;

	return screenP;
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

void Camera2D::Camera::setZoomProps(float zoomSpeed, float zoomToSpeed, float minZoom, float maxZoom, float snapBackSpeed, float maxSnapBackAmount)
{
	m_zoomSpeed = zoomSpeed;
	m_zoomToSpeed = zoomToSpeed;
	m_minZoom = minZoom;
	m_maxZoom = maxZoom;
	m_snapBackSpeed = snapBackSpeed;
	m_maxSnapBackAmount = maxSnapBackAmount;
	if (m_maxZoom - m_maxSnapBackAmount < 0.f)
	{
		m_maxSnapBackAmount = 0.f;
	}
}

void Camera2D::Camera::setZoomMinMax(float min, float max)
{
	m_minZoom = min;
	m_maxZoom = max;
}

void Camera2D::Camera::pan(int xDir, int yDir)
{
	m_acceleration += Vector2(xDir, yDir) * m_accelerationRate;
	m_timeSinceLastXAccel = 0.f;
	m_timeSinceLastYAccel = 0.f;
}

void Camera2D::Camera::panX(int xDir)
{
	m_acceleration += Vector2(xDir, 0.f) * m_accelerationRate;
	m_timeSinceLastXAccel = 0.f;
}

void Camera2D::Camera::panY(int yDir)
{
	m_acceleration += Vector2(0.f, yDir) * m_accelerationRate;
	m_timeSinceLastYAccel = 0.f;
}

void Camera2D::Camera::zoom(int dir)
{
	m_zoomToActive = false;
	m_zoomDirection = dir;
	m_zoom += m_zoomSpeed * m_zoomDirection;

	if (m_zoom < m_maxZoom - m_maxSnapBackAmount) //can't snap back futher than this
	{
		m_zoom = m_maxZoom - m_maxSnapBackAmount;
	}
	else if (m_zoom > m_minZoom + m_maxSnapBackAmount)
	{
		m_zoom = m_minZoom + m_maxSnapBackAmount;
	}

	if (m_zoom < m_maxZoom)  
	{
		zoomTo(m_maxZoom); //TODO: only do this when zooming stops? otherwise there's jerky zoom as you increase m_zoom and this decreases it. 
		m_zoomSnapping = true;
	}
	else if (m_zoom > m_minZoom && m_minZoom != -1)
	{
		zoomTo(m_minZoom);
		m_zoomSnapping = true;
	}
	else if (m_zoom < m_zoomSpeed) //if zoom set to 0 or less then nothing will display so set to change amount
	{
		m_zoom = m_zoomSpeed;
	}
	changeBoundsZoom();
}

void Camera2D::Camera::zoomTo(float target)
{
	float speed = m_zoomSpeed;
	if (m_zoomSnapping)
	{
		speed = m_snapBackSpeed;
	}
	target = (target < 0.f) ? speed : target;

	m_zoomToActive = true;
	m_zoomDirection = (target - m_zoom > 0) ? 1 : -1;
	if (target < m_maxZoom)
	{
		target = m_maxZoom;
	}
	else if (m_zoom > m_minZoom && m_minZoom != -1)
	{
		target = m_minZoom;
	}
	m_zoomTarget = target;
}

void Camera2D::Camera::update(float deltaTime)
{
	updateMotion(deltaTime);
	updateZoom(deltaTime);
}


void Camera2D::Camera::updateMotion(float deltaTime)
{
	std::cout << m_acceleration.x << " y: " << m_acceleration.y << std::endl;
	
	m_acceleration.limit(m_accelerationRate);
	m_velocity += m_acceleration * deltaTime;
	m_velocity.limit(m_maxVelocity);


	m_timeSinceLastXAccel += deltaTime; //TODO: make motion better as it seems to stop at some poitns
	m_timeSinceLastYAccel += deltaTime;

	if (m_timeSinceLastXAccel > MAX_TIME_BEFORE_ACCEL_RESET) //too long since last x accel
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
	if (m_timeSinceLastYAccel > MAX_TIME_BEFORE_ACCEL_RESET) //too long since last y accel
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

	m_centre += m_velocity * deltaTime;
	m_bounds.x = (int)(m_centre.x - m_bounds.w * 0.5f);
	m_bounds.y = (int)(m_centre.y - m_bounds.h * 0.5f);

	if (m_timeSinceLastXAccel > MAX_TIME_BEFORE_ACCEL_RESET && m_timeSinceLastYAccel > MAX_TIME_BEFORE_ACCEL_RESET)
	{
		m_acceleration.x = 0.f;
		m_acceleration.y = 0.f;
	}
}

void Camera2D::Camera::updateZoom(float deltaTime)
{
	if (m_zoomToActive)
	{
		m_zoom += m_zoomToSpeed * m_zoomDirection; 
		if ((m_zoomDirection != -1 && m_zoom > m_zoomTarget) ||
			(m_zoomDirection != 1 && m_zoom < m_zoomTarget))
		{
			m_zoom = m_zoomTarget;
			m_zoomToActive = false;
			m_zoomDirection = 0;
		}
		changeBoundsZoom();
	}
}

void Camera2D::Camera::changeBoundsZoom()
{
	//problem with less than zero or x and y not changing when using zoom to
	m_bounds.w = (int)m_windowWidth * m_zoom;
	m_bounds.h = (int)m_windowHeight * m_zoom;
}

void Camera2D::Camera::render()
{
}

void Camera2D::Camera::moveBy(float x, float y)
{
	m_bounds.x += x;
	m_bounds.y += y;
}

bool Camera2D::Camera::intersects(const SDL_Rect & r) const
{
	return (r.x < m_bounds.x + m_bounds.w && r.x + r.w > m_bounds.x &&
			r.y < m_bounds.y + m_bounds.h && r.y + r.h > m_bounds.y);
}

bool Camera2D::Camera::intersects(const Point & p) const
{
	return (p.x > m_bounds.x && p.x < m_bounds.x + m_bounds.w &&
			p.y > m_bounds.y && p.y < m_bounds.y + m_bounds.y);;
}
