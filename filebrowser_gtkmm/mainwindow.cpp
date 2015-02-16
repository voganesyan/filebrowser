#include "mainwindow.h"
#include "directorychooser.h"
#include "directoryscanner.h"

#include <gtkmm.h>

MainWindow::MainWindow()
{
  DirectoryChooser* chooser = new DirectoryChooser(this);
  DirectoryScanner* scanner = new DirectoryScanner(this);
              
  chooser->choosenDirectoryChanged().connect( sigc::mem_fun(scanner, &DirectoryScanner::scanDirectory));

  paned.add1(*chooser);
  paned.add2(*scanner);
  add(paned);
}

MainWindow::~MainWindow()
{

}
