varying vec4 vColor;
varying vec4 vTexCoord;

uniform sampler2D uTexture1;
uniform sampler2D uTexture2;

void main() {
	gl_FragColor = mix(texture(uTexture1, vTexCoord), texture(uTexture2, vTexCoord), 0.2);
	//gl_FragColor = texture(uTexture2, vTexCoord) * vColor;
} 