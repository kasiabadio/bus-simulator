#include "Scene.h"

#include <memory>
#include <memory>
#include <memory>


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
	//models.emplace_back(Model("res/models/Sting-Sword-lowpoly.obj", "res/textures/Sting_Base_Color.png"));
	//models[0].read_model();

	// Bus
	models.emplace_back(new Bus("res/models/Bus.obj", "res/textures/Bus_tex.tga.png"));
	models[0]->read_model();
	
	// Grass
	models.emplace_back(new Grass("res/models/grass.obj", "res/textures/grass.png"));
	models[1]->read_model();
	
}


void Scene::draw_scene(float angle_x, float angle_y) const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::mat4 M_scene = glm::mat4(1.0f);
	glm::mat4 P_scene = glm::mat4(1.0f);
	glm::mat4 V_scene = glm::mat4(1.0f);
	
	V_scene = lookAt(
		glm::vec3(0.0f, 0.0f, -20.0f), 
		glm::vec3(0.0f, 1.0f, 0.0f), 
		glm::vec3(0.0f, 1.0f, 0.0f));
	P_scene = glm::perspective(glm::radians(45.0f), 1.0f, 1.0f, 100.0f);

	models[0]->draw_model(Input(angle_x, angle_y, P_scene, V_scene, M_scene));
	models[1]->draw_model(Input(P_scene, V_scene, M_scene));

	
	glfwSwapBuffers(window);
}
