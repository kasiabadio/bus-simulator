
#define GLEW_STATIC
 
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



void error_callback(int error, const char* description) {
	fputs(description, stderr);
}



int main(void)
{
    
  
	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
		return -1;
	
    Scene scene(500, 500);
	glfwMakeContextCurrent(scene.read_window());
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error: glewInit()" << std::endl;
	}

   
	
	while(!glfwWindowShouldClose(scene.read_window()))
	{
		scene.draw_scene();
		glfwPollEvents();
	}


    glfwTerminate();
    return 0;
}