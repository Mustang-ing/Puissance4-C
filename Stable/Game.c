//
// Created by ing-mustang on 15/01/24.
//
#include "Game.h"

grille* alloc_grille(int32_t lignes, int32_t colonnes)
{
    grille* G = (grille*) malloc(sizeof (grille));
    G->lignes = lignes;
    G->colonnes = colonnes;
    G->actual_pos =  (place_case*)malloc(sizeof (place_case));
    G->actual_pos->lignes = 0;
    G->actual_pos->colonnes = 0;
    init_grille(G);
    return G;
}

void init_grille(grille* G)
{
    uint32_t i,j;
    uint32_t lignes = G->lignes;
    uint32_t colonnes = G->colonnes;
    G->grill_place = (place**)malloc(lignes*sizeof(place)); //On d'abord un tab de "lignes" lignes contenant des ptr double
    if (G->grill_place == NULL)
    {
        printf("Erreur critique lors de l'allocation\n");
        assert(0);
    }
    for (i = 0; i < lignes ; i++)
    {
        *(G->grill_place+i) = (place*) malloc(colonnes* sizeof(place)); // Chacun de ses ptr double pointe vers un ptr "place" pointant vers un int32
    }

    for ( i = 0; i < lignes; i++)
    {
        for (j = 0; j < colonnes; j++)
        {
            G->grill_place[i][j].value = 0;
            G->grill_place[i][j].position_i = i;
            G->grill_place[i][j].position_j = j;
        }
    }
}

void print_grille(grille* G, int32_t i, int32_t j)
{
    uint32_t k,l;
    for ( k = 0; k < i ; ++k)
    {
        // La première boucle forme la ligne de '='
        printf("\033[31m"); // >> Rouge
        for (l = 0; l < j; ++l)
        {
            printf("====");
        }
        printf("\033[0m\n");// >> Blanc

        // Cette deuxieme boucle les cases

        for ( l = 0; l < j; ++l)
        {
            //printf("| %d ",(G->grill_place[k][l]).value);
            printf("\033[32m|\033[0m"); // >> Barre verte
            printf(" %d ",G->grill_place[k][l].value);
        }
        printf("\033[32m|\033[0m\n"); // >> Barre verte


    }
    //On fait presque toute la grille

    for (l = 0; l < j; ++l)
    {
        printf("\033[31m");
        printf("====");
    }
    printf("\033[0m\n");
    //Dernière ligne
    //system("Color1");
    // Upgrade pour plus de lisibilité

    for (l = 0; l < j ; l++)
    {
        printf(" ^  ");
    }
    printf("\n");
    for (l = 0;  l < j ; l++)
    {
        printf(" ¦  ");
    }
    printf("\n");
    for (l = 1;  l < j+1 ; l++)
    {
        printf(" %d  ",l);
    }
    printf("\n");

}

void free_grille(grille* G)
{
    uint32_t i;
    for ( i = 0; i < G->lignes; i++)
    {
        free(G->grill_place[i]);
    }
    free(G->actual_pos);
    free((G->grill_place));// Ne pas oublier de free le tableau de double pointeur
    free(G);
}

_Bool place_chip(grille* G, int32_t colum, int32_t player)
{
    int32_t line = is_Already_Occupied(G,colum);

    if( line == -1) return EXIT_FAILURE;
    else
    {
        G->grill_place[line][colum].value = player; //On ecrit
        //On va placer le pointeur dans ces position
        G->actual_pos->lignes = line;
        G->actual_pos->colonnes = colum;
        return EXIT_SUCCESS; // 0
    }
}

int32_t is_Already_Occupied(grille* G,int32_t colonnes)
{
    int32_t line = G->lignes -1;/*Au début, on va chercher à placer le jeton tout en bas. C'est pourquoi, on commence à la ligne 0*/
    int32_t value_token = G->grill_place[line][colonnes].value;
    while ( value_token != 0)
    {
        line -= 1;
        // Si après avoir décrémenter line on est négatif, cela veut dire que la colonnes est remplie
        if (line < 0)
        {
            //printf("Placement imposible, veuillez choisir une autre colonnes\n");
            return -1;// EXIT_FAILURE
        }
        value_token = G->grill_place[line][colonnes].value;
    }
    //On renvoie le n° de la ligne si on a trouver une case vide, ou -1 sinon.
    return line;
}

_Bool is_Colonne_Occupied(grille* G,int32_t colonnes)
{
    int32_t value_token = G->grill_place[0][colonnes-1].value;
    return (value_token != 0);/* On la valeur de la case est differente de 0
    autrement dit : Si la case ne vaut pas 0, c'est que la case est occupé. La fct renvoie 1
    Sinon elle est vide. La fct renvoie 0
    */
}


_Bool test_win2(grille* G, int32_t player)
{
    int32_t line = G->actual_pos->lignes;
    int32_t colum = G->actual_pos->colonnes;
    uint32_t value;
    //int32_t index_i = line;
    //int32_t index_j = colum;
    place_case index;
    uint32_t score;
    _Bool stop = 1;
    direction2 p;

    for (p = C1;  p <= DB2 ; p++)
    {
        index.lignes = line;
        index.colonnes = colum;
        stop=1;

        //Parcours dans le sens positif
        score = 1; //Le score commence à un, car la case qu'on lit est tjr lié au joueur
        index = offset_index(p,index);
        while (bord(G,index.lignes,index.colonnes) && stop)
        {
            value = G->grill_place[index.lignes][index.colonnes].value;
            if(  value == player )
            {
                score += 1;
                index = offset_index(p,index);
                stop =1;
            }
            else
            {
                //printf("Sortie on a rencontré %u\n",value);
                stop = 0;
            }
        }
        //Reset position
        index.lignes = line;
        index.colonnes = colum;
        stop=1;

        //Pacours sens inverse
        p++; //On passe dans la direction contraire
        //printf("Cas %d\n",p);
        index = offset_index(p,index);
        while (bord(G,index.lignes,index.colonnes) && stop)
        {
            value = G->grill_place[index.lignes][index.colonnes].value;
            if(  value == player )
            {
                score +=1;
                index = offset_index(p,index);
                stop =1;
            }
            else
            {
                //printf("Sortie on a rencontré %u\n",value);
                stop = 0;
            }
        }
        if( score >= 4)
        {
            printf("Joueur %d, à gagné\n",player);
            return 0;
        }
    }
    printf("Personne n'a gagner pour le moment\n");
    return 1;
}

place_case offset_index(direction2 p, place_case index)
{
    switch (p)
    {
        case C1:
            //printf("cas 1\n");
            index.colonnes += 1;
            return index;

        case C2:
            //printf("cas 2\n");
            index.colonnes -=1;
            return index;

        case L1:
            //printf("cas 3\n");
            index.lignes -= 1;
            return index;

        case L2:
            //printf("cas 4\n");
            index.lignes += 1;
            return index;

        case DA1:
            //printf("cas 5\n");
            index.lignes -= 1;
            index.colonnes += 1;
            return index;

        case DA2:
            //printf("cas 6\n");
            index.lignes += 1;
            index.colonnes -= 1;
            return index;

        case DB1:
            //printf("cas 7\n");
            index.lignes -= 1;
            index.colonnes -= 1;
            return index;

        case DB2:
            //printf("cas 8\n");
            index.lignes += 1;
            index.colonnes += 1;
            return index;
        default:
            printf("Erreur position\n");
            assert(0);
    }
}

_Bool bord(grille* G, int32_t line, int32_t colum)
{
    int32_t Max_lignes = G->lignes, Max_colonnes = G->colonnes;
    _Bool result1 = (line >= 0 && line < Max_lignes );
    _Bool result2 = (colum >= 0 && colum < Max_colonnes );
    if( result1 && result2) return 1;
    else return 0;// On allait lire hors zone
}

