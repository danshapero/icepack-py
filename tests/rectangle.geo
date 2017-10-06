
cl = 1.0e15;
Point(1) = {0.0, 0.0, 0.0, cl};
Point(2) = {1.0, 0.0, 0.0, cl};
Point(3) = {1.0, 1.0, 0.0, cl};
Point(4) = {0.0, 1.0, 0.0, cl};

Line(5) = {1, 2};
Line(6) = {2, 3};
Physical Line(1) = {5, 6};

Line(7) = {3, 4};
Line(8) = {4, 1};
Physical Line(2) = {7, 8};

Line Loop(9) = {5, 6, 7, 8};
Plane Surface(10) = {9};
Physical Surface(3) = {10};

Recombine Surface{10};
Mesh.SubdivisionAlgorithm=1;
Mesh.Algorithm=8;

