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

#ifdef USE_NORMAL_MAP
attribute vec3 aTangent;
attribute vec3 aBitangent;
varying mat3 vTBN;
#endif

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
	
#ifdef USE_NORMAL_MAP
/*
	vec3 T = normalize(mat3(uNormalMatrix) * aTangent);
	vec3 B = normalize(mat3(uNormalMatrix) * aBitangent);
	vec3 N = normalize(mat3(uNormalMatrix) * aNormal);*/
	
	vec3 T = normalize(vec3(uModelMatrix * vec4(aTangent, 0.0)));
	vec3 B = normalize(vec3(uModelMatrix * vec4(aBitangent, 0.0)));
	vec3 N = normalize(vec3(uModelMatrix * vec4(aNormal, 0.0)));
	vTBN = mat3(T, B, N);
#endif
}