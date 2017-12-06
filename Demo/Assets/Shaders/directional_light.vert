uniform mat4 uMVP;
uniform mat4 uModelMatrix;
uniform mat4 uNormalMatrix;

attribute vec2 aTexCoords;
attribute vec3 aPosition;
attribute vec3 aNormal;

varying vec2 vTexCoords;
varying vec3 vNormal;
varying vec3 vFragPosition;

void main() {
	vTexCoords = aTexCoords;
	vNormal = mat3(uNormalMatrix) * aNormal;
	vFragPosition = vec3(uModelMatrix * vec4(aPosition, 1.0));
	gl_Position = uMVP * vec4(aPosition, 1.0);
}