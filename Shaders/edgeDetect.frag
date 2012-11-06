#version 120
#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect depthTex;
uniform sampler2DRect tex;
uniform float width;
uniform float height;

float IsEdge(in vec2 coords){
	float pix[9];
	int k = -1;
	float delta;

	// read neighboring pixel intensities
	for (int i=-1; i<2; i++) {
		for(int j=-1; j<2; j++) {
			k++;
			pix[k] = texture2DRect(depthTex, coords + vec2(i, j)).r;
		}
	}

	// average color differences around neighboring pixels
	delta = (abs(pix[1]-pix[7])+ abs(pix[5]-pix[3]) + abs(pix[0]-pix[8]) + abs(pix[2]-pix[6]))/4.0;
	return delta;
}

void main(void) {
	vec4 currentPixelColor = texture2DRect(tex, gl_TexCoord[0].st);
	float currentDepthPixelColor = texture2DRect(depthTex, gl_TexCoord[0].st).r;
	float isEdge = IsEdge(gl_TexCoord[0].st);
	if (isEdge > 0.1)
		gl_FragColor = vec4(currentPixelColor.x, 
						clamp(currentPixelColor.y + 100 * isEdge, 0.0, 1.0),
						currentPixelColor.z, 1.0);
	else
		gl_FragColor = vec4(currentPixelColor.x, 
						currentPixelColor.y,
						currentPixelColor.z, currentDepthPixelColor);
}