uniform mat4 uMVP;
uniform mat4 uModelMatrix;
uniform mat4 uNormalMatrix;
uniform vec3 uCameraPosition;
uniform float uTillingFactor;

attribute vec3 aPosition;
attribute vec2 aTexCoords;
attribute vec3 aNormal;

varying vec2 vTexCoords;
varying vec3 vNormal;
varying vec3 vFragPosition;
varying vec3 vViewDirection;

void main() {
	vTexCoords = aTexCoords * uTillingFactor;
	vNormal = normalize(mat3(uNormalMatrix) * aNormal);
	vFragPosition = vec3(uModelMatrix * vec4(aPosition, 1.0));
	vViewDirection = normalize(uCameraPosition - vFragPosition);
	gl_Position = uMVP * vec4(aPosition, 1.0);
}