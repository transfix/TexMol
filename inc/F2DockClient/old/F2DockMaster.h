#ifndef F2DOCKMASTERFORM_H
#define F2DOCKMASTERFORM_H

#include "f2dockmasterQt3.h"
#include <vector>
#include "jobParams.h"
#include "F2DockClient.h"
#include <F2DockClient/F2DockResult.h>
#include <F2DockClient/GBRerankResult.h>
#include <qthread.h>
//Added by qt3to4:
#include <QCustomEvent>

using namespace std;

class F2DockMaster : public f2dockmaster
{
		Q_OBJECT
	private:
		QWidget* my_parent;
		vector <JobParams*> jobs;
		int selectedJob;
		F2DockClient client;
		void updateTable();
		void startNag();

		class Nagger : public QThread
		{
			public:
				Nagger(F2DockMaster* parent, unsigned int stackSize = 0);
				virtual void run();
				void setId(string str, int a, int d);
			private:
				F2DockMaster* parent;
				string jobid;
				int nagId;
				int delay;
				F2DockClient client;
		} *nag;
		vector<Nagger*> naggers;

		class F2DockResultThread : public QThread
		{
			public:
				F2DockResultThread(F2DockMaster* parent, unsigned int stackSize = 0);
				virtual void run();
				void setF2DockResForm(F2DockResult* fdm);
			private:
				F2DockMaster* parent;
				F2DockResult* fdm;
		} *f2drest;

		F2DockResultThread* f2dresThread;
		F2DockResult* f2dresDialog;


		class GBRerankResultThread : public QThread
		{
			public:
				GBRerankResultThread(F2DockMaster* parent, unsigned int stackSize = 0);
				virtual void run();
				void setGBRerankResForm(GBRerankResult* fdm);
			private:
				F2DockMaster* parent;
				GBRerankResult* fdm;
		} *gbrest;

		GBRerankResultThread* gbresThread;
		GBRerankResult* gbresDialog;

	public:
		F2DockMaster(QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WFlags fl = 0);
		~F2DockMaster();

	public slots:
		void dockSlot();
		void f2dgenSlot();
		void rerankSlot();
		void quadgenSlot();
		void jobdetSlot();
		void resultSlot();
		void saveSlot();
		void loadSlot();
		void selectSlot();
		void customEvent(QEvent* ev);

};

#endif
