uniform mat4 uMVP;
uniform mat4 uModel;

attribute vec3 aPosition;
attribute vec3 aNormal;

varying vec3 vNormal;
varying vec3 vFragPosition;

void main() {
	vNormal = aNormal;
	vFragPosition = vec3(uModel * vec4(aPosition, 1.0));
	gl_Position = uMVP * vec4(aPosition, 1.0);
}