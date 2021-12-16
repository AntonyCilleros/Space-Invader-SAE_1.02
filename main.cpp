#define FPS_LIMIT 60

#include <iostream>
#include <thread>

#include "mingl/mingl.h"
#include "mingl/shape/rectangle.h"
#include "mingl/shape/circle.h"
#include "mingl/shape/line.h"
#include "mingl/shape/triangle.h"


using namespace std;

nsGraphics::Vec2D JPos(480,900);

void joueur (MinGL & window)
{
    window << nsShape::Rectangle(JPos, JPos + nsGraphics::Vec2D(50, 50), nsGraphics::KWhite);
}

void clavier(MinGL &window)
{
    if (window.isPressed({'q', false}) && JPos.getX()> 0)
        JPos.setX(JPos.getX() - 10);
    if (window.isPressed({' ', false}) && JPos.getX() < 950)
        JPos.setX(JPos.getX() + 10);
}

void dessiner(MinGL &window)
{
    nsGraphics::Vec2D posj1(480, 500);
     window << nsShape::Rectangle (nsGraphics::Vec2D(posj1), nsGraphics::Vec2D(posj1+40), nsGraphics::KWhite);
}
int main()
{
    // Initialise le système
    MinGL window("01 - Shapes", nsGraphics::Vec2D(1000, 1000), nsGraphics::Vec2D(128, 128), nsGraphics::KBlack);
    window.initGlut();
    window.initGraphic();

    // Variable qui tient le temps de frame
    chrono::microseconds frameTime = chrono::microseconds::zero();

    // On fait tourner la boucle tant que la fenêtre est ouverte
    while (window.isOpen())
    {
        // Récupère l'heure actuelle
        chrono::time_point<chrono::steady_clock> start = chrono::steady_clock::now();

        // On efface la fenêtre
        window.clearScreen();

        // On dessine les formes géométriques
        joueur (window);
        clavier(window);

        // On finit la frame en cours
        window.finishFrame();

        // On vide la queue d'évènements
        window.getEventManager().clearEvents();

        // On attend un peu pour limiter le framerate et soulager le CPU
        this_thread::sleep_for(chrono::milliseconds(1000 / FPS_LIMIT) - chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now() - start));

        // On récupère le temps de frame
        frameTime = chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now() - start);
    }
    return 0;
}
