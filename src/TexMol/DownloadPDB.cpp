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
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QUrl>
#include <TexMol/DownloadPDB.h>

DownloadPDB::DownloadPDB()
{
	m_manager = new QNetworkAccessManager(this);
}

DownloadPDB::~DownloadPDB()
{
	delete http_file;
	http_file = nullptr;
	// m_manager is parented to this, auto-deleted
}

void DownloadPDB::finishedop(bool error)
{
	if (error)
	{
		fprintf(stderr, "Download error\n");
		delete http_file;
		http_file = nullptr;
		return;
	}
	delete http_file;
	http_file = nullptr;
}

bool DownloadPDB::blockedDownload(const char* pdbID, const char* filename)
{
	if (!pdbID || !filename)
	{
		return false;
	}

	QString qpdbIDtemp = QString(pdbID);
	QString qpdbID = qpdbIDtemp.toUpper();
	if (qpdbID.length() > 4 && qpdbID.endsWith(".PDB"))
	{
		qpdbID = qpdbID.left(qpdbID.length() - 4);
	}

	// RCSB PDB REST API
	QUrl url("https://files.rcsb.org/download/" + qpdbID + ".pdb");
	QNetworkRequest request(url);

	QNetworkReply* reply = m_manager->get(request);

	// Block until download completes
	QEventLoop loop;
	connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
	loop.exec();

	if (reply->error() != QNetworkReply::NoError)
	{
		fprintf(stderr, "Download error: %s\n", reply->errorString().toLatin1().constData());
		reply->deleteLater();
		return false;
	}

	QFile file(filename);
	if (!file.open(QIODevice::WriteOnly))
	{
		fprintf(stderr, "Could not open file %s for writing\n", filename);
		reply->deleteLater();
		return false;
	}

	file.write(reply->readAll());
	file.close();
	reply->deleteLater();
	return true;
}
