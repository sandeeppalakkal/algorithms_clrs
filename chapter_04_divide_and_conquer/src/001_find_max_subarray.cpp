/**
  Find maximum subarray by divide and conquer approach.

  Complexity: n log n

Author: Sandeep Palakkal
Email: sandeep.dion@gmail.com
Created on: 20-Aug-2016
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
  Find the maximum subarray crossing the mid point given
  */
void find_max_subarray_crossing(const Dtype *array, const int left, const int
        mid, const int right, int &beg, int &end, Dtype &sum)
{
    //Find subarray in left subarray ending at the mid point
    Dtype cur_max = array[mid - 1];
    Dtype sum_so_far = cur_max;
    beg = mid - 1;
    for(int i = mid - 2; i >= left; --i)
    {
        sum_so_far += array[i];
        if(sum_so_far > cur_max)
        {
            cur_max = sum_so_far;
            beg = i;
        }
    }

    //Find subarray in right subarray starting at the mid point
    cur_max += array[mid];
    sum_so_far = cur_max;
    end = mid + 1;
    for(int i = mid + 1; i < right; ++i)
    {
        sum_so_far += array[i];
        if(sum_so_far > cur_max)
        {
            cur_max = sum_so_far;
            end = i + 1; //right bound is open
        }
    }
    sum = cur_max;
}


/**
  Find the maximum subarray
  */
void find_max_subarray(const Dtype *array, const int left, const int right, 
        int &beg, int &end, Dtype &sum)
{
    if(right == left + 1)
    {
        beg = left;
        end = right;
        sum = array[beg];
        return;
    }
   
    int mid = (left + right) / 2;
    
    //Find max subarray in left array
    int left_beg, left_end;
    Dtype left_sum;
    find_max_subarray(array, left, mid, left_beg, left_end, left_sum);

     //Find max subarray in right array
    int right_beg, right_end;
    Dtype right_sum;
    find_max_subarray(array, mid, right, right_beg, right_end, right_sum);
    
    //Find subarray that lines across left and right subarrays (crossing the mid
    //point)
    int center_beg, center_end;
    Dtype center_sum;
    find_max_subarray_crossing(array, left, mid, right, center_beg, center_end,
            center_sum);
    
    //Assign the final result
    if(left_sum >= right_sum && left_sum >= center_sum)
    {
        sum = left_sum;
        beg = left_beg;
        end = left_end;
    }
    else if(right_sum >= left_sum && right_sum >= center_sum)
    {
        sum = right_sum;
        beg = right_beg;
        end = right_end;
    }
    else
    {
        sum = center_sum;
        beg = center_beg;
        end = center_end;
    }
}


/**
  Find the maximum subarray: wrapper code
  */
void find_max_subarray(const Dtype *array, const int array_len, int &beg, 
        int &end, Dtype &sum)
{
    if(array_len > 0)
    {
        find_max_subarray(array, 0, array_len, beg, end, sum);
    }
    else
    {
        beg = end = 0;
        sum = 0;
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
    find_max_subarray(array, array_len, beg, end, sum);
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

