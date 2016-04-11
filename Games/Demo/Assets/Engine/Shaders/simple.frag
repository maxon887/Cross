precision mediump float;

uniform sampler2D uDiffuseTexture;

varying vec2 vDiffuseCoords;

void main() {
	//gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
	gl_FragColor = texture2D(uDiffuseTexture, vDiffuseCoords);
} 