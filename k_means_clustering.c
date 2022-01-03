#include <stdio.h>
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

int main() {
    
}
