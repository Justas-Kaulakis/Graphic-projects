#version 330
#ifdef GL_ES
precision highp float;
#endif

uniform vec2 u_resolution;


vec2 f(vec2 z, vec2 c) {
	return mat2(z,-z.y,z.x) * z + c;
}

void main() {
  vec2 uv = gl_FragCoord.xy / u_resolution;

  vec2 c = (uv * 4.0 - vec2(2.0)) * (3.1415926535 / 4.0);


  /* Now iterate the function. */
  vec2 z = vec2(0.0);
  bool escaped = false;
  int iterations;
  for (int i = 0; i < 120; i++) {
    z = f(z, c);
    if (length(z) > 2.0) {
      escaped = true;
      iterations = i;
      break;
    }
  }
  //gl_FragColor = escaped ? vec4(1.0) : vec4(vec3(0.0), 1.0);
  float alpha = 0.5;
  gl_FragColor = escaped ? vec4(float(iterations) / 120) - 0.1 : vec4(vec3(0.0), 0.0);
}
