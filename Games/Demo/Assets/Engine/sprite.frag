varying vec4 vTexCoord;

uniform sampler2D uTexture;

void main() {
	gl_FragColor = texture(uTexture, vTexCoord);
} 