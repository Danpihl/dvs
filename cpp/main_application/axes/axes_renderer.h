#ifndef DVS_AXES_RENDERER_H_
#define DVS_AXES_RENDERER_H_

#include <cmath>
#include <utility>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "axes/text_rendering.h"
#include "axes/structures/axes_limits.h"
#include "axes/structures/axes_settings.h"
#include "axes/structures/grid_vectors.h"
#include "axes/structures/view_angles.h"
#include "opengl_low_level/opengl_low_level.h"
#include "shader.h"
#include "axes/plot_box_walls.h"
#include "axes/plot_box_silhouette.h"
#include "axes/plot_box_grid.h"
#include "axes/grid_numbers.h"

class AxesRenderer
{
private:
    ViewAngles view_angles_;
    AxesLimits axes_limits_;

    AxesSettings axes_settings_;

    TextRenderer text_renderer_;

    Shader text_shader_;
    Shader plot_shader_;
    Shader plot_shader_2;

    PlotBoxWalls* plot_box_walls_;
    PlotBoxSilhouette* plot_box_silhouette_;
    PlotBoxGrid* plot_box_grid_;

    glm::mat4 orth_projection_mat;
    glm::mat4 persp_projection_mat;
    glm::mat4 projection_mat;
    glm::mat4 view_mat;
    glm::mat4 model_mat;
    glm::mat4 scale_mat;

    Matrix<double> rot_mat;

    float width_, height_;
    Vec3Dd scale_for_window_;

    GridVectors gv_;
    bool use_perspective_proj_;

    void renderPlotBox();
    void renderBoxGrid();

public:
    AxesRenderer();

    void updateStates(const AxesLimits& axes_limits,
                      const ViewAngles& view_angles,
                      const GridVectors& gv,
                      const bool use_perspective_proj,
                      const float width,
                      const float height);
    void render();
    void plotBegin();
    void plotEnd();
    GLuint getPlotShaderId() const;

};


#endif
