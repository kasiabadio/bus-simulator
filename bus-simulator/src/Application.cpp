
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include "Scene.h"

#define PI 3.14
float speed_x = 0;//[radians/s]
float speed_y = 0;//[radians/s]

GLfloat lastX = 1000 / 2.0f;
GLfloat lastY = 1000 / 2.0f;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
bool firstMouse = true;
bool keys[1024];
bool flag = false, first = true;
//glm::vec3(-2.0f, 5.0f, 20.0f), 
//glm::vec3(0.0f, 0.0f, 0.0f), 
//glm::vec3(0.0f, 15.0f, 0.0f)
//Camera camera(glm::vec3(0.0f, 5.0f, -10.0f), glm::vec3(0.0f, 15.0f, 0.0f), 90.0f, 0.0);
Camera camera(glm::vec3(-15.0f, 3.0f, -19.0f), glm::vec3(0.0f, 15.0f, 0.0f), 50.0f, 0.0);



void error_callback(int error, const char* description) {
	fputs(description, stderr);
}


void key_callback(
	GLFWwindow* window,
	int key,
	int scancode,
	int action,
	int mod
) {
	if (action == GLFW_PRESS) {
		keys[key] = true;
		flag = true;
		if (key == GLFW_KEY_LEFT) {
			speed_y = PI;
		}
		if (key == GLFW_KEY_RIGHT) {
			speed_y = -PI;
		}
		if (key == GLFW_KEY_UP) {
			speed_x = PI;

		}
		if (key == GLFW_KEY_DOWN) {
			speed_x = -PI;

		}
	}
	if (action == GLFW_RELEASE) {
		keys[key] = false;
		if (key == GLFW_KEY_LEFT || key == GLFW_KEY_RIGHT) {
			speed_y = 0;
		}
		if (key == GLFW_KEY_UP || key == GLFW_KEY_DOWN) {
			speed_x = 0;
		}
	}
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}


bool buttons[12];
void mouse_button(GLFWwindow* window,
	int button,
	int action,
	int mods
) {
	if (action == GLFW_PRESS)
		buttons[button] = true;
	if (action == GLFW_RELEASE)
		buttons[button] = false;
}

void mouse_callback(GLFWwindow* window,
	double xPos,
	double yPos
) {
	if (buttons[GLFW_MOUSE_BUTTON_2]) {
		if (firstMouse) {
			lastX = xPos;
			lastY = yPos;
			firstMouse = false;
		}

		GLfloat xOffset = xPos - lastX;
		GLfloat yOffset = lastY - yPos;

		lastX = xPos;
		lastY = yPos;

		camera.ProcessMouseMovement(xOffset, yOffset, true);
		std::cout << "Mouse Movement " << xPos << " : " << yPos << std::endl;
	}

}

void scroll_callback(GLFWwindow* window,
	double xOffset,
	double yOffset
) {
	camera.ProcessMouseScroll(yOffset);
	std::cout << "Scroll Movement " << xOffset << " : " << yOffset << std::endl;
}


void Movement()
{
	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, glfwGetTime());
	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, glfwGetTime());
	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, glfwGetTime());
	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, glfwGetTime());
	}
}


int main(void)
{

	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
		return -1;

	Scene scene(1000, 1000);

	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error: glewInit()" << std::endl;
	}

	initShaders();

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glfwSetKeyCallback(scene.read_window(), key_callback);
	glfwSetMouseButtonCallback(scene.read_window(), mouse_button);
	glfwSetCursorPosCallback(scene.read_window(), mouse_callback);
	glfwSetScrollCallback(scene.read_window(), scroll_callback);
	glfwSetInputMode(scene.read_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glEnable(GL_DEPTH_TEST);
	

	scene.write_models();
	Terrain terrain("res/textures/grass.png", 30, 30);


	float angle_x = 0;
	float angle_y = 0;
	glfwSetTime(0);
	
	while (!glfwWindowShouldClose(scene.read_window()))
	{
		if (flag && first) {
			camera.changePos(glm::vec3(0.0f, 5.0f, -10.0f));
			camera.changeUp(glm::vec3(0.0f, 15.0f, 0.0f));
			camera.changeYaw(90.0f);
			first = false;
		}
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		angle_x += speed_x * glfwGetTime();
		angle_y += speed_y * glfwGetTime();
		glfwSetTime(0);


		scene.draw_scene(angle_x, angle_y, terrain, camera);
		glfwPollEvents();

		Movement();
	}
	
	freeShaders();
	
	glfwTerminate();
	return 0;
}