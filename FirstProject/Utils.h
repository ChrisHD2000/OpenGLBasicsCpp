#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
namespace Utils {	
	GLuint createShaderProgram();
	std::string readShaderSource(const char* filePath);
}
