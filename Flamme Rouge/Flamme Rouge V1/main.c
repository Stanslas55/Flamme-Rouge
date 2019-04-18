#define DEBUG
#ifdef DEBUG
// Module utile à la detection de fuites memoires.
#include "vld.h"
#endif

#include "jeu.h"

int main() {
	// Demande du nombre de joueurs.
	int nb_joueur;
	nombre_joueur(&nb_joueur);
	// Initialisation du terrain de jeu.
	jeu* terrain = initialisation(nb_joueur);		
	// Affiche le terrain de jeu.
	plateau_jeu(terrain);	
	// Libère toute la mémoire (allocations dynamiques).
	free_game(terrain);
	system("pause");
	return 0;
}