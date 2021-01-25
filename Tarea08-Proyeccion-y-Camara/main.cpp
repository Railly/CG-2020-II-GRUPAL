
#define GLEW_STATIC
#include <GL/glew.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>

#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <iostream>
#include <string>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

const float toRadians = 3.14159265f / 180.0f;

bool activated = true;

// settings
const unsigned int SCR_WIDTH = 1800;
const unsigned int SCR_HEIGHT = 1000;

// camera
Camera camera(glm::vec3(-0.0791466f, 4.71589f, 6.1079f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;


int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetWindowPos(window, SCR_WIDTH/32, SCR_HEIGHT/25);

    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    stbi_set_flip_vertically_on_load(true);

    glEnable(GL_DEPTH_TEST);


    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window);

        float delta = currentFrame - previous;
        previous = currentFrame;

        glClearColor(39.0f/255.0f, 82.0f / 255.0f, 117.0f / 255.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        Model tank("finalTank.obj");
        Model target("finalTarget.obj");
        Model bullet("finalBullet.obj");
        Model canon("finalCanon.obj");

        // Drawing Tank
        tankShader.use();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, 0 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(-0.6f, -1.0f, 3.0f));
        model = glm::translate(model, glm::vec3(0.1f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        tankShader.setMat4("projection", projection);
        tankShader.setMat4("view", view);
        tankShader.setMat4("model", model);
        tank.Draw(tankShader);

        //Drawing Targets
        wallShader.use();
        glm::mat4 modelTarget = glm::mat4(1.0f);
        modelTarget = glm::rotate(modelTarget, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        modelTarget = glm::translate(modelTarget, glm::vec3(30.0f, -1.2f, 0.0f));
        modelTarget = glm::scale(modelTarget, glm::vec3(0.15f, 0.15f, 0.15f));
        wallShader.setMat4("projection", projection);
        wallShader.setMat4("view", view);
        wallShader.setMat4("model", modelTarget);
        target.Draw(wallShader);

        //Drawing Bullet
        bulletShader.use();
        glm::mat4 modelBullet = glm::mat4(1.0f);
        //Handled
        modelBullet = glm::rotate(modelBullet, angleX, glm::vec3(1.0f, 0.0f, 0.0f));
        modelBullet = glm::rotate(modelBullet, angleY, glm::vec3(0.0f, 1.0f, 0.0f));
        modelBullet = glm::rotate(modelBullet, angleZ, glm::vec3(0.0f, 0.0f, 1.0f));

        modelBullet = glm::translate(modelBullet, glm::vec3(0.0f, 0.0f, 0.0f));
        modelBullet = glm::scale(modelBullet, glm::vec3(0.3f, 0.3, 0.3f));
        bulletShader.setMat4("projection", projection);
        bulletShader.setMat4("view", view);

        bulletShader.setMat4("model", modelBullet);
        bullet.Draw(bulletShader);

        // Drawing Canon
        tankShader.use();
        glm::mat4 modelCanon = glm::mat4(1.0f);
        modelCanon = glm::rotate(modelCanon, 0 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        //Handled
        modelCanon = glm::rotate(modelCanon, angleX * 0.9f, glm::vec3(1.0f, 0.0f, 0.0f));
        modelCanon = glm::rotate(modelCanon, angleY * 0.95f, glm::vec3(0.0f, 1.0f, 0.0f));
        modelCanon = glm::rotate(modelCanon, angleZ * 0.9f, glm::vec3(0.0f, 0.0f, 1.0f));

        modelCanon = glm::scale(modelCanon, glm::vec3(1.0f, 1.0f, 1.0f));
        tankShader.setMat4("projection", projection);
        tankShader.setMat4("view", view);
        tankShader.setMat4("model", modelCanon);
        canon.Draw(tankShader);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(yoffset);
}
