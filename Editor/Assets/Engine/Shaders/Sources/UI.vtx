uniform mat4 uMVP;

attribute vec3 aPosition;
attribute vec2 aTexCoords;
attribute vec4 aColor;

varying vec2 vTexCoords;
varying vec4 vColor;

void main() {
	vTexCoords = aTexCoords;
	vColor = aColor;
	gl_Position = uMVP * vec4(aPosition, 1.0);
}