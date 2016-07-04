precision mediump float;

uniform sampler2D uDiffuseTexture;
uniform vec3 uColor;

varying vec2 vTexCoords;

void main() {
	gl_FragColor = texture2D(uDiffuseTexture, vTexCoords) * vec4(uColor, 1.0);
} 