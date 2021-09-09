#version 330

#ifdef GL_ES
precision highp float;
#endif


#extra shaders/Noise3D.glsl

//uniform float u_seed;
uniform vec2 u_resolution;
uniform vec2 u_mouse;

void main() {
    vec2 uv = gl_FragCoord.xy / u_resolution;

    vec2 point[5];
    point[0] = vec2(0.83,0.75);
    point[1] = vec2(0.60,0.07);
    point[2] = vec2(0.28,0.64);
    point[3] =  vec2(0.31,0.26);
    point[4] = u_mouse/u_resolution;


    vec2 distort;
    float strength = 0.005;
    distort.x = (snoise(vec3(uv * 10., 5.)) - 0.5) * 2 * strength;
    distort.y = (snoise(vec3(uv * 10., 100.)) - 0.5) * 2 * strength;

     uv += distort;


    float min_dist = 1.;
    for(int i = 0; i < 5; i++) {
        min_dist = min(min_dist, distance(uv, point[i]));
    }

    float mod_value = 0.2;
    float value = mod(min_dist, mod_value) * (1. / mod_value);

    //gl_FragColor = vec4(vec3(sin(value * 3.1415)), 1.);

    gl_FragColor = vec4(vec3(snoise(vec3(uv * 10., 1.))), 1.);
}
