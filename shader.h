#pragma once
#include <glad/glad.h>
struct Shader {
	GLuint programId = 0;

	bool loadShaderProgramFromFile(const char* vertexShaderPath,
		const char* fragmentShaderPath);

	bool buildShaderProgramFromData(const char* vertexShaderData,
		const char* fragmentShaderData);

	void bind();

	void clear();

	GLint getUniformLocation(const char* name);
};