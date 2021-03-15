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
uniform vec3 ambient_color =  vec3(0.2);
uniform vec3 diffuse_color =  vec3(0.75);
uniform vec3 specular_color = vec3(0.8);

//the default color that is used as the basis.
uniform vec3 shape_color = vec3(1.0,0.0,0.0);

//the coefficiants for the light modals (i.e. ambience is 10%)
uniform float coefA =0.1;
uniform float coefD = 1;
uniform float coefS = 1.5;

//the shine used for the specular lighting. The larger the value, the 
//brighter it is
uniform float shine = 5.0f;

void main(){

	vec3 lightPosArr [2];
	lightPosArr[0] = vec3(240,200,210);

	//the default vtx color. 
	vec4 vtxColor  = vec4(shape_color,1.0);
	vec3 lighting  = vec3(0,0,0);

	vec4 vtxPos = view_mat*world_mat*vec4(vertex, 1);

	for(int i = 0; i<2; i++){	
		//For the position, light source and normals, we need to convert them to view space
		vec4 vtxLight = view_mat*vec4(lightPosArr[i],1);
		vec4 vtxNorm = transpose(inverse(view_mat*world_mat))*vec4(normal, 1);

		vec3 N = vec3(vtxNorm);
		vec3 V = vec3(vtxPos);

		vec3 ambient, diffuse, specular;
	
		//ambient----------------------------------------
		ambient = coefA * ambient_color;

		//diffuse----------------------------------------
		vec3 L = normalize(vec3(vtxLight) - V); 
		N = normalize(N);

	
		diffuse = coefD * diffuse_color * max(dot(L, N), 0.f);
	
		//specular----------------------------------------

		//because we're in view space, everything is relative to the camera, so no need to 
		//subtract the vertex position from the camera position
		V =  normalize(-V);

		vec3 R = reflect(-L, N);
		specular = coefS * specular_color * pow(max(dot(R, V), 0.f), shine);
		//apply 
		//lighting += (ambient+diffuse+specular);
		lighting = ambient + diffuse;
	}
	vtxColor.rgb *=lighting;
	fColor = vec4(vtxColor.rgb, 1.0);
	
	gl_Position = projection_mat*vtxPos;
}
