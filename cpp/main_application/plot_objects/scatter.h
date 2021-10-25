#ifndef SCATTER_2D_H_
#define SCATTER_2D_H_

#include <string>
#include <vector>

#include "dvs/math/math.h"
#include "opengl_low_level/data_structures.h"
#include "opengl_low_level/opengl_low_level.h"
#include "plot_functions/plot_functions.h"
#include "plot_objects/plot_object_base.h"

class Scatter2D : public PlotObjectBase
{
private:
    uint8_t* points_ptr_;
    GLuint buffer_idx_;

    void findMinMax() override;
    Shader shader_;

public:
    Scatter2D();
    Scatter2D(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr);
    ~Scatter2D();

    void visualize() override;
};

Scatter2D::Scatter2D(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr)
    : PlotObjectBase(std::move(received_data), hdr)
{
    if (type_ != Function::SCATTER2)
    {
        throw std::runtime_error("Invalid function type for Scatter2D!");
    }

    shader_ = Shader::createFromCode(shaders::kBasicVertexCode, shaders::kBasicFragmentCode);
    std::cout << shader_.programId() << std::endl;

    points_ptr_ =
        convertData2DOuter(data_ptr_, data_type_, num_elements_, num_bytes_per_element_, num_bytes_for_one_vec_);
}

void Scatter2D::findMinMax()
{
    Vec2Dd min_vec_2d, max_vec_2d;
    std::tie<Vec2Dd, Vec2Dd>(min_vec_2d, max_vec_2d) =
        findMinMaxFromTwoVectors(data_ptr_, num_elements_, num_bytes_for_one_vec_, data_type_);

    min_vec.x = min_vec_2d.x;
    min_vec.y = min_vec_2d.y;
    min_vec.z = -1.0;

    max_vec.x = max_vec_2d.x;
    max_vec.y = max_vec_2d.y;
    max_vec.z = 1.0;
}

void Scatter2D::visualize()
{
    if (!visualize_has_run_)
    {
        visualize_has_run_ = true;
        glGenBuffers(1, &buffer_idx_);
        glUseProgram(shader_.programId());
        glBindBuffer(GL_ARRAY_BUFFER, buffer_idx_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_elements_ * 2, points_ptr_, GL_STATIC_DRAW);
    }
    setColor(color_);
    setPointSize(point_size_);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_idx_);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_POINTS, 0, num_elements_);
    glDisableVertexAttribArray(0);
    glUseProgram(0);
}

Scatter2D::~Scatter2D()
{
    delete[] points_ptr_;
}

#endif
