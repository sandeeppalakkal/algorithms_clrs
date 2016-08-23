/**
Insertion sort algorithm

Sorts an array of numbers "in place". It sorts from left to right.
Consider i-th element and assume elements on the left side are already
sorted. The algorithm scans through the elements on the left in left
direction and inserts the i-th element in it's position according the
sort order. Thus this algorithm uses an incremental approach to
solving the problem: sort a subarray, put a new element in its correct
position and so on.

Algorithm complexity: Theta(n^2)

Author: Sandeep Palakkal 
Email: sandeep.dion@gmail.com
12-Aug-2016
*/

#include <iostream>
#include <fstream>
#include <string>
#include <cstddef>
#include <ctime>
#include <iomanip>

using namespace std;

//input array type
typedef float Dtype;
//typedef char Dtype;

//sort type
enum SORT_TYPE {ASCEND, DESCEND};

/**
  returns true if a > b if sort_type if ASCEND
  returns true if a < b if sort_type if DESCEND
  In either case, if returned value is true, that means a should go
  after b according to the sorting order.
*/
bool compare(const Dtype a, const Dtype b, const SORT_TYPE sort_type)
{
    switch(sort_type)
    {
        case ASCEND:
            if(a > b) return true;
            break;
        case DESCEND:
            if(a < b) return true;
            break;
        default:
            cout << "Unkown sort type. Exiting" << endl;
            break;
    }
    return false;
}


/**
  Insertion sort algorithm
  */
void insertion_sort(Dtype *array, const int array_len, const SORT_TYPE sort_type)
{
    //loop through the array from 2nd element onwards
    for(int i = 1; i < array_len; ++i)
    {
        //consider the current element, loop through elements on the
        //left side right-to-left and insert the current number at its
        //correct place according the sorting type
        Dtype cur_elem = array[i];
        int j = i - 1;
        while(j >= 0 &&  compare(array[j], cur_elem, sort_type))
        {
            array[j+1] = array[j--];            
        }
        array[j+1] = cur_elem;
    }
}


/**
  Read input array from terminal
  */
void read_array_term(Dtype **array, int &array_len)
{
    cout << "Enter length of array to be sorted: ";
    cin >> array_len;
    if(array_len <= 0) return;
    *array = new Dtype[array_len];
    cout << "Enter a number array to be sorted: ";
    for(int i = 0; i < array_len; ++i)
    {
        cin >> (*array)[i];
    }
}


/**
  Write output to terminal
  */
void write_array_term(Dtype *array, int array_len)
{
        cout << "Sorted array:\n";
        for(int i = 0; i < array_len; ++i)
        {
            cout << array[i] << " ";
        }
        cout << endl;
}


/**
  Read input array from file
  */
bool read_array(const char *ifile, Dtype **array, int &array_len, SORT_TYPE &sort_type)
{
    ifstream in_file(ifile);
    if(in_file.is_open())
    {
        in_file >> array_len;
        if(array_len <= 0) return true;
        *array = new Dtype[array_len];
        for(int i = 0; i < array_len; ++i)
        {
            in_file >> (*array)[i];
        }
        in_file.close();
        return true;
    }
    else
    {
        return false;
    }
}


/**
  Write output to file
  */
bool write_array(const char *ofile, const Dtype *array, const int array_len)
{
    ofstream out_file(ofile);
    if(out_file.is_open())
    {
        out_file << fixed;
        for(int i = 0; i < array_len; ++i)
        {
            out_file << array[i] << endl;
        }
        out_file.close();
        return true;
    }
    else
    {
        return false;
    }
}


/**
  Helper function
  */
void usage(char *exe_file)
{
    cout << "Usage:\n1. " << exe_file << " (No arguments. Reads "\
        "input array from terminal.)" << endl;
    cout << "2. " << exe_file << "<input_file> (Input is read from"\
        " input_file.)" << endl;
    cout << "3. " << exe_file << "<input_file> <output_file> (Reads"\
        " input from input_file and writes output to output_file.)\n";
}

int main(int argc, char **argv)
{
    //Helper
    if(argc > 1)
    {
        if(string(argv[1]) == "-h" || string(argv[1]) == "--help")
        {
            usage(argv[0]);
            return 0;
        }
    }

    //Get the sort type (ascend or descend) from the terminal
    cout << "Select sort type:\n";
    cout << "1. ASCEND\n2. DESCEND\n";
    short sort_type_;
    cin >> sort_type_;
    if(sort_type_ != 1 &&  sort_type_ != 2)
    {
        cout << "Unkown choice\n";
        return 1;
    }
    //Set the sort type 
    SORT_TYPE sort_type;
    sort_type = (sort_type_ == 1)? ASCEND : DESCEND;
    int array_len;
    Dtype *array {nullptr};

    if(argc == 1)
    {
        //Read from terminal
        read_array_term(&array, array_len);
    }
    else
    {
        //Read from the file
        if(!read_array(argv[1], &array, array_len, sort_type))
        {
            cout << "Error in reading the input file" << endl;
        }
    }
    
    //Call sorting algorithm
    clock_t time = clock();
    insertion_sort(array, array_len, sort_type);
    time = clock() - time;
    cout << "Insertion sort took "<< 
        float(time) / CLOCKS_PER_SEC * 1000 << " ms." << endl;

    if(argc == 1 || argc == 2)
    {
        //If output file is not specified, display output on terminal
        write_array_term(array, array_len);
    }
    else
    {
        //Save output in a file
        if(!write_array(argv[2], array, array_len))
        {
            cout << "Error in writing the output file" << endl;
        }
    }

    //Free memory
    delete[] array;
    
    return 0;
}

