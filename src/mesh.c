#include "mesh.h"
#include "triangle.h"
#include "array.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/file.h>
#include <string.h>
#include <stdio.h>

vec3_t cube_vertices[N_CUBE_VERTICES] = {
    {.x = -1, .y = -1, .z = -1}, // 1
    {.x = -1, .y =  1, .z = -1}, // 2
    {.x =  1, .y =  1, .z = -1}, // 3
    {.x =  1, .y = -1, .z = -1}, // 4
    {.x =  1, .y =  1, .z =  1}, // 5
    {.x =  1, .y = -1, .z =  1}, // 6
    {.x = -1, .y =  1, .z =  1}, // 7
    {.x = -1, .y = -1, .z =  1}, // 8
};

face_t cube_faces[N_CUBE_FACES] = {
    // front 
    {.a = 1, .b = 2, .c = 3}, 
    {.a = 3, .b = 4, .c = 1}, 
    // back 
    {.a = 5, .b = 7, .c = 8}, 
    {.a = 5, .b = 8, .c = 6}, 
    // right 
    {.a = 3, .b = 5, .c = 4}, 
    {.a = 5, .b = 6, .c = 4}, 
    // left 
    {.a = 7, .b = 1, .c = 2}, 
    {.a = 1, .b = 8, .c = 7}, 
    // top 
    {.a = 2, .b = 7, .c = 5}, 
    {.a = 2, .b = 5, .c = 3}, 
    // bottom 
    {.a = 1, .b = 4, .c = 6}, 
    {.a = 1, .b = 6, .c = 8}, 
};

void load_cube_mesh_data(void) {
    // load all cube vertices to mesh
    for(int i=0; i<N_CUBE_VERTICES; i++)
        array_push(mesh.vertices, cube_vertices[i]);
    
    // load all cube faces to mesh
    for(int i=0; i<N_CUBE_FACES; i++)
    {
        array_push(mesh.faces, cube_faces[i]);
    }
    
    // Set the rotation
    vec3_t angle = {0, 0, 0};
    mesh.rotation = angle;
}

char *readLine(int fd) {    
    char *line = malloc(100);
    char str[1];
    int i=0;
    int eof = 1;
    while(read(fd, str, 1) != 0)
    {
        eof = 0;
        if(str[0] == '\n')
            break;
        line[i++] = str[0];
    }
    if(eof)
        return NULL;
    line[i] = '\0';
    return line;
}

char **tokenize(char *line, char delimiter, int size) {
    char **tokens = NULL;
    char *str = NULL;
    for(int i=0; i<size; i++)
    {
        if(line[i] == delimiter)
        {
            // Push str to tokens array
            array_push(str, '\0');
            array_push(tokens, str);
            str = NULL;
            continue;
        }

        array_push(str, line[i]);
        // curr = strcat(curr, delimiter_str);
    }
    
    // Push str to tokens array
    array_push(str, '\0');
    array_push(tokens, str);

    return tokens;
}


void load_obj_file_data(char *filename) {
    // 1. Open the file
    int fd = open(filename, O_RDONLY);
    if(fd == -1)
    {
        fprintf(stderr, "Error opening file %s.\n", filename);
        exit(1);
    }

    // 2. Read Each line one by one: 
    while(1)
    {
        // 2.1. Read a line
        char *line = readLine(fd);
        if(line == NULL)
            break;

        // 2.2. Tokenize the line
            // 2.2.1. if first token is 'v', then read and push vertices
            // 2.2.2. if first token is 'f', then read and push faces
        char **tokens = tokenize(line, ' ', strlen(line));
        if(strcmp(tokens[0], "v") == 0)
        {
            // TODO: Push the vertex to mesh.vertices
            vec3_t vertex;
            vertex.x = atof(tokens[1]);
            vertex.y = atof(tokens[2]);
            vertex.z = atof(tokens[3]);
            
            array_push(mesh.vertices, vertex);            
            continue;
        }
        if(strcmp(tokens[0], "f") == 0)
        {
            // TODO: Push the face to mesh.faces
            int vertex_indices[3], texture_indices[3], normal_indices[3];
            
            for(int i=1; i<=3; i++)
            {
                vertex_indices[i-1] = atoi(tokenize(tokens[i], '/', strlen(tokens[i]))[0]);
                texture_indices[i-1] =atoi( tokenize(tokens[i], '/', strlen(tokens[i]))[1]);
                normal_indices[i-1] = atoi(tokenize(tokens[i], '/', strlen(tokens[i]))[2]);
            }

            face_t face;
            face.a = vertex_indices[0];
            face.b = vertex_indices[1];
            face.c = vertex_indices[2];

            array_push(mesh.faces, face);
            continue;

            // // TODO: Push the face to mesh.faces
            // face_t face;
            // face.a = atoi(tokenize(tokens[1], '/', strlen(tokens[1]))[0]);
            // face.b = atoi(tokenize(tokens[2], '/', strlen(tokens[2]))[0]);
            // face.c = atoi(tokenize(tokens[3], '/', strlen(tokens[3]))[0]);
        
            // array_push(mesh.faces, face);
            // continue;
        }
    }
    
    // 3. Close the file
    close(fd);

    // Set the rotation
    vec3_t angle = {3.5, 0, 0}; // {0.2, 0.1, 0.3};
    mesh.rotation = angle;
}

void print_mesh(void) {
    printf("Mesh vertices:\n");
    for(int i=0; i<array_length(mesh.vertices); i++)
        printf("<%f, %f, %f>\n", mesh.vertices[i].x, mesh.vertices[i].y, mesh.vertices[i].z);
    printf("\n");

    printf("Mesh faces: \n");
    for(int i=0; i<array_length(mesh.faces); i++)
        printf("[%d, %d, %d]\n", mesh.faces[i].a, mesh.faces[i].b, mesh.faces[i].c);
    printf("\n");    
}