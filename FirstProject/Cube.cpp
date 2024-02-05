#include "Cube.h"
#include <iostream>
namespace BasicModels {
	void Cube::init(GLuint &vbo){
		renderingProgram = Utils::createShaderProgram("vertShader.glsl", "fragShader.glsl");
		// Cube with 36 vertices, 12 triangles, 6 faces, make 1x1x1 cube placed at origin
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
		std::cout << "Initializing cube and VBOs..." << std::endl;
		glBindBuffer(GL_ARRAY_BUFFER, vbo); // Select the vbo ID as the current GL_ARRAY_BUFFER, we have 2 in this case
																			 // 1. vao[0] SELECTED AND vbo SELECTED by their ID
		// So from now on vao[0] and vbo is the current buffer where our instructions are going to be applied
		// Upload vertex data to the selected VBO. Note that HERE is where we define the size of the buffer
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubePositions), cubePositions, GL_STATIC_DRAW);
		this->vbo = vbo;
	}

	void Cube::display(double& currentTime) {
		glUseProgram(renderingProgram);
		mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
		projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");
		glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(vMat));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glDrawArrays(GL_TRIANGLES, 0, 36); // draw the cube
	}

	void Cube::setupPerspectiveAndView(glm::mat4& perspective, glm::mat4& view){
		pMat = perspective;
		vMat = view;
	}

};