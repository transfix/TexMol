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

#include <GL/glew.h>

#include <math.h>
#include <ObjectRenderer/Texture.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace ObjectRendererLibrary;

static const int TRI_STATE_WORKS = 0;
static const int TRI_STATE_DOESNT_WORK= 1;
static const int TRI_STATE_UNKNOWN = 2;

Texture::Texture()
{
	m_Texture3dPresent = TRI_STATE_UNKNOWN;
	m_TextureID = 0;
	glGenTextures(1, (GLuint *)&m_TextureID);
}

Texture::~Texture()
{
  glDeleteTextures(1, (GLuint *)&m_TextureID);
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
	if (!glewIsSupported("GL_VERSION_1_2") ||
	    !glewIsSupported("GL_EXT_texture3D"))
	{
		m_Texture3dPresent = TRI_STATE_DOESNT_WORK;
		return false;
	}
	m_Texture3dPresent = TRI_STATE_WORKS;
	return true;
}

void Texture::normalize(float* n)
{
	float length = n[0]*n[0]+n[1]*n[1]+n[2]*n[2];
	n[0] /= length;
	n[1] /= length;
	n[2] /= length;
}

float Texture::clamp_0_to_1(float x)
{
	return (float)((fabs(x) - fabs(x-1.)) * .5 + .5);
}


void Texture::loadPGM(const char* name)
{
	GLubyte* dest = 0;
	FILE* input;
	int w,h,max;
	char grey[1];
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
	//fprintf(stderr,"reading %d columns %d rows \n",w,h);
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

void Texture::loadPPM(const char* name)
{
	GLubyte* dest = 0;
	FILE* input;
	int w,h,max;
	char rgb[3];
	//GLubyte* vol;
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
	//fprintf(stderr,"reading %d columns %d rows \n",w,h);
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
	glTexImage3DEXT(GL_TEXTURE_3D_EXT, 0, GL_LUMINANCE, w, h, d, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, dest);
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

void Texture::calculateNormalMap(unsigned int w, unsigned int h)
{
	GLshort* dest = new GLshort[w*h*2];
	GLshort* sip = dest;
	GLubyte* rgbdest = new GLubyte[w*h*3];
	GLubyte* uip = rgbdest;
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
		}
	}
	bindTexture(2);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, rgbdest);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	delete [] dest;
	dest = 0;
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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, w, h, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, rgbdest);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	delete [] rgbdest;
	rgbdest = 0;
}

void Texture::calculateDepthMap(int w, int h)
{
	GLushort* img = new GLushort[w*h*2];
	GLushort* ip = img;
	int i, j;
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
				*ip++ = 0;
			}
			else
			{
				n[2] = clamp_0_to_1((float)(sqrt(1. - fi*fi - fj*fj)));
				normalize(n);
				*ip++ = (GLushort)(n[2] * 65535); // depth
				*ip++ = 0;
			}
		}
	}
	bindTexture(2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_HILO_NV, w, h, 0, GL_HILO_NV, GL_UNSIGNED_SHORT, img);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
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
		n[0] = fi;
		n[1] = fj;
		n[2] = (float)(sqrt(1 - fi*fi - fj*fj));
		a = 1.0;
		normalize(n);
		if (n[2] <= 0)
		{
			n[0] = 0;
			n[1] = 0;
			n[2] = 1;
			a = 0.0;
		}
		*uip++ = (GLubyte)(n[0] * 127 + 128);
		*uip++ = (GLubyte)(n[1] * 127 + 128);
		*uip++ = (GLubyte)(n[2] * 127 + 128);
		*uip++ = (GLubyte)(a*255);
	}
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
	GLushort* img = new GLushort[w*h*2];
	GLushort* ip = img;
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
			*ip++ = (GLushort)(n[2] * 65535); // depth
			*ip++ = 0;
		}
	}
	bindTexture(2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_HILO_NV, w, h, 0,
				 GL_HILO_NV, GL_UNSIGNED_SHORT, img);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
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
