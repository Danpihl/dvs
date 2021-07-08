
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_FILE_OFFSET_BITS=64 -D__WXGTK__")

set(WX_CUSTOM_INC_PATH ${REPO_DIR}/cpp/externals/wxwidgets/include)
set(WX_CUSTOM_LIB_PATH ${REPO_DIR}/cpp/externals/wxwidgets/lib)


# set(wxWidgets_LIBRARIES ${REPO_DIR}/cpp/externals/wxwidgets/lib/libwx_gtk2u-3.1.a
# ${REPO_DIR}/cpp/externals/wxwidgets/lib/libwxregexu-3.1.a)

set(wxWidgets_LIBRARIES -lwx_gtk2u_xrc-3.1
                        -lwx_gtk2u_qa-3.1
                        -lwx_baseu_net-3.1
                        -lwx_gtk2u_html-3.1
                        -lwx_gtk2u_core-3.1
                        -lwx_baseu_xml-3.1
                        -lwx_baseu-3.1
                        -lwxregexu-3.1
                        -lwxscintilla-3.1
                        -lwx_gtk2u_gl-3.1
                        -lwx_gtk2u_adv-3.1
                        -lwx_gtk2u_aui-3.1
                        -lwx_gtk2u_propgrid-3.1
                        -lwx_gtk2u_ribbon-3.1
                        -lwx_gtk2u_richtext-3.1
                        -lwx_gtk2u_stc-3.1)



