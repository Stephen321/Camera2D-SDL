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
	, m_attractorStopVel(DEFAULT_ATTRACTOR_STOP_VEL)
	, m_zoomToActive(false)
	, m_zoomToFitActive(false)
	, m_allowedHorizontal(true)
	, m_allowedVertical(true)
	, m_bounds({0,0,0,0})
	, m_updating(false)
	, m_restrictCentre(false)
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
	p = screenToWorld(p);
	r.x = p.x;
	r.y = p.y;
	r.w = (int)(r.w / xScale);
	r.h = (int)(r.h / yScale);

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

void Camera2D::Camera::setAccelerationRate(float accelerationRate)
{
	m_accelerationRate = accelerationRate;
	if (m_accelerationRate < 0.f)
		m_accelerationRate = 0.f;
}

float Camera2D::Camera::getAccelerationRate() const
{
	return m_accelerationRate;
}

void Camera2D::Camera::setMaxVelocity(float maxVelocity)
{
	m_maxVelocity = maxVelocity;
	if (m_maxVelocity < 0.f)
		m_maxVelocity = 0.f;
}

float Camera2D::Camera::getMaxVelocity() const
{
	return m_maxVelocity;
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

	Vector2 zoomTarget;
	zoomTarget.x = clampZoom(m_zoom.x);
	zoomTarget.y = clampZoom(m_zoom.y);
	if (m_updating)
	{
		zoomTo(zoomTarget);
	}
	else
	{
		changeBoundsZoom();
	}
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

void Camera2D::Camera::zoomTo(const Vector2& target, float time)
{
	m_zoomTarget.x = clampZoom(target.x);
	m_zoomTarget.y = clampZoom(target.y);

	m_zoomStart = m_zoom;

	m_zoomToActive = true;
	m_zoomToTime = 0.f;
	m_zoomToMaxTime = ((m_zoomTarget - m_zoomStart).length()) / m_zoomToSpeed;
}

void Camera2D::Camera::zoomTo(float target, float time)
{
	target = clampZoom(target);
	m_zoomTarget.x = target; 
	m_zoomTarget.y = target;

	m_zoomStart = m_zoom;

	m_zoomToActive = true;
	m_zoomToTime = 0.f;
	m_zoomToMaxTime = ((m_zoomTarget - m_zoomStart).length()) / m_zoomToSpeed;
	if (time >= 0.f)
	{
		m_zoomToMaxTime = time;
	}
}

void Camera2D::Camera::zoomToFit(const std::vector<SDL_Rect>& rects, bool keepZoomRatio, float time)
{
	if (rects.size() < 2)
		return;
	float minX = (float)INT_MAX;
	float minY = (float)INT_MAX;
	float maxX = -(float)INT_MAX;
	float maxY = -(float)INT_MAX;
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

	zoomToFit(points, keepZoomRatio, time);
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
			m_zoomTarget.y = m_zoom.y + halfDiff;
		}
		m_ratioResetting = true;
		zoomTo(m_zoomTarget);
	}
}

void Camera2D::Camera::setZoom(float zoomAmount)
{
	m_zoomToFitActive = false;
	m_zoomToMaxTime = 0.f;
	m_zoom.x = clampZoom(zoomAmount);
	m_zoom.y = clampZoom(zoomAmount);
	changeBoundsZoom();
}

float Camera2D::Camera::getZoomMax() const
{
	return m_maxZoom;
}

float Camera2D::Camera::getZoomMin() const
{
	return m_minZoom;
}

void Camera2D::Camera::zoomToFit(const std::vector<Point>& points, bool keepZoomRatio, float time)
{
	if (points.size() < 2)
		return;
	if (m_timeSinceLastXAccel < 0.05f || m_timeSinceLastYAccel < 0.05f) //if moving with key press on one of the axes has been recently pressed
		return;
	float minX = (float)INT_MAX;
	float minY = (float)INT_MAX;
	float maxX = -(float)INT_MAX;
	float maxY = -(float)INT_MAX;
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
	m_zoomToFitTarget.x = minX + abs(maxX - minX) * 0.5f;
	m_zoomToFitTarget.y = minY + abs(maxY - minY) * 0.5f;

	m_zoomToFitActive = true;
	m_zoomToFitTime = 0.f;

	if (keepZoomRatio)
	{

		float desiredSize;
		float windowSize;
		float xSize = abs(maxX - minX);
		float ySize = abs(maxY - minY);
		if (xSize / m_windowWidth > ySize / m_windowHeight)
		{
			desiredSize = xSize;
			windowSize = m_windowWidth;
		}
		else
		{
			desiredSize = ySize;
			windowSize = m_windowHeight;
		}

		float zoomTarget = desiredSize / windowSize;
		zoomTo(zoomTarget);
	}
	else
	{
		Vector2 zoomTarget;
		zoomTarget.x = abs(maxX - minX) / m_windowWidth;
		zoomTarget.y = abs(maxY - minY) / m_windowHeight;
		zoomTo(zoomTarget);
	}

	m_zoomToFitMaxTime = (m_zoomToFitTarget - m_centre).length() / m_maxVelocity;

	if (m_zoomToFitMaxTime > m_zoomToMaxTime)
		m_zoomToMaxTime = m_zoomToFitMaxTime;
	else
		m_zoomToFitMaxTime = m_zoomToMaxTime;

	if (time >= 0.f) //user specified a time
	{
		m_zoomToFitMaxTime = time;
		m_zoomToMaxTime = time;
	}
	m_acceleration.limit(0.f);
	m_velocity.limit(0.f);
}

void Camera2D::Camera::update(float deltaTime)
{
	m_updating = true;
	updateMotion(deltaTime);
	updateZoom(deltaTime);
	updateEffects(deltaTime);
}

void Camera2D::Camera::startEffect(Effect::Type type)
{ 
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

Camera2D::ParallaxEffect* Camera2D::Camera::findParallax(const std::string& name)
{
	for (int i = 0; i < m_parallaxEffects.size(); i++)
	{
		if (m_parallaxEffects[i].getName() == name)
		{
			return &m_parallaxEffects[i];
		}
	}

	return nullptr;
}

Camera2D::ShakeEffect* Camera2D::Camera::findShake(const std::string& name)
{
	for (int i = 0; i < m_shakeEffects.size(); i++)
	{
		if (m_shakeEffects[i].getName() == name)
		{
			return &m_shakeEffects[i];
		}
	}

	return nullptr;
}

Camera2D::Effect * Camera2D::Camera::findEffect(const std::string & name)
{
	Effect* effect = nullptr;
	bool found = false;
	for (int i = 0; i < m_parallaxEffects.size(); i++)
	{
		if (m_parallaxEffects[i].getName() == name)
		{
			effect = &m_parallaxEffects[i];
			found = true;
			break;
		}
	}

	for (int i = 0; i < m_shakeEffects.size() && found == false; i++)
	{
		if (m_shakeEffects[i].getName() == name)
		{
			effect = &m_shakeEffects[i];
			break;
		}
	}

	return effect;
}

Camera2D::Influencer * Camera2D::Camera::findInfluencer(const std::string & name)
{
	Influencer* affector = nullptr;

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

void Camera2D::Camera::resetMotion()
{
	m_acceleration.x = 0.f;
	m_acceleration.y = 0.f;
	m_velocity.x = 0.f;
	m_velocity.y = 0.f;
}

void Camera2D::Camera::restrictCentre(SDL_Rect restrictBounds)
{
	m_restrictCentre = true;
	m_restrictBounds = restrictBounds;
}

void Camera2D::Camera::unrestrictCentre()
{
	m_restrictCentre = false;
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

void Camera2D::Camera::addInfluencer(Influencer & affector, const std::string & name)
{
	if (affector.getName() == "") //name hasnt been set yet
		affector.setName(name);

	switch (affector.getType())
	{
	case Influencer::Type::Attractor:
		m_attractors.push_back((static_cast<Attractor&>(affector)));
		break;
	case Influencer::Type::Repulsor:
		m_repulsors.push_back((static_cast<Repulsor&>(affector)));
		break;
	}
}

void Camera2D::Camera::removeInfluencer(const std::string & name)
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

void Camera2D::Camera::removeAllInfluencer(Influencer::Type type)
{
	if (type == Influencer::Type::Attractor)
		m_attractors.clear();
	if (type == Influencer::Type::Repulsor)
		m_repulsors.clear();
}

void Camera2D::Camera::setAttractorStopVel(float attractorStopVel)
{
	m_attractorStopVel = attractorStopVel;
}

float Camera2D::Camera::getAttractorStopVel() const
{
	return m_attractorStopVel;
}

void Camera2D::Camera::updateMotion(float deltaTime)
{
	if (m_zoomToFitActive) 
	{
		m_acceleration.limit(0.f);
		m_velocity.limit(0.f);
		float percent = 1.f;
		if (m_zoomToFitMaxTime != 0.f)
		{
			percent = m_zoomToFitTime / m_zoomToFitMaxTime;
		}
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
	else
	{

		m_acceleration += getInfluencorAccel();
		m_acceleration.limit(m_accelerationRate);
		m_velocity += m_acceleration * deltaTime;

		m_centre += m_velocity * deltaTime;
		m_velocity.limit(m_maxVelocity);
	}
	
	//if centre is restricted
	if (m_restrictCentre)
	{
		bool stopZoomTo = false;
		if (m_centre.x < m_restrictBounds.x)
		{
			m_centre.x = m_restrictBounds.x;
			m_acceleration.x = 0.f;
			m_velocity.x = 0.f;
			stopZoomTo = true;
		}
		else if (m_centre.x > m_restrictBounds.x + m_restrictBounds.w)
		{
			m_centre.x = m_restrictBounds.x + m_restrictBounds.w;
			m_acceleration.x = 0.f;
			m_velocity.x = 0.f;
			stopZoomTo = true;
		}
		if (m_centre.y < m_restrictBounds.y)
		{
			m_centre.y = m_restrictBounds.y;
			m_acceleration.y = 0.f;
			m_velocity.y = 0.f;
			stopZoomTo = true;
		}
		else if (m_centre.y > m_restrictBounds.y + m_restrictBounds.h)
		{
			m_centre.y = m_restrictBounds.y + m_restrictBounds.h;
			m_acceleration.y = 0.f;
			m_velocity.y = 0.f;
			stopZoomTo = true;
		}
		if (stopZoomTo)
		{
			m_zoomToFitActive = false;
		}
	}

	m_timeSinceLastXAccel += deltaTime;
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


	m_bounds.x = (int)(m_centre.x - m_bounds.w * 0.5f);
	m_bounds.y = (int)(m_centre.y - m_bounds.h * 0.5f);
}

void Camera2D::Camera::updateZoom(float deltaTime)
{ 
	if (m_zoomToActive)
	{
		float percent = 1.f;
		if (m_zoomToMaxTime != 0.f)
		{
			percent = m_zoomToTime / m_zoomToMaxTime;
		}
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
		m_zoom.x = roundf(m_zoom.x * 1000) / 1000;
		m_zoom.y = roundf(m_zoom.y * 1000) / 1000;
		changeBoundsZoom();
	}
}

void Camera2D::Camera::updateEffects(float deltaTime)
{
	//parallax
	if (m_currentParallax != nullptr)
	{
		Vector2 parallaxVel = m_velocity * -1.f;
		if (m_zoomToFitActive)
		{
			parallaxVel.x = (m_centre.x < m_zoomToFitTarget.x) ? -m_maxVelocity : m_maxVelocity;
			parallaxVel.y = (m_centre.y < m_zoomToFitTarget.y) ? -m_maxVelocity : m_maxVelocity;
		}
		if (m_currentShake != nullptr && m_currentShake->getEnabled()) //if shaking the add shake offset
		{
			m_currentParallax->update(parallaxVel * deltaTime, worldToScreen(m_bounds), m_currentShake->getShakeOffset());
		}
		else
		{
			m_currentParallax->update(parallaxVel * deltaTime, worldToScreen(m_bounds));
		}
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
	if (num < 0.025f) 
	{
		num = 0.025f;
	}
	return num;
}

Camera2D::Vector2 Camera2D::Camera::getInfluencorAccel()
{
	Vector2 accel;

	if (m_zoomToFitActive == false) //influencors dont stop zoom tos
	{
		accel += getAttractorsAccel() * REPULSOR_WEIGHT;
		accel += getRepulsorsAccel() * REPULSOR_WEIGHT;
	}

	if (m_allowedHorizontal && accel.x != 0.f)
	{
		m_timeSinceLastXAccel = 0.f;
	}
	if (m_allowedVertical && accel.y != 0.f)
	{
		m_timeSinceLastYAccel = 0.f;
	}

	return accel;
}

Camera2D::Vector2 Camera2D::Camera::getAttractorsAccel()
{

	Vector2 target;
	int indexOf = -1;
	for (int i = 0; i < m_attractors.size(); i++)
	{
		float d = (m_attractors[i].getPosition() - m_centre).length();
		if (d > 0 && d < m_attractors[i].getRange())
		{
			target += m_attractors[i].getPosition();
			indexOf = i;
			break;
		}
	}
	if (indexOf >= 0)
	{

		Vector2 desired = target - m_centre;
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
		target = desired;

		if (d < ATTRACTOR_ARRIVED_RADIUS)
		{
			m_acceleration += m_velocity * -(1 - m_drag);
			if (m_velocity.length() > m_attractorStopVel * 0.5f && m_velocity.length() < m_attractorStopVel)
			{
				m_velocity.x = 0.f;
				m_velocity.y = 0.f;
			}
			target.limit(0.f);
		}

		target = target * m_attractors[indexOf].getStrength();
	}

	return target;
}

Camera2D::Vector2 Camera2D::Camera::getRepulsorsAccel()
{
	Vector2 steer;
	int count = 0;
	for (int i = 0; i < m_repulsors.size(); i++)
	{
		float d = (m_centre - m_repulsors[i].getPosition()).length();

		if (d > 0 && d < m_repulsors[i].getRange())
		{
			Vector2 diff = (m_centre - m_repulsors[i].getPosition());
			float B = 1000.f;
			float M = 0.5f;

			d *= 1.f / m_repulsors[i].getStrength();
			float U = (B / pow(d, M)); //(-A / pow(d, N));
			diff = diff.normalize();
			diff = diff * U;
			steer += diff;
			count++;
		}
	}

	// Adds average difference of m_position to m_acceleration
	if (count > 0)
		steer /= (float)count;

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

void Camera2D::Camera::setAllowedHorizontal(bool value)
{
	if (value == false)
	{
		m_velocity.x = 0.f;
		m_acceleration.x = 0.f;
	}
	m_allowedHorizontal = value;
}

bool Camera2D::Camera::getAllowedHorizontal() const
{
	return m_allowedHorizontal;
}

void Camera2D::Camera::setAllowedVertical(bool value)
{
	if (value == false)
	{
		m_velocity.y = 0.f;
		m_acceleration.y = 0.f;
	}
	m_allowedVertical = value;
}

bool Camera2D::Camera::getAllowedVertical() const
{
	return m_allowedVertical;
}

void Camera2D::Camera::lockMotion()
{
	resetMotion();
	m_allowedHorizontal = false;
	m_allowedVertical = false;
}

void Camera2D::Camera::unlockMotion()
{
	resetMotion();
	m_allowedHorizontal = true;
	m_allowedVertical = true;
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
