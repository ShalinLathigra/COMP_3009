#version 330 

in vec4 f_color;
in vec2 f_uv;
in float f_time;

out vec4 fragColor;

uniform sampler2D t1;
uniform sampler2D t2;
void main(void)
{

	// Retrieve texture value
	vec2 uv = f_uv;
	uv += vec2(f_time, 0.0);		// Scroll Left
	uv.x *= -1;		// Reverse Texture

	
    vec4 color1 = texture(t1, uv);
    vec4 color2 = texture(t2, f_uv);

    // Use texture in determining fragment colour
    gl_FragColor = color1 * 0.5 + color2 * 0.5;
}
