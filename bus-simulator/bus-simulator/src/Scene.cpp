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
	
	
	// Bus
	models.emplace_back(new Bus("res/models/Bus.obj", "res/textures/Bus_tex.tga.png"));
	models[0]->write_model();
	
	// Grass
	//models.emplace_back(new Grass("res/models/grass.obj", "res/textures/grass.png"));
	//models[1]->write_model();

	
}


void Scene::draw_scene(float angle_x, float angle_y, Terrain &terrain) const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::mat4 M_scene = glm::mat4(1.0f);
	glm::mat4 P_scene = glm::mat4(1.0f);
	glm::mat4 V_scene = glm::mat4(1.0f);
	
	
	V_scene = glm::lookAt(
		glm::vec3(0.0f, 4.0f, 15.0f),
		glm::vec3(0.0f, 0.0f, -0.5f),
		glm::vec3(0.0f, 20.0f, 0.0f));
	P_scene = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
	//M_scene = glm::rotate(M_scene, glm::radians(50.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	// Draw terrain
	
	terrain.draw_terrain(Input(P_scene, V_scene, M_scene));
	
	// Draw grass
	//models[1]->draw_model(Input(P_scene, V_scene, M_scene));

	
	// Draw bus on the grass (it is relative to it, that is why we are reading model's matrices)
	//models[0]->draw_model(Input(angle_x, angle_y, models[1]->read_model_matrices().P, models[1]->read_model_matrices().V, models[1]->read_model_matrices().M));
	models[0]->draw_model(Input(angle_x, angle_y, terrain.read_model_matrices().P, terrain.read_model_matrices().V, terrain.read_model_matrices().M));
	
	
	glfwSwapBuffers(window);
}
