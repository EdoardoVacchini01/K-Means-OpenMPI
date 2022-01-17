#ifndef K_MEANS_CLUSTERING_UTILS_H
#define K_MEANS_CLUSTERING_UTILS_H

#include <stdio.h>

#define DIMENSION 2

typedef struct {
    double coordinates[DIMENSION];
    unsigned int clusterId;
} point_t;

typedef struct {
    double coordinates[DIMENSION];
} centroid_t;

typedef struct {
    double pointsCoordinatesSum[DIMENSION];
    unsigned int nPoints;
} prototype_t;

void printPoint(point_t *point);

void printPoints(point_t *points, unsigned int nPoints);

void printCentroid(centroid_t *centroid, FILE *outputFile);

void printCentroids(centroid_t *centroids, unsigned int nClusters, FILE *outputFile);

double getSquaredDistance(point_t *point, centroid_t *centroid);

void initCentroids(centroid_t *centroids, unsigned int nClusters, point_t *points);

void initPrototypes(prototype_t *prototypes, unsigned int nClusters);

void updateCentroid(centroid_t *centroid, prototype_t *prototype);

void updateCentroids(centroid_t *centroids, prototype_t *prototypes, unsigned int nClusters);

void updatePrototype(prototype_t *prototype, point_t *point);

unsigned int kMeansIteration(point_t *points, unsigned int nPoints, centroid_t *centroids,
    prototype_t *prototypes, unsigned int nClusters);

unsigned int readDataset(const char *path, point_t **points);

#endif
