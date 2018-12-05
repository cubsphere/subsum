#include <vector>
#include <string.h>
#include <boost/dynamic_bitset.hpp>

using namespace std;
using namespace boost;

long cumulative_sum(long x)
{
    return (x * (x + 1)) / 2;
}

long poscalc(long setsize, long y)
{
    return cumulative_sum(setsize) - cumulative_sum(setsize - y);
}

long Yaccess(long *Y, long setsize, long y, long x)
{
    return Y[poscalc(setsize, y) + x];
}

void Ywrite(long *Y, long setsize, long y, long x, long val)
{
    Y[poscalc(setsize, y) + x] = val;
}

void computeY(long *Y, long *set, long setsize)
{
    memcpy(Y, set, setsize * sizeof(long));
    for (long i = 1; i < setsize; ++i)
    {
        for (long j = 0; j < setsize - i; ++j)
        {
            long sum = Yaccess(Y, setsize, i - 1, j);
            sum += Yaccess(Y, setsize, 0, j + 1);
            Ywrite(Y, setsize, i, j, sum);
        }
    }
}

bool out_of_bounds(long sum, long *Y, long setsize, long subsetsize, long depth, long card, long target)
{
    long row = subsetsize - card - 1;
    long lower_bound = sum + Yaccess(Y, setsize, row, depth);
    long upper_bound = sum + Yaccess(Y, setsize, row, setsize - row - 1);
    return target < lower_bound | target > upper_bound;
}

static const bool LEFT = true;
static const bool RIGHT = false;

long subsum(long *set, long setsize, long subsetsize, long target, vector<unsigned long> *solutions)
{
    long Ysize = cumulative_sum(setsize);
    long *Y = new long[Ysize];
    computeY(Y, set, setsize);

    long leaves = 0;
    dynamic_bitset<> status(setsize+1);
    long depth = 0;
    long sum = 0;
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

        if (sum == target & card == subsetsize)
        {
            status.flip();
            solutions->push_back(status.to_ulong());
            status.flip();
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