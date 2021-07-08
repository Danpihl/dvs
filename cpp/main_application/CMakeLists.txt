cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

project(main-app)

include_directories(${REPO_DIR}/cpp/main_application)
include_directories(${REPO_DIR}/cpp/dvs)
include_directories(${REPO_DIR}/cpp/communication)

set(CPP_SOURCE_FILES main.cpp
                     opengl_low_level/3d/opengl_3d_functions.cpp
                     opengl_low_level/opengl_text.cpp
                     opengl_low_level/2d/opengl_2d_functions.cpp
                     opengl_low_level/2d/opengl_2d_shape_primitives.cpp
                     io_devices/keyboard_state.cpp
                     io_devices/mouse_state.cpp
                     axes/axes_interactor.cpp
                     axes/axes_painter.cpp

                     axes/structures/axes_limits.cpp
                     axes/structures/axes_settings.cpp
                     axes/structures/coordinate_converter.cpp
                     axes/structures/view_angles.cpp

                     axes/plot_box/plot_box_coordinate_arrows.cpp
                     axes/plot_box/plot_box_grid_numbers.cpp
                     axes/plot_box/plot_box_grid.cpp
                     axes/plot_box/plot_box_outline.cpp
                     axes/plot_box/plot_box_walls.cpp

                     misc/number_formatting.cpp
                     misc/color_map.cpp

                     tab_view.cpp
                     window_view.cpp
                     events.cpp
                     layout_tools_window.cpp
                     plot_data.cpp
                     plot_functions/plot_functions.cpp
                     plot_window_gl_pane.cpp
                     main_window.cpp
                     main_window_receive.cpp
                     main_window_gui.cpp)

set(EXTRA_LIBRARIES -lexpat
                    -lgthread-2.0
                    -pthread
                    -lX11
                    -lXxf86vm
                    -lSM
                    -lgtk-x11-2.0
                    -lgdk-x11-2.0
                    -lpangocairo-1.0
                    -latk-1.0
                    -lcairo
                    -lgdk_pixbuf-2.0
                    -lgio-2.0
                    -lXtst
                    -lpangoft2-1.0
                    -lpango-1.0
                    -lgobject-2.0
                    -lglib-2.0
                    -lfontconfig
                    -lfreetype
                    -lpng
                    -ljpeg
                    -ltiff
                    -ljbig
                    -lz
                    -ldl
                    -lcurl
                    -llzma
                    -lm)

add_executable(dvs ${CPP_SOURCE_FILES})
target_link_libraries(dvs ${wxWidgets_LIBRARIES}
                          ${OPENGL_LIBRARIES}
                          ${GLUT_LIBRARY}
                          pthread
                          png
                          X11
                          Xxf86vm
                          Xext
                          ${CMAKE_DL_LIBS}
                          gtk-x11-2.0
                          gdk-x11-2.0
                          stdc++fs
                          pango-1.0
                          pangocairo-1.0
                          cairo
                          gdk_pixbuf-2.0
                          glib-2.0
                          gio-2.0
                          gobject-2.0
                          SM
                          ${wxWidgets_LIBRARIES}
                          )
                          # iconv
                          # "-framework System"
                          # "-framework IOKit"
                          # "-framework Carbon"
                          # "-framework Cocoa"
                          # "-framework QuartzCore")<

                          


