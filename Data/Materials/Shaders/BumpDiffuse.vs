uniform highp mat4 ProjModelViewMatrix;
uniform highp mat4 CameraMatrix;
uniform highp mat4 ModelMatrix;
uniform highp mat4 ModelMatrixInv;
uniform highp vec4 Light0_Dir;
attribute highp vec4 myVertex;
attribute highp vec4 myNormal;
attribute highp vec4 myTangent;
attribute highp vec4 myBinormal;
attribute highp vec4 myTexture0;
varying highp vec2 uv0;
varying highp vec3 normal;
varying highp vec3 lightrel;
void main(void)
{
	highp vec3 lightdir = normalize(Light0_Dir.xyz);//normalize(vec3(0.0,1.0,1.0));
	lightdir = vec3(ModelMatrixInv * vec4(lightdir,0.0));
	highp vec3 lightProj;
	vec3 t = normalize(myTangent.xyz);
	vec3 b = normalize(myBinormal.xyz);
	vec3 n = normalize(myNormal.xyz);
	lightProj.x = dot(lightdir,t);
	lightProj.y = dot(lightdir,b);
	lightProj.z = dot(lightdir,n);
	lightrel = lightProj;
	vec4 nn = myNormal;
	nn.w = 0.0;
	normal = vec3(ModelMatrix * nn);
	uv0 = myTexture0.st;
	gl_Position = ProjModelViewMatrix * CameraMatrix * ModelMatrix * myVertex;
}
