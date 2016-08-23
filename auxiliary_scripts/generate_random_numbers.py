#!/usr/bin/python 
'''Code to generate random array

Author: Sandeep Palakkal 
Email: sandeep.dion@gmail.com 
Created on:12-Aug-2016'''

import sys 
import getopt
import numpy as np


def usage():
    print 'Usage: %s -d <int|float> -l <lower bound> -u '\
        '<upper bound> -o <output file>' % sys.argv[0]
    sys.exit(2)


def parse_args():
    if len(sys.argv) in range(1,6):
        usage()
    try: 
        opts, args = getopt.getopt(sys.argv[1:], "hd:l:u:o:n:", ["help",
                "d=", "l=", "u=", "o=", "n="])
            
    except getopt.GetoptError as e: 
        print e 
        usage()
    dtype, lower, upper, out_file = '','','',''
    for o, a in opts:
        if o == '--d':
            dtype = a
        elif o == '--l':
            lower = int(a)
        elif o == '--u':
            upper = int(a)
        elif o == '--o':
            out_file = a
        elif o == "--n":
            size = int(a)
        elif o in ["-h", "--help"]:
            usage()
    return dtype, lower, upper, out_file, size


def generate_random(dtype, lower, upper, size):
    if dtype == 'float':
        return np.random.random(size) * (upper - lower) + lower
    elif dtype == 'int':
        return np.random.random_integers(lower, upper, size)

def main():
    dtype, lower, upper, out_file, size = parse_args()
    numbers = generate_random(dtype, lower, upper, size)
    if dtype == 'int':
        np.savetxt(out_file, numbers, newline = "\n", fmt = "%d")
    elif dtype == 'float':
        np.savetxt(out_file, numbers, newline = "\n", fmt = "%.6f")

if __name__ == '__main__': 
    main()
