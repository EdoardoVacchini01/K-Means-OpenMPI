#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#include "k_means_clustering_utils.h"


void initDatatypes(MPI_Datatype *pointDatatype, MPI_Datatype *centroidDatatype,
        MPI_Datatype *prototypeDatatype) {
    point_t point = {0};
    prototype_t prototype = {0};
    int pointBlockLengths[] = {DIMENSION, 1};
    int centroidBlockLengths[] = {DIMENSION};
    int prototypeBlockLengths[] = {DIMENSION, 1};
    MPI_Aint pointIndices[] = {0, (void*)&point.clusterId - (void*)&point};
    MPI_Aint centroidIndices[] = {0};
    MPI_Aint prototypeIndices[] = {0, (void*)&prototype.nPoints - (void*)&prototype};
    MPI_Datatype pointTypes[] = {MPI_DOUBLE, MPI_UNSIGNED};
    MPI_Datatype centroidTypes[] = {MPI_DOUBLE};
    MPI_Datatype prototypeTypes[] = {MPI_DOUBLE, MPI_UNSIGNED};

    MPI_Type_create_struct(2, pointBlockLengths, pointIndices, pointTypes, pointDatatype);
    MPI_Type_create_struct(1, centroidBlockLengths, centroidIndices, centroidTypes,
        centroidDatatype);
    MPI_Type_create_struct(2, prototypeBlockLengths, prototypeIndices, prototypeTypes,
        prototypeDatatype);
    MPI_Type_commit(pointDatatype);
    MPI_Type_commit(centroidDatatype);
    MPI_Type_commit(prototypeDatatype);
}


int main(int argc, char *argv[]) {
    int rank = 0;
    int communicatorSize = 0;
    MPI_Datatype pointDatatype = NULL;
    MPI_Datatype centroidDatatype = NULL;
    MPI_Datatype prototypeDatatype = NULL;
    point_t *points = NULL;
    unsigned int nPoints = 0;
    unsigned int nScatteredPoints = 0;
    point_t *scatteredPoints = NULL;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &communicatorSize);

    initDatatypes(&pointDatatype, &centroidDatatype, &prototypeDatatype);

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

    // Scatter the data points to the processes
    MPI_Scatter(points, nScatteredPoints, pointDatatype, scatteredPoints, nScatteredPoints,
        pointDatatype, 0, MPI_COMM_WORLD);

    MPI_Finalize();

    return EXIT_SUCCESS;
}
