$input v_texcoord0

#include <torque6.sc>
#include <skyLight.sc>

uniform vec4 u_generateParams;
SAMPLER2D(Texture0, 0);     // Hammersley Lookup Table
SAMPLERCUBE(u_skyCube, 1);  // Sky Cubemap

void main()
{
    vec2 uv = (v_texcoord0.xy * 2.0) - 1.0;
    vec3 N  = texelCoordToVec(uv, int(u_generateParams.x));

    vec4 sum = vec4(0.0, 0.0, 0.0, 0.0);
    for(int sampleNum = 0; sampleNum < 1024; ++sampleNum)
    {
        //vec2 xi = Hammersley(sampleNum, 1024);
        vec2 xi = texture2D(Texture0, vec2((float)sampleNum / 1024.0, 0.5)).xy;
        vec3 H  = ImportanceSampleGGX( xi, 1.0, N );
        vec3 V  = N;
        vec3 L  = normalize(2.0 * dot( V, H ) * H - V);

        float ndotl = max(0.0, dot(N, L));
        vec3 sample = textureCube(u_skyCube, H).rgb * ndotl;

        sum += vec4(toFilmic(sample), 1.0);
    }
    sum /= sum.w;

    gl_FragColor = vec4(sum.rgb, 1.0);
}