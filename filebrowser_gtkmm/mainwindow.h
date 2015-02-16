#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <gtkmm.h>


class MainWindow : public Gtk::Window
{

public:
    MainWindow();
    ~MainWindow();
    
protected:
Gtk::HPaned paned;
};

#endif // MAINWINDOW_H
