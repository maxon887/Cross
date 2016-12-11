uniform mat4 uMVP;
uniform mat4 uModelMatrix;
uniform mat4 uNormalMatrix;
uniform vec3 uCameraPosition;

#ifdef USE_TILLING_FACTOR
uniform float uTillingFactor;
#endif

attribute vec3 aPosition;
attribute vec2 aTexCoords;
attribute vec3 aNormal;

varying vec2 vTexCoords;
varying vec3 vNormal;
varying vec3 vFragPosition;
varying vec3 vViewDirection;

void main() {
#ifdef USE_TILLING_FACTOR
	vTexCoords = aTexCoords * uTillingFactor;
#else
	vTexCoords = aTexCoords;
#endif
	vNormal = normalize(mat3(uNormalMatrix) * aNormal);
	vFragPosition = vec3(uModelMatrix * vec4(aPosition, 1.0));
	vViewDirection = normalize(uCameraPosition - vFragPosition);
	gl_Position = uMVP * vec4(aPosition, 1.0);
}