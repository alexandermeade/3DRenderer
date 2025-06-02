#ifndef MESH_H
#define MESH_H
#include "./Vector.h"
#include "./Color.h"
#include "./Tri.h"
#include "./Matrix.h"

// Perspective projection
void Matrix_perspectiveProjection(Vec3 *vertex, float fov, float aspect_ratio, float near_plane, float far_plane) {
    float f = 1.0f / tanf(fov / 2.0f);
    float z = (vertex->z == 0.0f) ? 0.001f : vertex->z;

    vertex->x = (vertex->x * f / aspect_ratio) / z;
    vertex->y = (vertex->y * f) / z;
}

// Interpolation helper
float interpolateX(int y, Vec3 p1, Vec3 p2) {
    if (p2.y == p1.y) return p1.x;
    return p1.x + (float)(y - p1.y) * (p2.x - p1.x) / (p2.y - p1.y);
}

#define EPSILON 0.0001f

// Helper to check same sign
int same_sign(float a, float b, float c) {
    return (a >= -EPSILON && b >= -EPSILON && c >= -EPSILON) || 
           (a <= EPSILON && b <= EPSILON && c <= EPSILON);
}

typedef struct {
    Tri3D *tris;    
    size_t count;
    Vec3 centroid;
} Mesh;

// Translation
void Mesh_translate(Mesh *mesh, Vec3 dir) {
    for (int i = 0; i < mesh->count; i++) {
        for (int j = 0; j < 3; j++) {
            mesh->tris[i].verts[j].pos.x += dir.x;
            mesh->tris[i].verts[j].pos.y += dir.y;
            mesh->tris[i].verts[j].pos.z += dir.z;
        }
    }
    mesh->centroid.x += dir.x;
    mesh->centroid.y += dir.y;
    mesh->centroid.z += dir.z;
}

void Mesh_scale(Mesh *mesh, Vec3 amount) {
    for (int i = 0; i < mesh->count; i++) {
        for (int j = 0; j < 3; j++) {
            mesh->tris[i].verts[j].pos.x *= amount.x;
            mesh->tris[i].verts[j].pos.y *= amount.y;
            mesh->tris[i].verts[j].pos.z *= amount.z;
        }
    }   
}

// Rotation
void Mesh_rotation(Mesh *mesh, Vec3 deg) {
    Vec3 pivot = mesh->centroid;
    for (int i = 0; i < mesh->count; i++) {
        for (int j = 0; j < 3; j++) {
            Vec3 p = mesh->tris[i].verts[j].pos;
            p = Vec3_sub(p, pivot);

            if (deg.x != 0) {
                float y = p.y, z = p.z;
                p.y = y * cos(deg.x) - z * sin(deg.x);
                p.z = y * sin(deg.x) + z * cos(deg.x);
            }
            if (deg.y != 0) {
                float x = p.x, z = p.z;
                p.x = x * cos(deg.y) + z * sin(deg.y);
                p.z = -x * sin(deg.y) + z * cos(deg.y);
            }
            if (deg.z != 0) {
                float x = p.x, y = p.y;
                p.x = x * cos(deg.z) - y * sin(deg.z);
                p.y = x * sin(deg.z) + y * cos(deg.z);
            }

            p = Vec3_add(p, pivot);
            mesh->tris[i].verts[j].pos = p;
        }
    }
}

// Drawing
void Mesh_draw(Mesh *mesh, SDL_Renderer *renderer, Vec3 camera, Matrix4x4 viewMat, float fov, float aspect, float near, float far, int width, int height) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for (size_t i = 0; i < mesh->count; i++) {
        Tri3D tri = mesh->tris[i];

        // Transform vertices into view space first
        Vec3 a = Matrix4x4_multiplyVec3(viewMat, tri.verts[0].pos);
        Vec3 b = Matrix4x4_multiplyVec3(viewMat, tri.verts[1].pos);
        Vec3 c = Matrix4x4_multiplyVec3(viewMat, tri.verts[2].pos);

        // Back-face culling (in view space)
        Vec3 line1 = Vec3_sub(b, a);
        Vec3 line2 = Vec3_sub(c, a);
        Vec3 normal = Vec3_crossProduct(line1, line2);
        normal = Vec3_unitVector(normal);

        // View direction in view space is just Z-axis forward
        if (Vec3_dotProduct(normal, Vec3_sub(a, ((Vec3){0,0,0}))) < 0.0f) {
            // Lighting
            Vec3 lightDir = Vec3_unitVector((Vec3){0, 0, -1});
            float dp = Vec3_dotProduct(normal, lightDir);

            // Perspective projection
            Matrix_perspectiveProjection(&a, fov, aspect, near, far);
            Matrix_perspectiveProjection(&b, fov, aspect, near, far);
            Matrix_perspectiveProjection(&c, fov, aspect, near, far);

            // Draw triangle
            Tri3D_fillTriangleLum(
                renderer,
                Vertex(a, tri.verts[0].color),
                Vertex(b, tri.verts[1].color),
                Vertex(c, tri.verts[2].color),
                dp,
                width, height,
                camera
            );
        }
    }
}
#endif

