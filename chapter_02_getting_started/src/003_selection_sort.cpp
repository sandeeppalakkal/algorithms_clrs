/**
Selection sort algorithm

In-place sorting algorithm. For ascending order sorting, the algorithm
selects the minimum in the array and moves it to the beginning of the
array. Then it finds the minimum from the remaining n-1 numbers and
moves it to the second position in the array. This is repeated until
it sorts the array. Selection sort is slower than insertion sort and
is used when memory is scarce.

Algorithm complexity: Theta(n^2)

Author: Sandeep Palakkal 
Email: sandeep.dion@gmail.com
13-Aug-2016
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
  Argfind function: finds argmin or argmax depending on sort_type.
  */
int argfind(Dtype *array, const int beg, const int end, const
        SORT_TYPE sort_type)
{
    Dtype elem = array[beg];
    int k = beg;
    for(int i = beg + 1; i < end; ++i)
    {
        if(compare(elem, array[i], sort_type))
        {
            elem = array[i];
            k = i;
        }
    }
    return k;
}


/**
  Swap function
  */
void swap(Dtype *x, Dtype *y)
{
    Dtype temp = *x;
    *x = *y;
    *y = temp;
}


/**
  Selection sort algorithm
  */
void selection_sort(Dtype *array, const int array_len, const SORT_TYPE
        sort_type)
{
    for(int i = 0; i < array_len - 1; ++i)
    {
        int k = argfind(array, i, array_len, sort_type);
        swap(&array[i], &array[k]);
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
  Read input array from file. The first element in the file should be
  the number of elements to be sorted.
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
  Write output (sorted array) to file.
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
    selection_sort(array, array_len, sort_type);
    time = clock() - time;
    cout << "Selection sort took "<< 
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

