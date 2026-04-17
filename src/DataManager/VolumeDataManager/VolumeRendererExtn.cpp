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
// VolumeRendererExtn.cpp: implementation of the VolumeRendererExtn class.

#include <DataManager/VolumeDataManager/VolumeRendererExtn.h>
#include <SimpleVolumeData/SimpleVolumeData.h>

VolumeRendererExtn::VolumeRendererExtn()
{
}

VolumeRendererExtn::~VolumeRendererExtn()
{
}

inline double VolumeRendererExtn::texCoordOfSample(double sample, int bufferWidth, int canvasWidth, double bufferMin, double bufferMax) const
{
	// get buffer min and max in the texture's space
	double texBufferMin = 0.5 / (double)canvasWidth;
	double texBufferMax = ((double)bufferWidth - 0.5) / (double)canvasWidth;
	return (sample-bufferMin)/(bufferMax-bufferMin) * (texBufferMax-texBufferMin) + texBufferMin;
}

void VolumeRendererExtn::copyToUploadableBufferDensity(unsigned char* inputData, unsigned char* outputData, unsigned int
		widthX, unsigned int widthY, unsigned int widthZ)
{
	unsigned int j, k;
	unsigned int targetSlice, targetLine;
	unsigned int sourceSlice, sourceLine;
	unsigned int canvasX = upToPowerOfTwo(widthX);
	unsigned int canvasY = upToPowerOfTwo(widthY);
	unsigned int canvasZ = upToPowerOfTwo(widthZ);

	for(k=0; k<widthZ; k++)
	{
		targetSlice = (k)*canvasX*canvasY;
		sourceSlice = k*widthX*widthY;

		for(j=0; j<widthY; j++)
		{
			targetLine = (j)*canvasX;
			sourceLine = j*widthX;
			memcpy(outputData+targetSlice+targetLine, inputData+sourceSlice+sourceLine, widthX);
		}
	}
}

inline unsigned int VolumeRendererExtn::upToPowerOfTwo(unsigned int value) const
{
	unsigned int c = 0;
	unsigned int v = value;

	// round down to nearest power of two
	while(v>1)
	{
		v = v>>1;
		c++;
	}

	// if that isn't exactly the original value
	if((v<<c)!=value)
	{
		// return the next power of two
		return (v<<(c+1));
	}
	else
	{
		// return this power of two
		return (v<<c);
	}
}

// The volume data consists of a simple volume data which needs to be sent to the graphics card.
// We need to check the data type, convert if needed and then upload it.
// Currently, we only support rendering  of unsigned char type.
bool VolumeRendererExtn::loadData(VolumeData* volumeData)
{
	if(!volumeData)
	{
		return false;
	}

	if(!volumeData->m_SimpleVolumeData)
	{
		return false;
	}

	if(volumeData->m_SimpleVolumeData->getNumberOfVariables() < 1)
	{
		return false;
	}

	// Store actual extents to render at correct location
	m_MinX = volumeData->m_SimpleVolumeData->getMinX();
	m_MinY = volumeData->m_SimpleVolumeData->getMinY();
	m_MinZ = volumeData->m_SimpleVolumeData->getMinZ();
	m_MaxX = volumeData->m_SimpleVolumeData->getMaxX();
	m_MaxY = volumeData->m_SimpleVolumeData->getMaxY();
	m_MaxZ = volumeData->m_SimpleVolumeData->getMaxZ();
	// Done storing
	int width = volumeData->m_SimpleVolumeData->getWidth();
	int height = volumeData->m_SimpleVolumeData->getHeight();
	int depth = volumeData->m_SimpleVolumeData->getDepth();
	unsigned int canvasX = upToPowerOfTwo(volumeData->m_SimpleVolumeData->getWidth());
	unsigned int canvasY = upToPowerOfTwo(volumeData->m_SimpleVolumeData->getHeight());
	unsigned int canvasZ = upToPowerOfTwo(volumeData->m_SimpleVolumeData->getDepth());

	// Load either single variable or 4 RGBA data to graphics card
	if(volumeData->m_SimpleVolumeData->getNumberOfVariables() < 4)
	{
		// Load a single variable
		if(!setAspectRatio(
					m_MaxX-m_MinX,
					m_MaxY-m_MinY,
					m_MaxZ-m_MinZ
				))
		{
			return false;
		}

		unsigned char* data = new unsigned char[width*height*depth];
		SimpleVolumeData::getNormalizedUnsignedCharData(volumeData->m_SimpleVolumeData->getData(0),
				volumeData->m_SimpleVolumeData->getType(0),
				data, width, height, depth);
		unsigned char* bigdata = new unsigned char[canvasX*canvasY*canvasZ];
		{
			int i;

			for(i=0; i<canvasX*canvasY*canvasZ; i++)
			{
				bigdata[i] = 0;
			}
		}
		{
			int i, j, k, c=0;

			for(i=0; i<depth; i++)
			{
				for(j=0; j<height; j++)
				{
					for(k=0; k<width; k++)
					{
						bigdata[i* canvasY* canvasX + j* canvasX + k] = data[c];
						c++;
					}
				}
			}
		}

		if(!uploadColorMappedData(bigdata, canvasX, canvasY, canvasZ))
		{
			delete []data;
			delete []bigdata;
			return false;
		}

		setTextureSubCube(
			texCoordOfSample(m_MinX, width,  canvasX, m_MinX, m_MaxX),
			texCoordOfSample(m_MinY, height, canvasY, m_MinY, m_MaxY),
			texCoordOfSample(m_MinZ, depth,  canvasZ, m_MinZ, m_MaxZ),
			texCoordOfSample(m_MaxX, width,  canvasX, m_MinX, m_MaxX),
			texCoordOfSample(m_MaxY, height, canvasY, m_MinY, m_MaxY),
			texCoordOfSample(m_MaxZ, depth,  canvasZ, m_MinZ, m_MaxZ)
		);
		volumeData->setLoaded(true);
		delete []data;
		delete []bigdata;
		// Done loading
	}
	else
	{
		// Load RGBA data
		if(!setAspectRatio(
					m_MaxX-m_MinX,
					m_MaxY-m_MinY,
					m_MaxZ-m_MinZ
				))
		{
			return false;
		}

		unsigned char* data = new unsigned char[width*height*depth*4];
		SimpleVolumeData::getUnsignedCharData(volumeData->m_SimpleVolumeData->getData(0),
											  volumeData->m_SimpleVolumeData->getType(0),
											  volumeData->m_SimpleVolumeData->getData(1),
											  volumeData->m_SimpleVolumeData->getType(1),
											  volumeData->m_SimpleVolumeData->getData(2),
											  volumeData->m_SimpleVolumeData->getType(2),
											  volumeData->m_SimpleVolumeData->getData(3),
											  volumeData->m_SimpleVolumeData->getType(3),
											  data, width, height, depth);
		unsigned char* bigdata = new unsigned char[canvasX*canvasY*canvasZ*4];
		{
			int i;

			for(i=0; i<canvasX*canvasY*canvasZ*4; i++)
			{
				bigdata[i] = 0;
			}
		}
		{
			int i, j, k, c=0;

			for(i=0; i<depth; i++)
			{
				for(j=0; j<height; j++)
				{
					for(k=0; k<width; k++)
					{
						int bigIndex = (i*canvasY*canvasX + j*canvasX + k) *4;
						bigdata[bigIndex+0] = data[c*4+0];
						bigdata[bigIndex+1] = data[c*4+1];
						bigdata[bigIndex+2] = data[c*4+2];
						bigdata[bigIndex+3] = data[c*4+3];
						c++;
					}
				}
			}
		}

		if(!uploadRGBAData(bigdata, canvasX, canvasY, canvasZ))
		{
			delete []data;
			return false;
		}

		setTextureSubCube(
			texCoordOfSample(m_MinX, width,  canvasX, m_MinX, m_MaxX),
			texCoordOfSample(m_MinY, height, canvasY, m_MinY, m_MaxY),
			texCoordOfSample(m_MinZ, depth,  canvasZ, m_MinZ, m_MaxZ),
			texCoordOfSample(m_MaxX, width,  canvasX, m_MinX, m_MaxX),
			texCoordOfSample(m_MaxY, height, canvasY, m_MinY, m_MaxY),
			texCoordOfSample(m_MaxZ, depth,  canvasZ, m_MinZ, m_MaxZ)
		);
		volumeData->setLoaded(true);
		delete []data;
		// Done loading
	}

	// Done loading
	return true;
}

float VolumeRendererExtn::getLargestSide()
{
	float len = 0;

	if(len < (m_MaxX-m_MinX))
	{
		len = (m_MaxX-m_MinX);
	}

	if(len < (m_MaxY-m_MinY))
	{
		len = (m_MaxY-m_MinY);
	}

	if(len < (m_MaxZ-m_MinZ))
	{
		len = (m_MaxZ-m_MinZ);
	}

	return len;
}

bool VolumeRendererExtn::renderVolume(double clipValue)
{
	VolumeRenderer::setNearPlane(clipValue);
	bool ret = false;
	float largestSide = getLargestSide();
	GLint matrixMode;
	glGetIntegerv(GL_MATRIX_MODE, &matrixMode);
	glMatrixMode(GL_MODELVIEW);
	float modelview_matrix[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix);
	glPushMatrix();
	// get it to right pos by translating and scaling
	// do it in right OpenGL order
	// 3. Shift by min vals
	glTranslatef(m_MinX, m_MinY, m_MinZ);
	// 2. Scale by largest dimension
	glScalef(largestSide, largestSide, largestSide);
	// 1. Shift by 0.5 in max, something smaller elsewhere to make it go from 0 - <=1
	glTranslatef((m_MaxX-m_MinX)/(largestSide * 2.0), (m_MaxY-m_MinY)/(largestSide * 2.0), (m_MaxZ-m_MinZ)/(largestSide * 2.0));
	ret = VolumeRenderer::renderVolume();
	glPopMatrix();
	glMatrixMode(matrixMode);
	return ret;
}

/*void NewVolumeMainWindow::updateRoverRenderable(RoverRenderable* roverRenderable, Extents* extents)
{
	double minX = extents->getXMin();
	double minY = extents->getYMin();
	double minZ = extents->getZMin();
	double maxX = extents->getXMax();
	double maxY = extents->getYMax();
	double maxZ = extents->getZMax();

	VolumeBuffer* densityBuffer = roverRenderable->getVolumeBufferManager()->getVolumeBuffer(getVarNum());

	unsigned int canvasX = upToPowerOfTwo(densityBuffer->getWidth());
	unsigned int canvasY = upToPowerOfTwo(densityBuffer->getHeight());
	unsigned int canvasZ = upToPowerOfTwo(densityBuffer->getDepth());

	// copy to uploadable buffer
	copyToUploadableBufferDensity(roverRenderable, extents, getVarNum());

	/ with border
	//viewer->uploadColorMappedDataWithBorder(m_MainBuffer, canvasWidthX, canvasWidthY, canvasWidthZ);
	/
	// without border
	//QTime t;
	//t.start();

	// upload to volume renderer
	roverRenderable->getVolumeRenderer()->uploadColorMappedData(m_UploadBuffer, canvasX, canvasY, canvasZ);
	//qDebug("Time to upload : %d", t.elapsed());

	//qDebug("Original Size: width: %d, height: %d, depth: %d", densityBuffer->getWidth(), densityBuffer->getHeight(), densityBuffer->getDepth());
	//qDebug("Uploading data: width: %d, height: %d, depth: %d", canvasX, canvasY, canvasZ);

	roverRenderable->setAspectRatio(fabs(maxX-minX), fabs(maxY-minY), fabs(maxZ-minZ));
	roverRenderable->getVolumeRenderer()->setTextureSubCube(
		texCoordOfSample(minX, densityBuffer->getWidth(), canvasX, densityBuffer->getMinX(), densityBuffer->getMaxX()),
		texCoordOfSample(minY, densityBuffer->getHeight(), canvasY, densityBuffer->getMinY(), densityBuffer->getMaxY()),
		texCoordOfSample(minZ, densityBuffer->getDepth(), canvasZ, densityBuffer->getMinZ(), densityBuffer->getMaxZ()),
		texCoordOfSample(maxX, densityBuffer->getWidth(), canvasX, densityBuffer->getMinX(), densityBuffer->getMaxX()),
		texCoordOfSample(maxY, densityBuffer->getHeight(), canvasY, densityBuffer->getMinY(), densityBuffer->getMaxY()),
		texCoordOfSample(maxZ, densityBuffer->getDepth(), canvasZ, densityBuffer->getMinZ(), densityBuffer->getMaxZ()));

	//qDebug("Done messing with volume viewer");

}*/

float VolumeRendererExtn::getExtents(double& minX, double& minY, double& minZ, double& maxX, double& maxY, double& maxZ)
{
	minX = m_MinX;
	minY = m_MinY;
	minZ = m_MinZ;
	maxX = m_MaxX;
	maxY = m_MaxY;
	maxZ = m_MaxZ;
	return 0.0f;
}
