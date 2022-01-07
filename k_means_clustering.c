#include <mpi.h>
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


centroid_t *initCentroids(point_t *points, unsigned int nPoints, unsigned int nClusters) {
    centroid_t *centroids = NULL;
    unsigned int cluster = 0;
    unsigned int coordinate = 0;

    if (nClusters > nPoints) {
        return NULL;
    }

    centroids = (centroid_t*) malloc(nClusters * sizeof(*centroids));
    if (centroids == NULL) {
        return NULL;
    }

    for (cluster = 0; cluster < nClusters; cluster++) {
        for (coordinate = 0; coordinate < DIMENSION; coordinate++) {
            (centroids + cluster)->coordinates[coordinate] =
                (points + cluster)->coordinates[coordinate];
        }
    }

    return centroids;
}

void initPrototypes(prototype_t *prototypes, unsigned int nClusters){
    unsigned int cluster = 0;
    unsigned int coordinate = 0;

    for (cluster = 0; cluster < nClusters; cluster++) {
        for (coordinate = 0; coordinate < DIMENSION; coordinate++) {
            (prototypes + cluster)->pointsCoordinatesSum[coordinate] = 0;
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

void updateCentroids(centroid_t *centroids, prototype_t *prototypes, unsigned int nClusters){
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

unsigned int kMeansIteration(point_t *points, unsigned int nPoints, centroid_t *centroids, prototype_t *prototypes, unsigned int nClusters){
    unsigned int point = 0;
    unsigned int cluster = 0;
    unsigned int oldCluster = 0;
    unsigned int clustersChanged = 0;
    double minimumSquaredDistance = 0.0;
    double squaredDistance = 0.0;

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

        if((points + point)->clusterId != oldCluster) {
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
    unsigned int tokens = 0;
    char *token = NULL;
    char coordinatesDelimiter[] = " ";
    unsigned int point = 0;

    file = fopen(path, "r");
    if (file == NULL) {
        return 0;
    }

    // The first line of the file contains the number of points in the file (so the array of points
    // is prepared accordingly)
    fgets(buffer, sizeof(buffer), file);
    sscanf(buffer, "%d", &nPoints);

    *points = (point_t*) malloc(nPoints * sizeof(**points));
    if (*points == NULL) {
        fclose(file);
        return 0;
    }

    // Every line is tokenized and if the number of tokens is different from the dimension of the
    // space we stop the procedure and return 0
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


void initDatatypes(MPI_Datatype *pointDatatype, MPI_Datatype *centroidDatatype, MPI_Datatype *prototypeDatatype) {
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
    MPI_Type_create_struct(1, centroidBlockLengths, centroidIndices, centroidTypes, centroidDatatype);
    MPI_Type_create_struct(2, prototypeBlockLengths, prototypeIndices, prototypeTypes, prototypeDatatype);
    MPI_Type_commit(pointDatatype);
    MPI_Type_commit(centroidDatatype);
    MPI_Type_commit(prototypeDatatype);
}


int main(int argc, char *argv[]) {
    int rank = 0;
    int communicatorSize = 0;
    point_t *points = NULL;
    unsigned int nPoints = 0;
    centroid_t *centroids = NULL;
    unsigned int nClusters = (argc > 2) ? atoi(argv[2]) : 3;
    unsigned int maxIterations = (argc > 3) ? atoi(argv[3]) : 100;
    prototype_t *prototypes = NULL;
    unsigned int clustersChanged = 0;
    unsigned int iteration = 0;
    MPI_Datatype pointDatatype = NULL;
    MPI_Datatype centroidDatatype = NULL;
    MPI_Datatype prototypeDatatype = NULL;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &communicatorSize);

    initDatatypes(&pointDatatype, &centroidDatatype, &prototypeDatatype);

    printf("Reading the dataset file...\n");
    nPoints = readDataset((argc > 1) ? argv[1] : "dataset.txt", &points);
    if (nPoints == 0) {
        printf("An error occurred while reading the dataset file.\n");
        return EXIT_FAILURE;
    }

    printf("Clustering the data points...\n");
    centroids = initCentroids(points, nPoints, nClusters);
    if (centroids == NULL) {
        free(points);
        printf("An error occurred while clustering the data points.\n");
        return EXIT_FAILURE;
    }

    prototypes = (prototype_t*) malloc(nClusters * sizeof(*prototypes));
    if (prototypes == NULL) {
        free(points);
        free(centroids);
        printf("An error occurred while clustering the data points.\n");
        return EXIT_FAILURE;
    }
    do {
        initPrototypes(prototypes, nClusters);
        clustersChanged = kMeansIteration(points, nPoints, centroids, prototypes, nClusters);
        updateCentroids(centroids, prototypes, nClusters);

    } while((++iteration < maxIterations) && clustersChanged);

    printf("\nClustering process completed.\n");
    printCentroids(centroids, nClusters);

    free(points);
    free(centroids);
    free(prototypes);

    MPI_Finalize();

    return EXIT_SUCCESS;
}