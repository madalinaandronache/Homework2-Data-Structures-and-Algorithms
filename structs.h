/* Andronache Madalina-Georgiana - 312CC */

#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdio.h>

// structura nod lista de adiacenta
typedef struct ListNode {
    int vertex;  // numarul nodului destinatie
    int weight;  // cost pentru a ajunge in nodul destinatie
    struct ListNode *next;
} ListNode;

// structura lista de adiacenta
typedef struct AdjList {
    int depth;
    struct ListNode *head;
} AdjList;

// structura graf
typedef struct Graph {
    int number;     // numarul de noduri
    AdjList *list;  // lista de adiacenta a nodurilor
} Graph;

// structura muchie
typedef struct Edge {
    int cost;
    int left;
    int right;
} Edge;

// functii cerinta 1

void addEdge(Graph *graph, int source, int destination, int weight);
void dfs(int number, int *components, Graph *graph, int component);
void sort(Edge *edges, int m);
void sort_cost(int *cost, int n);

int node_number(char **places, int n, char *s);
int number_nodes(int *components, int number, int n);
int countComponents(Graph *graph, int *components, FILE *fout);
int kruskal(Edge *edges, int *components, int nr_component, int nr_nodes, int n,
            int m);

char **addWord(char **places, int *n, char *s);

Graph *initGraph(int n);
Graph *createUndirectedGraph(Graph *graph, Edge *edges, int n, int m,
                             FILE *fin);

ListNode *newListNode(int vertex, int weight);

// functii cerinta 2

Graph *createDirectedGraph(Graph *graph, char **places, int m, FILE *fin);
Graph *getDepths(Graph *graph, int n, char **places, FILE *fin);

void coordinates(int *start, int *stop, int n, char **places);
void dijkstra(Graph *graph, int *previous, int source, int n);

int *getPath(int *prev, int number, int *lg);
int minDistance(float *distance, int *visited, int n);
int costPath(Graph *graph, int *path, int n);

#endif