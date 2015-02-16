#ifndef DIRECTORYCHOOSER_H
#define DIRECTORYCHOOSER_H

#include <gtkmm.h>
#include <sigc++/sigc++.h>

class DirectoryChooser : public Gtk::VBox
{
public:
    DirectoryChooser(Gtk::Widget *parent = 0);
    //signal accessor:
    typedef sigc::signal<void, const std::string&> type_signal_cdc;
    type_signal_cdc choosenDirectoryChanged();
    
protected:
    type_signal_cdc m_choosenDirectoryChanged;

private:
    void on_comboboxRoot_currentIndexChanged();
    void on_tree_selectionChanged();
    void populate_tree(const Gtk::TreeModel::iterator& iter, const Gtk::TreeModel::Path& path);
    void add_to_tree(Gtk::TreeModel::iterator& parentItem);
    void get_drives();

    
Gtk::TreeView tree;
Glib::RefPtr<Gtk::TreeStore> model;
Gtk::Statusbar statusbar;
Gtk::HBox hbox;
Gtk::ScrolledWindow scwin;
Gtk::ComboBoxText comboboxRoot;

class ModelColumns : public Gtk::TreeModelColumnRecord
{
public:

  ModelColumns()
    { add(m_col_name); add(m_col_path); }

  Gtk::TreeModelColumn<Glib::ustring> m_col_name;
  Gtk::TreeModelColumn<Glib::ustring> m_col_path;
};

ModelColumns m_Columns;
};

#endif // DIRECTORYCHOOSER_H
