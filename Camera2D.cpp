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
	, m_allowedHorizontal(true)
	, m_allowedVertical(true)
	, m_bounds({0,0,0,0})
	, m_edgeSnapping(false)
	, m_xEdgeInterval(0)
	, m_yEdgeInterval(0)
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
	m_centre.y = y;
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

void Camera2D::Camera::setSize(int width, int height)
{
	m_bounds.w = width;
	m_bounds.h = height;
	m_bounds.x = m_centre.x - m_bounds.w * 0.5f;
	m_bounds.y = m_centre.y - m_bounds.h * 0.5f;
}

Camera2D::Vector2 Camera2D::Camera::getCentre() const
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
	screenR.w = (int)(screenR.w * xScale);
	screenR.h = (int)(screenR.h * yScale);

	return screenR;
}

Camera2D::Point Camera2D::Camera::worldToScreen(const Point & p) const
{
	Point screenP = p;

	float xScale = (float)m_windowWidth / m_bounds.w;
	float yScale = (float)m_windowHeight / m_bounds.h;

	if (m_currentShake != nullptr && m_currentShake->getEnabled()) //if shaking the add shake offset
	{
		screenP.x -= (m_bounds.x + m_currentShake->getShakeOffset().x);
		screenP.y -= (m_bounds.y + m_currentShake->getShakeOffset().y);
	}
	else
	{
		screenP.x -= m_bounds.x;
		screenP.y -= m_bounds.y;
	}
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
	r.w = (int)(r.w / xScale);
	r.h /= (int)(r.h / yScale);

	return r;
}

Camera2D::Point Camera2D::Camera::screenToWorld(const Point& sp) const
{
	float xScale = (float)m_bounds.w / m_windowWidth;
	float yScale = (float)m_bounds.h / m_windowHeight;
	Point p = sp;

	p.x *= xScale;
	p.y *= yScale;
	if (m_currentShake != nullptr && m_currentShake->getEnabled()) //if shaking the add shake offset
	{
		p.x += (m_bounds.x + m_currentShake->getShakeOffset().x);
		p.y += (m_bounds.y + m_currentShake->getShakeOffset().y);
	}
	else
	{
		p.x += m_bounds.x;
		p.y += m_bounds.y;
	}

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
	if (m_allowedHorizontal)
	{
		m_timeSinceLastXAccel = 0.f;
		m_zoomToFitActive = false;
	}
	else
	{
		xDir = 0;
	}
	if (m_allowedVertical)
	{
		m_timeSinceLastYAccel = 0.f;
		m_zoomToFitActive = false;
	}
	else
	{
		yDir = 0;
	}
	m_acceleration += Vector2(xDir, yDir) * m_accelerationRate;
	
}

void Camera2D::Camera::panX(int xDir)
{
	if (m_allowedHorizontal)
	{
		m_timeSinceLastXAccel = 0.f;
		m_zoomToFitActive = false;
	}
	else
	{
		xDir = 0.f;
	}

	m_acceleration += Vector2(xDir, 0.f) * m_accelerationRate;
}

void Camera2D::Camera::panY(int yDir)
{
	if (m_allowedVertical)
	{
		m_timeSinceLastYAccel = 0.f;
		m_zoomToFitActive = false;
	}
	else
	{
		yDir = 0.f;
	}
	m_acceleration += Vector2(0.f, (float)yDir) * m_accelerationRate;
}

void Camera2D::Camera::zoom(int dir)
{
	if (m_ratioResetting)
		return;
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
	endEffect(Effect::Type::Shake); //TODO: dont end but do both instead 

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
	float minX = std::numeric_limits<float>::max();
	float minY = std::numeric_limits<float>::max();
	float maxX = std::numeric_limits<float>::min();
	float maxY = std::numeric_limits<float>::min();
	bool allRectsVisible = true;
	for (unsigned int i = 0; i < rects.size(); i++)
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

void Camera2D::Camera::resetZoomRatio()
{
	if (m_zoom.x != m_zoom.y)
	{
		float halfDiff = abs(m_zoom.x - m_zoom.y) * 0.5f;
		if (m_zoom.x < m_zoom.y)
		{
			m_zoomTarget.x = m_zoom.x + halfDiff;
			m_zoomTarget.y = m_zoom.y - halfDiff;
		}
		else
		{
			m_zoomTarget.x = m_zoom.x - halfDiff;
			m_zoomTarget.y = m_zoom.y = halfDiff;
		}
		m_ratioResetting = true;
		zoomTo(m_zoomTarget);
	}
}

void Camera2D::Camera::zoomToFit(const std::vector<Point>& points, bool keepZoomRatio)
{
	endEffect(Effect::Type::Shake); //TODO: dont end but do both instead

	float minX = INT_MAX;
	float minY = INT_MAX;
	float maxX = -INT_MAX;
	float maxY = -INT_MAX;
	bool allPointsVisible = true;
	for (unsigned int i = 0; i < points.size(); i++)
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
	m_zoomToFitTarget.x = (maxX - abs(minX)) * 0.5f;
	m_zoomToFitTarget.y = (maxY - abs(minY)) * 0.5f;

	m_zoomToFitActive = true;
	m_zoomToFitTime = 0.f;

	if (keepZoomRatio)
	{

		float desiredSize;
		float windowSize;
		if (maxX - minX > maxY - minY)
		{
			desiredSize = maxX - minX;
			windowSize = m_windowWidth;
		}
		else
		{
			desiredSize = maxY - minY;
			windowSize = m_windowHeight;
		}

		float zoomTarget = desiredSize / windowSize;
		std::cout << "window size: " << windowSize << " desiredSize: " << desiredSize << " zoomTarget: " << zoomTarget << std::endl;
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
	updateEffects(deltaTime);
}


void Camera2D::Camera::startEffect(Effect::Type type)
{ //TODO: restart?

	endEffect(type);

	if (m_parallaxEffects.empty() == false)
	{
		m_currentParallax = &m_parallaxEffects[0];
		m_currentParallax->setEnabled(true);
	}

	if (m_shakeEffects.empty() == false)
	{
		m_currentShake = &m_shakeEffects[0];
		m_currentShake->setEnabled(true);
	}
}

void Camera2D::Camera::endEffect(Effect::Type type, bool remove)
{
	switch (type)
	{
	case Effect::Type::Parallax:
		if (m_currentParallax != nullptr)
		{
			m_currentParallax->setEnabled(false);
			if (remove)
				removeEffect(m_currentParallax->getName());
			m_currentParallax = nullptr;
		}
		break;
	case Effect::Type::Shake:
		if (m_currentShake != nullptr)
		{
			m_currentShake->setEnabled(false);
			if (remove)
				removeEffect(m_currentShake->getName());
			m_currentShake = nullptr;
		}
		break;
	}
}

void Camera2D::Camera::startEffect(const std::string & name)
{
	endEffect(name);

	for (int i = 0; i < m_parallaxEffects.size(); i++)
	{
		if (m_parallaxEffects[i].getName() == name)
		{
			m_currentParallax = &m_parallaxEffects[i];
			m_currentParallax->setEnabled(true);
			break;
		}
	}

	for (int i = 0; i < m_shakeEffects.size(); i++)
	{
		if (m_shakeEffects[i].getName() == name)
		{
			m_currentShake = &m_shakeEffects[i];
			m_currentShake->setEnabled(true);
			break;
		}
	}
}

void Camera2D::Camera::endEffect(const std::string & name, bool remove)
{
	if (m_currentParallax != nullptr)
	{
		for (int i = 0; i < m_parallaxEffects.size(); i++)
		{
			if (m_parallaxEffects[i].getName() == name)
			{
				m_currentParallax->setEnabled(false);
				if (remove)
					removeEffect(m_currentParallax->getName());
				m_currentParallax = nullptr;
				break;
			}
		}
	}

	if (m_currentShake != nullptr)
	{
		for (int i = 0; i < m_shakeEffects.size(); i++)
		{
			if (m_shakeEffects[i].getName() == name)
			{
				m_currentShake->setEnabled(false);
				if (remove)
					removeEffect(m_currentShake->getName());
				m_currentShake = nullptr;
				break;
			}
		}
	}
}

void Camera2D::Camera::endEffects()
{
	if (m_currentParallax != nullptr)
	{
		endEffect(m_currentParallax->getName());
	}

	if (m_currentShake != nullptr)
	{
		endEffect(m_currentShake->getName());
	}
}

Camera2D::Effect * Camera2D::Camera::findEffect(const std::string & name)
{
	Effect* effect = nullptr;

	for (int i = 0; i < m_parallaxEffects.size(); i++)
	{
		if (m_parallaxEffects[i].getName() == name)
		{
			effect = &m_parallaxEffects[i];
			break;
		}
	}

	for (int i = 0; i < m_shakeEffects.size(); i++)
	{
		if (m_shakeEffects[i].getName() == name)
		{
			effect = &m_shakeEffects[i];
			break;
		}
	}

	return effect;
}

Camera2D::Affector * Camera2D::Camera::findAffector(const std::string & name)
{
	Affector* affector = nullptr;

	for (int i = 0; i < m_attractors.size(); i++)
	{
		if (m_attractors[i].getName() == name)
		{
			affector = &m_attractors[i];
			break;
		}
	}

	for (int i = 0; i < m_repulsors.size(); i++)
	{
		if (m_repulsors[i].getName() == name)
		{
			affector = &m_repulsors[i];
			break;
		}
	}

	return affector;
}

void Camera2D::Camera::stopMotion()
{
	m_acceleration.x = 0.f;
	m_acceleration.y = 0.f;
	m_velocity.x = 0.f;
	m_velocity.y = 0.f;
}

void Camera2D::Camera::removeEffect(const std::string & name)
{
	endEffect(name);
	for (int i = 0; i < m_parallaxEffects.size(); i++)
	{
		if (m_parallaxEffects[i].getName() == name)
		{
			m_parallaxEffects.erase(m_parallaxEffects.begin() + i);
			break;
		}
	}

	for (int i = 0; i < m_shakeEffects.size(); i++)
	{
		if (m_shakeEffects[i].getName() == name)
		{
			m_shakeEffects.erase(m_shakeEffects.begin() + i);
			break;
		}
	}
} 

void Camera2D::Camera::removeEffect(Effect::Type type)
{
	endEffect(type);

	if (m_parallaxEffects.empty() == false)
	{
		m_parallaxEffects.erase(m_parallaxEffects.begin());
	}

	if (m_shakeEffects.empty() == false)
	{
		m_shakeEffects.erase(m_shakeEffects.begin());
	}
}

void Camera2D::Camera::enableEdgeSnap()
{
	m_edgeSnapping = true;
}

void Camera2D::Camera::setEdgeSnapIntervals(int xInterval, int yInterval, float intervalTime, bool enable)
{
	m_xEdgeInterval = xInterval;
	m_yEdgeInterval = yInterval;
	m_intervalTime = intervalTime;
	m_edgeSnapping = enable;
}

void Camera2D::Camera::disableEdgeSnap()
{
	m_edgeSnapping = false;
}

void Camera2D::Camera::addAffector(Affector & affector, const std::string & name)
{
	if (affector.getName() == "") //name hasnt been set yet
		affector.setName(name);

	switch (affector.getType())
	{
	case Affector::Type::Attractor:
		m_attractors.push_back((static_cast<Attractor&>(affector)));
		break;
	case Affector::Type::Repulsor:
		m_repulsors.push_back((static_cast<Repulsor&>(affector)));
		break;
	}
}

void Camera2D::Camera::removeAffector(const std::string & name)
{
	for (int i = 0; i < m_attractors.size(); i++)
	{
		if (m_attractors[i].getName() == name)
		{
			m_attractors.erase(m_attractors.begin() + i);
			break;
		}
	}

	for (int i = 0; i < m_repulsors.size(); i++)
	{
		if (m_repulsors[i].getName() == name)
		{
			m_repulsors.erase(m_repulsors.begin() + i);
			break;
		}
	}
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

	m_acceleration += getAffectorAccel();
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

	if (m_timeSinceLastXAccel > MAX_TIME_BEFORE_ACCEL_RESET && m_timeSinceLastYAccel > MAX_TIME_BEFORE_ACCEL_RESET && m_zoomToFitActive == false)
	{
		m_acceleration.limit(0.f);
	}

	//if (m_edgeSnapping)
	//{
		/*m_intervalElapsedTime += deltaTime;

		float percent = m_intervalElapsedTime / m_intervalTime;

		Vector2 vel;
		if (m_acceleration.x > 0.f)
		{
			if (m_xStart == 0.f)
				m_xStart = m_centre.x;
			vel.x = 1.f;
		}
		else if (m_acceleration.x < 0.f)
		{
			if (m_xStart == 0.f)
				m_xStart = m_centre.x;
			vel.x = -1.f;
		}
		if (percent < 1.f)
		{
			m_centre.x = lerp(m_xStart, m_xStart + (vel.x * m_xEdgeInterval), percent);
		}
		else
		{
			m_intervalElapsedTime = 0.f;
		}
		std::cout << m_centre.x << std::endl;*/
		/*if (((int)m_centre.x % m_xEdgeInterval) != 0)
		{
			m_centre.x = ((int)m_centre.x / m_xEdgeInterval) * m_xEdgeInterval;
		}*/
	//}
	m_centre += m_velocity * deltaTime;
	m_bounds.x = (int)(m_centre.x - m_bounds.w * 0.5f);
	m_bounds.y = (int)(m_centre.y - m_bounds.h * 0.5f);

}

void Camera2D::Camera::updateZoom(float deltaTime)
{ 
	if (m_zoomToActive)
	{
		if (m_zoomToMaxTime == 0.f)
		{
			m_zoom = m_zoomTarget;
			m_zoomToActive = false;
			m_ratioResetting = false;
			return;
		}
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

void Camera2D::Camera::updateEffects(float deltaTime)
{
	//parallax
	if (m_currentParallax != nullptr)
	{
		float parallaxSpeed = (m_currentParallax->getScrollX()) ? -m_velocity.x : -m_velocity.y;
		if (m_zoomToFitActive)
		{
			parallaxSpeed = (m_centre.x < m_zoomToFitTarget.x) ? -m_maxVelocity : m_maxVelocity;
		}
		m_currentParallax->update(parallaxSpeed * deltaTime, worldToScreen(m_bounds)); //TODO: make shake affect this
	}

	if (m_currentShake != nullptr)
	{
		m_currentShake->update(deltaTime);
		if (m_currentShake->getEnabled() == false)
		{
			m_currentShake = nullptr;
		}
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
	if (percent > 1.f)
		percent = 1.f;
	else if (percent < 0.f)
		percent = 0.f;
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

Camera2D::Vector2 Camera2D::Camera::getAffectorAccel()
{
	Vector2 accel;

	accel += getAttractorsAccel() * ATTRACTOR_WEIGHT;
	accel += getRepulsorsAccel() * REPULSOR_WEIGHT; 

	return accel;
}

Camera2D::Vector2 Camera2D::Camera::getAttractorsAccel()
{
	Vector2 result;

	Vector2 attractorForceSum(0, 0);
	int attractorCount = 0;
	for (int i = 0; i < m_attractors.size(); i++)
	{
		float d = (m_attractors[i].getPosition() - m_centre).length();
		if (d > 0 && d < m_attractors[i].getRange())
		{
			attractorForceSum += m_attractors[i].getPosition();
			attractorCount++;
		}
	}
	if (attractorCount > 0)
	{
		attractorForceSum /= (float)attractorCount;

		Vector2 desired = attractorForceSum - m_centre;
		float d = desired.length();

		float targetSpeed;

		if (d > ATTRACTOR_ARIVE_RADIUS)
		{
			targetSpeed = m_maxVelocity;
		}
		else
		{
			targetSpeed = m_maxVelocity * (d / ATTRACTOR_ARIVE_RADIUS);
		}

		Vector2 targetVelocity = desired.normalize() * targetSpeed;
		desired = targetVelocity - m_velocity;
		desired /= ATTRACTOR_TIME_TO_ARRIVE;
		desired.limit(m_accelerationRate);
		result = desired;

		if (d > ATTRACTOR_ARRIVED_RADIUS)
		{
			if (m_allowedHorizontal && result.x != 0.f)
			{
				m_timeSinceLastXAccel = 0.f;
				m_zoomToFitActive = false;
			}
			if (m_allowedVertical && result.y != 0.f)
			{
				m_timeSinceLastYAccel = 0.f;
				m_zoomToFitActive = false;
			}
		}
		else
		{
			m_acceleration += m_velocity * -0.1f;
			if (m_velocity.length() < ATTRACTOR_STOP_VEL)
			{
				m_velocity.x = 0.f;
				m_velocity.y = 0.f;
			}
		}
	}

	return result;
}

Camera2D::Vector2 Camera2D::Camera::getRepulsorsAccel()
{
	Vector2 steer;
	int count = 0;
	for (int i = 0; i < m_repulsors.size(); i++)
	{
		float d = (m_centre - m_repulsors[i].getPosition()).length();

		if (d > 0 && d < 300.f) //DESIRED_SEPARATION)
		{
			Vector2 diff = (m_centre - m_repulsors[i].getPosition());
			diff = diff.normalize();
			diff /= d;      // Weight by distance. Further away doesnt influence as much
			steer += diff;
			count++;
		}
	}

	// Adds average difference of m_position to m_acceleration
	if (count > 0)
		steer /= (float)count;
	if (steer.length() > 0)
	{
		// Steering = Desired - Velocity
		steer = steer.normalize();
		steer = steer * m_maxVelocity;
		steer -= m_velocity;
		steer.limit(m_accelerationRate);
	}

	if (m_allowedHorizontal && steer.x != 0.f)
	{
		m_timeSinceLastXAccel = 0.f;
		m_zoomToFitActive = false;
	}
	if (m_allowedVertical && steer.y != 0.f)
	{
		m_timeSinceLastYAccel = 0.f;
		m_zoomToFitActive = false;
	}

	return steer;
}

void Camera2D::Camera::render()
{
	if (m_currentParallax != 0)
		m_currentParallax->draw(m_renderer);
}

void Camera2D::Camera::moveBy(float x, float y)
{
	m_bounds.x = (int)(m_bounds.x + x);
	m_bounds.y += (int)(m_bounds.y + y);
}

void Camera2D::Camera::allowedHorizontal(bool value)
{
	m_allowedHorizontal = value;
}

void Camera2D::Camera::allowedVertical(bool value)
{
	m_allowedVertical = value;
}

void Camera2D::Camera::setLock(bool value)
{
	m_allowedHorizontal = value;
	m_allowedVertical = value;
}

void Camera2D::Camera::addEffect(Effect& effect, const std::string & name)
{
	if (effect.getName() == "") //name hasnt been set yet
		effect.setName(name);

	switch (effect.getType())
	{
	case Effect::Type::Parallax:
		m_parallaxEffects.push_back((static_cast<ParallaxEffect&>(effect)));
		m_parallaxEffects.back().init(m_renderer, m_bounds);
		break;
	case Effect::Type::Shake:
		m_shakeEffects.push_back((static_cast<ShakeEffect&>(effect)));
		m_shakeEffects.back().init();
		break;
	}
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
