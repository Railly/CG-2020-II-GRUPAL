#include <iostream>
#include <GL\glew.h>
#include <GLFW\glfw3.h>

using namespace std;
 
void init(GLFWwindow* window) {
}

void display(GLFWwindow* window, double currentTime) {
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(16.0f / 255.0f, 131.0f / 255.0f, 218.0f / 255.0f, 1.0);
}

int main(void) {
	if (!glfwInit()) { exit(EXIT_FAILURE); }
    /*-----------OPENGL VERSION 4.3--------------*/
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	GLFWwindow* window = glfwCreateWindow(600, 600, "Hugo Quispe, Railly", NULL, NULL);

	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }

	init(window);

	while (!glfwWindowShouldClose(window)) {
		display(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);
}