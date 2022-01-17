from sklearn.cluster import KMeans
import pandas as pd
import sys


if __name__ == '__main__':
    fileName = sys.argv[1]
    df = pd.read_csv(fileName, sep=' ', header=None, skiprows=[0])
    p = KMeans(n_clusters=int(sys.argv[2]), max_iter=100, init=df.iloc[0:int(sys.argv[2])], tol=0.0, algorithm='full')
    cluster = p.fit(df);

    with open(sys.argv[3], 'w') as outfile:
        for x in p.cluster_centers_ :
            print(" ".join([f"{y:.6}" for y in x]), file=outfile)
