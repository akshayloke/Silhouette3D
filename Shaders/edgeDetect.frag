#version 120
#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect depthTex;
uniform sampler2DRect tex;

float IsThisPixelAnEdge(in vec2 coords){
	float pixelIntensity[9];
	int k = -1;
	float deltaIntensity;

	// populate intensities for neighboring pixels
	for (int i=-1; i<=1; i++) {
		for(int j=-1; j<=1; j++) {
			k++;
			pixelIntensity[k] = texture2DRect(depthTex, coords + vec2(i, j)).r;
		}
	}

	// average the differences
	deltaIntensity = (abs(pixelIntensity[1] - pixelIntensity[7])+ 
						abs(pixelIntensity[5] - pixelIntensity[3])+ 
						abs(pixelIntensity[0] - pixelIntensity[8])+
						abs(pixelIntensity[2] - pixelIntensity[6]))/4.0;
	return deltaIntensity;
}

void main(void) {
	vec4 currentPixelColor = texture2DRect(tex, gl_TexCoord[0].st);
	float currentDepthPixelColor = texture2DRect(depthTex, gl_TexCoord[0].st).r;
	float isEdge = IsThisPixelAnEdge(gl_TexCoord[0].st);
	//if it is an edge pixel give it a different color for the silhouette
	//else make sure the alpha is correctly rendered for the other pixels
	if (isEdge > 0.1)
		gl_FragColor = vec4(currentPixelColor.x, 
				     clamp(currentPixelColor.y + 100 * isEdge, 0.0, 1.0),
				     currentPixelColor.z, 1.0);
	else
		gl_FragColor = vec4(currentPixelColor.x, 
				     currentPixelColor.y,
				     currentPixelColor.z, currentDepthPixelColor);

}