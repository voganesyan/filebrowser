cmake_minimum_required (VERSION 2.8.11)

project (filebrowser)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

#Qt
#set(SRC_DIR filebrowser_qt/)
#set(CMAKE_AUTOMOC ON)
#find_package(Qt5Widgets)

#Gtkmm
find_package(PkgConfig)

pkg_check_modules(GTKMM gtkmm-2.4)
link_directories(
    ${GTKMM_LIBRARY_DIRS}  )

include_directories(
    ${GTKMM_INCLUDE_DIRS}  )

set(SRC_DIR filebrowser_gtkmm/)
#subdirs(filebrowser_gtkmm)

add_executable (filebrowser ${SRC_DIR}directorychooser.cpp
                            ${SRC_DIR}directoryscanner.cpp
                            ${SRC_DIR}mainwindow.cpp
                            ${SRC_DIR}main.cpp)

include_directories(filecleaner)
add_subdirectory (filecleaner)
target_link_libraries (filebrowser LINK_PUBLIC filecleaner)

#Qt
#target_link_libraries(filebrowser Qt5::Widgets)

#Gtkmm
target_link_libraries(filebrowser ${GTKMM_LIBRARIES})
