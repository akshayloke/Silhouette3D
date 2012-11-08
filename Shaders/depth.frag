#version 120
#extension GL_ARB_texture_rectangle : enable
uniform sampler2DRect depthTex;

//Linearizes the depth value to get it into the 0-1 range
float LinearizeDepth(float zOverW)  {  
    float n = 1.0; // camera z near  
    float f = 10000.0; // camera z far  
    return (2.0 * n) / (f + n - zOverW * (f - n));  
} 

void main(void) {
	float depth = texture2DRect(depthTex, gl_TexCoord[0].st).r;

	//so that black is far and white is near
	depth = 1.0 - LinearizeDepth(depth);

	//do the thresholding
	if (depth < 0.1)
		gl_FragColor = vec4(0, 0, 0, 0);
	else
		gl_FragColor = vec4(1, 1, 1, 1);
}
