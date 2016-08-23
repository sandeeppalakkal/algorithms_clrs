/**
  Given an array of integers of length and a number x. Write a Theta(n log n)
  algorithm that checks if two numbers in the array add to x.

Author: Sandeep Palakkal
Email: sandeep.dion@gmail.com
Created: 14-Aug-2016
*/


#include <iostream>
#include <cstddef>
#include <string>
#include <iomanip>
#include <fstream>
#include <ctime>

using namespace std;

typedef int Dtype;

/**
  Binary search algorithm
  */
int binary_search(Dtype x, Dtype *array, int beg, int end)
{
    if(end == beg) return -1;
    if(end - beg == 1)
    {
        if(array[beg] == x)
        {
            return beg;
        }
        return -1;
    }
    int mid = (beg + end) / 2;
    int j;
    
    if(array[mid] > x)
    {
        j = binary_search(x, array, beg, mid);
    }
    else
    {
        j = binary_search(x, array, mid, end);
    }
    return j;
}

/**
  Part of merge sort algorithm. Merges to arrays according the the sorting
  order.*/
void merge(Dtype *array1, Dtype *array2, int beg, int mid, int end)
{
    int i = beg, j = mid, k = beg;
    while(k < end)
    {
        if(array1[i] > array1[j])
        {
            array2[k++] = array1[j++];
        }
        else
        {
            array2[k++] = array1[i++];
        }
        if(i == mid || j == end)
        {
            break;
        }
    }
    while(i < mid)
    {
        array2[k++] = array1[i++];
    }
    while(j < end)
    {
        array2[k++] = array1[j++];
    }
    for(i = beg; i < end; ++i)
    {
        array1[i] = array2[i];
    }
}

/**
  Merge sort algorithm recursive implementation
  */
void merge_sort(Dtype *array1, Dtype *array2, int beg, int end)
{
    if(end - beg <= 1)
    {
        return;
    }
    int mid = (beg + end) / 2;
    merge_sort(array1, array2, beg, mid);
    merge_sort(array1, array2, mid, end);
    merge(array1, array2, beg, mid, end);
}

/**
  Uses merge sort to sort array1 and the result is given in array2
  */
void merge_sort(Dtype *array1, Dtype *array2, int array_len)
{
    for(int i = 0; i < array_len; ++i)
    {
        array2[i] = array1[i];
    }
    Dtype *array3 = new Dtype [array_len];
    merge_sort(array2, array3, 0, array_len);
    delete array3;
}

/**
  Find min and max in the array
  */
void findrange(Dtype *array, int array_len, int &min, int &max)
{
    min = max = array[0];
    for(int i = 1; i < array_len; ++i)
    {
        if(array[i] < min) min = array[i];
        if(array[i] > max) max = array[i];
    }
}


/**
  Solve the problem (method 1)
  Sort and binary search
  Complexity: n log n
  */
void solve1(Dtype *array, int array_len, Dtype x)
{
    //Sort the elements and create new array
    Dtype *array_srt = new Dtype [array_len];
    merge_sort(array, array_srt, array_len);

    //For each element p in the sorted array, search for q = x - p in the sorted
    //array. If a match is found, the problem is solved.
    int j = -1, i = 0;
    for(i = 0; i < array_len - 1; ++i)
    {
        j = binary_search(x - array_srt[i], array_srt, i + 1, array_len);
        if(j != -1) break;
    }

    if(j != -1)
    {
        cout << x << " can be factored as " << array_srt[i] << " + " <<
            array_srt[j] << endl;
    }
    else
    {
        cout << x << " cannot be written as sums of any two numbers in the "\
            "given array." << endl;
    }

    //Delete memory
    if(array_srt != nullptr)
    {
        delete array_srt;
    }
}

/**
  Solve the problem (method 2)
  Sort and search in both direction (complexity n)
  Complexity: n log n
*/
void solve2(Dtype *array, int array_len, Dtype x)
{
    //Sort the elements and create new array
    Dtype *array_sort = new Dtype [array_len];
    merge_sort(array, array_sort, array_len);
    
    //Start from beginning and end of the sorted array simultaneously and find
    //if sum of the first and last element is present. Move towards the middle.
    int i = 0, j = array_len - 1;
    bool success = false;
    while(i < j)
    {
        Dtype sum = array_sort[i] + array_sort[j];
        if( sum < x) ++i;
        else if(sum > x) --j;
        else
        {
            cout << x << " can be factored as " << array_sort[i] << " + " <<
                array_sort[j] << endl;
            success = true;
            break;
        }
    }
    if(!success)
    {
        cout << x << " cannot be written as sums of any two numbers in the"\
            "given array." << endl;
    }
    
    //Delete memory
    if(array_sort != nullptr)
    {
        delete array_sort;
    }
}

/**
  Solve problem (Method 3)
  Uses hashing
  Complexity: n
  Memory requirement: max - min
*/
void solve3(Dtype *array, int array_len, Dtype x)
{
    if(array_len == 0)
    {
        cout << "The given array is empty. I cannot process.\n";
        return;
    }
    int min, max, range;
    findrange(array, array_len, min, max);
    range = max - min + 1;
    bool *table;
    if(!(table = new bool [range]{}))
    {
        cout << "Solve3: Out of memory\n";
        return;
    }
   
    //Hash and search
    bool success = false;
    for(int i = 0; i < array_len; ++i)
    {
        int diff = x - array[i];
        if(diff >= min && diff <= max && table[diff - min])
        {
            cout << x << " can be factored as " << array[i] << " + " <<
                diff << endl;
            success = true;
            break;
        }
        table[array[i] - min] = true;
    }
    if(!success)
    {
        cout << x << " cannot be written as sums of any two numbers in the "\
            "given array." << endl;
    }

    if(table != nullptr)
    {
        delete table;
    }
}

// Function to read array from terminal
bool read_array_term(Dtype **array, int &array_len)
{
    cout << "Enter input array size: ";
    cin >> array_len;
    if(array_len <= 0) return true;
    *array = new Dtype[array_len];
    cout << "Enter input array elements: ";
    for(int i = 0; i < array_len; ++i)
    {
        cin >> (*array)[i];
    }
    return true;
}

//Function to read array from file
bool read_array_file(char *file_name, Dtype **array, int &array_len)
{
    ifstream fp {file_name};
    if(!fp.is_open())
    {
        cout << "Unable to read from the given file\n";
        return false;
    }
    fp >> array_len;
    if(array_len <= 0) return true;
    *array = new Dtype[array_len];
    for(int i = 0; i < array_len; ++i)
    {
        fp >> (*array)[i];
    }
    return true;
}


//Usage
void usage(char *exe_name)
{
    cout << "Usage: " << endl;
    cout << "1. " << exe_name << " (No input arguments. Reads input array from"\
        " terminal)\n";
    cout << "2. " << exe_name << " <input file> (Reads input array from"\
        " file)\n";
}


//Parse and get input
bool parse_input(int argc, char **argv, Dtype **array, int &array_len, Dtype &x)
{
    bool status;
    //Parse input
    if(argc > 2 || (argc == 2 && (string(argv[1]) == "-h" || string(argv[1]) ==
                    "--help")))
    {
        //Help function
        usage(argv[0]);
        status = false;
    }
    else if(argc == 1)
    {
        //Read from terminal
        status = read_array_term(array, array_len);
        
    }
    else 
    {
        //Read from file
        status = read_array_file(argv[1], array, array_len);
    }
    if(status)
    {
        cout << "Enter the number to factor: ";
        cin >> x;
    }
    return status;
}


//Main
int main(int argc, char **argv)
{
    int array_len;
    Dtype *array = nullptr;
    Dtype x;
    
    //Parse arguments and read input array
    if(!parse_input(argc, argv, &array, array_len, x))
    {
        return 0;
    }

    //Call the algorithm
    cout << "There are 3 solutions. Enter a binary number to decide which all"\
        " to use (0 - 7): ";
    unsigned int choice;
    cin >> choice;

    clock_t t0;
    if(choice & (1 << 2))
    {
        cout << "\nSolution 1 (complexity = n log n):\n";
        t0 = clock();
        solve1(array, array_len, x);
        cout << "Time taken for solving the problem is " <<
            float(clock() - t0)/CLOCKS_PER_SEC*1000 << " ms\n";
    }
    
    if(choice & (1 << 1))
    {
        cout << "\nSolution 2(complexity = n log n):\n";
        t0 = clock();
        solve2(array, array_len, x);
        cout << "Time taken by second method for solving the problem is " <<
            float(clock() - t0)/CLOCKS_PER_SEC*1000 << " ms\n";
    }

    if(choice & (1 << 0))
    {
        cout << "\nSolution 3(complexity = n):\n";
        t0 = clock();
        solve3(array, array_len, x);
        cout << "Time taken by third method for solving the problem is " <<
            float(clock() - t0)/CLOCKS_PER_SEC*1000 << " ms\n";
        cout << endl;
    }
    
    //Free memory
    if(array != nullptr)
    {
        delete[] array;
    }
    
    return 0;
}

