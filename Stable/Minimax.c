
#include "Game.h"


node* init_tree(grille* G)
{
    node* t = (node*) malloc(sizeof (node));
    assert(t != NULL);
    t->score = 0;
    t->m = (place_case*) malloc(sizeof(place_case));
    t->m->lignes= 0;
    t->m->colonnes = 0;
    t->copied_grill = G;
    t->parent = NULL;
    t->child = NULL;
    t->height = 5;
    t->nb_child = 0;
    t->leaf = 0;
    return t;

}

grille* cloning(grille* G)
{
    uint32_t i;
    assert(G != NULL);
    //Alocation de la struct de base
    grille* clone = (grille*) malloc(sizeof (grille));
    assert(clone != NULL);
    // Copie des champs
    clone->lignes = G->lignes;
    clone->colonnes = G->colonnes;
    clone->actual_pos = (place_case*)malloc(sizeof (place_case));
    assert(clone->actual_pos);
    clone->actual_pos->lignes = G->actual_pos->lignes;
    clone->actual_pos->colonnes = G->actual_pos->colonnes;

    //Allocation de grill_place
    clone->grill_place = (place**)malloc(G->lignes*sizeof(place*)); //On d'abord un tab de "lignes" lignes contenant des ptr double
    assert(clone->grill_place != NULL);
    if(G->lignes <= 0)
    {
        printf("Erreur avec le nombre de lignes\n ");
    }
    for ( i = 0; i < G->lignes ; i++)
    {
        *(clone->grill_place + i) = (place *) malloc(G->colonnes * sizeof(place));
        assert(clone->grill_place[i] != NULL);
    }
    //Initialisation
    for ( i = 0; i < clone->lignes; i++)
    {
        for (uint32_t  j = 0; j < clone->colonnes; j++)
        {
            clone->grill_place[i][j].value = G->grill_place[i][j].value;
            clone->grill_place[i][j].position_i = G->grill_place[i][j].position_i;
            clone->grill_place[i][j].position_j = G->grill_place[i][j].position_j;
        }
    }
    //printf("Sucessful clonning\n");
    return clone;

}



void init_child(node* child, place_case x,int32_t player,node* parent)
{

    // On initialise l'enfant
    child->score = 0;
    child->m = (place_case*) malloc(sizeof (place_case));
    child->m->lignes = x.lignes;
    child->m->colonnes = x.colonnes;

    child->copied_grill = cloning(parent->copied_grill); //On copie la grille du parent
    child->copied_grill->grill_place[x.lignes][x.colonnes].value = player; //On place le jeton

    child->height = parent->height -1;
    child->parent = parent;
    child->child = NULL;
    child->nb_child = 0;
    child->leaf = 0;

}

void init_void_child(node* child,node* parent)
{
    child->score = 0;
    child->m = NULL;
    child->copied_grill = NULL;
    child->parent = NULL;
    child->height = parent->height -1;
    child->nb_child = 0;
    child->child = NULL;
    child->leaf = 1;
}

uint32_t get_number_of_child(grille* G)
{
    if( G == NULL)
    {
        printf("Error in get num of child : grill is NULL\n");
        assert(0);
    }
    uint32_t max = G->colonnes;
    uint32_t nb_child = 0;
    for(uint32_t j = 0; j < max; j++)
    {
        if (is_Already_Occupied(G, j) != -1) nb_child += 1;
    }
    return nb_child;
}


void tree(node* parent, uint32_t height, int32_t player)
{
    if ( parent == NULL) {
        //printf("Feuille vide trouvé\n");
        return;
    }
    else if(height == 0 || parent->leaf == 1)
    {
        //printf("On est passé par là WRYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY\n");
        if(height == 0)
        {
            //printf("Hauteur max\n");
            parent->leaf = 1;
            return;
        }
        else
        {
            //printf("Feuille rencontrer\n");
            return;
        }

    }
    else
    {

        //On determine le nombre d'enfant
        uint32_t nb_child = get_number_of_child(parent->copied_grill);
        parent->nb_child = nb_child;
        //printf("Le nombre d'enfant réelle de ce parent est de %d\n", nb_child);

        //Du a des prob d'opti, on va quand même allouer le max d'enfant possible
        int32_t max = parent->copied_grill->colonnes;

        int32_t line;
        place_case x;
        parent->child = (node **) malloc(max * sizeof(node *)); // J'alloue les enfant
        assert(parent->child);

        //On boucle sur chacun des enfants
        for (uint32_t j = 0; j < max; j++) //i represente la colonnes
        {
            parent->child[j] = (node *) malloc(sizeof(node));// On alloue chaque enfant
            assert(parent->child[j]);
            line = is_Already_Occupied(parent->copied_grill, j);
            if (line != -1) // On peut mettre le jeton dans une colonnes
            {
                x.lignes = line;
                x.colonnes = j;
                init_child(parent->child[j], x, player, parent); // On met a jour la position avec une grille copier
            }
            else
            {
                //printf("On ne peut pas initaliser cette enfant, c'est une feuille\n");
                //init_void_child(parent->child[j],parent);// L'enfant aura ses ptr sur child à NULL
                // Initialise à tous les champs de ptr à NULL
                parent->child[j] = NULL;
                //parent->nb_child -= 1;
                //On baisse nombre de child initiale
            }
            tree(parent->child[j], height - 1, player);
        }

    }
}


// Reutilise le DFS pour estimer les leaf
void DFS_printf(node* Node) //Fct temporaire
{
    if(Node != NULL)
    {
        printf("Hauteur: %d | Nombre d'enfant %d | Score : %d | ",Node->height,Node->nb_child, Node->score);
        if(Node->leaf)
        {
            printf("Feuille : Oui\n");
        }
        else printf("Feuille : Non\n");
        if(Node->child != NULL)
        {

            for(int32_t i=0; i< Node->copied_grill->colonnes;i++)
            {
                DFS_printf(Node->child[i]);
            }
        }
    }
}

int32_t evaluerSegment(grille* G, int i, int j, int deltaI, int deltaJ) {
    int bleus = 0, rouges = 0;
    for (int k = 0; k < 4; k++) {
        if (G->grill_place[i][j].value == BLEU) bleus++;
        else if (G->grill_place[i][j].value == ROUGE) rouges++;
        i += deltaI;
        j += deltaJ;
    }
    if (bleus == 4) return 100;
    else if (rouges == 4) return -95;
    else if (bleus == 3 && rouges == 0) return 5;
    else if (rouges == 3 && bleus == 0) return -4;
    else if (bleus == 2 && rouges == 0) return 2;
    else if (rouges == 2 && bleus == 0) return -1;
    return 0;
}

// int evaluerGrille(grille*grille) {
//     int score = 0;

int32_t evaluerGrille(grille *G)
{
    int32_t score = 0;
    int32_t line = G->lignes;
    int32_t colum = G->colonnes;
    for (int i = 0; i < line; i++)
    {
        for (int j = 0; j < colum; j++)
        {

            if (j <= colum - 4)
                score += evaluerSegment(G, i, j, 0, 1); // Horizontal lin

            // Ensure there are enough spaces for a vertical segment of 4
            if (i <= line - 4)
                score += evaluerSegment(G, i, j, 1, 0); // Vertical lines

            // Ensure there are enough spaces for a diagonal up-right  segment of 4
            if (i <= line - 4 && j <= colum - 4)
                score += evaluerSegment(G, i, j, 1, 1); // Diagonal up - right

            // Ensure there are enough spaces for a diagonal down-right segment of 4
            if (i >= 3 && j <= colum - 4)
                score += evaluerSegment(G, i, j, -1, 1); // Diagonal down - right
        }
    }
    return score;
}

int32_t find_leaf_v1(node* Node)
{
    int32_t count=0;
    for (uint32_t i = 0; i < Node->copied_grill->colonnes; ++i)
    {
        if( Node->child[i] != NULL) count++;
    }
    return count;
}

int32_t compare(int32_t current_score, int32_t best_score, int32_t player)
{

    switch (player)
    {
        case 1:
            if( best_score > current_score) return current_score;
            else return best_score;
        case 2:
            if( best_score < current_score) return current_score;
            else return best_score;
        default:
            printf("Vous êtes qui ?\n");
            assert(0);
    }
}

int32_t minimax(node* N, int32_t depth, int32_t player, int32_t* best_col) {
    if (depth == 0 || N->leaf) {
        return evaluerGrille(N->copied_grill);
    }

    if (player == 1) { // Maximiser pour le joueur 1
        int32_t best_score = INT32_MIN;
        for (int32_t i = 0; i < N->nb_child; i++) {
            if (N->child[i] != NULL) {
                int32_t current_score = minimax(N->child[i], depth - 1, 2, best_col);
                if (current_score > best_score) {
                    best_score = current_score;
                    *best_col = i; // Mettre à jour la meilleure colonne
                }
            }
        }
        return best_score;
    } else { // Minimiser pour le joueur 2
        int32_t best_score = INT32_MAX;
        for (int32_t i = 0; i < N->nb_child; i++) {
            if (N->child[i] != NULL) {
                int32_t current_score = minimax(N->child[i], depth - 1, 1, best_col);
                if (current_score < best_score) {
                    best_score = current_score;
                    *best_col = i; // Mettre à jour la meilleure colonne
                }
            }
        }
        return best_score;
    }
}
