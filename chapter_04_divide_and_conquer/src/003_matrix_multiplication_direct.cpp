/**
  Matrix multiplication: direct method.

  Complexity: n^3

Author: Sandeep Palakkal
Email: sandeep.dion@gmail.com
Created on: 22-Aug-2016
*/

#include <iostream>
#include <string>
#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <ctime>

using namespace std;

typedef float Dtype;


/**
  Direct method for matrix multiplication
  */
bool matrix_multiply_direct(const Dtype *array1, int rows1, int cols1, Dtype *array2,
        int rows2, int cols2, Dtype **array3)
{
    //If any matrix is empty, go back
    if(rows1 == 0 || cols1 == 0 || rows2 == 0 || cols2 == 0)
    {
        cout << "Error: I cannot multiply empty matrix.\n";
        return false;
    }

    //Check compatibility of matrix sizes
    if(cols1 != rows2)
    {
        cout << "Error: Matrix size mismatch. I cannot perform multiplication.\n";
        return false;
    }

   //Multiply
    if(*array3 != nullptr)
    {
        delete[] *array3;
    }
    *array3 = new Dtype[rows1 * cols2];
    
    for(int i = 0; i < rows1; ++i)
    {
        for(int j = 0; j < cols2; ++j)
        {
            Dtype *sum = &(*array3)[i * cols2 + j];
            *sum = 0;
            for(int k = 0; k < cols1; ++k)
            {
                *sum += array1[i * cols1 + k] * array2[k * cols2 + j];
            }
        }
    }
    return true;
}


/**
  Read matrix from terminal
  */
bool read_matrix_term(Dtype **array, int &row, int &col)
{
    cout << "Enter the size of the matrix (as row col): ";
    cin >> row >> col;
    if(row <= 0 || col <= 0) return true;
    *array = new Dtype[row * col];
    cout << "Enter the matrix elements row-wise: ";
    for(int i = 0; i < row; ++i)
    {
        for(int j =0; j < col; ++j)
        {
            cin >> (*array)[i * col + j];
        }
    }
   return true;
}


/**
  Read array from file
  */
bool read_matrix_file(const string filename, Dtype **array, int &row, int &col)
{
    ifstream fp {filename};
    cout << "Reading input file... ";
    if(!fp.is_open())
    {
        cout << "Error: Input file could not be opened\n";
        return false;
    }
    fp >> row >> col;
    if(row <= 0 || col <= 0) return true;
    *array = new Dtype[row * col];
    for(int i = 0; i < row; ++i)
    {
        for(int j =0; j < col; ++j)
        {
            fp >> (*array)[i * col + j];
        }
    }
    fp.close();
    cout << "Read " << row << "X" << col << " matrix." << endl;
    return true;
}


/**
  Write array to terminal
  */
bool write_matrix_term(const Dtype *array, const int row, const int col)
{
    if(array == nullptr) 
    {
        cout << "Error: I cannot write an uninitialized matrix\n";
        return false;
    }
    cout << fixed;
    for(int i = 0; i < row; ++i)
    {
        for(int j = 0; j < col; ++j)
        {
            cout << array[i * row + j] << "\t";
        }
        cout << endl;
    }
    return true;
}


/**
  Write array to file
  */
bool write_matrix_file(const string filename, const Dtype *array, 
        const int row, const int col)
{
    ofstream ofp{filename};
    if(!ofp.is_open())
    {
        cout << "Error: Could not open file for writing.\n";
        return false;
    }
    ofp << fixed;
    for(int i = 0; i < row; ++i)
    {
        for(int j = 0; j < col; ++j)
        {
            ofp << array[i * row + j] << endl;
        }
    }
    ofp.close();
    cout << "Written matrix of size " << row << "X" << col << " in ";
    cout << filename << "." << endl;
    return true;
}


/**
  Prints usage
  */
void usage(const string command)
{
    cout << "Matrix multiplication (C = A X B). Usage:\n\n";
    cout << "1. " << command << "\n   No input arguments. Reads all inputs from"\
        " terminal.\n\n";
    cout << "2. " << command << " -a <input file 1>" << "-b <input file 2> ";
    cout << "-o <output file>" << endl; 
    cout << "   Reads input matrices from given input files. First two"\
        " elements in the file must be size (i.e., row col) of the matrix.\n";
}


/**
  Parse and get all inputs
  */
bool parse_and_get_inputs(const int argc, char **argv, Dtype **array1, 
        int &row1, int &col1, Dtype **array2, int &row2, int &col2,
        string &ofilename)
{
    if(argc % 2 == 0)
    {
        usage(string(argv[0]));
        return false;
    }
    *array1 = nullptr;
    *array2 = nullptr;
    for(int i = 1; i < argc; i += 2)
    {
        string arg_opt {argv[i]};
        if(arg_opt == "-h" || arg_opt == "--help")
        {
            usage(string(argv[0]));
            return false;
        }
        if(arg_opt == "-a")
        {
            if(!read_matrix_file(string(argv[i + 1]), array1, row1, col1))
            {
                return false;
            }
        }
        if(arg_opt == "-b")
        {
            if(!read_matrix_file(string(argv[i + 1]), array2, row2, col2))
            {
                return false;
            }
        }
        if(arg_opt == "-o")
        {
            ofilename = string(argv[i + 1]);
        }
    }
    if(*array1 == nullptr)
    {
        if(!read_matrix_term(array1, row1, col1))
        {
            return false;
        }
    }
    if(*array2 == nullptr)
    {
        if(!read_matrix_term(array2, row2, col2))
        {
            return false;
        }
    }
    return true;
}

int main(int argc, char **argv)
{
    Dtype *array1 = nullptr, *array2 = nullptr, *array3 = nullptr;
    int row1, col1, row2, col2, &row3 = row1, &col3 = col2;
    row1 = col1 = row2 = col2 = row3 = col3 = 0;
    string ofilename {""};

    
    //Parse and get inputs
    if(!parse_and_get_inputs(argc, argv, &array1, row1, col1, &array2, row2, col2, ofilename))
    {
        return 0;
    }

    //Find max subarray
    clock_t t0 = clock();
    matrix_multiply_direct(array1, row1, col1, array2, row2, col2, &array3);
    t0 = clock() - t0;
    cout << "Time taken for matrix multiplication: "\
        << float(t0)/CLOCKS_PER_SEC * 1000 << " ms.\n";

    //Print result
    if(ofilename.empty())
    {
        cout << "Result: " << endl;
        write_matrix_term(array3, row3, col3);
    }
    else
    {
        write_matrix_file(ofilename, array3, row3, col3);
    }
    //Free memory if any
    Dtype *array[] = {array1, array2, array3};
    for(int i = 0; i < (sizeof(array)/sizeof(Dtype *)); ++i)
    {
        if(array[i] != nullptr)
        {
            delete[] array[i];
        }
    }

    return 0;
}

