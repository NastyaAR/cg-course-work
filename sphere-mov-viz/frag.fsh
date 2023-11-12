struct Light {
    float power;
    vec3 color;
    vec4 pos;
    vec4 direction;
    int type;
};

uniform sampler2D qt_Texture0;
uniform int numberLights;

uniform Light lights[5];
Light corLights[5];

uniform highp float specParam;
uniform highp float ambParam;

varying highp vec4 qt_Vertex0;
varying highp vec2 qt_TexCoord0;
varying highp vec3 qt_Normal0;
varying highp mat4 qt_viewMatrix;

void main(void)
{
    for (int i = 0; i < numberLights; i++) {
	corLights[i].direction = lights[i].direction;
	corLights[i].pos = qt_viewMatrix * lights[i].pos;
	corLights[i].color = lights[i].color;
	corLights[i].power = lights[i].power;
	corLights[i].type = lights[i].type;
    }

    vec4 resClr = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    vec4 eyePos = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    vec4 srcClr = texture2D(qt_Texture0, qt_TexCoord0);
    vec3 eye = normalize(qt_Vertex0.xyz - eyePos.xyz);
    float len = length(qt_Vertex0.xyz - eyePos.xyz);

    for (int i = 0; i < numberLights; i++) {
	vec4 lightClr = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	vec3 light_vec = normalize(corLights[i].direction.xyz);
	vec3 reflectlLight = normalize(reflect(light_vec, qt_Normal0));
	vec4 diffFactor = srcClr * max(0.0, dot(qt_Normal0, -light_vec)) * corLights[i].power;
	vec4 ambFactor = ambParam * srcClr;
	vec4 specFactor = vec4(corLights[i].color, 1.0) * corLights[i].power * pow(max(0.0, dot(reflectlLight, -eye)), specParam);
	resClr += diffFactor + ambFactor + specFactor;
    }

    gl_FragColor = resClr;
}
