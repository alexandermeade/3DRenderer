#ifndef MATRIX_H
#define MATRIX_H
#include "./Vector.h"

typedef struct {
    float m[4][4];
}Matrix4x4;

#define Matrix4x4_IDENTITY (Matrix4x4){ .m = { \
    {1, 0, 0, 0}, \
    {0, 1, 0, 0}, \
    {0, 0, 1, 0}, \
    {0, 0, 0, 1}  \
} }
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

Vec3 Matrix4x4_multiplyVec3(Matrix4x4 m, Vec3 vec) {
    Vec3 v;
    v.x = vec.x * m.m[0][0] + vec.y * m.m[1][0] + vec.z * m.m[2][0] + vec.w * m.m[3][0];
    v.y = vec.x * m.m[0][1] + vec.y * m.m[1][1] + vec.z * m.m[2][1] + vec.w * m.m[3][1];
    v.z = vec.x * m.m[0][2] + vec.y * m.m[1][2] + vec.z * m.m[2][2] + vec.w * m.m[3][2];
    v.w = vec.x * m.m[0][3] + vec.y * m.m[1][3] + vec.z * m.m[2][3] + vec.w * m.m[3][3];
    return v;
}
/*
Matrix4x4 Matrix4x4_pointAt(Vec3 pos, Vec3 target, Vec3 up) {
// Calculate new forward direction
		Vec3 newForward = Vec3_sub(target, pos);
		newForward = Vec3_unitVector(newForward);

		// Calculate new Up direction
		Vec3 a = Vec3_mult(newForward, Vec3_dotProduct(up, newForward));
		Vec3 newUp = Vec3_sub(up, a);
		newUp = Vec3_unitVector(newUp);

		// New Right direction is easy, its just cross product
		Vec3 newRight = Vec3_crossProduct(newUp, newForward);

		// Construct Dimensioning and Translation Matrix	
		Matrix4x4 matrix;
		matrix.m[0][0] = newRight.x;	matrix.m[0][1] = newRight.y;	matrix.m[0][2] = newRight.z;	matrix.m[0][3] = 0.0f;
		matrix.m[1][0] = newUp.x;		matrix.m[1][1] = newUp.y;		matrix.m[1][2] = newUp.z;		matrix.m[1][3] = 0.0f;
		matrix.m[2][0] = newForward.x;	matrix.m[2][1] = newForward.y;	matrix.m[2][2] = newForward.z;	matrix.m[2][3] = 0.0f;
		matrix.m[3][0] = pos.x;			matrix.m[3][1] = pos.y;			matrix.m[3][2] = pos.z;			matrix.m[3][3] = 1.0f;
		return matrix;
}
*/

Matrix4x4 Matrix4x4_pointAt(Vec3 pos, Vec3 target, Vec3 up) {
    Vec3 newForward = Vec3_sub(target, pos);
    newForward = Vec3_unitVector(newForward);

    Vec3 a = Vec3_mult(newForward, Vec3_dotProduct(up, newForward));
    Vec3 newUp = Vec3_sub(up, a);
    newUp = Vec3_unitVector(newUp);

    Vec3 newRight = Vec3_crossProduct(newUp, newForward);

    Matrix4x4 matrix;
    matrix.m[0][0] = newRight.x;    matrix.m[0][1] = newRight.y;    matrix.m[0][2] = newRight.z;    matrix.m[0][3] = pos.x;
    matrix.m[1][0] = newUp.x;       matrix.m[1][1] = newUp.y;       matrix.m[1][2] = newUp.z;       matrix.m[1][3] = pos.y;
    matrix.m[2][0] = newForward.x;  matrix.m[2][1] = newForward.y;  matrix.m[2][2] = newForward.z;  matrix.m[2][3] = pos.z;
    matrix.m[3][0] = 0.0f;          matrix.m[3][1] = 0.0f;          matrix.m[3][2] = 0.0f;          matrix.m[3][3] = 1.0f;
    return matrix;
}


Matrix4x4 Matrix4x4_inverse(Matrix4x4 m) {
		Matrix4x4 matrix;
		matrix.m[0][0] = m.m[0][0]; matrix.m[0][1] = m.m[1][0]; matrix.m[0][2] = m.m[2][0]; matrix.m[0][3] = 0.0f;
		matrix.m[1][0] = m.m[0][1]; matrix.m[1][1] = m.m[1][1]; matrix.m[1][2] = m.m[2][1]; matrix.m[1][3] = 0.0f;
		matrix.m[2][0] = m.m[0][2]; matrix.m[2][1] = m.m[1][2]; matrix.m[2][2] = m.m[2][2]; matrix.m[2][3] = 0.0f;
		matrix.m[3][0] = -(m.m[3][0] * matrix.m[0][0] + m.m[3][1] * matrix.m[1][0] + m.m[3][2] * matrix.m[2][0]);
		matrix.m[3][1] = -(m.m[3][0] * matrix.m[0][1] + m.m[3][1] * matrix.m[1][1] + m.m[3][2] * matrix.m[2][1]);
		matrix.m[3][2] = -(m.m[3][0] * matrix.m[0][2] + m.m[3][1] * matrix.m[1][2] + m.m[3][2] * matrix.m[2][2]);
		matrix.m[3][3] = 1.0f;
		return matrix;

}


#endif
