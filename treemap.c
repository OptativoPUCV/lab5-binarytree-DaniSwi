#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap *createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap *new = (TreeMap *)malloc(sizeof(TreeMap));
    new->root = NULL;
    new->current = NULL;
    new->lower_than = lower_than;
    return new;
}


void insertTreeMap(TreeMap *tree, void *key, void *value) {
    if (tree == NULL || tree->root == NULL) 
        return;
    if (searchTreeMap(tree, key) != NULL) 
        return;
    TreeNode *parent = NULL;
    TreeNode *aux = tree->root;
    while(aux!=NULL){
        parent = aux;
        if(is_equal(tree, key, aux->pair->key)) {
            tree->current = aux;
            return; 
        }
        else {
            if(tree->lower_than(key, aux->pair->key) == 1)
                aux = aux->left;
            else if(tree->lower_than(key, aux->pair->key) == 0)
                aux = aux->right;
            else
                return;
            }
        }
    TreeNode *new = createTreeNode(key, value);
    new->parent = parent;
    if(parent == NULL)
        tree->root = new;
    else {
        if(tree->lower_than(key, parent->pair->key) == 1)
            parent->left = new;
        else
            parent->right = new;
    }
    tree->current = new;
}

TreeNode *minimum(TreeNode * x){
    if(x == NULL) 
        return NULL;
    while(x->left != NULL)
        x = x->left;
    return x;
}


void removeNode(TreeMap *tree, TreeNode *node) {
    if(tree == NULL || tree->root == NULL || node->pair == NULL) 
        return;
    if(node->left == NULL && node->right == NULL){
        if(node->parent == NULL){
            tree->root = NULL;
            tree->current = NULL;
        } 
        else{
            if(node->parent->left == node){
                node->parent->left = NULL;
                tree->current = node->parent;
            }
            else{
                node->parent->right = NULL;
                tree->current = node->parent;
            }
        }
    }
    else{
        if(node->left != NULL && node->right != NULL){
            TreeNode *menor = minimum(node->right);
            node->pair->key = menor->pair->key;
            node->pair->value = menor->pair->value; 
        }
        else{
            TreeNode *child = (node->left != NULL)? node->left: node->right;
            if(node->parent == NULL){
                tree->root = child;
                child->parent = NULL;
            }
            else{
                if(node->parent->left == node){
                    node->parent->left = child;
                    child->parent = node->parent;
                } 
                else{
                    node->parent->right = child;
                    child->parent = node->parent;
                }
            }
        }
    }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) 
        return;
    if (searchTreeMap(tree, key) == NULL) 
        return;
    TreeNode* node = tree->current;
    removeNode(tree, node);
}

Pair *searchTreeMap(TreeMap *tree, void *key) {
    if(tree == NULL || tree->root == NULL) 
        return NULL;
    TreeNode *aux = tree->root;
    while(aux != NULL){
        if(is_equal(tree,key,aux->pair->key)==1){
            tree->current = aux;
            return aux->pair;
        } 
        if(tree->lower_than(key,aux->pair->key)==1) //Si la key es menor a la del nodo
            aux = aux->left; //Nos movemos para la izquierda
        else //Caso contrario
            aux = aux->right; //Para la derecha
    }
    return NULL; //En todo otro caso retornamos null
}


Pair *upperBound(TreeMap *tree, void *key) {
    if(tree == NULL || tree->root == NULL)
        return NULL;
    TreeNode *aux = tree->root;
    TreeNode *test = NULL;
    while(aux != NULL){
        if(is_equal(tree,key,aux->pair->key)){
            tree->current = aux;
            return aux->pair;
        } else if(tree->lower_than(key,aux->pair->key)==1){
            test = aux;
            aux = aux->left;
        } else 
            aux = aux->right;
    }
    if(test == NULL)
        return NULL;
    return test->pair;
}

Pair *firstTreeMap(TreeMap *tree) {
    if(tree == NULL || tree->root == NULL) 
        return NULL;
    TreeNode *aux = minimum(tree->root);
    tree->current = aux;
    return aux->pair;
}

Pair *nextTreeMap(TreeMap * tree) {
    if(tree == NULL || tree->root == NULL) 
        return NULL;
    TreeNode *aux = tree->current;
    if(aux->right != NULL){
        aux = minimum(aux->right);
        tree->current = aux;
        return aux->pair;
    } else {
        TreeNode *parent = aux->parent;
        while(parent != NULL && aux == parent->right){
            aux = parent;
            parent = parent->parent;
        }
        tree->current = parent;
        if(parent != NULL)
            return parent->pair;
        else
            return NULL;
    }
    return NULL;
}
