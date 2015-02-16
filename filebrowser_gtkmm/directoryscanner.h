#ifndef DIRECTORYSCANNER_H
#define DIRECTORYSCANNER_H

#include <gtkmm.h>

class DirectoryScanner : public Gtk::VBox
{

public:
    DirectoryScanner(Gtk::Widget *parent = 0);
    void scanDirectory(const std::string &path);


private:
    void on_lineedit_returnPressed();
    void on_clearButton_clicked ();
    std::string size2string(unsigned long bytes);

Glib::RefPtr<Gtk::ListStore> model;
Gtk::TreeView list;
Gtk::Entry lineeditPath;
Gtk::Entry lineeditFilter;
Gtk::Statusbar statusbar;
Gtk::Button* clearButton;
Gtk::HBox hboxTop;
Gtk::HBox hboxBottom;
Gtk::ScrolledWindow scwin;

class ModelColumns : public Gtk::TreeModelColumnRecord
{
public:

  ModelColumns()
    { add(m_col_name); add(m_col_size); }

  Gtk::TreeModelColumn<Glib::ustring> m_col_name;
  Gtk::TreeModelColumn<Glib::ustring> m_col_size;
};

ModelColumns m_Columns;
};


#endif // DIRECTORYSCANNER_H
