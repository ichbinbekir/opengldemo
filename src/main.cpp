#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <spdlog/spdlog.h>
#include "FileWatch.hpp"

#include "util.hpp"
#include "shader.hpp"
#include "program.hpp"
#include "program_pipeline.hpp"

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

  Shader vert(ShaderType::Vertex);
  Shader frag(ShaderType::Fragment);

  Program prog;
  try
  {
    auto vert_src = readFile("shaders/test.vert");
    auto frag_src = readFile("shaders/test.frag");

    vert.compile({vert_src});
    frag.compile({frag_src});

    prog.link(false, {vert, frag});
  }
  catch (const std::exception &e)
  {
    spdlog::error(e.what());
    return EXIT_FAILURE;
  }
  
  prog.use();

  bool vert_reload = false;
  bool frag_reload = false;
  filewatch::FileWatch<std::string> vert_watch(
      "shaders/test.vert",
      [&vert_reload](const std::string &path, const filewatch::Event change_type)
      {
        if (change_type == filewatch::Event::modified)
        {
          spdlog::info("Vertex shader reload: {}", path);
          vert_reload = true;
        }
      });
  filewatch::FileWatch<std::string> frag_watch(
      "shaders/test.frag",
      [&frag_reload](const std::string &path, const filewatch::Event change_type)
      {
        if (change_type == filewatch::Event::modified)
        {
          spdlog::info("Fragment shader reload: {}", path);
          frag_reload = true;
        }
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
  uint32_t rotUni = prog.getUniformLocation("rot");

  spdlog::info("Main loop ...");
  while (!glfwWindowShouldClose(window))
  {
    prog.setUniformMatrix2fv(rotUni, rotate(angle).data());

    bool should_relink = false;
    if (vert_reload)
    {
      try
      {
        auto source = readFile("shaders/test.vert");
        vert.compile({source});
        spdlog::info("Vertex shader reloaded successfully");
        should_relink = true;
      }
      catch (const std::exception &e)
      {
        spdlog::error("Vertex shader reload failed: {}", e.what());
      }
      vert_reload = false;
    }

    if (frag_reload)
    {
      try
      {
        auto source = readFile("shaders/test.frag");
        frag.compile({source});
        spdlog::info("Fragment shader reloaded successfully");
        should_relink = true;
      }
      catch (const std::exception &e)
      {
        spdlog::error("Fragment shader reload failed: {}", e.what());
      }
      frag_reload = false;
    }

    if (should_relink)
    {
      try
      {
        prog.link(false, {vert, frag});
        rotUni = prog.getUniformLocation("rot");
        spdlog::info("Program re-linked successfully");
      }
      catch (const std::exception &e)
      {
        spdlog::error("Program link failed: {}", e.what());
      }
    }

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
