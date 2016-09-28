uniform mat4 uMVP;

attribute vec3 aPosition;
attribute vec2 aTexCoords;

varying vec2 vTexCoords;

void main() {
	vTexCoords = aTexCoords;
	gl_Position = uMVP * vec4(aPosition, 1.0);
}