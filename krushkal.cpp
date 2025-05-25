#include <stdio.h>
#include <stdlib.h>

// A structure to represent an edge in the graph
struct Edge {
    int src, dest, weight;
};

// A structure to represent a graph
struct Graph {
    int V, E;            // V = number of vertices, E = number of edges
    struct Edge* edge;   // Array of edges
};

// A structure to represent a subset for union-find
struct Subset {
    int parent;
    int rank;
};

// Function to create a graph with V vertices and E edges
struct Graph* createGraph(int V, int E) {
    struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));
    graph->V = V;
    graph->E = E;
    graph->edge = (struct Edge*) malloc(E * sizeof(struct Edge));
    return graph;
}

// Find set of an element i (uses path compression)
int find(struct Subset subsets[], int i) {
    if (subsets[i].parent != i)
        subsets[i].parent = find(subsets, subsets[i].parent);
    return subsets[i].parent;
}

// Union of two sets x and y (uses union by rank)
void Union(struct Subset subsets[], int x, int y) {
    int xroot = find(subsets, x);
    int yroot = find(subsets, y);

    // Attach smaller rank tree under root of high rank tree
    if (subsets[xroot].rank < subsets[yroot].rank)
        subsets[xroot].parent = yroot;
    else if (subsets[xroot].rank > subsets[yroot].rank)
        subsets[yroot].parent = xroot;
    else {
        subsets[yroot].parent = xroot;
        subsets[xroot].rank++;
    }
}

// Compare function to sort edges by weight (for qsort)
int compareEdges(const void* a, const void* b) {
    return ((struct Edge*)a)->weight - ((struct Edge*)b)->weight;
}

// Kruskal's algorithm
void KruskalMST(struct Graph* graph) {
    int V = graph->V;
    struct Edge result[V];  // Store the resulting MST
    int e = 0;              // Index for result[]
    int i = 0;              // Index for sorted edges

    // Step 1: Sort all the edges in non-decreasing order of weight
    qsort(graph->edge, graph->E, sizeof(graph->edge[0]), compareEdges);

    // Allocate memory for creating V subsets
    struct Subset* subsets = (struct Subset*) malloc(V * sizeof(struct Subset));
    for (int v = 0; v < V; ++v) {
        subsets[v].parent = v;
        subsets[v].rank = 0;
    }

    // Number of edges to be taken is V-1
    while (e < V - 1 && i < graph->E) {
        struct Edge next_edge = graph->edge[i++];

        int x = find(subsets, next_edge.src);
        int y = find(subsets, next_edge.dest);

        // If including this edge doesn't cause a cycle, include it
        if (x != y) {
            result[e++] = next_edge;
            Union(subsets, x, y);
        }
    }

    // Print the result
    printf("Edges in the Minimum Spanning Tree:\n");
    int totalWeight = 0;
    for (i = 0; i < e; ++i) {
        printf("%d -- %d == %d\n", result[i].src, result[i].dest, result[i].weight);
        totalWeight += result[i].weight;
    }
    printf("Total weight of MST: %d\n", totalWeight);
}

// Example usage
int main() {
    int V = 4; // Number of vertices
    int E = 5; // Number of edges
    struct Graph* graph = createGraph(V, E);

    // Add edges: src, dest, weight
    graph->edge[0] = (struct Edge){0, 1, 10};
    graph->edge[1] = (struct Edge){0, 2, 6};
    graph->edge[2] = (struct Edge){0, 3, 5};
    graph->edge[3] = (struct Edge){1, 3, 15};
    graph->edge[4] = (struct Edge){2, 3, 4};

    KruskalMST(graph);

    return 0;
}

