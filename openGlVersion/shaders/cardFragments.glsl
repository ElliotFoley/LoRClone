#version 330 core
out vec4 FragColor;
in vec2 texCords;

uniform sampler2D cardTexture;

void main(){
    FragColor = texture(cardTexture, texCords);
    //FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}
