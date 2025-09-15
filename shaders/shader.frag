#version 330 core
in vec4 gl_FragCoord;
out vec4 FragColor;

uniform ivec2 dim;
uniform vec2 initPoint;
uniform float zoom;
uniform bool toggleSets;
uniform vec2 offset;

const float PI = 3.14159265359;

vec4 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return vec4(c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y), 1.0f);
}

vec2 scale_coords(vec2 fragCoords, vec2 axisLen) {
  return vec2((fragCoords.x / dim.x - 0.5) * (axisLen.x) + offset.x,
              (fragCoords.y / dim.y - 0.5) * (axisLen.y) + offset.y);
}

vec4 map_to_color(float t) {
  float r = 9.0 * (1.0 - t) * t * t * t;
  float g = 15.0 * (1.0 - t) * (1.0 - t) * t * t;
  float b = 8.5 * (1.0 - t) * (1.0 - t) * (1.0 - t) * t;

  return vec4(r, g, b, 1.0);
}
              
float f(float x, float p) {
  return pow(cos(sqrt(x) + p), 2);
}

vec4 color(int i, int max) {
  if (i == max) {
    return vec4(0.0f, 0.0f, 0.0f, 1.0f);
  }
  float ratio = float (i) / max;

  return hsv2rgb(vec3(ratio, 1.0f, 1.0f));

  int mod_value = 10;

  return vec4(mod(i, mod_value) / (mod_value - 1), mod(i, mod_value) / (mod_value - 1), mod(i, mod_value) / (mod_value - 1), 1.0f);
}

int mandelbrot(vec2 coords, int max_iter) {
  vec2 z = initPoint;
  vec2 c = coords;
  float temp;

  int iter = 0;
  while (dot(z, z) <= 4 && iter < max_iter) {
    temp = z.x * z.x - z.y * z.y + c.x;
    z.y = 2 * z.x * z.y + c.y;
    z.x = temp;

    iter++;
  }

  return iter;
}

int julia(vec2 coords, int max_iter) {
  vec2 z = coords;
  vec2 c = initPoint;

  float temp;

  int iter = 0;
  while (dot(z, z) <= 4 && iter < max_iter) {
    temp = z.x * z.x - z.y * z.y + c.x;
    z.y = 2 * z.x * z.y + c.y;
    z.x = temp;

    iter++;
  }

  return iter;
}

float zoom_scale_func(float x) {
  return exp(-1 * x);
}

void main() {
  int max_iter = 100;
  float aspectRatio = float(dim.x) / dim.y;
  vec2 coords = scale_coords(gl_FragCoord.xy, vec2(2 * zoom_scale_func(zoom) * aspectRatio, 2 * zoom_scale_func(zoom)));
  int result;
  if (toggleSets) {
    result = julia(coords, max_iter);
  } else {
    result = mandelbrot(coords, max_iter);
  }

  float ratio = float(result) / max_iter;

  FragColor = color(result, max_iter);
}


