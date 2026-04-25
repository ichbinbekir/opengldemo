#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <spdlog/spdlog.h>

#include "FileWatch.hpp"
#include "util.hpp"

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

  uint32_t vs, fs;
  try
  {
    auto vertexSource = readFile("../shaders/test.vert");
    auto fragmentSource = readFile("../shaders/test.frag");

    auto vPtr = vertexSource.c_str();
    auto fPtr = fragmentSource.c_str();

    vs = glCreateShaderProgramv(GL_VERTEX_SHADER, 1, &vPtr);
    fs = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, &fPtr);
  }
  catch (const std::exception &e)
  {
    spdlog::error(e.what());
    return EXIT_FAILURE;
  }

  int32_t success;
  glGetProgramiv(vs, GL_LINK_STATUS, &success);
  if (!success)
  {
    int32_t len;
    glGetProgramiv(vs, GL_INFO_LOG_LENGTH, &len);

    std::vector<char> log(len);
    glGetProgramInfoLog(vs, len, nullptr, log.data());
    spdlog::error("Vertex shader could not be compile: {}", log.data());
    return EXIT_FAILURE;
  }

  glGetProgramiv(fs, GL_LINK_STATUS, &success);
  if (!success)
  {
    int32_t len;
    glGetProgramiv(fs, GL_INFO_LOG_LENGTH, &len);

    std::vector<char> log(len);
    glGetProgramInfoLog(fs, len, nullptr, log.data());
    spdlog::error("Fragment shader could not be compile: {}", log.data());
    return EXIT_FAILURE;
  }

  uint32_t pipeline;
  glCreateProgramPipelines(1, &pipeline);

  glUseProgramStages(pipeline, GL_VERTEX_SHADER_BIT, vs);
  glUseProgramStages(pipeline, GL_FRAGMENT_SHADER_BIT, fs);

  glBindProgramPipeline(pipeline);

  bool reload;
  filewatch::FileWatch<std::string> watch(
      "../shaders/test.frag",
      [&reload](const std::string &path, const filewatch::Event change_type)
      {
        if (change_type == filewatch::Event::modified)
        {
          spdlog::info("Shader reload: {}", path);
          reload = true;
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

  float angle;

  auto rotUni = glGetUniformLocation(vs, "rot");

  spdlog::info("Main loop ...");
  while (!glfwWindowShouldClose(window))
  {
    glProgramUniformMatrix2fv(vs, rotUni, 1, false, rotate(angle).data());

    if (reload)
    {
      uint32_t s;
      try
      {
        auto source = readFile("../shaders/test.frag");
        auto ptr = source.c_str();
        s = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, &ptr);
      }
      catch (const std::exception &e)
      {
        spdlog::error(e.what());
      }

      int32_t success;
      glGetProgramiv(s, GL_LINK_STATUS, &success);
      if (!success)
      {
        int32_t len;
        glGetProgramiv(s, GL_INFO_LOG_LENGTH, &len);

        std::vector<char> log(len);
        glGetProgramInfoLog(s, len, nullptr, log.data());
        spdlog::error("Fragment shader could not be compile: {}", log.data());
      }
      else
      {
        glDeleteProgram(fs);
        glUseProgramStages(pipeline, GL_FRAGMENT_SHADER_BIT, s);
        fs = s;
      }
      reload = false;
    }

    glClear(GL_COLOR_BUFFER_BIT);

    glDrawArrays(GL_QUADS, 0, std::size(vertices) / 3);

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
