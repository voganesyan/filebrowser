#include "directoryscanner.h"

#include <iostream>
#include "filecleaner.h"

using namespace std;
DirectoryScanner::DirectoryScanner(Gtk::Widget *parent)
{
    list.append_column("File", m_Columns.m_col_name);
    list.append_column("Size", m_Columns.m_col_size);
    list.set_headers_visible(true);  

    
    scwin.add(list);
    model = Gtk::ListStore::create(m_Columns);

    lineeditPath.signal_activate().connect( sigc::mem_fun(*this, &DirectoryScanner::on_lineedit_returnPressed));
    lineeditFilter.signal_activate().connect( sigc::mem_fun(*this, &DirectoryScanner::on_lineedit_returnPressed));
    
    clearButton = new Gtk::Button("Clear");
    clearButton->signal_clicked().connect( sigc::mem_fun(*this, &DirectoryScanner::on_clearButton_clicked));
    
    //statusbar->addPermanentWidget(clearButton);


    Gtk::Label* label = new Gtk::Label("Directory");
    hboxTop.pack_start(*label, Gtk::PACK_SHRINK);
    hboxTop.pack_start(lineeditPath);
                label = new Gtk::Label("Filter");
    hboxTop.pack_start(*label, Gtk::PACK_SHRINK);
    hboxTop.pack_start(lineeditFilter, Gtk::PACK_SHRINK);
    
    hboxBottom.pack_start(statusbar);
    hboxBottom.pack_end(*clearButton, Gtk::PACK_SHRINK);

    pack_start(hboxTop, Gtk::PACK_SHRINK);
    pack_start(scwin);
    pack_start(hboxBottom, Gtk::PACK_SHRINK);
}



void DirectoryScanner::scanDirectory(const string& path)
{
   lineeditPath.set_text(path);
   on_lineedit_returnPressed();
}


void DirectoryScanner::on_lineedit_returnPressed()
{
    string path = lineeditPath.get_text();
    string extension = lineeditFilter.get_text();
    //!!! validate path
    vector<FC_FileInfo> files;

    FC_GetFilesInDirectory(files, path, extension);

    list.unset_model ();

    model->clear();

    for (vector<FC_FileInfo>::iterator it = files.begin() ; it != files.end(); ++it)
    {
        FC_FileInfo info = *it;

        Gtk::TreeModel::iterator iter = model->append();
        Gtk::TreeModel::Row row = *iter;
        row[m_Columns.m_col_name] = info.name;
        row[m_Columns.m_col_size] = size2string(info.size);        
    }

    list.set_model (model);
}


void DirectoryScanner::on_clearButton_clicked()
{
    Glib::RefPtr<Gtk::TreeSelection> selection = list.get_selection();
    Gtk::TreeModel::iterator iter = selection->get_selected();
    Gtk::TreeModel::Row row = *iter;
    Glib::ustring name = row.get_value(m_Columns.m_col_name);

   //if (item_name == NULL) return;
//Glib::ustring
   string clearingFilePath = lineeditPath.get_text() + G_DIR_SEPARATOR + name;

   string error_message;
   if (FC_ClearFile(clearingFilePath, error_message))
     row[m_Columns.m_col_size] = size2string(0);
   else
   {
     statusbar.remove_all_messages (0);
     statusbar.push(error_message, 0);
   }
}


#include <string>
std::string  DirectoryScanner::size2string (unsigned long bytes)
{
size_t kilobyte = 1024;
size_t megabyte = kilobyte * 1024;
size_t gigabyte = megabyte * 1024;
size_t terabyte = gigabyte * 1024;

if ((bytes >= 0) && (bytes < kilobyte)) {
return std::to_string(bytes) + " B";
}
else if ((bytes >= kilobyte) && (bytes < megabyte)) {
return std::to_string( bytes / kilobyte ) + " KB";
}
else if ((bytes >= megabyte) && (bytes < gigabyte)) {
return std::to_string( bytes / megabyte ) + " MB";
}
else if ((bytes >= gigabyte) && (bytes < terabyte)) {
return std::to_string( bytes / gigabyte) + " GB";
}
else if (bytes >= terabyte) {
return std::to_string( bytes / terabyte) + " TB";
} else {
return std::to_string( bytes ) + " B";
}
}

