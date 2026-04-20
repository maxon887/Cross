uniform mat4 uMVP;
uniform float uRotationAngle;

attribute vec3 aPosition;
attribute vec2 aTexCoords;

varying vec2 vTexCoords;

void main() {
    float rotationRads = uRotationAngle * 3.14 / 180.0;
    float cosA = cos(rotationRads);
    float sinA = sin(rotationRads);
    mat4 rotationMat = mat4(	1.0, 0.0, 0.0, 0.0,
                                0.0, cosA, sinA * -1.0, 0.0,
                                0.0, sinA, cosA, 0.0,
                                0.0, 0.0, 0.0, 1.0	);
	vTexCoords = aTexCoords;
	gl_Position = uMVP * rotationMat * vec4(aPosition, 1.0);
}