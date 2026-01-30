#ifndef F2DOCKTHREAD_H
#define F2DOCKTHREAD_H

#include <qthread.h>
#include "F2DockClient/F2DockForm.h"

class F2DockThread : public QThread
{
		F2DockForm* dialog;
	public:
		void setDialog(F2DockForm* d);
		virtual void run();
};

#endif
