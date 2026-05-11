#include "app.hpp"
#include "shader.hpp"

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
  std::optional<App> app;
  std::optional<Shader> shader;
  try
  {
    app.emplace("OpenGL Demo");
    shader.emplace("shaders/test.vert", "shaders/test.frag");
  }
  catch (const std::exception &e)
  {
    spdlog::error(e.what());
    return EXIT_FAILURE;
  }

  shader->use();
  float angle = 0.0f;

  uint32_t vao, vbo;
  glCreateVertexArrays(1, &vao);
  glCreateBuffers(1, &vbo);

  glNamedBufferData(vbo, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glEnableVertexArrayAttrib(vao, 0);
  glVertexArrayAttribBinding(vao, 0, 0);
  glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, false, 0);

  glVertexArrayVertexBuffer(vao, 0, vbo, 0, 3 * sizeof(float));

  glBindVertexArray(vao);

  spdlog::info("Main loop ...");
  while (!glfwWindowShouldClose(app->window))
  {
    shader->setMat2("rot", rotate(angle).data());

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

    glfwSwapBuffers(app->window);
    glfwPollEvents();
  }

  glBindVertexArray(0);

  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  return EXIT_SUCCESS;
}
