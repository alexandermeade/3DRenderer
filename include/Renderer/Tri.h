
typedef struct {
    Vec3 pos;
    Color color;    
}Vertex;



typedef struct {
    Vec3 points[3]; 
}Tri;

Tri Tri_init(Vec3 p1, Vec3 p2, Vec3 p3) {
    return (Tri) {
        .points ={p1, p2, p3}
    };
}

typedef struct {
    Vertex verts[3];
}Tri3D;

#define Vertex(pos, color) (Vertex) {pos, color}
#define Tri3D(v1, v2, v3) (Tri3D) {.verts={v1, v2, v3}} 

Tri3D Tri3D_init(Vertex v1, Vertex v2, Vertex v3) {
    return (Tri3D) {.verts = {v1,v2,v3}};
}


// Function to swap two points
static void swap(Vec3* p1, Vec3* p2) {
    Vec3 temp = *p1;
    *p1 = *p2;
    *p2 = temp;
}


// Function to sort vertices by their y-coordinate (ascending order)
void Tri3D_sortByY(Vec3 *p1, Vec3 *p2, Vec3 *p3) {
    if (p1->y > p2->y) swap(p1, p2);
    if (p2->y > p3->y) swap(p2, p3);
    if (p1->y > p2->y) swap(p1, p2);
}


float Tri3D_edge(Vec3 a, Vec3 b, Vec3 c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}


// SDL2 function to draw the triangle
void Tri3D_drawTriangle(SDL_Renderer *renderer, Tri3D tri, int screen_width, int screen_height) {
    // Project the 3D points to 2D
    Vec2 v1 = to_screen_space(tri.verts[0].pos, screen_width, screen_height);
    Vec2 v2 = to_screen_space(tri.verts[1].pos, screen_width, screen_height);
    Vec2 v3 = to_screen_space(tri.verts[2].pos, screen_width, screen_height);

    // Draw the triangle using SDL_RenderDrawLine
    SDL_RenderDrawLine(renderer, v1.x, v1.y, v2.x, v2.y);
    SDL_RenderDrawLine(renderer, v2.x, v2.y, v3.x, v3.y);
    SDL_RenderDrawLine(renderer, v3.x, v3.y, v1.x, v1.y);
}

void Tri3D_fillTriangleSolid(SDL_Renderer* renderer, Vertex p1, Vertex p2, Vertex p3, Color color, int screenWidth, int screenHeight, Vec3 cameraDirection) {

    // Proceed with the existing code to draw the triangle (your rasterization code)
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

    Vec2 v1 = to_screen_space(p1.pos, screenWidth, screenHeight);
    Vec2 v2 = to_screen_space(p2.pos, screenWidth, screenHeight);
    Vec2 v3 = to_screen_space(p3.pos, screenWidth, screenHeight);

    float minX = fminf(fminf(v1.x, v2.x), v3.x);
    float maxX = fmaxf(fmaxf(v1.x, v2.x), v3.x);
    float minY = fminf(fminf(v1.y, v2.y), v3.y);
    float maxY = fmaxf(fmaxf(v1.y, v2.y), v3.y);

    int startX = fmaxf(0, floorf(minX));
    int endX = fminf(screenWidth - 1, ceilf(maxX));
    int startY = fmaxf(0, floorf(minY));
    int endY = fminf(screenHeight - 1, ceilf(maxY));

    float area = (v2.x - v1.x) * (v3.y - v1.y) - (v2.y - v1.y) * (v3.x - v1.x);
    if (fabsf(area) < 1e-6f) return;

    for (int y = startY; y <= endY; y++) {
        for (int x = startX; x <= endX; x++) {
            Vec2 p = {x + 0.5f, y + 0.5f};

            float w1 = ((v2.x - p.x) * (v3.y - p.y) - (v2.y - p.y) * (v3.x - p.x)) / area;
            float w2 = ((v3.x - p.x) * (v1.y - p.y) - (v3.y - p.y) * (v1.x - p.x)) / area;
            float w3 = ((v1.x - p.x) * (v2.y - p.y) - (v1.y - p.y) * (v2.x - p.x)) / area;

            if (w1 >= 0 && w2 >= 0 && w3 >= 0) {

                SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
                SDL_RenderDrawPoint(renderer, x, y);
            }
        }
    }
}



// Modify your fillTriangle function to include backface culling
void Tri3D_fillTriangle(SDL_Renderer* renderer, Vertex p1, Vertex p2, Vertex p3, int screenWidth, int screenHeight, Vec3 cameraDirection) {
    // Check if the triangle is back-facing and skip if it is
    /*if (isBackFacing(p1.pos, p2.pos, p3.pos, cameraDirection)) {
        return;  // Don't render the triangle if it's facing away from the camera
    }*/

    // Proceed with the existing code to draw the triangle (your rasterization code)
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

    Vec2 v1 = to_screen_space(p1.pos, screenWidth, screenHeight);
    Vec2 v2 = to_screen_space(p2.pos, screenWidth, screenHeight);
    Vec2 v3 = to_screen_space(p3.pos, screenWidth, screenHeight);

    float minX = fminf(fminf(v1.x, v2.x), v3.x);
    float maxX = fmaxf(fmaxf(v1.x, v2.x), v3.x);
    float minY = fminf(fminf(v1.y, v2.y), v3.y);
    float maxY = fmaxf(fmaxf(v1.y, v2.y), v3.y);

    int startX = fmaxf(0, floorf(minX));
    int endX = fminf(screenWidth - 1, ceilf(maxX));
    int startY = fmaxf(0, floorf(minY));
    int endY = fminf(screenHeight - 1, ceilf(maxY));

    float area = (v2.x - v1.x) * (v3.y - v1.y) - (v2.y - v1.y) * (v3.x - v1.x);
    if (fabsf(area) < 1e-6f) return;

    for (int y = startY; y <= endY; y++) {
        for (int x = startX; x <= endX; x++) {
            Vec2 p = {x + 0.5f, y + 0.5f};

            float w1 = ((v2.x - p.x) * (v3.y - p.y) - (v2.y - p.y) * (v3.x - p.x)) / area;
            float w2 = ((v3.x - p.x) * (v1.y - p.y) - (v3.y - p.y) * (v1.x - p.x)) / area;
            float w3 = ((v1.x - p.x) * (v2.y - p.y) - (v1.y - p.y) * (v2.x - p.x)) / area;

            if (w1 >= 0 && w2 >= 0 && w3 >= 0) {
                Uint8 r = w1 * p1.color.r + w2 * p2.color.r + w3 * p3.color.r;
                Uint8 g = w1 * p1.color.g + w2 * p2.color.g + w3 * p3.color.g;
                Uint8 b = w1 * p1.color.b + w2 * p2.color.b + w3 * p3.color.b;

                SDL_SetRenderDrawColor(renderer, r, g, b, 255);
                SDL_RenderDrawPoint(renderer, x, y);
            }
        }
    }
}


void Tri3D_fillTriangleLumSolid(SDL_Renderer* renderer, Vertex p1, Vertex p2, Vertex p3, float lum, int screenWidth, int screenHeight, Vec3 cameraDirection, Color c) {
    // Check if the triangle is back-facing and skip if it is
    /*if (isBackFacing(p1.pos, p2.pos, p3.pos, cameraDirection)) {
        return;  // Don't render the triangle if it's facing away from the camera
    }*/

    // Proceed with the existing code to draw the triangle (your rasterization code)
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

    Vec2 v1 = to_screen_space(p1.pos, screenWidth, screenHeight);
    Vec2 v2 = to_screen_space(p2.pos, screenWidth, screenHeight);
    Vec2 v3 = to_screen_space(p3.pos, screenWidth, screenHeight);

    float minX = fminf(fminf(v1.x, v2.x), v3.x);
    float maxX = fmaxf(fmaxf(v1.x, v2.x), v3.x);
    float minY = fminf(fminf(v1.y, v2.y), v3.y);
    float maxY = fmaxf(fmaxf(v1.y, v2.y), v3.y);

    int startX = fmaxf(0, floorf(minX));
    int endX = fminf(screenWidth - 1, ceilf(maxX));
    int startY = fmaxf(0, floorf(minY));
    int endY = fminf(screenHeight - 1, ceilf(maxY));

    float area = (v2.x - v1.x) * (v3.y - v1.y) - (v2.y - v1.y) * (v3.x - v1.x);
    if (fabsf(area) < 1e-6f) return;

    for (int y = startY; y <= endY; y++) {
        for (int x = startX; x <= endX; x++) {
            Vec2 p = {x + 0.5f, y + 0.5f};

            float w1 = ((v2.x - p.x) * (v3.y - p.y) - (v2.y - p.y) * (v3.x - p.x)) / area;
            float w2 = ((v3.x - p.x) * (v1.y - p.y) - (v3.y - p.y) * (v1.x - p.x)) / area;
            float w3 = ((v1.x - p.x) * (v2.y - p.y) - (v1.y - p.y) * (v2.x - p.x)) / area;

            if (w1 >= 0 && w2 >= 0 && w3 >= 0) {
                Uint8 r = w1 * c.r + w2 * c.r + w3 * c.r;
                Uint8 g = w1 * c.g + w2 * c.g + w3 * c.g;
                Uint8 b = w1 * c.b + w2 * c.b + w3 * c.b;
                Color finalColor = Color_calcLuminance(Color(r,g,b,225), lum);
                SDL_SetRenderDrawColor(renderer, finalColor.r, finalColor.g, finalColor.b, 255);
                SDL_RenderDrawPoint(renderer, x, y);
            }
        }
    }
}


// Modify your fillTriangle function to include backface culling
void Tri3D_fillTriangleLum(SDL_Renderer* renderer, Vertex p1, Vertex p2, Vertex p3, float lum, int screenWidth, int screenHeight, Vec3 cameraDirection) {
    // Check if the triangle is back-facing and skip if it is
    /*if (isBackFacing(p1.pos, p2.pos, p3.pos, cameraDirection)) {
        return;  // Don't render the triangle if it's facing away from the camera
    }*/

    // Proceed with the existing code to draw the triangle (your rasterization code)
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

    Vec2 v1 = to_screen_space(p1.pos, screenWidth, screenHeight);
    Vec2 v2 = to_screen_space(p2.pos, screenWidth, screenHeight);
    Vec2 v3 = to_screen_space(p3.pos, screenWidth, screenHeight);

    float minX = fminf(fminf(v1.x, v2.x), v3.x);
    float maxX = fmaxf(fmaxf(v1.x, v2.x), v3.x);
    float minY = fminf(fminf(v1.y, v2.y), v3.y);
    float maxY = fmaxf(fmaxf(v1.y, v2.y), v3.y);

    int startX = fmaxf(0, floorf(minX));
    int endX = fminf(screenWidth - 1, ceilf(maxX));
    int startY = fmaxf(0, floorf(minY));
    int endY = fminf(screenHeight - 1, ceilf(maxY));

    float area = (v2.x - v1.x) * (v3.y - v1.y) - (v2.y - v1.y) * (v3.x - v1.x);
    if (fabsf(area) < 1e-6f) return;

    for (int y = startY; y <= endY; y++) {
        for (int x = startX; x <= endX; x++) {
            Vec2 p = {x + 0.5f, y + 0.5f};

            float w1 = ((v2.x - p.x) * (v3.y - p.y) - (v2.y - p.y) * (v3.x - p.x)) / area;
            float w2 = ((v3.x - p.x) * (v1.y - p.y) - (v3.y - p.y) * (v1.x - p.x)) / area;
            float w3 = ((v1.x - p.x) * (v2.y - p.y) - (v1.y - p.y) * (v2.x - p.x)) / area;

            if (w1 >= 0 && w2 >= 0 && w3 >= 0) {
                Uint8 r = w1 * p1.color.r + w2 * p2.color.r + w3 * p3.color.r;
                Uint8 g = w1 * p1.color.g + w2 * p2.color.g + w3 * p3.color.g;
                Uint8 b = w1 * p1.color.b + w2 * p2.color.b + w3 * p3.color.b;
                Color finalColor = Color_calcLuminance(Color(r,g,b,225), lum);
                SDL_SetRenderDrawColor(renderer, finalColor.r, finalColor.g, finalColor.b, 255);
                SDL_RenderDrawPoint(renderer, x, y);
            }
        }
    }
}


// Function to check if the triangle is back-facing
int Tri3D_isBackFacing(Vec3 A, Vec3 B, Vec3 C, Vec3 cameraDirection) {
    // Compute two edges of the triangle
    Vec3 edge1 = {B.x - A.x, B.y - A.y, B.z - A.z};
    Vec3 edge2 = {C.x - A.x, C.y - A.y, C.z - A.z};

    // Compute the normal of the triangle using the cross product
    Vec3 normal = crossProduct(edge1, edge2);

    // Dot product between the normal and camera direction
    float dot = Vec3_dotProduct(normal, cameraDirection);

    // If the dot product is negative, the triangle is facing the camera
    // If the dot product is positive, the triangle is facing away (back-facing)
    return dot > 0;
}


