#version 430

layout(local_size_x = 128, local_size_y = 1, local_size_z = 1) in;

layout(std430, binding=0, row_major) buffer b0 
{
    vec4 data1[];
};

layout(std430, binding=1, row_major) buffer b1 
{
    vec4 data2[];
};

layout(std430, binding=2, row_major) buffer b2 
{
    vec4 colors[];
};

layout(std430, binding=3, row_major) buffer b3 
{
    uint types[];
};

layout(std430, binding=4, row_major) buffer b4 
{
    float misc[];
};

uint width = uint(misc[0]);
uint height = uint(misc[1]);
uint objCount = uint(misc[2]);
vec3 lightPos = vec3(misc[3], misc[4], misc[5]);
vec3 eye = vec3(misc[6], misc[7], misc[8]);
vec3 coi = vec3(misc[9], misc[10], misc[11]);
vec3 up = vec3(misc[12], misc[13], misc[14]);

layout(std430, binding=5, row_major) buffer b5 
{
    uint img[];
};

vec4 jReflect(vec4 V, vec4 N)
{
    return 2.0f * (dot(V, N) * N - V) + V;
}

float rayPlaneIntersect(vec4 e, vec4 v, vec4 pl)
{
    float denom = dot(pl, v);
    if (denom == 0)
    {
        return -1;
    }
    float numer = -dot(pl, e);
    float t = numer / denom;
    if (t < 0)
    {
        return -1;
    }
    return t;
}

float raySphereIntersect(vec4 e, vec4 v, vec4 centerAndRadius)
{
    vec4 center = centerAndRadius;
    center.w = 1;
    float radius = centerAndRadius.w;
    vec4 alpha = e - center;
    float a = dot(v, v);
    if (a == 0)
    {
        return -1;
    }
    float b = 2 * dot(alpha, v);
    float c = dot(alpha, alpha) - radius * radius;
    float disc = b * b - 4 * a * c;
    if (disc < 0)
    {
        return -1;
    }
    disc = sqrt(disc);
    float t1 = (-b + disc) / (2 * a);
    float t2 = (-b - disc) / (2 * a);
    if (t1 < 0 && t2 < 0)
    {
        return -1;
    }
    else if (t1 < 0)
    {
        return t2;
    }
    else if (t2 < 0)
    {
        return t1;
    }
    else if (t1 <= t2)
    {
        return t1;
    }
    else
    {
        return t2;
    }
}

vec2 getRayIntersection(mat4 viewMatrix, vec4 rayStart, vec4 rayDir, int skipIndex)
{
    float closestT = -1;
    int closestO = -1;
    for (uint i = 0; i < objCount; ++i)
    {
        if(skipIndex != -1 && i == skipIndex) continue;

        float t;
        if (types[i] == 0)
        {
            vec4 data3 = data1[i];
            data3.w = 1;
            data3 = data3 * viewMatrix;
            data3.w = data1[i].w;
            t = raySphereIntersect(rayStart, rayDir, data3);
        }
        else
        {
            vec4 tnormal = data1[i] * viewMatrix;
            vec4 tp = data2[i] * viewMatrix;
            vec4 data3 = tnormal;
            data3.w = -dot(tnormal, tp);
            t = rayPlaneIntersect(rayStart, rayDir, data3);
        }

        if (t != -1.0f && t > 0.001f && (closestO == -1 || t < closestT))
        {
            closestT = t;
            closestO = int(i);
        }
    }

    return vec2(closestT, closestO);
}

vec4 getNormal(mat4 viewMatrix, uint index, vec4 closestIp)
{
    vec4 N;
    if(types[index] == 0)
    {
        vec4 toSub = data1[index];
        toSub.w = 1;
        toSub = toSub * viewMatrix;
        N = closestIp - toSub;
    }
    else
    {
        N = data1[index] * viewMatrix;
        N.w = 0;
    }

    return normalize(N);
}

vec4 getColorForRay(mat4 viewMatrix, vec4 rayStart, vec4 rayDir)
{
    vec2 ri1 = getRayIntersection(viewMatrix, rayStart, rayDir, -1);
    float closestT = ri1.x;
    int closestO = int(ri1.y);
    vec4 lightOut = vec4(lightPos, 1) * viewMatrix;

    if (closestO != -1)
    {
        vec4 closestIp = rayStart + (closestT * rayDir);
        vec4 N = getNormal(viewMatrix, closestO, closestIp);

        vec2 ri2 = getRayIntersection(viewMatrix, closestIp, normalize(lightOut - closestIp), closestO);
        float t = ri2.x;

        float dp = 0;
        float sp = 0;
        if(t != -1.0f)
        {
            dp = 0;
            sp = 0;
        }
        else
        {
            vec4 L = normalize(lightOut - closestIp);
            dp = max(0.0f, dot(N, L));
            if (dp > 0)
            {
                vec4 V = normalize(rayStart - closestIp);
                vec4 R = jReflect(L, N);
                sp = dot(V, R);
                if (sp < 0)
                {
                    sp = 0;
                }
                sp = pow(sp, 32);
            }
            else
            {
                sp = 0;
            }
        }

        return (dp * colors[closestO]) + (sp * vec4(1, 1, 1, 1));
    }
    else
    {
        return vec4(0, 0, 0, 1);
    }
}

uint vec4_to_uint(vec4 color)
{
    uint r = clamp(uint(color.r * 255), 0u, 255u) & 0xFFu;
    uint g = clamp(uint(color.g * 255), 0u, 255u) & 0xFFu;
    uint b = clamp(uint(color.b * 255), 0u, 255u) & 0xFFu;
    uint a = clamp(uint(color.a * 255), 0u, 255u) & 0xFFu;
    return ((r << 0) & 0x000000FFu) | ((g << 8) & 0x0000FF00u) | ((b << 16) & 0x00FF0000u) | ((a << 24) & 0xFF000000u);
}

vec4 jCross(vec4 v, vec4 w)
{
    return vec4(v.y * w.z - v.z * w.y, v.z * w.x - v.x * w.z, v.x * w.y - v.y * w.x, 0);
}

void main()
{
    float fov = 45;
    float d = 1.0f / tan((fov / 180.0f) * 3.14159265358979323f);
    float y = 1.0f + -2.0f * gl_GlobalInvocationID.y / (height - 1.0f);
    float x = -1.0f + 2.0f * gl_GlobalInvocationID.x / (width - 1.0f);

    vec4 rayDir = vec4(x, y, -d, 0);
    rayDir = normalize(rayDir);

    vec4 W = normalize(vec4(eye, 1) - vec4(coi, 1));
    vec4 U = normalize(jCross(vec4(up, 0), W));
    vec4 V = normalize(jCross(W, U));
    vec4 T = vec4(-eye, 1);
    mat4 viewMatrix = mat4(
            U.x, V.x, W.x, 0,
            U.y, V.y, W.y, 0,
            U.z, V.z, W.z, 0,
            dot(T, U), dot(T, V), dot(T, W), 1
    );
    viewMatrix = transpose(viewMatrix);

    vec4 col = getColorForRay(viewMatrix, vec4(0, 0, 0, 1), rayDir);
    col.a = 1;

    uint pixel = (gl_GlobalInvocationID.y * width) + gl_GlobalInvocationID.x;
    img[pixel] = vec4_to_uint(col);
}
