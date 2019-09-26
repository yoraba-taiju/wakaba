#version 410 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 position;

void main(){
    gl_Position.xyz = position;
    gl_Position.w = 1.0;
}
