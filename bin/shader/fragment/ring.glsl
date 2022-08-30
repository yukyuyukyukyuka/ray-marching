#version 410 core

out vec4 FragColor;

uniform ivec2 resolution;
uniform vec2  mouse_pos;
uniform float playback_time;

void main() {
    vec2 m = vec2(mouse_pos.x / resolution[0] * 2.0 - 1.0, -mouse_pos.y / resolution[1] * 2.0 + 1.0);
    vec2 p = vec2(gl_FragCoord.xy * 2.0 - resolution) / resolution[0];
    float t = 0.02 / abs((sin(playback_time) + 1.0) / 2.0 - length(m - p));
    FragColor = vec4(vec3(t), 1.0);
}