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
const char cgGLCylFragmentShader[] =
	"// this is the fragment program\n"
	"\n"
	"struct inputs\n"
	"{\n"
	"	float4 HPOS : POSITION;\n"
	"	float4 texcoord : TEXCOORD0;\n"
	"	float4 texcoord1 : TEXCOORD1;\n"
	"	float4 texcoord2 : TEXCOORD2;\n"
	"	//float4 texcoord3 : TEXCOORD3;\n"
	"	float4 primarycolor : COLOR0;\n"
	"	float3 secondarycolor : COLOR1;\n"
	"};\n"
	"\n"
	"float3 expand(float3 vector)\n"
	"{\n"
	"    return 2.0 * (vector - 0.5);\n"
	"}\n"
	"\n"
	"// register combiner vector normalization function\n"
	"float3 normalize1(float3 vector)\n"
	"{\n"
	"    float3 VdotV = dot(expand(vector), expand(vector));\n"
	"    return (1-saturate(VdotV)) * (vector-0.5) + expand(vector);\n"
	"}\n"
	"float3 normalize2(float3 vector)\n"
	"{\n"
	"    float3 VdotV = dot(vector, vector);\n"
	"    float d = rsqrt( VdotV );\n"
	"    return vector * d;\n"
	"}\n"
	"\n"
	"float4 main(inputs IN, out float depth: DEPTH,\n"
	"			uniform float4 maincolor,\n"
	"			uniform sampler1D normalmap,\n"
	"			uniform sampler2D depthmap,\n"
	"			uniform float specweight,\n"
	"			uniform float4x4 PT) : COLOR\n"
	"{\n"
	"\n"
	"	//float3 color = tex2D(alphamap, IN.texcoord.xy).xyz;\n"
	"	float4 normal = 2.0 * (tex1D(normalmap, IN.texcoord.x) - 0.5);\n"
	"	IN.secondarycolor.rgb = normalize1(IN.secondarycolor.rgb);\n"
	"\n"
	"	float3 NdotL = dot(normal.rgb, IN.secondarycolor);\n"
	"	float3 color = IN.primarycolor.rgb*(saturate(NdotL) + IN.primarycolor.w);\n"
	"\n"
	"	// calculate reflection vector\n"
	"	// R = 2 * (N * (N dot L) - L/2)\n"
	"	//   = 2 * N * (N dot L) - L\n"
	"	//float3 reflectVec = ((NdotL * normal.rgb) + (-0.5 * IN.secondarycolor)) * 2.0;\n"
	"	float3 reflectVec = normal.rgb * NdotL * 2.f - IN.secondarycolor.rgb;\n"
	"	reflectVec = normalize2( reflectVec );\n"
	"\n"
	"	// eye vector E = (0, 0, 1)\n"
	"	// R dot E = (R.r*0 + R.g*0 + 1*R.b) = R.b\n"
	"	float RdotE = saturate(reflectVec.b);   // max(0, RdotE);\n"
	"\n"
	"	// calculate specular as pow(RdotE, 16)\n"
	"	float specular = pow(RdotE, 16); \n"
	"\n"
	"	// add specular light contribution to final color\n"
	"	color += specular * specweight;\n"
	"\n"
	"	float depthVal = tex2D(depthmap, IN.texcoord1.xy).r;\n"
	"\n"
	"	//CHA: depth correction: depth = p_ndc.z / p_ndc.w\n"
	"	float4 epos = float4(IN.texcoord2.xyz, 1.0);\n"
	"	epos.z += depthVal * IN.texcoord2.w;\n"
	"	float4 ndcpos = mul(PT, epos);\n"
	"	depth = (ndcpos.z / ndcpos.w)*0.5+0.5;\n"
	"\n"
	"	float4 outcolor = float4(color, normal.w);\n"
	"	return outcolor;\n"
	"}\n";

