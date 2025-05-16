#ifndef MESHREADER_H
#define MESHREADER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to load a file's contents into a string
char* Utils_loadFile(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        perror("Could not open file");
        return NULL;
    }

    // Find the file size
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    rewind(file);

    // Allocate a buffer to hold the file content
    char* buffer = (char*)malloc(length + 1);
    if (!buffer) {
        perror("Memory allocation failed");
        fclose(file);
        return NULL;
    }

    // Read file into buffer
    fread(buffer, 1, length, file);
    buffer[length] = '\0'; // Null-terminate the string

    fclose(file);
    return buffer;
}

size_t parse_obj_vertices(const char *filename, Vertex *vertices, size_t max_vertices) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return 0;
    }

    size_t count = 0;
    char line[256];

    while (fgets(line, sizeof(line), file)) {
        if (line[0] == 'v' && line[1] == ' ') {
            if (count >= max_vertices) {
                fprintf(stderr, "Vertex limit (%zu) reached.\n", max_vertices);
                break;
            }

            float x, y, z;
            if (sscanf(line + 2, "%f %f %f", &x, &y, &z) == 3) {
                Vec3 pos = {x, y, z};
                vertices[count++] = Vertex(pos, Color_WHITE);
            }
        }
    }

    fclose(file);
    return count;
}

size_t parse_obj_faces(const char *filename, Vec4 *indexs, size_t max_faces) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return 0;
    }

    size_t count = 0;
    char line[256];

    while (fgets(line, sizeof(line), file)) {
        if (line[0] == 'f' && line[1] == ' ') {
            if (count >= max_faces) {
                fprintf(stderr, "Face limit (%zu) reached.\n", max_faces);
                break;
            }

            int x, y, z, w;
            int matched = sscanf(line + 2, "%d %d %d %d", &x, &y, &z, &w);
            if (matched == 3) {
                indexs[count++] = Vec4(x, y, z, 0); // Triangle
            } else if (matched == 4) {
                indexs[count++] = Vec4(x, y, z, w); // Quad
            }
        }
    }

    fclose(file);
    return count;
}

Mesh Mesh_objToMesh(const char *file) {
    Mesh mesh;
    const size_t max = 10000;

    // Allocate arrays for vertices and face indices
    Vertex *vertices = calloc(max, sizeof(Vertex));
    Vec4 *indexs = calloc(max, sizeof(Vec4));

    if (!vertices || !indexs) {
        perror("Failed to allocate memory");
        return mesh;  // Return an empty mesh if memory allocation failed
    }

    // Parse the vertices
    size_t count = parse_obj_vertices(file, vertices, max);
    for (size_t i = 0; i < count; i++) {
        Vertex v = vertices[i];
        printf("v %f %f %f\n", v.pos.x, v.pos.y, v.pos.z);
    }

    // Parse the face indices
    size_t indexs_count = parse_obj_faces(file, indexs, max);

    // Allocate space for triangles (each quad gets split into 2 triangles)
    Tri3D *triangles = calloc(indexs_count * 2, sizeof(Tri3D));  // Two triangles per quad
    size_t tri_index = 0;

    for (size_t i = 0; i < indexs_count; i++) {
        Vec4 v = indexs[i];
        printf("f %d %d %d %d\n", (int)v.x, (int)v.y, (int)v.z, (int)v.w);

        // Split the quad into two triangles:
        // Triangle 1: (v.x, v.y, v.z)
        // Triangle 2: (v.x, v.z, v.w)

        // Ensure you are using zero-based indices, so subtract 1 from each index.
        triangles[tri_index] = Tri3D(vertices[(int)v.x - 1], vertices[(int)v.y - 1], vertices[(int)v.z - 1]);
        tri_index++;

        if (v.w != 0) { // Only process second triangle if it's a quad
            triangles[tri_index] = Tri3D(vertices[(int)v.x - 1], vertices[(int)v.z - 1], vertices[(int)v.w - 1]);
            tri_index++;
        }
    }

    // Create mesh structure
    mesh = (Mesh) {
        .tris = triangles,
        .count = tri_index,  // Total number of triangles
        .centroid = Vec3(0, 0, 0)   // Placeholder for the centroid (you can compute it if needed)
    };

    // Free the allocated memory for vertices and indices
    free(vertices);
    free(indexs);

    return mesh;
}

#endif

