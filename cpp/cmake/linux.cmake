
find_package(wxWidgets COMPONENTS core base gl adv)
include(${wxWidgets_USE_FILE})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_FILE_OFFSET_BITS=64 -D__WXGTK__")

set(WX_CUSTOM_INC_PATH ${REPO_DIR}/cpp/externals/wxwidgets/include)
set(WX_CUSTOM_LIB_PATH ${REPO_DIR}/cpp/externals/wxwidgets/lib)


set(PLATFORM_LIBRARIES pthread
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
                       SM)
