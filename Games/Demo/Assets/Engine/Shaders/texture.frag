precision mediump float;

uniform sampler2D uDiffuseTexture;
uniform vec4 uColor;

varying vec2 vDiffuseCoords;

void main() {
	gl_FragColor = texture2D(uDiffuseTexture, vDiffuseCoords) * uColor;
} 