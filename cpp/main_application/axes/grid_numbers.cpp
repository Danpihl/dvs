#include "grid_numbers.h"

#include <iostream>

#include "dvs/math/math.h"
#include "misc/misc.h"
#include "text_rendering.h"
#include "opengl_low_level/opengl_low_level.h"

#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

const float kTextScale = 1.0f / 8.0f;

void drawXLetter(const TextRenderer& text_renderer,
                 const glm::mat4& view_model,
                 const glm::vec4& v_viewport,
                 const glm::mat4& projection,
                 const float width,
                 const float height,
                 const float y,
                 const float z)
{
    const glm::vec3 v(0.0f, y * 1.1, z * 1.1);

    const glm::vec3 v_projected = glm::project(v,
                        view_model,
                        projection,
                        v_viewport);

    text_renderer.renderTextFromRightCenter("X", v_projected[0], v_projected[1], 0.0005f, width, height);
}

void drawYLetter(const TextRenderer& text_renderer,
                 const glm::mat4& view_model,
                 const glm::vec4& v_viewport,
                 const glm::mat4& projection,
                 const float width,
                 const float height,
                 const float x,
                 const float z)
{
    const glm::vec3 v(x * 1.1, 0.0f, z * 1.1);

    const glm::vec3 v_projected = glm::project(v,
                        view_model,
                        projection,
                        v_viewport);

    text_renderer.renderTextFromRightCenter("Y", v_projected[0], v_projected[1], 0.0005f, width, height);
}

void drawZLetter(const TextRenderer& text_renderer,
                 const glm::mat4& view_model,
                 const glm::vec4& v_viewport,
                 const glm::mat4& projection,
                 const float width,
                 const float height,
                 const float x,
                 const float y)
{
    const glm::vec3 v(x * 1.1, y * 1.1, 0.0f);

    const glm::vec3 v_projected = glm::project(v,
                        view_model,
                        projection,
                        v_viewport);

    text_renderer.renderTextFromRightCenter("Z", v_projected[0], v_projected[1], 0.0005f, width, height);
}

void drawXAxisNumbers(const TextRenderer& text_renderer,
                      const glm::mat4& view_model,
                      const glm::vec4& v_viewport,
                      const glm::mat4& projection,
                      const double azimuth,
                      const double elevation,
                      const float width,
                      const float height,
                      const SnappingAxis snapping_axis,
                      const Vec3Dd& axes_center,
                      const GridVectors& gv,
                      const GLint text_color_uniform)
{
    const bool cond = (azimuth > (M_PI / 2.0)) || (azimuth < (-M_PI / 2.0));
    const double y = (elevation == M_PI/2.0) ? -1.05 : (cond ? 1.05 : -1.05);
    const double z0 = elevation > 0.0 ? -1.05 : 1.05;
    const double z = ((snapping_axis == SnappingAxis::X) || (snapping_axis == SnappingAxis::Y)) ? -1.05 : z0;

    glUniform3f(text_color_uniform, 1.0f, 0.0f, 0.0f);
    drawXLetter(text_renderer, view_model, v_viewport, projection, width, height, y, z);
    glUniform3f(text_color_uniform, 0.0f, 0.0f, 0.0f);

    // std::cout << azimuth * 180.0 / M_PI << std::endl;

    const bool cond2 = ((azimuth <= 0) && (azimuth >= (-M_PI / 2.0))) || 
                       ((azimuth >= (M_PI / 2.0)) && (azimuth <= (M_PI)));

    for(size_t k = 0; k < gv.x.size(); k++)
    {
        const double x = gv.x(k);
        const glm::vec3 v3(x, y, z);

        const glm::vec3 v_projected = glm::project(v3,
                         view_model,
                         projection,
                         v_viewport);
        const std::string val = formatNumber(gv.x(k) + axes_center.x, 3);
        if(cond2)
        {
            text_renderer.renderTextFromRightCenter(val, v_projected[0], v_projected[1], 0.0005f, width, height);
        }
        else
        {
            text_renderer.renderTextFromLeftCenter(val, v_projected[0], v_projected[1], 0.0005f, width, height);
        }
    }
}

void drawYAxisNumbers(const TextRenderer& text_renderer,
                      const glm::mat4& view_model,
                      const glm::vec4& v_viewport,
                      const glm::mat4& projection,
                      const double azimuth,
                      const double elevation,
                      const float width,
                      const float height,
                      const SnappingAxis snapping_axis,
                      const Vec3Dd& axes_center,
                      const GridVectors& gv,
                      const GLint text_color_uniform)
{
    const double x = (elevation == M_PI/2.0) ? -1.05 : (azimuth < 0.0 ? 1.05 : -1.05);
    const double z0 = elevation > 0.0 ? -1.05 : 1.05;
    const double z = ((snapping_axis == SnappingAxis::X) || (snapping_axis == SnappingAxis::Y)) ? -1.05 : z0;

    const bool cond2 = ((azimuth <= 0) && (azimuth >= (-M_PI / 2.0))) || 
                       ((azimuth >= (M_PI / 2.0)) && (azimuth <= (M_PI)));

    glUniform3f(text_color_uniform, 1.0f, 0.0f, 0.0f);
    drawYLetter(text_renderer, view_model, v_viewport, projection, width, height, x, z);
    glUniform3f(text_color_uniform, 0.0f, 0.0f, 0.0f);

    for(size_t k = 0; k < gv.y.size(); k++)
    {
        const double y = gv.y(k);
        const glm::vec3 v3(x, y, z);

        const glm::vec3 v_projected = glm::project(v3,
                         view_model,
                         projection,
                         v_viewport);
        const std::string val = formatNumber(gv.y(k) + axes_center.y, 3);
        if(elevation == M_PI/2.0)
        {
            text_renderer.renderTextFromRightCenter(val, v_projected[0], v_projected[1], 0.0005f, width, height);
        }
        else if(cond2)
        {
            text_renderer.renderTextFromLeftCenter(val, v_projected[0], v_projected[1], 0.0005f, width, height);
        }
        else
        {
            text_renderer.renderTextFromRightCenter(val, v_projected[0], v_projected[1], 0.0005f, width, height);
        }
    }
}

void drawZAxisNumbers(const TextRenderer& text_renderer,
                      const glm::mat4& view_model,
                      const glm::vec4& v_viewport,
                      const glm::mat4& projection,
                      const double azimuth,
                      const double elevation,
                      const float width,
                      const float height,
                      const Vec3Dd& axes_center,
                      const GridVectors& gv,
                      const GLint text_color_uniform)
{
    const bool cond = (azimuth > (M_PI / 2.0)) || (azimuth < (-M_PI / 2.0));
    const double x = azimuth > 0.0f ? 1.05 : -1.05;
    const double y = cond ? 1.05 : -1.05;

    const bool cond2 = ((azimuth <= 0) && (azimuth >= (-M_PI / 2.0))) || 
                       ((azimuth >= (M_PI / 2.0)) && (azimuth <= (M_PI)));

    glUniform3f(text_color_uniform, 1.0f, 0.0f, 0.0f);
    drawZLetter(text_renderer, view_model, v_viewport, projection, width, height, x, y);
    glUniform3f(text_color_uniform, 0.0f, 0.0f, 0.0f);

    for(size_t k = 0; k < gv.z.size(); k++)
    {
        const double z = gv.z(k);
        const glm::vec3 v3(x, y, z);

        const glm::vec3 v_projected = glm::project(v3,
                         view_model,
                         projection,
                         v_viewport);
        const std::string val = formatNumber(gv.z(k) + axes_center.z, 3);
        if(cond2)
        {
            text_renderer.renderTextFromRightCenter(val, v_projected[0], v_projected[1], 0.0005f, width, height);
        }
        else
        {
            text_renderer.renderTextFromLeftCenter(val, v_projected[0], v_projected[1], 0.0005f, width, height);
        }
    }
}

void drawGridNumbers(const TextRenderer& text_renderer, const Shader text_shader, const AxesLimits& axes_limits, const ViewAngles& view_angles, const glm::mat4& view_mat, const glm::mat4& model_mat, const glm::mat4& projection_mat, const float width, const float height, const GridVectors& gv)
{
    glm::mat4 model_mat_local = model_mat;
    
    glUseProgram(text_shader.programId());

    const GLint text_color_uniform = glGetUniformLocation(text_shader.programId(), "textColor");

    // AxesLimits
    const Vec3Dd axes_center = axes_limits.getAxesCenter();

    // Scales
    const Vec3Dd scale = axes_limits.getAxesScale();

    model_mat_local[3][0] = 0.0;
    model_mat_local[3][1] = 0.0;
    model_mat_local[3][2] = 0.0;

    const glm::vec4 v_viewport = glm::vec4(-1, -1, 2, 2);

    const double az = view_angles.getSnappedAzimuth();
    const double el = view_angles.getSnappedElevation();

    glm::mat4 scale_mat = glm::mat4(1.0f);

    scale_mat[0][0] = 1.0 / scale.x;
    scale_mat[1][1] = 1.0;
    scale_mat[2][2] = 1.0;
    scale_mat[3][3] = 1.0;

    const glm::mat4 view_model_x = view_mat * model_mat_local * scale_mat;

    scale_mat[0][0] = 1.0;
    scale_mat[1][1] = 1.0 / scale.y;
    const glm::mat4 view_model_y = view_mat * model_mat_local * scale_mat;

    scale_mat[1][1] = 1.0;
    scale_mat[2][2] = 1.0 / scale.z;
    const glm::mat4 view_model_z = view_mat * model_mat_local * scale_mat;

    if(!view_angles.isSnappedAlongX())
    {
        drawXAxisNumbers(text_renderer, view_model_x, v_viewport, projection_mat, az, el, width, height, view_angles.getSnappingAxis(), axes_center, gv, text_color_uniform);
    }
    if(!view_angles.isSnappedAlongY())
    {
        drawYAxisNumbers(text_renderer, view_model_y, v_viewport, projection_mat, az, el, width, height, view_angles.getSnappingAxis(), axes_center, gv, text_color_uniform);
    }
    if(!view_angles.isSnappedAlongZ())
    {
        drawZAxisNumbers(text_renderer, view_model_z, v_viewport, projection_mat, az, el, width, height, axes_center, gv, text_color_uniform);
    }

    glUseProgram(0);
}