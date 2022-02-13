#ifndef MPI_UTILS_H
#define MPI_UTILS_H

#include <mpi.h>

#include "k_means_clustering_utils.h"


/**
 * @brief Initializes MPI data types for points, centroids and prototypes.
 *
 * Each MPI_Datatype is initialized by creating an MPI type struct with the corresponding number of
 * fields of specific lengths, offsets and types.
 *
 * @param pointDatatype A pointer to an MPI_Datatype that will be initialized to represent a struct
 *     of type point_t.
 * @param centroidDatatype A pointer to an MPI_Datatype that will be initialized to represent a
 *     struct of type centroid_t.
 * @param prototypeDatatype A pointer to an MPI_Datatype that will be initialized to represent a
 *     struct of type prototype_t.
 */
void initDatatypes(MPI_Datatype *pointDatatype, MPI_Datatype *centroidDatatype,
    MPI_Datatype *prototypeDatatype);


/**
 * @brief Performs a reduction operation on two arrays of prototypes.
 *
 * This user-defined function can be passed to MPI_Op_create() in order to define an MPI_Op that can
 * later be used by MPI_Allreduce() to join the partial prototypes that are calculated by each
 * process during a single K-Means iteration.
 *
 * The prototypes in inArray are joined with those in inOutArray and the resulting prototypes are
 * placed in inOutArray. Each couple of prototypes (the i-th in inArray and the i-th in inOutArray)
 * is reduced by adding together the sum of the coordinates of the data points that contribute to
 * the positioning of both prototypes (and consequently the number of data points that contribute
 * to the positioning of both prototypes).
 *
 * @param inArray A pointer to the array of prototypes of length nClusters that will be reduced
 *     together with those in inOutArray.
 * @param inOutArray A pointer to the array of prototypes of length nClusters that will be reduced
 *     together with those in inArray; the result of the reduction will be also placed in this
 *     array.
 * @param nClusters The number of prototypes to reduce (that is the number of elements both in
 *     inArray and inOutArray).
 * @param datatype Not used (kept for compatibility with the prototype of the user-defined function
 *     accepted by MPI_Op_create()).
 */
void reducePrototypes(void *inArray, void *inOutArray, int *nClusters, MPI_Datatype *datatype);

#endif
