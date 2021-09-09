#version 330
/*
#ifdef GL_ES
precision highp float;
#endif
*/

#extra shaders/Noise3D.glsl

uniform float u_time;
uniform vec2 u_resolution;

/// float myNoise(vec3 coords, int octives, float persistence, float scale, float low, float high);

void main() {
    vec2 uv = gl_FragCoord.xy / u_resolution.xy / 3.;
    //gl_FragColor = vec4(vec3(snoise(vec3(uv, u_time))), 1);

    float modValue = 0.05;
    float value = mod(myNoise(vec3(uv, u_time * 0.1), 5, 0.8, 0.5, 0.0, 1.0), modValue) * (1.0 / modValue);

    gl_FragColor = vec4(vec3(value), 1.0);
    //gl_FragColor = vec4(0.0, 0.5, 0.5, 1.0);
}
