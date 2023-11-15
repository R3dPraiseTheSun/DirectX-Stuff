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
};

struct VertexShaderOutput
{
	float4 Color    : COLOR;
    float4 Position : SV_Position;
};

VertexShaderOutput main(VertexPosColor IN)
{
    VertexShaderOutput OUT;

    OUT.Position = mul(gModelViewProjection.MVP, float4(IN.Position, 1.0f));
    OUT.Color = float4(IN.Color, 1.0f);

    return OUT;
}