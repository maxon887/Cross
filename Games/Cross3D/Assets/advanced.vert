attribute vec4 aPosition;
attribute vec4 aColor;
attribute vec4 aTexCoord;

varying	vec4 vColor;
varying vec4 vTexCoord;

void main() {
	vColor = aColor;
	vTexCoord = aTexCoord;
	gl_Position = aPosition;
}