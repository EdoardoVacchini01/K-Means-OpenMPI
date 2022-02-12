#!/usr/bin/env python3

import pandas as pd
from sklearn.cluster import KMeans
import sys

if __name__ == '__main__':
    # Read the data points of the dataset
    points = pd.read_csv(sys.argv[1] if len(sys.argv) > 1 else 'dataset.txt', sep=' ', header=None,
        skiprows=[0])

    # Get the number of clusters to identify in the dataset
    n_clusters = int(sys.argv[3]) if len(sys.argv) > 3 else 3

    # Get the maximum number of iterations to perform
    max_iterations = int(sys.argv[4]) if len(sys.argv) > 4 else 100

    # Run K-Means with the selected settings using the data points of the dataset
    k_means = KMeans(n_clusters=n_clusters, init=points.iloc[0:n_clusters], n_init=1,
        max_iter=max_iterations, tol=0.0, algorithm='full')
    fitted_k_means = k_means.fit(points)

    # Print the centroids to the output file
    with open(sys.argv[2] if len(sys.argv) > 2 else 'centroids.txt', 'w') as output_file:
        for centroid in fitted_k_means.cluster_centers_:
            print(' '.join([f'{coordinate:.10f}' for coordinate in centroid]), file=output_file)
