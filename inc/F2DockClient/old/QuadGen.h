#ifndef QUADGENFORM_H
#define QUADGENFORM_H

#include "quadgenQt3.h"
#include "quadgenParams.h"
#include "F2DockClient.h"

class QuadGen : public quadGen
{
		Q_OBJECT;
	private:
		QuadParams* quadpar;
		bool receptorFileFound;
		bool ligandFileFound;
		int id;
		FILE* receptorFP, *ligandFP, *receptorOutFP, *ligandOutFP;
		F2DockClient client;
		bool resultAvailable;
		bool jobSubmitted;
		bool paramsFound;

	public:
		QuadGen(QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WFlags fl = 0);
		~QuadGen();
		void prepareParams();
		bool submitJob();
		void setParams(QuadParams* quadpar);

	public slots:
		void rfButtonSlot();
		void lfButtonSlot();
		void genButtonSlot();
		void fineSlot();
		void coarseSlot();
};

#endif
