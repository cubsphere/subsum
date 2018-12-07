#include <vector>
#include <math.h>
#include <string.h>
#include <boost/dynamic_bitset.hpp>

using namespace std;
using namespace boost;

const double EPSILON = ((double) 1) / ((double) (1<<16));

long cumulative_sum(long x)
{
    return (x * (x + 1)) / 2;
}

long poscalc(long setsize, long y)
{
    return cumulative_sum(setsize) - cumulative_sum(setsize - y);
}

double Yaccess(double *Y, long setsize, long y, long x)
{
    return Y[poscalc(setsize, y) + x];
}

void Ywrite(double *Y, long setsize, long y, long x, double val)
{
    Y[poscalc(setsize, y) + x] = val;
}

void computeY(double *Y, double *set, long setsize)
{
    memcpy(Y, set, setsize * sizeof(double));
    for (long i = 1; i < setsize; ++i)
    {
        for (long j = 0; j < setsize - i; ++j)
        {
            double sum = Yaccess(Y, setsize, i - 1, j);
            sum += Yaccess(Y, setsize, 0, j + 1);
            Ywrite(Y, setsize, i, j, sum);
        }
    }
}

bool double_compare(double a, double b)
{
    return fabs(a - b) < EPSILON;
}

bool out_of_bounds(double sum, double *Y, long setsize, long subsetsize, long depth, long card, double target)
{
    long row = subsetsize - card - 1;
    double lower_bound = sum + Yaccess(Y, setsize, row, depth);
    double upper_bound = sum + Yaccess(Y, setsize, row, setsize - row - 1);
    return (target + EPSILON < lower_bound) | (target - EPSILON > upper_bound);
}

static const bool LEFT = true;
static const bool RIGHT = false;

long subsum(double *set, long setsize, long subsetsize, double target, vector<unsigned long> *solutions)
{
    long Ysize = cumulative_sum(setsize);
    double *Y = new double[Ysize];
    computeY(Y, set, setsize);

    long leaves = 0;
    dynamic_bitset<> status(setsize+1);
    long depth = 0;
    double sum = 0;
    long card = 0;

    status.set();
    status[setsize] = RIGHT;
    bool go_left = true;

    while (1)
    {
        if (go_left)
        {
            ++depth;
            status[setsize - depth] = LEFT;
        }
        else
        {
            sum += set[depth];
            ++depth;
            ++card;
            status[setsize - depth] = RIGHT;
        }
        string s;
        to_string(status, s);

        if (double_compare(sum, target) & card == subsetsize)
        {
            status.flip();
            solutions->push_back(status.to_ulong());
            status.flip();
            /*//solution checker
            long l = 0;
            for(long i = 0; i < setsize; ++i)
            {
                if(!((bool) status[setsize - 1 - i]))
                {
                    l += set[i];
                }
            }
            l += 1;
            */
        }

        if (depth == setsize)
        {
            ++leaves;
            long return_to = status.find_first();
            if(return_to == -1)
                break;
            long newdepth = setsize - return_to;
            for(long i = newdepth; i < depth; ++i)
            {
                sum -= set[i];
            }
            card -= depth - newdepth;
            depth = newdepth - 1;
            go_left = false;
        } else if (card == subsetsize || out_of_bounds(sum, Y, setsize, subsetsize, depth, card, target))
        {
            ++leaves;
            long return_to = status.find_next(setsize - depth - 1);
            if(return_to == -1)
                break;
            long newdepth = setsize - return_to;
            for(long i = newdepth; i < depth; ++i)
            {
                sum -= set[i];
            }
            card -= depth - newdepth;
            depth = newdepth - 1;
            go_left = false;
        }
        else
        {
            go_left = true;
        }
    }
    return leaves;
}