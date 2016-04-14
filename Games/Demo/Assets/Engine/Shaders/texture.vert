uniform mat4 uMVP;

attribute vec3 aPosition;
attribute vec2 aDiffuseCoords;

varying vec2 vDiffuseCoords;

void main() {
	vDiffuseCoords = aDiffuseCoords;
	gl_Position = uMVP * vec4(aPosition, 1.0);
}