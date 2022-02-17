#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "k_means_clustering_utils.h"


int main(int argc, char *argv[]) {
    point_t *points = NULL;
    unsigned int nPoints = 0;
    unsigned int nClusters = (argc > 3) ? atoi(argv[3]) : 3;
    struct timespec startTime = {0};
    centroid_t *centroids = NULL;
    prototype_t *prototypes = NULL;
    unsigned int clustersChanged = 0;
    unsigned int iteration = 0;
    unsigned int maxIterations = (argc > 4) ? atoi(argv[4]) : 100;
    struct timespec endTime = {0};
    FILE *outputFile = NULL;

    // Read the data points of the dataset
    printf("Reading the dataset file...\n");
    nPoints = readDataset((argc > 1) ? argv[1] : "dataset.txt", &points);
    if (nPoints == 0) {
        printf("An error occurred while reading the dataset file.\n");
        return EXIT_FAILURE;
    }

    // Check if the number of clusters exceeds the number of data points of the dataset
    if (nClusters > nPoints) {
        free(points);
        printf("The number of clusters must be less than or equal to the number of data points.\n");
        return EXIT_FAILURE;
    }

    // Record the K-Means start moment in time
    clock_gettime(CLOCK_REALTIME, &startTime);

    // Allocate the memory for the centroids
    centroids = (centroid_t*) malloc(nClusters * sizeof(*centroids));
    if (centroids == NULL) {
        free(points);
        printf("An error occurred while allocating the memory for the centroids.\n");
        return EXIT_FAILURE;
    }

    // Allocate the memory for the prototypes
    prototypes = (prototype_t*) malloc(nClusters * sizeof(*prototypes));
    if (prototypes == NULL) {
        free(points);
        free(centroids);
        printf("An error occurred while allocating the memory for the prototypes.\n");
        return EXIT_FAILURE;
    }

    // Run K-Means with the selected settings using the data points of the dataset
    printf("Clustering the data points...\n");
    initCentroids(centroids, nClusters, points);
    do {
        initPrototypes(prototypes, nClusters);
        clustersChanged = kMeansIteration(points, nPoints, centroids, prototypes, nClusters);
        updateCentroids(centroids, prototypes, nClusters);

        iteration++;
    } while ((iteration < maxIterations) && clustersChanged);

    // Record the K-Means end moment in time
    clock_gettime(CLOCK_REALTIME, &endTime);

    // Print the centroids to stdout
    printf("Clustering process completed in %.06lf s.\n\nCentroids:\n",
        (endTime.tv_sec - startTime.tv_sec) + (endTime.tv_nsec - startTime.tv_nsec) * 1e-9);
    printCentroids(centroids, nClusters, stdout);

    // Print the centroids and the labels of the data points to the output file
    outputFile = fopen((argc > 2) ? argv[2] : "clustering.txt", "w");
    if (outputFile != NULL) {
        printCentroids(centroids, nClusters, outputFile);
        fprintf(outputFile, "\n");
        printPointLabels(points, nPoints, outputFile);
        fclose(outputFile);
    } else {
        printf("\nAn error occurred while opening the output file.\n");
    }

    free(points);
    free(centroids);
    free(prototypes);
    return EXIT_SUCCESS;
}
