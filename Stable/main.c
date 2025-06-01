#include "Game.h"


int main()
{
    int32_t lignes,colonnes,choix,choix2;// Lignes colonnes
    int32_t controle,tour = 0,joueur=1; //La var joueur represente le tour du joueur, C'est tjr le J1 qui commence
    _Bool var_win = 1;
    int32_t col_occupied;
    printf("Bienvenue sur Puissance 4 !\n");
    printf("\n");
    int32_t pass = 1; // Initialisation de la variable pass à 1 pour entrer dans la boucle

    do {
        printf("Veuillez choisir le nombre de lignes pour la grille (La taille du jeu classique est de 7*7) :\n");
        controle = scanf("%d", &lignes);
        if (controle != 1 || lignes < 4 || lignes > 15)
        {
            while (getchar() != '\n'); // Vide le buffer
            printf("Entrée invalide. Le nombre de lignes choisi doit être compris entre 4 et 15. Veiller aussi à bien saisir un chiffre \n");
            pass = 1;
        }
        else
        {
            while (getchar() != '\n'); // Vide le buffer encore, sinon ce que tu as ecrit après est directement pris en compte
            printf("Veuillez choisir le nombre de colonnes pour la grille (entre 4 et 15) :\n");
            controle = scanf("%d", &colonnes);
            if (controle != 1 || colonnes < 4 || colonnes > 15)
            {
                while (getchar() != '\n'); // Vide le buffer
                printf("Entrée invalide. Le nombre de colonnes choisi doit être compris entre 4 et 15. Veiller aussi à bien saisir un chiffre \n");
                pass = 1;
            }
            else pass = 0;
        }
    } while (pass);
    printf("Très bien , grille sélectionnée : %d lignes par %d colonnes.\n", lignes, colonnes);


    //Les chiffres negatif sont proscrit
    // Taille max : 99*99
    // Saisie proteger avec un controle different de 0



    grille* G1 = alloc_grille(lignes,colonnes);
    node* T = init_tree(G1);
    printf("Selectionner une option dans le menu parmi : \n 1 - Mode 1J (Avec IA) \n 2 - Mode 2J \n 3- Règles du jeu \n 4- Quitter le jeu\n ");
    scanf("%d",&choix);
    if(choix == 1)
    {
        printf("Mode solo, préparez vous! Vous commencer en premier.\n");
        print_grille(G1,lignes,colonnes);
        while( var_win && tour < lignes*colonnes)
        {
            tour +=1;
            joueur =1;
            printf("Nous sommes au tour %d\n ", tour);
            printf("Joueur %d : Choisisser une colonnes ou deposer le jeton\n",joueur);
            // Relancer si le joueur voulait mettre un jeton dans une colonne pleine
            do
            {
                controle = scanf("%d",&choix);
                //Controle le type de caractère renseigné
                //Boucle de controle
                col_occupied = is_Colonne_Occupied(G1,choix);
                while ( (choix < 0 || choix > colonnes ) || controle == 0 || col_occupied != 0)
                {
                    printf("Choix impossible, veuiller recommencer\n");
                    while (getchar() != '\n');
                    controle = scanf("%d",&choix);
                    col_occupied = is_Colonne_Occupied(G1,choix);
                }

            } while (place_chip(G1,choix-1,joueur));

            
            
            print_grille(G1,lignes,colonnes);

            if(tour > 7)
            {
                var_win = test_win2(G1, joueur);
            }

            // Tour de l'IA

            printf("L'IA joue...\n");
            tour +=1;
            joueur = 2;
            tree(T,T->height,2); //Generation de l'arborescence

            int32_t best_col = 0; // Initialiser la meilleure colonne
            int32_t score = minimax(T, 5, 2, &best_col);
            printf("le score de L'IA est %d \n", score);
            choix2 = best_col; // Choix de la colonne par l'IA
            

            if( choix2 < 0 || choix2 > colonnes)
            {
                printf("Erreur l'IA essaye de placer le jeton en dehors de la grille: Choix %d\n",choix2);
                assert(0);
            }
            //else if(place_chip(G1,choix2-1,2) == 1)
            else if(place_chip(G1,choix2,2) == 1)
            {
                print_grille(G1,lignes,colonnes);
                printf("Erreur, l'IA veut placer sont jeton dans une colonne dejà occuper, choix : %d\n",choix2);
                assert(0);
            }
            else
            {
                print_grille(G1,lignes,colonnes);
                printf("L'IA à placé son jeton à la %d-ieme colonne\n",choix2);
                if(tour > 7)
                {
                    var_win = test_win2(G1,joueur);
                }
            }
        }
        if(!(var_win)) printf("Jeux terminé bravo !\n");
        else printf("Match nul , tenter encore une partie\n");

        free_grille(G1);
        return 0;
    }
    else if( choix == 2)
    {
        printf("Mode joueur contre joueur. C'est parti !\n");
        print_grille(G1,lignes,colonnes);
        /*
         //On continue de bouclé tant que var_win vaut 1(Personne n'a gagner)
         ET
         Si on a rempli toutes les cases d'ou le calcul lignes*colonnes
         //

         */
        while( var_win && tour < lignes*colonnes)
        {
            tour +=1;
            printf("Nous sommes au tour %d\n ", tour);
            printf("Joueur %d : Choisisser une colonnes ou deposer le jeton\n",joueur);
            // Relancer si le joueur voulait mettre un jeton dans une colonne pleine
            do
            {
                controle = scanf("%d",&choix);
                //Controle le type de caractère renseigné
                //Boucle de controle

                while ( (choix < 0 || choix > colonnes ) || controle == 0 )
                {
                    printf("Choix impossible, veuiller recommencer\n");
                    while (getchar() != '\n'); //Problème avec la saisie lorsque l'user tape un mauvais caractère à regler !!!
                    controle = scanf("%d",&choix);
                }

            } while (place_chip(G1,choix-1,joueur)); // On recommence toute la procedure si on ne peut pas mettre le jeton (Colonnes sature)
            //Le choix du joueur n'est pas indexé sur le tableau , on commence de 0 a .
            // C'est pourquoi on retire -1

            print_grille(G1,lignes,colonnes);
            // On skip les 7 premiers tour pour eviter de tester la condition trop tôt + C'est logique
            if (tour < 7)
            {
                if(tour%2 == 0) joueur=1;
                else joueur =2;
                //tree(T,T->height,2);
                //minimax(T,5);
                continue;// La on peut vraiment remplacer ça par un else
            }
            //tree(T,T->height,2);
            //minimax(T,5);
            var_win = test_win2(G1,joueur);
            if(tour%2 == 0) joueur=1;
            else joueur =2;


            /*
            if (tour == 10)
            {
                node * T1 = init_tree(G1);
                BFS_make(T1);
                DFS_printf(T1);
                //Fct printf pour noeud
                for (uint32_t m = 0; m < T1->nb_child ; m++)
                {
                    free(T1->child[m]);
                }

                free(T1);
            }
            */
        }
        if(!(var_win)) printf("Jeux terminé bravo !\n");
        else printf("Match nul , tenter encore une partie\n");

        free_grille(G1);
        return 0;
    }
    else if(choix == 3)
    {
        printf("Puissance 4 : Alignez 4 jetons de votre couleur horizontalement, verticalement ou en diagonale pour gagner. Les joueurs placent à tour de rôle un jeton dans la colonne de leur choix.\n");
        printf("Le premier joueur à aligner 4 jetons remporte la partie.\n");

        free_grille(G1);
        exit(0);
        //Penser à une option qui nous fait retouner aux choix (Peut être mettre en fonction la boucle de choix ?)
    }
    else
    {
        printf("vous avez quitté, vous avez perdu !\n");
        free_grille(G1);
        exit(0);
    }


}


