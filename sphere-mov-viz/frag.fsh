uniform sampler2D qt_Texture0;
uniform highp vec4 lightPos;
varying highp vec4 qt_Vertex0;
varying highp vec2 qt_TexCoord0;
varying highp vec3 qt_Normal0;



void main(void)
{
    vec4 resClr = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    vec4 eyePos = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    vec4 srcClr = texture2D(qt_Texture0, qt_TexCoord0);
    vec3 eye = normalize(qt_Vertex0.xyz - eyePos.xyz);
    vec3 light = normalize(qt_Vertex0.xyz - lightPos.xyz);
    vec3 reflectlLight = normalize(reflect(light, qt_Normal0));
    float len = length(qt_Vertex0.xyz - eyePos.xyz);
    float specParam = 10.0;
    float ambParam = 0.1;
    float lightPower = 3.0;
    vec4 diffFactor = srcClr * max(0.0, dot(qt_Normal0, -light)) * lightPower / (1.0 + 0.25 * pow(len, 2.0));
    vec4 ambFactor = ambParam * srcClr;
    vec4 specFactor = vec4(1.0, 1.0, 1.0, 1.0) * lightPower * pow(max(0.0, dot(reflectlLight, -eye)), specParam) / (1.0 + 0.25 * pow(len, 2.0));

    resClr += diffFactor + ambFactor + specFactor;

    gl_FragColor = resClr;
}
