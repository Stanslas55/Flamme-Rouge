#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include "jeu.h"
#include <string.h>
jeu* initialisation(int nb_joueur) 
{	
	int i, j, pos = 0, joueur = 1, cartes[15], plateau[82] = { 0 };	

	// Chaque joueur possède 2 vélos, un Rouleur, et un Sprinter.
	// Les cases s'ordonnent de cette maniere : R1 S1 R2 S2 [...] RN SN.
	cycliste* tab_cyclistes = (cycliste*)calloc(nb_joueur * 2, sizeof(cycliste));
	jeu* terrain = (jeu*) calloc(1, sizeof(jeu));
	// chaine contient une chaine de 2 caractères, sans oublier "\0", d'ou les 3 cases.  
	char chaine[3];
	terrain->nb_joueurs = nb_joueur;
	for (i = 0; i < 82; i++) // On rempli le plateau de jeu vide.
	{ 
		strcpy(terrain->plateau[0][i] ,"|");
		strcpy(terrain->plateau[1][i] , "|");
	}
	for (i = 0; i < nb_joueur * 2; i++)
	{
		// Si i est pair, on rempli un tableau de cartes associé à un Rouleur (mode 0).
		// Sinon,  on rempli un tableau de cartes associé à un sprinter (mode 1). 
		(i % 2 == 0) ? init_cartes(&cartes, 0) : init_cartes(&cartes, 1);
		for (j = 0; j < 15; j++) // On recopie le tableau de cartes dans la structure.
		{
			tab_cyclistes[i].m_cartes_deck[j] = cartes[j];
		}
	}	
	for (i = 0; i < nb_joueur * 2; i++) 
	{		
		do
		{						
			(i % 2 == 0) ? printf("Joueur %d, inscrivez la position de depart de votre Rouleur [1,6]\n> ", joueur) : 
						   printf("Joueur %d, inscrivez la position de depart de votre Sprinteur [1,6]\n> ", joueur);
			scanf("%d", &pos);			
		} while ((pos < 1 || pos > 6) || (plateau[pos - 1] == 2));		
		// L'utilisateur rentre naturellement la position de son cycliste, mais on stocke l'information à la manière d'un tableau, d'ou le -1.
		tab_cyclistes[i].m_indice_plateau = pos - 1;		
		plateau[pos - 1]++;

		// On créer la chaine indiquant le cycliste et le numéro du joueur.
		(i%2 == 0) ? sprintf(chaine, "R%d", joueur) : sprintf(chaine, "S%d", joueur);		
	
		// On fait une copie de la chaine, à la case du cycliste.
		strcpy(terrain->plateau[plateau[pos - 1] - 1][pos - 1], chaine); 
		joueur = (i % 2 == 1) ? joueur + 1 : joueur;
	}
	terrain->joueurs = tab_cyclistes;	

	return terrain;
}

void nombre_joueur(int *nb_joueur) 
{	
	// On demande le nombre de joueurs tant que le nombre est faux.
	do 
	{
		system("cls");
		printf("Combien de joueurs ? [2;4]\n");
		scanf("%d", nb_joueur);
	} while (*nb_joueur < 2 || *nb_joueur > 4);
}

void init_cartes(int *cartes, char mode) 
{
	for (int i = 0; i < 15; i++) 
	{
		if (mode == 0) // On genere un nombre aléatoire entre 3 et 7 pour les Rouleurs.
		{
			cartes[i] = 3 + (rand() % (7 - 3 + 1));
		}
		if (mode == 1) // On genere un nombre aléatoire entre 2 et 6 pour les Sprinter et si on tombe sur 6, alors c'est un 9.
		{ 
			cartes[i] = 2 + (rand() % (6 - 2 + 1));
			if (cartes[i] == 6) cartes[i] = 9;
		}		
	}		
}

void test_init(jeu* terrain) 
{	
	system("cls");
	for (int i = 0; i < terrain->nb_joueurs * 2; i++) 
	{
		printf("case %d ", terrain->joueurs[i].m_indice_plateau);
		for (int j = 0; j < 15; j++)
		{
			printf("[%d]", terrain->joueurs[i].m_cartes_deck[j]);
		}			
		printf("\n");
	}
}

void plateau_jeu(jeu *terrain) 
{	
	int i;
	for (i = 0; i < 82; i++)
	{
		printf("%s", terrain->plateau[0][i]);
	}
	printf("\n");
	for (i = 0; i < 82 + 2 + 2; i++)
	{	
		printf("-");
	}
	printf("\n");
	for (i = 0; i < 82; i++)
	{
		printf("%s", terrain->plateau[1][i]);
	}
	printf("\n");
}


void free_game(jeu *terrain) // On libère la tableau de joueurs, et le plateau.
{	
	free(terrain->joueurs);
	free(terrain);
}