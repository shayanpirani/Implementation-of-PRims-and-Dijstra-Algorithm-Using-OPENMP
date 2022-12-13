#include<stdio.h>
#include<curses.h>
#include <time.h>
#include<stdlib.h>
#include<limits.h>
#include<math.h>
#include <unistd.h>
#define INF 9999
//took inf to marks parts which arent possible to traverse to be infinity and as they will be marked by the process they will eventually get there values.

 
void dijkstra(int **graph,int n,int startnode);
 
int main()
{
int n,u,**graph,t,i,j;
	clock_t start,end;
	//Taking input of the graph
	printf("Enter no. of vertices:");
	scanf("%d",&n);
	
	//MEmory Allocation
	printf("Memory Allocation of %d vertices.\n",n);
	graph=(int**)malloc(n*sizeof(int*));
	for(int i=0;i<n;i++){
		graph[i]=(int*)malloc(n*sizeof(int));
	}
	printf("Generating the random adjanceny matrix of %d vertices.",n);
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			graph[i][j] = rand() % 10;
		}
	}

	 for (i = 0; i < n; i++)
    {
        graph[i][i] = 0;
    }

    for (i = 0; i < n; i++){
        for (j = 0; j < n; j++){
            graph[j][i] = graph[i][j];
            }
    }
	
	printf("\nEnter the starting node:");
	scanf("%d",&u);

	srand(time(NULL));
	start=clock();
	dijkstra(graph,n,u);
	end=clock();
	double tme=((double) (end-start))/ CLOCKS_PER_SEC;
	printf("\nThe algorithm took %f seconds to run.\n",tme);
	return 0;
}
 
void dijkstra(int **graph,int n,int startnode)
{
 
int **cost,*distance,*pred,*visited,i,j,count,min_distance ,nextnode;
	
    //Memory Allocay=tion of distance, precedence ,cost ,visited.
    cost=(int**)malloc(n*sizeof(int*));

	for(int i=0;i<n;i++){
		cost[i]=(int*)malloc(n*sizeof(int));
	}
	distance=(int*)malloc(n*sizeof(int));
	pred=(int*)malloc(n*sizeof(int));
	visited=(int*)malloc(n*sizeof(int));
//pred[] stores the predecessor of each node
//count gives the number of nodes seen so far
//create the cost matrix for traversing from one node to other
for(i=0;i<n;i++)
for(j=0;j<n;j++)
if(graph[i][j]==0)
cost[i][j]=INF;
else
cost[i][j]=graph[i][j];
//initialize pred[],distance[] and visited[]
for(i=0;i<n;i++)
{
distance[i]=cost[startnode][i];
pred[i]=startnode;
visited[i]=0;
}
distance[startnode]=0;
visited[startnode]=1;
count=1;
sleep(3);
while(count<n-1)
{
min_distance =INF;
//nextnode gives the node at minimum distance
for(i=0;i<n;i++)
if(distance[i]<min_distance &&!visited[i])
{
min_distance =distance[i];
nextnode=i;
}
//check if a better path exists through nextnode
visited[nextnode]=1;
for(i=0;i<n;i++)
if(!visited[i])
if(min_distance +cost[nextnode][i]<distance[i])
{
distance[i]=min_distance +cost[nextnode][i];
pred[i]=nextnode;
}
count++;
}
 int mst;
//print the path and distance of each node
for(i=0;i<n;i++)
if(i!=startnode)
{
printf("\nDistance of node %d = %d\n",i,distance[i]);
printf("\nPath= %d",i);
j=i;
mst += distance[i];
do
{
j=pred[j];
printf(" --------> %d",j);

}while(j!=startnode);
printf("\n");
}
printf("\n\nThe Minimum spanning tree is: %d\n ",mst);
}

