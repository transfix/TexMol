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
//The defaults are:
//	left:			view   translate
//	middle:			view   zoom
//	right:			view   rotate
//	left  +shift:		object translate
//	middle+shift:		object zoom
//	right +shift:		object rotate
//      left  +control:		object select

#include <qcombobox.h>
#include <qmessagebox.h>
#include <q3popupmenu.h>
//Added by qt3to4:
#include <QMouseEvent>
#include <TexMol/Dialogs/MouseHandlerDialog.h>
#include <TexMol/MouseHandlerHelper.h>

MouseHandler::MouseHandler(QWidget* parentWidget)
{
	m_ParentWidget = parentWidget;
	setDefaults();
}

MouseHandler::~MouseHandler()
{
}

void MouseHandler::setDefaults()
{
	m_ViewTranslateMouseButton = LEFT_BUTTON;
	m_ViewTranslateKey = NO_KEY;
	m_ViewZoomMouseButton = MIDDLE_BUTTON;
	m_ViewZoomKey = NO_KEY;
	m_ViewRotateMouseButton = RIGHT_BUTTON;
	m_ViewRotateKey = NO_KEY;
	m_ObjectTranslateMouseButton = LEFT_BUTTON;
	m_ObjectTranslateKey = SHIFT_KEY;
	m_ObjectZoomMouseButton = MIDDLE_BUTTON;
	m_ObjectZoomKey = SHIFT_KEY;
	m_ObjectRotateMouseButton = RIGHT_BUTTON;
	m_ObjectRotateKey = SHIFT_KEY;
	m_ObjectSelectMouseButton = LEFT_BUTTON;
	m_ObjectSelectKey = CONTROL_KEY;
}

MouseHandler::USER_SELECTED_TRANSFORMATION MouseHandler::getUserSelectedTransformation(QMouseEvent* qMouseEvent, bool controlPressed, bool shiftPressed, bool controlAndShiftPressed)
{
	MOUSE_BUTTONS mButton = NO_BUTTON;
	if (qMouseEvent->state() & Qt::LeftButton)
	{
		mButton = LEFT_BUTTON;
	}
	if (qMouseEvent->state() & Qt::MidButton)
	{
		mButton = MIDDLE_BUTTON;
	}
	if (qMouseEvent->state() & Qt::RightButton)
	{
		mButton = RIGHT_BUTTON;
	}
	KEYS key = NO_KEY;
	if (controlAndShiftPressed)
	{
		key = SHIFT_AND_CONTROL_KEYS;
	}
	else if (shiftPressed)
	{
		key = SHIFT_KEY;
	}
	else if (controlPressed)
	{
		key = CONTROL_KEY;
	}
	if (mButton == NO_BUTTON)
	{
		return NO_TRANSFORM;
	}
	// try all possibilities
	if (mButton == m_ViewTranslateMouseButton && key == m_ViewTranslateKey)
	{
		return VIEW_TRANSLATE;
	}
	if (mButton == m_ViewZoomMouseButton && key == m_ViewZoomKey)
	{
		return VIEW_ZOOM;
	}
	if (mButton == m_ViewRotateMouseButton && key == m_ViewRotateKey)
	{
		return VIEW_ROTATE;
	}
	if (mButton == m_ObjectTranslateMouseButton && key == m_ObjectTranslateKey)
	{
		return OBJECT_TRANSLATE;
	}
	if (mButton == m_ObjectZoomMouseButton && key == m_ObjectZoomKey)
	{
		return OBJECT_ZOOM;
	}
	if (mButton == m_ObjectRotateMouseButton && key == m_ObjectRotateKey)
	{
		return OBJECT_ROTATE;
	}
	if (mButton == m_ObjectSelectMouseButton && key == m_ObjectSelectKey)
	{
		return OBJECT_SELECT;
	}
	return NO_TRANSFORM;
}

void MouseHandler::addMouseComboBoxItems(QComboBox* qcombobox)
{
	qcombobox->insertItem("  --  ", 0);
	qcombobox->insertItem("Left", 1);
	qcombobox->insertItem("Middle", 2);
	qcombobox->insertItem("Right", 3);
}

void MouseHandler::addKeyComboBoxItems(QComboBox* qcombobox)
{
	qcombobox->insertItem("  --  ", 0);
	qcombobox->insertItem("Shift", 1);
	qcombobox->insertItem("Control", 2);
	qcombobox->insertItem("Shift+Control", 3);
}

void MouseHandler::setCurrentPreferenecs(MouseHandlerDialog* mouseHandlerDialog)
{
	mouseHandlerDialog->m_Button_View_Translate_ComboBox->setCurrentItem(m_ViewTranslateMouseButton);
	mouseHandlerDialog->m_Button_View_Zoom_ComboBox->setCurrentItem(m_ViewZoomMouseButton);
	mouseHandlerDialog->m_Button_View_Rotate_ComboBox->setCurrentItem(m_ViewRotateMouseButton);
	mouseHandlerDialog->m_Key_View_Translate_ComboBox->setCurrentItem(m_ViewTranslateKey);
	mouseHandlerDialog->m_Key_View_Zoom_ComboBox->setCurrentItem(m_ViewZoomKey);
	mouseHandlerDialog->m_Key_View_Rotate_ComboBox->setCurrentItem(m_ViewRotateKey);
	mouseHandlerDialog->m_Button_Object_Translate_ComboBox->setCurrentItem(m_ObjectTranslateMouseButton);
	mouseHandlerDialog->m_Button_Object_Zoom_ComboBox->setCurrentItem(m_ObjectZoomMouseButton);
	mouseHandlerDialog->m_Button_Object_Rotate_ComboBox->setCurrentItem(m_ObjectRotateMouseButton);
	mouseHandlerDialog->m_Key_Object_Translate_ComboBox->setCurrentItem(m_ObjectTranslateKey);
	mouseHandlerDialog->m_Key_Object_Zoom_ComboBox->setCurrentItem(m_ObjectZoomKey);
	mouseHandlerDialog->m_Key_Object_Rotate_ComboBox->setCurrentItem(m_ObjectRotateKey);
	mouseHandlerDialog->m_Button_Object_Select_ComboBox->setCurrentItem(m_ObjectSelectMouseButton);
	mouseHandlerDialog->m_Key_Object_Select_ComboBox->setCurrentItem(m_ObjectSelectKey);
}

void MouseHandler::initDialog(MouseHandlerDialog* mouseHandlerDialog)
{
	addMouseComboBoxItems(mouseHandlerDialog->m_Button_View_Translate_ComboBox);
	addMouseComboBoxItems(mouseHandlerDialog->m_Button_View_Zoom_ComboBox);
	addMouseComboBoxItems(mouseHandlerDialog->m_Button_View_Rotate_ComboBox);
	addMouseComboBoxItems(mouseHandlerDialog->m_Button_Object_Translate_ComboBox);
	addMouseComboBoxItems(mouseHandlerDialog->m_Button_Object_Zoom_ComboBox);
	addMouseComboBoxItems(mouseHandlerDialog->m_Button_Object_Rotate_ComboBox);
	addMouseComboBoxItems(mouseHandlerDialog->m_Button_Object_Select_ComboBox);
	addKeyComboBoxItems(mouseHandlerDialog->m_Key_View_Translate_ComboBox);
	addKeyComboBoxItems(mouseHandlerDialog->m_Key_View_Zoom_ComboBox);
	addKeyComboBoxItems(mouseHandlerDialog->m_Key_View_Rotate_ComboBox);
	addKeyComboBoxItems(mouseHandlerDialog->m_Key_Object_Translate_ComboBox);
	addKeyComboBoxItems(mouseHandlerDialog->m_Key_Object_Zoom_ComboBox);
	addKeyComboBoxItems(mouseHandlerDialog->m_Key_Object_Rotate_ComboBox);
	addKeyComboBoxItems(mouseHandlerDialog->m_Key_Object_Select_ComboBox);
	setCurrentPreferenecs(mouseHandlerDialog);
}

bool MouseHandler::repeatingCombination(MouseHandlerDialog* mouseHandlerDialog)
{
	return false;
}

bool MouseHandler::onlyKeySelected(MouseHandlerDialog* mouseHandlerDialog)
{
	if (mouseHandlerDialog->m_Button_View_Translate_ComboBox->currentItem() == NO_BUTTON &&
			mouseHandlerDialog->m_Key_View_Translate_ComboBox->currentItem() != NO_KEY)
	{
		return true;
	}
	if (mouseHandlerDialog->m_Button_View_Zoom_ComboBox->currentItem() == NO_BUTTON &&
			mouseHandlerDialog->m_Key_View_Zoom_ComboBox->currentItem() != NO_KEY)
	{
		return true;
	}
	if (mouseHandlerDialog->m_Button_View_Rotate_ComboBox->currentItem() == NO_BUTTON &&
			mouseHandlerDialog->m_Key_View_Rotate_ComboBox->currentItem() != NO_KEY)
	{
		return true;
	}
	if (mouseHandlerDialog->m_Button_Object_Translate_ComboBox->currentItem() == NO_BUTTON &&
			mouseHandlerDialog->m_Key_Object_Translate_ComboBox->currentItem() != NO_KEY)
	{
		return true;
	}
	if (mouseHandlerDialog->m_Button_Object_Zoom_ComboBox->currentItem() == NO_BUTTON &&
			mouseHandlerDialog->m_Key_Object_Zoom_ComboBox->currentItem() != NO_KEY)
	{
		return true;
	}
	if (mouseHandlerDialog->m_Button_Object_Rotate_ComboBox->currentItem() == NO_BUTTON &&
			mouseHandlerDialog->m_Key_Object_Rotate_ComboBox->currentItem() != NO_KEY)
	{
		return true;
	}
	if (mouseHandlerDialog->m_Button_Object_Select_ComboBox->currentItem() == NO_BUTTON &&
			mouseHandlerDialog->m_Key_Object_Select_ComboBox->currentItem() != NO_KEY)
	{
		return true;
	}
	return false;
}

void MouseHandler::updateUserPreferences(MouseHandlerDialog* mouseHandlerDialog)
{
	m_ViewTranslateMouseButton = mouseHandlerDialog->m_Button_View_Translate_ComboBox->currentItem();
	m_ViewZoomMouseButton = mouseHandlerDialog->m_Button_View_Zoom_ComboBox->currentItem();
	m_ViewRotateMouseButton = mouseHandlerDialog->m_Button_View_Rotate_ComboBox->currentItem();
	m_ViewTranslateKey = mouseHandlerDialog->m_Key_View_Translate_ComboBox->currentItem();
	m_ViewZoomKey = mouseHandlerDialog->m_Key_View_Zoom_ComboBox->currentItem();
	m_ViewRotateKey = mouseHandlerDialog->m_Key_View_Rotate_ComboBox->currentItem();
	m_ObjectTranslateMouseButton = mouseHandlerDialog->m_Button_Object_Translate_ComboBox->currentItem();
	m_ObjectZoomMouseButton = mouseHandlerDialog->m_Button_Object_Zoom_ComboBox->currentItem();
	m_ObjectRotateMouseButton = mouseHandlerDialog->m_Button_Object_Rotate_ComboBox->currentItem();
	m_ObjectTranslateKey = mouseHandlerDialog->m_Key_Object_Translate_ComboBox->currentItem();
	m_ObjectZoomKey = mouseHandlerDialog->m_Key_Object_Zoom_ComboBox->currentItem();
	m_ObjectRotateKey = mouseHandlerDialog->m_Key_Object_Rotate_ComboBox->currentItem();
	m_ObjectSelectMouseButton = mouseHandlerDialog->m_Button_Object_Select_ComboBox->currentItem();
	m_ObjectSelectKey = mouseHandlerDialog->m_Key_Object_Select_ComboBox->currentItem();
}

void MouseHandler::updateUserPreferences()
{
	MouseHandlerDialog* mouseHandlerDialog = new MouseHandlerDialog(m_ParentWidget);
	initDialog(mouseHandlerDialog);
	while (mouseHandlerDialog->exec() == QDialog::Accepted)
	{
		// check to make sure
		// 1. that only a key is not selected
		// 2. The same mouse key combo is not repeated
		if (onlyKeySelected(mouseHandlerDialog))
		{
			QMessageBox::critical(mouseHandlerDialog, "Error, re-enter values", "If you select a key, you have select a mouse button");
		}
		else if (repeatingCombination(mouseHandlerDialog))
		{
			QMessageBox::critical(mouseHandlerDialog, "Error, re-enter values", "There are repeating mouse+key combinations");
		}
		else
		{
			updateUserPreferences(mouseHandlerDialog);
			break;
		}
	}
	delete mouseHandlerDialog;
	mouseHandlerDialog = 0;
}
