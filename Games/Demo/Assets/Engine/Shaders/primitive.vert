uniform mat4 uMVP;

attribute vec2 aPosition;

void main() {
	gl_Position = uMVP * vec4(aPosition, 0.0, 1.0);
}