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
using namespace std;

#define numVAOs 1
#define numVBOs 2

using namespace std;

float cameraX, cameraY, cameraZ;
float cubeLocX, cubeLocY, cubeLocZ;
GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

// Allocate variables used in display() function, so that they won’t need to be allocated during rendering
GLuint mvLoc, projLoc;
int width, height;
float aspect, tfLoc;
glm::mat4 pMat, vMat, mMat, rMat, sMat, tMat, mvMat;

void setupVertices(void) {
	// model with 270 vertices
	float modelPositions[270] = {
		// Face 1
	 -1.02947f, 0.377139f, -0.377139f,
	 -1.02947f,	0.377139f, 0.377139f,
	 -0.377139f,0.377139f, 0.377139f,

	 -1.02947f, 0.377139f, -0.377139f,
	 -0.377139f,0.377139f, 0.377139f,
	 -0.377139, 0.377139f, -0.377139f,

	 // Face 2
	-1.02947f, 0.377139f, -0.377139,
	-0.377139, 0.377139f, -0.377139f,
	-0.377139f,-0.377139f,-0.377139f,

	-1.02947f, -0.377139f, -0.377139f,
	-1.02947f, 0.377139f,  -0.377139f,
	-0.377139f, -0.377139f,-0.377139f,

		// Face 3
	-1.02947f, 0.377139f, 0.377139f,
	-1.02947f, -0.377139f,0.377139f,
	-1.02947f, 0.377139f, -0.37581f,

	-1.02947f, 0.377139f,  -0.37581f,
	-1.02947f, -0.377139f,  0.377139f,
	-1.02947f, -0.377139f, -0.377139f,

		// Face 4
	 -0.377139, 0.377139f, 0.377139f,
	 -0.377139f, -0.377139f, 0.377139f,
	 -1.02947f,  0.377139f,  0.377139f,

	 -1.02947f, 0.377139f, 0.377139f,
	 -0.377139f,-0.377139f,0.377139f,
	 -1.02947f, -0.377139f,  0.377139f,

	 // Face 5
	 -0.377139f, 1.02947f, -0.377139f,
	 -0.377139f, 1.02947f, 0.377139f,
	 -0.377139f, 0.377139f, 0.377139f,

	 -0.377139f, 1.02947f, -0.377139f,
	 -0.377139f, 0.377139f, 0.377139f,
	 -0.377139f, 0.377139f, -0.377139f,

	 // Face 6
	 0.377139f,  1.02947f, -0.377139f,
	-0.377139f,  1.02947f, -0.377139f,
	-0.377139f,  0.377139f,-0.377139f,

	0.377139f,  1.02947f, -0.377139f,
	-0.377139f,  0.377139f,-0.377139f,
	 0.377139f,  0.377139f, -0.377139f,

	 // face 7 
	 -0.377139f, 1.02947f, -0.377139f,
	 0.377139f, 1.02947f, 0.377139f,
	-0.377139f, 1.02947f, 0.377139f,
	 
	0.377139f, 1.02947f, 0.377139f,
	 -0.377139f, 1.02947f, -0.377139f,
		0.377139f, 1.02947f, -0.377139f,
		
		// face 8 
		0.377139f, 1.02947f, 0.377139f,
		0.377139f, 1.02947f, -0.377139f,
		0.377139f, 0.377139f, -0.377139f,
	 
		0.377139f, 1.02947f, 0.377139f,
		0.377139f, 0.377139f,-0.377139f,
		0.377139f, 0.377139f, 0.377139f,
	 
		// face 9 
		-0.377139f, 1.02947f, 0.377139f,
		0.377139f, 1.02947f, 0.377139f,
		0.377139f, 0.377139f, 0.377139f,
	 
		-0.377139f, 1.02947f, 0.377139f,
		0.377139f, 0.377139f, 0.377139f,
		-0.377139f, 0.377139f, 0.377139f,
				
		// face 10 
	 -0.377139f, 0.377139f, 0.377139f,
		0.377139f, 0.377139f, 0.377139f,
		0.377139f,-0.377139f, 0.377139f,
	 
		-0.377139f, 0.377139f, 0.377139f,
		0.377139f, -0.377139f, 0.377139f,
		-0.377139f, -0.377139f, 0.377139f,
				
		// face 11
		0.377139f, 0.377139f, 0.377139f,
		1.02947f, -0.377139f, 0.377139f,
		0.377139f, -0.377139f, 0.377139f,
	 
		 0.377139f, 0.377139f, 0.377139f,
		 1.02947f, 0.377139f, 0.377139f,
		 1.02947f, -0.377139f, 0.377139f,
				
		// face 12 
		 0.377139f, 0.377139f, 0.377139f,
		 0.377139f, 0.377139f, -0.377139f,
		 1.02947f, 0.377139f, -0.377139f,
	 
		0.377139f, 0.377139f, 0.377139f,
		1.02947f, 0.377139f, -0.377139f,
		1.02947f, 0.377139f, 0.377139f,
				
		// face 13 
		1.02947f, 0.377139f, 0.377139f,
		1.02947f, 0.377139f, -0.377139f,
		1.02947f,-0.377139f,-0.377139f,
	 
		1.02947f, 0.377139f, 0.377139f,
		1.02947f, -0.377139f, -0.377139f,
		1.02947f, -0.377139f, 0.377139f,
				
		// face 14
		1.02947f, 0.377139f, -0.377139f,
		0.377139f, 0.377139f, -0.377139f,
		0.377139f, -0.377139f, -0.377139f,
	 
		1.02947f, 0.377139f, -0.377139f,
		0.377139, -0.377139f, -0.377139f,
		1.02947f, -0.377139f, -0.377139f,
				
		// face 15 
		 0.377139f, 0.377139f, -0.377139f,
		-0.377139f, 0.377139f, -0.377139f,
		-0.377139f, -0.377139f, -0.377139f,
	 
		 0.377139f, 0.377139f, -0.377139f,
		 -0.377139f, -0.377139f, -0.377139f,
		 0.377139f, -0.377139f, -0.377139f,
	};
	// this is working
	std::cout << "Initializing VAOs and VBOs..." << std::endl;
	// Initialize Vertex Array Object (VAO) and Vertex Buffer Objects (VBOs)
	glGenVertexArrays(numVAOs, vao);			 // Generate numVAOs = 1 VAO and store its ID all along vao[] array
																				 // So at this point, vao[0] already contains an ID value
	glBindVertexArray(vao[0]);						 // Select the vao[0] ID, the only one we have in this example																				 
																				 // 1. vao[0] SELECTED

	// Generate and bind Vertex Buffer Object (VBO)
	glGenBuffers(numVBOs, vbo);						 // Generate numVBOs = 2 VBOs and store their IDs all along vbo[] array
																				 // So at this point, vbo[0] and vbo[1] already contain an ID value

	//VBO 1
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); // Select the vbo[0] ID as the current GL_ARRAY_BUFFER, we have 2 in this case
																				 // 1. vao[0] SELECTED AND vbo[0] SELECTED by their ID
	// So from now on vao[0] and vbo[0] is the current buffer where our instructions are going to be applied
	// Upload vertex data to the selected VBO. Note that HERE is where we define the size of the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(modelPositions), modelPositions, GL_STATIC_DRAW);
}
	
void init(GLFWwindow* window) {
	renderingProgram = Utils::createShaderProgram("vertShader.glsl", "fragShader.glsl");
	cameraX = 0.0f; cameraY = 0.0f; cameraZ = 7.0f;
	cubeLocX = 0.0f; cubeLocY = 0.0f; cubeLocZ = 0.0f; // shift down Y to reveal perspective
	setupVertices();
	// pre compute perspective matrix.
	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f); // 1.0472 radians = 60 degrees

}

//float Ux =  0.707f, Uy = 0.f,  Uz = 0.707f;
//float Vx =  0.f,    Vy = 1.f,  Vz = 0.f;
//float Nx = -0.707f, Ny = 0.f,  Nz = 0.707f;

float Ux =  0.707f, Uy = 0.f,  Uz = 0.707f;
float Vx =  0.f,    Vy = 1.f,  Vz = 0.f;
float Nx = 0.316f, Ny = 0.f,  Nz = 0.948f;


void display(GLFWwindow* window, double currentTime) { // Default Program
	glClear(GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT); //clearing the depth buffer each time through 
																//display() to ensure correct hidden surface removal
	glUseProgram(renderingProgram);

	// get the uniform variables for the MV and projection matrices
	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");

	// Setup camera (view matrix)
	
	tMat = glm::mat4(
										1.f, 0.f, 0.f, 0.f,		
										0.f, 1.f, 0.f, 0.f,
										0.f, 0.f, 1.f, 0.f,
										0.f, 0.f, -10.f, 1.f); // This translation
	//rMat = glm::rotate(glm::mat4(1.0f), 0.758f, glm::vec3(0.f, 1.f, 0.f)); // This rotation
	rMat = glm::mat4(
		Ux,  Vx,  Nx,  0.f,
		Uy,  Vy,  Ny,  0.f,
		Uz,  Vz,  Nz,  0.f,
		0.f, 0.f, 0.f, 1.f); // This rotation
	
	vMat = tMat * rMat;

	// Setup 3D model matrix
	mMat = glm::translate(glm::mat4(1.0f), glm::vec3(cubeLocX, cubeLocY, cubeLocZ));

	// Setup model - view matrix
	mvMat = vMat * mMat;

	// draw the planet (use buffer #0)
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, 270);
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