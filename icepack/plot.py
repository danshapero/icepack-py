
import icepack_py
from icepack import ucd
import matplotlib.pyplot as plt
from io import StringIO

def plot_field(u):
    stream = StringIO()
    u.write_ucd(stream)
    text = stream.getvalue()
    stream.close()

    stream = StringIO(text)
    x, y, cell, U = ucd.read(stream)
    stream.close()

    triangles = ucd.quad_cells_to_triangles(x, y, cell);

    fig = plt.figure()
    ax = fig.gca()
    ax.set_aspect('equal')
    contours = ax.tricontourf(x, y, triangles, U)

    return fig

