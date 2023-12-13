varying highp vec4 qt_Vertex0;
float tmp[11];

void main(void)
{
    float depth = qt_Vertex0.z / qt_Vertex0.w;
    depth = depth * 0.5f + 0.5f;
    tmp[0] = tmp[1] = depth;

    for (int i = 0; i < 3; i++) {
	int cur = i * 3 + 2;
	tmp[cur] = tmp[cur-2] * 255.0f;
	tmp[cur+1] = fract(tmp[cur]);
	tmp[cur+2] = floor(tmp[cur]) / 255.0f;
    }
    gl_FragColor = vec4(tmp[4], tmp[7], tmp[10], tmp[9]);
}
