#ifndef MPI_UTILS_H
#define MPI_UTILS_H

#include <mpi.h>

#include "k_means_clustering_utils.h"

void initDatatypes(MPI_Datatype *pointDatatype, MPI_Datatype *centroidDatatype,
    MPI_Datatype *prototypeDatatype);

void reducePrototypes(void *in, void *inout, int *len, MPI_Datatype *dptr);

#endif
