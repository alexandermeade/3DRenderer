

typedef struct {
    Uint8 r,g,b,a;
}Color;

#define Color(r, g, b, a) (Color){r,g,b,a}

const Color Color_WHITE = Color(255,255,255,255);


#define ALPHA 255

#define Color_RED ((Color) {255, 0, 0, ALPHA}) 
#define Color_GREEN ((Color) {0, 255, 0, ALPHA})
#define Color_BLUE ((Color) {0, 0, 255, ALPHA})

Color Color_calcLuminance(Color color, float lum) {
    // Convert 0-255 values to 0-1 range for luminance calculation
    float r_norm = color.r / 255.0f;
    float g_norm = color.g / 255.0f;
    float b_norm = color.b / 255.0f;
    
    // Calculate current luminance using normalized values
    float currLum = 0.2126f * r_norm + 0.7152f * g_norm + 0.0722f * b_norm;
    
    // Prevent division by zero
    if (currLum < 0.001f) {
        currLum = 0.001f;
    }
    
    float scale = lum / currLum;
    
    // Scale the original color components and clamp to 0-255
    Uint8 r = (Uint8)fminf(fmaxf(color.r * scale, 0.0f), 255.0f);
    Uint8 g = (Uint8)fminf(fmaxf(color.g * scale, 0.0f), 255.0f);
    Uint8 b = (Uint8)fminf(fmaxf(color.b * scale, 0.0f), 255.0f);
    
    return (Color){r, g, b, 255};
}

