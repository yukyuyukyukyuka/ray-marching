#version 410 core

out vec4 FragColor;

uniform ivec2  resolution;
uniform dvec2  mouse_pos;
uniform double playback_time;

void main() {
    // normalize coordinates
    vec2 p = (gl_FragCoord.xy * 2.0 - resolution) / resolution[0];
    vec2 color = (vec2(1.0) + p.xy) * 0.5;
    FragColor = vec4(color, 0.0f, 1.0f);
}