#include <stdio.h>
#include <time.h>
#include <limits.h>
#include <stdlib.h>
#include <omp.h>
int mst;
double start,end; //time
int min(int a,int b) {
	return (((a)<(b))?(a):(b));
}
int max(int a,int b) {
	return (((a)>(b))?(a):(b));
}

void dijkstra(int **graph,int n,int startnode);

struct point
{
	int mindistance;
	int index;
};

//this sums the values 
// ? say pehlay condition and iskay baat if true : if false
#pragma omp declare reduction(min : struct point: \
	omp_out.mindistance=omp_in.mindistance > omp_out.mindistance ? omp_out.mindistance:omp_in.mindistance,\
	omp_out.index = omp_in.mindistance > omp_out.mindistance ? omp_out.index:omp_in.index) \
	initializer(omp_priv={INT_MAX,INT_MAX})
	//omp.priv is + operator clause of reduction.
	//int_max is the greatest value of integer type.


int main(int argc, char* argv[])
{
	int n,u,**graph,t;
	//Taking input of the graph
	printf("Enter no. of vertices:");
	scanf("%d",&n);
	printf("Memory Allocation of %d vertices.\n",n);
	graph=(int**)malloc(n*sizeof(int*));
	for(int i=0;i<n;i++){
		graph[i]=(int*)malloc(n*sizeof(int));
	}
	printf("Generating the random adjanceny matrix of %d vertices.\n",n);
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			graph[i][j] = rand() % 10;
		}
	}
	
	 for (int i = 0; i < n; i++)
    {
        graph[i][i] = 0;
    }

    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            graph[j][i] = graph[i][j];
            }
    }
	
	printf("\nEnter the starting node:");
	scanf("%d",&u);
	
	
	//Printing the data set  
	int i,j;
    /*printf("\n\nPrinting adjacency matrix\n\n");
     for (i = 0; i < n; i++)
     {
         for (j = 0; j < n; j++)
         {
             printf("%d ", graph[i][j]);
         }
         printf("\n");
     }*/

	
	dijkstra(graph,n,u);
	
    printf("Time for parallel execution is  = %f sec.\n", end - start);
	
	return 0;
}

void dijkstra(int **graph,int n,int startnode)
{
	int **cost,*distance,*predecessor,*visited;
	cost=(int**)malloc(n*sizeof(int*));
	for(int i=0;i<n;i++){
		cost[i]=(int*)malloc(n*sizeof(int));
	}
	distance=(int*)malloc(n*sizeof(int));
	predecessor=(int*)malloc(n*sizeof(int));
	visited=(int*)malloc(n*sizeof(int));
	
	 start = omp_get_wtime();
    printf("Starting time is: %f\n",(double)start);
	for(int i=0;i<n;i++)
		for(int j=0;j<n;j++)
			if(graph[i][j]==0)
				cost[i][j]=INT_MAX;
			else
				cost[i][j]=graph[i][j];
	for(int i=0;i<n;i++)
	{
		distance[i]=cost[startnode][i];
		predecessor[i]=startnode;
		visited[i]=0;
	}
	
	distance[startnode]=0;
	visited[startnode]=1;
	int count=1;
	while(count<n-1)
	{
		struct point mini={INT_MAX,INT_MAX},ans={INT_MAX,INT_MAX};
		int i,j;
		
		// parallel Region starts
		
		
		#pragma omp parallel for private(j) firstprivate(mini) reduction(min:ans)
		for(i=0;i<n;i++){
			j=omp_get_thread_num(); //get number of threads
			if(distance[i]<mini.mindistance&&!visited[i]){
				mini.mindistance=distance[i];
				mini.index=i;
				ans.mindistance=mini.mindistance;
				ans.index=mini.index;
			}
		}
		int nextnode=ans.index;
		visited[nextnode]=1;
		//This will check if the coming node has lesser distance and also goes to unexplored nodes
		mini.mindistance=ans.mindistance;
		mini.index=ans.index;
		
		#pragma omp parallel for
		for(int i=0;i<n;i++){
			if(!visited[i]){
				if(cost[nextnode][i]!=INT_MAX && mini.mindistance+cost[nextnode][i]<distance[i]){
					distance[i]=mini.mindistance+cost[nextnode][i];
					predecessor[i]=nextnode;
				}
			}
		}
		count++;
	}
	 end = omp_get_wtime();
    printf("Ending time is: %f\n",(double)end);
    
    
	//This will print the results
	#pragma omp parallel reduction(+:mst)
	for(int i=0;i<n;i++)
		if(i!=startnode)
		{
			printf("\nDistance of node%d=%d",i,distance[i]);
			printf("\nNode=%d",i);
			mst += distance[i];
			int j=i;
			do
			{
				j=predecessor[j];
				printf("-----> %d",j);
			}while(j!=startnode);
			printf("\n");
	}
	printf("\n\nThe Minimum spanning tree is: %d\n ",mst);
}
