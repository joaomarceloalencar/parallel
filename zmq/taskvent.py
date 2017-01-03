#!/usr/bin/python
# Task ventilator
# Binds PUSH socket to tcp://localhost:5557
# Sends batch of tasks to workers via that socket
#
# Author: Lev Givon <lev(at)columbia(dot)edu>

import zmq
import random
import time

context = zmq.Context()

# Socket to send messages on
sender = context.socket(zmq.PUSH)
sender.bind("tcp://*:5557")

# Socket with direct access to the sink: used to syncronize start of batch
sink = context.socket(zmq.PUSH)
sink.connect("tcp://localhost:5558")

print("Press Enter when the workers are ready: ")
_ = raw_input()
print("Sending tasks to workers...")

# The first message is "0" and signals start of batch
sink.send(b'0')

# Initialize random number generator

sender.send_string(u'0')
sender.send_string(u'1')
sender.send_string(u'2')
sender.send_string(u'3')

# Give 0MQ time to deliver
time.sleep(1)

