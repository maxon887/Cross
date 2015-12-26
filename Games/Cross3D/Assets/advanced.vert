attribute vec4 aPosition;
attribute vec4 aColor;
attribute vec4 aTexCoord;

varying	vec4 vColor;
varying vec4 vTexCoord;

uniform mat4 uTransform;

void main() {
	vColor = aColor;
	vTexCoord = aTexCoord;
	//gl_Position = aPosition *uTransform;
	gl_Position = uTransform * aPosition;
}