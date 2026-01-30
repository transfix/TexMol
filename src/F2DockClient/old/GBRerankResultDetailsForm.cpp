#include <F2DockClient/GBRerankResultDetailsForm.h>
#include <Utility/utility.h>
#include <qmessagebox.h>
#include <q3table.h>

using namespace std;

// Constructs a GBRerankResultDetails which is a child of 'parent', with the name 'name' and widget flags set to 'f'
// The dialog will by default be modeless, unless you set 'modal' to TRUE to construct a modal dialog.
GBRerankResultDetails::GBRerankResultDetails(QWidget* parent,  const char* name, bool modal, Qt::WFlags fl)
	: gbrerankresultdetails(parent, name, modal, fl)
{
	numCol = 23;
	QStringList labels;
	labels.append("New Rank");
	labels.append("New Score");
	labels.append("DelGpol");
	labels.append("AreaProp");
	labels.append("Old Rank");
	labels.append("Old Score");
	labels.append("Elec. Sc.");
	labels.append("Hbond Sc.");
	labels.append("VdW Sc.");
	labels.append("RMSD");
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
	gbResultsTable->setNumCols(23);
	gbResultsTable->setColumnLabels(labels);
	gbResultsTable->setColumnWidth(0, 50);
	gbResultsTable->setColumnWidth(1, 100);
	gbResultsTable->setColumnWidth(2, 100);
	gbResultsTable->setColumnWidth(3, 100);
	gbResultsTable->setColumnWidth(4, 100);
	gbResultsTable->setColumnWidth(5, 100);
	gbResultsTable->setColumnWidth(6, 100);
	gbResultsTable->setColumnWidth(7, 100);
	gbResultsTable->setColumnWidth(8, 100);
	gbResultsTable->setColumnWidth(9, 100);
	gbResultsTable->setColumnWidth(10, 75);
	gbResultsTable->setColumnWidth(11, 75);
	gbResultsTable->setColumnWidth(12, 75);
	gbResultsTable->setColumnWidth(13, 75);
	gbResultsTable->setColumnWidth(14, 75);
	gbResultsTable->setColumnWidth(15, 75);
	gbResultsTable->setColumnWidth(16, 75);
	gbResultsTable->setColumnWidth(17, 75);
	gbResultsTable->setColumnWidth(18, 75);
	gbResultsTable->setColumnWidth(19, 75);
	gbResultsTable->setColumnWidth(20, 75);
	gbResultsTable->setColumnWidth(21, 75);
	gbResultsTable->setColumnWidth(22, 75);
}

GBRerankResultDetails::~GBRerankResultDetails()
{
	// no need to delete child widgets, Qt does it all for us
}

// Public slot
void GBRerankResultDetails::doneSlot()
{
	this->close();
}

// Public slot
void GBRerankResultDetails::viewSlot()
{
	cout<<"Save detres"<<endl;
	int numsel = gbResultsTable->numSelections();
	int row;
	char filepath[200];
	char filename[250];
	strncpy(filepath, rbaspar->rerankOutputFile.ascii(), strlen(rbaspar->rerankOutputFile.ascii())-4);
	filepath[strlen(rbaspar->rerankOutputFile.ascii())-4]=0;
	cout<<numsel<<" conformations will be saved"<<endl;
	FILE* pdbfile;
	pdbfile = fopen(rbaspar->ligandPDB.ascii(),"rt");
	if (!pdbfile)
	{
		QMessageBox::information(this, "Unable to find the pdb file",  "Unable to find the pdb file.\n");
		return;
	}
	for (int i=0; i<numsel; i++)
	{
		row = (gbResultsTable->selection(i)).anchorRow();
		sprintf(filename, "%s_conf_%d.pdb", filepath, row+1);
		if (! transform_pdb(detres->at(row)->mat, pdbfile, filename))
		{
			QMessageBox::information(this, "Failed to save pdb",  "Failed to save pdb.\n");
			return;
		}
		cout<<"Saved pdb file : "<<filename<<endl;
	}
}

bool GBRerankResultDetails::transform_pdb(double* trans, FILE* ifp, char* ofname)
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

void GBRerankResultDetails::setParams(vector<RerankDetailResults*> *dr, RerankBasicParams* rb)
{
	detres = dr;
	numRow = detres->size();
	gbResultsTable->setNumRows(numRow+1);
	rbaspar = rb;
	/*
	for(int i=0; i<numRow; i++)
	{
	for(int j=0; j<23; j++)
	{
	    QTableItem *qti = new QTableItem(gbResultsTable, QTableItem::Never);
	    gbResultsTable->setItem(i,j, qti);
	}
	}*/
	setValues();
}

void GBRerankResultDetails::setValues()
{
	char temp[20];
	for (int i=0; i<numRow; i++)
	{
		sprintf(temp, "%d", (detres->at(numRow-i-1))->newRank);
		gbResultsTable->setText(i, 0, temp);
		sprintf(temp, "%0.4lf", (detres->at(numRow-i-1))->newScore);
		gbResultsTable->setText(i, 1, temp);
		sprintf(temp, "%0.4lf", (detres->at(numRow-i-1))->delGpol);
		gbResultsTable->setText(i, 2, temp);
		sprintf(temp, "%0.4lf", (detres->at(numRow-i-1))->areaProp);
		gbResultsTable->setText(i, 3, temp);
		sprintf(temp, "%d", (detres->at(numRow-i-1))->rank);
		gbResultsTable->setText(i, 4, temp);
		sprintf(temp, "%0.4lf", (detres->at(numRow-i-1))->score);
		gbResultsTable->setText(i, 5, temp);
		sprintf(temp, "%0.4lf", (detres->at(numRow-i-1))->elec);
		gbResultsTable->setText(i, 6, temp);
		sprintf(temp, "%0.4lf", (detres->at(numRow-i-1))->hbond);
		gbResultsTable->setText(i, 7, temp);
		sprintf(temp, "%0.4lf", (detres->at(numRow-i-1))->vdw);
		gbResultsTable->setText(i, 8, temp);
		sprintf(temp, "%0.4lf", (detres->at(numRow-i-1))->rmsd);
		gbResultsTable->setText(i, 9, temp);
		for (int j=0; j<12; j++)
		{
			sprintf(temp, "%0.2lf", (detres->at(numRow-i-1))->mat[0+j]);
			gbResultsTable->setText(i, 10+j, temp);
		}
		sprintf(temp, "%d", (detres->at(numRow-i-1))->conf);
		gbResultsTable->setText(i, 22, temp);
	}
}
