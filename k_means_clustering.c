#include <stdio.h>

#define N_COORDINATES 2
#define N_CLUSTERS 5

typedef struct {
    double coordinates[N_COORDINATES];
    unsigned int clusterId;
} point_t;

typedef struct {
    double coordinates[N_COORDINATES];
    unsigned int clusterId;
} centroid_t;

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
        (centroids + cluster)->clusterId = cluster;
    }

    return centroids;
}

int main() {
    
}
