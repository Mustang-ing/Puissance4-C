#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#define BLEU 1
#define ROUGE 2
#define VIDE 0
#define INF_MAX 999
#define  INF_MIN -999
//enum joueur {1,2};
typedef enum direction {RL,LL,HC,DC,HRD,DLD,HLD,DRD} direction;


typedef enum direction2 {C1,C2,L1,L2,DA1,DA2,DB1,DB2} direction2;

/* C1 >> Colonne vers la droite
 * C2 >> Colonne vers la gauche
 * L1 >> Ligne vers le haut
type


 */
//Represente une case
typedef struct place{
    uint32_t value; // Représente la valeur d'une case : 0 = Case vide / 1 = Case occupé(J1) / 2 = Case occupe (J2)
    int32_t position_i;
    int32_t position_j;
}place;

//Represente la position d'une pièce Position x,y
typedef struct place_case{
    int32_t lignes;
    int32_t colonnes;
}place_case;

typedef struct grille{
    place** grill_place;
    uint32_t lignes;
    uint32_t colonnes;
    place_case* actual_pos;
}grille;


//Tree

typedef struct node{
    int32_t score;
    place_case* m; //Regarde si l'usage de ptr pour place case est justifié
    grille *copied_grill; // Attentiion, on prend toujours une copie de la grille
    uint32_t height;// Le champ hauteur permet de determiner à quelle hauteur de l'arbre on se trouve
    uint32_t nb_child;
    struct node* parent;//Le pointeur vers parent est essentielle pour remonter l'info dans Minimax Phase 2
    struct node** child; //Double ptr mais pour l'instant j'en vois pas d'interêt si ce n'est le fit qu'on plus de controle depuis le noeud père
    _Bool leaf;


}node;




void print_grille(grille* G,int32_t i, int32_t j);
grille* alloc_grille( int32_t lignes, int32_t colonnes);
void init_grille(grille* G);
void free_grille(grille* G);
_Bool place_chip(grille* G, int32_t colum, int32_t player); // La fonction retourne 0, si le placement c'est bien passé 1 sinon.
int32_t is_Already_Occupied(grille* G,int32_t colonnes);

_Bool test_win2(grille* G, int32_t player);
place_case offset_index(direction2 p, place_case index);
_Bool bord(grille* G, int32_t line, int32_t colum);



// IA et Minimax

node* init_tree(grille* G);
void alloc_node(node* child);
void init_void_child(node* child,node* parent);
void init_child(node* child, place_case x,int32_t player,node* parent);
void tree(node* parent, uint32_t  height, int32_t player);
grille* cloning(grille* G);
uint32_t get_number_of_child(grille* G);
void find_leaf(node* Node,int32_t* compteur);
int32_t find_leaf_v1(node* Node);
//void free_node(node* Node);
int32_t evaluerSegment(grille* G, int i, int j, int deltaI, int deltaJ);
int32_t evaluerGrille(grille* G);
int32_t get_best_score(node* Node,int32_t player);
int32_t compare(int32_t current_score, int32_t best_score, int32_t player);
//int32_t minimax(node* Node, int32_t height);
int32_t minimax(node* N, int32_t depth, int32_t player, int32_t* best_col);

void DFS_printf(node* Node);

_Bool is_Colonne_Occupied(grille* G,int32_t colonnes);


#endif
