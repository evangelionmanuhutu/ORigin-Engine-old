#include "pch.h"
#include "CameraController.h"
#include "Origin\IO\Input.h"
#include "Origin\IO\KeyCodes.h"
#include "Origin/IO/MouseButtonCodes.h"
#include "Origin\Core\OriginCore.h"

namespace Origin
{
	OrthoCameraController::OrthoCameraController(float aspectRatio, bool rotate)
		: m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, 
			m_AspectRatio * m_ZoomLevel, -m_ZoomLevel , m_ZoomLevel), m_IsRotate(rotate)
	{
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}

	void OrthoCameraController::OnUpdate(float time)
	{
		float moveSpeed = 1.0f;

		if (m_EditorMode) {
			if (Input::IsKeyPressed(OGN_KEY_A) && Input::IsMouseButtonPressed(OGN_MOUSE_BUTTON_RIGHT))
				m_CameraPosition.x -= m_CameraTranslationSpeed * time;
			else if (Input::IsKeyPressed(OGN_KEY_D) && Input::IsMouseButtonPressed(OGN_MOUSE_BUTTON_RIGHT))
				m_CameraPosition.x += m_CameraTranslationSpeed * time;
			if (Input::IsKeyPressed(OGN_KEY_S) && Input::IsMouseButtonPressed(OGN_MOUSE_BUTTON_RIGHT))
				m_CameraPosition.y -= m_CameraTranslationSpeed * time;
			else if (Input::IsKeyPressed(OGN_KEY_W) && Input::IsMouseButtonPressed(OGN_MOUSE_BUTTON_RIGHT))
				m_CameraPosition.y += m_CameraTranslationSpeed * time;

			if (!m_IsRotate) {
				if (Input::IsKeyPressed(OGN_KEY_LEFT_SHIFT))
					moveSpeed = 3.0f;
			}

			if (m_IsRotate) {
				if (Input::IsKeyPressed(OGN_KEY_LEFT_SHIFT) && Input::IsKeyPressed(OGN_KEY_A))
					m_CameraRotation -= m_CameraRotationSpeed * time;

				if (Input::IsKeyPressed(OGN_KEY_LEFT_SHIFT) && Input::IsKeyPressed(OGN_KEY_D))
					m_CameraRotation += m_CameraRotationSpeed * time;
			}
		}
		else {
			if (Input::IsKeyPressed(OGN_KEY_A))
				m_CameraPosition.x -= m_CameraTranslationSpeed * time;
			else if (Input::IsKeyPressed(OGN_KEY_D))
				m_CameraPosition.x += m_CameraTranslationSpeed * time;
			if (Input::IsKeyPressed(OGN_KEY_S))
				m_CameraPosition.y -= m_CameraTranslationSpeed * time;
			else if (Input::IsKeyPressed(OGN_KEY_W))
				m_CameraPosition.y += m_CameraTranslationSpeed * time;

			if (!m_IsRotate) {
				if (Input::IsKeyPressed(OGN_KEY_LEFT_SHIFT))
					moveSpeed = 3.0f;
			}

			if (m_IsRotate) {
				if (Input::IsKeyPressed(OGN_KEY_LEFT_SHIFT) && Input::IsKeyPressed(OGN_KEY_A))
					m_CameraRotation -= m_CameraRotationSpeed * time;

				if (Input::IsKeyPressed(OGN_KEY_LEFT_SHIFT) && Input::IsKeyPressed(OGN_KEY_D))
					m_CameraRotation += m_CameraRotationSpeed * time;
			}
		}

		// reset position
		if (Input::IsKeyPressed(OGN_KEY_LEFT_CONTROL) && Input::IsKeyPressed(OGN_KEY_R)) {
			m_CameraPosition = glm::vec3(0);
			m_CameraRotation = 0.0f;
		}

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		m_CameraTranslationSpeed = m_ZoomLevel * moveSpeed;
	}

	void OrthoCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispathcer(e);
		dispathcer.Dispatch<MouseScrolledEvent>(OGN_BIND_EVENT_FN(OrthoCameraController::OnMouseScrolled));
		dispathcer.Dispatch<WindowResizeEvent>(OGN_BIND_EVENT_FN(OrthoCameraController::OnWindowResized));
	}

	void OrthoCameraController::OnResize(float width, float height)
	{
		m_Width = width;
		m_Height = height;

		m_AspectRatio = width / height;
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}

	bool OrthoCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetYoffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthoCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		OnResize((float)e.getWidth(), (float)e.getHeight());
		return false;
	}

}