#include "mainwindow.h"
#include <gtkmm.h>

int main(int argc, char *argv[])
{
    Gtk::Main kit(argc, argv);

    MainWindow w;
    w.show_all();

    Gtk::Main::run(w);
    
    return 0;
}
