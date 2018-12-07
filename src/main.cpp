#include <iostream>
#include <fstream>
#include <vector>
#include <subsum.hpp>

using namespace std;

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        cout << "specify input file path";
        return 1;
    }

    ifstream input_file(argv[1]);
    if (!input_file.is_open())
    {
        cout << "could not open " << argv[1] << "\nmake sure the file path is correct";
    }
    long len;
    input_file >> len;
    double *set = new double[len];
    for (long i = 0; i < len; ++i)
    {
        input_file >> set[i];
    }
    double target;
    input_file >> target;
    long k;
    input_file >> k;
    vector<unsigned long> solutions;
    long leaves = subsum(set, len, len - k, target, &solutions);

    cout << leaves << " leaves\n";
    if (solutions.empty())
        cout << "solutions: none";
    else
    {
        cout << "solutions:";
        for (auto i : solutions)
            cout << ' ' << i;
    }

    return 0;
}