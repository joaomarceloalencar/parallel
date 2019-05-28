from random import randint
from dictutil import *

## Task 1
def movie_review(name):
    review_options = ["See it!", "A gem!", "Ideological claptrap!"]
    return review_options[randint(0,2)]

## Tasks 2 and 3 are in dictutil.py

## Task 4    
def makeInverseIndex(strlist):
	dict = {}
	for enum in list(enumerate(strlist)):
		fileNumber = enum[0]
		file = enum[1]
		for word in file.split():
			if (word not in dict):
				dict[word]={fileNumber}
			else:
				dict[word].add(fileNumber)
	return dict

## Task 5
def orSearch(inverseIndex, query):
	files = set()
	for word in query:
		files.update(inverseIndex[word])
	return files

## Task 6
def andSearch(inverseIndex, query):
	inter = inverseIndex[query[0]]
	for i in range(1,len(query)):
		word = query[i]
		inter = inter & inverseIndex[word]
	return inter 
