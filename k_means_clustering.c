#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DIMENSION 2
#define BUFFER_LENGTH 4096

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


void printCentroid(centroid_t *centroid) {
    unsigned int coordinate = 0;

    for (coordinate = 0; coordinate < DIMENSION; coordinate++) {
        printf("%lf%s", centroid->coordinates[coordinate],
            (coordinate < DIMENSION - 1) ? " " : "\n");
    }
}


void printCentroids(centroid_t *centroids, unsigned int nClusters) {
    unsigned int cluster = 0;

    for (cluster = 0; cluster < nClusters; cluster++) {
        printf("Centroid of cluster #%d: ", cluster);
        printCentroid(centroids + cluster);
    }
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


unsigned int kMeansIteration(point_t *points, unsigned int nPoints, centroid_t *centroids, prototype_t *prototypes, unsigned int nClusters) {
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


int main(int argc, char *argv[]) {
    point_t *points = NULL;
    unsigned int nPoints = 0;
    unsigned int nClusters = (argc > 2) ? atoi(argv[2]) : 3;
    centroid_t *centroids = NULL;
    prototype_t *prototypes = NULL;
    unsigned int clustersChanged = 0;
    unsigned int iteration = 0;
    unsigned int maxIterations = (argc > 3) ? atoi(argv[3]) : 100;

    printf("Reading the dataset file...\n");

    nPoints = readDataset((argc > 1) ? argv[1] : "dataset.txt", &points);
    if (nPoints == 0) {
        printf("An error occurred while reading the dataset file.\n");
        return EXIT_FAILURE;
    }

    if (nClusters > nPoints) {
        free(points);
        printf("The number of clusters must be less than or equal to the number of data points.\n");
        return EXIT_FAILURE;
    }

    printf("Clustering the data points...\n");

    centroids = (centroid_t*) malloc(nClusters * sizeof(*centroids));
    if (centroids == NULL) {
        free(points);
        printf("An error occurred while allocating the memory for the centroids.\n");
        return EXIT_FAILURE;
    }

    prototypes = (prototype_t*) malloc(nClusters * sizeof(*prototypes));
    if (prototypes == NULL) {
        free(points);
        free(centroids);
        printf("An error occurred while allocating the memory for the prototypes.\n");
        return EXIT_FAILURE;
    }

    initCentroids(centroids, nClusters, points);
    do {
        initPrototypes(prototypes, nClusters);
        clustersChanged = kMeansIteration(points, nPoints, centroids, prototypes, nClusters);
        updateCentroids(centroids, prototypes, nClusters);

        iteration++;
    } while((iteration < maxIterations) && clustersChanged);

    printf("Clustering process completed.\n\n");

    printCentroids(centroids, nClusters);

    free(points);
    free(centroids);
    free(prototypes);
    return EXIT_SUCCESS;
}
