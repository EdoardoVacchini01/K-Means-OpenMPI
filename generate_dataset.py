#!/usr/bin/env python3

import argparse
import random


def generate_dataset(points, dimension, output_filename, random_seed=None):
    """
    Generates a dataset file containing random data points in the unit hypercube.

    The first line of the generated file contains an integer value that represents the number of
    data points contained in the subsequent lines. Each data point is represented by a
    whitespace-separated list of pseudo-random floating point coordinates, each between 0.0 and 1.0
    (with 10 decimal places).

    Args:
        points: An integer representing the number of data points that make up the dataset.
        dimension: An integer representing the dimension of the space in which the data points lie
            (the number of coordinates of each data point).
        output_filename: A string representing the filename of the output file.
        random_seed: An integer used as the seed for the pseudo-random number generator. If None,
            the result will not be replicable.
    """

    random.seed(random_seed)

    with open(output_filename, 'w') as output_file:
        # Write the number of data points in the first line of the output file
        print(points, file=output_file)

        # Write one data point per line, with the coordinates separated by a whitespace
        for _ in range(points):
            coordinates = [f'{random.random():.10f}' for _ in range(dimension)]
            print(' '.join(coordinates), file=output_file)


def parse_positive_int(string):
    """
    Parses the positive integer from the string argument.

    Args:
        string: The string from which to parse the positive integer.

    Returns:
        The positive integer parsed from the string argument.

    Raises:
        argparse.ArgumentTypeError: If the integer parsed from the string is zero or negative.
    """

    parsed_int = int(string)
    if parsed_int <= 0:
        raise argparse.ArgumentTypeError(f'invalid positive int value: {parsed_int}')
    return parsed_int


if __name__ == '__main__':
    # Define the argument parser and parse the actual command line arguments
    argumentParser = argparse.ArgumentParser(
        description='Generate a dataset file containing random data points in the unit hypercube.',
        formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    argumentParser.add_argument('-p', '--points', default=100, type=parse_positive_int,
        help='number of data points')
    argumentParser.add_argument('-d', '--dimension', default=3, type=parse_positive_int,
        help='dimension of the space in which the data points lie')
    argumentParser.add_argument('-o', '--output-filename', default='dataset.txt', type=str,
        help='name of the output file')
    argumentParser.add_argument('-s', '--random-seed', default=None, type=int,
        help='random seed of the pseudo-random number generator')
    args = argumentParser.parse_args()

    # Generate the dataset using the command line parameters as keyword arguments
    generate_dataset(**vars(args))
