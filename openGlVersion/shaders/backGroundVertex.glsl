#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCords;

out vec2 texCords;

void main(){
    gl_Position = vec4(aPos, 1.0);
    texCords = aTexCords;
}
