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
	
}


void Scene::write_models()
{
	// TODO: Search for all .obj files which are models
	
	models.emplace_back(Model("res/models/Sting-Sword-lowpoly.obj"));
	
}


void Scene::draw_scene() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// TODO: draw each model
	for (int i = 0; i < models.size(); i++)
	{
		models[i].draw_model();
	}
	
	glDrawElements(GL_TRIANGLES, 6, GL_INT, nullptr);
	glfwSwapBuffers(window);
}

