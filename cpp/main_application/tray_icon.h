#ifndef MAIN_APPLICATION_TRAY_ICON_H_
#define MAIN_APPLICATION_TRAY_ICON_H_

#include <wx/taskbar.h>
#include <wx/wx.h>

#include <iostream>
#include <map>
#include <functional>

#include "constants.h"

enum
{
    PU_EXIT = 10001,
    PU_SHOW_MAIN_WINDOW,
    PU_SHOW_SUB_WINDOW,
    PU_EDIT_LAYOUT,
    PU_FILE,
    PU_FILE_NEW,
    PU_FILE_OPEN,
    PU_FILE_SAVE,
    PU_FILE_SAVE_AS,
    PU_FIRST_WINDOW_ID,
    PU_LAST_WINDOW_ID = PU_FIRST_WINDOW_ID + kMaxNumWindows - 1,
    PU_SUBMAIN
};

class CustomTaskBarIcon : public wxTaskBarIcon
{
public:
#if defined(__WXOSX__) && wxOSX_USE_COCOA
    CustomTaskBarIcon(wxTaskBarIconType iconType = wxTBI_DEFAULT_TYPE);
#else
    CustomTaskBarIcon();
#endif

    int getNextFreeId()
    {
        const int val = free_ids_.back();
        free_ids_.pop_back();
        return val;
    }

    void freeId(int id_to_free)
    {
        taken_ids_.erase(std::find(taken_ids_.begin(), taken_ids_.end(), id_to_free));
        free_ids_.push_back(id_to_free);
    }

    void setOnExitCallback(std::function<void()> exit_function);

    void OnMenuExit(wxCommandEvent&);
    void OnMainWindow(wxCommandEvent& evt);
    void OnSubWindow(wxCommandEvent& evt);
    void addNewWindow(const std::string& window_name);
    void removeWindow(const std::string& window_name);
    virtual wxMenu *CreatePopupMenu();

    wxDECLARE_EVENT_TABLE();

private:
    std::function<void()> exit_function_;
    std::vector<int> free_ids_;
    std::vector<int> taken_ids_;
    wxMenu *windows_submenu_;
    wxMenu *menu_;
    wxMenu *file_submenu_;

    std::map<std::string, wxEventTypeTag<wxCommandEvent>> window_events_;
    std::map<std::string, int> window_event_ids_;

};


#endif
