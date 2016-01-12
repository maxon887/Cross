attribute vec2 aPosition;
attribute vec4 aTexCoord;

varying vec4 vTexCoord;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main() {
	vTexCoord = aTexCoord;
	gl_Position = uProjection * uModel * vec4(aPosition, 0.0, 1.0);
}