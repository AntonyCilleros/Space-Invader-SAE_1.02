#define FPS_LIMIT 60

#include <iostream>
#include <thread>
#include <unistd.h>
#include <termios.h>
#include <vector>
#include <space-const.h>
#include <stdio.h>
#include <time.h>
#include <algorithm>

#include "mingl/mingl.h"
#include "mingl/gui/text.h"
#include "mingl/shape/rectangle.h"
#include "mingl/shape/circle.h"
#include "mingl/shape/line.h"
#include "mingl/shape/triangle.h"

using namespace std;

// Structs

// Invaders

void invaderMovement(int &depH, unsigned depV, nsGraphics::Vec2D &enemyPos, bool gameIsOver){
    if (!gameIsOver)
        enemyPos.setX(enemyPos.getX() + depH);
    if(enemyPos.getX() == 140){
        depH = -1;
        enemyPos.setY(enemyPos.getY() + depV);
    }
    if(enemyPos.getX() == 10){
        depH = 1;
        enemyPos.setY(enemyPos.getY() + depV);
    }
}

nsGraphics::Vec2D newInvaderMissilePosition(nsGraphics::Vec2D invaderPos){
    nsGraphics::Vec2D invaderMissilePos;
    invaderMissilePos.setX(invaderPos.getX()+23);
    invaderMissilePos.setY(invaderPos.getY()+squareSize);
    return invaderMissilePos;
}

nsGraphics::Vec2D findInvaderShooter (vector<vector<bool>> invaderAlive, vector<vector<nsGraphics::Vec2D>> invaderPos){
    vector<bool> invaderColumnAlive(11); // Déclaration d'un vecteur indiquand si il y a au moins un invader en vie dans une des 11 colonnes d'invaders
    vector<unsigned> sortedInvaderColumnAlive;
    for (unsigned i = 0; i<11;++i){
        invaderColumnAlive[i] = invaderAlive[0][i] || invaderAlive[1][i] || invaderAlive[2][i] || invaderAlive[3][i] || invaderAlive[4][i];
        if (invaderColumnAlive[i]){
            sortedInvaderColumnAlive.push_back(i);
        }
    }
    unsigned invaderShooterColumn = rand()%sortedInvaderColumnAlive.size();
    for (unsigned i=4;i>=1;--i){
        if (invaderAlive[i][sortedInvaderColumnAlive[invaderShooterColumn]])
            return newInvaderMissilePosition(invaderPos[i][sortedInvaderColumnAlive[invaderShooterColumn]]);
    }
    return newInvaderMissilePosition(invaderPos[0][sortedInvaderColumnAlive[invaderShooterColumn]]);
}

void invaderMissileMovement(MinGL &window, vector<vector<bool>> invaderAlive, vector<vector<nsGraphics::Vec2D>> invaderPos, unsigned &cptInvaderMissileFired, vector<nsGraphics::Vec2D> &invaderMissilePos, unsigned cptInvaderAlive, bool gameIsOver)
{
    if (!gameIsOver && cptInvaderMissileFired < invaderMissilePos.size() && rand()%50==30 && cptInvaderAlive!=0){
        nsGraphics::Vec2D newInvaderMissilePos;
        for (unsigned i=0; i<invaderMissilePos.size();++i){
            if (invaderMissilePos[i] == nsGraphics::Vec2D(0,0)){
                invaderMissilePos[i] = findInvaderShooter(invaderAlive, invaderPos);
                ++cptInvaderMissileFired;
                break;
            }
        }
    }
    for (unsigned i=0; i<invaderMissilePos.size();++i){
        if (invaderMissilePos[i] != nsGraphics::Vec2D(0,0)){
            window << nsShape::Rectangle (invaderMissilePos[i], invaderMissilePos[i] + nsGraphics::Vec2D(missileThickness, +missileLength), nsGraphics::KRed);
            if (!gameIsOver)
                invaderMissilePos[i].setY(invaderMissilePos[i].getY()+5);
            if (invaderMissilePos[i].getY()>994){
                invaderMissilePos[i] = nsGraphics::Vec2D(0,0);
                --cptInvaderMissileFired;
            }
        }
    }
}

void invader(MinGL &window, nsGraphics::Vec2D &missilePos, nsGraphics::Vec2D &enemyPos, unsigned &score, vector<vector<bool>> &invaderAlive, vector<vector<nsGraphics::Vec2D>> &invaderPos, unsigned &cptInvaderAlive, vector<nsGraphics::RGBAcolor> invaderColor)
{
    vector<vector<nsShape::Rectangle>> invaderRect(5, vector<nsShape::Rectangle>(11, {nsGraphics::Vec2D(0, 0),nsGraphics::Vec2D(0, 0), nsGraphics::KBlack}));
    for(unsigned i=0;i<5;++i){
        for(unsigned j=0;j<11;++j){
            if (invaderAlive[i][j]){
                invaderPos[i][j] = enemyPos + nsGraphics::Vec2D(decal*j,decal*i);
                invaderRect[i][j] = nsShape::Rectangle (invaderPos[i][j], invaderPos[i][j] + nsGraphics::Vec2D(squareSize, squareSize), invaderColor[i]);
                if (missilePos.getX() < invaderPos[i][j].getX()+squareSize && missilePos.getY()-missileLength < invaderPos[i][j].getY()+squareSize && missilePos.getX()+missileThickness > invaderPos[i][j].getX() && missilePos.getY() > invaderPos[i][j].getY()){
                    invaderAlive[i][j] = false;
                    --cptInvaderAlive;
                    missilePos.setY(-21);
                    if (invaderColor[i]==nsGraphics::KRed)
                        score += 30;
                    else if (invaderColor[i]==nsGraphics::KGreen)
                        score += 20;
                    else
                        score += 10;
                }
                window << invaderRect[i][j];
            }
        }
    }
}

// général
bool gameOver (nsGraphics::Vec2D enemyPos, vector<vector<bool>> invaderAlive, unsigned cptInvaderAlive, short cptPlayerLife){
    vector<bool> invaderLineAlive(5, false); // Indique si la ligne d'invader a au moins un invader de vivant
    for (unsigned i=0;i<5;++i){
        for (unsigned j=0;j<11;++j){
            if (invaderAlive[i][j])
                invaderLineAlive[i]=true;
        }
    }
    for (short i=4;i>=0;--i){
        if (cptInvaderAlive==0 || cptPlayerLife==0 || (invaderLineAlive[i] && enemyPos.getY()+decal*i>=851))
            return true;
    }
    return false;
}

void playerLife (nsGraphics::Vec2D joueurPos, vector<nsGraphics::Vec2D> invaderMissilePos, short &cptPlayerLife, unsigned secondsTime, unsigned secondsOfInvulnerability, unsigned &impactTime, bool gameIsOver){
    for (unsigned i=0;i<invaderMissilePos.size();++i){
        if ((secondsTime-impactTime>secondsOfInvulnerability || (secondsTime<2 && impactTime==0)) && !gameIsOver && joueurPos.getX() < invaderMissilePos[i].getX()+missileThickness && joueurPos.getY() < invaderMissilePos[i].getY()+missileLength && joueurPos.getX()+squareSize > invaderMissilePos[i].getX() && joueurPos.getY()+squareSize > invaderMissilePos[i].getY()){
            --cptPlayerLife;
            impactTime=secondsTime;
        }
    }
}
// Actions du joueur
void clavier(MinGL &window, nsGraphics::Vec2D &joueurPos, bool &fire, bool &missileExistant, bool gameIsOver)
{
    if ((window.isPressed({'q', false}) || window.isPressed({81, false})) && joueurPos.getX()> 0 && !gameIsOver)
        joueurPos.setX(joueurPos.getX() - 10);
    if ((window.isPressed({'d', false}) || window.isPressed({68, false})) && joueurPos.getX() < 950 && !gameIsOver)
        joueurPos.setX(joueurPos.getX() + 10);
    if (window.isPressed({' ', false}) && missileExistant==false && !gameIsOver){
        fire=true;
        missileExistant=true;
    }
}

// Joueurs
void joueur (MinGL & window, nsGraphics::Vec2D &joueurPos)
{
    window << nsShape::Rectangle(joueurPos, joueurPos + nsGraphics::Vec2D(squareSize, squareSize), nsGraphics::KWhite);

}

// Missile Joueur
    // Position du missile initiale
nsGraphics::Vec2D positionMissile(nsGraphics::Vec2D joueurPos){
    nsGraphics::Vec2D missilePos;
    missilePos.setX(joueurPos.getX()+22);
    missilePos.setY(joueurPos.getY());
    return missilePos;
}
    // Affichage du missile
void missile(MinGL &window, nsGraphics::Vec2D &missilePos, bool &missileExistant, bool gameIsOver)
{
    window << nsShape::Rectangle (missilePos, missilePos + nsGraphics::Vec2D(missileThickness, -missileLength), nsGraphics::KRed);
    if (!gameIsOver)
        missilePos.setY(missilePos.getY()-10);
    if (missilePos.getY()-20<0){
        missileExistant = false;
        missilePos.setY(1000);
    }
}

//Fonctions générales
void clearScreen ()
{
    cout << "\033[H\033[2J";
}

void Couleur (const string & coul)
{
    cout << "\033[" << coul <<"m";
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

int main()
{
    srand (time(NULL)); // Initialisation de la seed de manière aléatoire
    // Initialisations des variables
    //! général
    bool gameIsOver = false;
    unsigned transparency = 0; // Variable utilisé pour contrôlé la transparence d'une forme
    unsigned cptTime=0; // Initialisation d'un compteur permettant de réduire la fréquence d'exécution de certaines fonctions en utilisant des modulo
    //! Invaders
    vector<nsGraphics::RGBAcolor> invaderColor(5); // Couleurs par ligne des invaders
    invaderColor[0]=nsGraphics::KRed; //
    invaderColor[1]=nsGraphics::KGreen; //
    invaderColor[2]=nsGraphics::KGreen; //
    invaderColor[3]=nsGraphics::KBlue; //
    invaderColor[4]=nsGraphics::KBlue; //
    vector<vector<bool>> invaderAlive(5, vector<bool>(11, true)); // Déclaration d'un vecteur indiquant si tel invader est en vie
    int depH = 1; // Vitesse de déplacement horizontal des invaders
    unsigned depV = 20; // Vitesse de déplacement vertical des invaders
    nsGraphics::Vec2D enemyPos(11,20); // Position des invaders avec leur position de début d'assigner
    unsigned cptInvaderMissileFired = 0; // Compte le nombre de missile invader affiché à l'écran
    unsigned cptInvaderAlive = 55; // Compte le nombre d'invader vivant
    vector<nsGraphics::Vec2D> invaderMissilePos(11); // Vecteur qui stoque la position des missiles tirés par les invaders
    vector<vector<nsGraphics::Vec2D>> invaderPos(5,vector<nsGraphics::Vec2D>(11)); // Position de chaque invader
    //! Joueurs
    nsGraphics::Vec2D joueurPos(480,900);
    unsigned score = 0;
    short cptPlayerLife = 3;
    unsigned secondsOfInvulnerability = 2;
    unsigned impactTime; // L'heure en second du dernier missile reçu par le joueur
    //! Missiles
    nsGraphics::Vec2D missilePos = nsGraphics::Vec2D(0,0);
    bool fire = false;
    bool missileExistant = false;

    // Initialise le système
    MinGL window("Space Invader", nsGraphics::Vec2D(1000, 1000), nsGraphics::Vec2D(128, 128), nsGraphics::KBlack);
    window.initGlut();
    window.initGraphic();

    // Variable qui tient le temps de frame
    chrono::microseconds frameTime = chrono::microseconds::zero();
    // Variables qui tient le temps en seconde
    chrono::seconds secondsTime=chrono::seconds::zero();
    // Constante qui retiens l'heure à laquelle le programme a été démarré
    const chrono::time_point<chrono::steady_clock> beginTime = chrono::steady_clock::now();

    // On fait tourner la boucle tant que la fenêtre est ouverte
    while (window.isOpen())
    {
        // Récupère l'heure actuelle
        chrono::time_point<chrono::steady_clock> start = chrono::steady_clock::now();


        // On efface la fenêtre
        window.clearScreen();

        // Affiche le missile du joueur
        if (missileExistant==true){
            if (fire==true){
                missilePos = positionMissile(joueurPos);
                fire = false;
            }
            missile(window, missilePos, missileExistant, gameIsOver);
        }
        // Affiche les invaders
        invader(window, missilePos, enemyPos, score, invaderAlive, invaderPos, cptInvaderAlive, invaderColor);
        // Réduit la vitesse de mouvement des invaders
        if(cptTime%2==0){
            invaderMovement(depH, depV, enemyPos, gameIsOver);
        }
        ++cptTime;
        // Fait tirer les invaders
        invaderMissileMovement(window, invaderAlive, invaderPos, cptInvaderMissileFired, invaderMissilePos, cptInvaderAlive, gameIsOver);
        // général
        playerLife(joueurPos,invaderMissilePos,cptPlayerLife, secondsTime.count(), secondsOfInvulnerability, impactTime, gameIsOver);
        gameIsOver = gameOver(enemyPos,invaderAlive, cptInvaderAlive, cptPlayerLife);
        // joueur
        joueur (window, joueurPos);
        clavier(window, joueurPos, fire, missileExistant, gameIsOver);
        // Assombri l'écran quand gameIsOver
        if (gameIsOver){
            if (transparency<150)
            transparency+=5;
            nsGraphics::RGBAcolor blacked {0,0,0,transparency};
            window << nsShape::Rectangle(nsGraphics::Vec2D(0,0), nsGraphics::Vec2D(1000,1000), blacked);
        }
        // Tableau de bord inGame

        window << nsGui::Text(nsGraphics::Vec2D(20, 40), to_string(score), nsGraphics::KWhite, nsGui::GlutFont::BITMAP_TIMES_ROMAN_24);
        window << nsGui::Text(nsGraphics::Vec2D(20, 80), to_string(cptInvaderMissileFired), nsGraphics::KWhite, nsGui::GlutFont::BITMAP_TIMES_ROMAN_24);
        window << nsGui::Text(nsGraphics::Vec2D(20, 120), to_string(secondsTime.count()), nsGraphics::KWhite, nsGui::GlutFont::BITMAP_TIMES_ROMAN_24);
        window << nsGui::Text(nsGraphics::Vec2D(20, 160), to_string(cptPlayerLife), nsGraphics::KWhite, nsGui::GlutFont::BITMAP_TIMES_ROMAN_24);
        // On finit la frame en cours
        window.finishFrame();

        // On vide la queue d'évènements
        window.getEventManager().clearEvents();

        // On attend un peu pour limiter le framerate et soulager le CPU
        this_thread::sleep_for(chrono::milliseconds(1000 / FPS_LIMIT) - chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now() - start));

        // On récupère le temps de frame
        frameTime = chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now() - start);
        if (!gameIsOver)
            secondsTime = chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now() - beginTime);
    }
    return 0;
}
