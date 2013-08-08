// Loads and renders a Wavefront OBJ model exported from Blender.
// Includes texture coordinates, texture loading and normals.
// --
// Only compatible with Triangulated meshes.

#ifndef MODELOBJ_H
#define MODELOBJ_H

#include <string>

/// TODO: Add VBO support again

struct t_vert
{
    float x, y, z;
}vert;


class ModelObj
{
protected:
    std::string texture;
    t_vert *verts;
public:
    ModelObj();
    ~ModelObj();

    void loadFromFile(std::string filename);
    void render();
};
#endif // MODELOBJ_H
