attribute vec2 aPosition;

uniform mat4 uProjection;

void main() {
	gl_Position = uProjection * vec4(aPosition, 0.0, 1.0);
	//gl_Position = vec4(0.0, 0.0, 0.0, 1.0);
	//gl_Position = vec4(aPosition, 0.0, 1.0);
}