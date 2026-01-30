#ifndef F2DOCKADVFORM_H
#define F2DOCKADVFORM_H

#include "f2dockadv.Qt3.h"
#include "advParams.h"

class F2DockAdvForm : public F2DockAdv
{
		Q_OBJECT
	private:
		AdvancedParams* advpar;
		bool bound;
	public:
		F2DockAdvForm(QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WFlags fl = 0);
		~F2DockAdvForm();
		void newFunction();
		void setAdvParams(AdvancedParams* adv);
		void setBound(bool b);
		void prepareParams();
		bool checkValidity();
		void resetToDefault();
	public slots:
		void saveSlot();
		void defaultSlot();
};

#endif
