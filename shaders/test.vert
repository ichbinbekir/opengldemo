#version 460 core

layout (location = 0) in vec3 aPos;

uniform mat2 rot;

void main()
{
  vec2 newcords = rot * vec2(aPos.x, aPos.y);
  gl_Position = vec4(newcords.x, newcords.y, aPos.z, 1);
}
