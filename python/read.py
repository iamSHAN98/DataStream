#!/usr/bin/python3

"""
	Estimate  Pi by means of rejection sampling. Here,
	the set of random number pairs generated by 'write'
	is used

	(Output is similar to that of 'read' C++ macro).
"""

import sys
import math
import h5py
import datastream as ds

# File from command line
file = h5py.File(sys.argv[1], 'r')

dset = ds.get_data(file, "Group/Data")
N = ds.get_event(file, "Group/Data")
gen = ds.get_attribute(file, "Group", "Generator")

area = dset['x']**2 + dset['y']**2
Pi = len(area[area <= 1.])

print("Total event :", N)
print("Random number generator :", gen)
print("Pi : %.5f" % math.pi)
print("Pi (estimated) :", 4.*Pi/N)