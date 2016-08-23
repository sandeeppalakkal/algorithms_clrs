/**
  Count inversions using merge sort

Complexity: n log n

Author:Sandeep Palakkal
Email: sandeep.dion@gmail.com
Created on: 20-Aug-2016
  */

#include <iostream>
#include <cstddef>
#include <string>
#include <ctime>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <cctype>

using namespace std;

typedef float Dtype;
enum SORT_TYPE {ASCEND, DESCEND, INVALID};

int k;


/**
  Compare function. If return is true, that means x should come after y while
  sorting. If false, y should come after x.
  */
bool compare(Dtype x, Dtype y, SORT_TYPE sort_type)
{
    bool grt = x > y;
    switch(sort_type)
    {
        case DESCEND:
            grt = !grt;
    }
    return grt;
}

/**
  Find correct position to insert element according to the sort order.
  Part of insertion sort.
  */
int find_place(Dtype *array, int beg, int end, SORT_TYPE sort_type)
{
    Dtype elem = array[end];
    int i = end - 1;
    while(i >= beg && compare(array[i], elem, sort_type))
    {
        --i;
    }
    return i + 1;
}


/**
  Insertion sort with binary search
  */
int insertion_sort(Dtype *array, int beg, int end, SORT_TYPE sort_type)
{
    int n_inv = 0;
    for(int i = beg + 1; i < end; ++i)
    {
        int j = find_place(array, beg, i, sort_type);
        if(j == -1)
        {
            cout << "Error: insertion failed\n";
            exit(1);
        }

        Dtype elem = array[i];
        for(int l = i; l > j; --l)
        {
            array[l] = array[l-1]; 
        }
        array[j] = elem;
        n_inv += i - j;
    }
    return n_inv;
}



/** 
  Merge (part of merge sort)
  */
int merge(Dtype *array1, Dtype *array2, int beg, int mid, int end, SORT_TYPE
        sort_type)
{
    int i, j, l, n_inv;
    i = l = beg;
    j = mid;
    n_inv = 0;
    while(i < mid && j < end)
    {
        if(compare(array1[i], array1[j], sort_type))
        {
            n_inv += mid - i;
            array2[l++] = array1[j++];
        }
        else
        {
            array2[l++] = array1[i++];
        }
    }
    while(i < mid) array2[l++] = array1[i++];
    while(j < end) array2[l++] = array1[j++];
    for(i = beg; i < end; ++i) array1[i] = array2[i];
    return n_inv;
}


/**
  Merge sort
  */
int merge_sort(Dtype *array1, Dtype *array2, int beg, int end, SORT_TYPE
        sort_type)
{
    int n_inv = 0;
    if(end - beg <= k)
    {
        if(k == 1) n_inv = 0;
        else n_inv = insertion_sort(array1, beg, end, sort_type);
    }
    else
    {
        int mid = (beg + end) / 2;
        n_inv = merge_sort(array1, array2, beg, mid, sort_type);
        n_inv += merge_sort(array1, array2, mid, end, sort_type);
        n_inv += merge(array1, array2, beg, mid, end, sort_type);
    }
    return n_inv;
}


/**
  Count inversions using merge sort
  */
int count_inversions(Dtype *array, int array_len, SORT_TYPE sort_type)
{
    k = 0;
    if(array_len != 0)
    {
        k = max(int(log(double(array_len))/log(2.0)) - 1, 1);
        //k = 1;
        cout << "Insertion sort called when n = " << k << endl;
    }
    Dtype *array2 = new Dtype[array_len];
    int n_inv = merge_sort(array, array2, 0, array_len, sort_type);
    delete[] array2;
    return n_inv;
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
bool read_array_file(string filename, Dtype **array, int &array_len)
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
  Set sort type
  */
bool set_sort_type(int sort_type_, SORT_TYPE &sort_type)
{
    switch(sort_type_)
    {
        case 1: 
            sort_type = ASCEND;
            break;
        case 2:
            sort_type = DESCEND;
            break;
        default:
            sort_type = INVALID;
            cout << "Error: Unkown sort type\n";
            return false;
    }
    return true;
}

/**
  Read sort type
  */
bool read_sort_type_term(SORT_TYPE &sort_type)
{
    cout << "Enter sort order: " << endl;
    cout << "1. ASCEND\n2. DESCEND\n(1/2): ";
    int sort_type_;
    cin >> sort_type_;
    return set_sort_type(sort_type_, sort_type);
}


/**
  Write array
  */
bool write_array_file(string filename, Dtype *array, int array_len)
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
void usage(string command)
{
    cout << "Inversion Count. Usage:\n\n";
    cout << "1. " << command << "\n   No input arguments. Reads all inputs from"\
        " terminal.\n\n";
    cout << "2. " << command << " -i <input file> -s <sort type> -o <output"\
        " file>" << endl;
        
    cout << "   Reads input array from input file. First element in the file"\
        " must be the length of the array.\n";
    cout << "   " << "Sort type (ascend (1) or descend (2)) may be given.\n";
    cout << "   " << "Output file may be given to store the sorted array.\n";
}


/**
  Parse and get all inputs
  */
bool parse_and_get_inputs(int argc, char **argv, Dtype **array, int &array_len,
        string &ofilename, SORT_TYPE &sort_type)
{
    if(argc % 2 == 0)
    {
        usage(string(argv[0]));
        return false;
    }
    sort_type = INVALID;
    *array = nullptr;
    ofilename = "";
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
        if(arg_opt == "-o" || arg_opt == "--output")
        {
            ofilename = string(argv[i + 1]);
        }
        if(arg_opt == "-s" || arg_opt == "--sort")
        {
            if(!set_sort_type(atoi(argv[i + 1]), sort_type))
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
    if(sort_type == INVALID)
    {
        if(!read_sort_type_term(sort_type))
        {
            return false;
        }
    } 
    return true;
}

int main(int argc, char **argv)
{
    Dtype *array = nullptr;
    int array_len;
    SORT_TYPE sort_type;
    string ofilename = "";
    
    //Parse and get inputs
    if(!parse_and_get_inputs(argc, argv, &array, array_len, ofilename, sort_type))
    {
        return 0;
    }

    //Find inversions
    clock_t t0 = clock();
    int n_inv = count_inversions(array, array_len, sort_type);
    t0 = clock() - t0;
    cout << "Time taken to count inversions: " 
        << float(t0)/CLOCKS_PER_SEC * 1000 << " ms.\n";

    //Print result
        cout << "No. of inversions = " << n_inv << endl;
        
    //Save the sorted array if required
    if(!ofilename.empty())
    {
        if(!write_array_file(ofilename, array, array_len))
        {
            cout << "Could not write the result to output file\n";
        }
    }

    //Free memory if any
    if(array != nullptr)
    {
        delete[] array;
    }

    return 0;
}
