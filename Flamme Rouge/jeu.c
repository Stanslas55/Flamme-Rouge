#define _CRT_SECURE_NO_WARNINGS
#pragma once

#include "jeu.h"

#include <string.h>

jeu* Initialisation(int p_nbJoueur)
{	
	int i = -1, j = -1, pos = -1, joueur = 1, total = p_nbJoueur * 2, cartes[15] = { 0 }, plateau[NB_CASES] = { 0 };	  
	char chaine[3] = { 0 }; // Contient une chaine de 2 caractères, sans oublier "\0", d'ou les 3 cases.
	
	cycliste* tab_cyclistes = (cycliste*)calloc(total, sizeof(cycliste)); // Chaque joueur possède 2 vélos, un Rouleur, et un Sprinter.
	jeu* terrain = (jeu*) calloc(1, sizeof(jeu));		
	terrain->m_nbJoueurs = p_nbJoueur;

	for (i = 0; i < NB_CASES; i++) // On rempli le plateau de jeu vide.
	{ 
		strcpy(terrain->m_plateau[BAS][i], MOTIF);
		strcpy(terrain->m_plateau[HAUT][i], MOTIF);
	}
	for (i = 0; i < total; i++)
	{
		// Si i est pair, on rempli un tableau de cartes associé à un Rouleur.		
		(i % 2 == ROULEUR) ? Init_paquet(cartes, ROULEUR) : Init_paquet(cartes, SPRINTEUR); // Sinon, on rempli un tableau de cartes associé à un Sprinter.		
		for (j = 0; j < 15; j++) tab_cyclistes[i].m_paquet[j] = cartes[j]; // On recopie le tableau de cartes dans la structure.
	}	
	for (i = 0; i < total; i++)
	{		
		do
		{		
			pos = 0;
			CLS;
			(i % 2 == ROULEUR) ? printf("Joueur %d, inscrivez la position de depart de votre Rouleur R%d [1,5]\n> ", joueur, joueur) : 
							     printf("Joueur %d, inscrivez la position de depart de votre Sprinteur S%d [1,5]\n> ", joueur, joueur);
			scanf("%d", &pos);	
			getchar();			
		} while (!Valide(pos, 1, 5) || (plateau[pos - 1] == 2));		
		// L'utilisateur rentre naturellement la position de son cycliste, mais on stocke l'information à la manière d'un tableau, d'ou le -1.
		tab_cyclistes[i].m_indexesPlateau[LIGNES] = plateau[pos - 1]; 
		tab_cyclistes[i].m_indexesPlateau[COLONNES] = pos - 1;
		plateau[pos - 1]++; // On incrémente le nombre de cyclistes présent sur la plateau à l'indice pos - 1.
		
		(i % 2 == ROULEUR) ? sprintf(chaine, "R%d", joueur) : sprintf(chaine, "S%d", joueur); // On créer la chaine indiquant le cycliste et son numéro.
			
		strcpy(terrain->m_plateau[plateau[pos - 1] - 1][pos - 1], chaine); // On fait une copie de la chaine, à la case du cycliste.
		joueur = (i % 2 == SPRINTEUR) ? joueur + 1 : joueur;
	}
	terrain->m_cyclistes = tab_cyclistes;	
	return terrain;
}

void Nb_joueur(int *p_nbJoueur)
{		
	do // On demande le nombre de joueurs tant que le nombre est faux.
	{
		*p_nbJoueur = 0;
		CLS;
		printf("Combien de joueurs ? [2;4]\n> ");
		scanf("%d", p_nbJoueur);
		getchar();
	} while (!Valide(*p_nbJoueur, 2, 4));
}

void Init_paquet(int p_cartes[15], char p_mode) 
{
	int j = -1, k = -1, compteur = -1, indice = 0;

	if (p_mode == ROULEUR) // On genere un nombre aléatoire entre 3 et 7 pour les Rouleurs.
	{
		compteur = 3;
		for (j = 0; j < 5; j++)
		{
			for (k = 0; k < 3; k++)
			{
				p_cartes[indice] = compteur;
				indice++;
			}
			compteur++;				
		}			
	}
	if (p_mode == SPRINTEUR) // On genere un nombre aléatoire entre 2 et 6 pour les Sprinteur et si on tombe sur 6, alors c'est un 9.
	{ 
		compteur = 2;
		for (j = 0; j < 5; j++)
		{
			for (k = 0; k < 3; k++)
			{
				p_cartes[indice] = compteur;
				indice++;
			}
			compteur++;
			if (compteur == 6)	compteur = 9;			
		}
	}
	Melange(p_cartes);
}

void Test_init(jeu* p_terrain) 
{	
	int i = -1, j = -1, borne = p_terrain->m_nbJoueurs * 2;
	printf("\nCartes des paquets : \n\n");
	for (i = 0; i < borne; i++) 
	{
		printf("%s [%d,%d] ", Chaine_j(i), p_terrain->m_cyclistes[i].m_indexesPlateau[LIGNES], p_terrain->m_cyclistes[i].m_indexesPlateau[COLONNES]);
		print1D(j, p_terrain->m_cyclistes[i].m_paquet,"[%d]", 15)
		printf("\n");
	}
	printf("\nCartes des defausses : \n\n");
	for (i = 0; i < borne; i++)
	{
		printf("%s [%d,%d] ", Chaine_j(i), p_terrain->m_cyclistes[i].m_indexesPlateau[LIGNES], p_terrain->m_cyclistes[i].m_indexesPlateau[COLONNES]);
		print1D(j, p_terrain->m_cyclistes[i].m_defausse, "[%d]", 15)
		printf("\n");
	}
	printf("\n");
}

void Affiche_jeu(jeu *p_terrain) 
{	
	int i = -1;
	printf("\n");
	print2D(i, HAUT, p_terrain->m_plateau, "%s", NB_CASES)
	printf("\n\n");
	print2D(i, BAS, p_terrain->m_plateau, "%s", NB_CASES)
	printf("\n\n");
}


void Liberer_jeu(jeu *p_terrain) // On libère la tableau de m_cyclistes, et le m_plateau.
{	
	free(p_terrain->m_cyclistes);
	free(p_terrain);
}

void Tour(jeu *p_terrain)
{
	int  i = -1, j = -1, jouable = -1, joueur = 1, indice = 0, total = p_terrain->m_nbJoueurs * 2;
	int *deplacements = (int*)calloc(total, sizeof(int)), *ajouer = (int*)calloc(total, sizeof(int)), cartes[4] = { 0 };

	for (j = 0; j < total; j++)
	{
		// S'il n'y a plus assez de cartes dans le paquet...
		if (Retirer(p_terrain->m_cyclistes[j].m_paquet) == 1) Transfere(p_terrain, j); // ...On vide la défausse.												
		
		indice = 0;

		for (i = 0; i < 4; i++) // On sort les 4 premières cartes du paquet.
		{
			while (p_terrain->m_cyclistes[j].m_paquet[indice] == 0) indice++;						
			
			cartes[i] = p_terrain->m_cyclistes[j].m_paquet[indice];
			p_terrain->m_cyclistes[j].m_paquet[indice] = 0;
		}
		if (DEBUG) Test_init(p_terrain);
		printf("Cartes [1,4] : ");
		print1D(i, cartes, "%d ", 4)
		printf("\n");
		Affiche_defausse(p_terrain, j);
		do
		{
			(j % 2 == 0) ? printf("Joueur %d, choisissez la carte de votre Rouleur R%d [1,4]\n> ", joueur, joueur) : 
						   printf("Joueur %d, choisissez la carte de votre Sprinteur S%d [1,4]\n> ", joueur, joueur);
			scanf("%d", &ajouer[j]);
			getchar();
		} while (!Valide(ajouer[j], 1, 4));
		printf("\t\t\t\t\t\t\t Vous jouez %d\n", cartes[ajouer[j] - 1]);
		
		deplacements[j] = cartes[ajouer[j] - 1]; // Pour correspondre aux entrées d'un tableau.
		indice = 0;
		for (i = 0; i < 4; i++)
		{
			if (i != ajouer[j] - 1)
			{
				while (p_terrain->m_cyclistes[j].m_defausse[indice] != 0) indice++;												
				
				p_terrain->m_cyclistes[j].m_defausse[indice] = cartes[i];
				indice++;				
			}
		}		
		joueur = (j % 2 == 1) ? joueur + 1 : joueur; // Pour l'affichage seulement.						
	}		
	Mouvements(p_terrain, deplacements); // On applique les déplacements.
	if (DEBUG) {
		printf("Plateau apres Mouvements.\n");
		Affiche_jeu(p_terrain);
	}	
	Aspiration(p_terrain); // On applique l'effet d'Aspiration.	
	Fatigue(p_terrain); // On applique les cartes Fatigues aux têtes de pelotons.
	free(deplacements);
	free(ajouer);
}

int Retirer(int p_cartes[15]) 
{
	int i = -1, compteur = 0;
	for (i = 0; i < 15; i++) if (p_cartes[i] != 0) compteur++; // On compte le nombre de cartes restantes dans la pioche.					
	return (compteur < 4) ? 1 : 0;
}

int Fini(jeu* p_terrain)
{
	int i = -1;
	for (i = 0; i < p_terrain->m_nbJoueurs; i++) if (p_terrain->m_cyclistes[i].m_indexesPlateau[COLONNES] > (NB_CASES - 1) - 5) return 1;			
	return 0;
}

void Echange(int *p_a, int *p_b)
{
	int temp = *p_a;
	*p_a = *p_b;
	*p_b = temp;
}

void Melange(int p_paquet[15])
{		
	int i = -1, j = -1;	
	for (i = 15 - 1; i > 0; i--) // Depart du dernier element et Echange un par un.
	{		
		j = rand() % (i + 1); // indexe aléatoire entre 0 et i.		
		Echange(&p_paquet[i], &p_paquet[j]); // Echange p_paquet[i] avec l'element à indexe j. 
	}
}

void Transfere(jeu *p_terrain, int p_joueur)
{
	int i = -1, indice = 0, compteur = 0, *cartes = NULL;
	
	for (i = 0; i < 15; i++) if (p_terrain->m_cyclistes[p_joueur].m_defausse[compteur] != 0) compteur++; // On compte le nombre de cartes dans la défausse.					
	
	cartes = (int*)calloc(compteur, sizeof(int));		
	for (i = 0; i < compteur; i++) // On enlève les cartes de la défausse.
	{
		cartes[i] = p_terrain->m_cyclistes[p_joueur].m_defausse[i];
		p_terrain->m_cyclistes[p_joueur].m_defausse[i] = 0;
	}		
	for (i = 14; i >= 0; i--) // On rempli la pioche par les cartes de la défausse.
	{
		if (p_terrain->m_cyclistes[p_joueur].m_paquet[i] == 0)
		{
			p_terrain->m_cyclistes[p_joueur].m_paquet[i] = cartes[indice];
			indice++;
			if (indice == compteur)	break;				
		}
	}		
	Melange(p_terrain->m_cyclistes[p_joueur].m_paquet); // On mélange le paquet.
	free(cartes);					
}

void Mouvements(jeu *p_terrain, int *p_cartes)
{
	int i = -1, indice = -1, new_indice = -1, total = p_terrain->m_nbJoueurs * 2;
	char chaine[3] = { 0 };
	if (DEBUG)	print1D(i, p_cartes, "[%d]", total)
	
	for (i = 0; i < total; i++)
	{		
		strcpy(chaine, p_terrain->m_plateau[p_terrain->m_cyclistes[i].m_indexesPlateau[LIGNES]][p_terrain->m_cyclistes[i].m_indexesPlateau[COLONNES]]);
		indice = p_terrain->m_cyclistes[i].m_indexesPlateau[COLONNES]; // On recupere l'indice du velo i dans le plateau.
		new_indice = indice + p_cartes[i]; // On ajoute à ce nombre, la carte jouée.		
		
		while(1)
		{			
			if (strcmp(p_terrain->m_plateau[BAS][new_indice], MOTIF) == 0) // On vérifie que le nouvel indice est disponilbe (case 0)...	
			{				
				strcpy(p_terrain->m_plateau[BAS][new_indice], chaine); // Si c'est le cas, on copie la chaine au nouvel indice dans le plateau.
				sprintf(p_terrain->m_plateau[p_terrain->m_cyclistes[i].m_indexesPlateau[LIGNES]][p_terrain->m_cyclistes[i].m_indexesPlateau[COLONNES]], MOTIF);
				p_terrain->m_cyclistes[i].m_indexesPlateau[LIGNES] = BAS;
				p_terrain->m_cyclistes[i].m_indexesPlateau[COLONNES] = new_indice;
				break;
			}			
			if (strcmp(p_terrain->m_plateau[HAUT][new_indice], MOTIF) == 0) // Sinon on regarde à la case 1.
			{
				strcpy(p_terrain->m_plateau[HAUT][new_indice], chaine);
				sprintf(p_terrain->m_plateau[p_terrain->m_cyclistes[i].m_indexesPlateau[LIGNES]][p_terrain->m_cyclistes[i].m_indexesPlateau[COLONNES]], MOTIF);
				p_terrain->m_cyclistes[i].m_indexesPlateau[LIGNES] = HAUT;
				p_terrain->m_cyclistes[i].m_indexesPlateau[COLONNES] = new_indice;
				break;
			}					
			new_indice--; // Sinon, on test les cases d'avant.
			if (new_indice == indice)
			{
				printf("%s est bloque !\n", chaine);
				break;
			}			
		} 			
	}
	Decallages(p_terrain);
}

void Aspiration(jeu *p_terrain)
{
	int i = -1, j = -1, compteur = -1, memoire = -1, indice = -1;
	char chaine[3] = { 0 };
	for (i = 0; i < NB_CASES; i++)
	{		
		if (strcmp(p_terrain->m_plateau[BAS][i], MOTIF) != 0) // Si la case est remplie par un joueur.
		{
			compteur = 1;						
			while (strcmp(p_terrain->m_plateau[BAS][i + 1], MOTIF) != 0)
			{
				i++;
				compteur++;
			}
			if (strcmp(p_terrain->m_plateau[BAS][i + 2], MOTIF) != 0) // Aspiration.
			{ 
				indice = 0;				
				while (indice < compteur)
				{	
					strcpy(chaine, p_terrain->m_plateau[BAS][i - indice]);
					if (DEBUG) printf("On joue [%s]\n", chaine);
					memoire = Indice_j(chaine);
					strcpy(p_terrain->m_plateau[BAS][i - indice], MOTIF);
					p_terrain->m_cyclistes[memoire].m_indexesPlateau[COLONNES]++;
					strcpy(p_terrain->m_plateau[BAS][i - indice + 1], chaine);
					
					if (strcmp(p_terrain->m_plateau[HAUT][i], MOTIF) != 0) // On test si il y a un deuxième cycliste sur la case à aspirer.
					{
						strcpy(chaine, p_terrain->m_plateau[HAUT][i - indice]);
						if(DEBUG) printf("On joue [%s]\n", chaine);
						memoire = Indice_j(chaine);
						strcpy(p_terrain->m_plateau[HAUT][i - indice], MOTIF);
						p_terrain->m_cyclistes[memoire].m_indexesPlateau[COLONNES]++;
						strcpy(p_terrain->m_plateau[HAUT][i - indice + 1], chaine);
					}						
					indice++;					
				}	
				i = 0; // On remet i à 0 au cas ou il faudrait aspirer le nouveau groupe formé.				 
			}			
		}		
	}
}

int Indice_j(char p_chaine[3])
{	
	if (strcmp(p_chaine, "R1") == 0) return 0;
	if (strcmp(p_chaine, "S1") == 0) return 1;
	if (strcmp(p_chaine, "R2") == 0) return 2;
	if (strcmp(p_chaine, "S2") == 0) return 3;
	if (strcmp(p_chaine, "R3") == 0) return 4;
	if (strcmp(p_chaine, "S3") == 0) return 5;
	if (strcmp(p_chaine, "R4") == 0) return 6;
	if (strcmp(p_chaine, "S4") == 0) return 7;

	return -1;
}

int Difference_j(char p_chaine[3])
{
	if (p_chaine[0] == 'R') return ROULEUR;
	if (p_chaine[0] == 'S') return SPRINTEUR;
	return -1;
}

void Decallages(jeu *p_terrain)
{
	int i = -1,  total = p_terrain->m_nbJoueurs * 2;
	char chaine[3] = { 0 };
	for (i = 0; i < total; i++)
	{
		// On recopie la chaine associée au cycliste en cours de déplacement.
		strcpy(chaine, p_terrain->m_plateau[p_terrain->m_cyclistes[i].m_indexesPlateau[LIGNES]][p_terrain->m_cyclistes[i].m_indexesPlateau[COLONNES]]);
		
		if (p_terrain->m_cyclistes[i].m_indexesPlateau[LIGNES] == HAUT) // Si le cycliste est sur la partie gauche de la file...
		{
			if (DEBUG) printf("\n%s etage 1 !\n", chaine);
			// ...On regarde si la partie droite de la file est prise.
			if (strcmp(p_terrain->m_plateau[BAS][p_terrain->m_cyclistes[i].m_indexesPlateau[COLONNES]], MOTIF) == 0)
			{ // Si c'est le cas, on le déplace dans la file de droite.				
				if (DEBUG) printf("On rabat %s sur le cote !\n", chaine);
				sprintf(p_terrain->m_plateau[HAUT][p_terrain->m_cyclistes[i].m_indexesPlateau[COLONNES]], MOTIF);
				strcpy(p_terrain->m_plateau[BAS][p_terrain->m_cyclistes[i].m_indexesPlateau[COLONNES]], chaine);
				p_terrain->m_cyclistes[i].m_indexesPlateau[LIGNES] = BAS;
			}
		}
	}
}

void Fatigue(jeu *p_terrain) {

	int aspi = -1, memoire = -1, haut = -1, i = -1, j = -1;
	char chaine[3] = { 0 };
	for (i = 0; i < NB_CASES; i++)
	{
		aspi = -1;
		// Si la case est remplie par un joueur et que la suivante est vide.
		if (strcmp(p_terrain->m_plateau[BAS][i], MOTIF) != 0 && strcmp(p_terrain->m_plateau[BAS][i + 1], MOTIF) == 0) aspi = 0;
		
		if (aspi == 0) // On affecte une carte de Fatigue à la défausse du joueur.
		{			
			strcpy(chaine, p_terrain->m_plateau[BAS][i]);
			memoire = Indice_j(chaine);

			for (j = 0; j < 15; j++)
			{
				if (p_terrain->m_cyclistes[memoire].m_defausse[j] == 0)
				{
					printf("%s Recupere une carte Fatigue !\n", chaine);
					p_terrain->m_cyclistes[memoire].m_defausse[j] = 2;
					break;
				}
			}			
		}
	}
}

void Affiche_defausse(jeu *p_terrain, int p_joueur)
{
	int i = -1, borne = p_terrain->m_nbJoueurs * 2;
	printf("\t\t\t\t\t\t\t Defausse : ");	
	for (i = 0; i < borne; i++)	print1D(i, p_terrain->m_cyclistes[p_joueur].m_defausse, "[%d]", 15)		
	printf("\n");
}

void Gagnant(jeu *p_terrain)
{
	int i = -1, total = p_terrain->m_nbJoueurs * 2;
	for (i = 0; i < total; i++) if (p_terrain->m_cyclistes[i].m_indexesPlateau[COLONNES] > (NB_CASES - 1) - 5) printf("%s a gagne !\n", Chaine_j(i));
}

char* Chaine_j(int p_indexe)
{
	if (p_indexe == 0) return "R1";
	if (p_indexe == 1) return "S1";
	if (p_indexe == 2) return "R2";
	if (p_indexe == 3) return "S2";
	if (p_indexe == 4) return "R3";
	if (p_indexe == 5) return "S3";
	if (p_indexe == 6) return "R4";
	if (p_indexe == 7) return "S4";
	return "ER";
}

int Valide(int p_test, int p_min, int p_max)
{
	int i = -1;
	for (i = p_min; i < p_max + 1; i++)	if (i == p_test) return 1;			
	return 0;
}