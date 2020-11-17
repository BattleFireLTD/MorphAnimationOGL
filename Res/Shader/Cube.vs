#ifdef GL_ES
precision highp float;
#endif
attribute vec4 position;
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
varying vec4 V_WorldPos;
void main(){
	V_WorldPos=ModelMatrix*position;
	gl_Position=ProjectionMatrix*ViewMatrix*V_WorldPos;
}