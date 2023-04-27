#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>


#define numVAOs 1

using namespace std;

GLuint renderingProgram;
GLuint vao[numVAOs];

string readShaderSource(const char* filePath) {
	string content;
	ifstream fileStream(filePath, ios::in);
	string line = "";
		while (!fileStream.eof()) {
			getline(fileStream, line);
			content.append(line + "\n");
		}
	fileStream.close();
	return content;
}


GLuint createShaderProgram() {

	// generates the two shaders of types GL_VERTEX_SHADER and GL_FRAGMENT_SHADER
	// returns an integer ID for each that is an index for referencing it later
	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

	// declaring two shaders as strings called vertShaderSource and fragShaderSource
	std::string vertShaderSource = readShaderSource("vertShader.glsl");
	std::string fragShaderSource = readShaderSource("fragShader.glsl");

	const char* vertShaderSrc = vertShaderSource.c_str();
	const char* fargShaderSrc = fragShaderSource.c_str();

	glShaderSource(vShader, 1, &vertShaderSrc, NULL);
	glShaderSource(fShader, 1, &fargShaderSrc, NULL);

	// loads the GLSL code from the strings into the empty shader objects.
	/* glShaderSource() has four parameters: 
	(a) the shader object ID in which to store the shader, 
	(b) the number of strings in the shader source code, note that the two calls specify the number of lines of 
	code in each shader as being “1”, 
	(c) an array of pointers to strings containing the source code, and 
	(d) an additional parameter we aren’t using */
	glShaderSource(vShader, 1, &vertShaderSrc, NULL);
	glShaderSource(fShader, 1, &fargShaderSrc, NULL);

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

// variables for movement

float x = 0.0f;
float inc = 0.01f;

void display(GLFWwindow* window, double currentTime) {
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT); // clear the background to black, each time

	glUseProgram(renderingProgram);
	//glPointSize(100.0f); // comment this line to let the vertex be on its default size
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	x += inc;						// along X axis
	if (x > 1.0f) inc = -0.01f;		// switch to moving the triangle to the left
	if (x < -1.0f) inc = 0.01f;		// switch to moving the triangle to the right

	GLuint offsetLoc = glGetUniformLocation(renderingProgram, "offset"); // get ptr to "offset" in the vertex shader program
	glProgramUniform1f(renderingProgram, offsetLoc, x); // send value of x to "offset" through offsetLoc

	glDrawArrays(GL_TRIANGLES, 0, 3);
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