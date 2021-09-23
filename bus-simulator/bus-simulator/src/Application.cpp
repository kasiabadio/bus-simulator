
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
		if (key == GLFW_KEY_LEFT) {
			speed_y = -PI;
		}
		if (key == GLFW_KEY_RIGHT) {
			speed_y = PI;
		}
		if (key == GLFW_KEY_UP) {
			speed_x = -PI;
		}
		if (key == GLFW_KEY_DOWN) {
			speed_x = PI;
		}
	}
	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_LEFT || key == GLFW_KEY_RIGHT) {
			speed_y = 0;
		}
		if (key == GLFW_KEY_UP || key == GLFW_KEY_DOWN) {
			speed_x = 0;
		}
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
	
	glEnable(GL_DEPTH_TEST | GL_FOG);
	glFogf(GL_FOG_MODE, GL_LINEAR);
	glFogf(GL_FOG_START, 1.0f);
	glFogf(GL_FOG_END, 5.0f);
	float color[] = {0.5f, 0.5f, 0.5f, 1.0f}; // gray fog same as clear color
	glFogfv(GL_FOG_COLOR, color);

	scene.write_models();
	Terrain terrain("res/textures/grass.png", 29, 30);
	
	float angle_x = 0;
	float angle_y = 0;
	glfwSetTime(0);
	while (!glfwWindowShouldClose(scene.read_window()))
	{
		angle_x += speed_x * glfwGetTime();
		angle_y += speed_y * glfwGetTime();
		glfwSetTime(0);
		scene.draw_scene(angle_x, angle_y, terrain);
		glfwPollEvents();
	}
	
	glDisable(GL_FOG);
	freeShaders();
	
	glfwTerminate();
	return 0;
}