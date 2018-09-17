float4 VS( float4 pos : POSITION ) : SV_POSITION
{
	return pos;
}

float4 PS() : SV_Target
{
	return float4(0, 0, 0, 1);
}