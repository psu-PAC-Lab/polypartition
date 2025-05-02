import polypartition
import numpy as np
import matplotlib.pyplot as plt

bdy = np.array([[0., 0.],
                 [10., 0.],
                 [0., 10.],
                 [10., 10.]])

hole = np.array([[4., 4.],
                  [6., 4.],
                  [4., 6.],
                  [6., 6.]])

part = polypartition.hertel_mehlhorn(bdy, [hole])

fig, ax = plt.subplots()
ax.set_aspect('equal')
for poly in part:
    _poly = np.vstack((poly, poly[0,:]))
    ax.fill(_poly[:, 0], _poly[:, 1])
plt.show()