
#include "Utils.h"
#include "Exceptions.h"
namespace Utils {
	namespace Logging {
		void logFrameSettings(float& currentTime, SettingType& type) {
			static double intialTime = 0;
			static float previousFrameDuration = 0;

			// Calculates time since last frame or frame duration in seconds
			float frameDuration = currentTime - intialTime;
			float frameRate = 1 / frameDuration; // frame Duration sec. -> 1 frame (s)
													// 1 sec.	             -> ? frame (s)
			float timeIncrement = frameDuration - previousFrameDuration;
			if (type == SettingType::FRAME_RATE)
				std::cout << frameRate << " FPS" << std::endl;
			else if (type == SettingType::FRAME_DURATION)
				std::cout << frameDuration << " sec." << std::endl;
			else if (type == SettingType::FRAME_DURATION_INCREMENT)
				std::cout << ((timeIncrement >= 0.0f) ? "+" : "") << timeIncrement << " sec." << std::endl;

			// Time stamp for the next iteration calculation.
			previousFrameDuration = frameDuration;
			intialTime = currentTime;
		}
	} // Logging
} // Utils

namespace Utils {
	GLuint createShaderProgram(const char* vertexShaderProgram, const char* fragmentShaderProgram) {
		// catch errors while compiling shaders
		GLint vertCompiled;
		GLint fragCompiled;
		GLint linked;

		// generates the two shaders of types GL_VERTEX_SHADER and GL_FRAGMENT_SHADER
		// returns an integer ID for each that is an index for referencing it later
		GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
		GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

		// declaring two shaders as strings called vertShaderSource and fragShaderSource
		std::string vertShaderSource = readShaderSource(vertexShaderProgram);
		std::string fragShaderSource = readShaderSource(fragmentShaderProgram);

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
		Exceptions::checkOpenGLError();
		glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);
		if (vertCompiled != 1) {
			cout << "vertex compilation failed" << endl;
			Exceptions::printShaderLog(vShader);
		}

		glCompileShader(fShader);
		Exceptions::checkOpenGLError();
		glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);
		if (fragCompiled != 1) {
			cout << "fragment compilation failed" << endl;
			Exceptions::printShaderLog(fShader);
		}

		// saves the integer ID that points to it
		GLuint vfProgram = glCreateProgram();
		glAttachShader(vfProgram, vShader);
		glAttachShader(vfProgram, fShader);
		// request that the GLSL compiler ensure that they are compatible
		glLinkProgram(vfProgram);
		Exceptions::checkOpenGLError();
		glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
		if (linked != 1) {
			cout << "linking failed" << endl;
			Exceptions::printProgramLog(vfProgram);
		}
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

} // namespace Utils