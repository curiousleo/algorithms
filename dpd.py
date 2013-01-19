from collections import deque
from random import uniform
from math import floor

"""
Implementation of Vose's Alias Method.

The original algorithm is from [1]. It uses two (array, pointer) tuples
as a poor man's stacks, which complicates things. This program uses
deques instead. For a good explanation, see [2]. The algorithm described
there is not entirely correct -- step 4 of the initialisation should add
i, not p_i, to Small or Large, respectively.

[1]: Michael Vose: A Linear Algorithm For Generating Random Numbers With
     A Given Distribution. IEEE Transactions on Software Engineering,
     Vol. 17, No. 9, September 1991
[2]: Keith Schwarz: Darts, Dice, and Coins.
     http://www.keithschwarz.com/darts-dice-coins/

"""

def init(pdict):
    """
    Takes a dictionary that describers a discrete probability
    distribution with identifiers as keys and their respective
    probabilities as values. Returns a generator that generates keys
    according this probability distribution.
    """
    p = list(pdict.values())
    names = list(pdict.keys())
    n = len(p)

    alias = [None for i in range(n)]
    prob = [None for i in range(n)]
    small = deque()
    large = deque()

    for j in range(n):
        if p[j] > 1/n:
            large.append(j)
        else:
            small.append(j)

    while len(small) != 0 and len(large) != 0:
        j = small.pop()
        k = large.pop()
        prob[j] = n*p[j]
        alias[j] = k
        p[k] += p[j] - 1/n
        if p[k] > 1/n:
            large.append(k)
        else:
            small.append(k)

    while len(small) != 0:
        prob[small.pop()] = 1
    while len(large) != 0:
        prob[large.pop()] = 1

    def rand():
        u = uniform(0, n)
        j = floor(u)
        if (u-j) <= prob[j]:
            return j
        else:
            return alias[j]

    def wrap():
        while True: yield names[rand()]

    return wrap()

if __name__ == "__main__":
    it = 1000000
    pdict = {"orange": 1/8, "yellow": 1/5, "green": 1/10, "cyan": 1/4,
             "grey": 1/10, "blue": 1/10, "purple": 1/8}
    rdict = {name: 0 for name in pdict.keys()}
    rand = init(pdict)

    print("Key" + "\t" + "P" + "\t" + "P generated")
    for i in range(it):
        rdict[next(rand)] += 1
    for (k, v) in rdict.items():
        print(k + "\t" + str(pdict[k]) + "\t" + str(v/it))
