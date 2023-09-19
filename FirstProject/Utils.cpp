
#include "Utils.h"

namespace Utils {

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

	std::string readShaderSource(const char* filePath) {
		std::string content;
		ifstream fileStream(filePath, ios::in);
		string line = "";
		while (!fileStream.eof()) {
			getline(fileStream, line);
			content.append(line + "\n");
		}
		fileStream.close();
		return content;
	}
}