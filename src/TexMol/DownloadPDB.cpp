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
#include <qfile.h>
#include <q3http.h>
#include <q3network.h>
#include <TexMol/DownloadPDB.h>

DownloadPDB::DownloadPDB()
{
}

DownloadPDB::~DownloadPDB()
{
	delete http;
	http = 0;
	delete http_file;
	http_file = 0;
}

void DownloadPDB::finishedop(bool error)
{
	if (error)
	{
	  printf("Download errer %s\n", http->errorString().ascii()); // arand, no warning
		delete http_file;
		http_file = 0;
		return;
	}
	delete http_file;
	http_file = 0;
}

bool DownloadPDB::blockedDownload(const char* pdbID, const char* filename)
{
	if (!pdbID || !filename)
	{
		return false;
	}
	http = new Q3Http();
	http_file = 0;
	connect(http, SIGNAL(done(bool)), this, SLOT(finishedop(bool)));
	QString qpdbIDtemp = QString(pdbID);
	QString qpdbIDtemp2 = qpdbIDtemp.upper();
	QString qpdbID = qpdbIDtemp2;
	if (qpdbIDtemp2.length() > 4 && qpdbIDtemp2.endsWith(".PDB"))
	{
		qpdbID = qpdbIDtemp2.remove(qpdbIDtemp2.length() - 4, 4);
	}
	q3InitNetworkProtocols();
	//http://www.rcsb.org/pdb/cgi/export.cgi/101M.pdb?format=PDB&pdbId=101M&compression=None
	QString pdbURL = "/pdb/cgi/export.cgi/" + QString(pdbID) + ".pdb?format=PDB&pdbId=" + QString(pdbID) + "&compression=None";
	http_file = new QFile(filename);
	if (!http_file->open(QIODevice::WriteOnly))
	{
		printf("Could not open file %s for writing\n", filename);
		delete http_file;
		http_file =0;
		return false;
	}
	http->setHost("www.rcsb.org");
	http->get(pdbURL, http_file);
	return true;
}
