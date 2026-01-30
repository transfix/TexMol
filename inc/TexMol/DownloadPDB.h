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
#ifndef CCV_DOWNLOAD_PDB_H
#define CCV_DOWNLOAD_PDB_H

#include <qobject.h>

class Q3Http;
class QFile;

class DownloadPDB : public QObject
{
		Q_OBJECT
	public:
		DownloadPDB();
		virtual ~DownloadPDB();
		virtual bool blockedDownload(const char* pdbID, const char* filename);

	public slots:
		virtual void finishedop(bool error);

	protected:
		Q3Http* http;
		QFile* http_file;
};

#endif
