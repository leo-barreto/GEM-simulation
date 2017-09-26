//Default Parameters
x0 = 0; y0 = 0; z0 = 0;  // Centre
R = 1;     // Radius
T = 1;     // Thickness
R_O = 0.3;   // Outer Radius
lc = 0.03;
A = 0;

Function gf_hole

  R_M = R - R_O;


// Points
  p1 = newp; Point(p1) = {x0 + R, y0, z0, lc};
  p2 = newp; Point(p2) = {x0, y0, z0, lc};
  p3 = newp; Point(p3) = {x0, y0 + R, z0, lc};

  p4 = newp; Point(p4) = {x0 + R_M, y0, z0 - T / 2, lc};
  p5 = newp; Point(p5) = {x0, y0, z0 - T / 2, lc};
  p6 = newp; Point(p6) = {x0, y0 + R_M, z0 - T / 2, lc};

  p7 = newp; Point(p7) = {x0 + R, y0, z0 - T, lc};
  p8 = newp; Point(p8) = {x0, y0, z0 - T, lc};
  p9 = newp; Point(p9) = {x0, y0 + R, z0 - T, lc};


// Lines
  c1 = newc; Circle(c1) = {p1, p2, p3};
  c2 = newc; Circle(c2) = {p4, p5, p6};
  c3 = newc; Circle(c3) = {p7, p8, p9};

  l1 = newl; Line(l1) = {p1, p4};
  l2 = newl; Line(l2) = {p4, p7};
  l3 = newl; Line(l3) = {p2, p5};
  l4 = newl; Line(l4) = {p5, p8};
  l5 = newl; Line(l5) = {p3, p6};
  l6 = newl; Line(l6) = {p6, p9};

  l7 = newl; Line(l7) = {p1, p2};
  l8 = newl; Line(l8) = {p2, p3};
  l9 = newl; Line(l9) = {p4, p5};
  l10 = newl; Line(l10) = {p5, p6};
  l11 = newl; Line(l11) = {p7, p8};
  l12 = newl; Line(l12) = {p8, p9};


// Lines Loops
  ll1 = newll; Line Loop(ll1) = {c1, l5, -c2, -l1};
  ll2 = newll; Line Loop(ll2) = {c2, l6, -c3, -l2};

  ll3 = newll; Line Loop(ll3) = {l7, l8, -c1};
  ll4 = newll; Line Loop(ll4) = {l11, l12, -c3};

  ll5 = newll; Line Loop(ll5) = {l7, l3, l4, -l11, -l2, -l1};
  ll6 = newll; Line Loop(ll6) = {-l8, l3, l4, l12, -l6, -l5};


// Surfaces
  s1 = news; Ruled Surface(s1) = {ll1};
  s2 = news; Ruled Surface(s2) = {ll2};
  s3 = news; Plane Surface(s3) = {ll3};
  s4 = news; Plane Surface(s4) = {ll4};
  s5 = news; Plane Surface(s5) = {ll5};
  s6 = news; Plane Surface(s6) = {ll6};

  sl = newsl; Surface Loop(sl) = {s3, s1, s5, s6, s2, s4};
  v = newv; Volume(v) = {sl};

  Rotate{{0, 0, 1}, {x0, y0, z0}, A} {
     Volume{v} ; }


  Return
