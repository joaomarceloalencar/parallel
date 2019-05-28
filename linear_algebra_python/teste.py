def makeInverseIndex(strlist):
	dict = {}
	for enum in list(enumerate(strlist)):
		fileNumber = enum[0]
		file = open(enum[1])
		lines = list(file)
		for line in lines:
			for word in line.split():
				if (word not in dict):
					dict[word]={fileNumber}
				else:
					dict[word].add(fileNumber)
	return dict
	
def orSearch(inverseIndex, query):
	files = set()
	for word in query:
		files.update(inverseIndex[word])
	return files
	
def andSearch(inverseIndex, query):
	inter = inverseIndex[query[0]]
	for i in range(1,len(query)):
		word = query[i]
		inter = inter & inverseIndex[word]
	return inter