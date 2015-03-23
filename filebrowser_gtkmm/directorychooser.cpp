#include "directorychooser.h"
#include <iostream>
using namespace std;
DirectoryChooser::DirectoryChooser(Gtk::Widget *parent)
{

    tree.append_column("Dirs", m_Columns.m_col_name);
    //tree.append_column("Path", m_Columns.m_col_path);
    tree.set_headers_visible(true); 
    
    scwin.add(tree);
    scwin.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC); 
    model = Gtk::TreeStore::create(m_Columns);
    tree.set_model(model);
    
    tree.signal_row_expanded().connect( sigc::mem_fun(*this, &DirectoryChooser::populate_tree));
    tree.get_selection()->signal_changed().connect( sigc::mem_fun(*this, &DirectoryChooser::on_tree_selectionChanged));

    comboboxRoot.signal_changed().connect( sigc::mem_fun(*this, &DirectoryChooser::on_comboboxRoot_currentIndexChanged));
      
    comboboxRoot.append("/");
/**
    QFileInfoList drives = QDir::drives();
    for (int i = 0; i < drives.size(); ++i) {
        QFileInfo fileInfo = drives.at(i);
        comboboxRoot.append(fileInfo.filePath());
    }
*/

    Gtk::Label* label = new Gtk::Label("Drive");
    hbox.pack_start(*label, Gtk::PACK_SHRINK);
    hbox.pack_end(comboboxRoot, Gtk::PACK_SHRINK);

    pack_start(hbox, Gtk::PACK_SHRINK);
    pack_start(scwin);
    pack_start(statusbar, Gtk::PACK_SHRINK);
}
//unfortunately there is no function get_drives in gtk
#ifdef G_OS_WIN32
#include <windows.h>
#include <direct.h>
#include <stdio.h>
#include <tchar.h>
#endif
void DirectoryChooser::get_drives ()
{
#ifdef G_OS_WIN32
 //GetLogicalDrives();
#else

#endif
}

void DirectoryChooser::add_to_tree(Gtk::TreeModel::iterator& parentItem)
{
   
  //If parentItem already has children, then remove them first    
  Gtk::TreeModel::Row row = *parentItem;
  Gtk::TreeModel::iterator child = row.children().begin();	 
  while (child != row.children().end())
  {
    child = model->erase (child);
  }

  //get path
  Glib::ustring path = row.get_value(m_Columns.m_col_path);

  if (path[path.size()-1] != G_DIR_SEPARATOR)
  {
         path+=G_DIR_SEPARATOR;
  }
//         cout << "Path " << path << endl;

  try
  {
    Glib::RefPtr<Gio::File> dir = Gio::File::create_for_path(path);
    Glib::RefPtr<Gio::FileEnumerator> infos = dir->enumerate_children("standard::name");
  
    Glib::RefPtr<Gio::FileInfo> info = infos->next_file();

    while(info)
    {
      if (info->get_file_type() == Gio::FILE_TYPE_DIRECTORY)
      {
        Gtk::TreeNodeChildren children = row.children();
        Gtk::TreeModel::iterator iter = model->append(children);
        Gtk::TreeModel::Row row = *iter;
        row[m_Columns.m_col_name] = info->get_name();
        row[m_Columns.m_col_path] = path + info->get_name();  
      }
    
      info = infos->next_file();

    }
  }
  catch(const Gio::Error& ex)
  {
    std::cout << ex.what () << std::endl;
  }
 

}


void DirectoryChooser::populate_tree(const Gtk::TreeModel::iterator& iter, const Gtk::TreeModel::Path& path)
{
  Gtk::TreeModel::Row row = *iter;
  Gtk::TreeModel::iterator child = row.children().begin();	 
  while (child != row.children().end())
  {
    add_to_tree(child);
    child++;
  }
}

void DirectoryChooser::on_tree_selectionChanged()
{
    Glib::RefPtr<Gtk::TreeSelection> selection = tree.get_selection();
    Gtk::TreeModel::iterator iter = selection->get_selected();
    Gtk::TreeModel::Row row = *iter;
    m_choosenDirectoryChanged.emit(row.get_value(m_Columns.m_col_path));
}

void DirectoryChooser::on_comboboxRoot_currentIndexChanged()
{
    
    model->clear();
    
    Glib::ustring path = comboboxRoot.get_active_text();
    
    if(path.empty()) return;

    Gtk::TreeModel::iterator iter = model->append();

    Gtk::TreeModel::Row row = *iter;
    row[m_Columns.m_col_name] = path;
    row[m_Columns.m_col_path] = path;

    add_to_tree(iter);
   
}

DirectoryChooser::type_signal_cdc DirectoryChooser::choosenDirectoryChanged()
{
  return m_choosenDirectoryChanged;
}
