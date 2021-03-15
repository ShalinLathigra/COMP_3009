#version 330 

in vec4 vColor; // the vertex colour
out vec4 fragColor;

void main(void)
{
	//Save the color

	//IF ON WINDOWS USE gl_FragColor
	gl_FragColor = vColor;

	//IF ON MAC USE fragColor
	//fragColor = vColor;
}
