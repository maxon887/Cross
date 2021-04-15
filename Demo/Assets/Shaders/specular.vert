uniform mat4 uMVP;
uniform mat4 uModelMatrix;
uniform mat4 uNormalMatrix;

attribute vec3 aPosition;
attribute vec3 aNormal;

varying vec3 vNormal;
varying vec3 vFragPosition;

void main() {
	vNormal = normalize(vec3(uNormalMatrix * vec4(aNormal, 1.0)));
	vFragPosition = vec3(uModelMatrix * vec4(aPosition, 1.0));
	gl_Position = uMVP * vec4(aPosition, 1.0);
}
