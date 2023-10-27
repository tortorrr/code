#include <stdio.h>
struct node
{
    unsigned dist[20]; // shortest distance between the nodes 
    unsigned from[20]; // information for the next hop to occur
} rt[10]; // possibly 10 nodes are created



int main()
{
    int costmat[20][20];
    int nodes, i, j, k, count = 0;
    printf("\nEnter the number of nodes : ");
    scanf("%d", &nodes); // Enter the nodes
    printf("\nEnter the cost matrix :\n");

    // getting the input the user for all the nodes
    for (i = 0; i < nodes; i++)
    {
        for (j = 0; j < nodes; j++)
        {
            scanf("%d", &costmat[i][j]);
            costmat[i][i] = 0; // setting the self traversal to 0
            rt[i].dist[j] = costmat[i][j]; // initialise the distance equal to cost matrix
            rt[i].from[j] = j;
        }
    }



    do
    {
        count = 0;
        for (i = 0; i < nodes; i++) // We choose arbitary vertex k and we calculate the direct distance from the node i to k using the cost matrix
            // and add the distance from k to node j
            for (j = 0; j < nodes; j++)
                for (k = 0; k < nodes; k++)
                    if (rt[i].dist[j] > costmat[i][k] + rt[k].dist[j]) // a13 = a12 + a23  simple simulation
                    { // We calculate the minimum distance
                        rt[i].dist[j] = rt[i].dist[k] + rt[k].dist[j];
                        rt[i].from[j] = k;
                        count++;
                    }
    } while (count != 0);

    // printing the result nodes 

    for (i = 0; i < nodes; i++)
    {
        printf("\n\n For router %d\n", i + 1);
        for (j = 0; j < nodes; j++)
        {
            printf("%d\t", rt[i].dist[j]);
        }
        printf("\n");
    }
    printf("\n\n");
    return 0;
}
