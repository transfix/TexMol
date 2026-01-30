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
#ifndef CCV_TEXMOL_MOUSE_HANDLER_H
#define CCV_TEXMOL_MOUSE_HANDLER_H

#include <qevent.h>
//Added by qt3to4:
#include <QMouseEvent>

class MouseHandlerDialog;
class QComboBox;

class MouseHandler
{
	public:
		MouseHandler(QWidget* parentWidget);
		virtual ~MouseHandler();
		void updateUserPreferences();
		enum USER_SELECTED_TRANSFORMATION {NO_TRANSFORM, VIEW_TRANSLATE, VIEW_ZOOM, VIEW_ROTATE, OBJECT_TRANSLATE, OBJECT_ZOOM , OBJECT_ROTATE , OBJECT_SELECT};
		USER_SELECTED_TRANSFORMATION getUserSelectedTransformation(QMouseEvent* qMouseEvent, bool controlPressed, bool shiftPressed, bool controlAndShiftPressed);
		enum MOUSE_BUTTONS {NO_BUTTON, LEFT_BUTTON, MIDDLE_BUTTON, RIGHT_BUTTON};
		enum KEYS          {NO_KEY, SHIFT_KEY, CONTROL_KEY, SHIFT_AND_CONTROL_KEYS};
		void setDefaults();

	protected:
		void setCurrentPreferenecs(MouseHandlerDialog* mouseHandlerDialog);
		void addMouseComboBoxItems(QComboBox* qcombobox);
		void addKeyComboBoxItems(QComboBox* qcombobox);
		bool onlyKeySelected(MouseHandlerDialog* mouseHandlerDialog);
		bool repeatingCombination(MouseHandlerDialog* mouseHandlerDialog);
		void updateUserPreferences(MouseHandlerDialog* mouseHandlerDialog);
		void initDialog(MouseHandlerDialog* mouseHandlerDialog);
		QWidget* m_ParentWidget;
		int m_ViewTranslateMouseButton;
		int m_ViewTranslateKey;
		int m_ViewZoomMouseButton;
		int m_ViewZoomKey;
		int m_ViewRotateMouseButton;
		int m_ViewRotateKey;
		int m_ObjectTranslateMouseButton;
		int m_ObjectTranslateKey;
		int m_ObjectZoomMouseButton;
		int m_ObjectZoomKey;
		int m_ObjectRotateMouseButton;
		int m_ObjectRotateKey;
		int m_ObjectSelectMouseButton;
		int m_ObjectSelectKey;
};

#endif
