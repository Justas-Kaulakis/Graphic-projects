#version 330

uniform vec2 u_srcPos;
uniform vec3 u_lightColor;
uniform float u_lightRadius;

void main() {
    //vec2 uv = gl_FragCoord.xy / u_Resolution;

    vec2 toLight = u_srcPos - gl_FragCoord.xy;


    float strength = 1.0 / u_lightRadius;
    float leng = length(toLight);
    //float brightness = clamp(1.0 - (leng / u_lightRadius), 0.0, 1.0);
    float brightness = clamp((1.0 / ( 3.0+ (strength * leng * leng))) * 5.0, 0.0, 1.0);

    vec3 color = u_lightColor * brightness;

    gl_FragColor = vec4(color, 1.f);
}
