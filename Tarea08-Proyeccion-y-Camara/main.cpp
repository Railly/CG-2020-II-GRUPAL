
#define GLEW_STATIC
#include <GL/glew.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>

#include <learnopengl/shader_m.h>
#include <learnopengl/model.h>

#include <iostream>
#include <string>

const float toRadians = 3.14159265f / 180.0f;

bool activated = true;

// settings
const unsigned int SCR_WIDTH = 1800;
const unsigned int SCR_HEIGHT = 1000;

// camera
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