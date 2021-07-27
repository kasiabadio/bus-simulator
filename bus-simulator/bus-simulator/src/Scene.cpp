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



void Scene::draw_scene() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// TODO: draw each model
	for (int i = 0; i < models.size(); i++)
	{
		// TODO: draw_model()
	}
	
	glDrawElements(GL_TRIANGLES, 6, GL_INT, nullptr);
	glfwSwapBuffers(window);
}

