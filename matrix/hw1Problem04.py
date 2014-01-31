#!/usr/bin/python3.2
from GF2 import one
from GF2 import zero
from itertools import chain
from itertools import combinations

def powerset(iterable):
	s = list(iterable)
	return chain.from_iterable(combinations(s, r) for r in range(len(s)+1))

def GF2VectorSum (a, b):
	l = []
	if len(a) != len(b):
		return l
	l = list(range(len(a)))
	for i in range(len(a)):
		l[i] = a[i] + b[i]
	return l	

a = [one, one, one, zero, zero, zero, zero]
b = [zero, one, one, one, zero, zero, zero]
c = [zero, zero, one, one, one, zero, zero]
d = [zero, zero, zero, one, one, one, zero]
e = [zero, zero, zero, zero, one, one, one]
f = [zero, zero, zero, zero, zero, one, one]

vectors = {'a': a, 'b': b, 'c': c, 'd': d, 'e': e, 'f': f } 

u = [zero, zero, one, zero, zero, one, zero]
v = [zero, one, zero, zero, zero, one, zero]

#print(list(powerset(vectors.keys())))

for s in list(powerset(vectors.keys())):
	if len(s) < 2:
		continue;
	sum = []
	for key in s:
		if sum == [] :
			sum = vectors[key]
		else :
			sum = GF2VectorSum(sum, vectors[key])	
	if sum == u :
		print("Soma igual a u:")
		print(list(s))
	if sum == v :
		print("Soma igual a v:")
		print(list(s))
#print(GF2VectorSum(a,b))
