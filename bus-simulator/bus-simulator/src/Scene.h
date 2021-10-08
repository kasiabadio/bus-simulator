#ifndef SCENE_H_
#define SCENE_H_

#include "Model.h"
#include "Camera.h"
#include <GLFW/glfw3.h>

#include <vector>

class Scene
{
private:
	GLFWwindow* window;
	std::vector<Model*> models;

public:
	Scene(int _width, int _height);
	void draw_scene(float angle_x, float angle_y, Terrain& terrain, Camera& camera) const;
	GLFWwindow* read_window() const { return window; }
	void write_models();
	
};
#endif
