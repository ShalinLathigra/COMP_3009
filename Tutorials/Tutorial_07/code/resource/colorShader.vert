#version 330

in vec3 vertex;
in vec3 color;
in vec3 normal;
out vec4 vColor;

uniform float time;
uniform mat4 projection_mat;
uniform mat4 view_mat;
uniform mat4 world_mat;

void main(void)

{
	vColor = vec4(normal,1.0);
	gl_Position = projection_mat*view_mat*world_mat*vec4(vertex,1.0);

}