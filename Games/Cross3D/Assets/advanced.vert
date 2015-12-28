attribute vec4 aPosition;
attribute vec4 aTexCoord;

varying vec4 vTexCoord;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main() {
	vTexCoord = aTexCoord;
	gl_Position = uProjection * uView * uModel * aPosition;
}