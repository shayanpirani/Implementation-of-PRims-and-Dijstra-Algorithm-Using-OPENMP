#include <stdio.h>
#include <iostream>
#include <limits.h>
#include <time.h>
#include <stdlib.h>
using namespace std;
int mst ,V;

int minimum_key(int key[], int visited[])
{
    
    int min = INT_MAX, minimum_index;
    int v;
    for (v = 0; v < V; v++)
    {
        if (visited[v] == 0 && key[v] < min)
        {
            min = key[v], minimum_index = v;
        }
    }

    return minimum_index;
}

// Print mst
int print(int from[], int n, int **graph)
{
    int i;
    printf("Node    ------->     Edge    =    Weight\n");
    for (i = 1; i < V; i++){
        printf("%d ------> %d    =   %d \n", from[i], i, graph[i][from[i]]);
        //summation of MST; 
        mst += graph[i][from[i]];
        }
        return 1;
}

void prims(int **graph)
{
    int from[V];    
    int key[V];     
    int visited[V]; 
    int i, count;
    
    for (i = 0; i < V; i++)
        key[i] = INT_MAX, visited[i] = 0;

    
    key[0] = 0;   
    from[0] = -1; 

    
    for (count = 0; count < V - 1; count++)
    {
    
        int u = minimum_key(key, visited);

    
        visited[u] = 1;
        int v;

    
        for (v = 0; v < V; v++)
            if (graph[u][v] && visited[v] == 0 && graph[u][v] < key[v])
                from[v] = u, key[v] = graph[u][v];
    }
     print(from, V, graph);
}


int main()
{
    printf("Enter the number of vertices: \n");
    scanf("%d",&V);

	 clock_t begin = clock();
    // Memory Allocation
    int **graph = (int **)malloc(V * sizeof(int *)); 
    for (int x=0; x<V; x++) 
        graph[x] = (int *)malloc(V * sizeof(int));
    int i, j;
    
    //Generate random adjacency matrix
    srand(time(NULL));
    printf("Generating the edges of graph with %d vertices.\n",V);
    for (i = 0; i < V; i++)
        for (j = 0; j < V; j++)
            graph[i][j] = rand() % 10;

    for (i = 0; i < V; i++)
    {
        graph[i][i] = 0;
    }

    for (i = 0; i < V; i++){
        for (j = 0; j < V; j++){
            graph[j][i] = graph[i][j];
         }
       }  

     //Adjancey Matrix Printing
     /*for (i = 0; i < V; i++){
         for (j = 0; j < V; j++)
         {
             printf("%d ", graph[i][j]);
         }
         printf("\n");
     }*/

    cout<<"Starting time is: "<<(double)begin<<endl;
    prims(graph);
    
    printf("The MST of the %d vertices is: %d\n\n",V,mst);
    
    clock_t end = clock();
    cout<<"Ending time is: "<<(double)end<<endl;

    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Time for serial execution is = %f sec.\n", time_spent);

    return 0;
}
