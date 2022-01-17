#include <stdlib.h>
#include <string.h>

#include "k_means_clustering_utils.h"

#define BUFFER_LENGTH 4096


void printPoint(point_t *point) {
    unsigned int coordinate = 0;

    for (coordinate = 0; coordinate < DIMENSION; coordinate++) {
        printf("%lf%s", point->coordinates[coordinate], (coordinate < DIMENSION - 1) ? " " : "\n");
    }
}


void printPoints(point_t *points, unsigned int nPoints) {
    unsigned int point = 0;

    for (point = 0; point < nPoints; point++) {
        printf("Point #%d: ", point);
        printPoint(points + point);
    }
}


void printCentroid(centroid_t *centroid, FILE *outputFile) {
    unsigned int coordinate = 0;

    for (coordinate = 0; coordinate < DIMENSION; coordinate++) {
        fprintf(outputFile, "%lf%s", centroid->coordinates[coordinate],
            (coordinate < DIMENSION - 1) ? " " : "\n");
    }
}


void printCentroids(centroid_t *centroids, unsigned int nClusters, FILE *outputFile) {
    unsigned int cluster = 0;

    for (cluster = 0; cluster < nClusters; cluster++) {
        printCentroid(centroids + cluster, outputFile);
    }
}


double getSquaredDistance(point_t *point, centroid_t *centroid) {
    unsigned int coordinate = 0;
    double difference = 0.0;
    double squaredDistance = 0.0;

    for (coordinate = 0; coordinate < DIMENSION; coordinate++) {
        difference = point->coordinates[coordinate] - centroid->coordinates[coordinate];
        squaredDistance += difference * difference;
    }

    return squaredDistance;
}


void initCentroids(centroid_t *centroids, unsigned int nClusters, point_t *points) {
    unsigned int cluster = 0;
    unsigned int coordinate = 0;

    for (cluster = 0; cluster < nClusters; cluster++) {
        for (coordinate = 0; coordinate < DIMENSION; coordinate++) {
            (centroids + cluster)->coordinates[coordinate] =
                (points + cluster)->coordinates[coordinate];
        }
    }
}


void initPrototypes(prototype_t *prototypes, unsigned int nClusters) {
    unsigned int cluster = 0;
    unsigned int coordinate = 0;

    for (cluster = 0; cluster < nClusters; cluster++) {
        for (coordinate = 0; coordinate < DIMENSION; coordinate++) {
            (prototypes + cluster)->pointsCoordinatesSum[coordinate] = 0.0;
        }
        (prototypes + cluster)->nPoints = 0;
    }
}


void updateCentroid(centroid_t *centroid, prototype_t *prototype) {
    unsigned int coordinate = 0;

    for (coordinate = 0; coordinate < DIMENSION; coordinate++) {
        centroid->coordinates[coordinate] =
            prototype->pointsCoordinatesSum[coordinate] / prototype->nPoints;
    }
}


void updateCentroids(centroid_t *centroids, prototype_t *prototypes, unsigned int nClusters) {
    unsigned int cluster = 0;

    for (cluster = 0; cluster < nClusters; cluster++) {
        updateCentroid(centroids + cluster, prototypes + cluster);
    }
}


void updatePrototype(prototype_t *prototype, point_t *point) {
    unsigned int coordinate = 0;

    for (coordinate = 0; coordinate < DIMENSION; coordinate++) {
        prototype->pointsCoordinatesSum[coordinate] += point->coordinates[coordinate];
    }

    prototype->nPoints++;
}


unsigned int kMeansIteration(point_t *points, unsigned int nPoints, centroid_t *centroids,
        prototype_t *prototypes, unsigned int nClusters) {
    unsigned int point = 0;
    double minimumSquaredDistance = 0.0;
    unsigned int oldCluster = 0;
    unsigned int cluster = 0;
    double squaredDistance = 0.0;
    unsigned int clustersChanged = 0;

    for (point = 0; point < nPoints; point++) {
        minimumSquaredDistance = __DBL_MAX__;
        oldCluster = (points + point)->clusterId;

        for (cluster = 0; cluster < nClusters; cluster++) {
            squaredDistance = getSquaredDistance(points + point, centroids + cluster);
            if (squaredDistance < minimumSquaredDistance) {
                minimumSquaredDistance = squaredDistance;
                (points + point)->clusterId = cluster;
            }
        }

        if ((points + point)->clusterId != oldCluster) {
            clustersChanged = 1;
        }

        updatePrototype(prototypes + (points + point)->clusterId, points + point);
    }

    return clustersChanged;
}


unsigned int readDataset(const char *path, point_t **points) {
    FILE *file = NULL;
    char buffer[BUFFER_LENGTH];
    unsigned int nPoints = 0;
    unsigned int point = 0;
    unsigned int tokens = 0;
    char *token = NULL;
    char coordinatesDelimiter[] = " ";

    file = fopen(path, "r");
    if (file == NULL) {
        return 0;
    }

    fgets(buffer, sizeof(buffer), file);
    sscanf(buffer, "%d", &nPoints);

    *points = (point_t*) malloc(nPoints * sizeof(**points));
    if (*points == NULL) {
        fclose(file);
        return 0;
    }

    while (fgets(buffer, sizeof(buffer), file) && point < nPoints) {
        tokens = 0;

        token = strtok(buffer, coordinatesDelimiter);
        while (token != NULL && tokens < DIMENSION) {
            (*points + point)->coordinates[tokens] = atof(token);
            tokens++;
            token = strtok(NULL, coordinatesDelimiter);
        }

        if (tokens != DIMENSION) {
            fclose(file);
            free(points);
            return 0;
        }

        (*points + point)->clusterId = 0;
        point++;
    }

    return nPoints;
}
