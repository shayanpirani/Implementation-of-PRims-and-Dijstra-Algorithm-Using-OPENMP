#include <stdio.h>
#include <limits.h>
#include <iostream>
#include <omp.h>
#include <time.h>
#include <stdlib.h>
#include <bits/stdc++.h>
using namespace std;


int V,num,mst=0;
double starting,ending;

int minKey(int key[], int visited[])
{
    int min = INT_MAX, index, i;
    //parallel region starts
#pragma omp parallel 
    {
        num = omp_get_num_threads();
        int index_local = index;
        int min_local = min;
        // all threads will continue there work and not wait for the other threads
#pragma omp for nowait
        for (i = 0; i < V; i++)
        {
            if (visited[i] == 0 && key[i] < min_local)
            {
                min_local = key[i];
                index_local = i;
            }
        }
        //one thread at time will be allowed to check if the edge which it has is smaller than the edge already chosen.
#pragma omp critical
        {
            if (min_local < min)
            {
                min = min_local;
                index = index_local;
            }
        }
    }
    return index;
}
//summation of MST; 
void printMST(int from[], int n, int **graph)
{
    
    
    ofstream result;
    result.open("local.txt");
    int i;
   // printf("\n--------Local Results--------\n");
    result<<"--------Local Results--------"<<endl<<endl;
    //printf("\n\nNode    ------->     Edge    =    Weight\n");
    result<<"Node    ------->     Edge    =    Weight"<<endl<<endl;
#pragma omp parallel for
    //int num1 = omp_get_num_threads();
    
    for (i = 1; i < V; i++){
        printf( "%d ------> %d    =   %d   calculated by thread %d from total %d threads.\n", from[i], i, graph[i][from[i]],omp_get_thread_num(),num);
        //result<<from[i]<<" ------> " <<i<<" = "<<graph[i][from[i]]<<" calculated by thread "<<omp_get_thread_num()<<" from total "<<num<<" threads"<<endl;
    }
    
    #pragma omp parallel for reduction(+:mst)
    for (i = 1; i < V; i++){
    mst += graph[i][from[i]];
    }
    
    
    
}


void primMST(int **graph)
{
    ofstream result;
    result.open("Results.txt");
    int from[V];
    //key is nodes
    int key[V], num_threads;
    int visited[V];
    int i, count;
    for (i = 0; i < V; i++)
        key[i] = INT_MAX, visited[i] = 0;

    key[0] = 0;
    from[0] = -1;
    
    //printf("\n-----Data partioning among the threads------\n\n");
    result<<"-----Data partioning among the threads------"<<endl<<endl;
    #pragma omp parallel for
    for (i = 0; i < V; i++)
     {
         for (int j = 0; j < V; j++)
         {
             printf("%d is given to thread %d out of total %d threads\n\n ", graph[i][j],omp_get_thread_num(),omp_get_num_threads());
             result<<graph[i][j]<<" is given to thread "<<omp_get_thread_num()<<" out of total "<<omp_get_num_threads()<<" threads."<<endl;
         }
         printf("\n");
     }
     starting = omp_get_wtime();
    cout<<endl<<endl<<"Starting time is: "<<(double)starting<<endl;

    for (count = 0; count < V - 1; count++)
    {
        int u = minKey(key, visited);
        visited[u] = 1;

        int v;
        
        //even partioning of data among threads 
     
#pragma omp parallel for schedule(static)
        
        for (v = 0; v < V; v++)
        {
            
            if (graph[u][v] && visited[v] == 0 && graph[u][v] < key[v])
                from[v] = u, key[v] = graph[u][v];
        }
    }
    
    ending = omp_get_wtime();
    cout<<endl<<endl<<"Ending time is: "<<(double)ending<<endl;
    
     printMST(from, V, graph);
}
int main()
{
    printf("Enter the number of vertices: \n");
    scanf("%d",&V);
	
    //allocation of memory
    int **graph = (int **)malloc(V * sizeof(int *)); 
    for (int x=0; x<V; x++) 
        graph[x] = (int *)malloc(V * sizeof(int));
    int i, j;
    //Generate random number for our data
    srand(time(NULL));
    printf("Generating the edges of graph with %d vertices.\n",V);
    for (i = 0; i < V; i++)
        for (j = 0; j < V; j++)
            graph[i][j] = rand() % 150;

    for (i = 0; i < V; i++)
    {
        graph[i][i] = 0;
    }

    for (i = 0; i < V; i++){
        for (j = 0; j < V; j++){
            graph[j][i] = graph[i][j];
            }
    }
    
    ofstream result;
    result.open("Matrix.txt");
    
    //Printing the data set  
    printf("\n\nPrinting adjacency matrix\n\n");
     for (i = 0; i < V; i++)
     {
         for (j = 0; j < V; j++)
         {
             //scanf("%d ", &graph[i][j]);
             result<<graph[i][j]<<" ";
         }
         result<<endl;
         printf("\n");
     }
     
     /*for (i = 0; i < V; i++)
     {
         for (j = 0; j < V; j++)
         {
             printf("%d ", graph[i][j]);
         }
         printf("\n");
     }*/
     
     
     
     
     
    primMST(graph);
    
    result.open("global_results.txt");
    printf("\n\n--------Global Results--------\n");
    printf("\nThe MST of the %d vertices is: %d\n\n",V,mst);
    result<<"The MST of the "<<V<<" vertices is : "<<mst<<endl;
    
    printf("\n\nTime for parallel execution is  = %f sec.\nThreads = %d\n", ending - starting, num);
    
    for(i=0;i<V;i++)
    {
    free(graph[i]);
    }
    free(graph);

    return 0;
}

