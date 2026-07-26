#include "raylib.h"
#include <math.h>
#include <stdio.h>

void displayCircle(int resolution, const Vector2 center, int radius, const Color color);

int main(int argc, char const *argv[])
{
    InitWindow(1000, 800, "test");

    SetTargetFPS(60);

    Vector2 center = {500, 400};
    int radius = 100;

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(WHITE);
        displayCircle(100, center, radius, BLACK);

        EndDrawing();
    }

    return 0;
}

void displayCircle(int resolution, const Vector2 center, int radius, const Color color)
{
    double scale = 2 * PI / resolution;

    for (size_t i = 0; i < resolution; i++)
    {
        DrawLine(center.x + radius * (cos(i * scale)), center.y + radius * (sin(i * scale)), center.x + radius * (cos((i + 1) * scale)), center.y + radius * (sin((i + 1) * scale)), color);
    }    
}
