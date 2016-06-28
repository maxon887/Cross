precision mediump float;
varying vec2 vTexCoords;

uniform sampler2D uDiffuseTexture;
uniform vec4 uColor;


void main() {
	gl_FragColor = vec4(1, 1, 1, texture2D(uDiffuseTexture, vTexCoords).r) * uColor;
} 