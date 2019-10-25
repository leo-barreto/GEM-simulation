X0 = 0;
Y0 = 0;
Z0 = 0;
R = 0.035;
D = 0.14;
lc = D / 15;

// Creates arrays for line loops that will generate the boundaries conditions
bnd_ll1 = {}; // +Y
bnd_ll2 = {}; // +X
bnd_ll3 = {}; // -Y
bnd_ll4 = {}; // -X

bnd_s1 = {}; // +Y
bnd_s2 = {}; // +X
bnd_s3 = {}; // -Y
bnd_s4 = {}; // -X



Function gf_plane
  // Creates the necessary points and lines for each GEM plane

  /* X1 = X0 - D / 2 - R;
  X2 = X0 - D / 2;
  X3 = X0 + D / 2;
  X4 = X0 + D / 2 + R;
  Y1 = Y0 + D / 2 + R;
  Y2 = Y0 + D / 2;
  Y3 = Y0 - D / 2;
  Y4 = Y0 - D / 2 - R; */

  X1 = X0 - D / 2;
  X2 = X0 - D / 2 + R;
  X3 = X0 + D / 2 - R;
  X4 = X0 + D / 2;
  Y1 = Y0 + Sqrt(3) * D / 2;
  Y2 = Y0 + Sqrt(3) * D / 2 - R;
  Y3 = Y0 - Sqrt(3) * D / 2 + R;
  Y4 = Y0 - Sqrt(3) * D / 2;

  // Points starting from superior left corner and rotating clockwise
  p1 = newp; Point(p1) = {X1, Y1, Z0, lc}; // Mid Up-Left
  p2 = newp; Point(p2) = {X2, Y1, Z0, lc};
  p3 = newp; Point(p3) = {X3, Y1, Z0, lc};

  p4 = newp; Point(p4) = {X4, Y1, Z0, lc}; // Mid Up-Right
  p5 = newp; Point(p5) = {X4, Y2, Z0, lc};
  p6 = newp; Point(p6) = {X4, Y3, Z0, lc};

  p7 = newp; Point(p7) = {X4, Y4, Z0, lc}; // Mid Down-Right
  p8 = newp; Point(p8) = {X3, Y4, Z0, lc};
  p9 = newp; Point(p9) = {X2, Y4, Z0, lc};

  p10 = newp; Point(p10) = {X1, Y4, Z0, lc}; // Mid Down-Left
  p11 = newp; Point(p11) = {X1, Y3, Z0, lc};
  p12 = newp; Point(p12) = {X1, Y2, Z0, lc};

  // Middle Hole
  p13 = newp; Point(p13) = {X0, Y0 + R, Z0, lc};
  p14 = newp; Point(p14) = {X0 + R, Y0, Z0, lc};
  p15 = newp; Point(p15) = {X0, Y0 - R, Z0, lc};
  p16 = newp; Point(p16) = {X0 - R, Y0, Z0, lc};
  p17 = newp; Point(p17) = {X0, Y0, Z0, lc};

  // Lines and Circles
  c1 = newl; Circle(c1) = {p12, p1, p2};
  l1 = newl; Line(l1) = {p2, p3};
  c2 = newl; Circle(c2) = {p3, p4, p5};
  l2 = newl; Line(l2) = {p5, p6};
  c3 = newl; Circle(c3) = {p6, p7, p8};
  l3 = newl; Line(l3) = {p8, p9};
  c4 = newl; Circle(c4) = {p9, p10, p11};
  l4 = newl; Line(l4) = {p11, p12};

  c5 = newl; Circle(c5) = {p13, p17, p14};
  c6 = newl; Circle(c6) = {p14, p17, p15};
  c7 = newl; Circle(c7) = {p15, p17, p16};
  c8 = newl; Circle(c8) = {p16, p17, p13};

Return


Function VerticalLines
  // Connect each point of one plane to another
  For i In {pmin : pmin + 15}
    If (i != pmin && i != pmin + 3 && i != pmin + 6 && i != pmin + 9)
      l = newl; Line(l) = {i, i + 17};
    EndIf
  EndFor
Return


Function Loops
  // Creates the necessary line and surface loops between two planes

  // Vertical Loops
  // GEM
  outholes = {};
  vloops[] = {};
  For i In {lmin : lmin + 7}
    l2 = i + 24;
    l3 = i + 12;
    If (i == lmin)
      l4 = i + 31;
    Else
      l4 = i + 23;
    EndIf

    ll = newll; Line Loop(ll) = {i, l2, -l3, -l4};
    s = news; Surface(s) = {ll};
    vloops += {s};

    If (i == lmin + 1)
      bnd_ll1 += {ll};
      bnd_s1 += {s};
    ElseIf (i == lmin + 3)
      bnd_ll2 += {ll};
      bnd_s2 += {s};
    ElseIf (i == lmin + 5)
      bnd_ll3 += {ll};
      bnd_s3 += {s};
    ElseIf (i == lmin + 7)
      bnd_ll4 += {ll};
      bnd_s4 += {s};
    Else
      outholes += {s};
    EndIf

  EndFor

  // Hole
  hloops = {};
  For i In {lmin + 8 : lmin + 11}
    l3 = i + 12;
    l4 = i + 24;
    If (i == lmin + 11)
      l2 = i + 21;
    Else
      l2 = i + 25;
    EndIf

    ll = newll; Line Loop(ll) = {i, l2, -l3, -l4};
    s = news; Surface(s) = {ll};
    hloops += {s};
  EndFor


  // Horizontal Loops
  up_outerll = newll; Line Loop(up_outerll) = {lmin, lmin + 1, lmin + 2,
                                               lmin + 3, lmin + 4, lmin + 5,
                                               lmin + 6, lmin + 7};
  up_holell = newll; Line Loop(up_holell) = {lmin + 8, lmin + 9, lmin + 10,
                                             lmin + 11};
  lo_outerll = newll; Line Loop(lo_outerll) = {lmin + 12, lmin + 13, lmin + 14,
                                               lmin + 15, lmin + 16, lmin + 17,
                                               lmin + 18, lmin + 19};
  lo_holell = newll; Line Loop(lo_holell) = {lmin + 20, lmin + 21, lmin + 22,
                                             lmin + 23};
  up_gemplane = news; Plane Surface(up_gemplane) = {up_outerll, up_holell};
  //up_holeplane = news; Surface(up_holeplane) = {up_holell};
  lo_gemplane = news; Plane Surface(lo_gemplane) = {lo_outerll, lo_holell};
  //lo_holeplane = news; Surface(lo_holeplane) = {lo_holell};


  // Surfaces Loops
  // For GEM: up gem -> vertical loops -> lo gem -> vertical hole loops
  loops = {};
  loops += {up_gemplane};
  loops += vloops[];
  loops += {lo_gemplane};
  loops += hloops[];

  sl = newsl; Surface Loop(sl) = loops[];
  gemv = newv; Volume(gemv) = {sl};

  outsideloops = {};
  //outsideloops += {up_gemplane};
  outsideloops += outholes[];
  outsideloops += hloops[];
  //outsideloops += {lo_gemplane};

Return
