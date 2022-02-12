#include <stdio.h>
#include <stdlib.h>

#include "k_means_clustering_utils.h"


int main(int argc, char *argv[]) {
    point_t *points = NULL;
    unsigned int nPoints = 0;
    unsigned int nClusters = (argc > 3) ? atoi(argv[3]) : 3;
    centroid_t *centroids = NULL;
    prototype_t *prototypes = NULL;
    unsigned int clustersChanged = 0;
    unsigned int iteration = 0;
    unsigned int maxIterations = (argc > 4) ? atoi(argv[4]) : 100;
    FILE *outputFile = NULL;

    printf("Reading the dataset file...\n");

    nPoints = readDataset((argc > 1) ? argv[1] : "dataset.txt", &points);
    if (nPoints == 0) {
        printf("An error occurred while reading the dataset file.\n");
        return EXIT_FAILURE;
    }

    if (nClusters > nPoints) {
        free(points);
        printf("The number of clusters must be less than or equal to the number of data points.\n");
        return EXIT_FAILURE;
    }

    printf("Clustering the data points...\n");

    centroids = (centroid_t*) malloc(nClusters * sizeof(*centroids));
    if (centroids == NULL) {
        free(points);
        printf("An error occurred while allocating the memory for the centroids.\n");
        return EXIT_FAILURE;
    }

    prototypes = (prototype_t*) malloc(nClusters * sizeof(*prototypes));
    if (prototypes == NULL) {
        free(points);
        free(centroids);
        printf("An error occurred while allocating the memory for the prototypes.\n");
        return EXIT_FAILURE;
    }

    initCentroids(centroids, nClusters, points);
    do {
        initPrototypes(prototypes, nClusters);
        clustersChanged = kMeansIteration(points, nPoints, centroids, prototypes, nClusters);
        updateCentroids(centroids, prototypes, nClusters);

        iteration++;
    } while((iteration < maxIterations) && clustersChanged);

    printf("Clustering process completed.\n\n");
    printCentroids(centroids, nClusters, stdout);

    outputFile = fopen((argc > 2) ? argv[2] : "centroids.txt", "w");
    if (outputFile != NULL) {
        printCentroids(centroids, nClusters, outputFile);
        fclose(outputFile);
    }

    free(points);
    free(centroids);
    free(prototypes);
    return EXIT_SUCCESS;
}
