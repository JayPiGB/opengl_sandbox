#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "shader.h"
#include "error.h"

#pragma region single triangle
//float triangleData[] = {
//	//positions		colors
//	//x, y, z		r  g  b
//	0, 1, 0,		1, 0, 0,  //vertex 1
//	-1, -1, 0,		0, 1, 0,  //vertex 2
//	1, -1, 0,		0, 0, 1   //vertex 3
//};
#pragma endregion

#pragma region two triangles (use index buffer)
float triangleData[] = {
	//positions			colors	
	0.5, 0.5, 0,		1, 0, 0,
	-0.5, 0.5, 0,		0, 1, 0,
	-0.5, -0.5, 0,		0, 0, 1,
	0.5, -0.5, 0,		0, 0, 1,
};

unsigned short indices[] = {
	0, 1, 2, //first triangle
	0, 2, 3  //second triangle
};
#pragma endregion

int main() {
	if (!glfwInit()) {
		std::cout << "GLFW init error!\n";
		return 1;
	}

	GLFWwindow *window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
	if (!window) {
		std::cout << "Window creation error!\n";
		return 1;
	}

	glfwMakeContextCurrent(window);

	//load opengl
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Loading opengl error!\n";
		return 1;
	}

#pragma region shader loading
	Shader shader;

	shader.loadShaderProgramFromFile("resources/uniform_example.vert", "resources/uniform_example.frag");

	GLint u_time = shader.getUniformLocation("u_time"); //get uniform location after loading the shader program
	shader.bind();


	/*glUniform1f(u_time, (float)(clock()) / 100.f);*/

#pragma endregion

#pragma region vertex attribute object (VAO)
	//encapsulates VBO, EBO and Vertex Attribute Pointer in this object instead of the global state
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
#pragma endregion
	
#pragma region vertex object buffet (VBO)
	//opengl will use this variable to store an ID of the buffer on the VRAM
	GLuint buffer;
	glGenBuffers(1, &buffer);

	//its possible to generate multiple buffers at the same time
	//GLuint buffers[10];
	//glGenBuffers(10, buffers);
	
	//only one buffer can be bound at a time. This means that this is the buffer related to all operations from now until another buffer is bound
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	//set the data of the current buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleData), triangleData, GL_STATIC_DRAW);
	//obs: to alter some of the data but not all of it, use glBufferSubData

#pragma endregion

#pragma region attribute pointers
	/*
		The following section instructs OpenGL on how to interpret the data on 'triangleData'
	*/

	//number of bytes between consecutive occurrences of the same vertex attribute
	//think of the size in bytes of the jump to get to the next vertex`s same attribute
	GLsizei stride = sizeof(float) * 6;

	//enables the specified vertex attribute
	//each attribute can contain any type of data the programmer wants, not only restricted to positions, colors etc
	glEnableVertexAttribArray(0);

	//here attribute 0 holds the position data for the triangles
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);

	glEnableVertexAttribArray(1);
	//here attribute 1 holds the color data for the triagles
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * 3));
#pragma endregion

#pragma region index buffer (EBO)
	//create buffer
	GLuint indexBuffer = 0;
	glGenBuffers(1, &indexBuffer);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

#pragma endregion

	glBindVertexArray(0); //good practice to unbind VAO after configuration

	//render loop
	//each iteration called a frame
	while (!glfwWindowShouldClose(window)) {
		int w, h;
		glfwGetWindowSize(window, &w, &h);
		glViewport(0, 0, w, h);

		glClear(GL_COLOR_BUFFER_BIT);

		//set the uniform value on render loop to "animate"
		glUniform1f(u_time, (float)(clock()) / 400.f);

		//bind the VAO to specify the vertex buffer, index buffer and attributes
		glBindVertexArray(vao);
			
		//when drawing single vertex buffer object
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		//when drawing element buffer object
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
