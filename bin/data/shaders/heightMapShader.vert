varying float depth;

void main()
{
	float near = 0.0;//0.0 ;
	float far = 255.0;//255.0;
	
	gl_Position = ftransform();
	gl_TexCoord[0] = gl_MultiTexCoord0;

	vec4 viewPos = gl_ModelViewMatrix * gl_Vertex; // this will transform the vertex into eyespace
	depth = 1.0 - ((- viewPos.z - near)/(far - near)); // will map near..far to 0..1
	gl_Position = ftransform();
   
}


