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
	models.emplace_back(Model(std::string("res/models/Bus.obj")));
	
	models[0].loadModel();
}


void Scene::draw_scene(float angle_x, float angle_y) const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (const auto& model : models)
	{
		model.draw_model(angle_x, angle_y);
	}
	
	glfwSwapBuffers(window);
}


