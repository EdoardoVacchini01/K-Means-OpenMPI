#!/usr/bin/env python3

import pandas as pd
from sklearn.cluster import KMeans
import sys

if __name__ == '__main__':
    points = pd.read_csv(sys.argv[1], sep=' ', header=None, skiprows=[0])

    k_means = KMeans(n_clusters=int(sys.argv[2]), init=points.iloc[0:int(sys.argv[2])], n_init=1,
        max_iter=100, tol=0.0, algorithm='full')
    fitted_k_means = k_means.fit(points)

    with open(sys.argv[3], 'w') as outputFile:
        for centroid in fitted_k_means.cluster_centers_:
            print(' '.join([f'{coordinate:.6}' for coordinate in centroid]), file=outputFile)
