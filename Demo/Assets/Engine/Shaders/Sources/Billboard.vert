uniform mat4 uMVP;
uniform vec3 uCamPos;

attribute vec3 aPosition;
attribute vec2 aTexCoords;

varying vec2 vTexCoords;

void main() {
    vec3 forward = uCamPos;
    forward = normalize(forward);
    vec3 right = normalize(cross(vec3(0.0, 1.0, 0.0), forward));
    vec3 up = normalize(cross(forward, right));
    mat4 lookAtMatrix = mat4(	right.x, up.x, forward.x, 0.0,
                                right.y, up.y, forward.y, 0.0,
                                right.z, up.z, forward.z, 0.0,
                                0.0, 0.0, 0.0, 1.0	);
	vTexCoords = aTexCoords;
	gl_Position = uMVP * lookAtMatrix * vec4(aPosition, 1.0);
}