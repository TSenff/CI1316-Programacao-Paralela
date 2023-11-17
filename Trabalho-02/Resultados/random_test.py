import os
import sys

from random import seed
from random import random

file_name = "random.txt"

seed(1)

if os.path.exists(file_name) :
    exit(1)

f = open(file_name,"w")
f.write(str(sys.argv[1]))
f.write("\n")

for x in range(0 , sys.argv[1]):
    f.write(str(random().randrange(1,300)))
    f.write("\n")
    f.write(str(random().randrange(1,300)))

# os.remove(file_name)

f.close
