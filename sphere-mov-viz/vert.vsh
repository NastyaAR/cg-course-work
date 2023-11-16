attribute highp vec4 qt_Vertex;
attribute highp vec2 qt_MultiTexCoord0;
attribute highp vec3 qt_Normal;
uniform highp mat4 qt_ProjectionMatrix;
uniform highp mat4 viewMatrix;
uniform highp mat4 modelMatrix;

uniform highp mat4 qt_ProjectionLightMatrix;
uniform highp mat4 shadowMatrixes[2];

varying highp vec4 qt_VertexesLightMatrix[2];

varying highp vec4 qt_Vertex0;
varying highp vec2 qt_TexCoord0;
varying highp vec3 qt_Normal0;
varying highp mat4 qt_viewMatrix;

void main(void)
{
    mat4 mvMatrix = viewMatrix * modelMatrix;
    gl_Position = qt_ProjectionMatrix * mvMatrix * qt_Vertex;
    qt_TexCoord0 = qt_MultiTexCoord0;
    qt_Normal0 = normalize(vec3(mvMatrix * vec4(qt_Normal, 0.0)));
    qt_Vertex0 = mvMatrix * qt_Vertex;
    qt_viewMatrix = viewMatrix;

    qt_VertexesLightMatrix[0] = qt_ProjectionLightMatrix * shadowMatrixes[0] * modelMatrix * qt_Vertex;
    qt_VertexesLightMatrix[1] = qt_ProjectionLightMatrix * shadowMatrixes[1] * modelMatrix * qt_Vertex;
}
