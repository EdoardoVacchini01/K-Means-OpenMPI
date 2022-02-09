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


void reducePrototypes(void *in, void *inout, int *len, MPI_Datatype *dptr) {
    unsigned int cluster = 0;
    unsigned int coordinate = 0;
    prototype_t *inPrototype = (prototype_t*) in;
    prototype_t *inoutPrototype = (prototype_t*) inout;

    for (cluster = 0; cluster < *len; cluster++) {
        for (coordinate = 0; coordinate < DIMENSION; coordinate++) {
            (inoutPrototype + cluster)->pointsCoordinatesSum[coordinate] +=
                (inPrototype + cluster)->pointsCoordinatesSum[coordinate];
        }
        (inoutPrototype + cluster)->nPoints += (inPrototype + cluster)->nPoints;
    }
}
