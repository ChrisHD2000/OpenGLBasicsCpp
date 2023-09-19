#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include "Utils.h"

#define numVAOs 1

using namespace std;

GLuint renderingProgram;
GLuint vao[numVAOs];

void init(GLFWwindow* window) {
	renderingProgram = Utils::createShaderProgramTriangle(); 
	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);
}

// variables for movement
float x = 0.0f;
float inc = 0.01f;

void display(GLFWwindow* window, double currentTime) { // Default Program
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT); // clear the background to black, each time

	glUseProgram(renderingProgram); //loads the program containing the 
	//compiled shaders (vertex and fragment shader) into the OpenGL pipeline stages(onto the GPU!)
	// NOTE that it doesn’t run the shaders, it just loads them onto the hardware
	
	//glPointSize(100.0f); // comment this line to let the vertex be on its default size
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	x += inc;						// along X axis
	if (x > 1.0f) inc = -0.01f;		// switch to moving the triangle to the left
	if (x < -1.0f) inc = 0.01f;		// switch to moving the triangle to the right

	GLuint offsetLoc = glGetUniformLocation(renderingProgram, "offset"); // get ptr to "offset" in the vertex shader program
	glProgramUniform1f(renderingProgram, offsetLoc, x); // send value of x to "offset" through offsetLoc

	glDrawArrays(GL_TRIANGLES, 0, 3);
}

//void display(GLFWwindow* window, double currentTime) { // For Points
//	/*2.1 Modify Program 2.2 to add animation that causes the drawn point to grow and
//	shrink, in a cycle.Hint: use the glPointSize() function, with a variable as the
//	parameter.*/
//	glClear(GL_DEPTH_BUFFER_BIT);
//	glClearColor(0.0, 0.0, 0.0, 1.0);
//	glClear(GL_COLOR_BUFFER_BIT); 
//
//	glUseProgram(renderingProgram); 
//
//	x += inc;						
//	if (x > 1.0f) inc = -0.01f;		
//	if (x < -0.0f) inc = 0.01f;		
//	glPointSize(100.0f*x); 
//	glDrawArrays(GL_POINTS, 0, 1);
//}

int main(void) {
	// (a) initializes the GLFW library
	if (!glfwInit()) { exit(EXIT_FAILURE); }

	// (b) instantiates a GLFWwindow
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* window = glfwCreateWindow(600, 600, "Chapter2 - program1", NULL, NULL);
	glfwMakeContextCurrent(window);

	// (c) initializes the GLEW library
	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);

	// (d) calls the function “init()” ONCE
	init(window);

	// (e) calls the function “display()” repeatedly
	while (!glfwWindowShouldClose(window)) {
		display(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}