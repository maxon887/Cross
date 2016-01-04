varying vec4 vTexCoord;

uniform sampler2D uTexture;

void main() {
	//gl_FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	gl_FragColor = texture(uTexture, vTexCoord);
} 