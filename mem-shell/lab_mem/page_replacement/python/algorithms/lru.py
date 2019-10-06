# This is the file where you must implement the LRU algorithm

# This file will be imported from the main code. The PhysicalMemory class
# will be instantiated with the algorithm received from the input. You may edit
# this file as you wish

# NOTE: there may be methods you don't need to modify, you must decide what
# you need...

class LRU:

  def __init__(self):
    from collections import deque
    self.deque = deque()

  def put(self, frameId):
    self.deque.appendleft(frameId)

  def evict(self):
    return self.deque.pop()

  def clock(self):
    pass

  def access(self, frameId, isWrite):
    self.deque.remove(frameId)
    self.deque.appendleft(frameId)
