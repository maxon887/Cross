varying vec2 vTexCoord;
uniform sampler2D tex;

uniform vec4 uColor;

void main(void) {
	gl_FragColor = vec4(1, 1, 1, texture2D(tex, vTexCoord).r) * uColor;
}