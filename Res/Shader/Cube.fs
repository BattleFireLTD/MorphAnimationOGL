#ifdef GL_ES
precision highp float;
#endif
varying vec4 V_WorldPos;
void main(){
	vec3 color=V_WorldPos.rgb*0.5+vec3(0.5);
	color=clamp(color,vec3(0.0),vec3(1.0));
	gl_FragColor=vec4(color,1.0);
}