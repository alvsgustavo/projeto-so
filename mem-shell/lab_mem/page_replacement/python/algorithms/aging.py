# This is the file where you must implement the Aging algorithm

# This file will be imported from the main code. The PhysicalMemory class
# will be instantiated with the algorithm received from the input. You may edit
# this file as you whish

# NOTE: there may be methods you don't need to modify, you must decide what
# you need...

ALGORITHM_AGING_NBITS = 8
"""How many bits to use for the Aging algorithm"""

class Frame:
  def __init__(self, frameId):
    self.frameId = frameId
    self.age = 0
    self.referenced = 1
  
  def aging(self):
    self.age = (self.age >> 1) + self.referenced * 2**(ALGORITHM_AGING_NBITS-1)
    self.referenced = 0

class Aging:

  def __init__(self):
    self.list = []

  def put(self, frameId):
    self.list.append(Frame(frameId))

  def evict(self):
    minimumFrame = self.list[0]
    for i in xrange(len(self.list)):
      elem = self.list[i]
      minimumFrame = elem if elem.age < minimumFrame.age else minimumFrame
    self.list.remove(minimumFrame)
    return minimumFrame.frameId

  def clock(self):
    for frame in self.list:
      frame.aging()

  def access(self, frameId, isWrite):
    frame = [x for x in self.list if x.frameId == frameId][0]
    frame.referenced = 1
