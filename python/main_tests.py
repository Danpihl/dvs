import dvs
import numpy as np
from structures import *


def setup_dvs_view():
    dvs.set_current_element("view_00")
    dvs.clear_view()


def test_scatter():
    dvs.set_current_element("view_00")
    dvs.clear_view()

    for i in range(0, 100):
        dvs.soft_clear_view()
        x = np.linspace(0, 3, 100, dtype=np.float32)
        y = np.sin(x * 5.0 + 0.1 * i)

        dvs.view(0, 90)
        dvs.scatter(x, y)


def test_plot():
    dvs.set_current_element("view_00")
    dvs.clear_view()

    for i in range(0, 100):
        dvs.soft_clear_view()
        x = np.linspace(0, 3, 100, dtype=np.float32)
        y = np.sin(x * 5.0 + 0.1 * i)

        dvs.axis([-10, -10, -10], [10, 10, 10])
        dvs.view(0, 90)
        dvs.plot(x, y)  # ,color=dvs.Color(23, 25, 66) alpha=100, line_width=13


def test_plot3():
    dvs.set_current_element("view_00")

    dvs.clear_view()
    x = np.linspace(0, 3, 100, dtype=np.float32)
    y = np.sin(x * 5.0)
    z = np.cos(x * 5.0)

    dvs.axis([-10, -10, -10], [10, 10, 10])
    dvs.view(0, 90)
    dvs.plot3(x, y, z)


def test_scatter3():
    dvs.set_current_element("view_00")

    dvs.clear_view()
    x = np.linspace(0, 3, 100, dtype=np.float32)
    y = np.sin(x * 5.0)
    z = np.cos(x * 5.0)
    dvs.scatter3(x, y, z)


def test_surf():
    dvs.set_current_element("view_00")

    dvs.clear_view()
    x = np.linspace(-0.1, 3, 100, dtype=np.float32)
    y = np.linspace(-0.1, 3, 100, dtype=np.float32)
    x, y = np.meshgrid(x, y)
    r = np.sqrt(x * x + y * y)
    z = np.sin(r) / r

    dvs.axis([-10, -10, -10], [10, 10, 10])
    dvs.view(0, 90)
    dvs.surf(x, y, z)


def test_imshow():
    dvs.set_current_element("view_00")

    dvs.clear_view()
    x = np.linspace(-0.1, 3, 100, dtype=np.float32)
    y = np.linspace(-0.1, 3, 100, dtype=np.float32)
    x, y = np.meshgrid(x, y)
    r = np.sqrt(x * x + y * y)
    zr = np.sin(r) / r
    zg = np.sin(r)
    zb = np.sin(r) / (r * r)

    zr = (zr - np.min(zr)) / (np.max(zr) - np.min(zr))
    zg = (zg - np.min(zg)) / (np.max(zg) - np.min(zg))
    zb = (zb - np.min(zb)) / (np.max(zb) - np.min(zb))

    q = np.zeros((3, 100, 100), dtype=np.float32)
    q[0, :, :] = zr
    q[1, :, :] = zg
    q[2, :, :] = zb

    dvs.axis([-10, -10, -10], [10, 10, 10])
    dvs.view(0, 90)
    dvs.imshow(q)


def test_draw_polygon_from_4_points():
    setup_dvs_view()
    p0 = Point3D(0.0, 0.0, 0.0)
    p1 = Point3D(1.0, 0.0, 1.0)
    p2 = Point3D(1.0, 1.0, 2.0)
    p3 = Point3D(0.0, 1.0, 3.0)

    dvs.draw_polygon_from_4_points(p0, p1, p2, p3)


def test_draw_triangle():
    setup_dvs_view()
    triangle = Triangle(Point3D(0.0, 0.0, 0.0), Point3D(
        1.0, 1.0, 0.0), Point3D(1.0, 0.0, 0.0))


def test_draw_triangles():
    setup_dvs_view()


def test_draw_mesh():
    setup_dvs_view()


def test_draw_plane_xy():
    setup_dvs_view()


def test_draw_plane_xz():
    setup_dvs_view()


def test_draw_plane_yz():
    setup_dvs_view()


def test_draw_line():
    setup_dvs_view()


def test_draw_line_2d():
    setup_dvs_view()


def test_draw_line_2d_between_x_values():
    setup_dvs_view()


def test_draw_line_2d_between_y_values():
    setup_dvs_view()


def test_draw_line_between_points():
    p0 = Point3D(0.0, 0.0, 0.0)
    p1 = Point3D(1.0, 2.0, 3.0)

    setup_dvs_view()
    draw_line_between_points(p0, p1)
