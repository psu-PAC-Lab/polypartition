import polypartition
import numpy as np
import matplotlib.pyplot as plt
from shapely.geometry import Polygon


### functions
def random_poly(center, n_verts, min_radius, max_radius, rng=None):
    """Makes random polytope"""
    
    # generate random radii
    if rng is not None:
        radii = rng.uniform(min_radius, max_radius, n_verts)
    else:
        radii = np.random.uniform(min_radius, max_radius, n_verts)

    # generate random angles
    if rng is not None:
        angles = rng.uniform(0, 2 * np.pi, n_verts)
    else:
        angles = np.random.uniform(0, 2 * np.pi, n_verts)

    angles = np.sort(angles)

    # generate random vertices
    V = np.zeros((n_verts, 2))
    for i in range(n_verts):
        x = radii[i] * np.cos(angles[i]) + center[0]
        y = radii[i] * np.sin(angles[i]) + center[1]
        V[i] = [x, y]

    # require convexity
    P = Polygon(V)
    P_ch = P.convex_hull

    if P.equals(P_ch): # i.e., is convex
        return P
    else:
        return random_poly(center, n_verts, min_radius, max_radius, rng=rng)
    
def shapely_poly_2_vrep(P):
    """Convert a Shapely Polygon to a list of vertices.
    P : shapely Polygon"""
    
    # get vertices
    V = np.array(P.exterior.coords.xy).transpose()
    V = V[0:-1,:] # remove duplicate vertex

    return V


### make map

# map parameters
min_radius = 1.0
max_radius = 2.0
x_min = 0.
x_max = 10.
y_min = -5.
y_max = 5.
n_obs = 3
n_sides = 5
dw = (max_radius+min_radius)/2.

# boundary
Vbdy = np.array([[x_min, y_min], [x_max, y_min], [x_max, y_max], [x_min, y_max]])
Pbdy = Polygon(Vbdy)

# random obstacles
seed = 0
rng = np.random.default_rng(seed)
Pobs = []
for i in range(n_obs):

    intersects_bdy = True
    intersects_polys = True
    while intersects_bdy or intersects_polys:
    
        cx = rng.uniform(x_min+dw, x_max-dw)
        cy = rng.uniform(y_min+dw, y_max-dw)
        poly = random_poly(np.array([cx, cy]), n_sides, min_radius, max_radius, rng=rng)

        intersects_bdy = poly.bounds[0] < x_min or poly.bounds[1] < y_min or poly.bounds[2] > x_max or poly.bounds[3] > y_max
        intersects_polys = False
        for P in Pobs:
            if P.intersects(poly):
                intersects_polys = True
                break

    Pobs.append(poly)

Vobs = [shapely_poly_2_vrep(P) for P in Pobs]
Vpart = polypartition.hertel_mehlhorn(Vbdy, Vobs)


fig, ax = plt.subplots()
ax.set_aspect('equal')
for V in Vpart:
    _V = np.vstack((V, V[0,:]))
    ax.fill(_V[:, 0], _V[:, 1])
plt.show()