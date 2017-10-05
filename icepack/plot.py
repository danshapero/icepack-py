
import icepack.ucd, icepack_py
import numpy as np
from io import StringIO

def plot_field(ax, u):
    stream = StringIO()
    u.write_ucd(stream)
    text = stream.getvalue()
    stream.close()

    stream = StringIO(text)
    if type(u) is icepack_py.Field2:
        x, y, cell, U = icepack.ucd.read(stream)
    elif type(u) is icepack_py.VectorField2:
        x, y, cell, Ux, Uy = icepack.ucd.read(stream)
        U = np.sqrt(Ux**2 + Uy**2)
    else:
        raise ValueError('Input argument must be a field or vector field.')
    stream.close()

    triangles = icepack.ucd.quad_cells_to_triangles(x, y, cell);
    contours = ax.tricontourf(x, y, triangles, U)
    return contours


def _plot_edges(ax, x, y, edges, color):
    # I copied this from the matplotlib source code, no idea why it works.
    lines_x = np.insert(x[edges], 2, np.nan, axis=1)
    lines_y = np.insert(y[edges], 2, np.nan, axis=1)

    lines = ax.plot(lines_x.ravel(), lines_y.ravel(), color=color)
    return lines


def plot_mesh(ax, mesh):
    mesh_edges = np.array(icepack_py._get_all_edges(mesh))
    edges = mesh_edges[:, :2]
    at_boundary = mesh_edges[:, 2]
    boundary_ids = mesh_edges[:, 3]

    x = np.array([X[0] for X in mesh.get_vertices()])
    y = np.array([X[1] for X in mesh.get_vertices()])

    interior_edges = edges[at_boundary == 0, :]
    lines = _plot_edges(ax, x, y, interior_edges, 'k')

    colors = ['r', 'g', 'b', 'c', 'm', 'y']
    ids = mesh.get_boundary_ids()
    for id in ids:
        boundary_edges = edges[boundary_ids == id, :]
        line, = _plot_edges(ax, x, y, boundary_edges, colors[id])
        line.set_label('{}'.format(id))
        lines.append(line)
    ax.legend()

    return lines

