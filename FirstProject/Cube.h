#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Utils.h"

namespace BasicModels {
	class Cube {
	private:
		glm::float32 X, Y, Z;
		GLuint vbo, renderingProgram,  mvLoc, projLoc;
		glm::mat4 vMat;
		glm::mat4 pMat;
	public:
		Cube() : X(0), Y(0), Z(0) {}
		Cube(float x, float y, float z) : X(x), Y(y), Z(z) {}
		~Cube() {};
		void init(GLuint& vbo);
		void display(double &currentTime);
		void setupPerspectiveAndView(glm::mat4 &perspective, glm::mat4 &view);


	};
}
