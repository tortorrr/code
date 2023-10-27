#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *f = fopen("input_data.txt", "r");
    char buf[10];

    int numNodes, numEdges, sourceNode, destinationNode, costNode; // costNode => cost to reach from sourceNode to destiationNode
    
    fscanf(f, "%d", &numNodes);
    printf("The number of nodes: %d\n", numNodes);

    int cost[numNodes][numNodes], distance[numNodes][numNodes];

    // Initialize the cost and distance matrices
    for (int i = 0; i < numNodes; i++) {
        for (int j = 0; j < numNodes; j++) {
            if (i == j) {
                distance[i][j] = 0;
                cost[i][j] = 0;
            } else {
                distance[i][j] = 999;
                cost[i][j] = 999;
            }
        }
    }

    fscanf(f, "%d", &numEdges);
    printf("The number of edges: %d\n", numEdges);

    // Read edge data and populate the cost matrix
    for (int i = 0; i < numEdges; i++) {
        fscanf(f, "%d %d %d", &sourceNode, &destinationNode, &costNode);
        cost[sourceNode][destinationNode] = costNode;
        cost[destinationNode][sourceNode] = costNode;
    }

    // Calculate shortest path for each node
    for (int i = 0; i < numNodes; i++) {
        printf("For node %d\n\n", i);
        for (int j = 0; j < numNodes - 1; j++) {
            int flag = 0;
            fseek(f, 0, SEEK_SET);
            fgets(buf, 10, f); // Skip the first two lines
            fgets(buf, 10, f);

            for (int k = 0; k < numEdges; k++) {    
                fscanf(f, "%d %d %d", &sourceNode, &destinationNode, &costNode);
                if (distance[i][sourceNode] + cost[sourceNode][destinationNode] < distance[i][destinationNode]) {
                    flag = 1;
                    distance[i][destinationNode] = distance[i][sourceNode] + cost[sourceNode][destinationNode];
                }
                if (distance[i][destinationNode] + cost[destinationNode][sourceNode] < distance[i][sourceNode]) {
                    flag = 1;
                    distance[i][sourceNode] = distance[i][destinationNode] + cost[destinationNode][sourceNode];
                }
            }
            if (flag == 1) {
                printf("Round: %d\n", j);
                for (int i = 0; i < numNodes; i++) {
                    for (int j = 0; j < numNodes; j++) {
                        printf("%d\t", distance[i][j]);
                    }
                    printf("\n");
                }
                printf("\n\n");
            } else {
                continue;
            }
        }
    }

    // Print the final routing table
    printf("Final Routing Table:\n\n");
    for (int i = 0; i < numNodes; i++) {
        for (int j = 0; j < numNodes; j++) {
            printf("%d\t", distance[i][j]);
        }
        printf("\n");
    }
    fclose(f);
}