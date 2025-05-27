#ifndef MATRIX_H
#define MATRIX_H
#include "./Vector.h"

typedef struct {
    float m[4][4];
}Matrix4x4;
/*
Matrix4x4 Matrix4x4_ pointAt(Vec3 &pos, vec3 &target, vec3d &up) {

    Vec3 newForward = Vec3_sub(target, pos);
    newForward = Vec3_unitVector(newForward);
    Vec3 oldForward = newForward;
    Vec3_scale(&newForward, Vec3_dotProduct(up, newForward));
    Vec3 newUp = Vector_sub(up, newFordward);

    newUp = Vec3_unitVector(newUp);

    Vec3 newRight = Vec3_crossProduct(newUp, oldForward);
    
}
*/

Vec3 Matrix4x4_multiplyVec3(Matrix4x4 mat, Vec3 vec) {
    Vec3 result;
    
    // Assume homogeneous coordinate w = 1
    float x = vec.x;
    float y = vec.y;
    float z = vec.z;
    float w = 1.0f;

    result.x = mat.m[0][0] * x + mat.m[0][1] * y + mat.m[0][2] * z + mat.m[0][3] * w;
    result.y = mat.m[1][0] * x + mat.m[1][1] * y + mat.m[1][2] * z + mat.m[1][3] * w;
    result.z = mat.m[2][0] * x + mat.m[2][1] * y + mat.m[2][2] * z + mat.m[2][3] * w;

    return result;
}

Matrix4x4 Matrix4x4_pointAt(Vec3 pos, Vec3 target, Vec3 up) {
    // Calculate new forward direction (target - position)
    Vec3 forward = Vec3_sub(target, pos);
    forward = Vec3_unitVector(forward);

    // Calculate the right vector from up and forward
    Vec3 right = Vec3_crossProduct(up, forward);
    right = Vec3_unitVector(right);

    // Recalculate orthonormal up vector
    Vec3 newUp = Vec3_crossProduct(forward, right);

    Matrix4x4 matrix;

    // Rotation part (basis vectors)
    matrix.m[0][0] = right.x;
    matrix.m[0][1] = right.y;
    matrix.m[0][2] = right.z;
    matrix.m[0][3] = 0.0f;

    matrix.m[1][0] = newUp.x;
    matrix.m[1][1] = newUp.y;
    matrix.m[1][2] = newUp.z;
    matrix.m[1][3] = 0.0f;

    matrix.m[2][0] = forward.x;
    matrix.m[2][1] = forward.y;
    matrix.m[2][2] = forward.z;
    matrix.m[2][3] = 0.0f;

    // Translation part (position)
    matrix.m[3][0] = pos.x;
    matrix.m[3][1] = pos.y;
    matrix.m[3][2] = pos.z;
    matrix.m[3][3] = 1.0f;

    return matrix;
}

Matrix4x4 Matrix4x4_inverse(Matrix4x4 m) {
    Matrix4x4 result;

    // Transpose the rotation part (upper-left 3x3)
    result.m[0][0] = m.m[0][0]; result.m[0][1] = m.m[1][0]; result.m[0][2] = m.m[2][0]; result.m[0][3] = 0.0f;
    result.m[1][0] = m.m[0][1]; result.m[1][1] = m.m[1][1]; result.m[1][2] = m.m[2][1]; result.m[1][3] = 0.0f;
    result.m[2][0] = m.m[0][2]; result.m[2][1] = m.m[1][2]; result.m[2][2] = m.m[2][2]; result.m[2][3] = 0.0f;

    // Inverse translation = -Rᵀ * T
    result.m[3][0] = -(m.m[3][0] * result.m[0][0] + m.m[3][1] * result.m[1][0] + m.m[3][2] * result.m[2][0]);
    result.m[3][1] = -(m.m[3][0] * result.m[0][1] + m.m[3][1] * result.m[1][1] + m.m[3][2] * result.m[2][1]);
    result.m[3][2] = -(m.m[3][0] * result.m[0][2] + m.m[3][1] * result.m[1][2] + m.m[3][2] * result.m[2][2]);
    result.m[3][3] = 1.0f;

    return result;
}


#endif
