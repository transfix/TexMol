#ifndef GBRERANKFORM_H
#define GBRERANKFORM_H
#include "gbrerank.Qt3.h"
#include "F2DockClient/rerankBasicParams.h"
#include "F2DockClient/F2DockClient.h"

class GBRerankForm : public GBRerank
{
		Q_OBJECT;
	private:
		RerankBasicParams* rbaspar;
		bool receptorPDBFileFound;
		bool ligandPDBFileFound;
		bool receptorQUADFileFound;
		bool ligandQUADFileFound;
		bool f2dockOutputFileFound;
		bool gbRerankInputFileFound;
		bool gbRerankOutputFileFound;
		int id;
		FILE* receptorPDBFP, *ligandPDBFP, *receptorQUADFP, *ligandQUADFP, *f2dOutFP, *gbInpFP, *gbOutFP;
		F2DockClient client;
		bool resultAvailable;
		bool jobSubmitted;

	public:
		GBRerankForm(QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WFlags fl = 0);
		~GBRerankForm();
		void setDefaults();
		bool checkValidity();
		void prepareParams();
		bool prepareInputFile();
		bool rerank();
		void closeFiles();
		void setParams(RerankBasicParams* rbaspar);

	public slots:
		void selectReceptorPQRFileSlot();
		void selectLigandPQRFileSlot();
		void selectReceptorQUADFileSlot();
		void selectLigandQUADFileSlot();
		void selectF2dockOutputFileSlot();
		void selectGbrerankOutputFileSlot();
		void resetSlot();
		void rerankSlot();
};

#endif
