#!/usr/bin/python
import random
import sys

def monte_carlo_pi(radius, npoints):
   circle_count = 0
   for i in range(npoints):
      x_coordinate = random.random()
      y_coordinate = random.random()
      if x_coordinate * x_coordinate + y_coordinate * y_coordinate < radius * radius:
         circle_count = circle_count + 1
   return circle_count

radius = float(sys.argv[1]) 
npoints = int(sys.argv[2])

PI = 4.0 * monte_carlo_pi(radius, npoints) / npoints
print "PI: " +  str(PI)
