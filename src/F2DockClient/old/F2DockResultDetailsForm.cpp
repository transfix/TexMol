#include <F2DockClient/F2DockResultDetailsForm.h>
#include <iostream>
#include <qapplication.h>
#include <qmessagebox.h>
#include <q3table.h>
//Added by qt3to4:
#include <QCustomEvent>
#include <QEvent>

using namespace std;

class ViewRowEventResultDetails : public QCustomEvent
{
	public:
		ViewRowEventResultDetails(const QString& m, double* t, const bool s) : QCustomEvent(QEvent::User+102), msg(m), mat(t), surf(s) {}
		QString message() const
		{
			return msg;
		}
		double* matrix()
		{
			return mat;
		}
		bool surface() const
		{
			return surf;
		}
	private:
		QString msg;
		double* mat;
		bool surf;
};


// Constructs a F2DockResultDetails which is a child of 'parent', with the name 'name' and widget flags set to 'f'
// The dialog will by default be modeless, unless you set 'modal' to true to construct a modal dialog.
F2DockResultDetails::F2DockResultDetails(QWidget* parent,  const char* name, bool modal, Qt::WindowFlags fl)
	: f2dockresultdetails(parent, name, modal, fl), my_parent(parent)
{
	cout<<"Init detres"<<endl;
	numCol = 24;
	QStringList labels;
	labels.append("Rank");
	labels.append("Total Score");
	labels.append("Sh. Comp. Sc.");
	labels.append("Elec. Sc.");
	labels.append("Hbond Sc.");
	labels.append("VDW Sc.");
	labels.append("RMSD");
	labels.append("# of Clashes");
	labels.append("Skin-Skin Sc.");
	labels.append("Core-Core Sc.");
	labels.append("Skin-Core Sc.");
	labels.append("matrix[0,0]");
	labels.append("matrix[0,1]");
	labels.append("matrix[0,2]");
	labels.append("matrix[0,3]");
	labels.append("matrix[1,0]");
	labels.append("matrix[1,1]");
	labels.append("matrix[1,2]");
	labels.append("matrix[1,3]");
	labels.append("matrix[2,0]");
	labels.append("matrix[2,1]");
	labels.append("matrix[2,2]");
	labels.append("matrix[2,3]");
	labels.append("Conf");
	cout<<"1"<<endl;
	//	this->view_Button->setFlat(true);
	f2dResultsTable->setNumCols(24);
	f2dResultsTable->setColumnLabels(labels);
	f2dResultsTable->setColumnWidth(0, 50);
	f2dResultsTable->setColumnWidth(1, 100);
	f2dResultsTable->setColumnWidth(2, 100);
	f2dResultsTable->setColumnWidth(3, 100);
	f2dResultsTable->setColumnWidth(4, 100);
	f2dResultsTable->setColumnWidth(5, 100);
	f2dResultsTable->setColumnWidth(6, 100);
	f2dResultsTable->setColumnWidth(7, 100);
	f2dResultsTable->setColumnWidth(8, 100);
	f2dResultsTable->setColumnWidth(9, 100);
	f2dResultsTable->setColumnWidth(10, 100);
	f2dResultsTable->setColumnWidth(11, 75);
	f2dResultsTable->setColumnWidth(12, 75);
	f2dResultsTable->setColumnWidth(13, 75);
	f2dResultsTable->setColumnWidth(14, 75);
	f2dResultsTable->setColumnWidth(15, 75);
	f2dResultsTable->setColumnWidth(16, 75);
	f2dResultsTable->setColumnWidth(17, 75);
	f2dResultsTable->setColumnWidth(18, 75);
	f2dResultsTable->setColumnWidth(19, 75);
	f2dResultsTable->setColumnWidth(20, 75);
	f2dResultsTable->setColumnWidth(21, 75);
	f2dResultsTable->setColumnWidth(22, 75);
	f2dResultsTable->setColumnWidth(23, 75);
	cout<<"1"<<endl;

	pdbSent = false;
	displaySurface = false;
}

F2DockResultDetails::~F2DockResultDetails()
{
	// no need to delete child widgets, Qt does it all for us
}



// Public slot
void F2DockResultDetails::doneSlot()
{
	this->close();
	if(remove(tempRecFileName))
		printf("Failed to delete temporary receptor pdb file: %s\n", tempRecFileName);
	if(remove(tempLigFileName))
		printf("Failed to delete temporary ligand pdb file: %s\n", tempLigFileName);
	QApplication::postEvent(my_parent, new ViewRowEventResultDetails("delete", 0, displaySurface));
}

// Public slot
void F2DockResultDetails::viewSlot()
{
	cout<<"Save detres"<<endl;
	int numsel = f2dResultsTable->numSelections();
	int i;
	int row;
	char filepath[200];
	char filename[250];
	strncpy(filepath, baspar->outputFile.ascii(), strlen(baspar->outputFile.ascii())-4);
	filepath[strlen(baspar->outputFile.ascii())-4]=0;
	cout<<numsel<<" conformations will be saved"<<endl;
	FILE* pdbfile;
	pdbfile = fopen(baspar->ligandFile.ascii(),"rt");
	if (!pdbfile)
	{
		QMessageBox::information(this, "Unable to find the pdb file",  "Unable to find the pdb file.\n");
		return;
	}
	for (i=0; i<numsel; i++)
	{
		row = (f2dResultsTable->selection(i)).anchorRow();
		sprintf(filename, "%s_conf_%d.pdb", filepath, row+1);
		if (! transform_pdb(detres->at(row)->mat, pdbfile, filename))
		{
			QMessageBox::information(this, "Failed to save pdb",  "Failed to save pdb.\n");
			return;
		}
		cout<<"Saved pdb file : "<<filename<<endl;
	}
}

/*class ViewRowEvent : public QCustomEvent
{
	public:
		ViewRowEvent(const QString& m) : QCustomEvent(QEvent::User+102), msg(m) {}
		QString message() const
		{
			return msg;
		}
	private:
		QString msg;
};*/

void F2DockResultDetails::selectRowSlot()
{
	if(!pdbSent)
	{
		cout<<"Save detres"<<endl;
		int numsel = f2dResultsTable->numSelections();
		int i;
		int row;
		char filepath[200];
		char filename[250];
		strncpy(filepath, baspar->outputFile.ascii(), strlen(baspar->outputFile.ascii())-4);
		filepath[strlen(baspar->outputFile.ascii())-4]=0;
		cout<<numsel<<" conformations will be saved"<<endl;
		FILE* pdbfile;
		pdbfile = fopen(baspar->ligandFile.ascii(),"rt");
		if (!pdbfile)
		{
			QMessageBox::information(this, "Unable to find the pdb file",  "Unable to find the pdb file.\n");
			return;
		}
		row = (f2dResultsTable->selection(0)).anchorRow();
		//sprintf(filename, "%s_conf_%d.pdb", filepath, row+1);
		sprintf(filename, "%stemplig.pdb", filepath);
		sprintf(tempLigFileName, "%stemplig.pdb", filepath);
		if (! non_transform_pdb(pdbfile, filename))
		{
			QMessageBox::information(this, "Failed to save pdb",  "Failed to save pdb.\n");
			return;
		}
		cout<<"Saved pdb file : "<<filename<<endl;
		fclose(pdbfile);
		//char m[10] = "select";
		QApplication::postEvent(my_parent, new ViewRowEventResultDetails(filename, detres->at(row)->mat, displaySurface));
		pdbSent = true;
	}
	else
	{
		int row = (f2dResultsTable->selection(0)).anchorRow();
		QApplication::postEvent(my_parent, new ViewRowEventResultDetails("matrix changed", detres->at(row)->mat, displaySurface));
	}
}


bool F2DockResultDetails::transform_pdb(double* trans, FILE* ifp, char* ofname)
{
	FILE* ofp = fopen(ofname, "wt");
	if (ofp == NULL)
	{
		fprintf(stderr, "\n\nError: Failed to create transformed PDB file ( %s )!\n\n", ofname);
		return false;
	}
	fseek(ifp, 0, SEEK_SET);
	char line[ 500 ];
	int l = 0;
	while (fgets(line, 300, ifp) != NULL)
	{
		l++;
		if (strncmp(line, "ATOM", 4) && strncmp(line, "HETATM", 6))
		{
			fprintf(ofp, "%s", line);
			continue;
		}
		double x, y, z;
		if (sscanf(line + 30, "%lf %lf %lf", &x, &y, &z) != 3)
		{
			fprintf(stderr, "\n\nError: Failed to read coordinates from line %d of input PDB file!\n\n", l);
			return false;
		}
		double nx = trans[  0 ] * x + trans[  1 ] * y + trans[  2 ] * z + trans[  3 ],
			   ny = trans[  4 ] * x + trans[  5 ] * y + trans[  6 ] * z + trans[  7 ],
			   nz = trans[  8 ] * x + trans[  9 ] * y + trans[ 10 ] * z + trans[ 11 ];
		line[ 30 ] = 0;
		fprintf(ofp, "%s%8.3lf%8.3lf%8.3lf%s", line, nx, ny, nz, line + 54);
	}
	fclose(ofp);
	return true;
}

bool F2DockResultDetails::non_transform_pdb(FILE* ifp, char* ofname)
{
	FILE* ofp = fopen(ofname, "wt");
	if (ofp == NULL)
	{
		fprintf(stderr, "\n\nError: Failed to create non-transformed PDB file ( %s )!\n\n", ofname);
		return false;
	}
	fseek(ifp, 0, SEEK_SET);
	char line[ 500 ];
	int l = 0;
	while (fgets(line, 300, ifp) != NULL)
	{
		l++;
		if (strncmp(line, "ATOM", 4) && strncmp(line, "HETATM", 6))
		{
			fprintf(ofp, "%s", line);
			continue;
		}
		double x, y, z;
		if (sscanf(line + 30, "%lf %lf %lf", &x, &y, &z) != 3)
		{
			fprintf(stderr, "\n\nError: Failed to read coordinates from line %d of input PDB file!\n\n", l);
			return false;
		}
		/*double nx = trans[  0 ] * x + trans[  1 ] * y + trans[  2 ] * z + trans[  3 ],
			   ny = trans[  4 ] * x + trans[  5 ] * y + trans[  6 ] * z + trans[  7 ],
			   nz = trans[  8 ] * x + trans[  9 ] * y + trans[ 10 ] * z + trans[ 11 ];*/
		line[ 30 ] = 0;
		//fprintf(ofp, "%s%8.3lf%8.3lf%8.3lf%s", line, nx, ny, nz, line + 54);
		fprintf(ofp, "%s%8.3lf%8.3lf%8.3lf%s", line, x, y, z, line + 54);
	}
	fclose(ofp);
	return true;
}

void F2DockResultDetails::setParams(vector<DetailResults*> *dr, BasicParams* bp)
{
	cout<<"Set detres"<<endl;
	detres = dr;
	numRow = detres->size();
	f2dResultsTable->setNumRows(numRow+1);
	baspar = bp;
	/*    for(int i=0; i<numRow; i++)
	    {
		for(int j=0; j<24; j++)
		{
		    QTableItem *qti = new QTableItem(f2dResultsTable, QTableItem::Never);
		    f2dResultsTable->setItem(i,j, qti);
		}
	    }*/
	setValues();
	/*char filepath[200];
	char filename[250];
	strncpy(filepath, baspar->outputFile.ascii(), strlen(baspar->outputFile.ascii())-4);
	filepath[strlen(baspar->outputFile.ascii())-4]=0;
	FILE* pdbfile;
	double identityMat[12] = {1,0,0,0, 0,1,0,0, 0,0,1,0};
	pdbfile = fopen(baspar->receptorFile.ascii(),"rt");
	if (!pdbfile)
	{
		QMessageBox::information(this, "Unable to find the pdb file",  "Unable to find the pdb file.\n");
		return;
	}
	sprintf(filename, "%stemprec.pdb", filepath);
	sprintf(tempRecFileName, "%stemprec.pdb", filepath);
	if (! transform_pdb(identityMat, pdbfile, filename))
	{
		QMessageBox::information(this, "Failed to save pdb",  "Failed to save pdb.\n");
		return;
	}
	cout<<"Saved pdb file : "<<filename<<endl;
	fclose(pdbfile);
	//char m[10] = "select";
	QApplication::postEvent(my_parent, new ViewRowEventResultDetails(filename));*/
}

void F2DockResultDetails::setValues()
{
	cout<<"Show detres"<<endl;
	int i;
	char temp[20];
	for (i=0; i<numRow; i++)
	{
		sprintf(temp, "%d", (detres->at(numRow-i-1))->rank);
		f2dResultsTable->setText(i, 0, temp);
		sprintf(temp, "%0.4lf", (detres->at(numRow-i-1))->score);
		f2dResultsTable->setText(i, 1, temp);
		sprintf(temp, "%0.4lf", (detres->at(numRow-i-1))->shape);
		f2dResultsTable->setText(i, 2, temp);
		sprintf(temp, "%0.4lf", (detres->at(numRow-i-1))->elec);
		f2dResultsTable->setText(i, 3, temp);
		sprintf(temp, "%0.4lf", (detres->at(numRow-i-1))->hbond);
		f2dResultsTable->setText(i, 4, temp);
		sprintf(temp, "%0.4lf", (detres->at(numRow-i-1))->vdw);
		f2dResultsTable->setText(i, 5, temp);
		sprintf(temp, "%0.4lf", (detres->at(numRow-i-1))->rmsd);
		f2dResultsTable->setText(i, 6, temp);
		sprintf(temp, "%d", (detres->at(numRow-i-1))->clashes);
		f2dResultsTable->setText(i, 7, temp);
		sprintf(temp, "%0.4lf", (detres->at(numRow-i-1))->ssr);
		f2dResultsTable->setText(i, 8, temp);
		sprintf(temp, "%0.4lf", (detres->at(numRow-i-1))->ccr);
		f2dResultsTable->setText(i, 9, temp);
		sprintf(temp, "%0.4lf", (detres->at(numRow-i-1))->scr);
		f2dResultsTable->setText(i, 10, temp);
		for (int j=0; j<12; j++)
		{
			sprintf(temp, "%0.2lf", (detres->at(numRow-i-1))->mat[0+j]);
			f2dResultsTable->setText(i, 11+j, temp);
		}
		sprintf(temp, "%d", (detres->at(numRow-i-1))->conf);
		f2dResultsTable->setText(i, 23, temp);
	}
}

void F2DockResultDetails::surfaceSlot(bool surface)
{
	displaySurface = surface;
	QApplication::postEvent(my_parent, new ViewRowEventResultDetails("surface button changed", 0, displaySurface));
}
