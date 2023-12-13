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

float SampleShadowMap(sampler2D map, vec2 coords, float compare)
{
    vec4 v = texture2D(map, coords);
    float value = v.x * 255.0f + (v.y * 255.0f + (v.z * 255.0f + v.w) / 255.0f) / 255.0f;
    return step(compare, value);
}

float SampleShadowMapLinear(sampler2D map, vec2 coords, float compare, vec2 texelsize)
{
    vec2 pixsize = coords / texelsize + 0.5f;
    vec2 pixfractpart = fract(pixsize);
    vec2 starttexel = (pixsize - pixfractpart) * texelsize;
    float bltexel = SampleShadowMap(map, starttexel, compare);
    float brtexel = SampleShadowMap(map, starttexel + vec2(texelsize.x, 0.0f), compare);
    float tltexel = SampleShadowMap(map, starttexel + vec2(0.0f, texelsize.y), compare);
    float trtexel = SampleShadowMap(map, starttexel + texelsize, compare);

    float mixL = mix(bltexel, tltexel, pixfractpart.y);
    float mixR = mix(brtexel, trtexel, pixfractpart.y);

    return mix(mixL, mixR, pixfractpart.x);
}

float SampleShadowMapPCF(sampler2D map, vec2 coords, float compare, vec2 texelsize)
{
    float result = 0.0f;
    float spcfq = 1.0;
    for(float y = -spcfq; y < spcfq; y += 1.0f)
	for(float x = -spcfq; x < spcfq; x += 1.0f)
	{
	    vec2 offset = vec2(x, y) * texelsize;
	    result += SampleShadowMapLinear(map, coords + offset, compare, texelsize);
	}
    return result / 9.0f;
}

float CalcShadowAmount(sampler2D map, int i)
{
    vec3 value = qt_VertexesLightMatrix[i].xyz / qt_VertexesLightMatrix[i].w;
    value = value * vec3(0.5f) + vec3(0.5f);
    float offset = 2.0f * dot(qt_Normal0, lights[i].direction.xyz);

    return SampleShadowMapPCF(map, value.xy, value.z * 255.0f + offset, vec2(1.0f / 1024.0f));
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
	shadowParam = CalcShadowAmount(qt_ShadowMaps0[i], i);
	shadowParam += 0.01f;
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
