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
Curve * createGraph(size_t quality, double from, double upto, double (* func)(double));
Curve * createPolarCurve(size_t quality, double (* func)(double));
void destroyCurve(Curve * curve);
void displayCurve(Curve * curve, World * world, const Color color);
void captureCurve(Curve * curve, World * world, const char * name, const Color background, const Color axies, const Color line);

// curves
double lemniscata2(double phi);
double cardioide2(double phi);
double espiralLogaritmica(double phi);

double myCos(double phi);
double quadratic(double x);


/*************************************************************
 *  Main
**************************************************************/
int main(int argc, char const *argv[])
{
    World world = {50, {500, 400}, 1000, 800};

    Curve * lemniscata = createPolarCurve(300, lemniscata2);
    if (lemniscata == NULL)
    {
        perror("Problems to create a polar curve (malloc)");
        return -1;
    }    

    Curve * card = createPolarCurve(300, cardioide2);
    if (card == NULL)
    {
        perror("Problems to create a polar curve (malloc)");
        return -1;
    }

    Curve * espLog = createPolarCurve(100, espiralLogaritmica);
    if (espLog == NULL)
    {
        perror("Problems to create a polar curve (malloc)");
        return -1;
    }

    Curve * cosseno = createGraph(100, -10, 10, myCos);
    if (cosseno == NULL)
    {
        perror("Problems to create a polar curve (malloc)");
        return -1;
    }

    Curve * parabola = createGraph(100, -10, 10, quadratic);
    if (quadratic == NULL)
    {
        perror("Problems to create a polar curve (malloc)");
        return -1;
    }

    InitWindow(world.windowWidth, world.windowHeight, "Testing The Origin");

    SetTargetFPS(60);

    captureCurve(card, &world, "teste_da_funcao_print.png", RED, GREEN, BLUE);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        
        ClearBackground(BLACK);
        axis(&world, ORANGE);

        displayCurve(lemniscata, &world, PURPLE);
        displayCurve(card, &world, YELLOW);
        displayCurve(cosseno, &world, WHITE);
        displayCurve(parabola, &world, RED);

        EndDrawing();
    }    

    destroyCurve(lemniscata);
    destroyCurve(card);
    destroyCurve(espLog);
    destroyCurve(cosseno);

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

    Curve * curve = malloc(sizeof(curve));
    curve->points = points;
    curve->size = quality;

    return curve;
}

Curve * createGraph(size_t quality, double from, double upto, double (* func)(double))
{
    Vector2 * points = malloc(quality * sizeof(Vector2));
    if (points == NULL)
    {
        perror("Points allocation");
        return NULL;
    }

    double step = (upto - from)/quality;
    double x = from;

    for (size_t i = 0; i < quality; i++)
    {
        points[i].x = x + step * i;
        points[i].y = func(x + step * i);
    }
        
    Curve * curve = malloc(sizeof(curve));
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
    for (size_t i = 1; i < curve->size; i++)
    {
        int startX = (curve->points[i - 1].x) * world->scale + world->origem.x;
        int startY = world->windowHeight - ((curve->points[i - 1].y) * world->scale + world->origem.y);

        int endX = (curve->points[i].x) * world->scale + world->origem.x;
        int endY = world->windowHeight - ((curve->points[i].y) * world->scale + world->origem.y);

        DrawLine(startX, startY, endX, endY, color);
    }
    
}

void captureCurve(Curve * curve, World * world, const char * name, const Color background, const Color axes, const Color line)
{
    BeginDrawing();    
    ClearBackground(background);
    axis(world, axes);
    displayCurve(curve, world, line);
    EndDrawing();

    Image screen = LoadImageFromScreen();
    ExportImage(screen, name);
    UnloadImage(screen);
}



/*************************************************************
 *  Curves
**************************************************************/
double lemniscata2(double phi) // r = a²cos(2 * phi), a > 0.
{
    return 4 * cos(2 * phi);
}

double cardioide2(double phi) // r = a(1 + cos(phi)), a > 0
{
    return 2 + 2 * cos(phi);
}

double espiralLogaritmica(double phi) // r = exp(a * phi)
{
    return exp(0.5*phi);
}

double myCos(double phi)
{
    return cos(phi);
}

double quadratic(double x)
{
    return x*x;
}