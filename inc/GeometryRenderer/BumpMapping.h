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
namespace CCVTexMolGeometryRenderer
{
	const char cgGLBumpMappingFragmentShader[] =
		"// this is the fragment program\n"
		"\n"
		"float4 main(float2 detailCoords : TEXCOORD0,\n"
		"            float2 bumpCoords: TEXCOORD1,\n"
		"            float3 lightVector : COLOR0,\n"
		"            uniform float3 ambientColor,\n"
		"            uniform sampler2D detailTexture : TEXUNIT0,\n"
		"            uniform sampler2D bumpTexture : TEXUNIT1): COLOR\n"
		"{\n"
		"    float3 detailColor = tex2D(detailTexture, detailCoords).rgb;\n"
		"\n"
		"    // Uncompress vectors ([0, 1] -> [-1, 1])\n"
		"    float3 lightVectorFinal = 2.0 * (lightVector.rgb - 0.5);\n"
		"    float3 bumpNormalVectorFinal = 2.0 * (tex2D(bumpTexture, bumpCoords).rgb - 0.5);\n"
		"\n"
		"    // Compute diffuse factor\n"
		"    float diffuse = dot(bumpNormalVectorFinal, lightVectorFinal);\n"
		"\n"
		"    return float4(diffuse * detailColor + ambientColor, 1.0);\n"
		"}\n";

};
