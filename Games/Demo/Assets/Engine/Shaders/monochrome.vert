uniform mat4 uMVP;

attribute vec2 aPosition;
attribute vec2 aTexCoords;

varying vec2 vTexCoords;

void main() {
	vTexCoords = aTexCoords;
	gl_Position = uMVP * vec4(aPosition, 0.0, 1.0);
}