#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/*************************************************************
 * Types
**************************************************************/

typedef struct
{
    double scale;
    Vector2 origem;
    int windowWidth;
    int windowHeight;
} World;

typedef struct
{
    size_t size;
    Vector2 * points;
} Curve;




/*************************************************************
 *  Functions declarations
**************************************************************/

void axis(World * world, const Color color);
Curve * createGraph(size_t quality, double (* func)(double));
Curve * createPolarCurve(size_t quality, double (* func)(double));
void destroyCurve(Curve * curve);
void displayCurve(Curve * curve, World * world, const Color color);

// curves
double lemniscata2(double phi);
double cadioide2(double phi);


/*************************************************************
 *  Main
**************************************************************/
int main(int argc, char const *argv[])
{
    World world = {50, {500, 400}, 1000, 800};

    Curve * lemniscata = createPolarCurve(100, lemniscata2);
    if (lemniscata == NULL)
    {
        perror("Problems to create a polar curve (malloc)");
        return -1;
    }    

    Curve * card = createPolarCurve(50, cadioide2);
    if (card == NULL)
    {
        perror("Problems to create a polar curve (malloc)");
        return -1;
    }

    InitWindow(world.windowWidth, world.windowHeight, "Testing The Origin");

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        
        ClearBackground(BLACK);
        axis(&world, ORANGE);

        displayCurve(lemniscata, &world, PURPLE);
        displayCurve(card, &world, YELLOW);

        EndDrawing();
    }    

    destroyCurve(lemniscata);

    return 0;
}



/*************************************************************
 *  Functions Implementation
**************************************************************/

void axis(World * world, const Color color)
{
    DrawLine(0, world->windowHeight - world->origem.y, world->windowWidth, world->windowHeight - world->origem.y, color);
    DrawLine(world->origem.x, 0, world->origem.x, world->windowHeight, color);
}

Curve * createPolarCurve(size_t quality, double (* func)(double))
{
    Vector2 * points = malloc(quality * sizeof(Vector2));
    if (points == NULL)
    {
        perror("Points allocation");
        return NULL;
    }

    for (size_t i = 0; i < quality; i++)
    {
        points[i].x = func((2*PI*i)/quality) * cos((2*PI*i)/quality);
        points[i].y = func((2*PI*i)/quality) * sin((2*PI*i)/quality);
    }    

    Curve * curve = malloc(sizeof(Curve));
    curve->points = points;
    curve->size = quality;

    return curve;
}

void destroyCurve(Curve * curve)
{
    free(curve->points);
    free(curve);
}

void displayCurve(Curve * curve, World * world, const Color color)
{
    for (size_t i = 0; i < curve->size - 1; i++)
    {
        int startX = (curve->points[i].x) * world->scale + world->origem.x;
        int startY = world->windowHeight - ((curve->points[i].y) * world->scale + world->origem.y);

        int endX = (curve->points[i + 1].x) * world->scale + world->origem.x;
        int endY = world->windowHeight - ((curve->points[i + 1].y) * world->scale + world->origem.y);

        DrawLine(startX, startY, endX, endY, color);
    }
    
}



/*************************************************************
 *  Curves
**************************************************************/
double lemniscata2(double phi) // r = a²cos(2 * phi), a > 0.
{
    return 4 * cos(2 * phi);
}

double cadioide2(double phi) // r = a(1 + cos(phi)), a > 0
{
    return 2 + 2 * cos(phi);
}