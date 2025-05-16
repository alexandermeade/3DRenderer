#ifndef MESH_H
#define MESH_H
#include "./Vector.h"
#include "./Color.h"
#include "./Tri.h"

void perspective_projection(Vec3 *vertex, float fov, float aspect_ratio, float near_plane, float far_plane) {
    float f = 1.0f / tanf(fov / 2.0f);

    // Avoid division by zero
    float z = (vertex->z == 0.0f) ? 0.001f : vertex->z;

    vertex->x = (vertex->x * f / aspect_ratio) / z;
    vertex->y = (vertex->y * f) / z;

    // You can still keep the depth value if needed
}

// Function to interpolate the X value between two points (p1 and p2) at a given Y
float interpolateX(int y, Vec3 p1, Vec3 p2) {
    if (p2.y == p1.y) return p1.x;  // Handle case where the Y values are the same
    return p1.x + (float)(y - p1.y) * (p2.x - p1.x) / (p2.y - p1.y);
}


#define EPSILON 0.0001f

// Helper function to check if all values have the same sign
int same_sign(float a, float b, float c) {
    return (a >= -EPSILON && b >= -EPSILON && c >= -EPSILON) || 
           (a <= EPSILON && b <= EPSILON && c <= EPSILON);
}



typedef struct {
    
    Tri3D *tris;    
    size_t count;
    Vec3 centroid;
}Mesh;

void Mesh_translate(Mesh *mesh, Vec3 dir) {
    for(int i = 0; i < mesh->count; i++) {
        for (int j =0; j < 3; j ++) {
            mesh->tris[i].verts[j].pos.x += dir.x;
            mesh->tris[i].verts[j].pos.y += dir.y;
            mesh->tris[i].verts[j].pos.z += dir.z;

        }

    }

    mesh->centroid.x += dir.x;
    mesh->centroid.y += dir.y;
    mesh->centroid.z += dir.z;

}

void Mesh_rotation(Mesh *mesh, Vec3 deg) {
    Vec3 pivot = mesh->centroid;

    for (int i = 0; i < mesh->count; i++) {
        for (int j = 0; j < 3; j++) {
            Vec3 p = mesh->tris[i].verts[j].pos;

            // Translate to pivot
            p.x -= pivot.x;
            p.y -= pivot.y;
            p.z -= pivot.z;

            // Rotate around X
            if (deg.x != 0) {
                float y = p.y, z = p.z;
                p.y = y * cos(deg.x) - z * sin(deg.x);
                p.z = y * sin(deg.x) + z * cos(deg.x);
            }

            // Rotate around Y
            if (deg.y != 0) {
                float x = p.x, z = p.z;
                p.x = x * cos(deg.y) + z * sin(deg.y);
                p.z = -x * sin(deg.y) + z * cos(deg.y);
            }

            // Rotate around Z
            if (deg.z != 0) {
                float x = p.x, y = p.y;
                p.x = x * cos(deg.z) - y * sin(deg.z);
                p.y = x * sin(deg.z) + y * cos(deg.z);
            }

            // Translate back
            p.x += pivot.x;
            p.y += pivot.y;
            p.z += pivot.z;

            mesh->tris[i].verts[j].pos = p;
        }
    }
}

void Mesh_draw(Mesh *mesh, SDL_Renderer *renderer, Vec3 camera, float fov, float aspect, float near, float far, int width, int height) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        //Mesh_draw(&mesh, renderer, camera, fov, aspect, near, far, width, height);
        
        // Draw all triangles in the mesh
        for (size_t i = 0; i < mesh->count; i++) {
            Tri3D tri = mesh->tris[i];

            Vec3 a = tri.verts[0].pos;
            Vec3 b = tri.verts[1].pos;
            Vec3 c = tri.verts[2].pos;
            
            Vec3 normal, line1, line2;
            //bac face culling
            
            line1.x = tri.verts[1].pos.x - tri.verts[0].pos.x;
            line1.y = tri.verts[1].pos.y - tri.verts[0].pos.y;
            line1.z = tri.verts[1].pos.z - tri.verts[0].pos.z;

            line2.x = tri.verts[2].pos.x - tri.verts[0].pos.x;
            line2.y = tri.verts[2].pos.y - tri.verts[0].pos.y;
            line2.z = tri.verts[2].pos.z - tri.verts[0].pos.z;

            normal.x = line1.y *line2.z - line1.z * line2.y; 
            normal.y = line1.z *line2.x - line1.x * line2.z; 
            normal.z = line1.x *line2.y - line1.y * line2.x; 
            float normalMag = Vec3_mag(normal);
            normal.x /= normalMag;
            normal.y /= normalMag;
            normal.z /= normalMag;
            //if(normal.z < 0) {
            //check for normals to determine passing on rendering triangels.
            if (
                    normal.x * (tri.verts[0].pos.x - camera.x) +
                    normal.y * (tri.verts[0].pos.y - camera.y) +
                    normal.z * (tri.verts[0].pos.z - camera.z)  < 0.0f //if we set it to be  > 0.0f we essentially flip the norms
                ) {

                //light source 
                Vec3 light = Vec3(0,0, -1);
                // Y=0.2126⋅R+0.7152⋅G+0.0722⋅B
                float dp = Vec3_dotProduct(normal, Vec3_unitVector(light));
                // Project
                perspective_projection(&a, fov, aspect, near, far);
                perspective_projection(&b, fov, aspect, near, far);
                perspective_projection(&c, fov, aspect, near, far);
                
                Vertex v1 = Vertex(a, Color_BLUE);
                Vertex v2 = Vertex(b, Color_BLUE);
                Vertex v3 = Vertex(c, Color_BLUE);

                //draw_triangle(renderer, (Tri3D){.verts = {v1,v2,v3}}, width, height);
                //fillTriangle(renderer, Vertex(a, tri.verts[0].color), Vertex(b, tri.verts[1].color), Vertex(c, tri.verts[0].color), width, height, camera);
                Tri3D_fillTriangleLum(renderer, Vertex(a, tri.verts[0].color), Vertex(b, tri.verts[1].color), Vertex(c, tri.verts[0].color), dp, width, height, camera);

                //printf("\ncolor c : <%u, %u, %u, %u>, dp %f", co.r, co.g, co.b, co.a, dp);
                //Tri3D_fillTriangleSolid(renderer, Vertex(a, tri.verts[0].color), Vertex(b, tri.verts[1].color), Vertex(c, tri.verts[0].color), Color_calcLuminance(Color_WHITE, dp), width, height, camera);


            }
           // fillTriangle(renderer, Vertex(a, tri.verts[0].color), Vertex(b, tri.verts[1].color), Vertex(c, tri.verts[0].color), width, height);
            
            //fillTriangleSolid(renderer, Vertex(a, tri.verts[0].color), Vertex(b, tri.verts[1].color), Vertex(c, tri.verts[0].color), width, height);
            /*if ((abs(move_back.z)  > 20 && !flip) || (move_back.z < -20 && flip)) {
                Vec3_scale(&move_back, -1);
                flip = !flip;
            }*/
        
        }
        
        //Mesh_rotation(&mesh, Vec3(.01, 0.005, .01));
        //printf("\r\nRotation Matrix <%f, %f, %f>", rotate_vec.x, rotate_vec.y, rotate_vec.z);
}
#endif

