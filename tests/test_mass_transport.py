
import icepack

mesh = icepack.read_msh("tests/rectangle.msh")
mesh.refine_global(2)
discretization = icepack.make_discretization(mesh, 1)

u = icepack.interpolate(discretization, lambda x: 0.5, lambda x: 0.0)
h0 = icepack.interpolate(discretization, lambda x: 1 - 0.5 * x[0])
a = icepack.interpolate(discretization, lambda x: 0.0)

def test_mass_transport():
    mass_transport = icepack.MassTransport({0})
    dt = icepack.compute_timestep(0.5, u)
    h = mass_transport.solve(dt, h0, a, u, h0)

