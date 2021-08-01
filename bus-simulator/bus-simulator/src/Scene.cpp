#include "Scene.h"


Scene::Scene(int _width, int _height)
{
	window = glfwCreateWindow(_width, _height, "bus-simulator", NULL, NULL);
	if (!window)
	{
		std::cout << "Error: glfwTerminate()" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
}


void Scene::write_models()
{
	// Sword
	models.emplace_back(Model("res/models/Sting-Sword-lowpoly.obj", "res/textures/Sting_Base_Color.png"));
	models[0].read_model();

	// Bus
	models.emplace_back(Model("res/models/Bus.obj", "res/textures/Bus_tex.tga.png"));
	models[1].read_model();

	// Grass
	models.emplace_back(Model("res/models/grass.obj", "res/textures/grass.png"));
	models[2].read_model();
}


void Scene::draw_scene(float angle_x, float angle_y) const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw all models
	for (const auto& model : models)
	{
		model.draw_model(angle_x, angle_y);
	}

	glfwSwapBuffers(window);
}
