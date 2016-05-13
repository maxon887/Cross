precision mediump float;

uniform sampler2D uDiffuseMap0;
uniform vec4 uColor;

varying vec2 vTexCoords;

void main() {
	gl_FragColor = texture2D(uDiffuseMap0, vTexCoords) * uColor;
} 