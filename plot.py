import matplotlib.pyplot as plt
import numpy as np
import math


angle = [0, 30, 60, 90, 120, 150, 180]
r = [100, 120, 14, 15, 16, 12, 100]

xvals = [0] * 7
yvals = [0] * 7

# x = rcos(theta)
# y = rsin(theta)

# convert degrees to rads
for index in range(len(angle)):
    angle[index] = angle[index] * np.pi / 180
    xvals[index] = r[index] * math.cos(angle[index])
    yvals[index] = r[index] * math.sin(angle[index])
    print('ANGLE: ', angle[index])
    print('X: ', xvals[index])
    print('Y: ', yvals[index])


ax = plt.gca()  # gca stands for 'get current axis'
ax.spines['right'].set_color('none')
ax.spines['top'].set_color('none')
ax.xaxis.set_ticks_position('bottom')
ax.spines['bottom'].set_position(('data',0))
ax.yaxis.set_ticks_position('left')
ax.spines['left'].set_position(('data',0))

plt.plot(xvals, yvals)
plt.axis([-200, 200, 0, 200])

plt.show()
