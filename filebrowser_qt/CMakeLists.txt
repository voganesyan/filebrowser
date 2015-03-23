add_executable(filebrowser
  directorychooser.cpp
  directoryscanner.cpp
  mainwindow.cpp
  main.cpp)

target_link_libraries(filebrowser LINK_PUBLIC
    Qt5::Widgets filecleaner)

