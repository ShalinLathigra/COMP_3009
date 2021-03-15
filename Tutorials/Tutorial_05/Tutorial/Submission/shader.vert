#version 330

in vec3 vertex;

out vec4 vColor;

uniform float time;
uniform mat4 projection_mat;
uniform mat4 view_mat;
uniform mat4 world_mat;

#define PI 3.1415926538

float easeInOutElastic(float t)
{
	float c5 = (2.0 * PI) / 4.5;

	if (t == 0.0)
	{
		return 0.0;
	} 
	else if (t == 1.0)
	{
		return 1.0;
	}
	else if (t < 0.5)
	{
		return -(pow(2.0, 20.0 * t - 10.0) * sin((20.0 * t - 11.125) * c5)) / 2.0;
	}
	else
	{
		return (pow(2.0, -20.0 * t + 10.0) * sin((20.0 * t - 11.125) * c5)) / 2.0 + 1.0;
	}
}

uniform float transition_rate = 0.5;

void main(void)
{
	// I realize this was probably overkill for the tutorial but I wanted to see what the elastic easing function would look like

	float t = easeInOutElastic((sin(time * transition_rate) + 1.0) * 0.5);
	vColor = vec4(1.0, 0,0,1.0) * (1.0 - t) + vec4(0.0, 1.0, 0.0, 1.0) * t;
	gl_Position = projection_mat*view_mat*world_mat*vec4(vertex,1.0);

}
