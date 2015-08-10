#extension GL_ARB_texture_rectangle : enable
varying float depth;


void main()

{
	gl_FragColor = vec4(depth, depth, depth, 1.0);
}



