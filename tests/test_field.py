
import icepack

mesh = icepack.read_msh("tests/rectangle.msh")
mesh.refine_global(2)

def test_discretization():
    discretization = icepack.make_discretization(mesh, 1);
    assert discretization.triangulation is not None


def test_interpolating():
    discretization = icepack.make_discretization(mesh, 1);
    u = icepack.interpolate(discretization, lambda x: x[0] * x[1])
    assert u.discretization is discretization

