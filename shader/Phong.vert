//Vertex shader for Phong shading + texture mapping
varying vec3 N; //normal vector
varying vec3 L[2]; //light direction vector
varying vec3 E; //V vector for specular.
varying vec3 FP; //fragment position

uniform vec3 EP; //eye position

void main()
{
	vec4 eyePosition = gl_ModelViewMatrix*gl_Vertex;
	vec4 eyeLightPos1 = gl_ModelViewMatrix*gl_LightSource[0].position;
	vec4 eyeLightPos2 = gl_ModelViewMatrix*gl_LightSource[1].position;

	//Compute the key vector parameters for the vertex.
	N =  normalize(gl_NormalMatrix*gl_Normal);
	L[0] = normalize(eyeLightPos1.xyz - eyePosition.xyz);
	L[1] = normalize(eyeLightPos2.xyz - eyePosition.xyz);
	E = -eyePosition.xyz;

	//Passing the texture coordinates
	gl_TexCoord[0] = gl_MultiTexCoord0;
	//Compute the projection space coordinates
	gl_Position = gl_ModelViewProjectionMatrix*gl_Vertex;
	//Passing and compute the fragment position
	FP = gl_Position.xyz/gl_Position.w;
}