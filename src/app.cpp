#include <glad/glad.h>
#include <glm/vec4.hpp>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <shader.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
float zoom_scale_func(float x);

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  
  GLFWwindow* window = glfwCreateWindow(800, 600, "openglwindow", NULL, NULL);

  if (window == NULL) {
    std::cout << "failed to create GLFW window";
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
  }

  int fbSizeX, fbSizeY;
  glfwGetFramebufferSize(window, &fbSizeX, &fbSizeY);
  glViewport(0, 0, fbSizeX, fbSizeY);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  float vertices[] = {
    1.0f, 1.0f, 0.0f,
    -1.0f, 1.0f, 0.0f,
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
  };

  unsigned int indices[] = {
    0, 1, 2,
    0, 2, 3,
  };

  unsigned int EBO;
  glGenBuffers(1, &EBO);

  unsigned int VBO;
  glGenBuffers(1, &VBO);

  unsigned int VAO;
  glGenVertexArrays(1, &VAO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glBindVertexArray(0);

  Shader shaderProgram = Shader("shaders/shader.vert", "shaders/shader.frag");
  shaderProgram.use();

  glm::vec2 initPoint = glm::vec2(0, 0);
  glm::vec2 offset = glm::vec2(0);
  float zoom = 0;
  float aspectRatio = float(fbSizeX) / fbSizeY;
  bool toggleSets = 0;
  bool press = 1;

  shaderProgram.setVec2i("dim", fbSizeX, fbSizeY);
  shaderProgram.setFloat("zoom", zoom);
  shaderProgram.setVec2f("initPoint", initPoint.x, initPoint.y);
  shaderProgram.setFloat("toggleSets", toggleSets);
  shaderProgram.setVec2f("offset", offset.x, offset.y);
  glm::vec2 dim = glm::vec2(2.0f * zoom_scale_func(zoom) * aspectRatio, 2.0f * zoom_scale_func(zoom));

  while (!glfwWindowShouldClose(window)) {
    // input
    processInput(window);
    glfwPollEvents(); // Process events (input, window resizing, etc.)
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwGetFramebufferSize(window, &fbSizeX, &fbSizeY);

    if (glfwGetKey(window, GLFW_KEY_UP)) {
      initPoint.x += 0.01;
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN)) {
      initPoint.x -= 0.01;
    }

    if (glfwGetKey(window, GLFW_KEY_I)) {
      zoom += 0.02;
    }

    if (glfwGetKey(window, GLFW_KEY_O)) {
      zoom -= 0.02;
    }

    if (glfwGetKey(window, GLFW_KEY_T) && press) {
      toggleSets ^= 1;
      press = 0;
    }
    else if (glfwGetKey(window, GLFW_KEY_T) == GLFW_RELEASE) {
      press = 1;
    }

    if (glfwGetKey(window, GLFW_KEY_W)) {
      offset.y += 0.0075 * dim.y;
    }
    if (glfwGetKey(window, GLFW_KEY_A)) {
      offset.x -= 0.0075 * dim.x; 
    }
    if (glfwGetKey(window, GLFW_KEY_S)) {
      offset.y -= 0.0075 * dim.y;
    }
    if (glfwGetKey(window, GLFW_KEY_D)) {
      offset.x += 0.0075 * dim.x;
    }

    shaderProgram.setFloat("toggleSets", toggleSets);
    shaderProgram.setVec2f("initPoint", initPoint.x, initPoint.y);
    shaderProgram.setFloat("zoom", zoom);
    shaderProgram.setVec2i("dim", fbSizeX, fbSizeY);
    shaderProgram.setVec2f("offset", offset.x, offset.y);
    dim = glm::vec2(2 * zoom_scale_func(zoom) * aspectRatio, 2 * zoom_scale_func(zoom));

    // rendering here
    shaderProgram.use();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glfwSwapBuffers(window); // Swap front and back buffers
  }

  glfwTerminate();
  return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}
void processInput(GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}
float zoom_scale_func(float x) {
  return exp(-1 * x);
}

