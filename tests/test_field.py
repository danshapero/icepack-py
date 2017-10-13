
import icepack

mesh = icepack.read_msh("tests/rectangle.msh")
mesh.refine_global(2)
discretization = icepack.make_discretization(mesh, 1)

def test_discretization():
    assert discretization.triangulation is not None


def test_interpolating():
    u = icepack.interpolate(discretization, lambda x: x[0] * x[1])
    assert u.discretization is discretization
    assert icepack.max(u) <= 1.0


def test_algebra():
    u = icepack.interpolate(discretization, lambda x: x[0] - x[1])
    v = icepack.interpolate(discretization, lambda x: x[0] * x[1])

    w = icepack.interpolate(discretization, lambda x: x[0] - x[1] + x[0] * x[1])
    assert icepack.dist(u + v, w) / icepack.norm(w) < 1.0e-8

    w = icepack.interpolate(discretization, lambda x: x[0] - x[1] - x[0] * x[1])
    assert icepack.dist(u - v, w) / icepack.norm(w) < 1.0e-8

    w = icepack.interpolate(discretization, lambda x: 2 * x[0] * x[1])
    assert icepack.dist(2.0 * v, w) / icepack.norm(w) < 1.0e-8

    assert abs(icepack.inner_product(u, v)) < 1.0e-8

