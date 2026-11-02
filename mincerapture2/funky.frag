#version 330 core
#define PI 3.1415926538

out vec4 FragColor;

uniform vec2 resolution;
uniform float time;

int pixelDist = 70;
int pixelFactor = 100;
float speed = 3.0;

float scalepx(float x)
{
	return x + 0.125 * sin(x * PI + PI);
}

void main()
{
	float rate = time * speed/10;
	vec2 screen = vec2(gl_FragCoord.x / resolution.x, gl_FragCoord.y / resolution.y);
	float ratio = resolution.x / resolution.y;
	float factor = resolution.y/pixelFactor;
	int pixel = int(true&&(mod((scalepx((screen.x - screen.y) * 4) + rate) * resolution.x, pixelDist*ratio) <= resolution.y/pixelFactor)
				 && (mod((sqrt(2*pow(scalepx(screen.x), 2)) + (screen.y * 4) / ratio) * resolution.y, pixelDist*ratio) <= resolution.y/pixelFactor)
				);
	FragColor = vec4(vec3(pixel), 1.0);
}