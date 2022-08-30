#version 410 core

out vec4 FragColor;

uniform ivec2 resolution;
uniform vec2  mouse_pos;
uniform float playback_time;

void main() {
    vec2 m = vec2(mouse_pos.x / resolution[0] * 2.0 - 1.0, -mouse_pos.y / resolution[1] * 2.0 + 1.0);
    vec2 p = vec2(gl_FragCoord.xy * 2.0 - resolution) / resolution[0];

    int j = 0;
    vec2 x = vec2(-0.345, 0.654);
    vec2 y = vec2(playback_time * 0.005, 0.0);
    vec2 z = p;

    for(int i = 0; i < 360; i++) {
        j++;
        if(length(z) > 2.0) { break; }
        z = vec2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y) + x + y;
    }

    float t = float(j) / 360.0;

    FragColor = vec4(vec3(t), 1.0);
}