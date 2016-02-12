precision mediump float;
varying vec2 vTexCoord;

uniform sampler2D uTexture;
uniform vec4 uColor;
uniform int uMonochrome;

void main() {
	if(uMonochrome != 0){
		gl_FragColor = vec4(1, 1, 1, texture2D(uTexture, vTexCoord).r) * uColor;
	}else{
		gl_FragColor = texture2D(uTexture, vTexCoord) * uColor;
	}
} 