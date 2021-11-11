#pragma once

#include "utils/glm_utils.h"


namespace transform3D
{
    // Translate matrix
    inline glm::mat4 Translate(float tx, float ty, float tz)
    {
        // TODO(student): Implement the translation matrix
        return glm::transpose(glm::mat4(1, 0, 0, tx,
                                        0, 1, 0, ty,
                                        0, 0, 1, tz,
                                        0, 0, 0, 1));
    }

    // Scale matrix
    inline glm::mat4 Scale(float sX, float sY, float sZ)
    {
        // TODO(student): Implement the scaling matrix
        return glm::transpose(glm::mat4(sX, 0, 0, 0,
                                        0, sY, 0, 0,
                                        0, 0, sZ, 0,
                                        0, 0, 0, 1));
    }

    // Rotate matrix relative to the OZ axis
    inline glm::mat4 RotateOZ(float radians)
    {
        float c = cos(radians);
        float s = sin(radians);

        return glm::transpose(glm::mat4(c, -s, 0, 0,
                                        s, c, 0, 0,
                                        0, 0, 1, 0,
                                        0, 0, 0, 1));
    }

    // Rotate matrix relative to the OY axis
    inline glm::mat4 RotateOY(float radians)
    {
        float c = cos(radians);
        float s = sin(radians);

        return glm::transpose(glm::mat4(c, 0, s, 0,
                                        0, 1, 0, 0,
                                        -s, 0, c, 0,
                                        0, 0, 0, 1));

    }

    // Rotate matrix relative to the OX axis
    inline glm::mat4 RotateOX(float radians)
    {
        float c = cos(radians);
        float s = sin(radians);

        return glm::transpose(glm::mat4(1, 0, 0, 0,
                                        0, c, -s, 0,
                                        0, s, c, 0,
                                        0, 0, 0, 1));

    }
}   // namespace transform3D
