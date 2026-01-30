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
const char cgGLFragmentFunctionShader[] =
	"// this is the fragment program\n"
	"\n"
	"struct inputs\n"
	"{\n"
	"	float4 HPOS : POSITION;\n"
	"	float4 texcoord : TEXCOORD0;\n"
	"	float4 texcoord1 : TEXCOORD1;\n"
	"	//float4 texcoord2 : TEXCOORD2;\n"
	"	//float4 texcoord3 : TEXCOORD3;	// duplicate of HPOS to read it\n"
	"	float4 primarycolor : COLOR0;\n"
	"	//float3 secondarycolor : COLOR1;\n"
	"};\n"
	"\n"
	"float3 expand(float3 vector)\n"
	"{\n"
	"    return 2.0 * (vector - 0.5);\n"
	"}\n"
	"\n"
	"// register combiner vector normalization function\n"
	"float3 normalize(float3 vector)\n"
	"{\n"
	"    float3 VdotV = dot(expand(vector), expand(vector));\n"
	"    return (1-saturate(VdotV)) * (vector-0.5) + expand(vector);\n"
	"}\n"
	"\n"
	"float3 interpolateColor( float3 color1, float3 color2, float left, float right, float val )\n"
	"{\n"
	"	float t = (val - left ) / ( right - left );\n"
	"	return (1-t)*color1 + t*color2;\n"
	"}\n"
	"\n"
	"float4 main(inputs IN, out float depth: DEPTH,\n"
	"			uniform float4 maincolor,\n"
	"			uniform sampler2D lightmap,\n"
	"			uniform sampler2D depthmap,\n"
	"			uniform sampler3D functionmap,\n"
	"			uniform float4x4 ModelViewProjInverse,\n"
	"			uniform float3 minExtent,\n"
	"			uniform float3 maxExtent,\n"
	"			uniform float4x4 PT\n"
	"			) : COLOR\n"
	"{\n"
	"	float depthVal = tex2D(depthmap, IN.texcoord.xy).r;\n"
	"\n"
	"	//CHA: depth correction: depth = p_ndc.z / p_ndc.w\n"
	"	float4 epos = float4(IN.texcoord1.xyz, 1.0);\n"
	"	epos.z += depthVal * IN.texcoord1.w;\n"
	"	float4 ndcpos = mul(PT, epos);\n"
	"	depth = (ndcpos.z / ndcpos.w)*0.5+0.5;\n"
	"\n"
	"	float4 objpos = mul(ModelViewProjInverse, ndcpos);\n"
	"\n"
	"	// compress object pos into unit cube\n"
	"	float3 unit = (objpos - minExtent) / (maxExtent - minExtent);\n"
	"	\n"
	"	// do a color map lookup, but now just do something ....\n"
	"	float dens = tex3D( functionmap, unit).r;\n"
	"\n"
	"	float3 surfaceColor;\n"
	"	if( dens >= 0 && dens < 0.46 )\n"
	"	{\n"
	"		surfaceColor = interpolateColor( float3(0.5,0,0), float3(1,0,0), 0, 0.46, dens );\n"
	"	}\n"
	"	if( dens >= 0.46 && dens < 0.5 )\n"
	"	{\n"
	"		surfaceColor = interpolateColor( float3(1,0,0), float3(0,0,0), 0.46, 0.5, dens );\n"
	"	}\n"
	"	if( dens >= 0.5 && dens < 0.529 )\n"
	"	{\n"
	"		surfaceColor = interpolateColor( float3(0,0,0), float3(0,1,0), 0.5, 0.529, dens );\n"
	"	}\n"
	"	if( dens >= 0.529 && dens <= 1.0 )\n"
	"	{\n"
	"		surfaceColor = interpolateColor( float3(0,1,0), float3(0,0.5,0), 0.529, 1.0, dens );\n"
	"	}	\n"
	"	\n"
	"	float4 lighting = tex2D(lightmap, IN.texcoord.xy);\n"
	"	float3 color = surfaceColor.xyz*(IN.primarycolor.a + lighting.y) + lighting.z;\n"
	"	color = saturate(color);\n"
	"\n"
	"	float4 outcolor = float4(color, lighting.w);    \n"
	"	return outcolor;\n"
	"}\n";
