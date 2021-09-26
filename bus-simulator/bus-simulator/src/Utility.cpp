#include "Utility.h"

void print_xyz(struct xyz& coords)
{
    std::cout << coords.x << " " << coords.y << " " << coords.z << std::endl;
}

// extracting x, y, z from a .obj file
std::vector<float> find_numbers(std::string numbers_line)
{
    std::stringstream s;
    s << numbers_line;
    std::string temp;
    std::vector<float> result;
    float found;
    
    while (!s.eof())
    {
        s >> temp;
        if (std::stringstream(temp) >> found)
        {
            result.emplace_back(found);
        }
        temp = "";
        
    }
    
    return result;
}


bounding_box Utility::create_box(std::string file_name)
{
    bounding_box temp_box;
    std::ifstream file;
    file.open(file_name);
	
	// now set as: min=infinity , max=(-infinity)
    float x[2]{ std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity() }; //min=0 , max=1
    float y[2]{ std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity() };
    float z[2]{ std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity() };
	
    if (file.is_open())
    {
        std::string line;
        std::vector<float> temp_xyz;
        float temp_x, temp_y, temp_z;
        while (std::getline(file, line))
        {
        	// returns the position of the first occurrence of "v " in the string
            size_t pos = line.find("v ");
            if (pos != -1)
            {
            	// returns a substring of the object, starting at position pos and of length npos
                std::string numbers_line = line.substr(pos + 1, line.size());

            	temp_xyz = find_numbers(numbers_line);
                temp_x = temp_xyz[0];
            	temp_y = temp_xyz[1];
                temp_z = temp_xyz[2];

            	// setting minimum and maximum values of x,y,z 
                if (temp_x < x[0]) x[0] = temp_x;
                else if (temp_x > x[1]) x[1] = temp_x;

                if (temp_y < y[0]) y[0] = temp_y;
                else if (temp_y > y[1]) y[1] = temp_y;

                if (temp_z < z[0]) z[0] = temp_z;
                else if (temp_z > z[1]) z[1] = temp_z;
            }

        }

        // create bounding box using extracted x, y, z values
        temp_box.a.x = x[0]; temp_box.a.y = y[0]; temp_box.a.z = z[1];
        temp_box.b.x = x[1]; temp_box.b.y = y[0]; temp_box.b.z = z[1];
        temp_box.c.x = x[1]; temp_box.c.y = y[0]; temp_box.c.z = z[0];
        temp_box.d.x = x[0]; temp_box.d.y = y[0]; temp_box.d.z = z[0];
        temp_box.e.x = x[0]; temp_box.e.y = y[1]; temp_box.e.z = z[1];
        temp_box.f.x = x[1]; temp_box.f.y = y[1]; temp_box.f.z = z[1];
        temp_box.g.x = x[1]; temp_box.g.y = y[1]; temp_box.g.z = z[0];
        temp_box.h.x = x[0]; temp_box.h.y = y[1]; temp_box.h.z = z[0];

    	/* NOT NEEDED NOW
    	temp_box.r_x = x[1] - x[0];
        temp_box.r_y = y[1] - y[0];
        temp_box.r_z = z[1] - z[0];
    	 
    	temp_box.centre.x = temp_box.r_x / 2;
        temp_box.centre.y = temp_box.r_y / 2;
        temp_box.centre.z = temp_box.r_z / 2;
    	 */
        
        temp_box.edges.emplace_back(temp_box.a);
        temp_box.edges.emplace_back(temp_box.b);
        temp_box.edges.emplace_back(temp_box.c);
        temp_box.edges.emplace_back(temp_box.d);

    	//print_xyz(temp_box.a); print_xyz(temp_box.b); print_xyz(temp_box.c); print_xyz(temp_box.d);
        //print_xyz(temp_box.e); print_xyz(temp_box.f); print_xyz(temp_box.g); print_xyz(temp_box.h);
    	
        file.close();
    }
    else
    {
        std::cout << "Error while reading file\n" << std::endl;
    	
    }
    return temp_box;
}


bounding_box::bounding_box()
{}


void bounding_box::make_bounding_box()
{
    vertices.emplace_back(a.x, a.y, a.z, 1.0); //0
    vertices.emplace_back(b.x, b.y, b.z, 1.0); //1
    vertices.emplace_back(c.x, c.y, c.z, 1.0); //2
    vertices.emplace_back(d.x, d.y, d.z, 1.0); //3
    vertices.emplace_back(e.x, e.y, e.z, 1.0); //4
    vertices.emplace_back(f.x, f.y, f.z, 1.0); //5
    vertices.emplace_back(g.x, g.y, g.z, 1.0); //6
    vertices.emplace_back(h.x, h.y, h.z, 1.0); //7

    indices = {
        5, 1, 4, 1, 0, 4, // f, b, e, b, a, e (front)
    	6, 2, 7, 2, 3, 7, // g, c, h, c, d, h (back)
    	6, 2, 5, 2, 1, 5, // g, c, f, c, b, f (right wall) 
        4, 0, 7, 0, 3, 7, // e, a, h, a, d, h (left wall) 
    	2, 1, 3, 1, 0, 3, // c, b, d, b, a, d (base) 
    	6, 5, 7, 5, 4, 7 // g, f, h, f, e, h (up) 
    };
}

void bounding_box::draw_bounding_box(glm::mat4 P, glm::mat4 V, glm::mat4 M)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    spColored->use();
    glUniformMatrix4fv(spColored->u("P"), 1, false, glm::value_ptr(P));
    glUniformMatrix4fv(spColored->u("V"), 1, false, glm::value_ptr(V));
    glEnableVertexAttribArray(spColored->a("vertex"));
    glEnableVertexAttribArray(spColored->a("color"));

    glUniformMatrix4fv(spColored->u("M"), 1, false, glm::value_ptr(M));

    glVertexAttribPointer(spColored->a("vertex"), 4, GL_FLOAT, false, 0, vertices.data());

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data());

    glDisableVertexAttribArray(spColored->a("vertex"));
    glDisableVertexAttribArray(spColored->a("color"));
	
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}



xyz bounding_box::rotate_around_x(float angle, struct xyz point) 
{
    return { point.x, cos(angle)*point.y - sin(angle)*point.z, sin(angle)*point.y + cos(angle)*point.z };
}

xyz bounding_box::rotate_around_y(float angle, struct xyz point) 
{
    return { cos(angle)*point.x + sin(angle)*point.z, point.y, -sin(angle)*point.x + cos(angle)*point.z };
}

xyz bounding_box::rotate_around_z(float angle, struct xyz point) 
{
    return { cos(angle)*point.x - sin(angle)*point.y, sin(angle)*point.x + cos(angle)*point.y, point.z };
}

xyz bounding_box::scale(glm::vec3 vector, xyz point) 
{
    return { vector.x*point.x, vector.y*point.y, vector.z*point.z };
}

xyz bounding_box::translate(glm::vec3 vector, xyz point) 
{
    return { vector.x+point.x, vector.y+point.y, vector.z+point.z };
}
