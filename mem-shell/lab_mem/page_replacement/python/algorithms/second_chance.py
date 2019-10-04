# This is the file where you must implement the Second Chance algorithm

# This file will be imported from the main code. The PhysicalMemory class
# will be instantiated with the algorithm received from the input. You may edit
# this file as you wish

# NOTE: there may be methods you don't need to modify, you must decide what
# you need...

class SecondChance:

  def __init__(self):
    self.list = []
    self.secondChance = []
    pass

  def put(self, frameId):
    self.list.append(frameId)
    self.secondChance.append(True)

  def evict(self):
    found = False
    while not found:
      for i in xrange(len(self.secondChance)):
        if self.secondChance[i] == False:
          self.secondChance.pop(i)
          evicted = self.list.pop(i)
          found = True
          break
        else: self.secondChance[i] = False
 
    for i in xrange(len(self.secondChance)):
      self.secondChance[i] = False
    return evicted

  def clock(self):
    pass

  def access(self, frameId, isWrite):
    self.secondChance[self.list.index(frameId)] = True
