#!/usr/bin/python
import random
import sys
import threading

total_circle_count = 0
lock = threading.Lock()

class MonteCarloPi(threading.Thread):
   def __init__(self, radius, npoints):
      threading.Thread.__init__(self)
      self.radius = radius
      self.npoints = npoints
      self.circle_count = 0
   def run(self):
      global total_circle_count
      for i in range(self.npoints):
         x_coordinate = random.random()
         y_coordinate = random.random()
         if x_coordinate * x_coordinate + y_coordinate * y_coordinate < self.radius * self.radius:
            self.circle_count = self.circle_count + 1
      with lock:
         total_circle_count += self.circle_count

radius = 1.0 
npoints = int(sys.argv[1])

thread1 = MonteCarloPi(radius, npoints / 2)
thread2 = MonteCarloPi(radius, npoints / 2)

thread1.start()
thread2.start()

thread1.join()
thread2.join()

PI = 4.0 * total_circle_count / npoints
print "PI: " +  str(PI)
