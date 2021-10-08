#include "Camera.h"
#include <iostream>

Camera::Camera(glm::vec3 position = glm::vec3(-2.0f, 5.0f, 20.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH) : front(glm::vec3(0.0f, 0.0f, -1.0f)),
movement_speed(SPEED), mouse_sensitivity(SENSITIVITY), zoom(ZOOM) {
	this->position = position;
	this->worldUp = up;
	this->yaw = yaw;
	this->pitch = pitch;
	this->updateCameraVectors();
}

Camera::Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) : front(glm::vec3(0.0f, 0.0f, -1.0f)),
movement_speed(SPEED), mouse_sensitivity(SENSITIVITY), zoom(ZOOM) {
	this->position = glm::vec3(posX, posY, posZ);
	this->worldUp = glm::vec3(upX, upY, upZ);
	this->yaw = yaw;
	this->pitch = pitch;
	this->updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() {
	return glm::lookAt(this->position, this->position + this->front, this->up);
}

void Camera::ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime) {
	GLfloat velocity = this->movement_speed * deltaTime;

	if (FORWARD == direction) {
		this->position += this->front * velocity;
	}
	if (BACKWARD == direction) {
		this->position -= this->front * velocity;
	}
	if (LEFT == direction) {
		this->position -= this->right * velocity;
	}
	if (RIGHT == direction) {
		this->position += this->right * velocity;
	}
}

void Camera::ProcessMouseMovement(GLfloat xOffset, GLfloat yOffset, GLboolean constrainPitch = true) {
	xOffset *= this->mouse_sensitivity;
	yOffset *= this->mouse_sensitivity;

	this->yaw += xOffset;
	this->pitch += yOffset;

	if (constrainPitch) {
		if (this->pitch > 89.0f) {
			this->pitch = 89.0f;
		}
		if (this->pitch < -89.0f) {
			this->pitch = -89.0f;
		}
	}

	this->updateCameraVectors();
}

void Camera::ProcessMouseScroll(GLfloat yOffset) {
	if (this->zoom >= 44.0f && this->zoom <= 45.0f) {
		this->zoom -= yOffset * 0.1;
	}
	if (this->zoom <= 44.0f) {
		this->zoom = 44.0f;
	}
	if (this->zoom >= 45.0f) {
		this->zoom = 45.0f;
	}
}

GLfloat Camera::GetZoom() {
	return this->zoom;
}

void Camera::changePos(glm::vec3 position) {
	this->position = position;
}

void Camera::changeUp(glm::vec3 up) {
	this->up = up;
}

void Camera::changeYaw(GLfloat yaw) {
	this->yaw = yaw;
}


void Camera::updateCameraVectors() {
	// Calculate the new Front vector
	float vertdistance = GetZoom() * sin(glm::radians(this->pitch));
	float hordistance = GetZoom() * cos(glm::radians(this->pitch));
	glm::vec3 front;
	front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
	front.y = sin(glm::radians(this->pitch));
	front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
	//front.x = 1;
	//front.y = GetZoom() * this->pitch;
	//front.z = 2;


	this->front = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	this->right = glm::normalize(glm::cross(this->front, this->worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	this->up = glm::normalize(glm::cross(this->right, this->front));

}