// A C++ program for Dijkstra's single source shortest path algorithm.
// The program is for adjacency matrix representation of the graph

#include <cstdio>
#include <climits>
#include <iostream>
#include <omp.h>
#include <boost/progress.hpp>
#include <boost/multi_array.hpp>

using namespace std;

//#define MY_USE_OPENMP
#ifdef MY_USE_OPENMP
    #define THREADS 8
#else
    #define THREADS 1
#endif

// Number of vertices in the graph
#define V 10000

// A utility function to find the vertex with minimum distance value, from
// the set of vertices not yet included in shortest path tree
int minDistance(const int dist[], const bool sptSet[]) {
// Initialize min value
    int min = INT_MAX, min_index;

    for (int v = 0; v < V; v++)
        if (sptSet[v] == false && dist[v] <= min)
            min = dist[v], min_index = v;

    return min_index;
}

// A utility function to print the constructed distance array
int printSolution(int dist[], int n) {
    printf("Vertex Distance from Source\n");
    for (int i = 0; i < V; i++)
        printf("%d \t %d\n", i, dist[i]);
}

// Function that implements Dijkstra's single source shortest path algorithm
// for a graph represented using adjacency matrix representation
void dijkstra(boost::multi_array<int, 2> &graph, int src) {
    // Time counter
    boost::progress_timer progress;

    int dist[V];     // The output array. dist[i] will hold the shortest
    // distance from src to i

    bool sptSet[V]; // sptSet[i] will true if vertex i is included in shortest
    // path tree or shortest distance from src to i is finalized

    // Initialize all distances as INFINITE and stpSet[] as false
    for (int i = 0; i < V; i++)
        dist[i] = INT_MAX, sptSet[i] = false;

    // Distance of source vertex from itself is always 0
    dist[src] = 0;

    // Find shortest path for all vertices
    for (int count = 0; count < V - 1; count++) {
        // Pick the minimum distance vertex from the set of vertices not
        // yet processed. u is always equal to src in the first iteration.
        int u = minDistance(dist, sptSet);

        // Mark the picked vertex as processed
        sptSet[u] = true;

        // Update dist value of the adjacent vertices of the picked vertex.
#ifdef MY_USE_OPENMP
        #pragma omp parallel for num_threads(THREADS)
#endif
        for (int v = 0; v < V; v++)

            // Update dist[v] only if is not in sptSet, there is an edge from
            // u to v, and total weight of path from src to v through u is
            // smaller than current value of dist[v]
            if (!sptSet[v] && graph[u][v] && dist[u] != INT_MAX
                && dist[u] + graph[u][v] < dist[v])
                dist[v] = dist[u] + graph[u][v];
    }

    // print the constructed distance array
    printSolution(dist, V);
    #ifdef MY_USE_OPENMP
        //printf("All %d threads use ", THREADS);
        cout << "All " << THREADS << " threads use ";
    #endif
}

// driver program to test above function
int main() {
/* Let us create the example graph discussed above */
    /*int graph[V][V] = {{0, 4,  0, 0,  0,  0,  0, 8,  0},
                       {4, 0,  8, 0,  0,  0,  0, 11, 0},
                       {0, 8,  0, 7,  0,  4,  0, 0,  2},
                       {0, 0,  7, 0,  9,  14, 0, 0,  0},
                       {0, 0,  0, 9,  0,  10, 0, 0,  0},
                       {0, 0,  4, 14, 10, 0,  2, 0,  0},
                       {0, 0,  0, 0,  0,  2,  0, 1,  6},
                       {8, 11, 0, 0,  0,  0,  1, 0,  7},
                       {0, 0,  2, 0,  0,  0,  6, 7,  0}
    };*/
    boost::multi_array<int, 2> graph(boost::extents[V][V]);
    ios::sync_with_stdio(false);

    for (int i = 0; i < V; i++)
        for (int j = 0; j < V; j++)
            cin >> graph[i][j];
    dijkstra(graph, 0);

    return 0;
}
