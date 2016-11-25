uniform mat4 uMVP;

attribute vec3 aPosition;

varying vec3 vPosition;

void main() {
	
	gl_Position = uMVP * vec4(aPosition, 1.0);
	vPosition = aPosition;
}