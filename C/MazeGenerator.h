#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

typedef struct {
    char** data;
    unsigned int width;
    unsigned int height;
} Maze;

Maze* generate(unsigned int width, unsigned int height) {
    if (width < 1 || height < 1)
        return NULL;

    const unsigned int top_limit = UINT_MAX;
    if ((top_limit - 1) / 2 <= width || (top_limit - 1) / 2 <= height)
        return NULL;

    const unsigned int output_height = height * 2 + 1;
    const unsigned int output_width = width * 2 + 1;

    Maze* maze = (Maze*)malloc(sizeof(Maze));
    maze->width = output_width;
    maze->height = output_height;
    maze->data = (char**)malloc(output_height * sizeof(char*));

    for (unsigned int i = 0; i < output_height; ++i) {
        maze->data[i] = (char*)malloc(output_width * sizeof(char));
        for (unsigned int j = 0; j < output_width; ++j) {
            if ((i % 2 == 1) && (j % 2 == 1))
                maze->data[i][j] = ' ';
            else if (((i % 2 == 1) && (j % 2 == 0) && (j != 0) && (j != output_width - 1)) ||
                     ((j % 2 == 1) && (i % 2 == 0) && (i != 0) && (i != output_height - 1)))
                maze->data[i][j] = ' ';
            else
                maze->data[i][j] = '#';
        }
    }

    unsigned int* row_set = (unsigned int*)calloc(width, sizeof(unsigned int));
    unsigned int set = 1;

    srand(time(NULL));

    for (unsigned int i = 0; i < height; ++i) {
        for (unsigned int j = 0; j < width; ++j)
            if (row_set[j] == 0)
                row_set[j] = set++;

        for (unsigned int j = 0; j < width - 1; ++j) {
            int right_wall = rand() % 2;
            if (right_wall == 1 || row_set[j] == row_set[j + 1])
                maze->data[i * 2 + 1][j * 2 + 2] = '#';
            else {
                unsigned int changing_set = row_set[j + 1];
                for (unsigned int l = 0; l < width; ++l)
                    if (row_set[l] == changing_set)
                        row_set[l] = row_set[j];
            }
        }

        for (unsigned int j = 0; j < width; ++j) {
            int bottom_wall = rand() % 2;
            unsigned int count_current_set = 0;
            for (unsigned int l = 0; l < width; ++l)
                if (row_set[j] == row_set[l])
                    count_current_set++;
            if (bottom_wall == 1 && count_current_set != 1)
                maze->data[i * 2 + 2][j * 2 + 1] = '#';
        }

        if (i != height - 1) {
            for (unsigned int j = 0; j < width; ++j) {
                unsigned int count_hole = 0;
                for (unsigned int l = 0; l < width; ++l)
                    if (row_set[l] == row_set[j] && maze->data[i * 2 + 2][l * 2 + 1] == ' ')
                        count_hole++;
                if (count_hole == 0)
                    maze->data[i * 2 + 2][j * 2 + 1] = ' ';
            }

            for (unsigned int j = 0; j < width; ++j)
                if (maze->data[i * 2 + 2][j * 2 + 1] == '#')
                    row_set[j] = 0;
        }
    }

    for (unsigned int j = 0; j < width - 1; ++j) {
        if (row_set[j] != row_set[j + 1])
            maze->data[output_height - 2][j * 2 + 2] = ' ';
    }

    free(row_set);
    return maze;
}

void print_maze(const Maze* maze) {
    if (maze == NULL)
        return;

    for (unsigned int i = 0; i < maze->height; ++i) {
        for (unsigned int j = 0; j < maze->width; ++j)
            printf("%c", maze->data[i][j]);
        printf("\n");
    }
}

void free_maze(Maze* maze) {
    if (maze == NULL)
        return;

    for (unsigned int i = 0; i < maze->height; ++i)
        free(maze->data[i]);
    free(maze->data);
    free(maze);
}

