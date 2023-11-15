varying highp vec4 qt_Vertex0;

void main(void)
{
    float depth = qt_Vertex0.z / qt_Vertex0.w;
    depth *= 0.5;
    depth += 0.5;
    float part1 = depth * 255.0;
    float fPart1 = fract(part1);
    float iPart1 = floor(part1) / 255.0;

    float part2 = fPart1 * 255.0;
    float fPart2 = fract(part2);
    float iPart2 = floor(part2) / 255.0;

    float part3 = fPart2 * 255.0;
    float fPart3 = fract(part3);
    float iPart3 = floor(part3) / 255.0;

    gl_FragColor = vec4(iPart1, iPart2, iPart3, fPart3);
}
