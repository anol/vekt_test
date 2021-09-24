cmake_minimum_required(VERSION 3.16)
project(Pinator_test)

set(CMAKE_CXX_STANDARD 14)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED IMPORTED_TARGET gtkmm-3.0)
pkg_check_modules(CAIRO REQUIRED IMPORTED_TARGET cairomm-1.0)
#pkg_check_modules(GLIB REQUIRED IMPORTED_TARGET GLIBC_2.4)

set(CMAKE_THREAD_LIBS_INIT "-lpthread")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_HAVE_THREADS_LIBRARY 1)

add_executable(Pinator_test main.cpp My_window.cpp My_window.h My_area.cpp My_area.h My_worker.cpp My_worker.h)

link_directories(/usr/local/include)
link_directories(/usr/local/lib)

include_directories(${GTK_INCLUDE_DIRS})
link_directories(${GTK_LIBRARY_DIRS})
target_link_libraries(Pinator_test  ${GTK_LIBRARIES})

include_directories(${CAIRO_INCLUDE_DIRS})
link_directories(${CAIRO_LIBRARY_DIRS})
target_link_libraries(Pinator_test  ${CAIRO_LIBRARIES} )

#target_link_libraries(Pinator_test  ${GLIB_LIBRARIES} )


# wx_test.cpp
#target_include_directories(Pinator_test PUBLIC "${PROJECT_BINARY_DIR}"
#        /home/pi/Downloads/wxWidgets-3.0.5/include
#        )
#
#link_directories(/home/pi/Downloads/wxWidgets-3.0.5/lib)
