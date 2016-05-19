#!/usr/bin/python
import random
import sys
import threading

class Resource(object):
   def __init__(self):
      self.n = 0
      self.lock = threading.Lock()
   def increment(self):
      with self.lock:
         self.n += 1
   def __str__(self):
      return "n: " + str(self.n)

class Counter(threading.Thread):
   def __init__(self, resource, ni):
      threading.Thread.__init__(self)
      self.resource = resource
      self.ni = ni
   def run(self):
      for i in range(self.ni):
         self.resource.increment()

n = int(sys.argv[1])
r = Resource()

thread1 = Counter(r, n)
thread2 = Counter(r, n)

thread1.start()
thread2.start()

thread1.join()
thread2.join()

print r
