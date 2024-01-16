#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
namespace Utils {		
	GLuint createShaderProgramTriangle();
	GLuint createShaderProgramPoint();
	std::string readShaderSource(const char* filePath);
	namespace Logging {
		enum SettingType {
			FRAME_RATE, 
			FRAME_DURATION,
			FRAME_DURATION_INCREMENT};
		void logFrameSettings(float& currentTime, SettingType& type);
	} // namespace Logging
} // namespace Utils
