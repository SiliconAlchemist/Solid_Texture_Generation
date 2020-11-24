#version 330
layout (location = 0)in vec3 vVertex;
layout (location = 1)in vec3 vertex_norm;

vec3 eyepos;
vec3 eye_normal;
uniform mat4 vModel;
uniform mat4 vView;
uniform mat4 vProjection;

vec3 lpos_world = vec3(0.0, 0.5, 1.0);

out vec4 fragColor;
vec3 kd, ld, ka, la

void main(){

    gl_Position = vProjection * vView * vModel * vec4(vVertex, 1.0);
    
    //to do 
    fragColor = vec4(, 1.0); //Create different fragColor implementations for different cases

    
}
