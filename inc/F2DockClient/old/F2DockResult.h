#ifndef F2DOCKRESULTFORM_H
#define F2DOCKRESULTFORM_H



#include <qthread.h>
//Added by qt3to4:
#include <QCustomEvent>

#include "F2DockResultDetailsForm.h"

#include "f2dockresultQt3.h"

#include "basicParams.h"
#include "summaryResults.h"
#include "detailResults.h"
#include <MolEnergyUtil/utils.h>

#include <vector>


class F2DockResult : public f2DockResult
{
		Q_OBJECT
	private:
		QWidget* my_parent;
		BasicParams* baspar;
		SummaryResults sumres;
		std::vector<DetailResults*> detres;

		class DetailResultThread : public QThread
		{
			public:
				DetailResultThread(F2DockResult* parent, unsigned int stackSize = 0);
				virtual void run();
				void setForm(F2DockResultDetails* fdm);
			private:
				F2DockResult* parent;
				F2DockResultDetails* fdm;
		} *fdt;

		DetailResultThread* detresThread;
		F2DockResultDetails* detresDialog;
		void updateValues();
		bool getValues();

	public:
		F2DockResult(QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WFlags fl = 0);
		~F2DockResult();
		void setParams(BasicParams* bp);

	public slots:
		void detailsSlot();
		void doneSlot();
		void customEvent(QCustomEvent* ev);
};

#endif
