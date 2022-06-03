#include "axes/legend_renderer.h"

#include <iostream>

#include "opengl_low_level/data_structures.h"

constexpr int kMaxNumPoints{100};

const float right_margin = 0.1;
const float top_margin = 0.1;

const float dx = 2.0;
const float dz = 0.2;

const float x_max = 3.0f - right_margin;
const float x_min = x_max - dx;
const float z_max = 3.0f - top_margin;
const float z_min = z_max - dz;

const float dz_text = 0.2;

float legend_inner_vertices[] = {
        x_min, 0.0f, z_min,
        x_max, 0.0f, z_min,
        x_max, 0.0f, z_max,

        x_min, 0.0f, z_min,
        x_min, 0.0f, z_max,
        x_max, 0.0f, z_max
    };

float legend_edge_vertices[] = {
        x_min, 0.0f, z_min,
        x_max, 0.0f, z_min,
        x_max, 0.0f, z_max,
        x_min, 0.0f, z_max,
        x_min, 0.0f, z_min,
    };

GLfloat legend_edge_color[] = {
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
};

GLfloat legend_color_inner[] = {
        0.866666666f, 0.827450f, 0.7843137f,
        0.866666666f, 0.827450f, 0.7843137f,
        0.866666666f, 0.827450f, 0.7843137f,

        0.866666666f, 0.827450f, 0.7843137f,
        0.866666666f, 0.827450f, 0.7843137f,
        0.866666666f, 0.827450f, 0.7843137f
};

void setValues(const float new_x_min, const float new_x_max, const float new_z_min, const float new_z_max)
{
    legend_inner_vertices[0] = new_x_min; legend_inner_vertices[2] = new_z_min;
    legend_inner_vertices[3] = new_x_max; legend_inner_vertices[5] = new_z_min;
    legend_inner_vertices[6] = new_x_max; legend_inner_vertices[8] = new_z_max;

    legend_inner_vertices[9] = new_x_min; legend_inner_vertices[11] = new_z_min;
    legend_inner_vertices[12] = new_x_min; legend_inner_vertices[14] = new_z_max;
    legend_inner_vertices[15] = new_x_max; legend_inner_vertices[17] = new_z_max;

    legend_edge_vertices[0] = new_x_min; legend_edge_vertices[2] = new_z_min;
    legend_edge_vertices[3] = new_x_max; legend_edge_vertices[5] = new_z_min;
    legend_edge_vertices[6] = new_x_max; legend_edge_vertices[8] = new_z_max;
    legend_edge_vertices[9] = new_x_min; legend_edge_vertices[11] = new_z_max;
    legend_edge_vertices[12] = new_x_min; legend_edge_vertices[14] = new_z_min;
}

void LegendRenderer::setVertexAtIdx(const float x, const float y, const float z, const size_t idx)
{
    points_.getDataPointer()[idx] = x;
    points_.getDataPointer()[idx + 1] = y;
    points_.getDataPointer()[idx + 2] = z;
}

void LegendRenderer::setColorAtIdx(const float r, const float g, const float b, const size_t idx)
{
    colors_.getDataPointer()[idx] = r;
    colors_.getDataPointer()[idx + 1] = g;
    colors_.getDataPointer()[idx + 2] = b;
}

void LegendRenderer::renderColorMapLegend(const size_t num_segments, const RGBColorMap<float>* const color_map, const float xc, const float yc, const float r)
{
    const float delta_phi = static_cast<float>(M_PI) * 2.0f / static_cast<float>(num_segments);
    float angle = 0.0f;
    int idx = 0;

    for(size_t k = 0; k < num_segments; k++)
    {
        const RGBTripletf color{color_map->getColor(angle / (2.0f * M_PI))};
        setVertexAtIdx(xc, 0.0f, yc, idx);
        setColorAtIdx(color.red, color.green, color.blue, idx);
        idx += 3;

        setVertexAtIdx(r * cos(angle) + xc, 0.0f, r * sin(angle) + yc, idx);
        setColorAtIdx(color.red, color.green, color.blue, idx);
        idx += 3;
        angle += delta_phi;

        setVertexAtIdx(r * cos(angle) + xc, 0.0f, r * sin(angle) + yc, idx);
        setColorAtIdx(color.red, color.green, color.blue, idx);
        idx += 3;
    }
}

void LegendRenderer::render(const std::vector<LegendProperties>& legend_names, const float axes_width, const float axes_height)
{
    setValues(x_min, x_max, z_max - (dz + legend_names.size() * dz_text), z_max);

    edge_vao_.renderAndUpdateData(legend_edge_vertices, sizeof(float) * 3 * num_vertices_edge_);
    inner_vao_.renderAndUpdateData(legend_inner_vertices, sizeof(float) * 3 * num_vertices_inner_);

    float* const legend_shape_vertices = points_.getDataPointer();
    float* const legend_shape_colors = colors_.getDataPointer();

    for(size_t k = 0; k < legend_names.size(); k++)
    {
        const float kf = k;
        if(legend_names[k].type == LegendType::LINE)
        {
            const RGBTripletf col = legend_names[k].color;
            legend_shape_vertices[0] = x_min + 0.01;
            legend_shape_vertices[1] = 0.0f;
            legend_shape_vertices[2] = z_max - kf * dz_text - dz;

            legend_shape_vertices[3] = x_min + 0.1;
            legend_shape_vertices[4] = 0.0f;
            legend_shape_vertices[5] = z_max - kf * dz_text - dz;

            legend_shape_colors[0] = col.red;
            legend_shape_colors[1] = col.green;
            legend_shape_colors[2] = col.blue;

            legend_shape_colors[3] = col.red;
            legend_shape_colors[4] = col.green;
            legend_shape_colors[5] = col.blue;
            
            legend_shape_.renderAndUpdateData(legend_shape_vertices, legend_shape_colors, 2, 2 * 3 * sizeof(float), GL_LINE_STRIP);
        }
        else if(legend_names[k].type == LegendType::POLYGON)
        {
            if(legend_names[k].color_map_set)
            {
                const size_t num_segments = 6;
                renderColorMapLegend(num_segments, legend_names[k].color_map, x_min + 0.05, z_max - kf * dz_text - dz, 0.05);
                legend_shape_.renderAndUpdateData(legend_shape_vertices, legend_shape_colors, num_segments * 3, num_segments * 3 * 3 * sizeof(float), GL_TRIANGLES);
            }
            else
            {
                const RGBTripletf edge_color = legend_names[k].edge_color;
                const RGBTripletf face_color = legend_names[k].face_color;
                const float x_c = x_min + 0.05;
                const float z_c = z_max - kf * dz_text - dz;
                const float dxc = 0.04;
                const float dzc = 0.04;

                // Face 0
                legend_shape_vertices[0] = x_c - dxc;
                legend_shape_vertices[1] = 0.0f;
                legend_shape_vertices[2] = z_c - dzc;

                legend_shape_vertices[3] = x_c + dxc;
                legend_shape_vertices[4] = 0.0f;
                legend_shape_vertices[5] = z_c - dzc;

                legend_shape_vertices[6] = x_c + dxc;
                legend_shape_vertices[7] = 0.0f;
                legend_shape_vertices[8] = z_c + dzc;

                legend_shape_vertices[9] = x_c - dxc;
                legend_shape_vertices[10] = 0.0f;
                legend_shape_vertices[11] = z_c - dzc;

                legend_shape_colors[0] = face_color.red;
                legend_shape_colors[1] = face_color.green;
                legend_shape_colors[2] = face_color.blue;

                legend_shape_colors[3] = face_color.red;
                legend_shape_colors[4] = face_color.green;
                legend_shape_colors[5] = face_color.blue;

                legend_shape_colors[6] = face_color.red;
                legend_shape_colors[7] = face_color.green;
                legend_shape_colors[8] = face_color.blue;
                
                legend_shape_.renderAndUpdateData(legend_shape_vertices, legend_shape_colors, 3, 3 * 3 * sizeof(float), GL_TRIANGLES);

                // Face 1
                legend_shape_vertices[3] = x_c - dxc;
                legend_shape_vertices[4] = 0.0f;
                legend_shape_vertices[5] = z_c + dzc;

                legend_shape_.renderAndUpdateData(legend_shape_vertices, legend_shape_colors, 3, 3 * 3 * sizeof(float), GL_TRIANGLES);

                // Edge 0
                legend_shape_colors[0] = edge_color.red;
                legend_shape_colors[1] = edge_color.green;
                legend_shape_colors[2] = edge_color.blue;

                legend_shape_colors[3] = edge_color.red;
                legend_shape_colors[4] = edge_color.green;
                legend_shape_colors[5] = edge_color.blue;

                legend_shape_colors[6] = edge_color.red;
                legend_shape_colors[7] = edge_color.green;
                legend_shape_colors[8] = edge_color.blue;

                legend_shape_colors[9] = edge_color.red;
                legend_shape_colors[10] = edge_color.green;
                legend_shape_colors[11] = edge_color.blue;

                legend_shape_.renderAndUpdateData(legend_shape_vertices, legend_shape_colors, 4, 4 * 3 * sizeof(float), GL_LINE_STRIP);

                // Edge 1
                legend_shape_vertices[3] = x_c + dxc;
                legend_shape_vertices[4] = 0.0f;
                legend_shape_vertices[5] = z_c - dzc;

                legend_shape_.renderAndUpdateData(legend_shape_vertices, legend_shape_colors, 4, 4 * 3 * sizeof(float), GL_LINE_STRIP);

            }
        }
        
    }

    glUseProgram(0);

    glUseProgram(shader_collection_.text_shader.programId());
    const GLint text_color_uniform = glGetUniformLocation(shader_collection_.text_shader.programId(), "textColor");
    glUniform3f(text_color_uniform, 0.0f, 0.0f, 0.0f);

    for(size_t k = 0; k < legend_names.size(); k++)
    {
        const float kf = k;

        // Divided by three because the legend pane is in the coordinate system that spans [-3.0, 3.0]
        const float xp = (x_min + 0.2) / 3.0f;
        const float zp = (z_max - kf * dz_text - dz) / 3.0f;
        text_renderer_.renderTextFromLeftCenter(legend_names[k].name, xp, zp, 0.0005f, axes_width, axes_height);
    }

    glUseProgram(0);
}

LegendRenderer::LegendRenderer(const TextRenderer& text_renderer, const ShaderCollection& shader_collection) :
    text_renderer_{text_renderer}, shader_collection_{shader_collection}, points_(kMaxNumPoints * 3), colors_(kMaxNumPoints * 3)
{
    num_vertices_edge_ = 5;
    num_vertices_inner_ = 6;

    edge_vao_ = VAOObject(num_vertices_edge_, legend_edge_vertices, legend_edge_color, GL_LINE_STRIP);
    inner_vao_ = VAOObject(num_vertices_inner_, legend_inner_vertices, legend_color_inner, GL_TRIANGLES);
    legend_shape_ = VAOObject2(kMaxNumPoints, points_.getDataPointer(), colors_.getDataPointer());
}
