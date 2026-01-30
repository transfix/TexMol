#ifndef F2DOCKRESULTDETAILSFORM_H
#define F2DOCKRESULTDETAILSFORM_H

#include "f2dockresultdetails.Qt3.h"
#include "detailResults.h"
#include "basicParams.h"
#include <vector>

class F2DockResultDetails : public f2dockresultdetails
{
		Q_OBJECT
	private:
		QWidget* my_parent;
		std::vector<DetailResults*> *detres;
		BasicParams* baspar;
		void setValues();
		int numCol;
		int numRow;
		bool transform_pdb(double* trans, FILE* ifp, char* ofname);
		bool non_transform_pdb(FILE* ifp, char* ofname);
		char tempRecFileName[250];
		char tempLigFileName[250];
		bool pdbSent;
		bool displaySurface;

	public:
		F2DockResultDetails(QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WFlags fl = 0);
		~F2DockResultDetails();
		void setParams(std::vector<DetailResults*> *dr, BasicParams* baspar);

	public slots:
		void doneSlot();
		void viewSlot();
		void selectRowSlot();
		void surfaceSlot(bool surface);
};

#endif
