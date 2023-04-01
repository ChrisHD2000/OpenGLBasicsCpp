#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define numVAOs 1

using namespace std;

GLuint renderingProgram;
GLuint vao[numVAOs];

GLuint createShaderProgram() {
	// declaring two shaders as character strings called vshaderSourceand fshaderSource
	const char* vshaderSource =
		"#version 430 \n"
		"void main(void) \n"
		"{ gl_Position = vec4(0.0, 0.0, 0.0, 1.0); }";
	const char* fshaderSource =
		"#version 430 \n"
		"out vec4 color; \n"
		"void main(void) \n"
		"{ color = vec4(1.0, 0.0, 0.0, 1.0); }";

	// generates the two shaders of types GL_VERTEX_SHADER and GL_FRAGMENT_SHADER
	// returns an integer ID for each that is an index for referencing it later
	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
	// loads the GLSL code from the strings into the empty shader objects.
	/* glShaderSource() has four parameters: 
	(a) the shader object ID in which to store the shader, 
	(b) the number of strings in the shader source code, note that the two calls specify the number of lines of 
	code in each shader as being “1”, 
	(c) an array of pointers to strings containing the source code, and 
	(d) an additional parameter we aren’t using */
	glShaderSource(vShader, 1, &vshaderSource, NULL);
	glShaderSource(fShader, 1, &fshaderSource, NULL);

	//The shaders are then each compiled using glCompileShader()
	glCompileShader(vShader);
	glCompileShader(fShader);

	// saves the integer ID that points to it
	GLuint vfProgram = glCreateProgram();
	glAttachShader(vfProgram, vShader);
	glAttachShader(vfProgram, fShader);
	// request that the GLSL compiler ensure that they are compatible
	glLinkProgram(vfProgram);
	return vfProgram;
}
void init(GLFWwindow* window) {
	renderingProgram = createShaderProgram();
	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);
}
void display(GLFWwindow* window, double currentTime) {
	glUseProgram(renderingProgram);
	glPointSize(30.0f); // comment this line to let the vertex be on its default size
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_POINTS, 0, 1);
}
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