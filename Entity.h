#pragma once
#include "TexturedModel.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

// An instance of a TexturedModel, it shares the underlying textured model.
// An Entity purely adds positioning/scale/rotation information
class Entity {
public:
    Entity(TexturedModel* model, float x, float y, float z) : _model(model) {
        _position = glm::vec3(x, y, z);
        _rx = _ry = _rz = 0.f;
        _scale = glm::vec3(1.f, 1.f, 1.f);
    }

    // Gets the model transformation (from model to world coords)
    glm::mat4 getTransformMat() {
        glm::mat4 mat(1.f);
        mat = glm::translate(mat, _position);
        mat = glm::scale(mat, _scale);
        mat = glm::rotate(mat, _rx, glm::vec3(1.f, 0.f, 0.f));
        mat = glm::rotate(mat, _ry, glm::vec3(0.f, 1.f, 0.f));
        mat = glm::rotate(mat, _rz, glm::vec3(0.f, 0.f, 1.f));
        return mat;
    }

    void translate(float dx = 0.f, float dy = 0.f, float dz = 0.f) {
        _position.x += dx;
        _position.y += dy;
        _position.z += dz;
    }

    void rotate(float rx = 0.f, float ry = 0.f, float rz = 0.f) {
        _rx += rx;
        _ry += ry;
        _rz += rz;
    }

    void scale(float xscale, float yscale, float zscale) {
        _scale = glm::vec3(xscale, yscale, zscale);
    }

    TexturedModel* getModel() {
        return _model;
    }

    float getRotateX() {
        return _rx;
    }
    float getRotateY() {
        return _ry;
    }
    float getRotateZ() {
        return _rx;
    }

    glm::vec3 getPos() {
        return _position;
    }

    ModelTexture& getTexture() {
        return _model->getTexture();
    }

    RawModel& getRawModel() {
        return _model->getModel();
    }

private:
    TexturedModel* _model;
    glm::vec3 _position;
    float _rx, _ry, _rz;
    glm::vec3 _scale;
};