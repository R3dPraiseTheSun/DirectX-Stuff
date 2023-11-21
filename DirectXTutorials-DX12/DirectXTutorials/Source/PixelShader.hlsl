struct PixelShaderInput
{
    float4 Color    : COLOR;
    float3 Normal   : NORMAL;
    float3 LightDir : TEXCOORD0;
};

float4 main( PixelShaderInput IN ) : SV_Target
{
     // Calculate diffuse component
    float diffuseIntensity = max(0.0f, dot(IN.Normal, -IN.LightDir));
    float3 diffuseColor = float3(1.0f, 1.0f, 1.0f); // White diffuse color
    float3 diffuse = diffuseIntensity * diffuseColor;
    
    // Combine ambient, diffuse, and specular components
    float3 ambient = float3(0.2f, 0.2f, 0.2f); // Example ambient color
    float3 finalColor = ambient + diffuse;
    
    float alpha = 1.0f;
    
    return float4(finalColor, alpha);
}