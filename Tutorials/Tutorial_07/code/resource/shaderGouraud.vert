#version 330

in vec3 vertex;
in vec3 color;
in vec3 normal;

//for gouraud, we only need to send the final vertex color
out vec4 fColor;

uniform float time;
uniform mat4 projection_mat;
uniform mat4 view_mat;
uniform mat4 world_mat;

//Uniform for the light position. By default its set to whatever value below. 
uniform vec3 lightPos = vec3(4.0,0,2);

//the colors that the light shines on for the modal 
uniform vec3 ambient_color =  vec3(1);
uniform vec3 diffuse_color =  vec3(1);
uniform vec3 specular_color = vec3(1);

//the default color that is used as the basis.
uniform vec3 shape_color = vec3(0.5,1.0,0.2);

//the coefficiants for the light modals (i.e. ambience is 10%)
uniform float coefA =0.1;
uniform float coefD = 1;
uniform float coefS = 1.5;

//the shine used for the specular lighting. The larger the value, the 
//brighter it is
uniform float shine = 32.0f;

void main(){
	//the default vtx color. 
	vec4 vtxColor  = vec4(shape_color,1.0);

	//For the position, light source and normals, we need to convert them to view space
	vec4 vtxPos = view_mat*world_mat*vec4(vertex, 1);
	vec4 vtxNorm = transpose(inverse(view_mat*world_mat))*vec4(normal, 1);

	vec3 N = vec3(vtxNorm);
	vec3 V = vec3(vtxPos);

	vec3 ambient;
	vec3 diffuse = vec3(0,0,0);
	vec3 specular = vec3(0,0,0);
	
	vec3 lightPositions[3] = vec3[](
								vec3(4.0,-2,2),
								vec3(-8.0,-8,0),
								vec3(0.0,4,-12)
								);
	
	//ambient----------------------------------------
	ambient = coefA * ambient_color;

	//diffuse----------------------------------------
	int i;
	for (i = 0; i < lightPositions.length(); i++)
	{
		vec4 vtxLight = view_mat*vec4(lightPositions[i],1);

		vec3 L = normalize(vec3(vtxLight) - V); 
		vec3 Nb = normalize(N);

		diffuse += coefD * diffuse_color * max(dot(L, Nb), 0.f);
		
		//specular----------------------------------------
		vec3 Vb =  normalize(-V);

		vec3 R=  -L+2*dot(L, Nb)*Nb;
		specular += coefS * specular_color * pow(max(dot(R, Vb), 0.f), shine);
	}

	//apply 
	vtxColor.rgb *= (ambient+diffuse+specular);
	fColor = vtxColor;
	
	gl_Position = projection_mat*vtxPos;
}
