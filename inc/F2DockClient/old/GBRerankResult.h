#ifndef GBRERANKRESULTFORM_H
#define GBRERANKRESULTFORM_H

#include "gbrerankresultQt3.h"
#include "rerankBasicParams.h"
#include "summaryResults.h"
#include "rerankDetailResults.h"
#include "GBRerankResultDetailsForm.h"
#include <MolEnergyUtil/utils.h>
#include <vector>
#include <qthread.h>

class GBRerankResult : public gbRerankResult
{
		Q_OBJECT
	private:
		RerankBasicParams* rbaspar;
		SummaryResults sumres;
		std::vector<RerankDetailResults*> detres;

		class GBDetailResultThread : public QThread
		{
			public:
				GBDetailResultThread(GBRerankResult* parent, unsigned int stackSize = 0);
				virtual void run();
				void setForm(GBRerankResultDetails* fdm);
			private:
				GBRerankResult* parent;
				GBRerankResultDetails* fdm;
		} *fdt;

		GBDetailResultThread* detresThread;
		GBRerankResultDetails* detresDialog;
		void updateValues();
		bool getValues();

	public:
		GBRerankResult(QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WFlags fl = 0);
		~GBRerankResult();
		void setParams(RerankBasicParams* rbp);

	public slots:
		void detailedsSlot();
		void doneSlot();
};

#endif
