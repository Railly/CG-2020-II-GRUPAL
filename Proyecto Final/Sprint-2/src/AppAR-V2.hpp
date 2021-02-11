#ifndef APP_HPP
#define APP_HPP
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <SOIL2/SOIL2.h>
#include "Utils.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>
#include <math.h>

#include <learnopengl/shader_m.h>
#include <learnopengl/model.h>

#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>
#include <stack>
#include "cube.h"

#define WIDTH 800
#define HEIGHT 600

// bool loadCameraCalibration(std::string name, cv::Mat& cameraMatrix, cv::Mat& distanceCoefficients);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);


class AppAR {
private:
	//VAOS
	GLuint VBO_bg;
	GLuint VAO_bg;
	GLuint texture_bg;

	// Model Matrices
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 modelMoon1 = glm::mat4(1.0f);
	glm::mat4 modelEarth1 = glm::mat4(1.0f);

	// Modelview Matrices
	glm::mat4 modelview = glm::mat4(1.0f);
	glm::mat4 modelview2 = glm::mat4(1.0f);
	glm::mat4 modelviewMoon = glm::mat4(1.0f);
	glm::mat4 modelviewEarth = glm::mat4(1.0f);
	glm::mat4 modelview_bg;

	// Perspective Matrix
	glm::mat4 projection_perspective;

	// Rotation Matrix
	cv::Mat rot_mat;
	cv::Mat rot_mat2;
	cv::Mat rotationVectors, translationVectors;
	cv::Mat rotationVectors2, translationVectors2;

	// Video Variables
	cv::Mat frame;
	cv::Mat frame2;
	cv::VideoCapture vid;
	std::vector<int> markerIds;
	std::vector<std::vector<cv::Point2f>> markerCorners;
	std::vector<std::vector<cv::Point2f>> rejectedCandidates;
	cv::Ptr<cv::aruco::Dictionary> markerDictionary;
	cv::Ptr<cv::aruco::GridBoard> board;

	std::vector<int> markerIds2;
	std::vector<std::vector<cv::Point2f>> markerCorners2;
	std::vector<std::vector<cv::Point2f>> rejectedCandidates2;
	cv::Ptr<cv::aruco::Dictionary> markerDictionary2;
	cv::Ptr<cv::aruco::GridBoard> board2;

public:
	AppAR() {}
	void init();
	void setup();
	void display();
	void cleanAll();
	bool shouldClose();
	bool loadCameraCalibration(std::string name, cv::Mat& cameraMatrix, cv::Mat& distanceCoefficients);

	GLFWwindow* window;
	GLuint bgProgram;

	Shader* sun;
	Shader* moon;
	Shader* earth;
	Shader* background;
	Shader* lampShader;

	// Models
	Model* modelSun;
	Model* modelMoon;
	Model* modelEarth;
	Model* lampModel;

	// Camera Matrix & Distortion Parameters
	cv::Mat intrinsicMatrix;
	cv::Mat distortionParameters;

	// Ambient light
	glm::vec3 light_color;
	glm::vec3 ambient_light;
	float ambient_strength;

	// Transformation Stack
	std::stack<glm::mat4> mvStack;
	std::stack<glm::mat4> mvStack2;
};

void AppAR::init() {
	// GLFW ====================================================================
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	window = glfwCreateWindow(WIDTH, HEIGHT, "AR-Solar-System", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);

	if (window == nullptr)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		exit(1);
	}
	// GLEW ====================================================================
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		exit(1);
	}

	// GLAD ====================================================================
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		exit(1);
	}
	std::cout << "GL_VERSION: " << glGetString(GL_VERSION) << std::endl;

	glEnable(GL_DEPTH_TEST);

	// Viewport dimensions
	glViewport(0, 0, 800, 600);
}

void AppAR::setup() {

	sun = new Shader("vertShader.glsl", "fragShader.glsl");
	moon = new Shader("vertShader.glsl", "fragShader.glsl");
	earth = new Shader("vertShader.glsl", "fragShader.glsl");
	background = new Shader("bgVertShader.glsl", "bgFragShader.glsl");
	lampShader = new Shader("lampVertShader.glsl", "lampFragShader.glsl");
	bgProgram = Utils::createShaderProgram("bgVertShader.glsl", "bgFragShader.glsl");

	modelSun = new Model("C:\\Users\\Railly\\source\\repos\\AR-Project\\AR-Project\\sun2.obj");
	modelMoon = new Model("C:\\Users\\Railly\\source\\repos\\AR-Project\\AR-Project\\moon3.obj");
	modelEarth = new Model("C:\\Users\\Railly\\source\\repos\\AR-Project\\AR-Project\\earth.obj");
	lampModel = new Model("C:\\Users\\Railly\\source\\repos\\AR-Project\\AR-Project\\cube.obj");

	intrinsicMatrix = cv::Mat::eye(3, 3, CV_64F);

	AppAR::loadCameraCalibration("CAMARA2", intrinsicMatrix, distortionParameters);
	markerDictionary = cv::aruco::getPredefinedDictionary(cv::aruco::PREDEFINED_DICTIONARY_NAME::DICT_6X6_50);
	board = cv::aruco::GridBoard::create(3, 3, 0.04, 0.01, markerDictionary);

	markerDictionary2 = cv::aruco::getPredefinedDictionary(cv::aruco::PREDEFINED_DICTIONARY_NAME::DICT_5X5_50);
	board2 = cv::aruco::GridBoard::create(3, 3, 0.04, 0.01, markerDictionary2);

	vid.open(1);

	if (!vid.isOpened()) {
		exit(1);
	}

	// ----------------------------------------------------------------------------------------------
	// Scaling models

	model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
	modelMoon1 = glm::scale(modelMoon1, glm::vec3(0.05f, 0.05f, 0.05f));
	modelEarth1 = glm::scale(modelEarth1, glm::vec3(0.05f, 0.05f, 0.05f));

	// Initial default value (invisible)

	modelview = glm::translate(model, glm::vec3(0.0f, 0.0f, 10.0f));
	modelviewMoon = glm::translate(model, glm::vec3(0.0f, 0.0f, 10.0f));
	modelviewEarth = glm::translate(model, glm::vec3(0.0f, 0.0f, 10.0f));

	// Defining parameter manually
	float near = 0.2f;
	float far = 400.0f;
	float fx = 612.9770191846128f;
	float fy = 612.9770191846128f;
	float cx = 319.5;
	float cy = 239.5;

	// OpenCV -> OpenGL perspective projection matrix generation
	projection_perspective = { fx / cx,                  0,                   0,                                   0,
											0,                  fy / cy,                0,                                   0,
											0,                     0,     -(far + near) / (far - near),     -(2 * far * near) / (far - near),
											0,                     0,                  -1,                                   0 };


	projection_perspective = glm::transpose(projection_perspective);

	// ----------------------------------------------------------------------------------------------
	// Background object definitions
	GLfloat vertices_bg[] =
	{
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, 0.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f,
	};
	// -----------------------------------------------------------------------------------------------
	// Background object
	glGenVertexArrays(1, &VAO_bg);
	glGenBuffers(1, &VBO_bg);
	glBindVertexArray(VAO_bg);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_bg);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_bg), vertices_bg, GL_STATIC_DRAW);
	// Position Attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Texture Attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0); // Unbind VAO_bg

	// -----------------------------------------------------------------------------------------------
	// Webcam texture
	int width, height;

	vid >> frame;
	width = frame.size().width;
	height = frame.size().height;

	glGenTextures(1, &texture_bg);
	glBindTexture(GL_TEXTURE_2D, texture_bg);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, frame.data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	// --------------------------------------------------------------------------------------------------
	// Transformation presets for the background object (model, view and orthographic projection)
	modelview_bg = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, -399.8, 0, 0, 0, 1 };
	modelview_bg = glm::transpose(modelview_bg);
	modelview_bg = glm::scale(modelview_bg, glm::vec3(798 * cx / fx, 798 * cy / fy, 0));

	// Ambient light
	ambient_strength = 0.2f;
	light_color = glm::vec3(255.0f / 255.0f, 219.0f / 255.0f, 58.0f / 255.9f);
	ambient_light = ambient_strength * light_color;
}

void AppAR::display() {
	glfwPollEvents();

	glClearColor(0.27f, 0.27f, 0.27f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float time = glfwGetTime();

	glm::mat4 modelview = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 10, 0, 0, 0, 1 };
	glm::mat4 modelviewMoon = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 10, 0, 0, 0, 1 };
	glm::mat4 modelviewEarth = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 10, 0, 0, 0, 1 };

	vid >> frame;
	vid >> frame2;
	cv::aruco::detectMarkers(frame, markerDictionary, markerCorners, markerIds);
	cv::aruco::detectMarkers(frame2, markerDictionary2, markerCorners2, markerIds2);

	// Detecting markers 1
	if (markerIds.size() > 0) {
		// Try to find markers
		int valid = estimatePoseBoard(markerCorners, markerIds, board, intrinsicMatrix, distortionParameters, rotationVectors, translationVectors);

		// If OpenCV finds it, then draw the model on top of it.
		if (valid > 0) {
			cv::Rodrigues(rotationVectors, rot_mat);

			modelview = { rot_mat.at<double>(0,0),  rot_mat.at<double>(0,1),  rot_mat.at<double>(0,2),  translationVectors.at<double>(0),
						 -rot_mat.at<double>(1,0), -rot_mat.at<double>(1,1), -rot_mat.at<double>(1,2), -translationVectors.at<double>(1),
						 -rot_mat.at<double>(2,0), -rot_mat.at<double>(2,1), -rot_mat.at<double>(2,2), -translationVectors.at<double>(2),
								   0.0f,                      0.0f,                     0.0f,                   1.0f };
			modelview = glm::transpose(modelview);
			//modelviewEarth = modelview;
		}
	}

	// Detecting markers 2
	if (markerIds2.size() > 0) {
		// Try to find markers
		int valid2 = estimatePoseBoard(markerCorners2, markerIds2, board2, intrinsicMatrix, distortionParameters, rotationVectors2, translationVectors2);

		// If OpenCV finds it, then draw the model on top of it.
		if (valid2 > 0) {
			cv::Rodrigues(rotationVectors2, rot_mat2);

			modelview2 = { rot_mat2.at<double>(0,0),  rot_mat2.at<double>(0,1),  rot_mat2.at<double>(0,2),  translationVectors2.at<double>(0),
						 -rot_mat2.at<double>(1,0), -rot_mat2.at<double>(1,1), -rot_mat2.at<double>(1,2), -translationVectors2.at<double>(1),
						 -rot_mat2.at<double>(2,0), -rot_mat2.at<double>(2,1), -rot_mat2.at<double>(2,2), -translationVectors2.at<double>(2),
								   0.0f,                      0.0f,                     0.0f,                   1.0f };
			modelviewEarth = glm::transpose(modelview2);
			modelviewMoon = modelviewEarth;
		}
	}

	// --------------------------------------------------------------------------------------------------
	// Draw sun
	modelview = glm::scale(modelview, glm::vec3(0.05f, 0.05f, 0.05f));

	// Light position
	glm::vec3 lamp_pos(1.0f, 1.0f, 0.0f);

	sun->use();
	mvStack.push(modelview);
	mvStack.push(mvStack.top());
	mvStack.top() *= glm::translate(glm::mat4(1.0f),
		glm::vec3(2.5f, 3.0f, 3.0f));  // sun position
	mvStack.push(mvStack.top());  // duplicating
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), (float)time,
		glm::vec3(0.0f, 1.0f, 0.0f));  // sun rotation
	mvStack.top() *= glm::scale(glm::mat4(1.0f),
		glm::vec3(1.5f, 1.5f, 1.5f));  // make the sun bigger

	sun->setMat4("model", model);
	sun->setMat4("modelview", mvStack.top());
	sun->setMat4("projection_perspective", projection_perspective);
	sun->setVec3("ambient_light", ambient_light);
	sun->setVec3("lamp_pos", lamp_pos);
	sun->setVec3("light_color", glm::vec3(1.0f, 1.0f, 1.0f));
	sun->setVec3("suzanne_color", glm::vec3(0.4f, 0.4f, 0.4f));
	sun->setVec3("camera_pos", glm::vec3(0.0f, 0.0f, 0.0f));
	modelSun->Draw(sun);

	mvStack.pop();

	// --------------------------------------------------------------------------------
	// Draw earth
	modelviewEarth = glm::scale(modelviewEarth, glm::vec3(0.05f, 0.05f, 0.05f));
	//modelviewEarth = glm::translate(modelviewEarth, glm::vec3(0.4f, 0.4f, 0.0f));

	// Light position
	//glm::vec3 lamp_pos2(sin(time / 2.0f), cos(time / 2.0f), 0.0f);
	glm::vec3 lamp_pos2(1.0f, 1.0f, 0.0f);

	earth->use();
	mvStack2.push(modelviewEarth);
	mvStack2.push(mvStack2.top());
	mvStack2.top() *= glm::translate(glm::mat4(1.0f),
		glm::vec3(2.5f, 3.0f, 3.0f));  // planet position
	mvStack2.push(mvStack2.top());  // duplicating
	mvStack2.top() *= glm::rotate(glm::mat4(1.0f), (float)time,
		glm::vec3(0.0, 0.0, 1.0));  // planet rotation
	mvStack2.top() *= glm::scale(glm::mat4(1.0f),
		glm::vec3(0.8f, 0.8f, 0.8f));  // make the moon smaller

	earth->setMat4("model", modelEarth1);
	earth->setMat4("modelview", mvStack2.top());
	earth->setMat4("projection_perspective", projection_perspective);
	earth->setVec3("ambient_light", ambient_light);
	earth->setVec3("lamp_pos", lamp_pos2);
	earth->setVec3("light_color", glm::vec3(1.0f, 1.0f, 1.0f));
	earth->setVec3("suzanne_color", glm::vec3(0.4f, 0.4f, 0.4f));
	earth->setVec3("camera_pos", glm::vec3(0.0f, 0.0f, 0.0f));
	modelEarth->Draw(earth);

	mvStack2.pop();

	//// --------------------------------------------------------------------------------
	//// Draw moon
	modelviewMoon = glm::scale(modelviewMoon, glm::vec3(0.05f, 0.05f, 0.05f));
	modelviewMoon = glm::translate(modelviewMoon, glm::vec3(0.2f, 0.2f, 0.0f));

	moon->use();
	//.push(modelviewMoon);  // mvMat of planet
	mvStack2.push(mvStack2.top());  // mvMat of planet
	mvStack2.top() *= glm::translate(glm::mat4(1.0f),
		glm::vec3(sin((float)time) * 2.0, cos((float)time) * 2.0,
			0.0f));  // moon position
	mvStack2.push(mvStack2.top());  // duplicating
	mvStack2.top() *= glm::rotate(glm::mat4(1.0f), (float)time,
		glm::vec3(0.0, 0.0, 1.0));  // moon rotation

	mvStack2.top() *= glm::scale(glm::mat4(1.0f),
		glm::vec3(0.50f, 0.50f, 0.50f));  // make the moon smaller
	moon->setMat4("model", modelMoon1);
	moon->setMat4("modelview", mvStack2.top());
	moon->setMat4("projection_perspective", projection_perspective);
	moon->setVec3("ambient_light", ambient_light);
	moon->setVec3("lamp_pos", lamp_pos2);
	moon->setVec3("light_color", glm::vec3(1.0f, 1.0f, 1.0f));
	moon->setVec3("suzanne_color", glm::vec3(0.4f, 0.4f, 0.4f));
	moon->setVec3("camera_pos", glm::vec3(0.0f, 0.0f, 0.0f));
	modelMoon->Draw(moon);

	mvStack2.pop();

	// ---------------------------------------------------------------------------------------------
	// Draw bagckground 
	glUseProgram(bgProgram);
	glBindVertexArray(VAO_bg);

	glActiveTexture(GL_TEXTURE2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame.cols, frame.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, frame.data);
	glBindTexture(GL_TEXTURE_2D, texture_bg);
	glUniform1i(glGetUniformLocation(bgProgram, "webcam_texture"), 2);

	glUniformMatrix4fv(glGetUniformLocation(bgProgram, "modelview_bg"), 1, GL_FALSE, glm::value_ptr(modelview_bg));
	glUniformMatrix4fv(glGetUniformLocation(bgProgram, "perspective_projection_bg"), 1, GL_FALSE, glm::value_ptr(projection_perspective));

	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	// ------------------------------------------------------------------------------------------
	// Draw lamp
	glm::mat4 model_lamp = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, lamp_pos.x, lamp_pos.y, 0.0f, 1.0f };
	model_lamp = glm::scale(model_lamp, glm::vec3(0.1f, 0.1f, 0.1f));

	glm::mat4 modelview_lamp = glm::mat4(1.0f);

	lampShader->use();
	lampShader->setMat4("model_lamp", model_lamp);
	lampShader->setMat4("modelview_lamp", modelview_lamp);
	lampShader->setMat4("projection_lamp", projection_perspective);
	lampModel->Draw(lampShader);

	glfwSwapBuffers(window);
}

void AppAR::cleanAll() {
	glDeleteVertexArrays(1, &VAO_bg);
	glDeleteBuffers(1, &VBO_bg);
	//mvStack.pop();

	//mvStack.pop();
	//mvStack.pop();
	//mvStack.pop();
	glfwDestroyWindow(window);
	glfwTerminate();
}

bool AppAR::shouldClose() {
	return glfwWindowShouldClose(window);
}

inline bool AppAR::loadCameraCalibration(std::string name, cv::Mat& cameraMatrix, cv::Mat& distanceCoefficients){
	std::ifstream inStream(name);
	if (inStream) {
		uint16_t rows;
		uint16_t columns;

		inStream >> rows;
		inStream >> columns;

		cameraMatrix = cv::Mat(cv::Size(columns, rows), CV_64F);

		for (int r = 0; r < rows; r++) {
			for (int c = 0; c < columns; c++) {
				double read = 0.0f;
				inStream >> read;
				cameraMatrix.at<double>(r, c) = read;
				std::cout << cameraMatrix.at<double>(r, c) << "\n";
			}
		}
		//Distance Coefficients
		inStream >> rows;
		inStream >> columns;

		distanceCoefficients = cv::Mat::zeros(rows, columns, CV_64F);

		for (int r = 0; r < rows; r++) {
			for (int c = 0; c < columns; c++) {
				double read = 0.0f;
				inStream >> read;
				distanceCoefficients.at<double>(r, c) = read;
				std::cout << distanceCoefficients.at<double>(r, c) << "\n";
			}
		}
		inStream.close();
		return true;
	}
	return false;
}

inline void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	// When the user presses the escape key, we set the window should close property to true, and close the application.
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

#endif