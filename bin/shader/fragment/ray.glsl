#version 410 core

out vec4 FragColor;

uniform ivec2 resolution;
uniform vec2  mouse_pos;
uniform float playback_time;

void main() {
    // fragment position
    vec2 frag_pos = vec2(gl_FragCoord.xy * 2.0 - resolution) / resolution[0];

    // camera
    vec3 camera_pos  = vec3(0.0, 0.0,  3.0);
    vec3 camera_dir  = vec3(0.0, 0.0, -1.0);
    vec3 camera_up   = vec3(0.0, 1.0,  0.0);
    vec3 camera_side = cross(camera_dir, camera_up);
    float target_depth = 0.1;

    // ray
    vec3 ray = normalize(camera_side * frag_pos.x + camera_up * frag_pos.y + camera_dir * target_depth);

    FragColor = vec4(ray.xy, -ray.z, 1.0);
}