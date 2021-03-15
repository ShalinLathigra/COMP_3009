#version 330 

in vec4 fColor;

out vec4 fragColor;

void main(void)
{
	//Save the color

	//IF ON WINDOWS USE gl_FragColor
	gl_FragColor = fColor;

	//IF ON MAC USE fragColor
	//fragColor = fColor;
}
