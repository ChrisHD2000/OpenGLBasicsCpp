#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
namespace Exceptions {
	void printShaderLog(GLuint shader);
	void printProgramLog(int prog);
	bool checkOpenGLError();
};