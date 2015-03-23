link_directories(
    ${GTKMM_LIBRARY_DIRS}  )

include_directories(
    ${GTKMM_INCLUDE_DIRS}  )

add_executable(filebrowser
  directorychooser.cpp
  directoryscanner.cpp
  mainwindow.cpp
  main.cpp)

target_link_libraries(filebrowser LINK_PUBLIC
    ${GTKMM_LIBRARIES}  filecleaner)
