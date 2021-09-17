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
	
	// Tree
	//models.emplace_back(new Tree("res/models/raw2.obj", "res/textures/raw3.jpg"));
	//models[1]->write_model();

	// Road
	models.emplace_back(new Road("res/models/Road.obj", "res/textures/RoadDiffuse.png"));
	models[1]->write_model();
	models[1]->write_model_static_transformations();
}


void Scene::draw_scene(float angle_x, float angle_y, Terrain& terrain) const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::mat4 M_scene = glm::mat4(1.0f);
	glm::mat4 P_scene = glm::mat4(1.0f);
	glm::mat4 V_scene = glm::mat4(1.0f);

	glm::vec3 temp_translate;
	glm::vec3 temp_rotate;
	glm::vec3 temp_scale;
	
	V_scene = glm::lookAt(
		glm::vec3(-2.0f, 5.0f, 20.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 15.0f, 0.0f));
	P_scene = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
	//M_scene = glm::rotate(M_scene, glm::radians(50.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	// DRAW TERRAIN
	terrain.draw_terrain(Input(P_scene, V_scene, M_scene));

	// DRAW ROAD
	models[1]->draw_model(Input(terrain.read_model_matrices().P, terrain.read_model_matrices().V, terrain.read_model_matrices().M));

	
	// DRAW BUS
	models[0]->draw_model(Input(angle_x, angle_y, terrain.read_model_matrices().P, terrain.read_model_matrices().V, terrain.read_model_matrices().M));

	
	
	glfwSwapBuffers(window);
}
