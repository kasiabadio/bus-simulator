#ifndef SCENE_H_
#define SCENE_H_

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <iostream>

#include "Model.h"

class Scene
{
private:
	GLFWwindow* window;
	std::vector<Model> models;
	

public:
	Scene(int _width, int _height);
	void draw_scene() const;
	GLFWwindow* read_window() const { return window; }
	std::vector<Model> read_models() const { return models; }
	
};


#endif
