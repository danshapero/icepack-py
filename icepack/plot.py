
import icepack.ucd, icepack_py
import numpy as np
import matplotlib.pyplot as plt
from io import StringIO

def plot_field(u):
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
    fig = plt.figure()
    ax = fig.gca()
    ax.set_aspect('equal')
    contours = ax.tricontourf(x, y, triangles, U)

    return fig

