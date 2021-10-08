
#include "Shader.h"

#include <iostream>
#include <malloc.h>

ShaderProgram *spLambert;
ShaderProgram *spConstant;
ShaderProgram* spColored;
ShaderProgram* spTextured;
ShaderProgram* spLambertTextured;
ShaderProgram* spLambertMultiTextured;



void initShaders() {
	spLambert = new ShaderProgram("res/shaders/v_lambert.glsl", NULL, "res/shaders/f_lambert.glsl");
	spConstant = new ShaderProgram("res/shaders/v_constant.glsl", NULL, "res/shaders/f_constant.glsl");
	spTextured = new ShaderProgram("res/shaders/v_textured.glsl", NULL, "res/shaders/f_textured.glsl");
	spColored = new ShaderProgram("res/shaders/v_colored.glsl", NULL, "res/shaders/f_colored.glsl");
	spLambertTextured = new ShaderProgram("res/shaders/v_lamberttextured.glsl", NULL, "res/shaders/f_lamberttextured.glsl");
	spLambertMultiTextured = new ShaderProgram("res/shaders/v_lambertmultitextured.glsl", NULL, "res/shaders/f_lambertmultitextured.glsl");
}

void freeShaders() {
	delete spLambert;
	delete spConstant;
	delete spTextured;
	delete spColored;
	delete spLambertTextured;
	delete spLambertMultiTextured;
}


// Reads shaders files
char* ShaderProgram::readFile(const char* fileName) {
	int filesize;
	FILE *plik;
	char* result;

	#pragma warning(suppress : 4996)
	
	// rb is an option you should use when opening non-text files
	plik = fopen(fileName,"rb");
	if (plik != NULL) {
		fseek(plik, 0, SEEK_END);
		filesize = ftell(plik);
		fseek(plik, 0, SEEK_SET);
		result = new char[filesize + 1];

		#pragma warning(suppress : 6386) 
		int readsize = fread(result, 1, filesize, plik);
		//std::cout << "readsize: " << readsize << std::endl;
		result[filesize] = 0;
		fclose(plik);

		return result;
	}

	return NULL;

}

// A method which reads and compiles shader, and then returns its handle
GLuint ShaderProgram::compile_shader(GLenum _shader_type, const char* _filename) {

	// Generate handle for a shader
	GLuint shader_id = glCreateShader(_shader_type);
	const GLchar* shader_src = readFile(_filename);
	
	glShaderSource(shader_id, 1, &shader_src, NULL);
	glCompileShader(shader_id);
	
	int result;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);
		
		// alloca allocates memory on a stack dynamically
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(shader_id, length, &length, message);

		std::cout << "Failed to compile ";
		if (_shader_type == GL_VERTEX_SHADER)
		{
			std::cout << "vertex shader!" << std::endl;
		}
		else if (_shader_type == GL_GEOMETRY_SHADER)
		{
			std::cout << "geometry shader!" << std::endl;
		}
		else
		{
			std::cout << "fragment shader!" << std::endl;
		}
		std::cout << message << std::endl;

		glDeleteShader(shader_id);
		return 0;
	}
	
	return shader_id;
}


ShaderProgram::ShaderProgram(const char* vertexShaderFile, const char* geometryShaderFile, const char* fragmentShaderFile) {

	shaderProgram = glCreateProgram();
	
	vertexShader = compile_shader(GL_VERTEX_SHADER, vertexShaderFile);
	fragmentShader = compile_shader(GL_FRAGMENT_SHADER, fragmentShaderFile);

	if (geometryShaderFile != NULL) {
		geometryShader = compile_shader(GL_GEOMETRY_SHADER, geometryShaderFile);
	}
	else {
		geometryShader = 0;
	}
	
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	if (geometryShaderFile != NULL) glAttachShader(shaderProgram,geometryShader);
	glLinkProgram(shaderProgram);

	// Checking whether linking was successful
	int result;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &length);

		// alloca allocates memory on a stack dynamically
		char* message = (char*)alloca(length * sizeof(char));
		glGetProgramInfoLog(shaderProgram, length, &length, message);

		std::cout << message << std::endl;

		glDeleteProgram(shaderProgram);
		if (geometryShaderFile != NULL) glDeleteShader(geometryShader);
	}
	
}

ShaderProgram::~ShaderProgram() {

	glDetachShader(shaderProgram, vertexShader);
	if (geometryShader != 0) glDetachShader(shaderProgram, geometryShader);
	glDetachShader(shaderProgram, fragmentShader);

	glDeleteShader(vertexShader);
	if (geometryShader != 0) glDeleteShader(geometryShader);
	glDeleteShader(fragmentShader);

	glDeleteProgram(shaderProgram);
}


void ShaderProgram::use() {
	glUseProgram(shaderProgram);
}

GLuint ShaderProgram::u(const char* variableName) {
	return glGetUniformLocation(shaderProgram, variableName);
}

GLuint ShaderProgram::a(const char* variableName) {
	return glGetAttribLocation(shaderProgram,variableName);
}
