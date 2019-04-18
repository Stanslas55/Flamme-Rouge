#pragma once

#ifndef JEU_H 
#define JEU_H

#define DEBUG 1
#ifdef DEBUG
// Module utile à la detection de fuites memoires.
#include "vld.h"
#endif

#define NB_CASES 78
#define MOTIF "[]"
#define BAS 0
#define HAUT 1
#define LIGNES 0
#define COLONNES 1
#define ROULEUR 0
#define SPRINTEUR 1
#define print1D(i, tab, tag, taille) for(i = 0; i < taille; i++) printf(tag, tab[i]);
#define print2D(i, j, tab, tag, taille) for(i = 0; i < taille; i++) printf(tag, tab[j][i]);
#define CLS system("cls")
#define PAUSE system("pause")

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Structure associée aux cyclistes : position sur le plateau, paquet de cartes associé, paquet de cartes défaussées.
typedef struct cycliste {

	int m_indexesPlateau[2]; // Position [0, 1][0, 81].
	int m_paquet[15]; // Paquet de 15 cartes associé à chaque cycliste.
	int m_defausse[15]; // Paquets de 15 cartes qu'on completera 3 cartes par 3 cartes.
}cycliste;

// Structure associée au jeu : Plateau de jeu, cyclistes et nombre de joueur.
typedef struct jeu {

	char m_plateau[2][NB_CASES][3]; // Tableau 2 lignes, NB_CASES colonnes, de chaines de 3 caractères ("Ri\0").
	cycliste *m_cyclistes; // Tableau de cyclistes R1 S1 R2 S2 [...] RN SN.
	int m_nbJoueurs; // m_nbJoueurs * 2 -> Taille du tableau de cyclistes.	
}jeu;

// 1 Fonction qui initialise une partie pour 'p_nbJoueur' joueurs.
jeu* Initialisation(int p_nbJoueur);

// 2 Fonction qui demande le nombre de joueur et le stocke dans 'p_nbJoueur'.
void Nb_joueur(int *p_nbJoueur);

// 3 Fonction qui rempli le tableau de cartes en fonction de 'mode' (0 : Rouleur, 1 : Sprinteur).
void Init_paquet(int p_cartes[15], char p_mode);

// 4 Fonction de test des indices des joueurs et des cartes associées avec le jeu '*p_terrain'.
void Test_init(jeu *p_terrain);

// 5 Fonction d'affichage du plateau et des personnages dessus.
void Affiche_jeu(jeu *p_terrain);

// 6 Fonction de libération de la mémoire associée à tout '*p_terrain'.
void Liberer_jeu(jeu *p_terrain);

// 7 Fonction réalisant un tour de jeu sur le terrain.
void Tour(jeu *p_terrain);

// 8 Fonction qui dit s'il faut Transferer la defausse dans la pioche.
int Retirer(int p_cartes[15]);

// 9 Fonction qui indique si il y a un vainqueur.
int Fini(jeu *p_terrain);

// 10 Fonction qui échange la valeur de deux pointeurs.
void Echange(int *p_a, int *p_b);

// 11 Fonction qui mélange le paquet de cartes.
void Melange(int p_paquet[15]);

// 12 Fonction qui Transfere les cartes de la defausse, les met à la suite des cartes de la pioche et mélange le tout.
void Transfere(jeu *p_terrain, int p_joueur);

// 13 Fonction qui applique les déplacements aux cyclistes.	
void Mouvements(jeu *p_terrain, int *p_cartes);

// 14 Fonction qui applique l'effet d'Aspiration aux cyclistes.
void Aspiration(jeu *p_terrain);

// 15 Fonction permettant de retrouver l'indice du joueur dans le tableau à partir de sa chaine.
int Indice_j(char p_chaine[3]);

// 16 Fonction qui différencie un Rouleur d'un Sprinter.
int Difference_j(char p_chaine[3]);

// 17 Fonction qui réalise les décallages adéquats aux cyclistes.
void Decallages(jeu *p_terrain);

// 18 Fonction qui applique les cartes Fatigue aux cyclistes concernés.
void Fatigue(jeu *p_terrain);

// 19 Fonction qui affiche la défausse du joueur courant.
void Affiche_defausse(jeu *p_terrain, int p_joueur);

// 20 Fonction qui affiche le ou les gagnants.
void Gagnant(jeu *p_terrain);

// 21 Fonction qui renvoie la chaine associée à l'indexe du tableau.
char* Chaine_j(int p_indexe);

// 22 Fonction qui vérifie si 'p_test' est bien compris dans les bornes [p_min;p_max].
int Valide(int p_test, int p_min, int p_max);

#endif // !JEU_H 