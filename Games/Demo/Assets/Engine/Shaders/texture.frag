precision mediump float;

uniform sampler2D uTexture;
uniform vec3 uColor;

varying vec2 vTexCoords;

void main() {
	gl_FragColor = texture2D(uTexture, vTexCoords) * vec4(uColor, 1.0);
} 