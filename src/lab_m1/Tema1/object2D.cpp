// Taken from EGC - Lab 2

#include "object2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object2D::CreateSquare(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* object2D::CreateRectangle(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float x_size,
    float y_size,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(x_size, 0, 0), color),
        VertexFormat(corner + glm::vec3(x_size, y_size, 0), color),
        VertexFormat(corner + glm::vec3(0, y_size, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* object2D::CreateCircle(
    const std::string& name,
    glm::vec3 center,
    float radius,
    glm::vec3 color,
    bool fill)
{
    std::vector<VertexFormat> vertices;

    // Create the circle in the origin
    int sides = 100;
    
    std::vector<unsigned int> indices;
    int currIndex = 0;

    for (int a = 0; a < 360; a += 360 / sides)
    {
        float heading = a * (M_PI / 180);
        VertexFormat* vertexFormat = new VertexFormat(center + glm::vec3(cos(heading) * radius, sin(heading) * radius, 0), color);
        vertices.push_back(*vertexFormat);
        indices.push_back(currIndex++);
    }

    Mesh* circle = new Mesh(name);
    
    if (fill)
        circle->SetDrawMode(GL_TRIANGLE_FAN);
    else
        circle->SetDrawMode(GL_LINE_LOOP);

    circle->InitFromData(vertices, indices);
 
    return circle;
}
