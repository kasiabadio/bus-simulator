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
	models.emplace_back(new Road("res/models/Road.obj", "res/textures/RoadDiffuse.png"));
	models[0]->write_model();
	models[0]->write_model_static_transformations();
	
	// Bus
	models.emplace_back(new Bus("res/models/Bus.obj", "res/textures/Bus_tex.tga.png"));
	models[1]->write_model();
	models[1]->box.make_bounding_box();
	
	// Tree
	models.emplace_back(new Tree("res/models/MapleTreeStem.obj", "res/textures/maple_bark.png"));
	models[2]->write_model();
	models[2]->write_model_static_transformations();
	models[2]->box.make_bounding_box();
	
	// Grass
	models.emplace_back(new Grass("res/models/grass variations.obj", "res/textures/Blades of Grass.png"));
	models[3]->write_model();
	models[3]->write_model_static_transformations();
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
	terrain.draw_terrain(Input(P_scene, V_scene, M_scene, false));

	// DRAW ROAD
	models[0]->draw_relative_to_terrain(Input(terrain.read_model_matrices().P, terrain.read_model_matrices().V, terrain.read_model_matrices().M, false));
	
	// DRAW BUS
	models[1]->draw_model(Input(angle_x, angle_y, terrain.read_model_matrices().P, terrain.read_model_matrices().V, terrain.read_model_matrices().M));
	
	// DRAW TREES
	models[2]->draw_relative_to_terrain(Input(terrain.read_model_matrices().P, terrain.read_model_matrices().V, terrain.read_model_matrices().M, true));

	// DRAW GRASS
	models[3]->draw_relative_to_terrain(Input(angle_x, angle_y, terrain.read_model_matrices().P, terrain.read_model_matrices().V, terrain.read_model_matrices().M));

	/*
	std::vector<struct xyz> temp_centre = models[2]->temp_centre;
	std::cout << "-------" << std::endl;
	for (int i = 0; i < temp_centre.size(); i++)
	{
		std::cout << "Numer drzewa: " << i << std::endl;
		std::cout << temp_centre[i].x << " " << temp_centre[i].y << " " << temp_centre[i].z << std::endl;
		std::cout << models[1]->box.centre.x << " " << models[1]->box.centre.y << " " << models[1]->box.centre.z << std::endl;
		models[1]->box.check_collision(temp_centre[i], models[2]->box.r_x);
	}
	std::cout << "-------" << std::endl;
	models[2]->temp_centre.clear();
	*/
	
	/*
	 std::cout << "BUS: " << std::endl << "a: " << models[1]->rectangles[0][0].x << " " <<
		models[1]->rectangles[0][0].y << " " <<
		models[1]->rectangles[0][0].z << " " << std::endl <<

		"b: " << models[1]->rectangles[0][1].x << " " <<
		models[1]->rectangles[0][1].y << " " <<
		models[1]->rectangles[0][1].z << std::endl <<

		"c: " << models[1]->rectangles[0][2].x << " " <<
		models[1]->rectangles[0][2].y << " " <<
		models[1]->rectangles[0][2].z << std::endl <<

		"d: " << models[1]->rectangles[0][3].x << " " <<
		models[1]->rectangles[0][3].y << " " <<
		models[1]->rectangles[0][3].z << std::endl;
	 
	
	std::cout << "bus rectangles size: " << models[1]->rectangles.size() << std::endl; 
	 */
	
	
	for (int i = 0; i < models[2]->rectangles.size(); i++)
	{
		/*
		std::cout << "TREE NR: " << i << std::endl;
		std::cout << std::endl << "a: " << models[2]->rectangles[i][0].x << " " <<
			models[2]->rectangles[i][0].y << " " <<
			models[2]->rectangles[i][0].z << " " << std::endl <<

			"b: " << models[2]->rectangles[i][1].x << " " <<
			models[2]->rectangles[i][1].y << " " <<
			models[2]->rectangles[i][1].z << std::endl <<

			"c: " << models[2]->rectangles[i][2].x << " " <<
			models[2]->rectangles[i][2].y << " " <<
			models[2]->rectangles[i][2].z << std::endl <<

			"d: " << models[2]->rectangles[i][3].x << " " <<
			models[2]->rectangles[i][3].y << " " <<
			models[2]->rectangles[i][3].z << std::endl;
		 */
		
		
	}
	
	// Cleaning so that on each update we can have another set of rectangles (bases of models)
	models[2]->rectangles.clear();
	models[1]->rectangles.clear();
	
	glfwSwapBuffers(window);
}
