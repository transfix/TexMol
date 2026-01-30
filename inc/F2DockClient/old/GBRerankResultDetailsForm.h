#ifndef GBRERANKRESULTDETAILSFORM_H
#define GBRERANKRESULTDETAILSFORM_H

#include "gbrerankresultdetails.Qt3.h"
#include "rerankDetailResults.h"
#include "rerankBasicParams.h"
#include <vector>

class GBRerankResultDetails : public gbrerankresultdetails
{
		Q_OBJECT
	private:
		std::vector<RerankDetailResults*> *detres;
		RerankBasicParams* rbaspar;
		void setValues();
		int numCol;
		int numRow;
		bool transform_pdb(double* trans, FILE* ifp, char* ofname);

	public:
		GBRerankResultDetails(QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WFlags fl = 0);
		~GBRerankResultDetails();
		void setParams(std::vector<RerankDetailResults*> *dr,RerankBasicParams* rbaspar);

	public slots:
		void doneSlot();
		void viewSlot();
};

#endif
