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
	// Road
	models.emplace_back(new Road("res/models/Road.obj", "res/textures/RoadDiffuse.png", "null"));
	models[0]->write_model();
	models[0]->write_model_static_transformations();
	
	// Bus
	models.emplace_back(new Bus("res/models/Bus.obj", "res/textures/Bus_tex.tga.png", "null"));
	models[1]->write_model();
	//models[1]->box.make_bounding_box();
	
	// Tree
	models.emplace_back(new Tree("res/models/MapleTreeStem.obj", "res/textures/maple_bark.png", "null"));
	models[2]->write_model();
	models[2]->write_model_static_transformations();
	//models[2]->box.make_bounding_box();
	
	// Grass
	models.emplace_back(new Grass("res/models/grass variations.obj", "res/textures/Blades of Grass.png", "null"));
	models[3]->write_model();
	models[3]->write_model_static_transformations();

	// Rock
	models.emplace_back(new Rock("res/models/sutr_tmave_sedy.fbx", "res/textures/sutr_tmave_sedy.png", "res/textures/sutr_tmave_sedy_ao.png"));
	models[4]->write_model();
	
} 


void Scene::draw_scene(float angle_x, float angle_y, Terrain& terrain, Camera& camera) const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::mat4 M_scene = glm::mat4(1.0f);
	glm::mat4 P_scene = glm::mat4(1.0f);
	glm::mat4 V_scene = glm::mat4(1.0f);


	//V_scene = glm::lookAt(glm::vec3(-2.0f, 5.0f, 20.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 15.0f, 0.0f));
	V_scene = camera.GetViewMatrix();
	//std::cout << glm::to_string(V_scene) << std::endl;
	glm::vec3 temp_translate;
	glm::vec3 temp_rotate;
	glm::vec3 temp_scale;

	P_scene = glm::perspective(camera.GetZoom(), 1.0f, 0.1f, 100.0f);


	// DRAW TERRAIN
	terrain.draw_terrain(Input(P_scene, V_scene, M_scene, false));

	// DRAW ROAD
	models[0]->draw_relative_to_terrain(Input(terrain.read_model_matrices().P, terrain.read_model_matrices().V, terrain.read_model_matrices().M, false));
	
	// DRAW BUS
	models[1]->draw_model(Input(angle_x, angle_y, terrain.read_model_matrices().P, terrain.read_model_matrices().V, terrain.read_model_matrices().M));
	
	// DRAW TREES
	models[2]->draw_relative_to_terrain(Input(terrain.read_model_matrices().P, terrain.read_model_matrices().V, terrain.read_model_matrices().M, false));

	// DRAW GRASS
	models[3]->draw_relative_to_terrain(Input(angle_x, angle_y, terrain.read_model_matrices().P, terrain.read_model_matrices().V, terrain.read_model_matrices().M));

	// DRAW ROCK
	models[4]->draw_model(Input(terrain.read_model_matrices().P, terrain.read_model_matrices().V, terrain.read_model_matrices().M, false));
	

	
	// COLLISION DETECTION
	/*
	std::cout << "BUS: " << std::endl;
	models[1]->print_rectangle_coords();
	
	if (Utility::check_collision(models[1]->rectangles[0], models[2]->rectangles[0]) == true)
	{
			std::cout << "COLLISION!" << std::endl;
	}

	// cleaning bus so that on each update we can have another set of rectangles (base of model)
	models[1]->rectangles.clear(); 
	 */
	
	glfwSwapBuffers(window);
}
