/*
  Copyright 2011 The University of Texas at Austin

	Advisor: Chandrajit Bajaj <bajaj@cs.utexas.edu>

  This file is part of TexMol.

  TexMol is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License version 2.1 as published by the Free Software Foundation.

  TexMol is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include <Qt3Support>	//by cha

#include <TexMol/Dialogs/MainWindow.h>
#include <TexMol/Server.h>
#include <TexMol/Test.h>

#include <cmath>
#include <qapplication.h>

int main(int argc, char** argv)
{
	//TexMol::test();
	//return 0;
	// Allowing both UI and batch mode for the program
	// By default, we open the UI.
    if (argc == 1)
     {
      QApplication app(argc, argv);
      MainWindow mainwindow;
      app.setMainWidget(&mainwindow);
      mainwindow.show();
      return app.exec();
       }
      else
      {
      // // 8-16-2011
      // // arand: batch mode has been unsupported for at least a year now
      // //        all functions have been moved to MolSurf/MolEnergy
      // //        TexMol is only the GUI interface
      //
      Server server;
      if (server.execute(argc, argv, NULL))
      {
        return 0;
      }
      return -1;
     }
}
