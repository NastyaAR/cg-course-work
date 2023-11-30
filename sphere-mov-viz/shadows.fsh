varying highp vec4 qt_Vertex0;

void main(void)
{
    float depth = qt_Vertex0.z / qt_Vertex0.w;
    depth = depth * 0.5f + 0.5f;
    gl_FragColor = vec4(depth, 0.0f, 0.0f, 0.0f);
}
