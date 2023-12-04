struct Light {
    float power;
    vec3 color;
    vec4 pos;
    vec4 direction;
    int type;
};

uniform sampler2D qt_Texture0;
uniform sampler2D qt_ShadowMaps0[10];
uniform int numberLights;

uniform Light lights[10];
Light corLights[10];
vec3 tmp[10];
float resShadowParams[10];

uniform highp float specParam;
uniform highp float ambParam;
uniform highp float diffParam;

varying highp vec4 qt_Vertex0;
varying highp vec2 qt_TexCoord0;
varying highp vec3 qt_Normal0;
varying highp mat4 qt_viewMatrix;
varying highp vec4 qt_VertexesLightMatrix[10];

float ShadowMapping(sampler2D map, vec2 coordinates, float cur_depth)
{
    vec4 cur = texture2D(map, coordinates);
    float val = cur.x * 255.0 + 0.5f;
    return step(cur_depth, val);
}

void main(void)
{
    for (int i = 0; i < numberLights; i++) {
	tmp[i] = qt_VertexesLightMatrix[i].xyz / qt_VertexesLightMatrix[i].w;
	tmp[i] = tmp[i] * vec3(0.5) + vec3(0.5);
    }

    vec3 clr = vec3(1.0f, 1.0f, 1.0f);
    float shadowParam = 1.0f;

    for (int i = 0; i < numberLights; i++) {
	shadowParam = ShadowMapping(qt_ShadowMaps0[i], tmp[i].xy, tmp[i].z * 255.0f - 0.6f);
	shadowParam += 0.1f;
	if (shadowParam >= 1.0f)
	    shadowParam = 1.0f;
	resShadowParams[i] = shadowParam;
    }

    for (int i = 0; i < numberLights; i++) {
	corLights[i].direction = qt_viewMatrix * lights[i].direction;
	corLights[i].pos = qt_viewMatrix * lights[i].pos;
	corLights[i].color = lights[i].color;
	corLights[i].power = lights[i].power;
	corLights[i].type = lights[i].type;
    }

    vec4 resClr = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    vec4 eyePos = vec4(0.0f, 0.0f, 0.0f, 1.0f);

    for (int i = 0; i < numberLights; i++) {
	vec3 light_vec;
	light_vec = normalize(corLights[i].direction.xyz);
	vec4 srcClr = texture2D(qt_Texture0, qt_TexCoord0);
	float len = length(qt_Vertex0.xyz - eyePos.xyz);
	vec3 eye = normalize(qt_Vertex0.xyz - eyePos.xyz);
	vec3 reflectlLight = normalize(reflect(light_vec, qt_Normal0));
	vec4 diffFactor = diffParam * srcClr * max(0.0f, dot(qt_Normal0, -light_vec)) * corLights[i].power;
	vec4 ambFactor = ambParam * srcClr;
	vec4 specFactor = vec4(corLights[i].color, 1.0f) * corLights[i].power * pow(max(0.0f, dot(reflectlLight, -eye)), specParam);
	resClr += ((diffFactor * vec4(clr, 1.0f)) + (ambFactor * vec4(clr, 1.0f)) + (specFactor * vec4(clr, 1.0f)) * resShadowParams[i]);
    }

    gl_FragColor = resClr;
}
