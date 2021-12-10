#include <iostream>
#include <vector>
using namespace std;


#define FPS_LIMIT 60

#include <iostream>
#include <thread>

#include "mingl/mingl.h"

#include "mingl/shape/rectangle.h"
#include "mingl/shape/circle.h"
#include "mingl/shape/line.h"
#include "mingl/shape/triangle.h"

using namespace std;

void dessiner(MinGL &window)
{

    // (Vous voyez par ailleurs que l'ordre d'affichage est important, le rectangle violet masque maintenant une partie du rectangle bleu.)
    // Vous pouvez combiner les différentes formes disponibles pour faire des choses plus complexes.

    // Voilà un bouton de fermeture.
    window << nsShape::Circle(nsGraphics::Vec2D(100, 320), 50, nsGraphics::KRed);
    window << nsShape::Line(nsGraphics::Vec2D(70, 290), nsGraphics::Vec2D(130, 350), nsGraphics::KWhite, 3.f);
    window << nsShape::Line(nsGraphics::Vec2D(130, 290), nsGraphics::Vec2D(70, 350), nsGraphics::KWhite, 3.f);

    // N'hésitez pas a lire la doc pour plus de détails.

    window << nsShape::Rectangle(nsGraphics::Vec2D(10, 10), nsGraphics::Vec2D(50, 50), nsGraphics::KYellow);
    window << nsShape::Rectangle(nsGraphics::Vec2D(10, 60), nsGraphics::Vec2D(50, 100), nsGraphics::KBlue);
    window << nsShape::Rectangle(nsGraphics::Vec2D(10, 110), nsGraphics::Vec2D(50, 150), nsGraphics::KRed);
    window << nsShape::Rectangle(nsGraphics::Vec2D(10, 160), nsGraphics::Vec2D(50, 200), nsGraphics::KGreen);
    window << nsShape::Rectangle(nsGraphics::Vec2D(10, 210), nsGraphics::Vec2D(50, 250), nsGraphics::KPurple);
    window << nsShape::Rectangle(nsGraphics::Vec2D(10, 260), nsGraphics::Vec2D(50, 300), nsGraphics::KWhite);
    window << nsShape::Rectangle(nsGraphics::Vec2D(10, 310), nsGraphics::Vec2D(50, 350), nsGraphics::KSilver);
    window << nsShape::Rectangle(nsGraphics::Vec2D(10, 360), nsGraphics::Vec2D(50, 400), nsGraphics::KCyan);
    window << nsShape::Rectangle(nsGraphics::Vec2D(10, 410), nsGraphics::Vec2D(50, 450), nsGraphics::KMagenta);
    window << nsShape::Rectangle(nsGraphics::Vec2D(10, 460), nsGraphics::Vec2D(50, 500), nsGraphics::KMaroon);


    window << nsShape::Circle(nsGraphics::Vec2D(450, 120), 100, nsGraphics::KRed);
    window << nsShape::Circle(nsGraphics::Vec2D(250, 120), 100, nsGraphics::KRed);
    window << nsShape::Rectangle(nsGraphics::Vec2D(300, 120), nsGraphics::Vec2D(400, 500), nsGraphics::KRed);
    window << nsShape::Circle(nsGraphics::Vec2D(350, 500), 50, nsGraphics::KRed);
}

int main()
{
    // Initialise le système
    MinGL window("01 - Shapes", nsGraphics::Vec2D(640, 640), nsGraphics::Vec2D(128, 128), nsGraphics::KBlack);
    window.initGlut();
    window.initGraphic();
    dessiner(window);

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
        dessiner(window);

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
