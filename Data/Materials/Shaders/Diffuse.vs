uniform highp mat4 ProjModelViewMatrix;
uniform highp mat4 CameraMatrix;
uniform highp mat4 ModelMatrix;
uniform highp vec4 Light0_Dir;
attribute highp vec4 myVertex;
attribute highp vec4 myNormal;
attribute highp vec4 myTexture0;
varying highp vec2 uv0;
varying highp vec3 normal;
void main(void)
{
	vec4 nn = myNormal;
	nn.w = 0.0;
	normal = vec3(ModelMatrix * nn);
	uv0 = myTexture0.st;
	gl_Position = ProjModelViewMatrix * CameraMatrix * ModelMatrix * myVertex;
}
