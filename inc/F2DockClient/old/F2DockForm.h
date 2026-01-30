#ifndef F2DOCKFORM_H
#define F2DOCKFORM_H

#include "f2dock.Qt3.h"
#include "F2DockClient/basicParams.h"
#include "F2DockClient/advParams.h"
#include "F2DockClient/F2DockClient.h"

class F2DockForm : public F2Dock
{
		Q_OBJECT;
	private:
		QWidget* my_parent;
		BasicParams* baspar;
		AdvancedParams advpar;
		bool receptorFileFound;
		bool receptorPDBFound;
		bool ligandFileFound;
		bool rmsdFileFound;
		bool outputFileFound;
		bool inputFileFound;
		bool advParamsSet;
		bool bound;
		int id;
		FILE* receptorFP, *receptorPDBP, *ligandFP, *rmsdFP, *outFP, *inpFP;
		F2DockClient client;
		bool resultAvailable;
		bool jobSubmitted;

	public:
		F2DockForm(QWidget* parent = 0, const char* name = 0, bool modal = TRUE, Qt::WFlags fl = 0);
		~F2DockForm();
		void setDefaults();
		bool checkValidity();
		void prepareParams();
		bool prepareInputFile();
		bool dock();
		void closeFiles();
		void setParams(BasicParams* baspar);

	public slots:
		void selectReceptorFileSlot();
		void selectReceptorPDBSlot();
		void selectLigandFileSlot();
		void selectRMSDFileSlot();
		void selectOutputFileSlot();
		void advencedOptionsSlot();
		void dockSlot();
		void defaultSlot();
		void uuSlot();
		void bbSlot();
};

#endif
