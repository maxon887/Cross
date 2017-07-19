precision mediump float;

uniform sampler2D uTexture;
uniform vec4 uColor;

varying vec2 vTexCoords;

void main() {
#ifdef MONOCHROME
	gl_FragColor = uColor;
	gl_FragColor.a = texture2D(uTexture, vTexCoords).r * uColor.a;
#else
	gl_FragColor = texture2D(uTexture, vTexCoords) * uColor;
#endif
} 