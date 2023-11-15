attribute highp vec4 qt_Vertex;
uniform highp mat4 qt_ShadowMatrix;
uniform highp mat4 qt_ModelMatrix;
uniform highp mat4 qt_ProjectionLightMatrix;
varying highp vec4 qt_Vertex0;

void main(void)
{
    qt_Vertex0 = qt_ProjectionLightMatrix * qt_ShadowMatrix * qt_ModelMatrix * qt_Vertex;
    gl_Position = qt_Vertex0;
}
