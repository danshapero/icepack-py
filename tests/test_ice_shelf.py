
import icepack
from icepack import rho_ice, rho_water, gravity, rate_factor

mesh = icepack.read_msh("tests/rectangle.msh")
mesh.refine_global(2)
discretization = icepack.make_discretization(mesh, 1)

h0, dh = 500.0, 100.0
length, width = 20.0e3, 20.0e3
temp = 254.15

def velocity(x):
    u0 = 100.0
    rho = rho_ice * (1 - rho_ice / rho_water)
    A = (rho * gravity * h0 / 4)**3 * rate_factor(temp)
    q = 1 - (1 - (dh / h0) * (x[0] / length))**4
    return u0 + 0.25 * A * q * length * h0 / dh


def thickness(x):
    return h0 - dh * x[0] / length


def temperature(x):
    dtheta = 5.0
    inside = ((length/4 < x[0] < 3*length/4) and (width/4 < x[1] < 3*width/4))
    return temp + inside * dtheta


def test_ice_shelf():
    ice_shelf = icepack.IceShelf({0})

    h = icepack.interpolate(discretization, thickness)
    u0 = icepack.interpolate(discretization, velocity, lambda x: 0.0)
    theta = icepack.interpolate(discretization, temperature)

    u = ice_shelf.solve(h, theta, u0)

