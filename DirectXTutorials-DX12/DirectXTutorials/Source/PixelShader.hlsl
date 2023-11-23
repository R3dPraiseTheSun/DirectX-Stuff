Texture2D g_Texture     : register(t0);
SamplerState g_Sampler  : register(s0);

struct PixelShaderInput
{
    float4 Color    : COLOR;
    float3 Normal   : NORMAL;
    float3 LightDir : TEXCOORD0;
    float2 TexCoord : TEXCOORD1;
};

float4 main( PixelShaderInput IN ) : SV_Target
{
    float4 color = g_Texture.Sample(g_Sampler, IN.TexCoord);
    
     // Calculate diffuse component
    float diffuseIntensity = max(0.0f, dot(IN.Normal, -IN.LightDir));
    float3 diffuseColor = color.rgb + float3(1.0f, 1.0f, 1.0f);
    float3 diffuse = diffuseIntensity * diffuseColor;
    
    // Combine ambient, diffuse, and specular components
    float3 ambient = float3(0.1f, 0.1f, 0.1f); // Example ambient color
    float3 finalColor = ambient + diffuse;
    
    float alpha = color.a;
    
    return float4(finalColor, alpha);
}