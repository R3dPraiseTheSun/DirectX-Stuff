struct ModelViewProjection
{
    matrix MVP;
};

// Declare the constant buffer
cbuffer ModelViewProjectionCB : register(b0)
{
    ModelViewProjection gModelViewProjection;
};

struct VertexPosColor
{
    float3 Position : POSITION;
    float3 Color    : COLOR;
    float3 Normal   : NORMAL;
    float3 LightDir : TEXCOORD0;
    float2 TexCoord : TEXCOORD1;
};

struct VertexShaderOutput
{
	float4 Color    : COLOR;
    float3 Normal   : NORMAL;
    float3 LightDir : TEXCOORD0;
    float2 TexCoord : TEXCOORD1;
    
    // Always last so it doesnt interfere with the order!!
    float4 Position : SV_Position;
};

VertexShaderOutput main(VertexPosColor IN)
{
    VertexShaderOutput OUT;
    
    // Calculate the normal matrix to transform normals in world space
    float3x3 normalMatrix = (float3x3) gModelViewProjection.MVP;
    normalMatrix[0] = normalize(normalMatrix[0]);
    normalMatrix[1] = normalize(normalMatrix[1]);
    normalMatrix[2] = normalize(normalMatrix[2]);
    
    OUT.Position = mul(gModelViewProjection.MVP, float4(IN.Position, 1.0f));
    
    // Transform normal to world space
    OUT.Normal = normalize(mul(IN.Position, normalMatrix));
    
    // Calculate light direction in world space
    OUT.LightDir = normalize(float3(1.0f, 1.0f, -1.0f));
    
    OUT.Color = float4(IN.Color, 1.0f);
    OUT.TexCoord = IN.TexCoord;

    return OUT;
}