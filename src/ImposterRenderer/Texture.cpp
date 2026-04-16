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
// Texture.cpp: implementation of the Texture class.

#include <GL/glew.h>

#include <ImposterRenderer/Texture.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#endif

using CCVImposterRenderer::Texture;

static const int TRI_STATE_WORKS = 0;
static const int TRI_STATE_DOESNT_WORK= 1;
static const int TRI_STATE_UNKNOWN = 2;

void normalize(float* n)
{
	float length = n[0]*n[0]+n[1]*n[1]+n[2]*n[2];
	n[0] /= length;
	n[1] /= length;
	n[2] /= length;
}

float clamp_0_to_1(float x)
{
	return (float)((fabs(x) - fabs(x-1.)) * .5 + .5);
}

Texture::Texture()
{
	m_Texture3dPresent = TRI_STATE_UNKNOWN;
	m_TextureID = 0;
	glGenTextures(1, (GLuint *) &m_TextureID);
}

bool Texture::init3DTexture()
{
	if (m_Texture3dPresent != TRI_STATE_UNKNOWN)
	{
		if (m_Texture3dPresent == TRI_STATE_WORKS)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	if(!glewIsSupported("GL_VERSION_1_2") ||
	   !glewIsSupported("GL_EXT_texture3D"))
	  {
	    m_Texture3dPresent = TRI_STATE_DOESNT_WORK;
	    return false;
	  }

	m_Texture3dPresent = TRI_STATE_WORKS;
	return true;
}

Texture::~Texture()
{
  glDeleteTextures(1, (GLuint *)&m_TextureID);
}

void Texture::loadPGM(const char* name)
{
	GLubyte* dest = 0;
	FILE* input;
	int w,h,max;
	char grey[1];
	//GLubyte* vol;
	char buffer[200];
	if ((input = fopen(name,"rb")) == 0)
	{
		printf("Cannot open file %s \n",name);
		exit(1);
	}
	/* read a line of input */
	fgets(buffer,200,input);
	if (strncmp(buffer,"P5",2) != 0)
	{
		fprintf(stderr,"%s is not a binary PGM file \n",name);
		exit(1);
	}
	/* get second line, ignoring comments */
	do
	{
		fgets(buffer,200,input);
	}
	while (strncmp(buffer,"#",1) == 0);
	if (sscanf(buffer,"%d %d",&w,&h) != 2)
	{
		fprintf(stderr,"can't read sizes! \n");
		exit(1);
	}
	dest = new GLubyte[w*h*4];
	/* third line, ignoring comments */
	do
	{
		fgets(buffer,200,input);
	}
	while (strncmp(buffer,"#",1) == 0);
	if (sscanf(buffer,"%d",&max) != 1)
	{
		fprintf(stderr,"what about max size? \n");
		exit(1);
	}
	//	fprintf(stderr,"reading %d columns %d rows \n",w,h);
	int count;
	for (int i=h-1; i>=0; i--)
	{
		for (int j=0; j<w; j++)
		{
			count = fread(grey,sizeof(char),1,input);
			if (count != 1)
			{
				count=10;
			}
			*(dest+i*w*4+j*4+3) = (GLubyte) grey[0];
		}
	}
	fclose(input);
	GLubyte* uip = dest;
	for (int j=0; j < (int)h; j++)
	{
		float fj = 2*j/(h-1.f) - 1;
		for (int i=0; i < (int)w; i++)
		{
			float fi = 2*i/(w-1.f) - 1;
			float n[3];
			n[0] = fi;
			n[1] = fj;
			n[2] = (float)(sqrt(1 - fi*fi - fj*fj));
			normalize(n);
			if (n[2] <= 0)
			{
				n[0] = 0;
				n[1] = 0;
				n[2] = 1;
			}
			*uip++ = (GLubyte)(n[0] * 127 + 128);
			*uip++ = (GLubyte)(n[1] * 127 + 128);
			*uip++ = (GLubyte)(n[2] * 127 + 128);
			uip++;
		}
	}
	bindTexture(2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, w, h, GL_RGBA, GL_UNSIGNED_BYTE, dest);
	delete [] dest;
	dest = 0;
}

void Texture::calculateSphereOpacityAndNormalMap(int size)
{
	GLubyte* dest = 0;
	dest = new GLubyte[size*size*4];
	for (int j=0; j < size; j++)
	{
		float fj = 2*j/(size-1.f) - 1;
		for (int i=0; i < size; i++)
		{
			float fi = 2*i/(size-1.f) - 1;
			if (fi*fi + fj*fj > 1)
			{
				*(dest+i*size*4+j*4+3) = 0;
			}
			else
			{
				*(dest+i*size*4+j*4+3) = 255;
			}
		}
	}
	GLubyte* uip = dest;
	for (int j=0; j < (int)size; j++)
	{
		float fj = 2*j/(size-1.f) - 1;
		for (int i=0; i < (int)size; i++)
		{
			float fi = 2*i/(size-1.f) - 1;
			float n[3];
			n[0] = fi;
			n[1] = fj;
			n[2] = (float)(sqrt(1 - fi*fi - fj*fj));
			normalize(n);
			if (n[2] <= 0)
			{
				n[0] = 0;
				n[1] = 0;
				n[2] = 1;
			}
			*uip++ = (GLubyte)(n[0] * 127 + 128);
			*uip++ = (GLubyte)(n[1] * 127 + 128);
			*uip++ = (GLubyte)(n[2] * 127 + 128);
			uip++;
		}
	}
	bindTexture(2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, size, size, GL_RGBA, GL_UNSIGNED_BYTE, dest);
	delete [] dest;
	dest = 0;
}

void Texture::calculateSphereOpacityAndLightMap(int size)
{
	GLubyte* dest = 0;
	dest = new GLubyte[size*size*4];
	for (int j=0; j < size; j++)
	{
		float fj = 2*j/(size-1.f) - 1;
		for (int i=0; i < size; i++)
		{
			float fi = 2*i/(size-1.f) - 1;
			if (fi*fi + fj*fj > 1)
			{
				*(dest+i*size*4+j*4+0) = 0;
				*(dest+i*size*4+j*4+1) = 0;
				*(dest+i*size*4+j*4+2) = 0;
				*(dest+i*size*4+j*4+3) = 0;
			}
			else
			{
				*(dest+i*size*4+j*4+3) = 255;
			}
		}
	}
	GLubyte* uip = dest;
	for (int j=0; j < (int)size; j++)
	{
		float fj = 2*j/(size-1.f) - 1;
		for (int i=0; i < (int)size; i++)
		{
			float fi = 2*i/(size-1.f) - 1;
			if (fi*fi + fj*fj > 1) {
				uip++;
				uip++;
				uip++;
				uip++;
				continue;
			}

			float n[3];
			n[0] = fi;
			n[1] = fj;
			n[2] = (float)(sqrt(1 - fi*fi - fj*fj));
			normalize(n);
			if (n[2] <= 0)
			{
				n[0] = 0;
				n[1] = 0;
				n[2] = 1;
			}
			//*uip++ = (GLubyte)(n[0] * 127 + 128);
			//*uip++ = (GLubyte)(n[1] * 127 + 128);
			//*uip++ = (GLubyte)(n[2] * 127 + 128);
			//ambient value
			//float ambient = 0.1;
			//diffuse value
			float L[3] = {5.0, 10.0, 14.0};  //this is the same light vector that is hard coded into the vertex shader
			float length = sqrt(L[0]*L[0] + L[1]*L[1] + L[2]*L[2]);
			L[0] = L[0]/length;
			L[1] = L[1]/length;
			L[2] = L[2]/length;
			float NdotL = n[0]*L[0] + n[1]*L[1] + n[2]*L[2];
			//saturate (clamp NdotL to [0,1])
			if(NdotL < 0.0)
				NdotL = 0.0;
			else if(NdotL > 1.0)
				NdotL = 1.0;
			//specular value
			float ref = 2.0 * ((NdotL * n[2]) + (-0.5 * L[2]));
			if(ref < 0.0)
				ref = 0.0;
			else if(ref > 1.0)
				ref = 1.0;
			float spec = pow(ref, 16);
			//*uip++ = (GLubyte)(ambient * 255);
			*uip++ = 0.0;
			*uip++ = (GLubyte)(NdotL * 255);
			*uip++ = (GLubyte)(spec * 255);
			uip++;
		}
	}
	//writePGM( "sphereLight.pgm", (int)size, (int)size, 255, dest, 4 );
	//writePPM( "sphereLight.ppm", (int)size, (int)size, 255, dest, 4 );

	bindTexture(2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, size, size, GL_RGBA, GL_UNSIGNED_BYTE, dest);
	delete [] dest;
	dest = 0;
}

void Texture::calculateElectronDensityBlurMap(int size, float blobby, float denScale)
{
	//#define USE_SHORT_EDB
	#ifdef USE_SHORT_EDB
		#define DATA_TYPE_EDB GLshort
		#define DATA_RANGE_EDB 65535
	#else
		#define DATA_TYPE_EDB GLubyte
		#define DATA_RANGE_EDB 255
	#endif

	DATA_TYPE_EDB* dest = 0;
	dest = new DATA_TYPE_EDB[size*size*4];
	double maxDen = exp( -blobby );
	const double radius = 1.0;
	const double smoothScale = 2.0;

	for (int j=0; j < size; j++)
	{
		double fj = smoothScale*(2*j/(size-1.0) - 1);
		for (int i=0; i < size; i++)
		{
			double fi = smoothScale*(2*i/(size-1.0) - 1);
			double r = fi*fi + fj*fj;
			double r0 = radius * radius;
			double expval = blobby*r/r0 - blobby;
			double den = exp(expval)*denScale;
			
			*(dest+i*size*4+j*4+3) = (DATA_TYPE_EDB)(den/maxDen * DATA_RANGE_EDB);
		}
	}
	DATA_TYPE_EDB* uip = dest;
	for (int j=0; j < (int)size; j++)
	{
		float fj = 2*j/(size-1.f) - 1;
		for (int i=0; i < (int)size; i++)
		{
			float fi = 2*i/(size-1.f) - 1;
			float n[3];
			n[0] = fi;
			n[1] = fj;
			n[2] = (float)(sqrt(1 - fi*fi - fj*fj));
			normalize(n);
			if (n[2] <= 0)
			{
				n[0] = 0;
				n[1] = 0;
				n[2] = 1;
			}
			//*uip++ = (GLubyte)(n[0] * 127 + 128);
			//*uip++ = (GLubyte)(n[1] * 127 + 128);
			//*uip++ = (GLubyte)(n[2] * 127 + 128);
			//ambient value
			//float ambient = 0.1;
			//diffuse value
			float L[3] = {5.0, 10.0, 14.0};  //this is the same light vector that is hard coded into the vertex shader
			float length = sqrt(L[0]*L[0] + L[1]*L[1] + L[2]*L[2]);
			L[0] = L[0]/length;
			L[1] = L[1]/length;
			L[2] = L[2]/length;

			float NdotL = n[0]*L[0] + n[1]*L[1] + n[2]*L[2];
			//saturate (clamp NdotL to [0,1])
			if(NdotL < 0.0)
				NdotL = 0.0;
			else if(NdotL > 1.0)
				NdotL = 1.0;
			//specular value
			float ref = 2.0 * ((NdotL * n[2]) + (-0.5 * L[2]));
			if(ref < 0.0)
				ref = 0.0;
			else if(ref > 1.0)
				ref = 1.0;
			float spec = pow(ref, 16);
			//*uip++ = (GLubyte)(ambient * 255);
			*uip++ = 0.0;
			*uip++ = (DATA_TYPE_EDB)(NdotL * DATA_RANGE_EDB);
			*uip++ = (DATA_TYPE_EDB)(spec * DATA_RANGE_EDB);
			uip++;
		}
	}
	//writePGM( "electronDen.pgm", (int)size, (int)size, 255, dest, 4, 3 );
	
	bindTexture(2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	#ifdef USE_SHORT_EDB 
		gluBuild2DMipmaps(GL_TEXTURE_2D, 4, size, size, GL_RGBA, GL_UNSIGNED_SHORT, dest);
	#else
		gluBuild2DMipmaps(GL_TEXTURE_2D, 4, size, size, GL_RGBA, GL_UNSIGNED_BYTE, dest);
	#endif
	delete [] dest;
	dest = 0;
}

void Texture::loadPPM(const char* name)
{
	GLubyte* dest = 0;
	FILE* input;
	int w,h,max;
	char rgb[3];
	char buffer[200];
	if ((input = fopen(name,"rb")) == 0)
	{
		fprintf(stderr,"Cannot open file %s \n",name);
		exit(1);
	}
	/* read a line of input */
	fgets(buffer,200,input);
	if (strncmp(buffer,"P6",2) != 0)
	{
		fprintf(stderr,"%s is not a binary PPM file \n",name);
		exit(1);
	}
	/* get second line, ignoring comments */
	do
	{
		fgets(buffer,200,input);
	}
	while (strncmp(buffer,"#",1) == 0);
	if (sscanf(buffer,"%d %d",&w,&h) != 2)
	{
		fprintf(stderr,"can't read sizes! \n");
		exit(1);
	}
	dest = new GLubyte[w*h*3];
	/* third line, ignoring comments */
	do
	{
		fgets(buffer,200,input);
	}
	while (strncmp(buffer,"#",1) == 0);
	if (sscanf(buffer,"%d",&max) != 1)
	{
		fprintf(stderr,"what about max size? \n");
		exit(1);
	}
	int count;
	for (int i=h-1; i>=0; i--)
	{
		for (int j=0; j<w; j++)
		{
			count = fread(rgb,sizeof(char),3,input);
			if (count != 3)
			{
				count=10;
			}
			for (int k=0; k<3; k++)
			{
				*(dest+i*w*3+j*3+k) = (GLubyte) rgb[k];
			}
		}
	}
	fclose(input);
	bindTexture(2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, w, h, GL_RGB, GL_UNSIGNED_BYTE, dest);
	delete [] dest;
	dest = 0;
}

bool Texture::loadFunction(unsigned char* m_Data, int w, int h, int d)
{
	if (!init3DTexture())
	{
		return false;
	}
	GLubyte* dest = 0;
	dest = new GLubyte[w*h*d];
	for (int i=0; i<h; i++)
	{
		for (int j=0; j<w; j++)
		{
			for (int k=0; k<d; k++)
			{
				*(dest+i*w*d+j*d+k) = (GLubyte)(m_Data[i*w*d+j*d+k]);
			}
		}
	}
	bindTexture(3);
	// clear previous errors
	GLenum error;
	int c =0;
	while (glGetError()!=GL_NO_ERROR && c<10)
	{
		c++;
	}
	glTexImage3DEXT(GL_TEXTURE_3D_EXT, 0, GL_LUMINANCE, w, h,
								  d, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, dest);
	glTexParameteri(GL_TEXTURE_3D_EXT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D_EXT, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	delete [] dest;
	dest = 0;
	// test for error
	error = glGetError();
	if (error == GL_NO_ERROR)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Texture::calculateHelixOpacityMap(unsigned int w, unsigned int h)
{
	GLubyte* dest = 0;
	dest = new GLubyte[w*h];
	for (int i=0; i<w; i++)
	{
		for (int j=0; j<h; j++)
		{
			*(dest + j*w + i) = 0;
		}
	}
	for (int i=0; i<w; i++)
	{
		for (int j=0; j<2.0 / 4.0 * h; j++)
		{
			int y = i*h/w + j; // w = h is useful here !
			if (y >= (int)h)
			{
				y = y - h;
			}
			if (y >= (int)h)
			{
				printf("what???\n");
			}
			*(dest + (y)*w + i) = (GLubyte)255;
		}
	}
	// clear previous errors
	GLenum error;
	int c =0;
	while (glGetError()!=GL_NO_ERROR && c<10)
	{
		c++;
	}
	// try loading the texture
	bindTexture(2);
	glTexImage2D(GL_TEXTURE_2D, 0, 1, w, h, 0,
				 GL_LUMINANCE, GL_UNSIGNED_BYTE, dest);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	delete [] dest;
	dest = 0;
	// test for error
	error = glGetError();
	if (error == GL_NO_ERROR)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Texture::calculateHelixOpacityMap2(unsigned int w, unsigned int h, int nspiral)
{
	GLubyte* dest = 0;
	dest = new GLubyte[w*h];
	for (int i=0; i<w; i++)
	{
		for (int j=0; j<h; j++)
		{
			*(dest + j*w + i) = 0;
		}
	}
	float b = 1.f / (float)nspiral;
	float a = (float)h / (float)w;

	for (int i=0; i<w; i++)
	{
		for (int j=0; j<h; j++)
		{
			float x = (float)i/(float)w;
			int level = (int)(x/b);
			x = x - level*b;
			float y = (float)j/(float)h;
			y *= a;
			if( level % 2 ) {
				if( x*x/(b*b) + y*y/(a*a) <= 1 )
					*(dest + j*w + i) = 255;
			}
			else {
				if( x*x/(b*b) + y*y/(a*a) > 1 )
					*(dest + j*w + i) = 255;
			}
		}
	}
	//writePGM( "helixOpacity.pgm", (int)w, (int)h, 255, dest, 1, 0 );
	//writePPM( "helixOpacity.ppm", (int)w, (int)h, 255, dest, 1 );

	// clear previous errors
	GLenum error;
	int c =0;
	while (glGetError()!=GL_NO_ERROR && c<10)
	{
		c++;
	}
	// try loading the texture
	bindTexture(2);
	glTexImage2D(GL_TEXTURE_2D, 0, 1, w, h, 0,
				 GL_LUMINANCE, GL_UNSIGNED_BYTE, dest);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	delete [] dest;
	dest = 0;
	// test for error
	error = glGetError();
	if (error == GL_NO_ERROR)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Texture::calculateHelixSilhouetteMap(unsigned int w, unsigned int h, int nspiral, int fw, int fh)
{
	GLubyte* dest = 0;
	dest = new GLubyte[w*h];
	for (int i=0; i<w; i++)
	{
		for (int j=0; j<h; j++)
		{
			*(dest + j*w + i) = 0;
		}
	}
	float b = 1.f / (float)nspiral;
	float a = (float)h / (float)w;

	for (int i=0; i<w; i++)
	{
		for (int j=0; j<h; j++)
		{
			float x = (float)i/(float)w;
			int level = (int)(x/b);
			x = x - level*b;
			float y = (float)j/(float)h;
			y *= a;
			if( level % 2 ) {
				if( x*x/(b*b) + y*y/(a*a) <= 1 )
					*(dest + j*w + i) = 255;
			}
			else {
				if( x*x/(b*b) + y*y/(a*a) > 1 )
					*(dest + j*w + i) = 255;
			}
		}
	}

	GLubyte* sil = 0;
	sil = new GLubyte[w*h];
	for (int i=0; i<w; i++) {
	for (int j=0; j<h; j++) {
		bool silhouette = false;
		GLubyte me = *(dest + j*w + i);
		int avg = 0;
		int cnt = 0;
		for( int m=-fw; m<=fw; m++ ) {
		for( int n=-fh; n<=fh; n++ ) {
			int xi = i + m;
			int yi = j + n;
			if( (xi > -1) && (xi < w) && ( yi > -1) && (yi < h) ) {
				GLubyte val = *(dest + yi*w + xi);
				avg += val;
				cnt++;
				if( me != val ) silhouette = true;
			}
		}}
		if( silhouette )
 			*(sil + j*w + i) = (GLubyte) ((float)avg/(float)cnt);
		else
 			*(sil + j*w + i) = 0;
	}}
	//writePGM( "helixSilhouette.pgm", (int)w, (int)h, sil );

	// clear previous errors
	GLenum error;
	int c =0;
	while (glGetError()!=GL_NO_ERROR && c<10)
	{
		c++;
	}
	// try loading the texture
	bindTexture(2);
	glTexImage2D(GL_TEXTURE_2D, 0, 1, w, h, 0,
				 GL_LUMINANCE, GL_UNSIGNED_BYTE, sil);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	delete [] dest;
	dest = 0;
	delete [] sil;
	sil = 0;
	// test for error
	error = glGetError();
	if (error == GL_NO_ERROR)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Texture::calculateNormalMap(unsigned int w, unsigned int h)
{
	//GLshort* dest = new GLshort[w*h*2];
	//GLshort* sip = dest;
	GLubyte* rgbdest = new GLubyte[w*h*3];
	GLubyte* uip = rgbdest;
	for (int j=0; j < (int)h; j++)
	{
		float fj = 2*j/(h-1.f) - 1;
		for (int i=0; i < (int)w; i++)
		{
			float fi = 2*i/(w-1.f) - 1;
			float n[3];
			if( fi*fi + fj*fj > 1 ) {
				n[0] = fi;
				n[1] = fj;
				n[2] = 0;
			}
			else {
				n[0] = fi;
				n[1] = fj;
				n[2] = (float)(sqrt(1 - fi*fi - fj*fj));
			}
			normalize(n);
/*
			if (n[2] < 0.0001f)
			{
				n[0] = 0;
				n[1] = 0;
				n[2] = 1;
				*sip++ = (GLshort)(n[0] * 32767);
				*sip++ = (GLshort)(n[1] * 32767);
				*uip++ = (GLubyte)(n[0] * 127 + 128);
				*uip++ = (GLubyte)(n[1] * 127 + 128);
				*uip++ = (GLubyte)(n[2] * 127 + 128);
			}
			else
			{
				*sip++ = (GLshort)(n[0] * 32767);
				*sip++ = (GLshort)(n[1] * 32767);
				*uip++ = (GLubyte)(n[0] * 127 + 128);
				*uip++ = (GLubyte)(n[1] * 127 + 128);
				*uip++ = (GLubyte)(n[2] * 127 + 128);
			}
*/
			*uip++ = (GLubyte)(n[0] * 127 + 128);
			*uip++ = (GLubyte)(n[1] * 127 + 128);
			*uip++ = (GLubyte)(n[2] * 127 + 128);
		}
	}
	bindTexture(2);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
				 GL_RGB, GL_UNSIGNED_BYTE, rgbdest);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//delete [] dest;
	//dest = 0;
	delete [] rgbdest;
	rgbdest = 0;
}

void Texture::calculateShadingLookupTable(int w, int h, double shininess)
{
	GLubyte* rgbdest = new GLubyte[w*h*2];
	GLubyte* uip = rgbdest;
	double fi, fj;
	for (int j=0; j<h; j++)
	{
		fj = j / (h-1.0f);
		unsigned char specular = (unsigned char)(255*pow(fj, shininess));
		for (int i=0; i<w; i++)
		{
			fi = i / (w-1.0f);
			*uip++ = (unsigned char)(fi*255); // diffuse
			*uip++ = specular; // specular
		}
	}
	bindTexture(2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, w, h, 0,
				 GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, rgbdest);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	delete [] rgbdest;
	rgbdest = 0;
}

void Texture::calculateDepthMap(int w, int h)
{
	//CHA: get rid of nvidia specific variable GL_HILO_NV

	//GLushort* img = new GLushort[w*h*2];
	//GLushort* ip = img;
	GLfloat* img = new GLfloat[w*h];
	GLfloat* ip = img;
	for (int j=0; j < h; j++)
	{
		float fj = 2*j/(h-1.f) - 1;
		for (int i=0; i < w; i++)
		{
			float fi = 2*i/(w-1.f) - 1;
			float n[3];
			n[0] = fi;
			n[1] = fj;
			if (fi*fi + fj*fj > 1)
			{
				*ip++ = 0;// outside circle, doesnt matter
				//*ip++ = 0;
			}
			else
			{
				n[2] = clamp_0_to_1((float)(sqrt(1. - fi*fi - fj*fj)));
				normalize(n);
				//*ip++ = (GLushort)(n[2] * 65535); // depth
				*ip++ = n[2]; // depth
				//*ip++ = 0;
			}
		}
	}
	//writePGM( "sphereDepth.pgm", (int)w, (int)h, 65535, img, 2, 0 );
	//writePPM( "sphereDepth.ppm", (int)w, (int)h, 65535, img, 2 );

	bindTexture(2);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_HILO_NV, w, h, 0,
	//			 GL_HILO_NV, GL_UNSIGNED_SHORT, img);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, w, h, 0,
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_FLOAT, w, h, 0,
	//			 GL_RED, GL_FLOAT, img);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, w, h, GL_RED, GL_FLOAT, img);
	delete [] img;
	img = 0;
}

void Texture::calculateCylinderNormalMap(unsigned int w)
{
	GLubyte* rgbdest = new GLubyte[w*4];
	GLubyte* uip = rgbdest;
	float a;
	float fj = 0;
	for (int i=0; i < (int)w; i++)
	{
		float fi = 2*i/(w-1.f) - 1;
		float n[3];
		if( fi*fi + fj*fj > 1 ) {
			n[0] = fi;
			n[1] = fj;
			n[2] = 0;
		}
		else {
			n[0] = fi;
			n[1] = fj;
			n[2] = (float)(sqrt(1 - fi*fi - fj*fj));
		}
		normalize(n);
		a = 1.0;
		*uip++ = (GLubyte)(n[0] * 127 + 128);
		*uip++ = (GLubyte)(n[1] * 127 + 128);
		*uip++ = (GLubyte)(n[2] * 127 + 128);
		*uip++ = (GLubyte)(a*255);
	}
/*
	//===============================
	int h = 2048;
	FILE *output = fopen( "cylinderNormal.ppm", "wb" );
	fprintf( output, "P6\n%d %d\n%d\n", w, h, 255 );
	for (int j=h-1; j >= 0; j--)
		for (int i=0; i < w; i++)
		{
			fwrite( rgbdest+i*4,sizeof(GLubyte),3,output);
		}
	fclose(output);
	//===============================
*/

	bindTexture(1);
	glTexImage1D(GL_TEXTURE_1D, 0, 4, w, 0,
				 GL_RGBA, GL_UNSIGNED_BYTE, rgbdest);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	delete [] rgbdest;
	rgbdest = 0;
}

void Texture::calculateCylinderDepthMap(unsigned int w)
{
	unsigned int h = w;
	//CHA: get rid of nvidia specific variable GL_HILO_NV
	//GLushort* img = new GLushort[w*h*2];
	//GLushort* ip = img;
	GLfloat* img = new GLfloat[w*h];
	GLfloat* ip = img;
	for (int j=0; j < (int)h; j++)
	{
		float fj =0;
		for (int i=0; i < (int)w; i++)
		{
			float fi = 2*i/(w-1.f) - 1;
			float n[3];
			n[0] = fi;
			n[1] = fj;
			n[2] = clamp_0_to_1((float)(sqrt(1. - fi*fi - fj*fj)));
			normalize(n);
			*ip++ = n[2]; // depth
			//*ip++ = (GLushort)(n[2] * 65535); // depth
			//*ip++ = 0;
		}
	}
	//writePGM( "cylinderDepth.pgm", w, h, 65535, img, 2, 0);

	bindTexture(2);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_HILO_NV, w, h, 0,
	//			 GL_HILO_NV, GL_UNSIGNED_SHORT, img);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, w, h, GL_RED, GL_FLOAT, img);
	delete [] img;
	img = 0;
}

void Texture::test3DTexture(int w, int h, int d)
{
	if( !init3DTexture() ) return;
	GLfloat *img = new GLfloat[w*h*d*4];
	for(int i = 0; i< w*h*d; i++ ) {
		img[ i*4   ] = 1.f;
		img[ i*4 +1] = 0.f;
		img[ i*4 +2] = 0.f;
		img[ i*4 +3] = 0.f;
	}

	bindTexture(3);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, w, h, d, 0,
				 GL_RGBA, GL_FLOAT, img);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	delete [] img;
	img = 0;
}

unsigned int Texture::getTextureID() const
{
	return m_TextureID;
}


void Texture::bindTexture(int dim) const
{
	if (dim == 1)
	{
		glBindTexture(GL_TEXTURE_1D, m_TextureID);
	}
	if (dim == 2)
	{
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
	}
	if (dim == 3)
	{
		glBindTexture(GL_TEXTURE_3D, m_TextureID);
	}
}

void Texture::writePGM(const char* name, int w, int h, int max, void *img, int ncomp, int compidx)
{
	FILE* output;
	char grey[1];
	if ((output = fopen(name,"wb")) == 0)
	{
		printf("Cannot open file %s \n",name);
		exit(1);
	}
	/* read a line of input */
	fprintf( output, "P5\n");
	fprintf( output, "%d %d\n%d\n", w, h, 255);
	if( max == 255 ) {
		GLubyte *src = (GLubyte *)img;
		for (int i=h-1; i>=0; i--)
		{
			for (int j=0; j<w; j++)
				fwrite( src+(i*w+j)*ncomp+compidx,sizeof(GLubyte),1,output);
		}
	}
	else if( max == 65535 ) {
		GLushort *src = (GLushort *)img;
		for (int i=h-1; i>=0; i--)
		{
			for (int j=0; j<w; j++) {
				GLubyte val = (GLubyte)((*(src+(i*w+j)*ncomp+compidx) / (float)65535)*255.f);
				fwrite(&val,sizeof(GLubyte),1,output);
			}
		}
	}
	else
		printf( "unsupported max: %d\n", max );
	fclose(output);
}

void Texture::writePPM(const char* name, int w, int h, int max, void *img, int ncomp)
{
	FILE *output = fopen( name, "wb" );
	fprintf( output, "P6\n%d %d\n%d\n", w, h, max );
	if( max == 255 ) {
		GLubyte *src = (GLubyte *)img;
		for (int i=h-1; i>=0; i--)
		{
			for (int j=0; j<w; j++)
			{
				if( ncomp > 3 )
					fwrite( src+(i*w+j)*ncomp,sizeof(GLubyte),3,output);
				else {
					fwrite( src+(i*w+j)*ncomp,sizeof(GLubyte),1,output);
					fwrite( src+(i*w+j)*ncomp,sizeof(GLubyte),1,output);
					fwrite( src+(i*w+j)*ncomp,sizeof(GLubyte),1,output);
				}
			}
		}
	}
	else if( max == 65535 ) {
		GLushort *src = (GLushort *)img;
		for (int i=h-1; i>=0; i--)
		{
			for (int j=0; j<w; j++)
			{
				if( ncomp > 3 )
					fwrite(src+(i*w+j)*ncomp,sizeof(GLushort),3,output);
				else {
					fwrite(src+(i*w+j)*ncomp,sizeof(GLushort),1,output);
					fwrite(src+(i*w+j)*ncomp,sizeof(GLushort),1,output);
					fwrite(src+(i*w+j)*ncomp,sizeof(GLushort),1,output);
				}
			}
		}
	}
	else
		printf( "unsupported max: %d\n", max );

	fclose(output);
}

