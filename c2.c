/* Andronache Madalina-Georgiana - 312CC */

#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structs.h"

// creaza un graf orientat cu m muchii, folosindu-ne de datele din
// fisierul fin
Graph *createDirectedGraph(Graph *graph, char **places, int m, FILE *fin) {
    int i, node1, node2, weight, nr_places = 0;
    char s1[100], s2[100];

    for (i = 0; i < m; i++) {
        fscanf(fin, "%s", s1);
        fscanf(fin, "%s", s2);
        fscanf(fin, "%d", &weight);

        places = addWord(places, &nr_places, s1);
        places = addWord(places, &nr_places, s2);

        node1 = node_number(places, nr_places, s1);
        node2 = node_number(places, nr_places, s2);

        // deoarce graful este orientat, adaugam doar muchia de la node1 la
        // node2 in lista de adiacenta
        addEdge(graph, node1, node2, weight);
    }

    return graph;
}

// prin intermediul parametrilor start si stop, functia returneaza
// indicele nodului denumit "Insula" si indicele nodului denumit "Corabie"
// din vectorul places
void coordinates(int *start, int *stop, int n, char **places) {
    int i;

    for (i = 0; i < n; i++) {
        if (strcmp(places[i], "Insula") == 0) {
            *start = i;
        } else if (strcmp(places[i], "Corabie") == 0) {
            *stop = i;
        }
    }
}

// atribuie tuturor nodurilor adancimea corespunzatoare, citita din
// fisierul de intrare fin
Graph *getDepths(Graph *graph, int n, char **places, FILE *fin) {
    int i, j, depth, index;
    char s[100];

    for (i = 0; i < n; i++) {
        fscanf(fin, "%s", s);
        fscanf(fin, "%d", &depth);

        // gasim indicele nodului care are denumirea *s
        // folosindu-ne de vectorul places
        index = -1;
        for (j = 0; j < n; j++) {
            if (strcmp(places[j], s) == 0) {
                index = j;
                break;
            }
        }
        graph->list[index].depth = depth;
    }
    return graph;
}

// folosindu-ne de vectorul de tati *prev, returneaza calea valida
// pana la nodul cu numarul number
int *getPath(int *prev, int number, int *lg) {
    int lg_prev;
    int *path;

    // inseamna ca am ajuns la capatul cautarii si path contine un
    // singur element
    if (prev[number] == -1) {
        path = malloc(2 * sizeof(int));
        path[0] = number;
        *lg = 1;

        return path;
    }

    // se construieste recursiv calea catre nod
    int *prevPath = getPath(prev, prev[number], &lg_prev);

    path = realloc(prevPath, (lg_prev + 1) * sizeof(int));
    path[lg_prev] = number;
    *lg = lg_prev + 1;

    return path;
}

// determina indexul nodului care nu a fost inca vizitat si pentru
// care distanta este minima
int minDistance(float *distance, int *visited, int n) {
    float min = FLT_MAX;
    int index, i;

    for (i = 0; i < n; i++) {
        if (visited[i] == 0 && distance[i] <= min) {
            min = distance[i];
            index = i;
        }
    }

    return index;
}

// aplica algoritmul Dijkstra pornind din nodul source, determinand
// drumul de cost minim pentru toate celelalte noduri
void dijkstra(Graph *graph, int *previous, int source, int n) {
    int i, index, weight, destination, depth;
    float scor;

    float *distance = (float *)malloc((n + 1) * sizeof(float));
    int *visited = (int *)malloc((n + 1) * sizeof(int));

    for (i = 0; i < n; i++) {
        previous[i] = -1;
    }

    for (i = 0; i < n; i++) {
        distance[i] = FLT_MAX;
        visited[i] = 0;
    }

    // distanta in punctul sursa este 0
    distance[source] = 0;

    for (i = 0; i < n - 1; i++) {
        // determinam numarul nodului cu distanta minima si finita
        index = minDistance(distance, visited, n);

        // il marcam in vectorul de vizitat
        visited[index] = 1;

        ListNode *node = graph->list[index].head;

        // parcurgem lista de adiacenta a nodului
        while (node != NULL) {
            // determinam numarul nodului destinatie
            destination = node->vertex;

            weight = node->weight;
            depth = graph->list[destination].depth;

            // calculam scorul
            scor = (float)weight / depth;

            // daca nodul este nevizitat si distanta in nodul din care porneste
            // drumul este diferita de infinit
            if (visited[destination] == 0 && distance[index] != FLT_MAX) {
                // verificam daca noul arc, relaxeaza distanta sau nu
                if (distance[index] + scor < distance[destination]) {
                    distance[destination] = distance[index] + scor;
                    previous[destination] = index;
                }
            }
            // se trece la urmatorul vecin
            node = node->next;
        }
    }

    for (i = 0; i < n; i++) {
        if (distance[i] == FLT_MAX) {
            distance[i] = -1;
        }
    }
    free(distance);
    free(visited);
}

// determina pornind de la vectorul path care contine indicii nodurilor
// care formeaza drumul, costul acestuia
int costPath(Graph *graph, int *path, int n) {
    int i, node1, node2, cost = 0;

    for (i = 0; i < n - 1; i++) {
        node1 = path[i];
        node2 = path[i + 1];

        ListNode *node = graph->list[node1].head;
        while (node->vertex != node2) {
            node = node->next;
        }

        cost += node->weight;
    }
    return cost;
}
