#ifndef K_MEANS_CLUSTERING_UTILS_H
#define K_MEANS_CLUSTERING_UTILS_H

#include <stdio.h>

/**
 * @brief Dimension of the space in which the data points lie.
 */
#define DIMENSION 2

/**
 * @brief A single data point of the dataset.
 */
typedef struct {
    /**
     * @brief Coordinates of the data point in the space (of dimension DIMENSION) in which it lies.
     */
    double coordinates[DIMENSION];

    /**
     * @brief ID of the cluster to which the data point has been assigned (index of the
     *     corresponding cluster in the centroids array).
     */
    unsigned int clusterId;
} point_t;

/**
 * @brief The centroid of a cluster.
 */
typedef struct {
    /**
     * @brief Coordinates of the centroid in the space (of dimension DIMENSION) in which it lies.
     */
    double coordinates[DIMENSION];
} centroid_t;

/**
 * @brief The new centroid (prototype) of a cluster calculated by each K-Means iteration.
 */
typedef struct {
    /**
     * @brief Sum of the coordinates of the data points that contribute to the positioning of the
     *     prototype.
     */
    double pointsCoordinatesSum[DIMENSION];

    /**
     * @brief Number of data points that contribute to the positioning of the prototype.
     */
    unsigned int nPoints;
} prototype_t;


/**
 * @brief Prints the clustering labels of the data points given as parameter.
 *
 * @param points A pointer to the array of data points of which to print the labels.
 * @param nPoints The number of data points of which to print the labels.
 * @param outputFile A pointer to the file to which the labels will be printed.
 */
void printPointLabels(point_t *points, unsigned int nPoints, FILE *outputFile);


/**
 * @brief Prints the coordinates of the centroid given as parameter.
 *
 * @param centroid A pointer to the centroid to print.
 * @param outputFile A pointer to the file to which the centroid coordinates will be printed.
 */
void printCentroid(centroid_t *centroid, FILE *outputFile);


/**
 * @brief Prints the coordinates of the centroids given as parameter.
 *
 * @param centroids A pointer to the array of centroids to print.
 * @param nClusters The number of centroids to print.
 * @param outputFile A pointer to the file to which the centroids coordinates will be printed.
 */
void printCentroids(centroid_t *centroids, unsigned int nClusters, FILE *outputFile);


/**
 * @brief Returns the square of the euclidean distance between the data point and the centroid given
 *     as parameters.
 *
 * @param point A pointer to a data point.
 * @param centroid A pointer to a centroid.
 * @return The square of the euclidean distance between the data point and the centroid.
 */
double getSquaredDistance(point_t *point, centroid_t *centroid);


/**
 * @brief Initializes the centroids given as parameters by placing them in the same positions as the
 *     first data points of the dataset (first centroid in correspondence of the first data point,
 *     second centroid in correspondence of the second data point and so on).
 *
 * @param centroids A pointer to the array of centroids to initialize.
 * @param nClusters The number of centroids to initialize.
 * @param points A pointer to the array of data points that will be used to initialize the
 *     centroids.
 */
void initCentroids(centroid_t *centroids, unsigned int nClusters, point_t *points);


/**
 * @brief Initializes the prototypes given as parameter by setting all their data fields to 0.
 *
 * @param prototypes A pointer to the array of prototypes to initialize.
 * @param nClusters The number of prototypes to initialize.
 */
void initPrototypes(prototype_t *prototypes, unsigned int nClusters);


/**
 * @brief Places the centroid given as parameter in the center of mass of the data points that
 *     contribute to the positioning of the prototype, also given as parameter.
 *
 * @param centroid A pointer to the centroid to update.
 * @param prototype A pointer to the prototype that will be used to update the centroid.
 */
void updateCentroid(centroid_t *centroid, prototype_t *prototype);


/**
 * @brief Places each centroid given as parameter in the center of mass of the data points that
 *     contribute to the positioning of the associated prototype, also given as parameter.
 *
 * @param centroids A pointer to the array of centroids to update.
 * @param prototypes A pointer to the array of prototypes that will be used to update the centroids.
 * @param nClusters The number of centroids to update.
 */
void updateCentroids(centroid_t *centroids, prototype_t *prototypes, unsigned int nClusters);


/**
 * @brief Updates the prototype given as parameter by adding the coordinates of the data point (also
 *     given as parameter) to the sum of the coordinates already in the prototype and incrementing
 *     the number of data points that contribute to the positioning of the prototype.
 *
 * @param prototype A pointer to the prototype to update.
 * @param point A pointer to the data point with which the prototype will be updated.
 */
void updatePrototype(prototype_t *prototype, point_t *point);


/**
 * @brief Performs a single K-Means iteration.
 *
 * The array of data points is traversed. For each data point its closest centroid is identified and
 * the corresponding cluster ID is memorized. Each point is used to update the prototype of the
 * corresponding cluster, in order to obtain in the end an array of prototypes ready to provide the
 * updated centroids (the ones that will be used in the next iteration).
 *
 * To allow the detection of convergence, the function returns 1 if at least one data point has been
 * assigned to a different cluster with respect to the previous one in the current iteration, 0
 * otherwise.
 *
 * @param points A pointer to the array of data points.
 * @param nPoints The number of data points.
 * @param centroids A pointer to the array of current centroids, one for each cluster.
 * @param prototypes A pointer to the array of prototypes that will be updated in the current
 *     K-Means iteration.
 * @param nClusters The number of clusters.
 * @return 1 if at least one data point has been assigned to a different cluster with respect to the
 *     previous one in the current iteration, 0 otherwise.
 */
unsigned int kMeansIteration(point_t *points, unsigned int nPoints, centroid_t *centroids,
    prototype_t *prototypes, unsigned int nClusters);


/**
 * @brief Reads the dataset file whose name is given as parameter and loads the data points that it
 *     contains into an array.
 *
 * The first line of the dataset file must contain an integer number that represents the number of
 * data points in the dataset. After the first line each line represents a single data point, with
 * its coordinates separated by a whitespace.
 *
 * The cluster ID of each data point is initialized to 0.
 *
 * If the dataset file cannot be read, or if at least one data point is not represented in the right
 * format, the function returns 0 and does not initialize the array of data points.
 *
 * @param path A string containing the path to the dataset file to read.
 * @param points A pointer to a pointer to the array of data points read from the dataset file (the
 *     pointer to the array will be set by the function).
 * @return The number of data points read from the dataset file.
 */
unsigned int readDataset(const char *path, point_t **points);

#endif
