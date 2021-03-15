#version 330

in vec3 vertex;
in vec3 color;
in vec3 normal;
in vec2 uv;

out vec4 f_color;
out vec2 f_uv;
out float f_time;

uniform float time;
uniform mat4 projection_mat;
uniform mat4 view_mat;
uniform mat4 world_mat;

void main(void)

{
	f_color = vec4(color,1.0);
	f_uv = uv;
	f_time = time;
	gl_Position = projection_mat*view_mat*world_mat*vec4(vertex,1.0);

}