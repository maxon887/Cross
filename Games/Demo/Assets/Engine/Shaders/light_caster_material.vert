uniform mat4 uMVP;
uniform mat4 uNormalMatrix;

attribute vec3 aPosition;
attribute vec3 aNormal;

varying vec3 vNormal;
varying vec3 vFragPosition;

void main() {
	vNormal = mat3(uNormalMatrix) * aNormal;
	vFragPosition = vec3(uNormalMatrix * vec4(aPosition, 1.0));
	gl_Position = uMVP * vec4(aPosition, 1.0);
}