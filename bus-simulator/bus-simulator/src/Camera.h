#ifndef CAMERA_H
#endif CAMERA_H


#include <vector>
#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

const GLfloat YAW = -45.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 3.0f;
const GLfloat SENSITIVITY = 0.25f;
const GLfloat ZOOM = 45.0f;
const GLfloat distanceFromBus = 50.0f;

class Camera {
public:
	Camera();
	Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch);

	Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch);

	glm::mat4 GetViewMatrix();

	void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime);

	void ProcessMouseMovement(GLfloat xOffset, GLfloat yOffset, GLboolean constrainPitch);

	void ProcessMouseScroll(GLfloat yOffset);

	GLfloat GetZoom();

	void changePos(glm::vec3 position);

	void changeUp(glm::vec3 up);

	void changeYaw(GLfloat yaw);

private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;
	glm::vec3 left;

	GLfloat distanceFromBus;
	GLfloat yaw;
	GLfloat pitch;
	GLfloat movement_speed;
	GLfloat mouse_sensitivity;
	GLfloat zoom;

	void updateCameraVectors();
};
