

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/Users/annotelldaniel/work/repos/dvs/cpp/externals/wxwidgets/lib/wx/include/osx_cocoa-unicode-static-3.1 -I/Users/annotelldaniel/work/repos/dvs/cpp/externals/wxwidgets/include -D_FILE_OFFSET_BITS=64 -D__WXMAC__ -D__WXOSX__ -D__WXOSX_COCOA__")

link_directories(/usr/local/Cellar/libpng/1.6.37/lib)


set(WX_CUSTOM_INC_PATH /Users/annotelldaniel/work/repos/dvs/cpp/externals/wxwidgets/include)
set(WX_CUSTOM_LIB_PATH /Users/annotelldaniel/work/repos/dvs/cpp/externals/wxwidgets/lib)


set(wxWidgets_LIBRARIES -lwx_osx_cocoau_core-3.1
                        -lwx_baseu-3.1
                        -lwx_osx_cocoau_gl-3.1
                        -lwx_osx_cocoau_adv-3.1)