#ifndef VECTORS_H
#define VECTORS_H

typedef struct {
    float x;
    float y;
}Vec2;

typedef struct Vec3_t{
    float x;
    float y;
    float z;
    float w;
}Vec3;

typedef struct {
    float x;
    float y;
    float z;
    float w;
}Vec4;


#define Vec2(x, y) (Vec2) {x, y}
#define Vec3(x, y, z) (Vec3) {x, y, z}
#define Vec4(x, y, z, w) (Vec4) {x, y, z, w}
#define Vec3_mag(v) sqrtf(v.x*v.x + v.y*v.y + v.z*v.z)


// "normalized vector" accordingf to chatgpt :(

#define Vec3_add(v1, v2) ((Vec3){v1.x+v2.x, v1.y+v2.y, v1.z + v2.z})
#define Vec3_sub(v1, v2) ((Vec3){v1.x-v2.x, v1.y-v2.y, v1.z - v2.z})
#define Vec3_mult(v, s) ((Vec3){v.x * s, v.y * s, v.z * s})

Vec3 Vec3_unitVector(Vec3 v) {
    float mag = Vec3_mag(v);
    return Vec3(v.x/mag, v.y/mag, v.z/mag); 
}

Vec3 Vec3_init(float x, float y, float z) {
    return (Vec3) {x,y,z};
}

void Vec2_scale(Vec2 *v, float c) {v->x *= c; v->y*=c;}
void Vec3_scale(Vec3 *v, float c) {v->x *= c; v->y*=c; v->z*=c;}
Vec3 Vec3_scaleBack(Vec3 v, float c) {return Vec3(v.x * c, v.y*c, v.z*c);}


// Convert 3D vertex to 2D screen coordinates
Vec2 to_screen_space(Vec3 vertex, int screen_width, int screen_height) {
    Vec2 screen_pos;

    // Normalize to 2D screen space
    screen_pos.x = (vertex.x + 1) * 0.5 * screen_width;
    screen_pos.y = (1 - vertex.y) * 0.5 * screen_height;

    return screen_pos;
}

// Helper function to compute the normal of a triangle in 3D
Vec3 Vec3_crossProduct(Vec3 a, Vec3 b) {
    Vec3 result;
    result.x = a.y * b.z - a.z * b.y;
    result.y = a.z * b.x - a.x * b.z;
    result.z = a.x * b.y - a.y * b.x;
    return result;
}

// Helper function to compute the dot product
float Vec3_dotProduct(Vec3 a, Vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}



#endif
