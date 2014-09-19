import math
# this is the x position
pos[0]=pos[0]+dir[0]*speed[0]
# this is the y position
pos[1]=pos[1]+dir[1]*speed[1]*6
# this is the z position
pos[2]=pos[2]+dir[2]*speed[2]
# now do some bounds checking
if pos[1] > 8.0 :
  speed[1] = -speed[1]
if pos[1] < -8.0 :
  speed[1] = -speed[1]
