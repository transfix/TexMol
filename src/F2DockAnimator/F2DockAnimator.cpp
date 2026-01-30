#include <DataManager/SurfaceDataManager/SurfaceData.h>
#include <Docking/Complex.h>
#include <Docking/Interface.h>
#include <F2DockAnimator/F2DockAnimator.h>
#include <fstream>
#include <GeometryFileTypes/RawncFile.h>
#include <Geometry/Geometry.h>
#include <GOAFileTypes/PDBFile.h>
#include <iostream>
#include <sstream>

const float TO_READ = 10;

F2DockAnimator::F2DockAnimator(const char* F2DockOutput, const char* ProteinA_PDB, const char* ProteinB_PDB, const char* OutputDir)
{
	std::cerr << "WARNING: this now reads in Transformation files (i.e. _trans.txt files)...\n";
	std::cerr << "Initializing... ";
	m_Loaded = false;
	m_OutputDir = OutputDir;
	// load proteins A and B
	m_ProteinA = PDBFile::getRepresentative()->loadFile(ProteinA_PDB);
	if (!m_ProteinA)
	{
		std::cerr << "Error loading " << ProteinA_PDB << "\n";
		return;
	}
	m_ProteinB = PDBFile::getRepresentative()->loadFile(ProteinB_PDB);
	if (!m_ProteinB)
	{
		std::cerr << "Error loading " << ProteinB_PDB << "\n";
		return;
	}
	// parse the output file & apply transformations
	parseF2DockOutput(F2DockOutput);
	m_Loaded = true;
	std::cerr << "done.\n";
}

F2DockAnimator::~F2DockAnimator()
{
	delete m_ProteinA;
	delete m_ProteinB;
}

void F2DockAnimator::parseF2DockOutput(const char* filename)
{
	try
	{
		std::ifstream infile(filename);
		for (int cnt = 0; cnt < TO_READ; cnt++)
		{
			float m[16];
			for (int i=0; i<16; i++)
			{
				infile >> m[i];
			}
			CCVOpenGLMath::Matrix transformation(m[0],m[1],m[2],m[3], m[4],m[5],m[6],m[7], m[8],m[9],m[10],m[11], m[12],m[13],m[14],m[15]);
			m_Transformations.push_back(transformation);
		}
	}
	catch (std::exception e)
	{
	}
}

void F2DockAnimator::buildFrame(const int frameNo)
{
	if (!m_Loaded)
	{
		return;
	}
	// transform the proteins to their respective positions
	// note... leaking data because parent group cleans it up
	PDBParser::GroupOfAtoms* ProteinA = new PDBParser::GroupOfAtoms(*m_ProteinA);
	PDBParser::GroupOfAtoms* ProteinB = new PDBParser::GroupOfAtoms(*m_ProteinB);
	// transform
	// FIXME: disabled so we can get the TOP solution
	ProteinB->transform(m_Transformations[frameNo]);
	// filename
	std::ostringstream filenameSS;
	filenameSS.width(3);
	filenameSS.fill('0');
	filenameSS << frameNo;
	// build output PDB
	std::cerr << "Building PDB files...\n";
	PDBParser::GroupOfAtoms OutputProtein;
	// now ProteinA, ProteinB are deleted by OutputProtein
	OutputProtein.addChain(ProteinA);
	OutputProtein.addChain(ProteinB);
	std::string filename = m_OutputDir + "_" + filenameSS.str() + ".pdb";
	PDBFile::getRepresentative()->saveFile(&OutputProtein,filename);
	// build output complex
	{
		// handler data structures
		Docking::DockComplex complex;
		bool deleteWater = false;
		complex.init(ProteinA, ProteinB, deleteWater);
		Docking::Interface interface;
		// output geometry
		Geometry* surface1;
		Geometry* surface2;
		Geometry* surface1blue;
		Geometry* surface2blue;
		Geometry* surface1no;
		Geometry* surface2no;
		Geometry* interface1;
		Geometry* interface2;
		// mark the interface
		std::cerr << "Building geometry...\n";
		const float distanceCutoff = 4;
		interface.getInterfaceMarkedGeometry(&surface1, &surface2, &surface1blue, &surface2blue, &surface1no, &surface2no, &interface1, &interface2, &complex, distanceCutoff);
		// merge the geometry together
		Geometry* mergedGeometry = surface1->merge(surface2);
		Geometry* mergedInterface = interface1->merge(interface2);
		// save files
		std::string filenameLno = m_OutputDir + "_l_no_interface_" + filenameSS.str() + ".rawnc";
		std::string filenameRno = m_OutputDir + "_r_no_interface_" + filenameSS.str() + ".rawnc";
		std::string filename = m_OutputDir + "_" + filenameSS.str() + ".rawnc";
		std::string filenameL = m_OutputDir + "_l_" + filenameSS.str() + ".rawnc";
		std::string filenameR = m_OutputDir + "_r_" + filenameSS.str() + ".rawnc";
		std::string filenameLinterface = m_OutputDir + "_l_interface_only_" + filenameSS.str() + ".rawnc";
		std::string filenameRinterface = m_OutputDir + "_r_interface_only_" + filenameSS.str() + ".rawnc";
		std::string filenameLblue = m_OutputDir + "_l_blue_" + filenameSS.str() + ".rawnc";
		std::string filenameRblue = m_OutputDir + "_r_blue_" + filenameSS.str() + ".rawnc";
		std::string filenameMergedInterface = m_OutputDir + "_interface_merged_" + filenameSS.str() + ".rawnc";
		std::cerr << "Saving...\n";
		RawncFile::getRepresentative()->saveFile(surface1no, filenameLno);
		RawncFile::getRepresentative()->saveFile(surface2no, filenameRno);
		RawncFile::getRepresentative()->saveFile(mergedGeometry, filename);
		RawncFile::getRepresentative()->saveFile(surface1, filenameL);
		RawncFile::getRepresentative()->saveFile(surface2, filenameR);
		RawncFile::getRepresentative()->saveFile(interface1, filenameLinterface);
		RawncFile::getRepresentative()->saveFile(interface2, filenameRinterface);
		RawncFile::getRepresentative()->saveFile(surface1blue, filenameLblue);
		RawncFile::getRepresentative()->saveFile(surface2blue, filenameRblue);
		RawncFile::getRepresentative()->saveFile(mergedInterface, filenameMergedInterface);
		delete mergedGeometry;
		delete surface1;
		delete surface1no;
		delete interface1;
		delete surface2;
		delete surface2no;
		delete interface2;
		delete surface1blue;
		delete surface2blue;
	}
}

void F2DockAnimator::buildFiles()
{
	if (!m_Loaded)
	{
		std::cerr << "Error loading, possibly incorrect parameters ? \n";
		return;
	}
	std::cerr << "Building files...\n";
	buildFrame(0);
	std::cerr << "\n";
}

void F2DockAnimator::saveScreenshot(int& frameNo, RendererSet* rendererSet)
{
	rendererSet->updateGL();
/*Q3Err:CS
	std::ostringstream filenameSS;
	filenameSS.width(4);
	filenameSS.fill('0');
	filenameSS << frameNo++;
	rendererSet->saveImages(filenameSS.str(), "JPEG", false);
*/
	char fno[8];
	sprintf(fno, "%04d", frameNo++);
	QString filenameSS;
	rendererSet->saveImages(filenameSS, QString("JPEG"), false);
}

// TERRIBLE HACK TO BUILD THIS VIDEO QUICKLY
void F2DockAnimator::buildMovie(DataManager* dataManager, RendererSet* rendererSet)
{
	// load the files
	Geometry* surface1 = RawncFile::getRepresentative()->loadFile("1A2K_DOCKING_l_no_interface_000.rawnc");
	Geometry* surface2 = RawncFile::getRepresentative()->loadFile("1A2K_DOCKING_r_no_interface_000.rawnc");
	Geometry* surface1interface = RawncFile::getRepresentative()->loadFile("1A2K_DOCKING_l_000.rawnc");
	Geometry* surface2interface = RawncFile::getRepresentative()->loadFile("1A2K_DOCKING_r_000.rawnc");
	Geometry* interface1 = RawncFile::getRepresentative()->loadFile("1A2K_DOCKING_l_interface_only_000.rawnc");
	Geometry* interface2 = RawncFile::getRepresentative()->loadFile("1A2K_DOCKING_r_interface_only_000.rawnc");
	Geometry* surface1blue = RawncFile::getRepresentative()->loadFile("1A2K_DOCKING_l_blue_000.rawnc");
	Geometry* surface2blue = RawncFile::getRepresentative()->loadFile("1A2K_DOCKING_r_blue_000.rawnc");
	SurfaceData* data2interfaceonly = new SurfaceData(dataManager);
	data2interfaceonly->setGeometry(interface2);
	dataManager->addData(data2interfaceonly, "data2interfaceonly");
	SurfaceData* data1interfaceonly = new SurfaceData(dataManager);
	data1interfaceonly->setGeometry(interface1);
	dataManager->addData(data1interfaceonly, "data1interfaceonly");
	SurfaceData* data2 = new SurfaceData(dataManager);
	data2->setGeometry(surface2);
	dataManager->addData(data2, "surface2");
	data2->renderEnabledSlot(true);
	data2->setVisibilityInPropertiesWidget();
	SurfaceData* data1 = new SurfaceData(dataManager);
	data1->setGeometry(surface1);
	dataManager->addData(data1, "surface1");
	data1->renderEnabledSlot(true);
	data1->setVisibilityInPropertiesWidget();
	SurfaceData* data2interface = new SurfaceData(dataManager);
	data2interface->setGeometry(surface2interface);
	dataManager->addData(data2interface, "surface2interface");
	SurfaceData* data1interface = new SurfaceData(dataManager);
	data1interface->setGeometry(surface1interface);
	dataManager->addData(data1interface, "surface1interface");
	SurfaceData* data2blue = new SurfaceData(dataManager);
	data2blue->setGeometry(surface2blue);
	dataManager->addData(data2blue, "surface2blue");
	SurfaceData* data1blue = new SurfaceData(dataManager);
	data1blue->setGeometry(surface1blue);
	dataManager->addData(data1blue, "surface1blue");
	// vectors that translate the PDB back to the origin
	CCVOpenGLMath::Vector transA(-60.095505, 2.520505, -74.019495, 0);
	CCVOpenGLMath::Vector transB(-28.189000,-8.379000,-84.119500, 0);
	const float rotationZ = -90 * (M_PI/180.0f);
	const float rotationY = 90 * (M_PI/180.0f);
	CCVOpenGLMath::Vector startPos = CCVOpenGLMath::Vector(0,50,50,1);
	CCVOpenGLMath::Vector endPos = -transB;
	int FRAMES = 100;
	int frameid = 0;
	double translation[4] = {19.5478, 29.1375, 59.6367, 0};
	double orientation[4] = {0.908284, -0.157076, -0.211121, -0.326265};
	double window = 146.8;
	rendererSet->getCurrentRenderer()->setViewingParameters(translation, orientation, window);
	CCVOpenGLMath::Matrix bTransform = CCVOpenGLMath::Matrix::translation(transB);
	bTransform.preMultiplication(CCVOpenGLMath::Matrix::rotationZ(rotationZ));
	bTransform.preMultiplication(CCVOpenGLMath::Matrix::rotationY(rotationY));
	bTransform.preMultiplication(CCVOpenGLMath::Matrix::translation(startPos));
	data2->setTransformation(bTransform);
	// fade in interface
	FRAMES = 100;
	data1interface->setAlpha(0);
	data1interface->renderEnabledSlot(true);
	data1interface->setVisibilityInPropertiesWidget();
	data2interface->setAlpha(0);
	data2interface->renderEnabledSlot(true);
	data2interface->setVisibilityInPropertiesWidget();
	for (int i=0; i<FRAMES; i++)
	{
		float alpha = .99 * (float)i/(FRAMES-1);
		data1interface->setAlpha(alpha);
		data2interface->setAlpha(alpha);
		CCVOpenGLMath::Matrix bTransform = CCVOpenGLMath::Matrix::translation(transB);
		bTransform.preMultiplication(CCVOpenGLMath::Matrix::rotationZ(rotationZ));
		bTransform.preMultiplication(CCVOpenGLMath::Matrix::rotationY(rotationY));
		bTransform.preMultiplication(CCVOpenGLMath::Matrix::translation(startPos));
		data2interface->setTransformation(bTransform);
		saveScreenshot(frameid, rendererSet);
	}
	data1->renderEnabledSlot(false);
	data1->setVisibilityInPropertiesWidget();
	data2->renderEnabledSlot(false);
	data2->setVisibilityInPropertiesWidget();
	data1interface->setAlpha(1.0f);
	data2interface->setAlpha(1.0f);
	// hold that pose
	for (int i=0; i<FRAMES*3; i++)
	{
		saveScreenshot(frameid, rendererSet);
	}
	// fade to blue interface
	FRAMES = 100;
	data1blue->setAlpha(0);
	data1blue->renderEnabledSlot(true);
	data1blue->setVisibilityInPropertiesWidget();
	data2blue->setAlpha(0);
	data2blue->renderEnabledSlot(true);
	data2blue->setVisibilityInPropertiesWidget();
	for (int i=0; i<FRAMES; i++)
	{
		float alpha = .99 * (float)i/(FRAMES-1);
		data1blue->setAlpha(alpha);
		data2blue->setAlpha(alpha);
		CCVOpenGLMath::Matrix bTransform = CCVOpenGLMath::Matrix::translation(transB);
		bTransform.preMultiplication(CCVOpenGLMath::Matrix::rotationZ(rotationZ));
		bTransform.preMultiplication(CCVOpenGLMath::Matrix::rotationY(rotationY));
		bTransform.preMultiplication(CCVOpenGLMath::Matrix::translation(startPos));
		data2blue->setTransformation(bTransform);
		saveScreenshot(frameid, rendererSet);
	}
	data1interface->renderEnabledSlot(false);
	data1interface->setVisibilityInPropertiesWidget();
	data2interface->renderEnabledSlot(false);
	data2interface->setVisibilityInPropertiesWidget();
	data1blue->setAlpha(1.0f);
	data2blue->setAlpha(1.0f);
	// hold that pose
	for (int i=0; i<FRAMES*3; i++)
	{
		saveScreenshot(frameid, rendererSet);
	}
	FRAMES = 100;
	// STOPPED HERE
	// dock the iso+interface
	for (int i=0; i<FRAMES; i++)
	{
		CCVOpenGLMath::Matrix movieTransform = CCVOpenGLMath::Matrix::translation(transB);
		float alpha = 1.0f - (float)i/(FRAMES-1);
		movieTransform.preMultiplication(CCVOpenGLMath::Matrix::rotationZ(rotationZ * alpha));
		movieTransform.preMultiplication(CCVOpenGLMath::Matrix::rotationY(rotationY * alpha));
		movieTransform.preMultiplication(CCVOpenGLMath::Matrix::translation((startPos * alpha) + (endPos *(1.0f-alpha))));
		data2blue->setTransformation(movieTransform);
		saveScreenshot(frameid, rendererSet);
	}
	double translation2[4] = {11.0928, 66.3305, 103.833, 1};
	double orientation2[4] = {0.575109, 0.685215, 0.444386, 0.0474518};
	double window2 = 43.6469;
	for (int i=0; i<FRAMES; i++)
	{
		float alpha = 1.0f - (float)i/(FRAMES-1);
		double thisTranslation[4] = { alpha* translation[0] + (1.0f-alpha)*translation2[0],
									  alpha* translation[1] + (1.0f-alpha)*translation2[1],
									  alpha* translation[2] + (1.0f-alpha)*translation2[2],
									  alpha* translation[3] + (1.0f-alpha)*translation2[3]
									};
		CCVOpenGLMath::Quaternion first(orientation[0], orientation[1], orientation[2], orientation[3]);
		CCVOpenGLMath::Quaternion second(orientation2[0], orientation2[1], orientation2[2], orientation2[3]);
		CCVOpenGLMath::Quaternion thisquat = second.slerp(first, alpha); // slerp;
		double thisOrientation[4] = {thisquat[0],thisquat[1],thisquat[2],thisquat[3]};
		double thisWindow = alpha*window + (1.0f-alpha)*window2;
		rendererSet->getCurrentRenderer()->setViewingParameters(thisTranslation, thisOrientation, thisWindow);
		saveScreenshot(frameid, rendererSet);
	}
	// add interface of A, fade A out too
	data1interfaceonly->renderEnabledSlot(true);
	data1interfaceonly->setVisibilityInPropertiesWidget();
	for (int i=0; i<FRAMES; i++)
	{
		float alpha = 1.0f - (float)i/(FRAMES-1);
		data1blue->setAlpha(alpha);
		saveScreenshot(frameid, rendererSet);
	}
	// add interface of B, fade B out
	data2interfaceonly->renderEnabledSlot(true);
	data2interfaceonly->setVisibilityInPropertiesWidget();
	FRAMES = 100;
	for (int i=0; i<FRAMES; i++)
	{
		float alpha = 1.0f - (float)i/(FRAMES-1);
		data2blue->setAlpha(alpha);
		saveScreenshot(frameid, rendererSet);
	}
	// middle of spin
	FRAMES = 100;
	double translation3[4] = {80.5305, 53.0221, 47.5533, 1};
	double orientation3[4] = {0.661256, 0.480013, 0.0133041, -0.576326};
	double window3 = 43.6469;
	for (int i=0; i<FRAMES; i++)
	{
		float alpha = 1.0f - (float)i/(FRAMES-1);
		double thisTranslation[4] = { alpha* translation2[0] + (1.0f-alpha)*translation3[0], alpha* translation2[1] + (1.0f-alpha)*translation3[1], alpha* translation2[2] + (1.0f-alpha)*translation3[2], alpha* translation2[3] + (1.0f-alpha)*translation3[3] };
		CCVOpenGLMath::Quaternion first(orientation2[0], orientation2[1], orientation2[2], orientation2[3]);
		CCVOpenGLMath::Quaternion second(orientation3[0], orientation3[1], orientation3[2], orientation3[3]);
		CCVOpenGLMath::Quaternion thisquat = second.slerp(first, alpha); // slerp;
		double thisOrientation[4] = {thisquat[0],thisquat[1],thisquat[2],thisquat[3]};
		double thisWindow = alpha*window2 + (1.0f-alpha)*window3;
		rendererSet->getCurrentRenderer()->setViewingParameters(thisTranslation, thisOrientation, thisWindow);
		saveScreenshot(frameid, rendererSet);
	}
	double translation4[4] = {68.3913, -22.6694, 65.8323, 1};
	double orientation4[4] = {0.265174, 0.0709244, -0.556051, -0.784512};
	double window4 = 43.6469;
	for (int i=0; i<FRAMES; i++)
	{
		float alpha = 1.0f - (float)i/(FRAMES-1);
		double thisTranslation[4] = { alpha* translation3[0] + (1.0f-alpha)*translation4[0],
									  alpha* translation3[1] + (1.0f-alpha)*translation4[1],
									  alpha* translation3[2] + (1.0f-alpha)*translation4[2],
									  alpha* translation3[3] + (1.0f-alpha)*translation4[3]
									};
		CCVOpenGLMath::Quaternion first(orientation3[0], orientation3[1], orientation3[2], orientation3[3]);
		CCVOpenGLMath::Quaternion second(orientation4[0], orientation4[1], orientation4[2], orientation4[3]);
		CCVOpenGLMath::Quaternion thisquat = second.slerp(first, alpha); // slerp;
		double thisOrientation[4] = {thisquat[0],thisquat[1],thisquat[2],thisquat[3]};
		double thisWindow = alpha*window3 + (1.0f-alpha)*window4;
		rendererSet->getCurrentRenderer()->setViewingParameters(thisTranslation, thisOrientation, thisWindow);
		saveScreenshot(frameid, rendererSet);
	}
	// hold that pose
	for (int i=0; i<FRAMES; i++)
	{
		saveScreenshot(frameid, rendererSet);
	}
}
