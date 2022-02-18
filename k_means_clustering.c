#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "k_means_clustering_utils.h"
#include "mpi_utils.h"


int main(int argc, char *argv[]) {
    int rank = 0;
    int communicatorSize = 0;
    MPI_Datatype pointDatatype = 0;
    MPI_Datatype centroidDatatype = 0;
    MPI_Datatype prototypeDatatype = 0;
    MPI_Op reducePrototypesOp = 0;
    point_t *points = NULL;
    unsigned int nPoints = 0;
    int *pointsSendCounts = NULL;
    int *pointsDisplacements = NULL;
    unsigned int process = 0;
    unsigned int nScatteredPoints = 0;
    struct timespec startTime = {0};
    point_t *scatteredPoints = NULL;
    unsigned int nClusters = (argc > 3) ? atoi(argv[3]) : 3;
    centroid_t *centroids = NULL;
    prototype_t *prototypes = NULL;
    unsigned int clustersChanged = 0;
    unsigned int iteration = 0;
    unsigned int maxIterations = (argc > 4) ? atoi(argv[4]) : 100;
    struct timespec endTime = {0};
    FILE *outputFile = NULL;

    // Initialize the MPI environment
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &communicatorSize);

    // Initialize the MPI data types for points, centroids and prototypes.
    initDatatypes(&pointDatatype, &centroidDatatype, &prototypeDatatype);

    // Define an MPI_Op that performs a reduction operation on two arrays of prototypes.
    MPI_Op_create(reducePrototypes, 1, &reducePrototypesOp);

    if (rank == 0) {
        // Read the data points of the dataset
        printf("Reading the dataset file...\n");
        nPoints = readDataset((argc > 1) ? argv[1] : "dataset.txt", &points);
        if (nPoints == 0) {
            printf("An error occurred while reading the dataset file.\n");
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }

        // Allocate the memory for the array that will contain the number of data points that each
        // process should handle
        pointsSendCounts = (int*) malloc(communicatorSize * sizeof(*pointsSendCounts));
        if (pointsSendCounts == NULL) {
            printf("An error occurred while allocating the memory for the array that should "
                "contain the number of data points that each process should handle.\n");
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }

        // Allocate the memory for the array that will contain the displacement indices of the first
        // data point that each process will handle relative to the array of all data points
        pointsDisplacements = (int*) malloc(communicatorSize * sizeof(*pointsDisplacements));
        if (pointsDisplacements == NULL) {
            printf("An error occurred while allocating the memory for the array that should "
                "contain the displacement indices of the first data point that each process "
                "should handle relative to the array of all data points.\n");
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }

        // Initialize pointsSendCounts and pointsDisplacements (in general, processes with a lower
        // rank will handle one more data point than the other processes because the number of data
        // points is not guaranteed to be a multiple of the total number of processes)
        for (process = 0; process < communicatorSize; process++) {
            *(pointsSendCounts + process) = (nPoints / communicatorSize) +
                ((process < (nPoints % communicatorSize)) ? 1 : 0);
            *(pointsDisplacements + process) = (process == 0) ?
                0 : (*(pointsDisplacements + process - 1) + *(pointsSendCounts + process - 1));
        }

        // Record the K-Means start moment in time
        clock_gettime(CLOCK_REALTIME, &startTime);
    }

    // Scatter to each process the number of points that it will have to handle
    MPI_Scatter(pointsSendCounts, 1, MPI_INT, &nScatteredPoints, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Allocate the memory for the scattered points
    scatteredPoints = (point_t*) malloc(nScatteredPoints * sizeof(*points));
    if (scatteredPoints == NULL) {
        if (rank == 0) {
            free(points);
        }
        printf("An error occurred while allocating the memory for the scattered points.\n");
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    // Scatter the data points to all the processes
    MPI_Scatterv(points, pointsSendCounts, pointsDisplacements, pointDatatype, scatteredPoints,
        nScatteredPoints, pointDatatype, 0, MPI_COMM_WORLD);

    // Allocate the memory for the centroids
    centroids = (centroid_t*) malloc(nClusters * sizeof(*centroids));
    if (centroids == NULL) {
        if (rank == 0) {
            free(points);
        }
        free(scatteredPoints);
        printf("An error occurred while allocating the memory for the centroids.\n");
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    // Initialize the centroids and broadcast them to all the processes
    if (rank == 0) {
        initCentroids(centroids, nClusters, points);
    }
    MPI_Bcast(centroids, nClusters, centroidDatatype, 0, MPI_COMM_WORLD);

    // Allocate the memory for the prototypes
    prototypes = (prototype_t*) malloc(nClusters * sizeof(*prototypes));
    if (prototypes == NULL) {
        if (rank == 0) {
            free(points);
        }
        free(scatteredPoints);
        free(centroids);
        printf("An error occurred while allocating the memory for the prototypes.\n");
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    // Run K-Means with the selected settings using the data points of the dataset
    if (rank == 0) {
        printf("Clustering the data points...\n");
    }
    do {
        initPrototypes(prototypes, nClusters);
        clustersChanged = kMeansIteration(scatteredPoints, nScatteredPoints, centroids, prototypes,
            nClusters);
        MPI_Allreduce(MPI_IN_PLACE, prototypes, nClusters, prototypeDatatype, reducePrototypesOp,
            MPI_COMM_WORLD);
        updateCentroids(centroids, prototypes, nClusters);
        MPI_Allreduce(MPI_IN_PLACE, &clustersChanged, 1, MPI_C_BOOL, MPI_LOR, MPI_COMM_WORLD);

        iteration++;
    } while ((iteration < maxIterations) && clustersChanged);

    // Gather the clustered data points from all the processes
    MPI_Gatherv(scatteredPoints, nScatteredPoints, pointDatatype, points, pointsSendCounts,
        pointsDisplacements, pointDatatype, 0, MPI_COMM_WORLD);

    if (rank == 0) {
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
    }

    MPI_Finalize();

    free(scatteredPoints);
    free(centroids);
    free(prototypes);
    return EXIT_SUCCESS;
}
