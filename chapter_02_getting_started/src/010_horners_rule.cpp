/**
  Horner's rule to evaluate polynomial.

Author: Sandeep Palakkal
Email: sandeep.dion@gmail.com
Date: 18-Aug-2016
*/

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cstddef>
#include <iomanip>

using namespace std;

//Input array type
typedef float Dtype;

//sort type
enum SORT_TYPE {ASCEND, DESCEND};


Dtype horners_rule(Dtype *array, int array_len, Dtype x)
{
    Dtype y = 0;
    for(int i = 0; i < array_len; ++i)
    {
        y = array[i] + x * y;
    }
    return y;
}


/**
  Read input array from terminal
  */
void read_array_term(Dtype **array, int &array_len)
{
    cout << "Enter number of coefficients: ";
    cin >> array_len;
    if(array_len <= 0) return;
    *array = new Dtype[array_len];
    cout << "Enter the coefficients: ";
    for(int i = 0; i < array_len; ++i)
    {
        cin >> (*array)[i];
    }
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
  Helper function
  */
void usage(char *exe_file)
{
    cout << "Usage:\n1. " << exe_file << " (No arguments. Reads "\
        "input array from terminal. Enter coefficients from higher order to"\
        " lower order (a_n, ..., a_0)." << endl;
    cout << "2. " << exe_file << " <input_file> (Input is read from"\
        " input_file. Coefficients must be saved from higher order to"\
        " lower order (a_n, ..., a_0). The first element should be the number"\
        " of elements in the file" << endl;
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

    Dtype *array = nullptr;
    int array_len;
    SORT_TYPE sort_type;

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
    
    //Get point at which polynomial has to be evaluated
    Dtype x;
    cout << "Enter value at which polynomial to be evaluated: ";
    cin >> x;
 
    //Call Horner's rule
    clock_t time = clock();
    Dtype y = horners_rule(array, array_len, x);
    time = clock() - time;
    cout << "Horner's rule took "<< 
        float(time) / CLOCKS_PER_SEC * 1000 << " ms." << endl;

    //Write the output to the terminal
    cout << "P(x) = " << y << endl;

    //Free memory
    if(array != nullptr) delete[] array;
    
    return 0;
}
