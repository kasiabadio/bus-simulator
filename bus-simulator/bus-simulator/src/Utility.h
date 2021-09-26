#ifndef UTILITY_H_
#define UTILITY_H_

#include "Shader.h"

#include <fstream>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>
#include "math.h"

// to draw 3d bounding box we use all x, y, z coordinates
// for SAP collision we use x, z coordinates: an edge coordinates 
struct xyz
{
	float x;
	float y;
	float z;
};


class bounding_box
{
public:
	// lower square
	struct xyz a;
	struct xyz b;
	struct xyz c;
	struct xyz d;

	// upper square
	struct xyz e;
	struct xyz f;
	struct xyz g;
	struct xyz h;

	// NOT NEEDED NOW centre of the cuboid and its "radius" = half of length of 
	//struct xyz centre;
	//float r_x, r_y, r_z;

	// for SAP collision detection, we need to transform 3d points in space
	std::vector<struct xyz> edges;
	
	std::vector<glm::vec4> vertices;
	std::vector<unsigned int> indices;
	
	bounding_box();
	void make_bounding_box();
	void draw_bounding_box(glm::mat4 P, glm::mat4 V, glm::mat4 M);

	// transformations of 3d point
	struct xyz rotate_around_x(float angle, struct xyz point);
	struct xyz rotate_around_y(float angle, struct xyz point);
	struct xyz rotate_around_z(float angle, struct xyz point);
	struct xyz scale(glm::vec3 vector, struct xyz point);
	struct xyz translate(glm::vec3 vector, struct xyz point);

};

namespace Utility
{
	bounding_box create_box(std::string file_name);
};

#endif