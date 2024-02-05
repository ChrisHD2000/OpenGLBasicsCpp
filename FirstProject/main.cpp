#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stack>
#include "Utils.h"
#include "Cube.h"
using namespace std;

#define numVAOs 1
#define numVBOs 3

using namespace std;

GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

// Allocate variables used in display() function, so that they won’t need to be allocated during rendering
GLuint mvLoc, projLoc;
int width, height;
float aspect, tfLoc;

glm::mat4 pMat, vMat, mMat, mvMat;
BasicModels::Cube myCube;

void init(GLFWwindow* window) {
	renderingProgram = Utils::createShaderProgram("vertShader.glsl", "fragShader.glsl");
	std::cout << "Initializing VAOs and VBOs..." << std::endl;
	// Initialize Vertex Array Object (VAO) and Vertex Buffer Objects (VBOs)
	glGenVertexArrays(numVAOs, vao);			 // Generate numVAOs = 1 VAO and store its ID all along vao[] array
																				 // So at this point, vao[0] already contains an ID value
	glBindVertexArray(vao[0]);						 // Select the vao[0] ID, the only one we have in this example																				 
																				 // 1. vao[0] SELECTED

	// Generate and bind Vertex Buffer Object (VBO)
	glGenBuffers(numVBOs, vbo);						 // Generate numVBOs = 2 VBOs and store their IDs all along vbo[] array
																				 // So at this point, vbo[0] and vbo[1] already contain an ID value
	glfwGetFramebufferSize(window, &width, &height); // pre compute perspective matrix.
	aspect = (float)width / (float)height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f); // 1.0472 radians = 60 degrees
	myCube.init(vbo[0]);
}

void display(GLFWwindow* window, double currentTime) { // Default Program
	glClear(GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT); //clearing the depth buffer each time through 
																//display() to ensure correct hidden surface removal

	glm::vec3 eye = glm::vec3(10.f, 10.f, -12.f);
	glm::vec3 target = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	vMat = glm::lookAt(eye, target, cameraUp);
	myCube.setupPerspectiveAndView(pMat, vMat);
	myCube.display(currentTime);
}

void window_reshape_callback(GLFWwindow* window, int newWidth, int newHeight) {
	aspect = (float)newWidth / (float)newHeight; // new width&height provided by the callback
	glViewport(0, 0, newWidth, newHeight); // sets screen region associated with framebuffer 
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
}

int main(void) {
	std::cout << "Starting GLFW..." << std::endl;
	// (a) initializes the GLFW library
	if (!glfwInit()) { exit(EXIT_FAILURE); }
			  
	std::cout << "Instantiating window..." << std::endl;
	// (b) instantiates a GLFWwindow
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* window = glfwCreateWindow(600, 600, "Chapter 4 - Managing 3D Graphics Data", NULL, NULL);
	glfwMakeContextCurrent(window);

	std::cout << "Instantiating GLEW library..." << std::endl;
	// (c) initializes the GLEW library
	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);

	// Gets called everytime the Windows is resized
	glfwSetWindowSizeCallback(window, window_reshape_callback);

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