//============================================================================
// Name        :	Tarea 05: IMGUI
// Student     :	Hugo Quispe, Railly Angelo
//			Romero Yañac, Luis
//			Romero Ángeles, Luis
// Version     :
// Description :
//============================================================================
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include "Utils.hpp"
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#define _USE_MATH_DEFINES
#define M_PI 3.14159265358979323846
#define HEIGHT 800
#define WIDTH 800
#define numVAOs 1
#define numVBOs 2

/*--------------- VARIABLES GLOBALES --------------- */
const float toRadians = 3.14159265f / 180.0f;
GLuint renderingProgram;

GLfloat radius = 1;
GLint const numberOfSides = 50;
GLint const numberOfVertices = numberOfSides + 2;
GLfloat twicePi = 2.0f * M_PI;

GLuint vao[numVAOs];
GLuint vbo[numVBOs];

float color_fondo[3] = { 144. / 255.0f, 251. / 255.0f, 232. / 255.0f };
float color_uno[3] = { 40. / 255.0f, 40. / 255.0f, 40. / 255.0f };
float color_dos[3] = { 242. / 255.0f, 239. / 255.0f, 232. / 255.0f };

float up = 0.0f;
float down = 0.0f;
float left = 0.0f;
float right = 0.0f;
float scale = 1.0f;

float curAngle = 0.0f;
float offset = 0.1f;
float increment = 0.1f;

void init(GLFWwindow* window) {
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");
	renderingProgram = Utils::createShaderProgram("vertShader.glsl", "fragShader.glsl");

	GLfloat verticesX[numberOfVertices];
	GLfloat verticesY[numberOfVertices];
	GLfloat verticesZ[numberOfVertices];

	verticesX[0] = 0;
	verticesY[0] = 0;
	verticesZ[0] = 0;

	for (int i = 1; i < numberOfVertices; i++) {
		verticesX[i] = (radius * cos((i - 1) * twicePi / (numberOfSides)+twicePi / 4));
		verticesY[i] = (radius * sin((i - 1) * twicePi / (numberOfSides)+twicePi / 4));
		verticesZ[i] = 0;
	}

	GLfloat vPositions[(numberOfVertices) * 3];

	for (int i = 0; i < numberOfVertices; i++) {
		vPositions[i * 3] = verticesX[i];
		vPositions[i * 3 + 1] = verticesY[i];
		vPositions[i * 3 + 2] = verticesZ[i];
	}

	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);

	glGenBuffers(1, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vPositions), vPositions, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
}

void display(GLFWwindow* window, double currentTime) {
	/*--[FRAME IMGUI]-----------*/
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::SetNextWindowSize(ImVec2(WIDTH * 0.75f, HEIGHT * 0.35f));
	ImGui::SetNextWindowPos(ImVec2(200, 0));
	/*--[FRAME IMGUI]-----------*/

	/*--[CODIGO IMGUI]-----------*/

	ImGuiStyle& style = ImGui::GetStyle();
	style.FrameBorderSize = 1.0f;
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_MenuBar;
	window_flags |= ImGuiWindowFlags_NoCollapse;

	ImVec2 buttonSize = ImVec2(80.0f, 20.f);

	ImGui::Begin("Panel de Control");
		ImGui::TextColored(ImVec4(0.5, 0.9, 0.5, 1), "Background Color: ");
		ImGui::Spacing();

		ImGui::ColorEdit3("Edite el color de fondo", color_fondo);
		ImGui::TextColored(ImVec4(0.5, 0.9, 0.5, 1), "Ying Yang Colors: ");
		ImGui::Spacing();

		ImGui::ColorEdit3("Edite el color 1", color_uno);
		ImGui::ColorEdit3("Edite el color 2", color_dos);
		ImGui::Spacing();

		ImGui::TextColored(ImVec4(0.5, 0.9, 0.5, 1), "Rotation Controls: ");
		ImGui::Spacing();
		ImGui::Indent(190.0f);

		if (ImGui::Button("Rotate ++", buttonSize)) {
			increment = increment + 0.5f;
		}
		ImGui::SameLine();
		if (ImGui::Button("Rotate --", buttonSize)) {
			increment = increment - 0.5f;
		}
		ImGui::Unindent(190.0f);
		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::TextColored(ImVec4(0.5, 0.9, 0.5, 1), "Movement Controls: ");
		ImGui::Spacing();
		ImGui::Indent(150.0f);
		ImGui::Indent(88.0f);
		if (ImGui::Button("Move UP", buttonSize)) {
			up = up + 0.05f;
		}
		ImGui::Unindent(88.0f);
		ImGui::Spacing();

		if (ImGui::Button("Move LEFT", buttonSize)) {
			left = left - 0.05f;
		}
		ImGui::SameLine();
		if (ImGui::Button("Move DOWN", buttonSize)) {
			down = down - 0.05f;
		}
		ImGui::SameLine();
		if (ImGui::Button("Move RIGHT", buttonSize)) {
			right = right + 0.05f;
		}
	ImGui::End();

	curAngle -= increment;
	if (curAngle <= -360)
	{
		curAngle += 360;
	}

	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(color_fondo[0], color_fondo[1], color_fondo[2], 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(renderingProgram);
	glBindVertexArray(vao[0]);

	// se obtienen los uniforms del vertex y el fragment shader
	GLuint uniformColor = glGetUniformLocation(renderingProgram, "u_color");
	GLuint uniformModel = glGetUniformLocation(renderingProgram, "u_model");

	// PINTA CIRCULO NEGRO MAYOR
	glm::mat4 model(1.0f);

	glUniform3f(uniformColor, color_uno[0], color_uno[1], color_uno[2]);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);


	// PINTA SEMICIRCULO BLANCO MAYOR
	model = glm::mat4(1.0f);

	model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.98f, 0.98f, 1.0f));

	glUniform3f(uniformColor, color_dos[0], color_dos[1], color_dos[2]);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices / 2 + 1);


	//PINTA CIRCULO NEGRO MEDIO
	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.49f, 0.49f, 1.0f));
	model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniform3f(uniformColor, color_uno[0], color_uno[1], color_uno[2]);

	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);


	//PINTA CIRCULO BLANCO MEDIO
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	model = glm::translate(model, glm::vec3(0.0f, 2.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniform3f(uniformColor, color_dos[0], color_dos[1], color_dos[2]);

	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);


	// PINTA CIRCULO BLANCO PEQUEÑO
	model = glm::mat4(1.0f);
	model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(0.0f + right + left, -0.49f + up + down, 0.0f));
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 1.0f));

	glUniform3f(uniformColor, color_dos[0], color_dos[1], color_dos[2]);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);


	// PINTA CIRCULO NEGRO PEQUEÑO
	model = glm::mat4(1.0f);
	model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(0.0f + right + left, 0.49f + up + down, 0.0f));
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 1.0f));

	glUniform3f(uniformColor, color_uno[0], color_uno[1], color_uno[2]);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);

	glBindVertexArray(0);
	glUseProgram(0);
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

int main(void) {
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  //
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            //
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); 	// Resizable option.
	//Antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);
	glEnable(GL_MULTISAMPLE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Mouse	", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) {
		exit(EXIT_FAILURE);
	}
	glfwSwapInterval(1);

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
