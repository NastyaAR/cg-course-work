attribute highp vec4 qt_Vertex;
uniform highp mat4 qt_ModelViewLightMatrix;
uniform highp mat4 qt_ProjectionLightMatrix;
varying highp vec4 qt_Vertex0;

void main(void)
{
    qt_Vertex0 = qt_ProjectionLightMatrix * qt_ModelViewLightMatrix * qt_Vertex;
    gl_Position = qt_Vertex0;
}
