import random

# This is the file where you must implement the NRU algorithm

# This file will be imported from the main code. The PhysicalMemory class
# will be instantiated with the algorithm received from the input. You may edit
# this file as you wish

# NOTE: there may be methods you don't need to modify, you must decide what
# you need...

class Frame:
  def __init__(self, frameId):
    self.frameId = frameId
    self.ref = 1
    self.mod = 0

class NRU:

  def __init__(self):
    self.list = []

  def put(self, frameId):
    self.list.append(Frame(frameId))

  def evict(self):
    classes = { 0: [], 1: [], 2: [], 3:[]}
    for frame in self.list:
      classes.get(frame.ref*2 + frame.mod).append(frame) ##Every frame goes into their class list.
    for x in xrange(4):
      if len(classes.get(x)) > 0:
        deletedFrame = self.list.pop(self.list.index(random.choice(classes.get(x))))
        return deletedFrame.frameId
    
  def clock(self):
    for frame in self.list:
      frame.ref = 0

  def access(self, frameId, isWrite):
    elem = [frame for frame in self.list if frame.frameId == frameId][0]
    elem.ref = 1
    if isWrite and elem.mod == 0: 
      elem.mod = isWrite