float4x4 world;
float4x4 view;
float4x4 proj;

float4x4 vp;

texture tex;

float uv_width;
float uv_height;
float uv_left;
float uv_top;

float alpha;

sampler smp = sampler_state {
	texture = <tex>;
};

struct VS_IN {
	float3 pos : POSITION;
	float2 uv : TEXCOORD0;
};

struct VS_OUT {
	float4 pos : POSITION;
	float2 uv : TEXCOORD0;
};

// vertex shader
VS_OUT vs_main(VS_IN In) {
	VS_OUT Out = (VS_OUT)0;

	//Out.pos = mul(float4(In.pos, 1.0f), vp);
	Out.pos = mul(float4(In.pos, 1.0f), world);
	//Out.pos = mul(Out.pos, view);
	Out.pos = mul(Out.pos, proj);
	Out.uv = In.uv * float2(uv_width, uv_height) + float2(uv_left, uv_top);

	return Out;
}

// pixel shader
float4 ps_main(VS_OUT In) : COLOR0{
	float4 color = tex2D(smp, In.uv);
	color.a *= alpha;
	return color;
}

technique SpriteDrawing {
	pass p0 {
		VertexShader = compile vs_2_0 vs_main();
		PixelShader = compile ps_2_0 ps_main();

		// alpha blend
		AlphaBlendEnable = true;
		SrcBlend = SRCALPHA;
		DestBlend = INVSRCALPHA;
	}
}
