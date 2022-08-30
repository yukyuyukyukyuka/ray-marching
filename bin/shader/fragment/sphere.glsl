#version 410 core

out vec4 FragColor;

uniform ivec2 resolution;
uniform vec2  mouse_pos;
uniform float playback_time;

const float sphere_size = 1.0;
const vec3 light_dir = vec3(-0.577, 0.577, 0.577);

float sdf(vec3 p) {
    return length(p) - sphere_size;
}

vec3 get_normal(vec3 p) {
    const float d = 0.0001;
    return normalize(vec3(
        sdf(p + vec3(  d, 0.0, 0.0)) - sdf(p + vec3( -d, 0.0, 0.0)),
        sdf(p + vec3(0.0,   d, 0.0)) - sdf(p + vec3(0.0,  -d, 0.0)),
        sdf(p + vec3(0.0, 0.0,   d)) - sdf(p + vec3(0.0, 0.0,  -d))
    ));
}

void main() {
    // fragment position
    vec2 frag_pos = vec2(gl_FragCoord.xy * 2.0 - resolution) / resolution[0];

    // camera
    vec3 camera_pos  = vec3(0.0, 0.0,  2.0);
    vec3 camera_dir  = vec3(0.0, 0.0, -1.0);
    vec3 camera_up   = vec3(0.0, 1.0,  0.0);
    vec3 camera_side = cross(camera_dir, camera_up);
    float target_depth = 1.0;

    // ray
    vec3 ray = normalize(camera_side * frag_pos.x + camera_up * frag_pos.y + camera_dir * target_depth);

    // marching loop
    float distance = 0.0;
    float ray_len = 0.0;
    vec3 ray_pos = camera_pos;
    for(int i = 0; i < 16; i++) {
        distance = sdf(ray_pos);
        ray_len += distance;
        ray_pos = camera_pos + ray * ray_len;
    }

    // hit check
    if(abs(distance) < 0.001) {
        vec3 normal = get_normal(ray_pos);
        float diff = clamp(dot(light_dir, normal), 0.1, 1.0);
        FragColor = vec4(vec3(diff), 1.0);
    } else {
        FragColor = vec4(vec3(0.0), 1.0);
    }
}