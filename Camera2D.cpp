#include "Camera2D.h"

Camera2D::Camera::Camera()
	: m_accelerationRate(DEFAULT_ACCEL)
	, m_maxVelocity(DEFAULT_MAX_VEL)
	, m_drag(DEFAULT_DRAG)
	, m_zoom(Vector2(1.f, 1.f))
	, m_zoomSpeed(DEFAULT_ZOOM_SPEED)
	, m_zoomToSpeed(DEFAULT_ZOOMTO_SPEED)
	, m_minZoom(DEFAULT_MIN_ZOOM)
	, m_maxZoom(DEFAULT_MAX_ZOOM)
	, m_zoomToActive(false)
	, m_zoomToFitActive(false)
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

void Camera2D::Camera::setZoomProps(float zoomSpeed, float zoomToSpeed, float minZoom, float maxZoom)
{
	m_zoomSpeed = zoomSpeed;
	m_zoomToSpeed = zoomToSpeed;
	m_minZoom = minZoom;
	m_maxZoom = maxZoom;

	m_zoomTarget.x = clampZoom(m_zoom.x);
	m_zoomTarget.y = clampZoom(m_zoom.y);
	zoomTo(m_zoomTarget);
}

void Camera2D::Camera::setZoomMinMax(float min, float max)
{
	m_minZoom = min;
	m_maxZoom = max;

	m_zoomTarget.x = clampZoom(m_zoom.x);
	m_zoomTarget.y = clampZoom(m_zoom.y);
	zoomTo(m_zoomTarget);
}

void Camera2D::Camera::pan(int xDir, int yDir)
{
	m_acceleration += Vector2(xDir, yDir) * m_accelerationRate;
	m_timeSinceLastXAccel = 0.f;
	m_timeSinceLastYAccel = 0.f;
	m_zoomToFitActive = false;
}

void Camera2D::Camera::panX(int xDir)
{
	m_acceleration += Vector2(xDir, 0.f) * m_accelerationRate;
	m_timeSinceLastXAccel = 0.f;
	m_zoomToFitActive = false;
}

void Camera2D::Camera::panY(int yDir)
{
	m_acceleration += Vector2(0.f, yDir) * m_accelerationRate;
	m_timeSinceLastYAccel = 0.f;
	m_zoomToFitActive = false;
}

void Camera2D::Camera::zoom(int dir)
{
	if (m_ratioResetting)
		return;
	std::cout << m_zoom.x << " y: " << m_zoom.y << std::endl;
	m_zoomToActive = false;
	m_zoomDirection.x = dir;
	m_zoomDirection.y = dir;
	m_zoom += m_zoomDirection * m_zoomSpeed;

	m_zoom.x = clampZoom(m_zoom.x);
	m_zoom.y = clampZoom(m_zoom.y);

	changeBoundsZoom();
}

void Camera2D::Camera::zoomTo(const Vector2& target)
{
	zoomTo(target.x, target.y);
}

void Camera2D::Camera::zoomTo(float targetX, float targetY) 
{
	targetX = clampZoom(targetX);
	targetY = clampZoom(targetY);

	m_zoomTarget.x = targetX; 
	m_zoomTarget.y = targetY;

	m_zoomStart = m_zoom;

	m_zoomToActive = true;
	m_zoomToTime = 0.f;
	m_zoomToMaxTime = ((m_zoomTarget - m_zoomStart).length()) / m_zoomToSpeed;
}

void Camera2D::Camera::zoomTo(float target)
{
	target = (target <= 0.f) ? m_zoomToSpeed : target;

	if (target < m_maxZoom)
	{
		target = m_maxZoom;
	}
	else if (target > m_minZoom && m_minZoom != -1)
	{
		target = m_minZoom;
	}
	m_zoomTarget.x = target; //TODO: change x and y seperately
	m_zoomTarget.y = target;

	m_zoomStart = m_zoom;

	m_zoomToActive = true;
	m_zoomToTime = 0.f;
	m_zoomToMaxTime = ((m_zoomTarget - m_zoomStart).length()) / m_zoomToSpeed;
}

void Camera2D::Camera::zoomToFit(const std::vector<SDL_Rect>& rects, bool keepZoomRatio)
{
	float minX = INT_MAX;
	float minY = INT_MAX;
	float maxX = -INT_MAX;
	float maxY = -INT_MAX;
	bool allRectsVisible = true;
	for (int i = 0; i < rects.size(); i++)
	{
		if (rects[i].x < minX)
			minX = rects[i].x;
		if (rects[i].x + rects[i].w > maxX)
			maxX = rects[i].x + rects[i].w;
		if (rects[i].y < minY)
			minY = rects[i].y;
		if (rects[i].y + rects[i].h > maxY)
			maxY = rects[i].y + rects[i].h;
		if (intersects(rects[i]) == false)
			allRectsVisible = false;
	}

	//if all rects are visible then no need to change zoom
	//if (allRectsVisible)
	//	return;

	std::vector<Point> points;
	points.push_back(Point(minX, minY)); //min
	points.push_back(Point(maxX, maxY)); //max

	zoomToFit(points, keepZoomRatio);
}

void Camera2D::Camera::resetZoomRatio(bool resetToX)
{
	if (m_zoom.x != m_zoom.y)
	{
		if (resetToX)
		{
			m_zoomTarget.x = m_zoom.x;
			m_zoomTarget.y = m_zoom.x;
		}
		else
		{
			m_zoomTarget.x = m_zoom.y;
			m_zoomTarget.y = m_zoom.y;
		}
		m_ratioResetting = true;
		zoomTo(m_zoomTarget);
	}
}

void Camera2D::Camera::zoomToFit(const std::vector<Point>& points, bool keepZoomRatio)
{
	float minX = INT_MAX;
	float minY = INT_MAX;
	float maxX = -INT_MAX;
	float maxY = -INT_MAX;
	bool allPointsVisible = true;
	for (int i = 0; i < points.size(); i++)
	{
		if (points[i].x < minX)
			minX = points[i].x;
		if (points[i].x > maxX)
			maxX = points[i].x;
		if (points[i].y < minY)
			minY = points[i].y;
		if (points[i].y > maxY)
			maxY = points[i].y;
		if (intersects(points[i]) == false)
			allPointsVisible = false;
	}
	//if all points are visible then no need to change zoom
	//if (allPointsVisible)
	//	return;
	m_zoomToFitStart = m_centre;
	m_zoomToFitTarget.x = (maxX - minX) * 0.5f;
	m_zoomToFitTarget.y = (maxY - minY) * 0.5f;

	m_zoomToFitActive = true;
	m_zoomToFitTime = 0.f;

	if (keepZoomRatio)
	{

		float desiredSize;
		if (maxX - minX > maxY - maxY)
		{
			desiredSize = maxX - minX;
		}
		else
		{
			desiredSize = maxY - maxY;
		}

		float windowSize = (m_windowWidth < m_windowHeight) ? m_windowWidth : m_windowHeight;
		float zoomTarget = desiredSize / windowSize;
		zoomTo(zoomTarget);
	}
	else
	{
		Vector2 zoomTarget;
		zoomTarget.x = (maxX - minX) / m_windowWidth;
		zoomTarget.y = (maxY - minY) / m_windowHeight;
		zoomTo(zoomTarget);
	}

	if (m_zoomToMaxTime == 0.f)
		m_zoomToFitMaxTime = (m_zoomToFitTarget - m_centre).length() / m_maxVelocity;
	else
		m_zoomToFitMaxTime = m_zoomToMaxTime;

	m_acceleration.limit(0.f);
	m_velocity.limit(0.f);

}

void Camera2D::Camera::update(float deltaTime)
{
	updateMotion(deltaTime);
	updateZoom(deltaTime);
}


void Camera2D::Camera::updateMotion(float deltaTime)
{
	if (m_zoomToFitActive)
	{
		float percent = m_zoomToFitTime / m_zoomToFitMaxTime;
		if (percent > 1.f)
		{
			m_centre = m_zoomToFitTarget;
			m_zoomToFitActive = false;
		}
		else
		{
			m_centre = lerp(m_zoomToFitStart, m_zoomToFitTarget, percent);
			m_zoomToFitTime += deltaTime;
		}
	}
	
	m_acceleration.limit(m_accelerationRate);
	m_velocity += m_acceleration * deltaTime;
	m_velocity.limit(m_maxVelocity);


	m_timeSinceLastXAccel += deltaTime; //TODO: make motion better as it seems to stop at some poitns
	m_timeSinceLastYAccel += deltaTime;

	if (m_timeSinceLastXAccel > MAX_TIME_BEFORE_ACCEL_RESET && m_zoomToFitActive == false) //too long since last x accel
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
	if (m_timeSinceLastYAccel > MAX_TIME_BEFORE_ACCEL_RESET && m_zoomToFitActive == false) //too long since last y accel
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

	if (m_timeSinceLastXAccel > MAX_TIME_BEFORE_ACCEL_RESET && m_timeSinceLastYAccel > MAX_TIME_BEFORE_ACCEL_RESET && m_zoomToFitActive == false)
	{
		m_acceleration.limit(0.f);
	}
}

void Camera2D::Camera::updateZoom(float deltaTime)
{ //TODO: use lerp here as well
	if (m_zoomToActive)
	{
		float percent = m_zoomToTime / m_zoomToMaxTime;
		if (percent > 1.f)
		{
			m_zoom = m_zoomTarget;
			m_zoomToActive = false;
			m_ratioResetting = false;
		}
		else
		{
			m_zoom = lerp(m_zoomStart, m_zoomTarget, percent);
			m_zoomToTime += deltaTime;
		}
		changeBoundsZoom();
	}
}

Camera2D::Vector2 Camera2D::Camera::lerp(const Vector2 & v1, const Vector2 & v2, float percent)
{
	if (percent > 1.f)
		percent = 1.f;
	else if (percent < 0.f)
		percent = 0.f;
	Vector2 v = v2 - v1;
	float distanceAlong = v.length() * percent;
	
	v = v1 + (v.normalize() * distanceAlong);
	return v;
}

float Camera2D::Camera::lerp(float start, float end, float percent)
{
	return start + ((end - start) * percent);
}

void Camera2D::Camera::changeBoundsZoom()
{
	//problem with less than zero or x and y not changing when using zoom to
	m_bounds.w = (int)m_windowWidth * m_zoom.x;
	m_bounds.h = (int)m_windowHeight * m_zoom.y;
}

float Camera2D::Camera::clampZoom(float num)
{
	if (num < m_maxZoom)
	{
		num = m_maxZoom;
	}
	else if (num > m_minZoom  && m_minZoom != -1)
	{
		num = m_minZoom;
	}
	if (num < m_zoomSpeed) //if zoom set to 0 or less then nothing will display so set to change amount
	{
		num = m_zoomSpeed;
	}
	return num;
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
