# K-Means-OpenMPI

This repository contains the code for the final parallel programming project of the Advanced Computer Architecture course (AY 2021/22).

## Execution Instructions

### 1 - Setup the Git Repository
If not already done, download and install ```git``` from [git-scm.com](https://git-scm.com/book/en/v2/Getting-Started-Installing-Git). Then clone the repository with the following command:
```bash
$ git clone https://github.com/EdoardoVacchini01/K-Means-OpenMPI.git
```

### 2 - Install OpenMPI
Download Open MPI 4.0.5 from the official [Open MPI website](https://www.open-mpi.org/software/ompi/v4.0/) and install it following the [official guide](https://www.open-mpi.org/faq/?category=building#easy-build). The project has been developed using Open MPI 4.0.5, but consider dowloading any compatible version of your choice. Furthermore, if you are not interested in the parallel implementation, this step is optional.

Please note that due to a lack of interest, the Microsoft Windows Open MPI version has been discontinued as stated on [Open MPI website](https://www.open-mpi.org/software/ompi/v1.6/ms-windows.php).

### 3 - Generate the Dataset
Provided you have installed Python on you computer, if you do not have a dataset you want to run the K-Means clustering algorithm on, you can run the Python script ```generate_dataset.py``` to generate sample datasets of your choice. You can find a detailed description of the parameters for this script by running:
```bash
$ ./generate_dataset.py --help
```

Please note that if you intend to run the application with your own dataset, it must comply with two requisites:
- the first line of the dataset must contain the number of data points in the dataset;
- each line of the file must represent a single data point, with the coordinates separated by a single space.

### 4 - Compile and Run the Program
To compile and run the serial application, run the following commands:
```bash
$ gcc -Wall *.c -o executableFile
$ ./executableFile [datasetFile] [outputFile] [nClusters] [maxIterations]
```

If you want to compile and run the parallel application instead, first make sure to switch to the ```parallel``` branch, then compile and run the Open MPI program:
```bash
$ git checkout parallel
$ mpicc -Wall *.c -o executableFile
$ mpirun -n N executableFile [datasetFile] [outputFile] [nClusters] [maxIterations]
```

In the two commands above, ```N``` is the number of cores you want to execute the parallel application on, ```executableFile``` is the name you want to give to the executable application, ```datasetFile``` is the file containing the dataset, ```outputFile``` is the output file, ```nClusters``` is the number of clusters you want the algorithm to find and ```maxIterations``` is the maximum number of iterations the algorithm will go through.

Every parameter is optional (a default value will be used if a parameter is not provided), but the order of the parameters must match the one reported previously. If not set, ```nClusters``` will default to 3 while ```maxIterations``` will default to 100.
