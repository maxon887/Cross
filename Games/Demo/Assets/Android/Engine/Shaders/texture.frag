precision mediump float;

uniform sampler2D uTexture;
uniform vec3 uColor;

varying vec2 vTexCoords;

void main() {
#ifdef MONOCHROME
	gl_FragColor = vec4(uColor, texture2D(uTexture, vTexCoords).r);
#else
	gl_FragColor = texture2D(uTexture, vTexCoords) * vec4(uColor, 1.0);
#endif
} 