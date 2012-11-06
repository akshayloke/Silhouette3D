#version 120
#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect depthTex;

float LinearizeDepth(float zoverw)  
{  
    float n = 1.0; // camera z near  
    float f = 10000.0; // camera z far  
    return (2.0 * n) / (f + n - zoverw * (f - n));  
} 

void main(void) {
	float depth = texture2DRect(depthTex, gl_TexCoord[0].st).r;
	depth = 1.0 - LinearizeDepth(depth);
	if (depth < 0.1)
		gl_FragColor = vec4(0, 0, 0, 0);
	else
		gl_FragColor = vec4(1, 1, 1, 1);
}