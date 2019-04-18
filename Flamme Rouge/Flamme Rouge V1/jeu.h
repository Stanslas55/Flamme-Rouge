#pragma once
#ifndef JEU_H 
#define JEU_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Structure associ�e aux cyclistes : position sur le plateau, paquet de cartes associ�, paquet de cartes d�fauss�es.
typedef struct cycliste {

	int m_indice_plateau; // Position [0, 81].
	int m_cartes_deck[15]; // Paquet de 15 cartes associ� � chaque cycliste.
	int m_cartes_defausse[15]; // Paquets de cartes qu'on completera 3 cartes par 3 cartes.

}cycliste;

// Structure associ�e au jeu : Plateau de jeu, cyclistes et nombre de joueur.
typedef struct jeu {

	char plateau[2][82][3]; // Tableau 2 lignes, 82 colonnes, de chaines de 3 caract�res ("Ri\0").
	cycliste *joueurs; // Tableau de cyclistes R1 S1 R2 S2 [...] RN SN.
	int nb_joueurs; // nb_joueurs * 2 -> Taille du tableau de cyclistes.

}jeu;

// Fonction qui initialise une partie.
jeu* initialisation(int nb_joueur);

// Fonction qui demande le nombre de joueur et le stocke dans nb_joueur.
void nombre_joueur(int *nb_joueur);

// Fonction qui rempli le tableau de 15 cartes en fonction de mode (0 : Rouleur, 1 : Sprinteur).
void init_cartes(int *cartes, char mode);

// Fonction de test des indices des joueurs et des cartes associ�es.
void test_init(jeu *terrain);

// Fonction d'affichage du plateau et des personnages dessus.
void plateau_jeu(jeu *terrain);

// Fonction de lib�ration de la m�moire.
void free_game(jeu *terrain);

#endif // !JEU_H 
