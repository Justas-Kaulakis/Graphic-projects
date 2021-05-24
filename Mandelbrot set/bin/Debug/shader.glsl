#version 330
#ifdef GL_ES
precision highp float;
#endif

uniform vec2 u_zoomCenter;
uniform float u_zoomSize;
uniform vec2 u_resolution;
uniform int u_maxIterations;

vec3 palette(float t, vec3 c1, vec3 c2, vec3 c3, vec3 c4) {
  float x = 1.0 / 3.0;
  if (t < x) return mix(c1, c2, t/x);
  else if (t < 2.0 * x) return mix(c2, c3, (t - x)/x);
  else if (t < 3.0 * x) return mix(c3, c4, (t - 2.0*x)/x);
  return c4;
}


vec2 f(vec2 z, vec2 c) {
	return mat2(z,-z.y,z.x) * z + c;
}

void main() {
  vec2 uv = gl_FragCoord.xy / u_resolution;

  /* Decide which point on the complex plane this fragment corresponds to.*/
  vec2 c = u_zoomCenter + (uv * 4.0 - vec2(2.0)) * (u_zoomSize / 4.0);


  /* Now iterate the function. */
  vec2 z = vec2(0.0);
  //vec2 z = u_mousePosition;
  bool escaped = false;
  int iterations;
  for (int i = 0; i < 10000; i++) {
    /* Unfortunately, GLES 2 doesn't allow non-constant expressions in loop
       conditions so we have to do this ugly thing instead. */
    if (i > u_maxIterations) break;
    z = f(z, c);
    if (length(z) > 2.0) {
      escaped = true;
      iterations = i;
      break;
    }
  }
  //gl_FragColor = escaped ? vec4(1.0) : vec4(vec3(0.0), 1.0);
  //gl_FragColor = escaped ? vec4(vec3(float(iterations)) / float(u_maxIterations), 1.0) - 0.1 : vec4(vec3(0.0), 1.0);
gl_FragColor = escaped ?
    vec4(palette(float(iterations)/ float(u_maxIterations), vec3(0.02, 0.02, 0.03), vec3(0.1, 0.2, 0.3), vec3(0.0, 0.3, 0.2), vec3(0.0, 0.5, 0.8)), 1.0)
  : vec4(vec3(0.3, 0.5, 0.8), 1.0);
}
