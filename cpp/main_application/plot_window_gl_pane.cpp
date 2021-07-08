#include "plot_window_gl_pane.h"


#include <wx/event.h>
#include <wx/glcanvas.h>

#include "math/math.h"
#include "axes/axes.h"
#include "io_devices/io_devices.h"
#include "opengl_low_level/opengl_low_level.h"
#include "events.h"
#include "enumerations.h"

using namespace dvs::internal;

CursorSquareState mouseState(const Bound2Df bound, const Bound2Df bound_margin, const Vec2Df mouse_pos)
{
    if((bound.x_min <= mouse_pos.x) && (mouse_pos.x <= bound.x_max) &&
        (bound.y_min <= mouse_pos.y) && (mouse_pos.y <= bound.y_max))
    {
        if(mouse_pos.x <= bound_margin.x_min)
        {
            if(mouse_pos.y <= bound_margin.y_min)
            {
                return CursorSquareState::TOP_LEFT;
            }
            else if(bound_margin.y_max <= mouse_pos.y)
            {
                return CursorSquareState::BOTTOM_LEFT;
            }
            else
            {
                return CursorSquareState::LEFT;
            }
        }
        else if(bound_margin.x_max <= mouse_pos.x)
        {
            if(mouse_pos.y <= bound_margin.y_min)
            {
                return CursorSquareState::TOP_RIGHT;
            }
            else if(bound_margin.y_max <= mouse_pos.y)
            {
                return CursorSquareState::BOTTOM_RIGHT;
            }
            else
            {
                return CursorSquareState::RIGHT;
            }
        }
        else if(mouse_pos.y <= bound_margin.y_min)
        {
            return CursorSquareState::TOP;
        }
        else if(bound_margin.y_max <= mouse_pos.y)
        {
            return CursorSquareState::BOTTOM;
        }
        else
        {
            return CursorSquareState::INSIDE;
        }
    }
    else
    {
        return CursorSquareState::OUTSIDE;
    }
}

PlotWindowGLPane::PlotWindowGLPane(wxWindow* parent, const ElementSettings& element_settings, const float grid_size)
    : wxGLCanvas(parent, wxID_ANY, getArgsPtr(), wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE), GuiElement(element_settings)
{
    // wxGLContextAttrs cxtAttrs;
    // cxtAttrs.PlatformDefaults().OGLVersion(99, 2).EndList(); // Doesn't work on linux!!
    // https://stackoverflow.com/questions/41145024/wxwidgets-and-modern-opengl-3-3
    // m_context = new wxGLContext(this, NULL, &cxtAttrs);
    m_context = new wxGLContext(this);

    is_editing_ = false;
    parent_size_ = parent->GetSize();
    grid_size_ = grid_size;
    edit_size_margin_ = 20.0f;

    SetBackgroundStyle(wxBG_STYLE_CUSTOM);

    const float min_x = -1.0;
    const float max_x = 1.0;
    const float min_y = -1.0;
    const float max_y = 1.0;
    const float min_z = -1.0;
    const float max_z = 1.0;

    view_parent_ = dynamic_cast<SuperBase*>(parent);

    is_selected_ = false;

    const AxesSettings axes_settings({min_x, min_y, min_z}, {max_x, max_y, max_z});

    bindCallbacks();

    axes_interactor_ = new AxesInteractor(axes_settings, getWidth(), getHeight());
    axes_painter_ = new AxesPainter(axes_settings);

    hold_on_ = true;
    axes_set_ = false;
    view_set_ = false;

    glEnable(GL_MULTISAMPLE);

    glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);
}

void PlotWindowGLPane::updateSizeFromParent(const wxSize& parent_size)
{
    parent_size_ = parent_size;
    const float px = parent_size_.GetWidth();
    const float py = parent_size_.GetHeight();

    const float xpos = px * element_settings_.x;
    const float ypos = py * element_settings_.y;

    const float width = px * element_settings_.width;
    const float height = py * element_settings_.height;

    this->setPosition(wxPoint(xpos, ypos));
    this->setSize(wxSize(width, height));
}

void PlotWindowGLPane::setPosition(const wxPoint& new_pos)
{
    this->SetPosition(new_pos);
}

void PlotWindowGLPane::setSize(const wxSize& new_size)
{
    axes_painter_->setWindowSize(new_size.GetWidth(), new_size.GetHeight());


    this->SetSize(new_size);
}

void PlotWindowGLPane::show()
{
    this->Show();
}

void PlotWindowGLPane::hide()
{
    this->Hide();
}

void PlotWindowGLPane::bindCallbacks()
{
    Bind(wxEVT_MOTION, &PlotWindowGLPane::mouseMoved, this);
    Bind(wxEVT_LEFT_DOWN, &PlotWindowGLPane::mouseLeftPressed, this);
    Bind(wxEVT_LEFT_UP, &PlotWindowGLPane::mouseLeftReleased, this);
    Bind(wxEVT_KEY_DOWN, &PlotWindowGLPane::keyPressed, this);
    Bind(wxEVT_KEY_UP, &PlotWindowGLPane::keyReleased, this);
    Bind(wxEVT_PAINT, &PlotWindowGLPane::render, this);
    Bind(wxEVT_LEAVE_WINDOW, &PlotWindowGLPane::mouseLeftWindow, this);
}

void PlotWindowGLPane::setPosAndSize(const wxPoint& pos, const wxSize& size)
{
    this->SetPosition(pos);
    this->SetSize(size);
}

int* PlotWindowGLPane::getArgsPtr()
{
    args[0] = WX_GL_RGBA;
    args[1] = WX_GL_DOUBLEBUFFER;
    args[2] = WX_GL_DEPTH_SIZE;
    args[3] = 16;
    args[4] = WX_GL_SAMPLES;
    args[5] = 4;
    args[6] = WX_GL_SAMPLE_BUFFERS;
    args[7] = 1;
    args[8] = 0;

    return args;
}

PlotWindowGLPane::~PlotWindowGLPane()
{
    delete m_context;
}

void PlotWindowGLPane::addData(std::unique_ptr<const ReceivedData> received_data, const dvs::internal::FunctionHeader& hdr)
{
    const internal::Function fcn = hdr.getObjectAtIdx(0).getAs<internal::Function>();

    if (fcn == Function::HOLD_ON)
    {
        hold_on_ = true;
    }
    else if (fcn == Function::HOLD_OFF)
    {
        hold_on_ = false;
    }
    else if (fcn == Function::AXES_2D)
    {
        axes_set_ = true;

        const AxesBounds axes_bnd = hdr.getObjectFromType(FunctionHeaderObjectType::AXIS_MIN_MAX_VEC).getAs<AxesBounds>();
        axes_interactor_->setAxesLimits(Vec2Dd(axes_bnd.lower.x, axes_bnd.lower.y),
                                        Vec2Dd(axes_bnd.upper.x, axes_bnd.upper.y));
    }
    else if (fcn == Function::AXES_3D)
    {
        axes_set_ = true;

        const AxesBounds axes_bnd = hdr.getObjectFromType(FunctionHeaderObjectType::AXIS_MIN_MAX_VEC).getAs<AxesBounds>();
        axes_interactor_->setAxesLimits(Vec3Dd(axes_bnd.lower.x, axes_bnd.lower.y, axes_bnd.lower.z),
                                        Vec3Dd(axes_bnd.upper.x, axes_bnd.upper.y, axes_bnd.upper.z));
    }
    else if (fcn == Function::VIEW)
    {
        view_set_ = true;
        const float azimuth = hdr.getObjectFromType(internal::FunctionHeaderObjectType::AZIMUTH).getAs<float>();
        const float elevation = hdr.getObjectFromType(internal::FunctionHeaderObjectType::ELEVATION).getAs<float>();

        const float azimuth_rad = azimuth * M_PI / 180.0f;
        const float elevation_rad = elevation * M_PI / 180.0f;

        axes_interactor_->setViewAngles(azimuth_rad, elevation_rad);
    }
    else if (fcn == Function::CLEAR)
    {
        axes_set_ = false;
        hold_on_ = true;
        view_set_ = false;

        plot_data_handler_.clear();
        axes_interactor_->setViewAngles(0, M_PI);
        axes_interactor_->setAxesLimits(Vec3Dd(-1.0, -1.0, -1.0),
                                        Vec3Dd(1.0, 1.0, 1.0));
    }
    else if (fcn == Function::SOFT_CLEAR)
    {
        plot_data_handler_.softClear();
    }
    else
    {
        if (!hold_on_)
        {
            plot_data_handler_.clear();
        }
        plot_data_handler_.addData(std::move(received_data), hdr);

        if (!axes_set_)
        {
            const std::pair<Vec3Dd, Vec3Dd> min_max = plot_data_handler_.getMinMaxVectors();
            const Vec3Dd mean_vec = (min_max.second + min_max.first) / 2.0;

            const Vec3Dd min_vec = (min_max.first - mean_vec) * 1.001 + mean_vec;
            const Vec3Dd max_vec = (min_max.second - mean_vec) * 1.001 + mean_vec;

            axes_interactor_->setAxesLimits(min_vec, max_vec);
        }
        if (!view_set_)
        {
            if(is3DFunction(fcn))
            {
                const float azimuth = -64.0f * M_PI / 180.0f;
                const float elevation = 34.0f * M_PI / 180.0f;

                axes_interactor_->setViewAngles(azimuth, elevation);
            }
            else if(isImageFunction(fcn))
            {
                const float azimuth = 0.0f * M_PI / 180.0f;
                const float elevation = -90.0f * M_PI / 180.0f;

                axes_interactor_->setViewAngles(azimuth, elevation);
                view_set_ = true; // Let imshow be dominant once used
            }
            else
            {
                const float azimuth = 0.0f * M_PI / 180.0f;
                const float elevation = 90.0f * M_PI / 180.0f;

                axes_interactor_->setViewAngles(azimuth, elevation);
            }
        }

    }

    Refresh();
}

bool PlotWindowGLPane::isImageFunction(const Function fcn)
{
    return fcn == Function::IM_SHOW;
}

bool PlotWindowGLPane::is3DFunction(const Function fcn)
{
    return (fcn == Function::DRAW_LINE3D) ||
           (fcn == Function::PLANE_XY) ||
           (fcn == Function::PLANE_XZ) ||
           (fcn == Function::PLANE_YZ) ||
           (fcn == Function::DRAW_MESH) ||
           (fcn == Function::DRAW_TRIANGLES_3D) ||
           (fcn == Function::DRAW_TRIANGLE_3D) ||
           (fcn == Function::SURF) ||
           (fcn == Function::PLOT3) ||
           (fcn == Function::SCATTER3) ||
           (fcn == Function::DRAW_LINE_BETWEEN_POINTS_3D) ||
           (fcn == Function::POLYGON_FROM_4_POINTS) ||
           (fcn == Function::DRAW_TRIANGLES_3D);
}

void PlotWindowGLPane::setSelection()
{
    is_selected_ = true;
}

void PlotWindowGLPane::resetSelection()
{
    is_selected_ = false;
}

void PlotWindowGLPane::destroy()
{
    this->Destroy();
}

void PlotWindowGLPane::mouseLeftPressed(wxMouseEvent& event)
{
    view_parent_->resetSelectionForAllChildren();
    wxCommandEvent evt(MY_EVENT, GetId());
    evt.SetEventObject(this);
    evt.SetString(element_settings_.name);
    ProcessWindowEvent(evt);

    is_selected_ = true;

    const wxPoint current_point = event.GetPosition();

    Bound2Df bnd;
    bnd.x_min = 0.0f;
    bnd.x_max = this->GetSize().GetWidth();
    bnd.y_min = 0.0f;
    bnd.y_max = this->GetSize().GetHeight();

    Bound2Df bnd_margin;
    bnd_margin.x_min = bnd.x_min + edit_size_margin_;
    bnd_margin.x_max = bnd.x_max - edit_size_margin_;
    bnd_margin.y_min = bnd.y_min + edit_size_margin_;
    bnd_margin.y_max = bnd.y_max - edit_size_margin_;

    cursor_state_at_press_ = mouseState(bnd, bnd_margin, Vec2Df(current_point.x, current_point.y));

    mouse_pos_at_press_ = Vec2Df(current_point.x, current_point.y);
    pos_at_press_ = this->GetPosition();
    size_at_press_ = this->GetSize();

    left_mouse_button_.setIsPressed(current_point.x, current_point.y);
    Refresh();
}

void PlotWindowGLPane::mouseLeftReleased(wxMouseEvent& event)
{
    left_mouse_button_.setIsReleased();
    Refresh();
}

void PlotWindowGLPane::mouseLeftWindow(wxMouseEvent& event)
{
    wxSetCursor(wxCursor(wxCURSOR_ARROW));
}

void PlotWindowGLPane::notifyParentAboutModification()
{
    wxCommandEvent parent_event(GUI_ELEMENT_CHANGED_EVENT);
    wxPostEvent(GetParent(), parent_event);
}

void PlotWindowGLPane::mouseMoved(wxMouseEvent& event)
{
    const wxPoint current_point = event.GetPosition();

    if (left_mouse_button_.isPressed())
    {
        if(is_editing_)
        {
            const Vec2Df mouse_pos = Vec2Df(current_point.x, current_point.y);
            Vec2Df delta = mouse_pos - mouse_pos_at_press_;

            delta = Vec2Df(std::round(delta.x / grid_size_) * grid_size_,
                           std::round(delta.y / grid_size_) * grid_size_);
            const Vec2Df current_pos(this->GetPosition().x, this->GetPosition().y);
            const Vec2Df changed_pos = current_pos + delta;

            const wxSize size_now = this->GetSize();

            wxPoint new_position = this->GetPosition();
            wxSize new_size = this->GetSize();

            switch(cursor_state_at_press_)
            {
                case CursorSquareState::LEFT:
                    new_position = wxPoint(changed_pos.x, pos_at_press_.y);
                    new_size = wxSize(size_now.GetWidth() - delta.x, size_now.GetHeight());
                    break;
                case CursorSquareState::RIGHT:
                    new_size = wxSize(size_at_press_.GetWidth() + delta.x, size_at_press_.GetHeight());
                    break;
                case CursorSquareState::TOP:
                    new_position = wxPoint(pos_at_press_.x, changed_pos.y);
                    new_size = wxSize(size_now.GetWidth(), size_now.GetHeight() - delta.y);
                    break;
                case CursorSquareState::BOTTOM:
                    new_size = wxSize(size_at_press_.GetWidth(), size_at_press_.GetHeight() + delta.y);
                    break;
                case CursorSquareState::BOTTOM_RIGHT:
                    new_size = wxSize(size_at_press_.GetWidth() + delta.x, size_at_press_.GetHeight() + delta.y);
                    break;
                case CursorSquareState::BOTTOM_LEFT:
                    new_position = wxPoint(changed_pos.x, pos_at_press_.y);
                    new_size = wxSize(size_now.GetWidth() - delta.x, size_at_press_.GetHeight() + delta.y);
                    break;
                case CursorSquareState::TOP_RIGHT:
                    new_position = wxPoint(pos_at_press_.x, changed_pos.y);
                    new_size = wxSize(size_at_press_.GetWidth() + delta.x, size_now.GetHeight() - delta.y);
                    break;
                case CursorSquareState::TOP_LEFT:
                    new_position = wxPoint(changed_pos.x, changed_pos.y);
                    new_size = wxSize(size_now.GetWidth() - delta.x, size_now.GetHeight() - delta.y);
                    break;
                case CursorSquareState::INSIDE:
                    new_position = wxPoint(changed_pos.x, changed_pos.y);
                    new_size = size_at_press_;
                    break;
                default:
                    std::cout << "Do nothing..." << std::endl;
            }
            new_size.SetWidth(std::max(50, new_size.GetWidth()));
            new_size.SetHeight(std::max(50, new_size.GetHeight()));

            const float px = parent_size_.GetWidth();
            const float py = parent_size_.GetHeight();

            element_settings_.width = static_cast<float>(new_size.GetWidth()) / px;
            element_settings_.height = static_cast<float>(new_size.GetHeight()) / py;
            element_settings_.x = static_cast<float>(new_position.x) / px;
            element_settings_.y = static_cast<float>(new_position.y) / py;

            if((this->GetPosition().x != new_position.x) || (this->GetPosition().y != new_position.y) || 
               (new_size.GetWidth() != this->GetSize().GetWidth()) || (new_size.GetHeight() != this->GetSize().GetHeight()))
            {
                notifyParentAboutModification();
                this->setPosition(new_position);
                this->setSize(new_size);
            }
        }
        else
        {
            left_mouse_button_.updateOnMotion(current_point.x, current_point.y);
            axes_interactor_->registerMouseDragInput(left_mouse_button_.getDeltaPos().x,
                                                     left_mouse_button_.getDeltaPos().y);
        }

        Refresh();
    }
    else
    {
        if(is_editing_)
        {
            Bound2Df bnd;
            bnd.x_min = 0.0f;
            bnd.x_max = this->GetSize().GetWidth();
            bnd.y_min = 0.0f;
            bnd.y_max = this->GetSize().GetHeight();

            Bound2Df bnd_margin;
            bnd_margin.x_min = bnd.x_min + edit_size_margin_;
            bnd_margin.x_max = bnd.x_max - edit_size_margin_;
            bnd_margin.y_min = bnd.y_min + edit_size_margin_;
            bnd_margin.y_max = bnd.y_max - edit_size_margin_;
            const CursorSquareState cms = mouseState(bnd, bnd_margin, Vec2Df(current_point.x, current_point.y));
            switch(cms)
            {
                case CursorSquareState::LEFT:
                    wxSetCursor(wxCursor(wxCURSOR_SIZEWE));
                    break;
                case CursorSquareState::RIGHT:
                    wxSetCursor(wxCursor(wxCURSOR_SIZEWE));
                    break;
                case CursorSquareState::TOP:
                    wxSetCursor(wxCursor(wxCURSOR_SIZENS));
                    break;
                case CursorSquareState::BOTTOM:
                    wxSetCursor(wxCursor(wxCURSOR_SIZENS));
                    break;
                case CursorSquareState::BOTTOM_RIGHT:
                    wxSetCursor(wxCursor(wxCURSOR_SIZENWSE));
                    break;
                case CursorSquareState::BOTTOM_LEFT:
                    wxSetCursor(wxCursor(wxCURSOR_SIZENESW));
                    break;
                case CursorSquareState::TOP_RIGHT:
                    wxSetCursor(wxCursor(wxCURSOR_SIZENESW));
                    break;
                case CursorSquareState::TOP_LEFT:
                    wxSetCursor(wxCursor(wxCURSOR_SIZENWSE));
                    break;
                case CursorSquareState::INSIDE:
                    wxSetCursor(wxCursor(wxCURSOR_HAND));
                    break;
                default:
                    wxSetCursor(wxCursor(wxCURSOR_HAND));
            }
        }
    }
}

void PlotWindowGLPane::keyPressed(wxKeyEvent& event)
{
    const int key_code = event.GetKeyCode();

    // Only add alpha numeric keys due to errors when clicking outside of window
    if (std::isalnum(key_code))
    {
        keyboard_state_.keyGotPressed(key_code);
    }
    Refresh();
}

void PlotWindowGLPane::keyReleased(wxKeyEvent& event)
{
    const int key_code = event.GetKeyCode();

    // Only add alpha numeric keys due to errors when clicking outside of window
    if (std::isalnum(key_code))
    {
        keyboard_state_.keyGotReleased(key_code);
    }
    Refresh();
}

// Returns window width in pixels
int PlotWindowGLPane::getWidth()
{
    return GetSize().x;
}

// Returns window height in pixels
int PlotWindowGLPane::getHeight()
{
    return GetSize().y;
}

InteractionType keyboardStateToInteractionType(const KeyboardState& keyboard_state)
{
    if (keyboard_state.keyIsPressed('C'))
    {
        return InteractionType::RESET;
    }
    else if (keyboard_state.keyIsPressed('P'))
    {
        return InteractionType::PAN;
    }
    else if (keyboard_state.keyIsPressed('R'))
    {
        return InteractionType::ROTATE;
    }
    else if (keyboard_state.keyIsPressed('Z'))
    {
        return InteractionType::ZOOM;
    }
    else
    {
        return InteractionType::UNCHANGED;
    }
}

void PlotWindowGLPane::refresh()
{
    Refresh();
}

void PlotWindowGLPane::render(wxPaintEvent& evt)
{
    (void)evt;
    if (!IsShown())
        return;

    // clang-format off
    // const double m[] = {1, 0, 0, 0, 
    //                     0, 0, 1, 0, 
    //                     0, 1, 0, 0, 
    //                     0, 0, 0, 1};
    // // clang-format on
    // glLoadMatrixd(m);

    wxGLCanvas::SetCurrent(*m_context);
    wxPaintDC(this);

    glEnable(GL_MULTISAMPLE);

    const float bg_color = 190.0f;

    glClearColor(bg_color / 255.0f, bg_color / 255.0f, bg_color / 255.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    axes_interactor_->update(
        keyboardStateToInteractionType(keyboard_state_), getWidth(), getHeight());

    const bool draw_selected_bb = is_selected_ && is_editing_;

    axes_painter_->paint(axes_interactor_->getAxesLimits(),
                         axes_interactor_->getViewAngles(),
                         axes_interactor_->generateGridVectors(),
                         axes_interactor_->getCoordConverter(),
                         draw_selected_bb);

    glEnable(GL_DEPTH_TEST);  // TODO: Put in "plotBegin" and "plotEnd"?
    axes_painter_->plotBegin();

    plot_data_handler_.visualize();

    axes_painter_->plotEnd();
    glDisable(GL_DEPTH_TEST);

    // glFlush();
    SwapBuffers();
}
