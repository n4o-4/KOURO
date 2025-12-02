// 参考 https://www.ronja-tutorials.com/post/024-white-noise/



float rand2dTo1d(float2 value, float2 dotDir = float2(12.9898, 78.233))
{
    float2 smallValue = sin(value);
    float random = dot(smallValue, dotDir);
    random = frac(sin(random) * 143758.5453);
    return random;
}

float rand3dTo1d(float3 value, float3 dotDir = float3(12.9898, 78.233, 37.719))
{
    //make value smaller to avoid artefacts
    float3 smallValue = sin(value);
    //get scalar value from 3d vector
    float random = dot(smallValue, dotDir);
    //make value more random by making it bigger and then taking the factional part
    random = frac(sin(random) * 143758.5453);
    return random;
}

float3 rand3dTo3d(float3 value)
{
    return float3(
        rand3dTo1d(value, float3(12.989, 78.233, 37.719)),
        rand3dTo1d(value, float3(39.346, 11.135, 83.155)),
        rand3dTo1d(value, float3(73.156, 52.235, 09.151))
    );
}

/// chatGPT

//------------------------------------------------------------------------------
// 3D Perlin Noise 関数
//------------------------------------------------------------------------------

// 補間関数（スムーズステップ）
float fade(float t)
{
    return t * t * t * (t * (t * 6 - 15) + 10);
}

// グラデーション関数
float grad(int hash, float x, float y, float z)
{
    int h = hash & 15;
    float u = h < 8 ? x : y;
    float v = h < 4 ? y : (h == 12 || h == 14 ? x : z);
    return ((h & 1) ? -u : u) + ((h & 2) ? -v : v);
}

// パーマネーションテーブル
static const int perm[256] =
{
    151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233, 7, 225,
    140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23,
    190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117,
    35, 11, 32, 57, 177, 33, 88, 237, 149, 56, 87, 174, 20, 125, 136, 171,
    168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166, 77, 146, 158, 231, 83,
    111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244,
    102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208,
    89, 18, 169, 200, 196, 135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173,
    186, 3, 64, 52, 217, 226, 250, 124, 123, 5, 202, 38, 147, 118, 126, 255,
    82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42, 223,
    183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167,
    43, 172, 9, 129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178,
    185, 112, 104, 218, 246, 97, 228, 251, 34, 242, 193, 238, 210, 144, 12, 191,
    179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107, 49, 192, 214, 31, 181,
    199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254, 138,
    236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215,
    61, 156, 180
};

// Perlin Noise（3D）
float PerlinNoise3D(float3 p)
{
    // グリッド座標
    int X = (int) floor(p.x) & 255;
    int Y = (int) floor(p.y) & 255;
    int Z = (int) floor(p.z) & 255;

    // 小数部分
    float x = p.x - floor(p.x);
    float y = p.y - floor(p.y);
    float z = p.z - floor(p.z);

    // フェード関数
    float u = fade(x);
    float v = fade(y);
    float w = fade(z);

    // パーマネーション参照
    int A = (perm[X] + Y) & 255;
    int AA = (perm[A] + Z) & 255;
    int AB = (perm[A + 1] + Z) & 255;
    int B = (perm[X + 1] + Y) & 255;
    int BA = (perm[B] + Z) & 255;
    int BB = (perm[B + 1] + Z) & 255;

    // 補間
    float res = lerp(
        lerp(
            lerp(grad(perm[AA], x, y, z),
                 grad(perm[BA], x - 1, y, z), u),
            lerp(grad(perm[AB], x, y - 1, z),
                 grad(perm[BB], x - 1, y - 1, z), u), v),
        lerp(
            lerp(grad(perm[AA + 1], x, y, z - 1),
                 grad(perm[BA + 1], x - 1, y, z - 1), u),
            lerp(grad(perm[AB + 1], x, y - 1, z - 1),
                 grad(perm[BB + 1], x - 1, y - 1, z - 1), u), v),
        w
    );

    return (res + 1.0f) * 0.5f; // [0,1]に正規化
}

//------------------------------------------------------------------------------
// Curl Noise 生成関数
// PerlinNoise3D() が必要
//------------------------------------------------------------------------------

float3 CurlNoise3D(float3 p)
{
    // 微小値（数値微分のステップ）
    const float eps = 0.1f;

    // 3軸それぞれのノイズ値を取得
    float n1 = PerlinNoise3D(float3(p.x, p.y + eps, p.z));
    float n2 = PerlinNoise3D(float3(p.x, p.y - eps, p.z));
    float n3 = PerlinNoise3D(float3(p.x, p.y, p.z + eps));
    float n4 = PerlinNoise3D(float3(p.x, p.y, p.z - eps));
    float n5 = PerlinNoise3D(float3(p.x + eps, p.y, p.z));
    float n6 = PerlinNoise3D(float3(p.x - eps, p.y, p.z));

    // カール（回転成分）の近似計算
    float3 curl;
    curl.x = n4 - n3 - n2 + n1; // ∂Fz/∂y - ∂Fy/∂z
    curl.y = n5 - n6 - n3 + n4; // ∂Fx/∂z - ∂Fz/∂x
    curl.z = n2 - n1 - n6 + n5; // ∂Fy/∂x - ∂Fx/∂y

    // 正規化（方向ベクトルとして使いやすく）
    return normalize(curl);
}