#include "mpi_utils.h"


void initDatatypes(MPI_Datatype *pointDatatype, MPI_Datatype *centroidDatatype,
        MPI_Datatype *prototypeDatatype) {
    point_t point = {0};
    prototype_t prototype = {0};
    int pointBlockLengths[] = {DIMENSION, 1};
    int centroidBlockLengths[] = {DIMENSION};
    int prototypeBlockLengths[] = {DIMENSION, 1};
    MPI_Aint pointIndices[] = {0, (void*) &point.clusterId - (void*) &point};
    MPI_Aint centroidIndices[] = {0};
    MPI_Aint prototypeIndices[] = {0, (void*) &prototype.nPoints - (void*) &prototype};
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


void reducePrototypes(void *inArray, void *inOutArray, int *nClusters, MPI_Datatype *datatype) {
    unsigned int cluster = 0;
    unsigned int coordinate = 0;
    prototype_t *inPrototypeArray = (prototype_t*) inArray;
    prototype_t *inOutPrototypeArray = (prototype_t*) inOutArray;

    for (cluster = 0; cluster < *nClusters; cluster++) {
        for (coordinate = 0; coordinate < DIMENSION; coordinate++) {
            (inOutPrototypeArray + cluster)->pointsCoordinatesSum[coordinate] +=
                (inPrototypeArray + cluster)->pointsCoordinatesSum[coordinate];
        }
        (inOutPrototypeArray + cluster)->nPoints += (inPrototypeArray + cluster)->nPoints;
    }
}
