attribute highp vec4 qt_Vertex;
attribute highp vec2 qt_MultiTexCoord0;
attribute highp vec3 qt_Normal;
uniform highp mat4 qt_ProjectionMatrix;
uniform highp mat4 mvMatrix;
varying highp vec4 qt_Vertex0;
varying highp vec2 qt_TexCoord0;
varying highp vec3 qt_Normal0;

void main(void)
{
    gl_Position = qt_ProjectionMatrix * mvMatrix * qt_Vertex;
    qt_TexCoord0 = qt_MultiTexCoord0;
    qt_Normal0 = normalize(vec3(mvMatrix * vec4(qt_Normal, 0.0)));
    qt_Vertex0 = mvMatrix * qt_Vertex;
}
