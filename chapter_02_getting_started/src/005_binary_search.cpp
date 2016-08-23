/**

Binary Search in a sorted array.

Since the array is sorted, to search for a value, we can compare the
middle value. Depending on whether the middle value is greater or less
than the search value, and depending on whether the array is sorted in
ascending or descending order, we can reduce the search into one of
the halves of the array. If repeated, this procedure reduces the array
size to be searched by half every time. So, algorithm is log n.

Complexity: Theta(log n)

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
  Binary search
  */
int binary_search(Dtype *array, int beg, int end, Dtype value, SORT_TYPE sort_type)
{
    if(end == beg) return -1;
    if(end - beg == 1)
    {
        return (array[beg] == value) ? beg : -1;
    }
    int mid = (beg + end) / 2;
    if(array[mid] == value)
    {
        return mid;
    }
    if(compare(array[mid], value, sort_type))
    {
        return binary_search(array, beg, mid, value, sort_type);
    }
    else
    {
        return binary_search(array, mid, end, value, sort_type);
    }
}


/**
  Boilerplate function for binary search
  */
int binary_search(Dtype *array, int array_len, Dtype value, SORT_TYPE sort_type)
{
    return binary_search(array, 0, array_len, value, sort_type);
}


/**
  Read input array from terminal
  */
void read_array_term(Dtype **array, int &array_len)
{
    cout << "Enter length of array to be searched in: ";
    cin >> array_len;
    if(array_len <= 0) return;
    *array = new Dtype[array_len];
    cout << "Enter a number array to be searched in: ";
    for(int i = 0; i < array_len; ++i)
    {
        cin >> (*array)[i];
    }
}


/**
  Read input array from file. The first element in the file should be
  the size of the array.
  */
bool read_array(const char *ifile, Dtype **array, int &array_len)
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
    Dtype value;

    if(argc == 1)
    {
        //Read from terminal
        read_array_term(&array, array_len);
    }
    else
    {
        //Read from the file
        if(!read_array(argv[1], &array, array_len))
        {
            cout << "Error in reading the input file" << endl;
        }
    }

    //Read the element to be searched
    cout << "Enter the value to be searched for: ";
    cin >> value;
    
    //Call search algorithm
    clock_t time = clock();
    int pos = binary_search(array, array_len, value, sort_type);
    time = clock() - time;
    cout << "Binary search took "<< 
        float(time) / CLOCKS_PER_SEC * 1000 << " ms." << endl;

    if(pos != -1)
    {    
        cout << "Searched value appears at: " << pos + 1 << endl;
        cout << "Array[" << pos << "] = " << setprecision(6) << fixed
            << array[pos] << endl;
    }
    else
    {
        cout << "The searched value does not exist in the array\n";
    }
    //Free memory
    delete[] array;
    
    return 0;
}

