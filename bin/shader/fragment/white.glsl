#version 410 core

out vec4 FragColor;

uniform ivec2  resolution;
uniform dvec2  mouse_pos;
uniform double playback_time;

void main() {
    FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}