def getitem(v,d):
    assert d in v.D
    if d not in v.f.keys() :
        return 0
    else :
        return v.f[d]

def setitem(v,d,val):
    assert d in v.D
    v.f[d] = val
    return 		

def equal(u,v):
    assert u.D == v.D
    for i in set(u.D).union(set(v.D)):
        if getitem(u,i) != getitem(v,i):
            return False
    return True        	

def add(u,v):
    assert u.D == v.D
    sum = Vec(u.D, {})
    for i in set(u.D).union(set(v.D)):
        a = getitem(u,i)
        b = getitem(v,i)
        setitem(sum,i,a+b)
    return sum	    
	
def dot(u,v):
    assert u.D == v.D
    dot = 0
    for i in set(u.D).union(set(v.D)):
        dot += getitem(u,i) * getitem(v,i)
    return dot

def scalar_mul(v, alpha):
    scalar = Vec(v.D, {})
    for i in v.f.keys():
        setitem(scalar, i, alpha * getitem(v,i))
    return scalar    	 

def neg(v):
	return scalar_mul(v, -1)

##### NO NEED TO MODIFY BELOW HERE #####
class Vec:
    """
    A vector has two fields:
    D - the domain (a set)
    f - a dictionary mapping (some) domain elements to field elements
        elements of D not appearing in f are implicitly mapped to zero
    """
    def __init__(self, labels, function):
        self.D = labels
        self.f = function

    __getitem__ = getitem
    __setitem__ = setitem
    __neg__ = neg
    __rmul__ = scalar_mul #if left arg of * is primitive, assume it's a scalar

    def __mul__(self,other):
        #If other is a vector, returns the dot product of self and other
        if isinstance(other, Vec):
            return dot(self,other)
        else:
            return NotImplemented  #  Will cause other.__rmul__(self) to be invoked

    def __truediv__(self,other):  # Scalar division
        return (1/other)*self

    __add__ = add

    def __radd__(self, other):
        "Hack to allow sum(...) to work with vectors"
        if other == 0:
            return self
    
    def __sub__(a,b):
         "Returns a vector which is the difference of a and b."
         return a+(-b)

    __eq__ = equal

    def __str__(v):
        "pretty-printing"
        try:
            D_list = sorted(v.D)
        except TypeError:
            D_list = sorted(v.D, key=hash)
        numdec = 3
        wd = dict([(k,(1+max(len(str(k)), len('{0:.{1}G}'.format(v[k], numdec))))) if isinstance(v[k], int) or isinstance(v[k], float) else (k,(1+max(len(str(k)), len(str(v[k]))))) for k in D_list])
        # w = 1+max([len(str(k)) for k in D_list]+[len('{0:.{1}G}'.format(value,numdec)) for value in v.f.values()])
        s1 = ''.join(['{0:>{1}}'.format(k,wd[k]) for k in D_list])
        s2 = ''.join(['{0:>{1}.{2}G}'.format(v[k],wd[k],numdec) if isinstance(v[k], int) or isinstance(v[k], float) else '{0:>{1}}'.format(v[k], wd[k]) for k in D_list])
        return "\n" + s1 + "\n" + '-'*sum(wd.values()) +"\n" + s2

    def __repr__(self):
        return "Vec(" + str(self.D) + "," + str(self.f) + ")"

    def copy(self):
        "Don't make a new copy of the domain D"
        return Vec(self.D, self.f.copy())
