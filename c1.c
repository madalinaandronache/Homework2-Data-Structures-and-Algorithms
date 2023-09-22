/* Andronache Madalina-Georgiana - 312CC */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structs.h"

// creaza un nod nou din lista de adiacenta
ListNode *newListNode(int vertex, int weight) {
    ListNode *newNode = (ListNode *)malloc(sizeof(ListNode));
    newNode->vertex = vertex;
    newNode->weight = weight;
    newNode->next = NULL;

    return newNode;
}

// initializeaza un graf cu n noduri
Graph *initGraph(int n) {
    int i;

    Graph *newGraph = (Graph *)malloc(sizeof(Graph));
    newGraph->number = n;

    newGraph->list = (AdjList *)malloc(n * sizeof(AdjList));

    for (i = 0; i < n; i++) {
        newGraph->list[i].head = NULL;
        // initializam adancimea nodului i cu 0
        newGraph->list[i].depth = 0;
    }

    return newGraph;
}

// adauga in lista de adiancenta latura de la source la destination
void addEdge(Graph *graph, int source, int destination, int weight) {
    ListNode *newNode = newListNode(destination, weight);

    // daca nu are inca nici un vecin
    if (graph->list[source].head == NULL) {
        graph->list[source].head = newNode;
    } else {
        ListNode *last = graph->list[source].head;
        while (last->next != NULL) {
            last = last->next;
        }
        // parcurgem lista nodului sorce pana la ultimul nod existent
        // si adaugam nodul destination la finalul acesteia
        last->next = newNode;
    }
}

// adauga in vectorul de obiective places, sirul de caractere s, daca acesta
// nu exista deja in sir
char **addWord(char **places, int *n, char *s) {
    int i, lg_word, ok = 0;

    for (i = 0; i < *n; i++) {
        // daca cuvantul exista in vector deja ok = 1
        if (strcmp(places[i], s) == 0) {
            ok = 1;
            break;
        }
    }

    // daca cuvantul nu exista in vector
    if (ok == 0) {
        lg_word = strlen(s) + 1;
        places[*n] = (char *)malloc(lg_word * sizeof(char));
        strcpy(places[*n], s);
        (*n)++;
    }

    return places;
}

// returneaza pozitia unui nod in vectorul places, avand denumirea *s
// sau -1, daca nu gaseste denumirea *s in sir
int node_number(char **places, int n, char *s) {
    int i;

    for (i = 0; i < n; i++) {
        if (strcmp(places[i], s) == 0) {
            return i;
        }
    }

    return -1;
}

// creaza un graf neorientat cu n noduri si m muchii, folosindu-ne de datele din
// fisierul fin
Graph *createUndirectedGraph(Graph *graph, Edge *edges, int n, int m,
                             FILE *fin) {
    char **places = (char **)malloc(n * sizeof(char *));

    int nr_places = 0, i, weight, node1, node2;
    char s1[100], s2[100];

    for (i = 0; i < m; i++) {
        fscanf(fin, "%s", s1);
        fscanf(fin, "%s", s2);
        fscanf(fin, "%d", &weight);

        places = addWord(places, &nr_places, s1);
        places = addWord(places, &nr_places, s2);

        node1 = node_number(places, nr_places, s1);
        node2 = node_number(places, nr_places, s2);

        // deoarce graful este neorientat, adaugam atat muchia de la node1 la
        // node2 in lista de adiacenta, cat si pe cea de la node2 la node1
        addEdge(graph, node1, node2, weight);
        addEdge(graph, node2, node1, weight);

        // actualizam si vectorul de structuri Edge, avand la final lista
        // completa a muchiilor din graf
        edges[i].left = node1;
        edges[i].right = node2;
        edges[i].cost = weight;
    }

    // eliberam memoria alocata pentru places
    for (i = 0; i < n; i++) {
        free(places[i]);
    }
    free(places);

    return graph;
}

// sorteaza vectorul de structuri de tip Edge, crescator in functie de campul
// cost al fiecarui element
void sort(Edge *edges, int m) {
    int i, j;
    Edge aux;

    for (i = 0; i < m - 1; i++) {
        for (j = i + 1; j < m; j++) {
            if (edges[i].cost > edges[j].cost) {
                aux = edges[i];
                edges[i] = edges[j];
                edges[j] = aux;
            }
        }
    }
}

// determina numarul de componente conexe ale grafului primit ca argument
// si il returneaza, in plus, al doilea argument, components, va retine
// pe fiecare pozitie i, numarul componentei conexe din care face parte
// nodul i
int countComponents(Graph *graph, int *components, FILE *fout) {
    int i, count = 0;

    for (i = 0; i < graph->number; i++) {
        if (components[i] == -1) {
            dfs(i, components, graph, count);
            count++;
        }
    }

    fprintf(fout, "%d\n", count);
    return count;
}

// parcurgerea in adancime a unui graf dat ca parametru, vectorul components
// va retine pe o pozitie i, nr. componentei din care nodul i face parte
void dfs(int number, int *components, Graph *graph, int component) {
    components[number] = component;
    ListNode *node = graph->list[number].head;

    while (node != NULL) {
        if (components[node->vertex] == -1) {
            dfs(node->vertex, components, graph, component);
        }
        node = node->next;
    }
}

// aplica algoritmul Kruskal pentru o componenta conexa, determinand
// subarborele de cost minim
int kruskal(Edge *edges, int *components, int nr_component, int nr_nodes, int n,
            int m) {
    int i, j, node1, node2, index1, index2, total_cost = 0, nr = 0;

    int *tree = (int *)malloc((n + 1) * sizeof(int));

    // initial fiecare nod al componentei va fi intr-un alt subarbore
    for (i = 0; i < n; i++) {
        if (components[i] == nr_component) {
            tree[i] = i;
        }
    }

    i = 0;

    // cand obtinem nr_nodes - 1 muchii ne oprim
    while (nr < nr_nodes - 1) {
        // daca nodul din stanga muchiei apartine componentei actuale
        if (components[edges[i].left] == nr_component) {
            node1 = edges[i].left;
            node2 = edges[i].right;

            // daca se afla in subarbori diferiti
            if (tree[node1] != tree[node2]) {
                // se ia in considerare muchia dintre cele doua noduri
                // si se adauga la costul total
                nr++;
                total_cost += edges[i].cost;

                index1 = tree[node1];
                index2 = tree[node2];

                // actualizam vectorul in care memoram din ce subarbore face
                // parte fiecare nod
                for (j = 0; j < n; j++) {
                    if (components[j] == nr_component && tree[j] == index2) {
                        tree[j] = index1;
                    }
                }
            }
        }
        // se trece la urmatoarea muchie
        i++;
    }

    // eliberam memoria alocata pentru tree
    free(tree);

    return total_cost;
}

// returneaza numarul de noduri care fac parte din componenta conexa cu
// numarul number
int number_nodes(int *components, int number, int n) {
    int i, nr = 0;

    for (i = 0; i < n; i++) {
        if (components[i] == number) {
            nr++;
        }
    }

    return nr;
}

// sorteaza vectorul *cost cu n elemente in ordine crescatoare
void sort_cost(int *cost, int n) {
    int i, j, aux;

    for (i = 0; i < n - 1; i++) {
        for (j = i + 1; j < n; j++) {
            if (cost[i] > cost[j]) {
                aux = cost[i];
                cost[i] = cost[j];
                cost[j] = aux;
            }
        }
    }
}
