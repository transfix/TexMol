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
const char cgGLVertexFunctionShader[] =
	"// this is the vertex program\n"
	"\n"
	"//#pragma bind appdata.position = ATTR0\n"
	"//#pragma bind appdata.normal = ATTR1\n"
	"\n"
	"struct appdata\n"
	"{\n"
	"	float4 center : POSITION;\n"
	"	float4 color : COLOR0;\n"
	"	//float4 offset : TEXCOORD0;\n"
	"};\n"
	"\n"
	"struct vfconn\n"
	"{\n"
	"	float4 HPOS : POSITION;\n"
	"	float4 TEX0 : TEXCOORD0;\n"
	"	float4 TEX1 : TEXCOORD1;\n"
	"	//float4 TEX2 : TEXCOORD2;\n"
	"	//float4 TEX3 : TEXCOORD3;\n"
	"	float4 primarycolor : COLOR0;\n"
	"	//float3 secondarycolor : COLOR1;\n"
	"};\n"
	"\n"
	"float4 remapTo01(float4 v, float4 low, float4 high) {\n"
	"	return saturate((v - low)/(high-low));\n"
	"}\n"
	"\n"
	"vfconn main(appdata IN,\n"
	"        uniform float4x4 ModelView,\n"
	"        uniform float4x4 ModelViewProj,\n"
	" 	 uniform float4x4 Proj,\n"
	"	 uniform float4x4 ModelViewInverse)\n"
	"{\n"
	"	vfconn OUT;\n"
	"	float3 offset;\n"
	"	float ambient;\n"
	"	if(IN.center.w == 0) {\n"
	"               offset = float3(-1,-1,0);\n"
	"               ambient = 0.1;\n"
	"       }\n"
	"       else if(IN.center.w == 1) {\n"
	"               offset = float3(1,-1,0);\n"
	"               ambient = 0.6;\n"
	"       }\n"
	"       else if(IN.center.w == 2) {\n"
	"               offset = float3(1,1,0);\n"
	"               ambient = 0.1;\n"
	"       }\n"
	"       else if(IN.center.w == 3) {\n"
	"               offset = float3(-1,1,0);\n"
	"               ambient = 0.1;\n"
	"       }\n"
	"	OUT.TEX0 = remapTo01(float4(offset, 0.0), (float4)-1.0, (float4)1.0);\n"
	"	OUT.TEX0.zw = float2(0,1);\n"
	"	IN.center.w = 1.0;\n"
	"	float radius = IN.color.w;\n"
	"\n"
	"	// eye_space_offset = radius * v[OPOS]\n"
	"	float4 eye_space_offset = radius * float4(offset, 0.0);\n"
	"\n"
	"	// object_space_offset = INV_MV * eye_space_offset\n"
	"	float4 object_space_offset = mul(ModelViewInverse, eye_space_offset);\n"
	"\n"
	"	IN.center.xyz = IN.center.xyz + object_space_offset.xyz;\n"
	"	//CHA: send eye position to fragment shader for depth correction\n"
	"	OUT.TEX1 = mul(ModelView, IN.center);\n"
	"	OUT.TEX1.w = radius;\n"
	"\n"
	"	// clip coordinates\n"
	"	OUT.HPOS = mul(ModelViewProj, IN.center);\n"
	"	OUT.primarycolor.xyz = IN.color.xyz;\n"
	"	OUT.primarycolor.w = ambient;\n"
	"\n"
	"	return OUT;\n"
	"} // main;\n";
