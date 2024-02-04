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
#define numVBOs 3

using namespace std;

float cameraX, cameraY, cameraZ;
float cubeLocX, cubeLocY, cubeLocZ;
float pyrLocX, pyrLocY, pyrLocZ;
GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

// Allocate variables used in display() function, so that they won’t need to be allocated during rendering
GLuint mvLoc, projLoc;
int width, height;
float aspect, tfLoc;
glm::mat4 pMat, vMat, mMat, mvMat;

void setupVertices(void) { 
	// cube with 36 vertices, 12 triangles, 6 faces, make 2x2x2 cube placed at origin
	float cubePositions[108] = {
		// Face 1

	 -1.0f,	 1.0f, -1.0f,
	 -1.0f, -1.0f, -1.0f,
	  1.0f, -1.0f, -1.0f,

	  1.0f, -1.0f, -1.0f,
	  1.0f,  1.0f, -1.0f, 
	 -1.0f,  1.0f, -1.0f,

	 // Face 2

	  1.0f, -1.0f, -1.0f, 
	  1.0f, -1.0f,  1.0f,
	  1.0f,  1.0f, -1.0f,

		1.0f, -1.0f,  1.0f, 
		1.0f,  1.0f,  1.0f, 
		1.0f,  1.0f, -1.0f,

		// Face 3

		1.0f, -1.0f,  1.0f, 
	 -1.0f, -1.0f,  1.0f, 
		1.0f,  1.0f,  1.0f,

   -1.0f, -1.0f,  1.0f, 
	 -1.0f,  1.0f,  1.0f, 
		1.0f,  1.0f,  1.0f,

		// Face 4

	 -1.0f, -1.0f,  1.0f, 
	 -1.0f, -1.0f, -1.0f, 
	 -1.0f,  1.0f,  1.0f,

	 -1.0f, -1.0f, -1.0f, 
	 -1.0f,  1.0f, -1.0f, 
	 -1.0f,  1.0f,  1.0f,

	 // Face 5

	 -1.0f, -1.0f,  1.0f, 
		1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,

		1.0f, -1.0f, -1.0f,
	 -1.0f, -1.0f, -1.0f,
	 -1.0f, -1.0f,  1.0f,

	 // Face 6

	 -1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,

		1.0f,  1.0f,  1.0f,
	 -1.0f,  1.0f,  1.0f,
	 -1.0f,  1.0f, -1.0f
	};
	// pyramid with 18 vertices, comprising 6 triangles (four sides, and two on the bottom)
	float pyramidPositions[54] = { 
		// front face
		-1.0f, -1.0f, 1.0f, 
		 1.0f, -1.0f, 1.0f, 
		 0.0f, 1.0f, 0.0f, 
		 
		// right face
		 1.0f, -1.0f, 1.0f, 
		 1.0f, -1.0f, -1.0f, 
		 0.0f, 1.0f, 0.0f, 

		// back face
		 1.0f, -1.0f, -1.0f, 
		-1.0f, -1.0f, -1.0f, 
		 0.0f, 1.0f, 0.0f,

		// left face
	  -1.0f, -1.0f, -1.0f, 
		-1.0f, -1.0f, 1.0f, 
		 0.0f, 1.0f, 0.0f, 

		 // base – left front
	  -1.0f, -1.0f, -1.0f, 
		 1.0f, -1.0f, 1.0f, 
		-1.0f, -1.0f, 1.0f, 
		
		 // base – right back
	   1.0f, -1.0f, 1.0f, 
		-1.0f, -1.0f, -1.0f, 
		 1.0f, -1.0f, -1.0f 
	};
	// model with 270 vertices (counting with the repeated ones)
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubePositions), cubePositions, GL_STATIC_DRAW);
	
	//VBO 2
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]); // Select the vbo[0] ID as the current GL_ARRAY_BUFFER, we have 2 in this case
																				 // 1. vao[0] SELECTED AND vbo[1] SELECTED by their ID
	// So from now on vao[0] and vbo[1] is the current buffer where our instructions are going to be applied
	// Upload vertex data to the selected VBO. Note that HERE is where we define the size of the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidPositions), pyramidPositions, GL_STATIC_DRAW);

	//VBO 3
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(modelPositions), modelPositions, GL_STATIC_DRAW);
}

void init(GLFWwindow* window) {
	renderingProgram = Utils::createShaderProgram("vertShader.glsl", "fragShader.glsl");
	cameraX = 0.0f; cameraY = 0.0f; cameraZ = 20.0f;
	cubeLocX = 0.0f; cubeLocY = -2.0f; cubeLocZ = 0.0f; // shift down Y to reveal perspective
	pyrLocX = 0.0f; pyrLocY = 2.0f; pyrLocZ = 0.0f; // shift down Y to reveal perspective
	setupVertices();
	// pre compute perspective matrix.
	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f); // 1.0472 radians = 60 degrees

}

std::stack<glm::mat4> mvStack;
void display(GLFWwindow* window, double currentTime) { // Default Program
	glClear(GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT); //clearing the depth buffer each time through 
																//display() to ensure correct hidden surface removal
	glUseProgram(renderingProgram);

	// get the uniform variables for the MV and projection matrices
	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");

	// push view matrix onto the stack
	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
	mvStack.push(vMat);

	// ---------------------- pyramid == sun --------------------------------------------
	mvStack.push(mvStack.top());
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)); // sun position
	mvStack.push(mvStack.top());
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(1.0f, 0.0f, 0.0f)); // sun rotation

	// draw the sun (use buffer #1)
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, 18); // draw the sun
	mvStack.pop(); // remove the sun’s axial rotation from the stack

	//----------------------- model == planet2 ---------------------------------------------
	mvStack.push(mvStack.top());
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(-sin((float)currentTime) * 8.0, 0.0f, cos((float)currentTime) * 8.0));
	mvStack.push(mvStack.top());
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(0.0, 1.0, 0.0));
	mvStack.push(mvStack.top());
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));
	
	// draw the planet (use buffer #3)
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, 270);
	mvStack.pop(); // remove last scale
	mvStack.pop(); // remove last rotation
	mvStack.pop(); // remove last position
	// we have sun's position back in the top

	//----------------------- cube == planet ---------------------------------------------
	mvStack.push(mvStack.top());
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(sin((float)currentTime) * 4.0, 0.0f, cos((float)currentTime) * 4.0));
	mvStack.push(mvStack.top());
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(0.0, 1.0, 0.0));

	// draw the planet (use buffer #0)
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	mvStack.pop();

	//----------------------- small cube == moon ---------------------------------------------
	mvStack.push(mvStack.top());
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, sin((float)currentTime) * 2.0, cos((float)currentTime) * 2.0));
	mvStack.push(mvStack.top());
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(0.0, 0.0, 1.0)); // moon rotation
	mvStack.push(mvStack.top());
	mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(0.25f, 0.25f, 0.25f)); // make the moon smaller


	// draw the moon (use buffer #0)
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	// remove moon scale / rotation / position, planet position, sun position, and view matrices from stack
	//mvStack.pop(); mvStack.pop(); mvStack.pop(); mvStack.pop();
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