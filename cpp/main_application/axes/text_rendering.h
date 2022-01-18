#ifndef TEXT_RENDERING_NEW_H_
#define TEXT_RENDERING_NEW_H_

#include <glm/glm.hpp>

#include <string>

#include "dvs/math/math.h"
#include "opengl_low_level/opengl_low_level.h"

class TextRenderer
{
private:
    unsigned int vao, vbo;

public:
    TextRenderer();
    void renderTextFromCenter(std::string text, float x, float y, float scale, const float axes_width, const float axes_height) const;
    void renderTextFromRightCenter(std::string text, float x, float y, float scale, const float axes_width, const float axes_height) const;
    void renderTextFromLeftCenter(std::string text, float x, float y, float scale, const float axes_width, const float axes_height) const;
};

bool initFreetype();
dvs::Vec2Df calculateStringSize(std::string text, float x, float y, float scale, const float axes_width, const float axes_height);

#endif
