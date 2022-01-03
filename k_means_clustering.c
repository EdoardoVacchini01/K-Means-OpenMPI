#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N_COORDINATES 2
#define BUFFER_LENGTH 1000

typedef struct {
    double coordinates[N_COORDINATES];
    unsigned int clusterId;
} point_t;

typedef struct {
    double coordinates[N_COORDINATES];
} centroid_t;

typedef struct {
    double pointsCoordinatesSum[N_COORDINATES];
    unsigned int nPoints;
} prototype_t;

double getSquaredDistance(point_t *point, centroid_t *centroid) {
    unsigned int coordinate = 0;
    double difference = 0.0;
    double squaredDistance = 0.0;

    for (coordinate = 0; coordinate < N_COORDINATES; coordinate++) {
        difference = point->coordinates[coordinate] - centroid->coordinates[coordinate];
        squaredDistance += difference * difference;
    }

    return squaredDistance;
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
        for (coordinate = 0; coordinate < N_COORDINATES; coordinate++) {
            (centroids + cluster)->coordinates[coordinate] =
                (points + cluster)->coordinates[coordinate];
        }
    }

    return centroids;
}

void updateCentroid(centroid_t *centroid, prototype_t *prototype){
    int i;
    for(i = 0; i < N_COORDINATES; i++)
        centroid->coordinates[i] = prototype->pointsCoordinatesSum[i]/prototype->nPoints;
}

void updatePrototype(prototype_t *prototype, point_t *point){
    int i;
    prototype->nPoints++;
    for(i = 0; i < N_COORDINATES; i++)
        prototype->pointsCoordinatesSum[i] += point->coordinates[i];
}

void kMeansClustering(centroid_t *centroids, unsigned int nClusters, point_t *dataset, unsigned int nPoints, unsigned int maxIterations){
    centroids = initCentroids(dataset, nPoints, nClusters);

    int i = 0, j, k, isChanged, oldCluster;
    double minimumDistance, distance;
    prototype_t *prototypes = malloc(nClusters * sizeof(*centroids)); //STA NEL MAIN E POI LO SI PASSA COME PARAMETRO O QUI?

    do{
        isChanged = 0;

        //azzera le somme delle coordinate
        for(k = 0; k < nClusters; k++)
            for(j = 0; j < N_COORDINATES; j++){
                prototypes[k].pointsCoordinatesSum[j] = 0;
                prototypes[k].nPoints = 0;
            }

        //assegnare a tutti i punti il cluster basandosi sulla distanza
        for(j = 0; j < nPoints; j++){
            minimumDistance = __DBL_MAX__;
            oldCluster = dataset[j].clusterId;
            for(k = 0; k < nClusters; k++){
                distance = getSquaredDistance(&dataset[j], &centroids[j]);
                if(distance < minimumDistance){
                    minimumDistance = distance;
                    dataset[j].clusterId = k;
                }
            }
            updatePrototype(&prototypes[dataset[j].clusterId], &dataset[j]);
            if(oldCluster != dataset[j].clusterId)
                isChanged = 1;
        }

        //ricalcolare il centroide
        for(k = 0; k < nClusters; k++)
            updateCentroid(&centroids[k], &prototypes[k]);
    }while((++i < maxIterations) && isChanged);
    free(prototypes);
}

point_t *readDataset(int *nPoints, char *path) {
    FILE *file = fopen(path, "r");
    if (!file) {
        return NULL;
    }

    // The first line of the file contains the number of points in the file
    // the array of points is prepared accordingly so
    point_t *points;
    char buffer[BUFFER_LENGTH];
    fgets(buffer, sizeof(buffer), file);
    sscanf(buffer, "%d", nPoints);
    if (!( points = malloc((*nPoints) * sizeof(*points)) ))
        return NULL;

    // For all the lines, it reads them, tokenize them one token by one
    // token and if the number of token is different from the number of 
    // dimensions stops the procedure and returns NULL
    int converted, i;
    char *delimiter = " ", *token = NULL;
    for ( i = 0; fgets(buffer, sizeof(buffer), file); i++ ) {
        converted = 0;
        token = strtok( buffer, delimiter );
        while( token != NULL ) {
            token = strtok( NULL, delimiter );
            points[i].coordinates[converted] = atof(token);
            converted ++;
        }   
        if (converted != N_COORDINATES){
            free(points);
            return NULL;
        }
    }

    return points;
}

int main() {
    return 0;
}
