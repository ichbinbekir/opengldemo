#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <spdlog/spdlog.h>

#include <string>
#include <stdexcept>
#include <format>

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}

class App
{
private:
public:
  GLFWwindow *window;

  App(const std::string &title);
  ~App();
};

App::App(const std::string &title)
{
  spdlog::info("Initializing");
  if (!glfwInit())
  {
    throw std::runtime_error("GLFW could not be load");
  }

  window = glfwCreateWindow(800, 600, title.c_str(), NULL, NULL);
  if (!window)
  {
    throw std::runtime_error("Window could not be create");
  }

  glfwMakeContextCurrent(window);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    throw std::runtime_error("GLAD could not load");
  }

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  ImGui::CreateContext();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init();

  spdlog::info("Renderer: {}", (const char *)glGetString(GL_RENDERER));
  spdlog::info("Version: {}", (const char *)glGetString(GL_VERSION));

  // Resync viewport for graphical overflows
  // For KDE because KDE creating window any size. different by 800 width, 600 height
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);

  glClearColor(0.15, 0.15, 0.15, 1);
}

App::~App()
{
  glfwTerminate();
}
