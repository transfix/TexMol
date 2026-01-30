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
const char cgGLGaussFragmentShader[] =
	"// this is the fragment program\n"
	"\n"
	"\n"
	"struct inputs \n"
	"{\n"
	"	float4 HPOS : POSITION;\n"
	"	float4 texcoord : TEXCOORD0;\n"
	"	float4 primarycolor : COLOR0;\n"
	"};\n"
	"\n"
	"float4 main(inputs IN,\n"
	"			uniform float4 maincolor,\n"
	"			uniform sampler2D blurmap\n"
	"			) : COLOR\n"
	"{\n"
	"\n"
	"	float4 blur = tex2D(blurmap, IN.texcoord.xy);\n"
	"	float3 color = IN.primarycolor.rgb;\n"
	"	//float3 color = IN.primarycolor.rgb*(IN.primarycolor.a + blur.y) + blur.z;\n"
	"	//float3 color = IN.primarycolor.rgb*(IN.primarycolor.a + blur.y);\n"
	"\n"
	"	float4 outcolor = float4(color, blur.w);\n"
	"\n"
	"	return outcolor;\n"
	"}\n";
