/**

Linear Search

Complexity: Theta(n)

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


/**
  Linear search
  */
int linear_search(Dtype *array, int array_len, Dtype value)
{
    int index = -1;
    for(int i = 0; i < array_len; ++i)
    {
        if(array[i] == value)
        {
            index = i;
            break;
        }
    }
    return index;
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
    int pos = linear_search(array, array_len, value);
    time = clock() - time;
    cout << "Linear search took "<< 
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

