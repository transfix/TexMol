/*
  Copyright 2011 The University of Texas at Austin

	Advisor: Chandrajit Bajaj <bajaj@cs.utexas.edu>

  This file is part of TexMol.

  TexMol is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License version 2.1 as published by the Free Software Foundation.

  TexMol is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
const char cgGLFragmentShader[] =
	"// this is the fragment program\n"
	"\n"
	"struct inputs \n"
	"{\n"
	"	float4 HPOS : POSITION;\n"
	"	float4 texcoord : TEXCOORD0;\n"
	"	float4 texcoord1 : TEXCOORD1;\n"
	"	float4 texcoord2 : TEXCOORD2;\n"
	"	//float4 texcoord3 : TEXCOORD3;\n"
	"	float4 primarycolor : COLOR0;\n"
	"	//float3 secondarycolor : COLOR1;\n"
	"};\n"
	"\n"
	"float4 main(inputs IN, out float depth: DEPTH,\n"
	"			uniform float4 maincolor,\n"
	"			uniform float ambient,\n"
	"			uniform sampler2D lightmap,\n"
	"			uniform sampler2D depthmap,\n"
	"			uniform sampler2D normalmap,\n"
	"			uniform sampler3D AOmap,\n"
	"			uniform float2 AOdelta,\n"
	"			uniform float3 bboxmin,\n"
	"			uniform float3 rAOresol,\n"
	"			uniform float4x4 MVT,\n"
	"			uniform float4x4 PT\n"
	"			) : COLOR\n"
	"{\n"
	"\n"
	"	float4 lighting = tex2D(lightmap, IN.texcoord.xy);\n"
	"	float3 color = IN.primarycolor.rgb*(IN.primarycolor.a + lighting.y) + lighting.z;\n"
	"	color = saturate(color);\n"
	"\n"
	"	float depthVal = tex2D(depthmap, IN.texcoord.xy).r;\n"
	"\n"
	"	//CHA: depth correction: depth = p_ndc.z / p_ndc.w\n"
	"	float4 epos = IN.texcoord1;\n"
	"	epos.z += depthVal * IN.texcoord2.w;\n"
	"	float4 ndcpos = mul(PT, epos);\n"
	"	depth = (ndcpos.z / ndcpos.w)*0.5+0.5;\n"
	"\n"
	"	float4 normaleye = float4(tex2D(normalmap, IN.texcoord.xy).rgb, 1.f);\n"
	"	normaleye = normaleye*2.f - 1.f;\n"
	"	float4 normalobj = mul(MVT, normaleye);\n"
	"	float3 AOpos = IN.texcoord2.xyz + normalobj*(IN.texcoord2.w + AOdelta.x*0.5f);\n"
	"	AOpos = (AOpos - bboxmin) * AOdelta.y;\n"
	"	AOpos = AOpos * rAOresol;\n"
	"	float AOval = tex3D(AOmap, AOpos).r;\n"
	"	if( AOval > 1.f ) AOval = 1.f;\n"
	"\n"
	"	float4 outcolor = float4(color*(1.f-AOval) + ambient, lighting.w);\n"
	"	//float4 outcolor = float4(AOval, 1.0f-AOval, 1.0f-AOval, lighting.w);\n"
	"	//float4 outcolor = float4(color, lighting.w);\n"
	"	//float4 outcolor = float4(AOval, AOval, AOval, lighting.w);\n"
	"	//float4 outcolor = float4(AOpos, lighting.w);\n"
	"	return outcolor;\n"
	"}\n";
