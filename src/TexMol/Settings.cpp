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
#include <QMenu>
#include <TexMol/Settings.h>

Settings::Settings():m_MaxRecentFiles(10)
{
	initDefaults();
	loadSettings();
}

Settings::~Settings()
{
}

int Settings::getNumberOfRecentFiles() const
{
	return m_RecentFiles.count();
}

void Settings::pushRecentFile(QString filename, QMenu* recentFilesPopupMenu, QWidget* parent)
{
	if (m_RecentFiles.contains(filename))
	{
		return;
	}
	m_RecentFiles.push_back(filename);
	if ((int)(m_RecentFiles.count()) > m_MaxRecentFiles)
	{
		m_RecentFiles.pop_front();
	}
	loadPreviousFiles(recentFilesPopupMenu, parent);
}

bool Settings::loadSettings()
{
	for (int i = 0; i < m_MaxRecentFiles; ++i)
	{
		QString filename = value("RecentFiles/FileName" +
									 QString::number(i)).toString();
		if (!filename.isEmpty())
		{
			m_RecentFiles.push_back(filename);
		}
	}
	return true;
}

bool Settings::saveSettings()
{
	return true;
}

void Settings::initDefaults()
{
	m_NumberOfRecentFiles = 0;
	for (int i = 0; i < int(m_RecentFiles.count()); ++i)
	{
		m_RecentFiles[i] = "";
	}
	m_Dirty = false;
	m_UserSettingsChanged = false;
	// setPath removed in Qt6 — use QCoreApplication::setOrganizationName/setApplicationName instead
}

bool Settings::userSettingsChanged()
{
	return m_UserSettingsChanged;
}

void Settings::saveUserSettings()
{
}

void Settings::save()
{
	for (int i = 0; i < int(m_RecentFiles.count()); ++i)
		setValue("RecentFiles/FileName" + QString::number(i),
				   m_RecentFiles[i]);
}

void Settings::loadPreviousFiles(QMenu* recentFilesPopupMenu, QWidget* parent)
{
	if (!m_RecentFiles.count())
	{
		return;
	}
	recentFilesPopupMenu->clear();
	for (int i=0; i<(int)(m_RecentFiles.count()); i++)
	{
		recentFilesPopupMenu->addAction(m_RecentFiles[i]);
	}
}

QString Settings::getFileName(int previousFileIndexInMenu)
{
	if (previousFileIndexInMenu >= m_MaxRecentFiles)
	{
		return 0;
	}
	return m_RecentFiles[previousFileIndexInMenu];
}
