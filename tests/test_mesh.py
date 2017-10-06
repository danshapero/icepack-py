
import icepack

def test_reading():
    mesh = icepack.read_msh("tests/rectangle.msh")
    assert mesh.n_active_cells() == 16
    assert mesh.n_vertices() == 25

    boundary_ids = mesh.get_boundary_ids()
    assert len(boundary_ids) == 2
    assert boundary_ids.count(1) == 1
    assert boundary_ids.count(2) == 1


def test_refining():
    mesh = icepack.read_msh("tests/rectangle.msh")
    num_cells = mesh.n_active_cells()
    mesh.refine_global(1)
    assert mesh.n_active_cells() == 4 * num_cells

