#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <spdlog/spdlog.h>

#include "util.hpp"
#include "shader.hpp"
#include "program.hpp"
#include "program_pipeline.hpp"
#include "asset_manager.hpp"

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}

float vertices[] = {
    -0.5f,
    -0.5f,
    0.0f,

    0.5f,
    -0.5f,
    0.0f,

    0.5f,
    0.5f,
    0.0f,

    -0.5f,
    0.5f,
    0.0f,
};

std::array<float, 4> rotate(float angle)
{
  auto ca = cosf(angle);
  auto sa = sinf(angle);
  return {ca, sa,   // column 0
          -sa, ca}; // column 1
}

int main()
{
  spdlog::info("Initializing");
  if (!glfwInit())
  {
    spdlog::error("GLFW could not be load");
    return EXIT_FAILURE;
  }

  // TODO: defer glfwTerminate();

  auto window = glfwCreateWindow(800, 600, "OpenGL Demo", NULL, NULL);
  if (!window)
  {
    spdlog::error("Window could not be create");
    return EXIT_FAILURE;
  }
  glfwMakeContextCurrent(window);
  // glfwSwapInterval(0);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    spdlog::error("GLAD could not load");
    return EXIT_FAILURE;
  }

  ImGui::CreateContext();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init();

  spdlog::info("Renderer: {}", (const char *)glGetString(GL_RENDERER));
  spdlog::info("Version: {}", (const char *)glGetString(GL_VERSION));

  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  glClearColor(0.15, 0.15, 0.15, 1);

  uint32_t rotUni = 0;
  auto prog = AssetManager::instance().createProgram(
      "test", false,
      {{"shaders/test.vert", ShaderType::Vertex},
       {"shaders/test.frag", ShaderType::Fragment}},
      [&rotUni](Program &p)
      {
        rotUni = p.getUniformLocation("rot");
      });

  uint32_t vao, vbo;
  glCreateVertexArrays(1, &vao);
  glCreateBuffers(1, &vbo);

  glNamedBufferData(vbo, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glEnableVertexArrayAttrib(vao, 0);
  glVertexArrayAttribBinding(vao, 0, 0);
  glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, false, 0);

  glVertexArrayVertexBuffer(vao, 0, vbo, 0, 3 * sizeof(float));

  glBindVertexArray(vao);

  float angle = 0.0f;

  spdlog::info("Main loop ...");
  while (!glfwWindowShouldClose(window))
  {
    AssetManager::instance().update();

    prog->use();
    prog->setUniformMatrix2fv(rotUni, rotate(angle).data());

    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLE_FAN, 0, std::size(vertices) / 3);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    ImGui::NewFrame();
    ImGui::Begin("Controller");
    ImGui::SliderAngle("Angle", &angle, 0);
    ImGui::End();
    ImGui::EndFrame();

    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // TODO: close all handlers, unload ram, vram. we can do with defer system

  glfwTerminate(); // TODO: remove line after defer system
  return EXIT_SUCCESS;
}
