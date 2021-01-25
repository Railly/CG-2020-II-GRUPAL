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
const float toDegrees = 180.0f / 3.14159265f;

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

// gravity
const double g = 9.8;

//activation camera
bool act = true;

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


    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    Shader tankShader("vertShader.glsl", "fragShader.glsl");
    Shader wallShader("vertShader.glsl", "fragShader.glsl");
    Shader canonShader("lampVertShader.glsl", "lampFragShader.glsl");
    Shader bulletShader("vertShader.glsl", "fragShader.glsl");

    Model tank("finalTank.obj");
    Model target("finalTarget.obj");
    Model bullet("finalBullet.obj");
    Model canon("finalCanon.obj");

    float time = 2.f; // two seconds

    float previous = glfwGetTime();

    bool oneTime = true;
    bool oneTime2 = true;
    bool oneTime3 = true;
    bool oneTime4 = true;
    bool oneTime5 = true;

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


        /*--[FRAME IMGUI]-----------*/
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::SetNextWindowSize(ImVec2(SCR_WIDTH * 0.24f, SCR_HEIGHT * 0.15f));
        ImGui::SetNextWindowPos(ImVec2(15, 15));

        /*--[CODIGO IMGUI]-----------*/

        ImGuiStyle& style = ImGui::GetStyle();
        style.FrameBorderSize = 1.0f;
        ImGuiWindowFlags window_flags = 0;
        window_flags |= ImGuiWindowFlags_MenuBar;
        window_flags |= ImGuiWindowFlags_NoCollapse;
        ImVec2 buttonSize = ImVec2(120.0f, 30.f);


        ImGui::Begin("Camera Controls");
            ImGui::SetWindowFontScale(1.6);
            ImGui::Spacing();
            ImGui::Indent(88.0f);
            if (ImGui::Button("Move UP", buttonSize)) {
                camera.ProcessKeyboard(FORWARD, deltaTime);
            }
            ImGui::Unindent(88.0f);
            ImGui::Spacing();

            if (ImGui::Button("Move LEFT", buttonSize)) {
                camera.ProcessKeyboard(LEFT, deltaTime);
            }
            ImGui::SameLine();
            if (ImGui::Button("Move DOWN", buttonSize)) {
                camera.ProcessKeyboard(BACKWARD, deltaTime);
            }
            ImGui::SameLine();
            if (ImGui::Button("Move RIGHT", buttonSize)) {
                camera.ProcessKeyboard(RIGHT, deltaTime);
            }
        ImGui::End();

        static float angleX = 0.0f;
        static float angleY = 0.0f;
        static float angleZ = 0.0f;
        static float velocity = 10.0f;

        ImGui::SetNextWindowSize(ImVec2(SCR_WIDTH * 0.24f, SCR_HEIGHT * 0.18f));
        ImGui::SetNextWindowPos(ImVec2(15, 200));
        ImGui::Begin("Shoot Controls");
            ImGui::SetWindowFontScale(1.6);
            ImGui::SliderAngle("Angle XY", &angleZ, -20.0, 20.0);
            ImGui::SliderAngle("Angle XZ", &angleY, -20.0, 20.0);
            ImGui::SliderAngle("Angle YZ", &angleX, 0.0, 40.0);
            ImGui::SliderFloat("Velocity", &velocity, 1.0, 30.0);
        ImGui::End();

        ImGui::SetNextWindowSize(ImVec2(SCR_WIDTH * 0.20f, SCR_HEIGHT * 0.15f));
        ImGui::SetNextWindowPos(ImVec2(15, 450));
        static int numberShots = 0;

        ImGui::Begin("User Panel");
            ImGui::SetWindowFontScale(1.6);
            ImGui::TextWrapped("Shots fired: ");
            ImGui::SameLine();
            string strShots = std::to_string(numberShots);
            const char* nShots = strShots.c_str();
            ImGui::Text(nShots);
            if (ImGui::Button("Shoot", buttonSize)) {
                glfwSetTime(0);
                numberShots++;
                activated = false;
            }
        ImGui::End();

        ImGui::SetNextWindowSize(ImVec2(SCR_WIDTH * 0.24f, SCR_HEIGHT * 0.15f));
        ImGui::SetNextWindowPos(ImVec2(15, 650));
        static int score = 0;

        ImGui::Begin("Score");
            ImGui::SetWindowFontScale(1.6);
            ImGui::TextWrapped("Current Score: ");
            ImGui::SameLine();
            string strScore = std::to_string(score);
            const char* nScore = strScore.c_str();
            ImGui::Text(nScore);
        ImGui::End();

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

        int dAngleX = angleX * toDegrees;
        int dAngleY = angleY * toDegrees;
        int dAngleZ = angleZ * toDegrees;

        time -= delta;
        if (!activated) {
            // SCORE + 20
            if (round(time) == 0 && round(dAngleX) == 26 && round(velocity) == 30) {
                std::cout << "dAngleX: " << dAngleX << std::endl;
                std::cout << "dAngleY: " << dAngleY << std::endl;
                std::cout << "dAngleZ: " << dAngleZ << std::endl;

                if (round(dAngleY) == 0 || round(dAngleY) == 1 || round(dAngleY) == 2 || round(dAngleY) == 3) {
                    while (oneTime) {
                        std::cout << "+25" << std::endl;
                        score = score + 25;
                        oneTime = false;
                    }
                }
                if (round(dAngleY) == 11 || round(dAngleY) == 12) {
                    while (oneTime) {
                        std::cout << "+20" << std::endl;
                        score = score + 20;
                        oneTime = false;
                    }
                }
                if (round(dAngleY) == -7 || round(dAngleY) == -8 || round(dAngleY) == -9) {
                    while (oneTime) {
                        std::cout << "+30" << std::endl;
                        score = score + 30;
                        oneTime = false;
                    }
                }
            }
            else {
                oneTime = true;
            }

            if (round(time) == 0 && round(dAngleX) == 27 && round(velocity) == 25) {
                std::cout << "dAngleX: " << dAngleX << std::endl;
                std::cout << "dAngleY: " << dAngleY << std::endl;
                std::cout << "dAngleZ: " << dAngleZ << std::endl;

                if (round(dAngleY) == 1 || round(dAngleY) == 2 || round(dAngleY) == 3) {
                    while (oneTime2) {
                        std::cout << "40" << std::endl;
                        score = score + 40;
                        oneTime2 = false;
                    }
                }
                if (round(dAngleY) == 11 || round(dAngleY) == 12) {
                    while (oneTime2) {
                        std::cout << "+35" << std::endl;
                        score = score + 35;
                        oneTime2 = false;
                    }
                }
                if (round(dAngleY) == -7 || round(dAngleY) == -8 || round(dAngleY) == -9) {
                    while (oneTime2) {
                        std::cout << "+45" << std::endl;
                        score = score + 45;
                        oneTime2 = false;
                    }
                }
            }
            else {
                oneTime2 = true;
            }
            if (round(time) == 0 && round(dAngleX) == 28 && round(velocity) == 22) {
                std::cout << "dAngleX: " << dAngleX << std::endl;
                std::cout << "dAngleY: " << dAngleY << std::endl;
                std::cout << "dAngleZ: " << dAngleZ << std::endl;

                if (round(dAngleY) == 1 || round(dAngleY) == 2 || round(dAngleY) == 3) {
                    while (oneTime3) {
                        std::cout << "+55" << std::endl;
                        score = score + 55;
                        oneTime3 = false;
                    }
                }
                if (round(dAngleY) == 11 || round(dAngleY) == 12) {
                    while (oneTime3) {
                        std::cout << "+50" << std::endl;
                        score = score + 50;
                        oneTime3= false;
                    }
                }
                if (round(dAngleY) == -7 || round(dAngleY) == -8 || round(dAngleY) == -9) {
                    while (oneTime3) {
                        std::cout << "+60" << std::endl;
                        score = score + 60;
                        oneTime3 = false;
                    }
                }
            }
            else {
                oneTime3 = true;
            }

            if (round(time) == 0 && round(dAngleX) == 28 && round(velocity) == 27) {
                std::cout << "dAngleX: " << dAngleX << std::endl;
                std::cout << "dAngleY: " << dAngleY << std::endl;
                std::cout << "dAngleZ: " << dAngleZ << std::endl;

                if (round(dAngleY) == -17 || round(dAngleY) == -16 || round(dAngleY) == -15) {
                    while (oneTime4) {
                        std::cout << "+70" << std::endl;
                        score = score + 70;
                        oneTime4 = false;
                    }
                }
            }
            else {
                oneTime4 = true;
            }

            if (round(time) == -1 && round(dAngleX) == 28 && round(velocity) == 23) {

                if (round(dAngleY) == -17 || round(dAngleY) == -16 || round(dAngleY) == -15) {
                    while (oneTime5) {
                        std::cout << "+80" << std::endl;
                        score = score + 80;
                        oneTime5 = false;
                    }
                }
            }
            else {
                oneTime5 = true;
            }

            // Parabolic Movement
            double timeSquare = pow(currentFrame, 2);
            static float parabolicoY = 0;
            static float U = -currentFrame * velocity * 0.5;
            parabolicoY = U * sin(angleX - 0.2) * currentFrame - (g * timeSquare / 2.0);

            modelBullet = glm::translate(modelBullet, glm::vec3(0.0f, 0.0f, -currentFrame * velocity));
            modelBullet = glm::translate(modelBullet, glm::vec3(0.0f, parabolicoY * 0.35, 0.0f));
        }

        // Turn off/ on camera movement

        if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
            glfwSetCursorPosCallback(window, mouse_callback);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
            glfwSetCursorPosCallback(window, NULL);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }

        // Shot Function
        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
            glfwSetTime(0);
            numberShots++;
            activated = false;
        }
        // Print Camera
        if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
            std::cout << "X: " << camera.Position.x << std::endl;
            std::cout << "Y: " << camera.Position.y << std::endl;
            std::cout << "Z: " << camera.Position.z << std::endl;
        }

        // Print Angles
        if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
            std::cout << "AngleX: " << dAngleX << std::endl;
            std::cout << "AngleY: " << dAngleY << std::endl;
            std::cout << "AngleZ: " << dAngleZ << std::endl;
            std::cout << "Velocity: " << round(velocity) << std::endl;
        }

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
