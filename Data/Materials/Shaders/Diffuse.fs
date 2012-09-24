uniform sampler2D samplerDiff;
uniform highp vec4 Light0_Dir;
varying highp vec2 uv0;
varying highp vec3 normal;
void main(void)
{
	highp float amb = 0.3;
	highp vec3 nn = normalize(normal);
	highp vec3 lightdir = normalize(Light0_Dir.xyz);
	highp float c0 = dot(lightdir,nn);
	if(c0 < 0.0) c0 = 0.0;
	c0 = c0 + amb;
	gl_FragColor = texture2D(samplerDiff,uv0) * c0;
}
