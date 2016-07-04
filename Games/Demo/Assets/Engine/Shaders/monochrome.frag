precision mediump float;
varying vec2 vTexCoords;

uniform sampler2D uDiffuseTexture;
uniform vec3 uColor;


void main() {
	gl_FragColor = vec4(uColor, texture2D(uDiffuseTexture, vTexCoords).r);
} 