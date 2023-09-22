/* Andronache Madalina-Georgiana - 312CC */

#include <float.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structs.h"

// elibereaza memoria pentru graful dat ca parametru
void freeGraph(Graph *graph) {
    int i;

    for (i = 0; i < graph->number; i++) {
        ListNode *node = graph->list[i].head;
        
        while (node != NULL) {
            ListNode *temp = node;
            node = node->next;
            free(temp);
        }
    }

    free(graph->list);
    free(graph);
}

void resolve_C1(FILE *fin, FILE *fout) {
    int i, n, m, count, nodes;
    fscanf(fin, "%d", &n);
    fscanf(fin, "%d", &m);

    // initializam graful neorientat
    Graph *graph = initGraph(n);
    int *components = (int *)malloc((n + 1) * sizeof(int));
    Edge *edges = (Edge *)malloc((m + 1) * sizeof(Edge));

    // cream graful conform datelor din fisierul de int
    graph = createUndirectedGraph(graph, edges, n, m, fin);

    for (i = 0; i < n; i++) {
        components[i] = -1;
    }

    // determinam numarul de componente conexe ale grafului
    count = countComponents(graph, components, fout);
    int *cost_total = (int *)malloc((count + 1) * sizeof(int));

    // sortam muchiile dupa cost
    sort(edges, m);

    // pentru fiecare componenta conexa, determinam numarul de noduri
    // si aplicam kruskal pentru a determina subgraful de cost minim
    for (i = 0; i < count; i++) {
        nodes = number_nodes(components, i, n);
        cost_total[i] = kruskal(edges, components, i, nodes, n, m);
    }

    // sortam costurile obtinute pentru fiecare componenta conexa in ordine
    // crescatoare
    sort_cost(cost_total, count);

    for (i = 0; i < count; i++) {
        fprintf(fout, "%d\n", cost_total[i]);
    }

    // eliberam memoria alocata
    free(edges);
    free(cost_total);
    free(components);
    freeGraph(graph);
}

void resolve_C2(FILE *fin, FILE *fout) {
    int i, n, m, treasure, lg;
    int start = -1, stop = -1, min = INT_MAX;
    fscanf(fin, "%d", &n);
    fscanf(fin, "%d", &m);

    // initializam graful orientat
    Graph *graph = initGraph(n);
    char **places = (char **)malloc(n * sizeof(char *));
    int *prev = (int *)malloc((n + 1) * sizeof(int));

    // cream graful orientat
    createDirectedGraph(graph, places, m, fin);

    // dupa acest apel, start va avea valoarea nodului corespunzator pentru
    // Insula, iar stop va avea valoarea nodului corespunzator pentru Corabie
    coordinates(&start, &stop, n, places);

    // atribuim adancimea corespunzatoare fiecarui nod
    graph = getDepths(graph, n, places, fin);

    fscanf(fin, "%d", &treasure);

    // verificam daca exista drum de la Corabie la Insula
    dijkstra(graph, prev, stop, n);

    // memoram in path drumul corespunzator pentru acest traseu
    int *path = getPath(prev, start, &lg);

    if (lg == 1) {
        fprintf(fout, "Echipajul nu poate ajunge la insula\n");
        exit(0);
    } else {
        // eliberam memoria alocata pentru drum
        free(path);

        // verificam daca exista drum de la Insula la Corabie
        dijkstra(graph, prev, start, n);

        lg = -1;

        // memoram in path drumul corespunzator pentru acest traseu
        path = getPath(prev, stop, &lg);

        if (lg == 1) {
            fprintf(
                fout,
                "Echipajul nu poate transporta comoara inapoi la corabie\n");
            exit(0);
        }

        // determinam adancimea minima
        for (i = 0; i < lg; i++) {
            fprintf(fout, "%s ", places[path[i]]);
            if (i != 0 && i != lg - 1 && graph->list[path[i]].depth < min) {
                min = graph->list[path[i]].depth;
            }
        }

        fprintf(fout, "\n");
        fprintf(fout, "%d\n", costPath(graph, path, lg));
        fprintf(fout, "%d\n", min);

        int drumuri = treasure / min;

        fprintf(fout, "%d\n", drumuri);
    }

    // eliberam memoria alocata
    for (i = 0; i < n; i++) {
        free(places[i]);
    }
    free(path);
    free(places);
    free(prev);
    freeGraph(graph);
}

int main(int argc, char const *argv[]) {
    FILE *fin, *fout;

    fin = fopen("tema3.in", "r");
    if (fin == NULL) {
        printf("Fisierul de intrare nu a putut fi deschis\n");
        return 1;
    }

    fout = fopen("tema3.out", "w");
    if (fout == NULL) {
        printf("Fisierul de iesire nu a putut fi deschis\n");
        return 1;
    }

    if (argc != 2) {
        printf("Numarul de argumente este incorect!\n");
        return 1;
    } else {
        // se rezolva cerinta 1
        if (strcmp(argv[1], "1") == 0) {
            resolve_C1(fin, fout);
            // se rezolva cerinta 2
        } else if (strcmp(argv[1], "2") == 0) {
            resolve_C2(fin, fout);
        }
    }

    // inchidem fisierul de intrare si de iesire
    fclose(fin);
    fclose(fout);

    return 0;
}
