uniform sampler2D samplerDiff;
uniform sampler2D samplerNormal;
uniform sampler2D samplerIncandescence;
uniform highp vec4 Light0_Dir;
varying highp vec2 uv0;
varying highp vec3 normal;
varying highp vec3 lightrel;
void main(void)
{
	highp float amb = 0.3;
	highp vec3 nn = normalize(normal);
	highp vec3 bump = normalize( texture2D(samplerNormal, uv0).xyz * 2.0 - 1.0);
	highp float diffuse = dot(normalize(lightrel), bump);
	if(diffuse < 0.0) diffuse = 0.0;
	diffuse = diffuse + amb;
	vec4 bTex = vec4(diffuse,diffuse,diffuse,1) * texture2D(samplerDiff, uv0);
	vec4 Inca = texture2D(samplerIncandescence, uv0);
	if(Inca.x > bTex.x) bTex.x = Inca.x;
	if(Inca.y > bTex.y) bTex.y = Inca.y;
	if(Inca.z > bTex.z) bTex.z = Inca.z;
	if(Inca.w > bTex.w) bTex.w = Inca.w;
	gl_FragColor = bTex;//max(vec4(diffuse,diffuse,diffuse,1) * texture2D(samplerDiff, uv0),texture2D(samplerIncandescence, uv0));//texture2D(sampler2d,uv0) * c0;
}
