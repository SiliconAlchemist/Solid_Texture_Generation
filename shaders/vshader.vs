#version 330 core
layout (location = 0) in vec3 vVertex;
layout (location = 1) in vec3 vt;

uniform mat4 vModel;
uniform mat4 vView;
uniform mat4 vProjection;

out vec3 texture_coordinates;


void main()
{
  texture_coordinates = vt;
  gl_Position = vProjection*vView*vModel*vec4(vVertex.x, vVertex.y, vVertex.z, 1.0);
}