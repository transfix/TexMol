#ifndef F2DGENIMP_H
#define F2DGENIMP_H

#include "f2dgenQt3.h"
#include "f2dgenParams.h"
#include "F2DockClient.h"

class F2dGen : public f2dGen
{
		Q_OBJECT;
	private:
		F2dParams* f2dpar;
		bool receptorFileFound;
		bool ligandFileFound;
		int id;
		FILE* receptorFP, *ligandFP, *receptorOutFP, *ligandOutFP;
		F2DockClient client;
		bool resultAvailable;
		bool jobSubmitted;
		bool paramsFound;

	public:
		F2dGen(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = {});
		~F2dGen();
		void prepareParams();
		bool submitJob();
		void setParams(F2dParams* f2dpar);

	public slots:
		void rfButtonSlot();
		void lfButtonSlot();
		void genButtonSlot();
};

#endif
