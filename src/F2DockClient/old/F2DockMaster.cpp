#include <F2DockClient/F2dGen.h>
#include <F2DockClient/F2DockForm.h>
#include <F2DockClient/GBRerankForm.h>
#include <F2DockClient/QuadGen.h>
#include "F2DockMaster.h"
#include <qapplication.h>
#include <q3filedialog.h>
#include <qmessagebox.h>
#include <q3table.h>
//Added by qt3to4:
#include <QCustomEvent>
#include <QEvent>

// Constructs a F2DockMaster which is a child of 'parent', with the name 'name' and widget flags set to 'f'
// The dialog will by default be modeless, unless you set 'modal' to TRUE to construct a modal dialog.
F2DockMaster::F2DockMaster(QWidget* parent,  const char* name, bool modal, Qt::WFlags fl)
	: f2dockmaster(parent, name, modal, fl), my_parent(parent)
{
	QStringList labels;
	labels.append("Job Id");
	labels.append("Job Type");
	labels.append("Current Status");
	labels.append("Job Files ...");
	jobTable->setNumCols(4);
	jobTable->setColumnLabels(labels);
	jobTable->setColumnWidth(0, 50);
	jobTable->setColumnWidth(1, 150);
	jobTable->setColumnWidth(2, 100);
	jobTable->setColumnWidth(3, 250);
	jobTable->setNumRows(10);
	selectedJob = -1;
}

F2DockMaster::~F2DockMaster()
{
}

void F2DockMaster::dockSlot()
{
	JobParams* newjob = new JobParams();
	newjob->jobtype = 1;
	newjob->baspar.submitted = false;
	newjob->baspar.completed = false;
	F2DockForm dialog(this);
	dialog.setParams(&(newjob->baspar));
	dialog.exec();
	if (newjob->baspar.submitted)
	{
		newjob->submitted = true;
		newjob->running = false;
		newjob->completed = false;
		newjob->available = false;
		newjob->jobid = newjob->baspar.jobid;
		jobs.push_back(newjob);
		updateTable();
		startNag();
	}
	else
	{
		QMessageBox::information(this, "job Submission Failed",  "Job Submission Failed.\n");
	}
}

void F2DockMaster::f2dgenSlot()
{
	JobParams* newjob = new JobParams();
	newjob->jobtype = 2;
	newjob->f2dpar.submitted = false;
	newjob->f2dpar.completed = false;
	F2dGen dialog(this);
	dialog.setParams(&(newjob->f2dpar));
	dialog.exec();
	if (newjob->f2dpar.submitted)
	{
		newjob->submitted = true;
		newjob->running = false;
		newjob->completed = false;
		newjob->available = false;
		newjob->jobid = newjob->f2dpar.jobid;
		jobs.push_back(newjob);
		updateTable();
		startNag();
	}
	else
	{
		QMessageBox::information(this, "job Submission Failed",  "Job Submission Failed.\n");
	}
}

void F2DockMaster::rerankSlot()
{
	JobParams* newjob = new JobParams();
	newjob->jobtype = 3;
	newjob->gbpar.submitted = false;
	newjob->gbpar.completed = false;
	GBRerankForm dialog(this);
	dialog.setParams(&(newjob->gbpar));
	dialog.exec();
	if (newjob->gbpar.submitted)
	{
		newjob->submitted = true;
		newjob->running = false;
		newjob->completed = false;
		newjob->available = false;
		newjob->jobid = newjob->gbpar.jobid;
		jobs.push_back(newjob);
		updateTable();
		startNag();
	}
	else
	{
		QMessageBox::information(this, "job Submission Failed",  "Job Submission Failed.\n");
	}
}

void F2DockMaster::quadgenSlot()
{
	JobParams* newjob = new JobParams();
	newjob->jobtype = 4;
	newjob->quadpar.submitted = false;
	newjob->quadpar.completed = false;
	QuadGen dialog(this);
	dialog.setParams(&(newjob->quadpar));
	dialog.exec();
	if (newjob->quadpar.submitted)
	{
		newjob->submitted = true;
		newjob->running = false;
		newjob->completed = false;
		newjob->available = false;
		newjob->jobid = newjob->quadpar.jobid;
		jobs.push_back(newjob);
		updateTable();
		startNag();
	}
	else
	{
		QMessageBox::information(this, "job Submission Failed",  "Job Submission Failed.\n");
	}
}

void F2DockMaster::jobdetSlot()
{
	qWarning("F2DockMaster::jobdetSlot() not yet implemented!");
}

void F2DockMaster::resultSlot()
{
	int size = jobs.size();
	if (selectedJob < 0 || selectedJob >= size)
	{
		QMessageBox::information(this, "Get result",  "Please select a job first\n");
		return;
	}
	int type = jobs[selectedJob]->jobtype;
	cout<<type<<endl;
	if (jobs[selectedJob]->running)
	{
		QMessageBox::information(this, "Get result",  "Job is still executing. Please wait for it to finish\n");
		return;
	}
	if (jobs[selectedJob]->completed && !jobs[selectedJob]->available)
	{
		switch (type)
		{
		case 1:
		{
			client.setValues(&(jobs[selectedJob]->baspar));
			if (client.getResult())
			{
				jobs[selectedJob]->available = true;
				jobTable->setText(selectedJob, 2, "Available");
			}
			else
			{
				QMessageBox::information(this,  "Get result",  "Problem in saving result.\n");
			}
			break;
		}
		case 2:
		{
			client.setF2dGenValues(&(jobs[selectedJob]->f2dpar));
			if (client.getF2dGenResult())
			{
				jobs[selectedJob]->available = true;
				jobTable->setText(selectedJob, 2, "Available");
			}
			else
			{
				QMessageBox::information(this,  "Get result",  "Problem in saving result.\n");
			}
			break;
		}
		case 3:
		{
			client.setGBRerankValues(&(jobs[selectedJob]->gbpar));
			if (client.getGBRerankResult())
			{
				jobs[selectedJob]->available = true;
				jobTable->setText(selectedJob, 2, "Available");
			}
			else
			{
				QMessageBox::information(this,  "Get result",  "Problem in saving result.\n");
			}
			break;
		}
		case 4:
		{
			client.setQuadGenValues(&(jobs[selectedJob]->quadpar));
			if (client.getQuadGenResult())
			{
				jobs[selectedJob]->available = true;
				jobTable->setText(selectedJob, 2, "Available");
			}
			else
			{
				QMessageBox::information(this,  "Get result",  "Problem in saving result.\n");
			}
			break;
		}
		}
	}
	if (jobs[selectedJob]->available)
	{
		switch (type)
		{
		case 1:
		{
			f2dresDialog = new F2DockResult(this);
			f2dresDialog->setParams(&(jobs[selectedJob]->baspar));
			f2dresThread = new F2DockResultThread(this);
			f2dresThread->setF2DockResForm(f2dresDialog);
			f2dresThread->start();
			break;
		}
		case 2:
		{
			QMessageBox::information(this, "Get result",  "Output file saved in the specified location\n");
			break;
		}
		case 3:
		{
			gbresDialog = new GBRerankResult(this);
			gbresDialog->setParams(&(jobs[selectedJob]->gbpar));
			gbresThread = new GBRerankResultThread(this);
			gbresThread->setGBRerankResForm(gbresDialog);
			gbresThread->start();
			break;
		}
		case 4:
		{
			QMessageBox::information(this, "Get result",  "Output file saved in the specified location\n");
			break;
		}
		}
	}
}

void F2DockMaster::saveSlot()
{
	int size = jobs.size();
	if (selectedJob < 0 || selectedJob >= size)
	{
		QMessageBox::information(this, "Save result",  "Please select a job first\n");
		return;
	}
	
	char filepath[200];
	char filename[250];
	switch(jobs[selectedJob]->jobtype)
	{
	case 1:
	{
		//strncpy(filepath, jobs[selectedJob]->baspar.outputFile.ascii(), strlen(jobs[selectedJob]->baspar.outputFile.ascii())-4);
		string str = string(jobs[selectedJob]->baspar.outputFile.ascii());
		const char* data = str.substr(0, str.find_last_of('/') + 1).data();
		strcpy(filepath, data);
		break;
	}
	case 2:
	{
		//strncpy(filepath, jobs[selectedJob]->f2dpar.receptorFile.ascii(), strlen(jobs[selectedJob]->f2dpar.receptorFile.ascii())-4);
		string str = string(jobs[selectedJob]->f2dpar.receptorFile.ascii());
		const char* data = str.substr(0, str.find_last_of('/') + 1).data();
		strcpy(filepath, data);
		break;
	}
	case 3:
	{
		//strncpy(filepath, jobs[selectedJob]->gbpar.rerankOutputFile.ascii(), strlen(jobs[selectedJob]->gbpar.rerankOutputFile.ascii())-4);
		string str = string(jobs[selectedJob]->gbpar.rerankOutputFile.ascii());
		const char* data = str.substr(0, str.find_last_of('/') + 1).data();
		strcpy(filepath, data);
		break;
	}
	case 4:
	{
		//strncpy(filepath, jobs[selectedJob]->quadpar.receptorFile.ascii(), strlen(jobs[selectedJob]->quadpar.receptorFile.ascii())-4);
		string str = string(jobs[selectedJob]->quadpar.receptorFile.ascii());
		const char* data = str.substr(0, str.find_last_of('/') + 1).data();
		strcpy(filepath, data);
		break;
	}
	}
	if(jobs[selectedJob]->available)
	{
		sprintf(filename, "%sjob%s-available.txt", filepath, jobs[selectedJob]->jobid.c_str());
	}
	else if(jobs[selectedJob]->completed)
	{
		sprintf(filename, "%sjob%s-completed.txt", filepath, jobs[selectedJob]->jobid.c_str());
	}
	else if(jobs[selectedJob]->running)
	{
		//won't ever get here since running state is unused in code
		sprintf(filename, "%sjob%s-running.txt", filepath, jobs[selectedJob]->jobid.c_str());
	}
	else if(jobs[selectedJob]->submitted)
	{
		sprintf(filename, "%sjob%s-submitted.txt", filepath, jobs[selectedJob]->jobid.c_str());
	}
	else {
		sprintf(filename, "%sjob%s.txt", filepath, jobs[selectedJob]->jobid.c_str());
	}
	
	FILE* ofp = fopen(filename, "wt");
	if (ofp == NULL)
	{
		fprintf(stderr, "\n\nError: Failed to create job file ( %s )!\n\n", filename);
		return;
	}
	else
	{
		printf("\n\nCreated job file ( %s )!\n\n", filename);
	}
	
	//write out generic job params
	JobParams* jobpar = jobs[selectedJob];
	fprintf(ofp, "%s\n", jobpar->hostName.ascii());
	fprintf(ofp, "%d\n", jobpar->port);
	fprintf(ofp, "%s\n", jobpar->jobid.c_str());
	fprintf(ofp, "%d\n", jobpar->jobtype);
	fprintf(ofp, "%u\n", jobpar->submitted);
	fprintf(ofp, "%u\n", jobpar->running);
	fprintf(ofp, "%u\n", jobpar->completed);
	fprintf(ofp, "%u\n", jobpar->available);
	
	switch(jobpar->jobtype)
	{
	case 1:
	{
		//write out basic parameters
		BasicParams* baspar = &(jobs[selectedJob]->baspar);
		fprintf(ofp, "%s\n", baspar->receptorFile.ascii());
		fprintf(ofp, "%s\n", baspar->receptorPDB.ascii());
		fprintf(ofp, "%s\n", baspar->ligandFile.ascii());
		fprintf(ofp, "%s\n", baspar->rmsdFile.ascii());
		fprintf(ofp, "%s\n", baspar->outputFile.ascii());
		fprintf(ofp, "%s\n", baspar->inputFile.ascii());
		fprintf(ofp, "%s\n", baspar->hostName.ascii());
		fprintf(ofp, "%d\n", baspar->port);
		fprintf(ofp, "%s\n", baspar->jobid.c_str());
		fprintf(ofp, "%d\n", baspar->rotSeparation);
		fprintf(ofp, "%f\n", baspar->gridSpacing);
		fprintf(ofp, "%d\n", baspar->numSol);
		fprintf(ofp, "%f\n", baspar->ssWeight);
		fprintf(ofp, "%f\n", baspar->scWeight);
		fprintf(ofp, "%f\n", baspar->ccWeight);
		fprintf(ofp, "%f\n", baspar->elecWeight);
		fprintf(ofp, "%u\n", baspar->bound);
		fprintf(ofp, "%u\n", baspar->submitted);
		fprintf(ofp, "%u\n", baspar->completed);
		break;
	}
	case 2:
	{
		//write out f2dgenParams
		F2dParams* f2dpar = &(jobs[selectedJob]->f2dpar);
		fprintf(ofp, "%s\n", f2dpar->receptorFile.ascii());
		fprintf(ofp, "%s\n", f2dpar->ligandFile.ascii());
		fprintf(ofp, "%s\n", f2dpar->hostName.ascii());
		fprintf(ofp, "%d\n", f2dpar->port);
		fprintf(ofp, "%s\n", f2dpar->jobid.c_str());
		fprintf(ofp, "%u\n", f2dpar->submitted);
		fprintf(ofp, "%u\n", f2dpar->completed);
		break;
	}
	case 3:
	{
		//write out rerank params
		RerankBasicParams* gbpar = &(jobs[selectedJob]->gbpar);
		fprintf(ofp, "%s\n", gbpar->receptorPDB.ascii());
		fprintf(ofp, "%s\n", gbpar->ligandPDB.ascii());
		fprintf(ofp, "%s\n", gbpar->receptorQUAD.ascii());
		fprintf(ofp, "%s\n", gbpar->ligandQUAD.ascii());
		fprintf(ofp, "%s\n", gbpar->f2dockOutputFile.ascii());
		fprintf(ofp, "%s\n", gbpar->rerankInputFile.ascii());
		fprintf(ofp, "%s\n", gbpar->rerankOutputFile.ascii());
		fprintf(ofp, "%s\n", gbpar->hostName.ascii());
		fprintf(ofp, "%d\n", gbpar->port);
		fprintf(ofp, "%s\n", gbpar->jobid.c_str());
		fprintf(ofp, "%f\n", gbpar->gpolWeight);
		fprintf(ofp, "%f\n", gbpar->gnonpolWeight);
		fprintf(ofp, "%f\n", gbpar->f2dockWeight);
		fprintf(ofp, "%f\n", gbpar->epsilonBR);
		fprintf(ofp, "%f\n", gbpar->epsilonGpol);
		fprintf(ofp, "%d\n", gbpar->numSol);
		fprintf(ofp, "%u\n", gbpar->submitted);
		fprintf(ofp, "%u\n", gbpar->completed);
		break;
	}
	case 4:
	{
		//write out quadgen params
		QuadParams* quadpar = &(jobs[selectedJob]->quadpar);
		fprintf(ofp, "%s\n", quadpar->receptorFile.ascii());
		fprintf(ofp, "%s\n", quadpar->ligandFile.ascii());
		fprintf(ofp, "%u\n", quadpar->fine);
		fprintf(ofp, "%s\n", quadpar->jobid.c_str());
		fprintf(ofp, "%u\n", quadpar->submitted);
		fprintf(ofp, "%u\n", quadpar->completed);
		break;
	}
	}
	
	fclose(ofp);
}

void F2DockMaster::loadSlot()
{
	QString filename = Q3FileDialog::getOpenFileName("", "Select Job file (*.txt);;All files (*.*)");
	FILE* jobfile;
	jobfile = fopen(filename,"rt");
	if (!jobfile)
	{
		QMessageBox::information(this, "Unable to find the job file",  "Unable to find the job file.\n");
		return;
	}
	
	char lineraw[500];
	char line[500];
	int l = 0;
	JobParams* jobpar = new JobParams();
	BasicParams* baspar = &(jobpar->baspar);
	F2dParams* f2dpar = &(jobpar->f2dpar);
	RerankBasicParams* gbpar = &(jobpar->gbpar);
	QuadParams* quadpar = &(jobpar->quadpar);
	while(l <= 7 && fgets(lineraw, 500, jobfile) != NULL)
	{
		sscanf(lineraw, "%s\n", line);
		switch(l)
		{
		//job params
		case 0:
		{
			jobpar->hostName = line;
			//printf("host name: %s\n", line);
			l++; break;
		}
		case 1:
		{
			jobpar->port = atoi(line);
			//printf("port: %s\n", line);
			l++; break;
		}
		case 2:
		{
			jobpar->jobid = line;
			//printf("jobid: %s\n", line);
			l++; break;
		}
		case 3:
		{
			jobpar->jobtype = atoi(line);
			//printf("job type: %s\n", line);
			l++; break;
		}
		case 4:
		{
			jobpar->submitted = atoi(line);
			//printf("submitted: %s\n", line);
			l++; break;
		}
		case 5:
		{
			jobpar->running = atoi(line);
			//printf("running: %s\n", line);
			l++; break;
		}
		case 6:
		{
			jobpar->completed = atoi(line);
			//printf("completed: %s\n", line);
			l++; break;
		}
		case 7:
		{
			jobpar->available = atoi(line);
			//printf("available: %s\n", line);
			l++; break;
		}
		}
	}
	if(l < 8)
	{
		printf("could not get all arguments, l = %d\n", l);
		delete jobpar;
		jobpar = 0;
		return;
	}
	if(jobpar->jobtype == 1)
	{
		while(l <= 26 && fgets(lineraw, 500, jobfile) != NULL)
		{
			sscanf(lineraw, "%s\n", line);
			//basic parameters
			switch(l)
			{
			case 8:
			{
				baspar->receptorFile = line;
				//printf("receptor file: %s\n", line);
				l++; break;
			}
			case 9:
			{
				baspar->receptorPDB = line;
				//printf("receptor pdb: %s\n", line);
				l++; break;
			}
			case 10:
			{
				baspar->ligandFile = line;
				//printf("ligand file: %s\n", line);
				l++; break;
			}
			case 11:
			{
				baspar->rmsdFile = line;
				//printf("rmsd file: %s\n", line);
				l++; break;
			}
			case 12:
			{
				baspar->outputFile = line;
				//printf("output file: %s\n", line);
				l++; break;
			}
			case 13:
			{
				baspar->inputFile = line;
				//printf("input file: %s\n", line);
				l++; break;
			}
			case 14:
			{
				baspar->hostName = line;
				//printf("host name: %s\n", line);
				l++; break;
			}
			case 15:
			{
				baspar->port = atoi(line);
				//printf("port: %s\n", line);
				l++; break;
			}
			case 16:
			{
				baspar->jobid = line;
				//printf("jobid: %s\n", line);
				l++; break;
			}
			case 17:
			{
				baspar->rotSeparation = atoi(line);
				//printf("rot separation: %s\n", line);
				l++; break;
			}
			case 18:
			{
				baspar->gridSpacing = atof(line);
				//printf("grid spacing: %s\n", line);
				l++; break;
			}
			case 19:
			{
				baspar->numSol = atoi(line);
				//printf("num sol: %s\n", line);
				l++; break;
			}
			case 20:
			{
				baspar->ssWeight = atof(line);
				//printf("ss weight: %s\n", line);
				l++; break;
			}
			case 21:
			{
				baspar->scWeight = atof(line);
				//printf("sc weight: %s\n", line);
				l++; break;
			}
			case 22:
			{
				baspar->ccWeight = atof(line);
				//printf("cc weight: %s\n", line);
				l++; break;
			}
			case 23:
			{
				baspar->elecWeight = atof(line);
				//printf("elec weight: %s\n", line);
				l++; break;
			}
			case 24:
			{
				baspar->bound = atoi(line);
				//printf("bound: %s\n", line);
				l++; break;
			}
			case 25:
			{
				baspar->submitted = atoi(line);
				//printf("submitted: %s\n", line);
				l++; break;
			}
			case 26:
			{
				baspar->completed = atoi(line);
				//printf("completed: %s\n", line);
				l++; break;
			}
			}
		}
		if(l < 27)
		{
			printf("could not get all arguments, l = %d\n", l);
			delete jobpar;
			jobpar = 0;
			return;
		}
	}
	else if(jobpar->jobtype == 2)
	{
		while(l <= 14 && fgets(lineraw, 500, jobfile) != NULL)
		{
			sscanf(lineraw, "%s\n", line);
			switch(l)
			{
			//f2dgen parameters
			case 8:
			{
				f2dpar->receptorFile = line;
				l++; break;
			}
			case 9:
			{
				f2dpar->ligandFile = line;
				l++; break;
			}
			case 10:
			{
				f2dpar->hostName = line;
				l++; break;
			}
			case 11:
			{
				f2dpar->port = atoi(line);
				l++; break;
			}
			case 12:
			{
				f2dpar->jobid = line;
				l++; break;
			}
			case 13:
			{
				f2dpar->submitted = atoi(line);
				l++; break;
			}
			case 14:
			{
				f2dpar->completed = atoi(line);
				l++; break;
			}
			}
		}
		if(l < 15)
		{
			printf("could not get all arguments, l = %d\n", l);
			delete jobpar;
			jobpar = 0;
			return;
		}
	}
	else if(jobpar->jobtype == 3)
	{
		while(l <= 25 && fgets(lineraw, 500, jobfile) != NULL)
		{
			sscanf(lineraw, "%s\n", line);
			switch(l)
			{
			//rerank parameters
			case 8:
			{
				gbpar->receptorPDB = line;
				l++; break;
			}
			case 9:
			{
				gbpar->ligandPDB = line;
				l++; break;
			}
			case 10:
			{
				gbpar->receptorQUAD = line;
				l++; break;
			}
			case 11:
			{
				gbpar->ligandQUAD = line;
				l++; break;
			}
			case 12:
			{
				gbpar->f2dockOutputFile = line;
				l++; break;
			}
			case 13:
			{
				gbpar->rerankInputFile = line;
				l++; break;
			}
			case 14:
			{
				gbpar->rerankOutputFile = line;
				l++; break;
			}
			case 15:
			{
				gbpar->hostName = line;
				l++; break;
			}
			case 16:
			{
				gbpar->port = atoi(line);
				l++; break;
			}
			case 17:
			{
				gbpar->jobid = line;
				l++; break;
			}
			case 18:
			{
				gbpar->gpolWeight = atof(line);
				l++; break;
			}
			case 19:
			{
				gbpar->gnonpolWeight = atof(line);
				l++; break;
			}
			case 20:
			{
				gbpar->f2dockWeight = atof(line);
				l++; break;
			}
			case 21:
			{
				gbpar->epsilonBR = atof(line);
				l++; break;
			}
			case 22:
			{
				gbpar->epsilonGpol = atof(line);
				l++; break;
			}
			case 23:
			{
				gbpar->numSol = atoi(line);
				l++; break;
			}
			case 24:
			{
				gbpar->submitted = atoi(line);
				l++; break;
			}
			case 25:
			{
				gbpar->completed = atoi(line);
				l++; break;
			}
			}
		}
		if(l < 26)
		{
			printf("could not get all arguments, l = %d\n", l);
			delete jobpar;
			jobpar = 0;
			return;
		}
	}
	else if(jobpar->jobtype == 4)
	{
		while(l <= 13 && fgets(lineraw, 500, jobfile) != NULL)
		{
			sscanf(lineraw, "%s\n", line);
			switch(l)
			{
			//quadgen params
			case 8:
			{
				quadpar->receptorFile = line;
				l++; break;
			}
			case 9:
			{
				quadpar->ligandFile = line;
				l++; break;
			}
			case 10:
			{
				quadpar->fine = atoi(line);
				l++; break;
			}
			case 11:
			{
				quadpar->jobid = line;
				l++; break;
			}
			case 12:
			{
				quadpar->submitted = atoi(line);
				l++; break;
			}
			case 13:
			{
				quadpar->completed = atoi(line);
				l++; break;
			}
			}
		}
		if(l < 14)
		{
			printf("could not get all arguments, l = %d\n", l);
			delete jobpar;
			jobpar = 0;
			return;
		}
	}
	fclose(jobfile);
	jobs.push_back(jobpar);
	updateTable();
	
	//make sure that the mentioned job files exist
	switch(jobpar->jobtype)
	{
	case 1:
	{
		FILE* file;
		file = fopen(jobpar->baspar.receptorFile.ascii(),"rt");
		if (!file)
		{
			QMessageBox::information(this, "Unable to find receptor f2d file",  "Unable to find receptor f2d file.\n");
			printf("file: %s\n", jobpar->baspar.receptorFile.ascii());
			return;
		}
		file = fopen(jobpar->baspar.receptorPDB.ascii(),"rt");
		if (!file)
		{
			QMessageBox::information(this, "Unable to find receptor pdb file",  "Unable to find receptor pdb file.\n");
			return;
		}
		file = fopen(jobpar->baspar.ligandFile.ascii(),"rt");
		if (!file)
		{
			QMessageBox::information(this, "Unable to find ligand file",  "Unable to find ligand file.\n");
			return;
		}
		file = fopen(jobpar->baspar.rmsdFile.ascii(),"rt");
		if (!file)
		{
			QMessageBox::information(this, "Unable to find rmsd file",  "Unable to find rmsd file.\n");
			return;
		}
		file = fopen(jobpar->baspar.outputFile.ascii(),"rt");
		if (!file)
		{
			QMessageBox::information(this, "Unable to find output file",  "Unable to find input file.\n");
			return;
		}
		file = fopen(jobpar->baspar.inputFile.ascii(),"rt");
		if (!file)
		{
			QMessageBox::information(this, "Unable to find input file",  "Unable to find input file.\n");
			return;
		}
		break;
	}
	case 2:
	{
		FILE* file;
		file = fopen(jobpar->f2dpar.receptorFile.ascii(),"rt");
		if (!file)
		{
			QMessageBox::information(this, "Unable to find receptor file",  "Unable to find receptor file.\n");
			return;
		}
		file = fopen(jobpar->f2dpar.ligandFile.ascii(),"rt");
		if (!file)
		{
			QMessageBox::information(this, "Unable to find ligand file",  "Unable to find ligand file.\n");
			return;
		}
		break;
	}
	case 3:
	{
		FILE* file;
		file = fopen(jobpar->gbpar.receptorPDB.ascii(),"rt");
		if (!file)
		{
			QMessageBox::information(this, "Unable to find receptor pdb file",  "Unable to find receptor pdb file.\n");
			return;
		}
		file = fopen(jobpar->gbpar.ligandPDB.ascii(),"rt");
		if (!file)
		{
			QMessageBox::information(this, "Unable to find ligand pdb file",  "Unable to find ligand pdb file.\n");
			return;
		}
		file = fopen(jobpar->gbpar.receptorQUAD.ascii(),"rt");
		if (!file)
		{
			QMessageBox::information(this, "Unable to find receptor quad file",  "Unable to find receptor quad file.\n");
			return;
		}
		file = fopen(jobpar->gbpar.ligandQUAD.ascii(),"rt");
		if (!file)
		{
			QMessageBox::information(this, "Unable to find ligand quad file",  "Unable to find ligand quad file.\n");
			return;
		}
		file = fopen(jobpar->gbpar.f2dockOutputFile.ascii(),"rt");
		if (!file)
		{
			QMessageBox::information(this, "Unable to find f2dock output file",  "Unable to find f2dock output file.\n");
			return;
		}
		file = fopen(jobpar->gbpar.rerankInputFile.ascii(),"rt");
		if (!file)
		{
			QMessageBox::information(this, "Unable to find rerank input file",  "Unable to find rerank input file.\n");
			return;
		}
		file = fopen(jobpar->gbpar.rerankOutputFile.ascii(),"rt");
		if (!file)
		{
			QMessageBox::information(this, "Unable to find rerank output file",  "Unable to find rerank output file.\n");
			return;
		}
		break;
	}
	case 4:
	{
		FILE* file;
		file = fopen(jobpar->quadpar.receptorFile.ascii(),"rt");
		if (!file)
		{
			QMessageBox::information(this, "Unable to find receptor file",  "Unable to find receptor file.\n");
			return;
		}
		file = fopen(jobpar->quadpar.ligandFile.ascii(),"rt");
		if (!file)
		{
			QMessageBox::information(this, "Unable to find ligand file",  "Unable to find ligand file.\n");
			return;
		}
		break;
	}
	}
	
	if(jobpar->available)
	{
		//do nothing.  data will be loaded from file when the details button is pushed.
	}
	else if(jobpar->completed)
	{
		//do nothing.  data will be gotten from the server when the result details button is pushed.
	}
	else if(jobpar->running)
	{
		//this state isn't used
	}
	else if(jobpar->submitted)
	{
		//just initiate the nagger
		startNag();
	}
}

void F2DockMaster::selectSlot()
{
	selectedJob = jobTable->currentRow();
	cout<<" Job "<<selectedJob <<" selected"<<endl;
}

void F2DockMaster::startNag()
{
	Nagger* nagger = new Nagger(this);
	int size = jobs.size()-1;
	cout<<"starting nagger for job "<< jobs[size]->jobid <<endl;
	nagger->setId(jobs[size]->jobid, size, 5);
	naggers.push_back(nagger);
	nagger->start();
}

void F2DockMaster::updateTable()
{
	int size = jobs.size();
	jobTable->setNumRows(size+1);
	char status[4][20] = {"Submitted", "Running", "Completed", "Available"};
	for (int i=0; i<size; i++)
	{
		int type = jobs[i]->jobtype;
		int st = 0;
		if (jobs[i]->running)
		{
			st =1;
		}
		if (jobs[i]->completed)
		{
			st = 2;
		}
		if (jobs[i]->available)
		{
			st = 3;
		}
		switch (type)
		{
		case 1:
			jobTable->setText(i, 0, jobs[i]->baspar.jobid.c_str());
			jobTable->setText(i, 1, "Docking");
			jobTable->setText(i, 2, status[st]);
			jobTable->setText(i, 3, jobs[i]->baspar.receptorFile.ascii());
			break;
		case 2:
			jobTable->setText(i, 0, jobs[i]->f2dpar.jobid.c_str());
			jobTable->setText(i, 1, "F2D Generation");
			jobTable->setText(i, 2, status[st]);
			jobTable->setText(i, 3, jobs[i]->f2dpar.receptorFile.ascii());
			break;
		case 3:
			jobTable->setText(i, 0, jobs[i]->gbpar.jobid.c_str());
			jobTable->setText(i, 1, "Reranking");
			jobTable->setText(i, 2, status[st]);
			jobTable->setText(i, 3, jobs[i]->gbpar.receptorPDB.ascii());
			break;
		case 4:
			jobTable->setText(i, 0, jobs[i]->quadpar.jobid.c_str());
			jobTable->setText(i, 1, "Quad File Generation");
			jobTable->setText(i, 2, status[st]);
			jobTable->setText(i, 3, jobs[i]->quadpar.receptorFile.ascii());
			break;
		default:
			break;
		}
	}
}

class JobFinishedEvent : public QCustomEvent
{
	public:
		JobFinishedEvent(const QString& m) : QCustomEvent(QEvent::User+101), msg(m) {}
		QString message() const
		{
			return msg;
		}
	private:
		QString msg;
};

class ViewRowEventMaster : public QCustomEvent
{
	public:
		ViewRowEventMaster(const QString& m, double* t, const bool s) : QCustomEvent(QEvent::User+104), msg(m), mat(t), surf(s) {}
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

void F2DockMaster::customEvent(QEvent* e)
{
	int index;
	if (e->type() == QEvent::User+101)
	{
		index = atoi(static_cast<JobFinishedEvent*>(e)->message());
		cout<<"Job "<<jobs[index]->jobid<<" is now complete"<<endl;
		jobs[index]->completed = true;
		jobTable->setText(index, 2, "Completed");
	}
	else if (e->type() == QEvent::User+103)
	{
		QApplication::postEvent(my_parent, new ViewRowEventMaster(static_cast<ViewRowEventMaster*>(e)->message(), static_cast<ViewRowEventMaster*>(e)->matrix(), static_cast<ViewRowEventMaster*>(e)->surface()));
	}
}

/*Q3Err:CS
F2DockMaster::Nagger::Nagger(F2DockMaster* par,  unsigned int stackSize)  : QThread(stackSize), parent(par)
*/
F2DockMaster::Nagger::Nagger(F2DockMaster* par,  unsigned int stackSize)  : QThread(par)
{
}

void F2DockMaster::Nagger::setId(string str, int id, int d)
{
	jobid = str;
	nagId = id;
	delay = d;
}

void F2DockMaster::Nagger::run()
{
	char ch[10];
	sprintf(ch, "%d", nagId);
	while (1)
	{
		sleep(delay);
		cout << " Nagging about job "<< jobid << endl;
		if (client.isComplete(jobid))
		{
			QApplication::postEvent(parent, new JobFinishedEvent(ch));
			break;
		}
	}
}

/*Q3Err:CS
F2DockMaster::F2DockResultThread::F2DockResultThread(F2DockMaster* par,  unsigned int stackSize)  : QThread(stackSize), parent(par)
*/
F2DockMaster::F2DockResultThread::F2DockResultThread(F2DockMaster* par,  unsigned int stackSize)  : QThread(par)
{
}

void F2DockMaster::F2DockResultThread::setF2DockResForm(F2DockResult* f)
{
	fdm = f;
}

void F2DockMaster::F2DockResultThread::run()
{
	cout<<"Launching F2Dock Result"<<endl;
	fdm->show();
}

/*Q3Err:CS
F2DockMaster::GBRerankResultThread::GBRerankResultThread(F2DockMaster* par,  unsigned int stackSize)  : QThread(stackSize), parent(par)
*/
F2DockMaster::GBRerankResultThread::GBRerankResultThread(F2DockMaster* par,  unsigned int stackSize)  : QThread(par)
{
}

void F2DockMaster::GBRerankResultThread::setGBRerankResForm(GBRerankResult* f)
{
	fdm = f;
}

void F2DockMaster::GBRerankResultThread::run()
{
	cout<<"Launching GB-Rerank Result"<<endl;
	fdm->show();
}
