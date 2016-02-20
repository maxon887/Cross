precision mediump float;
varying vec2 vTexCoord;

uniform sampler2D uTexture;
uniform vec4 uColor;


void main() {
	gl_FragColor = vec4(1, 1, 1, texture2D(uTexture, vTexCoord).r) * uColor;
} 