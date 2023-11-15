varying highp vec4 qt_Vertex0;

void main(void)
{
    float depth = qt_Vertex0.z / qt_Vertex0.w;
    depth = depth * 0.5f + 0.5f;

    float v1 = depth * 255.0f;
    float f1 = fract(v1);
    float vn1 = floor(v1) / 255.0f;

    float v2 = f1 * 255.0f;
    float f2 = fract(v2);
    float vn2 = floor(v2) / 255.0f;

    float v3 = f2 * 255.0f;
    float f3 = fract(v3);
    float vn3 = floor(v3) / 255.0f;

    gl_FragColor = vec4(vn1, vn2, vn3, f3);
}
