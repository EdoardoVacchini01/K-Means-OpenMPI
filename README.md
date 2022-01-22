# K-Means-OpenMPI

## Execution Instructions

### 0 - Git repository setup
If not already done, download and install Git from [git-scm.com](https://git-scm.com/book/en/v2/Getting-Started-Installing-Git). Then clone the repository with the following command:
```
>> git clone https://github.com/EdoardoVacchini01/K-Means-OpenMPI.git
```

### 1 - Install OpenMPI on your computer
Download OpenMPI 4.0.5 from the official [OpenMPI website](https://www.open-mpi.org/software/ompi/v4.0/) and install it following the [official guide](https://www.open-mpi.org/faq/?category=building#easy-build). The project has been developed using OpenMPI 4.0.5, but consider dowloading any compatible version of your choice. Furthermore, if you are not interested in the parallel implementation, this step is optional.

Please note that due to a lack of interest, the Microsoft Windows OpenMPI version has been discontinued as stated on [OpenMPI website](https://www.open-mpi.org/software/ompi/v1.6/ms-windows.php).

### 2 - Generate the dataset
Provided you have installed Python on you computer, if you do not posses a dataset you want to run the K-Means clustering algorithm on, you can run the Python script ```generate_dataset.py``` with
```
>> py generate_dataset.py
```
Please note that if you intend to run the application with your own dataset, it must comply with two requisites:
- The first line of the dataset has to contain the number of data samples in the dataset.
- Each line of the file represents a different sample and is written such that each coordinate is separated by a single space.

### 3 - Compile and run the program
To compile and run the parallel application, first make sure to switch the branch to the ```parallel``` branch, and then compile and run the MPI application with:

```
>> mpicc -Wall *.c -o executableFile
>> mpirun -n N executableFile nClusters maxIterations dataset outputFile
```
Where ```N``` is the number of cores you either have or want to execute the parallel application on, ```executableFile``` is the name you want to give to the executable application, ```nClusters``` is the number of clusters you want the algorithm to find, ```maxIterations``` is the maximum number of iterations the algorithm will go through, ```dataset``` is the file containing the points to cluster, and finally ```outputFile``` is the optional output file.

If not set, ```maxIterations``` will be 100 by default. By not setting ```outputFile```, no file will be created and the clustering result will only be displayed on the terminal.


Please note that you need to compile multiple C files, so you either compile them by writing ```*.c``` or by listing them one by one. 
