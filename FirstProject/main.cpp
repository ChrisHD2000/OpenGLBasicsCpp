#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <fstream>
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

// Texture
GLuint brickTexture;

void setupVertices(void) {
	// model with 270 vertices
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

	float pyrTexCoords[36] = { 
		0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,       0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f, // top and right faces
	  0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,       0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f, // back and left faces
	  0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,       1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f }; // base triangles

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
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidPositions), pyramidPositions, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);   // Select the vbo[1] to map texels within the pyramid
																					 // 1. vao[1] SELECTED AND vbo[0] SELECTED by their ID
	// So from now on vao[0] and vbo[1] is the current buffer where our instructions are going to be applied
	// Upload texture data to the selected VBO. Note that HERE is where we define the size of the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyrTexCoords), pyrTexCoords, GL_STATIC_DRAW);

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
	brickTexture = Utils::loadTexture("Textures/brick1.jpg");

}

float Ux = 0.707f, Uy = 0.f, Uz = 0.707f;
float Vx = 0.f, Vy = 1.f, Vz = 0.f;
float Nx = 0.316f, Ny = 0.f, Nz = 0.948f;


void display(GLFWwindow* window, double currentTime) { // Default Program
	glClear(GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT); //clearing the depth buffer each time through 
																//display() to ensure correct hidden surface removal
	glUseProgram(renderingProgram);

	// get the uniform variables for the MV and projection matrices
	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");

	// Setup camera (view matrix)
	vMat = glm::lookAt(glm::vec3(-5.f, 2.f, 3.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));

	// Setup 3D model matrix
	mMat = glm::translate(glm::mat4(1.0f), glm::vec3(cubeLocX, cubeLocY, cubeLocZ));
	mMat = glm::rotate(mMat, 2.f*(float)currentTime, glm::vec3(0.f, 1.f, 0.f));

	// Setup model - view matrix
	mvMat = vMat * mMat;

	// Pass model view and projection matrix to the vertex shader
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	
	// For the pyramid
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	// Notice that we bind the pyramid and the texture and then we draw
	// For the texture
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	
	glActiveTexture(GL_TEXTURE0); // Actives the texture unit 0, layout (binding=0) uniform sampler2D samp;
	glBindTexture(GL_TEXTURE_2D, brickTexture); // binds the texture with the GLuint value

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, 18);
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