#ifndef DVS_STAIRS_H_
#define DVS_STAIRS_H_

#include <string>
#include <vector>

#include "dvs/math/math.h"
#include "opengl_low_level/data_structures.h"
#include "opengl_low_level/opengl_low_level.h"
#include "plot_functions/plot_functions.h"
#include "plot_objects/plot_object_base.h"

class Stairs : public PlotObjectBase
{
private:
    uint8_t* points_ptr_;
    GLuint buffer_idx_;

    void findMinMax() override;

public:
    Stairs();
    Stairs(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr);
    ~Stairs();

    void visualize() override;
};

Stairs::Stairs(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr)
    : PlotObjectBase(std::move(received_data), hdr)
{
    if (type_ != Function::STAIRS)
    {
        throw std::runtime_error("Invalid function type for Stairs!");
    }

    points_ptr_ =
        convertStairsOuter(data_ptr_, data_type_, num_elements_, num_bytes_per_element_, num_bytes_for_one_vec_);
}

void Stairs::findMinMax()
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

void Stairs::visualize()
{
    if (!visualize_has_run_)
    {
        visualize_has_run_ = true;
        glGenBuffers(1, &buffer_idx_);
        glBindBuffer(GL_ARRAY_BUFFER, buffer_idx_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (num_elements_ * 2 - 1) * 2, points_ptr_, GL_STATIC_DRAW);
    }
    setColor(color_);
    setLinewidth(line_width_);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_idx_);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_LINE_STRIP, 0, num_elements_ * 2 - 1);
    glDisableVertexAttribArray(0);
}

Stairs::~Stairs()
{
    delete[] points_ptr_;
}

#endif
