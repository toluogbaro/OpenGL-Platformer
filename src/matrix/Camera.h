#pragma once

#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>


enum Camera_Mode
{
	FREE_ROAM,
	PLAYER
};

class Camera
{

	private:

		Camera_Mode m_CameraMode = Camera_Mode::FREE_ROAM;
		glm::vec3 m_CameraOffset = glm::vec3(1.0f);

		glm::vec3 m_CameraPosition;
		glm::vec3 m_CameraUp;
		glm::vec3 m_CameraFront;
		glm::vec3 m_CameraRight;

		float m_xAxisEuler;
		float m_yAxisEuler;
		float m_MoveSpeed;
		float m_MouseSensitivity;
		float m_FOV;
		float m_LastMouseX;
		float m_LastMouseY;
		float m_XOffset;
		float m_YOffset;

		bool m_FirstMouseInput;
		bool m_MouseDisabled;

	public:
		Camera(float moveSpeed, float fov)
		{
			m_CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
			m_CameraPosition = glm::vec3(1.0f);
			m_CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
			m_CameraRight = glm::vec3(1.0f, 0.0f, 0.0f);
			m_xAxisEuler = 0.0f;
			m_yAxisEuler = 0.0f;
			m_MouseSensitivity = 0.1f;
			m_MoveSpeed = moveSpeed;
			m_FOV = fov;
			m_LastMouseX = 0.0f;
			m_LastMouseY = 0.0f;
			m_XOffset = 0.0f;
			m_YOffset = 0.0f;
			m_FirstMouseInput = true;
			m_MouseDisabled = false;
		}

		~Camera();

		//Camera Movement
		void ProcessInput(GLFWwindow* window, float deltaTime);

		//Mouse Movement
		void ProcessMovement(GLFWwindow* currentWindow, double xMousePosition, double yMousePosition);
		void ProcessScroll(double scrollDepth);

		void PlayerCamera(glm::vec3 playerPos);

		//calculate lookat matrix after getting vector positions
		glm::mat4 ProcessViewMatrix();

		inline float GetFOV() { return m_FOV;  }
		inline Camera_Mode GetCameraMode(){ return m_CameraMode;  }

		inline void ChangeCameraMode(Camera_Mode newCamMode) { m_CameraMode = newCamMode;  }
		inline void SetCameraOffset(float x, float y, float z) { m_CameraOffset = glm::vec3(x, y, z);  }

		inline void SetCameraDirection()
		{

			glm::vec3 newFront = glm::vec3(1.0f);
			newFront.x = cos(glm::radians(m_xAxisEuler)) * cos(glm::radians(m_yAxisEuler));
			newFront.y = sin(glm::radians(m_yAxisEuler));
			newFront.z = sin(glm::radians(m_xAxisEuler)) * cos(glm::radians(m_yAxisEuler));

			m_CameraFront = glm::normalize(newFront);

			m_CameraRight = glm::normalize(glm::cross(m_CameraFront, glm::vec3(0.0f, 1.0f, 0.0f)));
			m_CameraUp = glm::normalize(glm::cross(m_CameraRight, m_CameraFront));
		}




};
