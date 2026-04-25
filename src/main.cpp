#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <spdlog/spdlog.h>
#include "FileWatch.hpp"

#include "util.hpp"
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

  std::optional<Program> vert_prog, frag_prog;

  try
  {
    auto vert_src = readFile("shaders/test.vert");
    auto frag_src = readFile("shaders/test.frag");

    vert_prog.emplace(ShaderType::Vertex, vert_src);
    frag_prog.emplace(ShaderType::Fragment, frag_src);
  }
  catch (const std::exception &e)
  {
    spdlog::error(e.what());
    return EXIT_FAILURE;
  }

  ProgramPipeline ppline;
  ppline.setBit(*vert_prog);
  ppline.setBit(*frag_prog);

  ppline.bind();

  bool reload = false;
  filewatch::FileWatch<std::string> watch(
      "shaders/test.frag",
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

  float angle = 0.0f;
  int rotUni = vert_prog->getUniformLocation("rot");

  spdlog::info("Main loop ...");
  while (!glfwWindowShouldClose(window))
  {
    vert_prog->setUniformMatrix2fv(rotUni, rotate(angle).data());

    if (reload)
    {
      try
      {
        auto source = readFile("shaders/test.frag");
        // Dispose of the old program and create a new one
        frag_prog.reset();
        frag_prog.emplace(ShaderType::Fragment, source);
        ppline.setBit(*frag_prog);
        spdlog::info("Fragment shader reloaded successfully");
      }
      catch (const std::exception &e)
      {
        spdlog::error("Fragment shader reload failed: {}", e.what());
      }
      reload = false;
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
