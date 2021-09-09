#version 330
/*
#ifdef GL_ES
precision highp float;
#endif
*/

#extra Noise3D.glsl

uniform float u_seed;
uniform vec2 u_resolution;

/// float myNoise(vec3 coords, int octives, float persistence, float scale, float low, float high);

void main() {
    vec2 uv = gl_FragCoord.xy / u_resolution.xy;
    //gl_FragColor = vec4(vec3(snoise(vec3(uv, u_seed))), 1);

    float modValue = 0.1;
    float value = mod(myNoise(vec3(uv, u_seed * 0.1), 5, 0.8, 0.5, 0.0, 1.0), modValue) * (1.0 / modValue);

    gl_FragColor = vec4(vec3(value), 1.0);
}
