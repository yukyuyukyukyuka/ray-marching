#version 410 core

out vec4 FragColor;

uniform ivec2 resolution;
uniform vec2  mouse_pos;
uniform float playback_time;

void main() {
    FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
}