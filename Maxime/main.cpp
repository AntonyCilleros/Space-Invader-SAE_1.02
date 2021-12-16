#define FPS_LIMIT 60

#include <unistd.h>
#include <iostream>
#include <termios.h>
#include <vector>
#include <space-const.h>
#include <thread>

#include "mingl/mingl.h"

#include "mingl/shape/rectangle.h"
#include "mingl/shape/circle.h"
#include "mingl/shape/line.h"
#include "mingl/shape/triangle.h"
#include "mingl/shape/triangle.h"

using namespace std;

void initGl(MinGL &window)
{
    // Pour dessiner quelque chose avec minGL 2, vous pouvez soit instancier l'objet dans une variable et l'injecter dans la fenêtre...
    window << enemy;
    // N'hésitez pas a lire la doc pour plus de détails.
}

void deplacementEnvahisseur(MinGL &window)
{
    for(unsigned i = 0; i < 950;++i)
        enemyPos.setX(enemyPos.setX() + 1);
}

void clearScreen ()
{
    cout << "\033[H\033[2J";
}

void Couleur (const string & coul)
{
    cout << "\033[" << coul <<"m";
}

void displaySpace(const CVString & Space){
    clearScreen();
    for(unsigned i = 0; i < Space.size(); ++i){
    cout << Space[i]<< endl;
    }
}

void set_input_mode (void)
{
  struct termios tattr;

  /* Make sure stdin is a terminal. */
  if (!isatty (STDIN_FILENO))
  {
    fprintf (stderr, "Not a terminal.\n");
    exit (EXIT_FAILURE);
  }

  /* Set the funny terminal modes. */
  tcgetattr (STDIN_FILENO, &tattr);
  tattr.c_lflag &= ~(ICANON|ECHO); /* Clear ICANON   and ECHO. */
  tattr.c_cc[VMIN] = 0;
  tattr.c_cc[VTIME] = 3;
  tcsetattr (STDIN_FILENO, TCSAFLUSH, &tattr);
}

// In main, call it first
int main() {
    MinGL window("01 - Shapes", nsGraphics::Vec2D(1000, 1000), nsGraphics::Vec2D(420, 0), nsGraphics::KBlack);
    window.initGlut();
    window.initGraphic();
    chrono::microseconds frameTime = chrono::microseconds::zero();
    // On fait tourner la boucle tant que la fenêtre est ouverte
    while (window.isOpen())
    {
        // Récupère l'heure actuelle
        chrono::time_point<chrono::steady_clock> start = chrono::steady_clock::now();

        // On efface la fenêtre
        window.clearScreen();

        // On dessine les formes géométriques
        initGl(window);

        deplacementEnvahisseur(window);

        // On finit la frame en cours
        window.finishFrame();

        // On vide la queue d'évènements
        window.getEventManager().clearEvents();

        // On attend un peu pour limiter le framerate et soulager le CPU
        this_thread::sleep_for(chrono::milliseconds(1000 / FPS_LIMIT) - chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now() - start));

        // On récupère le temps de frame
        frameTime = chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now() - start);
    }
    //set_input_mode();
    return 0;
}
