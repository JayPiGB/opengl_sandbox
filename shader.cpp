#include "shader.h"
#include <string>
#include <fstream>
#include <iostream>

char* readEntireFile(const char* source) {
	std::ifstream file;
	file.open(source);

	if (!file.is_open()) {
		std::cout << "Error openning file: " << source << "\n";
		return nullptr;
	}

	GLint size = 0;
	file.seekg(0, file.end); //sets the cursor to read characters to the end
	size = file.tellg();	//takes the end position as a size of the whole file string
	file.seekg(0, file.beg);//puts the cursor back to the beginning

	char* fileContent = new char[size] {};

	file.read(fileContent, size);
	file.close();
	return fileContent;
}

bool Shader::loadShaderProgramFromFile(const char* vertexShaderPath, const char* fragmentShaderPath) {
	char* vertexData = readEntireFile(vertexShaderPath);
	char* fragmentData = readEntireFile(fragmentShaderPath);

	if (vertexData == nullptr || fragmentData == nullptr) {
		delete[] vertexData;
		delete[] fragmentData;

		return 0;
	}

	bool result = buildShaderProgramFromData(vertexData, fragmentData);

	delete[] vertexData;
	delete[] fragmentData;

	return result;
}


GLint createAndCompileShaderFromData(const char* data, GLenum shaderType) {
	GLuint shaderId = glCreateShader(shaderType);	//shader object that holds the source code strings that define the shader
	glShaderSource(shaderId, 1, &data, nullptr); //attribute source code to shader object
	glCompileShader(shaderId);	//compiles shader to IR (intermediate representation)

	GLint result = 0;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result); //gets specified parameter of the shader object

	if (result != GL_TRUE) {
		char* message = 0;
		int logLength = 0;

		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);

		if (logLength) {
			message = new char[logLength];

			glGetShaderInfoLog(shaderId, logLength, &logLength, message);	//returns log generated from compiling

			message[logLength - 1] = 0;

			std::cout << data << ":\n" << message << "\n";

			delete[] message;
		}

		else {
			std::cout << data << ":\n" << "unkown error" << "\n";
		}

		glDeleteShader(shaderId);

		shaderId = 0;
		return shaderId;
	}

	return shaderId;
}


bool Shader::buildShaderProgramFromData(const char* vertexShaderData, const char* fragmentShaderData) {
	GLint vertexId = createAndCompileShaderFromData(vertexShaderData, GL_VERTEX_SHADER);
	if (vertexId == 0) {
		glDeleteShader(vertexId);
		return false;
	}

	GLint fragmentId = createAndCompileShaderFromData(fragmentShaderData, GL_FRAGMENT_SHADER);
	if (fragmentId == 0) {
		glDeleteShader(fragmentId);
		return 0;
	}

	programId = glCreateProgram();

	glAttachShader(programId, vertexId);
	glAttachShader(programId, fragmentId);

	glLinkProgram(programId);

	glDeleteShader(vertexId);
	glDeleteShader(fragmentId);

	GLint info = 0;
	glGetProgramiv(programId, GL_LINK_STATUS, &info);

	if (info != GL_TRUE) {
		char* message = 0;
		int   l = 0;

		glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &l);

		message = new char[l];

		glGetProgramInfoLog(programId, l, &l, message);

		message[l - 1] = 0;

		std::cout << "Link error: " << message << "\n";

		delete[] message;

		glDeleteProgram(programId);

		return 0;
	}

	glValidateProgram(programId);

	return true;
}

void Shader::bind()	{
	glUseProgram(programId);
}

void Shader::clear() {
	glDeleteProgram(programId);

	*this = {};
}

GLint Shader::getUniformLocation(const char* name) {
	GLint result = glGetUniformLocation(programId, name);

	if (result == -1) {
		std::cout << "Uniform error: " << name << "\n";
	}

	return result;
}
