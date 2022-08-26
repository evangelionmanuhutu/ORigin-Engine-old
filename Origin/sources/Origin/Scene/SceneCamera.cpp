#include "pch.h"
#include "SceneCamera.h"

#include <glm\gtc\matrix_transform.hpp>

namespace Origin
{
	SceneCamera::SceneCamera()
	{
		RecalculateProjection();
	}

	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
	{
		m_OrthographicSize = size;
		m_OrthographicNear = nearClip;
		m_OrthographicFar = farClip;

		RecalculateProjection();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		m_AspectRatio = (float)width / (float)height;
		RecalculateProjection();
	}

	void SceneCamera::RecalculateProjection()
	{
		float OrthoLeft = -m_OrthographicSize * m_AspectRatio * 0.5f;
		float OrthoRight = m_OrthographicSize * m_AspectRatio * 0.5f;
		float OrthoTop = -m_OrthographicSize * 0.5f;
		float OrthoBottom = m_OrthographicSize * 0.5f;

		m_Projection = glm::ortho(OrthoLeft, OrthoRight, OrthoTop, OrthoBottom,
			m_OrthographicNear, m_OrthographicFar);
	}
}
