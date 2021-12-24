#ifndef SPACECONST_H
#define SPACECONST_H

#include <unistd.h>
#include <iostream>
#include <termios.h>
#include <vector>
#include <thread>

#include "mingl/mingl.h"

#include "mingl/shape/rectangle.h"
#include "mingl/shape/circle.h"
#include "mingl/shape/line.h"
#include "mingl/shape/triangle.h"
#include "mingl/shape/triangle.h"

using namespace std;

const string KReset   ("0");
const string KNoir    ("30");
const string KRouge   ("31");
const string KVert    ("32");
const string KJaune   ("33");
const string KBleu    ("34");
const string KMAgenta ("35");
const string KCyan    ("36");


const char KEmpty               = ' ';  // case vide de l'écran
const char KRight               = '6';  // déplacement vers la droite
const char KLeft                = '4';  // Déplacement vers la gauche
const char KShoot               = '5';  // Lancé de torpille

//  Constantes liées à l'envahisseur

const string KInvadersColor (KJaune);  // Couleur de l'envahisseur
const char KInsideInvader       = 'W';  // caractèrere formant l'envahisseur
const char KMissile             = 'T';  // missile
const unsigned KInvadersSize    =  4;   // nombre de caractères qui forment l'envahisseur
const unsigned KInvadersMiddle  = KInvadersSize / 2;
const string KInvadersForm (KInvadersSize, KInsideInvader);  // forme de l'envahisseur

// Constantes liées au joueur

const string KMyColor (KVert);
const char KInsideMe            = 'A';
const char KTorpedo             = '|';
const unsigned KMySize          = 1;
const unsigned KMyMiddle        = KMySize / 2;
const string KMyForm (KMySize, KInsideMe);

// Constantes liées à l'espace (l'écran)

const unsigned KSizeLine   = 10;   // Nombre de lignes de l'écran (de l'espace)
const unsigned KSizeSpace  = 10;   // Nombre de colonnes de l'écran (de l'espace)

const unsigned KBegInvader = 0;    // Numéro de colonne où commence l'envahisseur
const unsigned KBegMe      = KSizeLine / 2;  // Numéro de colonne où commence le joueur

typedef vector <string> CVString;    // c'est le type de l'écran (l'espace, la matrice)

const string KEmptyLine (KSizeLine, KEmpty);  // Une ligne vide de la matrice

const unsigned KRatioMeInvaders = 4;    // Nombre de fois où c'est le tour du joueur pour un tour de l'envahisseur

nsShape::Rectangle ship(nsGraphics::Vec2D(20, 20), nsGraphics::Vec2D(100, 100), nsGraphics::KBlue);
const nsShape::Rectangle torpedo(nsGraphics::Vec2D(20, 20), nsGraphics::Vec2D(120, 120), nsGraphics::KGreen);
const unsigned decal = 80;
const int missileThickness = 6;
const int missileLength = 25;
const int squareSize = 50;
const vector<nsGraphics::RGBAcolor> invaderColor{nsGraphics::KRed,nsGraphics::KGreen,nsGraphics::KGreen,nsGraphics::KBlue,nsGraphics::KBlue}; // Couleurs par ligne des invaders
const vector<unsigned> cptAllDifficultyPlayerLife{0,3,1,1}; // Impose le nombre de vie au joueur selon le choixDifficulte dans main.cpp
//const vector<unsigned> cptMaxInvaderMissile{0,5,10,20}; // Impose le nombre max de missile invader à l'écran selon le choixDifficulte dans main.cpp

struct pos {
        size_t numLigne;
        size_t numCol;
};

struct CObjet {
        vector<pos> envahisseur;
        vector<pos> missile;
        vector<pos> vaisseau;
        vector<pos> torpille;
};

#endif // SPACECONST_H
