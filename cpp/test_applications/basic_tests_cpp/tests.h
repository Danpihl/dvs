#ifndef TESTS_H_
#define TESTS_H_

#include <algorithm>

#include "dvs/dvs.h"

using namespace dvs;

void testSurf()
{
    const int num_rows = 20, num_cols = 25;
    Matrix<double> x(num_rows, num_cols), y(num_rows, num_cols), z(num_rows, num_cols);

    double inc = 0.4;
    for (int r = 0; r < num_rows; r++)
    {
        for (int c = 0; c < num_cols; c++)
        {
            const double rd = static_cast<double>(r - 5) * inc;
            const double cd = static_cast<double>(c - 5) * inc * 2;
            x(r, c) = c;
            y(r, c) = r;
            z(r, c) = 50.0 * std::sin(std::sqrt(rd * rd + cd * cd));
        }
    }

    setCurrentElement("view_00");
    clearView();
    surf(x, y, z, properties::EdgeColor(0, 0, 0), properties::FaceColor(150, 244, 244), properties::LineWidth(1));
}

void testScatter()
{
    Vector<double> x(20), y(20);
    Vector<float> xf(20), yf(20);

    double t = 0.0;

    for (size_t k = 0; k < 20; k++)
    {
        xf(k) = k;
        yf(k) = 5.0 * std::sin(3.0 * t);

        x(k) = xf(k);
        y(k) = yf(k);

        t = t + 0.1;
    }

    setCurrentElement("view_00");
    clearView();
    // axis({-1.1, -2.2, -3.3}, {4.4, 5.5, 6.6});

    // plot(xf, yf, properties::Color(212, 14, 55));
    scatter(x, y, properties::Color(0, 255, 255), properties::PointSize(3));
}

void testScatter3()
{
    const size_t num_elements = 14600;
    Vector<double> x(num_elements), y(num_elements), z(num_elements);

    double t0 = 0.0f, t1 = 0.0f, t2 = 0.0f, y0 = 0.0f, z0 = 0.0f;

    for (size_t k = 0; k < num_elements; k++)
    {
        const float xf = 7.0 * cos(t0) + 20.0f;
        const float yf = 7.0 * sin(t0) - 110.0f;
        const float zf = 0.0f;

        const Vec3Df v0 = rotationMatrixY<float>(t1) * Vec3Df(xf, yf, zf);
        const Vec3Df v1 = rotationMatrixZ<float>(t2) * (v0 + Vec3Df(50.0f, y0, 0.0f));

        x(k) = v1.x;
        y(k) = v1.y;
        z(k) = v1.z + z0 - 70;
        t0 = t0 + 0.3;
        t1 = t1 - 0.01;
        t2 = t2 + 0.002;
        y0 = y0 + 0.05;
        z0 = z0 + 0.02;
    }

    setCurrentElement("view_00");
    clearView();

    axis({-128.0, -128.0, -128.0}, {128.0, 128.0, 128.0});
    scatter3(x, y, z, properties::Color(212, 14, 55), properties::PointSize(3));
    plot3(x, y, z, properties::Color(21, 14, 55), properties::LineWidth(1));
}

void testPlotCollection()
{
    const size_t num_elements = 40;
    Vector<double> x0(num_elements), y0(num_elements), x1(num_elements), y1(num_elements), x2(num_elements),
        y2(num_elements);

    double t = 0.0;

    for (size_t k = 0; k < num_elements; k++)
    {
        const double kd = k;
        x0(k) = 10.0 * cos(t);
        y0(k) = kd * 0.05 * 10.0 * sin(t) + 0.2;

        x1(k) = 14.0 * cos(t);
        y1(k) = kd * 0.07 * 11.0 * sin(t) + 0.3;

        x2(k) = 9.0 * cos(t);
        y2(k) = kd * 0.08 * 15.0 * sin(t) + 0.4;
        t = t + 0.1;
    }

    std::vector<Vector<double>> pc_x = {x0, x1, x2};
    std::vector<Vector<double>> pc_y = {y0, y1, y2};

    std::vector<Vector<double>> pcm_x;
    std::vector<Vector<double>> pcm_y;

    auto rand_num = [](void) -> double { return static_cast<double>(rand() % 1001) / 1000.0; };

    uint32_t num_points = 0;

    std::vector<size_t> np = {50, 50, 50, 50, 56};

    for (size_t k = 0; k < 30; k++)
    {
        const size_t num_elems = (rand() % 20) + 50;
        Vector<double> x(num_elems), y(num_elems);
        num_points += num_elems;
        t = 0.0;

        const double t_inc = rand_num() / 100.0 + 0.05;
        const double x_a = rand_num() * 3.0;
        const double y_a = rand_num() * 3.0;
        const double x_o = rand_num() * 10.0;
        const double y_o = rand_num() * 10.0;

        for (size_t i = 0; i < num_elems; i++)
        {
            x(i) = x_a * cos(t) + x_o;
            y(i) = y_a * t * sin(t) + y_o;
            t += t_inc;
        }
        pcm_x.push_back(std::move(x));
        pcm_y.push_back(std::move(y));
    }

    setCurrentElement("view_00");
    clearView();

    axis({-48.0, -48.0, -1.0}, {48.0, 48.0, 1.0});

    plotCollection(pc_x, pc_y, properties::Color(0, 0, 0));
    scatter(x0, y0, properties::Color(212, 14, 55), properties::PointSize(3));
    scatter(x1, y1, properties::Color(0, 14, 55), properties::PointSize(3));
    scatter(x2, y2, properties::Color(0, 255, 55), properties::PointSize(3));

    setCurrentElement("view_01");
    clearView();

    for (size_t k = 0; k < pcm_x.size(); k++)
    {
        scatter(pcm_x[k], pcm_y[k], properties::Color(255, 0, 0), properties::PointSize(3));
    }

    axis({-8.0, -8.0, -1.0}, {8.0, 8.0, 1.0});

    plotCollection(pcm_x, pcm_y, properties::Color(0, 0, 0));
}

void testPlot()
{
    const size_t num_elements = 30;
    Vector<int64_t> x(num_elements), y(num_elements);
    Vector<float> xf(num_elements), yf(num_elements);

    double t = 0.0;

    for (size_t k = 0; k < num_elements; k++)
    {
        xf(k) = 10.0 * cos(t) + 20.0;
        yf(k) = 10.0 * sin(t) + 20.0 + k;

        x(k) = xf(k);
        y(k) = yf(k);
        t = t + 0.3;
    }

    setCurrentElement("view_00");
    clearView();

    axis({0.0, 16.0, -1.0}, {50.0, 64.0, 1.0});
    plot(x, y, properties::Color(212, 14, 55), properties::LineWidth(1));
    plot(xf, yf, properties::Color(21, 14, 55), properties::LineWidth(4), properties::LINE_STRIP);
    plot(xf, yf, properties::Color(21, 14, 55), properties::LineWidth(1));
}

void testStairs()
{
    const size_t num_elements = 30;
    Vector<float> xf(num_elements), yf(num_elements);

    double t = 0.0;

    for (size_t k = 0; k < num_elements; k++)
    {
        xf(k) = t;
        yf(k) = 10.0 * sin(t) + 20.0 + k;

        t = t + 0.3;
    }

    setCurrentElement("view_00");
    clearView();

    stairs(xf, yf, properties::Color(21, 14, 55), properties::LineWidth(2));
    plot(xf, yf, properties::Color(21, 14, 255), properties::LineWidth(2));
    scatter(xf, yf, properties::Color(255, 0, 0), properties::PointSize(10));
}

void testPlot3()
{
    const size_t num_elements = 30;
    Vector<double> x(num_elements), y(num_elements), z(num_elements);
    Vector<float> xf(num_elements), yf(num_elements), zf(num_elements);

    double t = 0.0;

    for (size_t k = 0; k < num_elements; k++)
    {
        xf(k) = 10.0 * cos(t) + 20.0;
        yf(k) = 10.0 * sin(t) + 20.0;
        zf(k) = k;

        x(k) = xf(k);
        y(k) = yf(k);
        z(k) = k;
        t = t + 0.3;
    }

    setCurrentElement("view_00");
    clearView();

    plot3(x, y, z, properties::Color(212, 14, 55), properties::LineWidth(1));
    plot3(xf, yf, zf, properties::Color(21, 14, 55), properties::LineWidth(1));
}

void testImShow()
{
    const uint32_t num_rows = 800, num_cols = 800;
    ImageC3<float> img3(num_rows, num_cols);
    ImageC1<float> img1(num_rows, num_cols);
    // const double max_val = num_rows * num_cols;

    for (uint32_t r = 0; r < num_rows; r++)
    {
        for (uint32_t c = 0; c < num_cols; c++)
        {
            const float xr = 3.0f * (static_cast<float>(c) - 300.5f) / 800.0f;
            const float yr = 3.0f * (static_cast<float>(r) - 400.5f) / 800.0f;
            const float rr = std::sqrt(xr * xr + yr * yr);

            const float xg = 2.0f * (static_cast<float>(c) - 500.5f) / 800.0f;
            const float yg = 2.0f * (static_cast<float>(r) - 350.5f) / 800.0f;
            const float rg = std::sqrt(xg * xg + yg * yg);

            const float xb = 4.0f * (static_cast<float>(c) - 200.5f) / 800.0f;
            const float yb = 4.0f * (static_cast<float>(r) - 600.5f) / 800.0f;
            const float rb = std::sqrt(xb * xb + yb * yb);

            img3(r, c, 0) = (std::sin(rb) / rr + 1.0f) * 0.5f;
            img3(r, c, 1) = (std::sin(rb) / rg + 1.0f) * 0.5f;
            img3(r, c, 2) = (std::sin(rb) / rb + 1.0f) * 0.5f;

            img1(r, c) = (std::sin(rb) / rr + 1.0f) * 0.5f;

            // img(r, c, 0) = (r * c) / max_val;
            // img(r, c, 1) = 1.0f - (r * c) / max_val;
            // img(r, c, 2) = (r * (num_cols - 1 - c)) / max_val;
        }
    }

    setCurrentElement("view_00");
    imShow(img3, properties::Alpha(137));

    setCurrentElement("view_01");
    imShow(img1, properties::Alpha(137));
}

void testDrawPolygonFrom4Points()
{
    setCurrentElement("view_00");

    Point3D<double> p0(0.0, 0.0, 0.0), p1(1.0, 0.0, 1.0), p2(1.0, 1.0, 2.0), p3(0.0, 1.0, 3.0);
    drawPolygonFrom4Points(p0, p1, p2, p3);
}

void testDrawXYPlane()
{
    VecXY<double> p0_xy(0.0, 0.0), p1_xy(1.0, 1.0);
    VecXZ<float> p0_xz(0.0, 0.0), p1_xz(1.0, 1.0);
    VecYZ<int16_t> p0_yz(0.0, 0.0), p1_yz(1.0, 1.0);
    setCurrentElement("view_00");
    clearView();
    drawPlaneXY(p0_xy,
                p1_xy,
                Plane<float>(0.1, 0.1, 0.1, 0.4),
                properties::FaceColor(126, 225, 12),
                properties::EdgeColor(0, 0, 0));

    setCurrentElement("view_01");
    clearView();
    drawPlaneXZ(p0_xz,
                p1_xz,
                Plane<double>(0.1, 0.1, 0.1, 0.4),
                properties::FaceColor(126, 225, 12),
                properties::EdgeColor(0, 0, 0));

    setCurrentElement("view_02");
    clearView();
    drawPlaneYZ(p0_yz,
                p1_yz,
                Plane<double>(0.1, 0.1, 0.1, 0.4),
                properties::FaceColor(126, 225, 12),
                properties::EdgeColor(0, 0, 0));
}

void testAxis()
{
    setCurrentElement("view_00");
    clearView();

    axis({1.0, 2.5}, {7.2, 8.4});

    setCurrentElement("view_01");
    clearView();

    axis({-1.0, 1.5, 3.4}, {5.4, 9.2, 5.5});
}

void testDrawTriangles()
{
    const size_t num_elements = 10;
    using tp = float;
    Vector<Triangle3D<tp>> triangles(num_elements);

    double t = 0.0;
    Point3D<double> p0(-1.0, -1.0, 0.0), p1(1.0, -1.0, 0.0), p2(0.0, 1.0, 0.0);

    for (size_t k = 0; k < num_elements; k++)
    {
        const Matrix<double> r = rotationMatrixZ(t);

        const Point3D<double> p0r = r * p0 + Point3D<double>(0.0, 0.0, t * 3);
        const Point3D<double> p1r = r * p1 + Point3D<double>(0.0, 0.0, t * 3);
        const Point3D<double> p2r = r * p2 + Point3D<double>(0.0, 0.0, t * 3);

        Triangle3D<tp> tri(p0r, p1r, p2r);
        triangles(k) = tri;

        t = t + 0.3;
    }

    setCurrentElement("view_00");
    clearView();

    drawTriangles(triangles, properties::EdgeColor(0, 0, 0), properties::FaceColor(12, 244, 244));
}

void testDrawTiles()
{
    const int num_rows = 20, num_cols = 25;
    Matrix<float> x(num_rows, num_cols), y(num_rows, num_cols), z(num_rows, num_cols);

    double inc = 0.4;
    for (int r = 0; r < num_rows; r++)
    {
        for (int c = 0; c < num_cols; c++)
        {
            const double rd = static_cast<double>(r - 5) * inc;
            const double cd = static_cast<double>(c - 5) * inc * 2;
            x(r, c) = c;
            y(r, c) = r;
            z(r, c) = 50.0 * std::sin(std::sqrt(rd * rd + cd * cd));
        }
    }

    const Vec2Dd tile_size(0.1, 0.2);

    setCurrentElement("view_00");
    clearView();

    drawTiles(z, tile_size, properties::EdgeColor(0, 0, 0), properties::FaceColor(12, 244, 244));
}

void testDrawMesh()
{
    using tp = double;

    Vector<Point3D<tp>> vertices(4);
    vertices(0) = Point3D<tp>(-1.0, -1.0, -1.0);
    vertices(1) = Point3D<tp>(1.0, -1.0, -1.0);
    vertices(2) = Point3D<tp>(1.0, 1.0, -1.0);
    vertices(3) = Point3D<tp>(0.0, 0.0, 1.0);

    Vector<IndexTriplet> indices(4);
    indices(0) = IndexTriplet(0, 1, 2);
    indices(1) = IndexTriplet(0, 1, 3);
    indices(2) = IndexTriplet(1, 2, 3);
    indices(3) = IndexTriplet(2, 0, 3);

    setCurrentElement("view_00");
    clearView();

    axis({-2.0, -2.0, -2.0}, {2.0, 2.0, 2.0});

    drawMesh(vertices, indices, properties::EdgeColor(0, 0, 0), properties::FaceColor(12, 244, 244));
}

void testDrawTriangle()
{
    using tp = double;

    Triangle3D<tp> triangle;
    triangle.p0 = Point3D<tp>(-1.0, -1.0, -1.0);
    triangle.p1 = Point3D<tp>(1.0, -1.0, -1.0);
    triangle.p2 = Point3D<tp>(1.0, 1.0, 1.0);

    setCurrentElement("view_00");
    clearView();

    axis({-2.0, -2.0, -2.0}, {2.0, 2.0, 2.0});

    drawTriangle(triangle, properties::EdgeColor(0, 0, 0), properties::FaceColor(12, 244, 244));
}

void testDrawArrow()
{
    using tp = double;

    Point2D<tp> p(0.1, 0.2);
    Vec2D<tp> v(0.5, -0.3);

    setCurrentElement("view_00");
    clearView();

    axis({-2.0, -2.0, -2.0}, {2.0, 2.0, 2.0});

    drawArrow(p, v, properties::Color(12, 244, 244));
}

void testQuiver()
{
    using tp = float;
    const size_t num_rows = 100, num_cols = 100;

    Matrix<tp> x, y, u, v;

    const std::pair<Matrix<tp>, Matrix<tp>> mesh_grid = meshGrid(0.1, 100.0, 0.1, 100.0, num_cols, num_rows);
    x = mesh_grid.first;
    y = mesh_grid.second;

    u = dvs::cos(x);
    v = dvs::sin(y);

    setCurrentElement("view_00");
    clearView();

    axis({-2.0, -2.0, -2.0}, {2.0, 2.0, 2.0});

    quiver(x, y, u, v, properties::Color(12, 244, 244));
}

void testDrawLine3D()
{
    using tp = double;

    Line3D<tp> line(Point3D<tp>(0.0, 0.0, 0.0), Vec3D<tp>(1.0, 1.0, 1.0));
    tp t0 = 0;
    tp t1 = 1.0;

    Vector<double> x = {0.0, 1.0};
    Vector<double> y = {0.0, 1.0};
    Vector<double> z = {0.0, 1.0};

    setCurrentElement("view_00");
    clearView();

    axis({-2.0, -2.0, -2.0}, {2.0, 2.0, 2.0});

    drawLine(line, t0, t1, properties::Color(12, 244, 244));
    scatter3(x, y, z, properties::Color(0, 0, 0), properties::PointSize(3));
}

void testDrawLine2D()
{
    using tp = double;

    ParametricLine2D<tp> line(Point2D<tp>(0.0, 0.0), Vec2D<tp>(1.0, 1.0));
    HomogeneousLine2D<tp> line_h(0.1, 0.2, 0.3);
    tp t0 = 0;
    tp t1 = 1.0;

    Vector<double> x = {0.0, 1.0};
    Vector<double> y = {0.0, 1.0};
    Vector<double> z = {0.0, 0.0};

    setCurrentElement("view_00");
    clearView();

    axis({-2.0, -2.0, -2.0}, {2.0, 2.0, 2.0});

    drawLine2D(line, t0, t1, properties::Color(12, 244, 244));
    drawLine2DBetweenXValues(line_h, -3.0, 3.0, properties::Color(12, 15, 244));
    drawLine2DBetweenYValues(line_h, -2.0, 2.0, properties::Color(12, 15, 244));
    scatter3(x, y, z, properties::Color(0, 0, 0), properties::PointSize(3));
}

void testDrawLineBetweenPoints()
{
    using tp = double;

    Point3D<tp> p0(0.1, 0.2, 0.2);
    Point3D<tp> p1(2.0, 1.0, 1.0);

    Vector<double> x = {p0.x, p1.x};
    Vector<double> y = {p0.y, p1.y};
    Vector<double> z = {p0.z, p1.z};

    setCurrentElement("view_00");
    clearView();

    axis({-2.0, -2.0, -2.0}, {2.0, 2.0, 2.0});

    drawLineBetweenPoints(p0, p1, properties::Color(12, 244, 0));
    scatter3(x, y, z, properties::Color(0, 0, 0), properties::PointSize(3));
}

#endif
