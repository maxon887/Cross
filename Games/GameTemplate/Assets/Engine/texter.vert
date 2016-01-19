attribute vec4 aPosition;
varying vec2 vTexCoord;

void main(void) {
  gl_Position = vec4(aPosition.xy, 0, 1);
  vTexCoord = aPosition.zw;
}