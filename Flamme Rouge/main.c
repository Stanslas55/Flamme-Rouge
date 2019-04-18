#include "jeu.h"

int main() {
	int nb_joueur = -1, compteur = 1;
	CLS;
	srand((unsigned int)time(NULL));	
	
	Nb_joueur(&nb_joueur); // Demande du nombre de joueurs.	

	jeu* terrain = Initialisation(nb_joueur); // Initialisation du terrain de jeu.	
	
	while (!Fini(terrain)) // Boucle de jeu.
	{		
		printf("\n------\nTour %d\n------\n", compteur);
		Affiche_jeu(terrain);
		Tour(terrain);
		compteur++;
	}	
	Affiche_jeu(terrain);
	Gagnant(terrain);
	printf("\nLe jeu est fini !\n");
	Liberer_jeu(terrain);
	PAUSE;
	return 0;
}