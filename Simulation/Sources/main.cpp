#define STB_IMAGE_IMPLEMENTATION

// Local Headers
#include "simulation.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "particle_emitter.hpp"
#include "camera.hpp"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Headers
#include <vector>
#include <iostream>

void resize_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

// camera
Camera camera(glm::vec3(1.0f, -1.0f, 3.0f));
float lastX = mWidth / 2.0f;
float lastY = mHeight / 2.0f;
bool firstMouse = true;

// flag for color of fire
bool isRed = true;

int main(int argc, char * argv[]) {

    // Load GLFW and Create a Window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
   // glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    auto mWindow = glfwCreateWindow(mWidth, mHeight, "OpenGL", nullptr, nullptr);

    // Check for Valid Context
    if (mWindow == nullptr) {
        std::cout << "Failed to Create GLFW Window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Create Context and Load OpenGL Functions
    glfwMakeContextCurrent(mWindow);
    glfwSetFramebufferSizeCallback(mWindow, resize_callback);
    glfwSetCursorPosCallback(mWindow, mouse_callback);
   // glfwSetScrollCallback(mWindow, scroll_callback);
    
    gladLoadGL();
    std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl;
    
    // configure global opengl state
    glDisable (GL_DEPTH_TEST);
    glEnable (GL_BLEND);
    glEnable(GL_CULL_FACE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
    // build and compile our shader program
    Shader particleShader("../Shaders/shader1.vert", "../Shaders/shader1.frag");
    Shader sceneShader("../Shaders/shader2.vert", "../Shaders/shader2.frag");
  
    float planeVertices[] = {
        // positions            // normals         // texcoords
        10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
        -10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
        -10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
        
        10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
        -10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
        10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,  10.0f, 10.0f
    };
    
    // plane VAO, VBO
    unsigned int planeVAO, planeVBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
    
    // define plane attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(0);
    
    // unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    // load plane texture
    int planeWidth, planeHeight, planeChannels;
    unsigned char *planeData = stbi_load("../Resources/ground_frozen_ground_0004_01.jpg", &planeWidth, &planeHeight, &planeChannels, 0);
    Texture2D planeTexture;
    planeTexture.Generate(planeWidth, planeHeight, planeData);
    
    // load particle texture
    int particleWidth, particleHeight, particleChannels;
    unsigned char *particleData = stbi_load("../Resources/Particle2.png", &particleWidth, &particleHeight, &particleChannels, 0);
    Texture2D particleTexture;
    particleTexture.Generate(particleWidth, particleHeight, particleData);
    
    // initialize particle simulation
    ParticleEmitter *particles = new ParticleEmitter(particleShader, particleTexture, 35000);
    
    // time variable between last and current frame
    GLfloat deltaTime = 0.0f;
    GLfloat lastFrame = 0.0f;
    
    // light source (should be similar to fire position)
    glm::vec3 lightPos(1.3f, 0.0f, 2.7f);
    
    // rendering Loop
    while (glfwWindowShouldClose(mWindow) == false) {
        // input
        if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(mWindow, true);
        if (glfwGetKey(mWindow, GLFW_KEY_E) == GLFW_PRESS)
            isRed = !isRed;
        
        // time diff between curr and last frame
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        // background Fill Color
        glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT );
        
        // perspective projection transformation
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float) mWidth / (float) mHeight, 0.1f, 100.0f);
    
        // view transformation
        glm::mat4 view = camera.GetViewMatrix();
        
        // set the scene texture and lighting properties
        sceneShader.use();
        sceneShader.setMat4("projection", projection);
        sceneShader.setMat4("view", view);
        sceneShader.setVec3("viewPos", camera.Position);
        sceneShader.setVec3("lightPos", lightPos);
        glBindVertexArray(planeVAO);
        planeTexture.Bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        
        // update particles and draw simulation
        particles->update(deltaTime, 11, isRed);
        particles->draw(view, projection);
        
        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }
    
    // deallocate resources used
    particles->clear();
    glDeleteVertexArrays(1, &planeVAO);
    glDeleteBuffers(1, &planeVBO);
    
    glfwTerminate();
    return 1;
}

// Whenever the window size changed this function executes
void resize_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

//
//  Author: Joey de Vries
//  Availability: https://learnopengl.com/#!In-Practice/2D-Game/Setting-up
//  License: https://creativecommons.org/licenses/by/4.0/legalcode
//
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    
    lastX = xpos;
    lastY = ypos;
    
    camera.ProcessMouseMovement(xoffset, yoffset);
}


