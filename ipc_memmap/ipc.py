#!/usr/bin/python3

import mmap, time

with open('/tmp/a', 'rw+b') as f:
   mm = mmap.mmap(f.fileno(), 0)
   while True:
      time.sleep(1)
      mm[0] = '0'
      time.sleep(2)
      mm[0] = '1'
#class Memory():
#    def __init__()
