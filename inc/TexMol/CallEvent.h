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
// CallEvent
// Purpose:
//   This event is meant to be used to perform calls on various QObjects
//   in a way that doesn't require compile time knowledge of what methods are
//   supported on that object.  Basically it is a carrier for call information:
//   The name of the function and arguments supplied to it.  One caveat, it
//   doesn't directly support return values.

#ifndef __TEXMOL__CALLEVENT_H__
#define __TEXMOL__CALLEVENT_H__

#include <qapplication.h>
#include <qevent.h>
#include <qwidget.h>
//Added by qt3to4:
#include <QCustomEvent>
#include <string>
#include <vector>
#include <boost/any.hpp>

class CallEvent : public QCustomEvent
{
	public:
		static const int TypeId = QEvent::User+256;
		CallEvent(const std::string& funcName, const std::vector<boost::any>& args)
			: QCustomEvent(TypeId), _funcName(funcName), _args(args) {}
		const std::string& funcName() const
		{
			return _funcName;
		}
		const std::vector<boost::any>& args() const
		{
			return _args;
		}
		static void call(const std::string& widgetName,
						 const std::string& funcName,
						 const std::vector<boost::any>& args = std::vector<boost::any>())
		{
		/*Q3Err:I
			QWidgetList*  list = QApplication::allWidgets();
			QWidgetListIt it(*list);           // iterate over the widgets // this is only in qt3
			QWidget* w;
			while ((w=it.current()) != 0)      // for each widget...
			{
				++it;
				if (std::string(w->name()) == widgetName)
				{
					call(w,funcName,args);
				}
			}
			delete list;                      // delete the list, not the widgets

		*/
			// this might work...maybe,,, might,,,
			QWidgetList list = QApplication::allWidgets();	//by cha
			QWidget* w;
			while ((w=list.back()) != 0)   // for each widget...
			{
				list.pop_back();
				if (std::string(w->name()) == widgetName)
				{
					call(w,funcName,args);
				}
			}
			list.clear();                      // delete the list, not the widgets
		}

		static void call(QObject* o,
						 const std::string& funcName,
						 const std::vector<boost::any>& args = std::vector<boost::any>())
		{
			QApplication::postEvent(o,new CallEvent(funcName,args));
		}

	private:
		std::string _funcName;
		std::vector<boost::any> _args;
};

#endif
