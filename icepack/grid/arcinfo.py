
import numpy as np
from icepack.grid import GridData

# --------------------
def write(file, q):
    """
    Write a gridded data set to an ArcInfo ASCII format file, which can be read
    by GIS software such as ArcGIS or QGIS.

    Parameters:
    ==========
    filename: the destination file
    q: a GridData object
    """
    nx, ny = len(q.x), len(q.y)

    file.write("ncols           {0}\n".format(nx))
    file.write("nrows           {0}\n".format(ny))
    file.write("xllcorner       {0}\n".format(q.x[0]))
    file.write("yllcorner       {0}\n".format(q.y[0]))
    file.write("cellsize        {0}\n".format(q.x[1] - q.x[0]))
    file.write("NODATA_value    {0}\n".format(q.missing))

    for i in range(ny - 1, -1, -1):
        for j in range(nx):
            file.write("{0} ".format(q.data[i, j]))
        file.write("\n")


# ----------------
def read(file):
    """
    Read an ArcInfo ASCII file into a gridded data set.

    Returns:
    =======
    q: a `GridData` object describing the gridded data set's coordinates, values
       and missing data descriptor
    """
    def rd():
        return file.readline().split()[1]

    nx = int(rd())
    ny = int(rd())
    xo = float(rd())
    yo = float(rd())
    dx = float(rd())
    missing = float(rd())

    x = np.array([xo + dx * i for i in range(nx)], dtype = np.float64)
    y = np.array([yo + dx * i for i in range(ny)], dtype = np.float64)
    data = np.zeros((ny, nx), dtype = np.float64)

    for i in range(ny-1, -1, -1):
        data[i, :] = [float(q) for q in file.readline().split()]

    return GridData(x, y, data, missing)

