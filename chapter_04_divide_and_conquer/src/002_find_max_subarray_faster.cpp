/**
  Find maximum subarray by divide and conquer approach. Instead of subdividing
  array at the middle, start from the first element (i.e., subarray of length 1
  and each time add the next element on the right side to it. If maximum
  subarray is found for a subarray, once an element is augmented to the right,
  the maximum subarray of the new array is either the same as the left subarray
  or starts from an element in the left subarray and extends upto and including
  the newly augmented element. The subarray that ends at the middle can be found
  by checking the sum starting from it and scanning leftward. On the other hand,
  this sum can be maintained for each subarray and updated when a new element
  is augmented to the subarray on the right side. It is important to note that a
  maximum subarray does not start with a negative  number or zero. It only
  starts with a positive number. So, this sum must be modified to zero when it
  goes below zero. The resulting algorithm is faster.

  This is called Kadane's algorithm.

  Complexity: n

Author: Sandeep Palakkal
Email: sandeep.dion@gmail.com
Created on: 21-Aug-2016
*/

#include <iostream>
#include <string>
#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <ctime>

using namespace std;

typedef float Dtype;
enum SORT_TYPE {ASCEND, DESCEND, INVALID};


/**
  Find the maximum subarray faster algorithm.
  */
void find_max_subarray_faster(const Dtype *array, int array_len,
        int &beg, int &end, Dtype &sum)
{
    //If the array length is zero, return
    if(array_len == 0)
    {
        beg = end = 0;
        sum = 0;
        return;
    }

    //Start from the first element
    sum = array[0];
    beg = 0;
    end = 1;
    
    Dtype sum_so_far = sum;
    int left = beg;
    if(sum_so_far < 0)
    {
        sum_so_far = 0;
        left = 1;
    }
    for(int i = 1; i < array_len; ++i)
    {
        Dtype sum_here = sum_so_far + array[i];
        if(sum_here > sum)
        {
            sum = sum_here;
            beg = left;
            end = i + 1;
        }
        if(sum_here < 0) 
        {
            sum_so_far = 0;
            left = i + 1;
        }
        else sum_so_far = sum_here;
    }
}


/**
  Read array from terminal
  */
bool read_array_term(Dtype **array, int &array_len)
{
    cout << "Enter the length of the array: ";
    cin >> array_len;
    if(array_len <= 0) return true;
    *array = new Dtype[array_len];
    cout << "Enter the array elements: ";
    for(int i = 0; i < array_len; ++i)
    {
        cin >> (*array)[i];
    }
   return true;
}


/**
  Read array from file
  */
bool read_array_file(const string filename, Dtype **array, int &array_len)
{
    ifstream fp {filename};
    cout << "Reading input file... ";
    if(!fp.is_open())
    {
        cout << "Error: Input file could not be opened\n";
        return false;
    }
    fp >> array_len;
    if(array_len <= 0) return true;
    *array = new Dtype[array_len];
    for(int i = 0; i < array_len; ++i)
    {
        fp >> (*array)[i];
    }
    fp.close();
    cout << "Read " << array_len << " elements." << endl;
    return true;
}


/**
  Write array
  */
bool write_array_file(const string filename, const Dtype *array, 
        const int array_len)
{
    ofstream ofp{filename};
    if(!ofp.is_open())
    {
        cout << "Could not open file for writing.\n";
        return false;
    }
    ofp << fixed;
    for(int i = 0; i < array_len; ++i)
    {
        ofp << array[i] << endl;
    }
    ofp.close();
    cout << "Written array of length " << array_len << " in ";
    cout << filename << endl;
    return true;
}


/**
  Prints usage
  */
void usage(const string command)
{
    cout << "Find maximum subarray. Usage:\n\n";
    cout << "1. " << command << "\n   No input arguments. Reads all inputs from"\
        " terminal.\n\n";
    cout << "2. " << command << " -i <input file>" << endl;
        
    cout << "   Reads input array from input file. First element in the file"\
        " must be the length of the array.\n";
}


/**
  Parse and get all inputs
  */
bool parse_and_get_inputs(const int argc, char **argv, Dtype **array, 
        int &array_len)
{
    if(argc % 2 == 0)
    {
        usage(string(argv[0]));
        return false;
    }
    *array = nullptr;
    for(int i = 1; i < argc; i += 2)
    {
        string arg_opt {argv[i]};
        if(arg_opt == "-h" || arg_opt == "--help")
        {
            usage(string(argv[0]));
            return false;
        }
        if(arg_opt == "-i" || arg_opt == "--input")
        {
            if(!read_array_file(string(argv[i + 1]), array, array_len))
            {
                return false;
            }
        }
    }
    if(*array == nullptr)
    {
        if(!read_array_term(array, array_len))
        {
            return false;
        }
    }
    return true;
}

int main(int argc, char **argv)
{
    Dtype *array = nullptr;
    int array_len = 0;
    
    //Parse and get inputs
    if(!parse_and_get_inputs(argc, argv, &array, array_len))
    {
        return 0;
    }

    //Find max subarray
    int beg, end;
    Dtype sum;
    clock_t t0 = clock();
    find_max_subarray_faster(array, array_len, beg, end, sum);
    t0 = clock() - t0;
    cout << "Time taken to find the subarray: " 
        << float(t0)/CLOCKS_PER_SEC * 1000 << " ms.\n";

    //Print result
    if(end > beg)
    {
        cout << fixed;
        cout << "Maximum subarray sum is " << sum << endl;
        cout << "Maximum subarray lies at [" << beg + 1 << ", " << end << "].\n";
    }
    else
    {
        cout << "Maximum subarray is empty!" << endl;
    }
        
    //Free memory if any
    if(array != nullptr)
    {
        delete[] array;
    }

    return 0;
}

