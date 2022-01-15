#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

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
    unsigned int nScatteredPoints = 0;
    point_t *scatteredPoints = NULL;
    centroid_t *centroids = NULL;
    prototype_t *prototypes = NULL;
    unsigned int clustersChanged = 0;
    unsigned int iteration = 0;
    unsigned int nClusters = (argc > 2) ? atoi(argv[2]) : 3;
    unsigned int maxIterations = (argc > 3) ? atoi(argv[3]) : 100;


    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &communicatorSize);

    initDatatypes(&pointDatatype, &centroidDatatype, &prototypeDatatype);
    MPI_Op_create(reducePrototypes, 1, &reducePrototypesOp);

    if (rank == 0) {
        printf("Reading the dataset file...\n");

        nPoints = readDataset((argc > 1) ? argv[1] : "dataset.txt", &points);
        if (nPoints == 0) {
            printf("An error occurred while reading the dataset file.\n");
            return EXIT_FAILURE;
        }

        nScatteredPoints = nPoints / communicatorSize;
    }

    // Broadcast nScatteredPoints to all processes
    MPI_Bcast(&nScatteredPoints, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);

    // Allocate the memory for the point scatter test
    scatteredPoints = (point_t*) malloc(nScatteredPoints * sizeof(*points));
    if (scatteredPoints == NULL) {
        printf("An error occurred while allocating memory for the centroids.\n");
        //TODO: handling failure on single process
        MPI_Finalize();
    }
    //TODO: distribuire tutti i punti scartati dalla divisione
    // Scatter the data points to the processes
    MPI_Scatter(points, nScatteredPoints, pointDatatype, scatteredPoints, nScatteredPoints,
        pointDatatype, 0, MPI_COMM_WORLD);

    centroids = (centroid_t*) malloc(nClusters * sizeof(*centroids)); 
    if (centroids == NULL) {
        //TODO: handling failure on single process
        printf("An error occurred while allocating memory for the centroids.\n");
        if (rank == 0) {
            free(points);
        }
        free(scatteredPoints);
        MPI_Finalize();
        return EXIT_FAILURE;
    }

    if (rank == 0) {
        initCentroids(centroids, nClusters, points);
    }
    
    MPI_Bcast(centroids, nClusters, centroidDatatype, 0, MPI_COMM_WORLD);
    prototypes = (prototype_t*) malloc(nClusters * sizeof(*prototypes));
    if (prototypes == NULL) {
        //TODO: handling failure on single process
        if (rank == 0)
            free(points);
        free(centroids);
        free(scatteredPoints);
        printf("An error occurred while allocating the memory for the prototypes.\n");
        MPI_Finalize();
        return EXIT_FAILURE;
    }

    if (rank == 0){
        printf("Clustering the data points...\n");
    }
    do {
        initPrototypes(prototypes, nClusters);
        clustersChanged = kMeansIteration(scatteredPoints, nScatteredPoints, centroids, prototypes, nClusters);
        //Si può fare con lo stesso buffer la Allreduce?
        MPI_Allreduce(MPI_IN_PLACE, prototypes, nClusters, prototypeDatatype, reducePrototypesOp, MPI_COMM_WORLD);
        updateCentroids(centroids, prototypes, nClusters);
        MPI_Allreduce(MPI_IN_PLACE, &clustersChanged, 1, MPI_C_BOOL, MPI_LOR, MPI_COMM_WORLD);
        iteration++;
    } while((iteration < maxIterations) && clustersChanged);

    //TODO: gather all clustered points and print them
    if (rank == 0) {
        printCentroids(centroids, nClusters);
        free(points);
    }

    free(scatteredPoints);
    free(centroids);
    free(prototypes);

    MPI_Finalize();

    return EXIT_SUCCESS;
}
