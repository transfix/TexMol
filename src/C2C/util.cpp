/*
  Copyright 2011 The University of Texas at Austin

	Advisor: Chandrajit Bajaj <bajaj@cs.utexas.edu>

  This file is part of MolSurf.

  MolSurf is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License version 2.1 as published by the Free Software Foundation.


  MolSurf is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include <C2C/util.h>

// Write contents of a BitBuffer to a DiskIO
void writeBitBuffer(BitBuffer* buf, DiskIO* io)
{
	int nbyte = buf->curByte();
	char nbit = (char)buf->curBit();
	unsigned char* bits = buf->getBits();
	int n = buf->getNumBytes();
	io->put(&nbyte, 1);
	io->put(&nbit, 1);
	io->put(bits, n);
}

// Write contents of a BitBuffer to a stream,
// which can be a file or a memory buffer.
void writeBitBuffer(BitBuffer* buf, ByteStream* stream)
{
	int nbyte = buf->curByte();
	char nbit = (char)buf->curBit();
	unsigned char* bits = buf->getBits();
	int n = buf->getNumBytes();
	stream->write32(nbyte);
	stream->write8(nbit);
	stream->write(bits, n);
}

// Read contents of a BitBuffer from a DiskIO
BitBuffer* readBitBuffer(DiskIO* io)
{
	int nbyte;
	unsigned char nbit;
	io->get(&nbyte, 1);
	io->get(&nbit, 1);
	int n = nbyte+((nbit > 0)? 1:0);
	char* bits = (char*)malloc(n);
	memset(bits, 0, n);
	io->get(bits, n);
	BitBuffer* bitbuf = new BitBuffer(bits, nbyte, nbit);
	free(bits);
	return bitbuf;
}

// Read contents of a BitBuffer from a stream,
// which can be a file or a memory buffer.
BitBuffer* readBitBuffer(ByteStream* stream)
{
	int nbyte;
	unsigned char nbit;
	nbyte = stream->read32();
	nbit = stream->read8();
	int n = nbyte+((nbit > 0)? 1:0);
	char* bits = (char*)malloc(n);
	stream->read(bits, n);
	BitBuffer* bitbuf = new BitBuffer(bits, nbyte, nbit);
	free(bits);
	return bitbuf;
}
