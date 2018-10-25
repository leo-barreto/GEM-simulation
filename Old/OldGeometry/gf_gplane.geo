//Default Parameters
x0 = 0; y0 = 0; z0 = 0;  // Centre
R = 1;                   // Radius
D = 3;                   // Hole Distace
T = 1;                   // Thickness
R_O = 0;                 // Outer Radius
lc = 0.03;

Function gf_gplane

  R_M = R - R_O;
  H = Sqrt(3) * D / 2;

  // Points
  p1 = newp; Point(p1) = {x0, y0 - R + H / 2, z0, lc};
  p2 = newp; Point(p2) = {x0, y0 + H / 2, z0, lc};
  p3 = newp; Point(p3) = {x0 - R, y0 + H / 2, z0, lc};
  p4 = newp; Point(p4) = {x0 - D / 2, y0 + H / 2, z0, lc};
  p5 = newp; Point(p5) = {x0 - D / 2, y0 - H / 2+ R, z0, lc};
  p6 = newp; Point(p6) = {x0 - D / 2, y0 - H / 2, z0, lc};
  p7 = newp; Point(p7) = {x0 - D / 2 + R, y0 - H / 2, z0, lc};
  p8 = newp; Point(p8) = {x0 + D / 2 - R, y0 - H / 2, z0, lc};
  p9 = newp; Point(p9) = {x0 + D / 2, y0 - H / 2, z0, lc};
  p10 = newp; Point(p10) = {x0 + D / 2, y0 - H / 2 + R, z0, lc};
  p11 = newp; Point(p11) = {x0 + D / 2, y0 + H / 2, z0, lc};
  p12 = newp; Point(p12) = {x0 + R, y0 + H / 2, z0, lc};

  p13 = newp; Point(p13) = {x0, y0 - R_M + H / 2, z0 - T / 2, lc};
  p14 = newp; Point(p14) = {x0, y0 + H / 2, z0 - T / 2, lc};
  p15 = newp; Point(p15) = {x0 - R_M, y0 + H / 2, z0 - T / 2, lc};
  p16 = newp; Point(p16) = {x0 - D / 2, y0 + H / 2, z0 - T / 2, lc};
  p17 = newp; Point(p17) = {x0 - D / 2, y0 - H / 2+ R_M, z0 - T / 2, lc};
  p18 = newp; Point(p18) = {x0 - D / 2, y0 - H / 2, z0 - T / 2, lc};
  p19 = newp; Point(p19) = {x0 - D / 2 + R_M, y0 - H / 2, z0 - T / 2, lc};
  p20 = newp; Point(p20) = {x0 + D / 2 - R_M, y0 - H / 2, z0 - T / 2, lc};
  p21 = newp; Point(p21) = {x0 + D / 2, y0 - H / 2, z0 - T / 2, lc};
  p22 = newp; Point(p22) = {x0 + D / 2, y0 - H / 2 + R_M, z0 - T / 2, lc};
  p23 = newp; Point(p23) = {x0 + D / 2, y0 + H / 2, z0 - T / 2, lc};
  p24 = newp; Point(p24) = {x0 + R_M, y0 + H / 2, z0 - T / 2, lc};

  p25 = newp; Point(p25) = {x0, y0 - R + H / 2, z0 - T, lc};
  p26 = newp; Point(p26) = {x0, y0 + H / 2, z0 - T, lc};
  p27 = newp; Point(p27) = {x0 - R, y0 + H / 2, z0 - T, lc};
  p28 = newp; Point(p28) = {x0 - D / 2, y0 + H / 2, z0 - T, lc};
  p29 = newp; Point(p29) = {x0 - D / 2, y0 - H / 2+ R, z0 - T, lc};
  p30 = newp; Point(p30) = {x0 - D / 2, y0 - H / 2, z0 - T, lc};
  p31 = newp; Point(p31) = {x0 - D / 2 + R, y0 - H / 2, z0 - T, lc};
  p32 = newp; Point(p32) = {x0 + D / 2 - R, y0 - H / 2, z0 - T, lc};
  p33 = newp; Point(p33) = {x0 + D / 2, y0 - H / 2, z0 - T, lc};
  p34 = newp; Point(p34) = {x0 + D / 2, y0 - H / 2 + R, z0 - T, lc};
  p35 = newp; Point(p35) = {x0 + D / 2, y0 + H / 2, z0 - T, lc};
  p36 = newp; Point(p36) = {x0 + R, y0 + H / 2, z0 - T, lc};


  // Lines
  c1 = newc; Circle(c1) = {p1, p2, p3};
  l1 = newl; Line(l1) = {p3, p4};
  l2 = newl; Line(l2) = {p4, p5};
  c2 = newc; Circle(c2) = {p5, p6, p7};
  l3 = newl; Line(l3) = {p7, p8};
  c3 = newc; Circle(c3) = {p8, p9, p10};
  l4 = newl; Line(l4) = {p10, p11};
  l5 = newl; Line(l5) = {p11, p12};
  c4 = newc; Circle(c4) = {p12, p2, p1};

  c5 = newc; Circle(c5) = {p13, p14, p15};
  l6 = newl; Line(l6) = {p15, p16};
  l7 = newl; Line(l7) = {p16, p17};
  c6 = newc; Circle(c6) = {p17, p18, p19};
  l8 = newl; Line(l8) = {p19, p20};
  c7 = newc; Circle(c7) = {p20, p21, p22};
  l9 = newl; Line(l9) = {p22, p23};
  l10 = newl; Line(l10) = {p23, p24};
  c8 = newc; Circle(c8) = {p24, p14, p13};

  c9 = newc; Circle(c9) = {p25, p26, p27};
  l11 = newl; Line(l11) = {p27, p28};
  l12 = newl; Line(l12) = {p28, p29};
  c10 = newc; Circle(c10) = {p29, p30, p31};
  l13 = newl; Line(l13) = {p31, p32};
  c11 = newc; Circle(c11) = {p32, p33, p34};
  l14 = newl; Line(l14) = {p34, p35};
  l15 = newl; Line(l15) = {p35, p36};
  c12 = newc; Circle(c12) = {p36, p26, p25};

  l16 = newl; Line(l16) = {p1, p13};
  l17 = newl; Line(l17) = {p2, p14};
  l18 = newl; Line(l18) = {p3, p15};
  l19 = newl; Line(l19) = {p4, p16};
  l20 = newl; Line(l20) = {p5, p17};
  l21 = newl; Line(l21) = {p6, p18};
  l22 = newl; Line(l22) = {p7, p19};
  l23 = newl; Line(l23) = {p8, p20};
  l24 = newl; Line(l24) = {p9, p21};
  l25 = newl; Line(l25) = {p10, p22};
  l26 = newl; Line(l26) = {p11, p23};
  l27 = newl; Line(l27) = {p12, p24};

  l28 = newl; Line(l28) = {p13, p25};
  l29 = newl; Line(l29) = {p14, p26};
  l30 = newl; Line(l30) = {p15, p27};
  l31 = newl; Line(l31) = {p16, p28};
  l32 = newl; Line(l32) = {p17, p29};
  l33 = newl; Line(l33) = {p18, p30};
  l34 = newl; Line(l34) = {p19, p31};
  l35 = newl; Line(l35) = {p20, p32};
  l36 = newl; Line(l36) = {p21, p33};
  l37 = newl; Line(l37) = {p22, p34};
  l38 = newl; Line(l38) = {p23, p35};
  l39 = newl; Line(l39) = {p24, p36};


  // Line Loops
  ll1 = newll; Line Loop(ll1) = {c1, l1, l2, c2, l3, c3, l4, l5, c4};
  ll2 = newll; Line Loop(ll2) = {c1, l18, -c5, -l16};
  ll3 = newll; Line Loop(ll3) = {l1, l19, -l6, -l18};
  ll4 = newll; Line Loop(ll4) = {l2, l20, -l7, -l19};
  ll5 = newll; Line Loop(ll5) = {c2, l22, -c6, -l20};
  ll6 = newll; Line Loop(ll6) = {l3, l23, -l8, -l22};
  ll7 = newll; Line Loop(ll7) = {c3, l25, -c7, -l23};
  ll8 = newll; Line Loop(ll8) = {l4, l26, -l9, -l25};
  ll9 = newll; Line Loop(ll9) = {l5, l27, -l10, -l26};
  ll10 = newll; Line Loop(ll10) = {c4, l16, -c8, -l27};

  ll11 = newll; Line Loop(ll11) = {c5, l30, -c9, -l28};
  ll12 = newll; Line Loop(ll12) = {l6, l31, -l11, -l30};
  ll13 = newll; Line Loop(ll13) = {l7, l32, -l12, -l31};
  ll14 = newll; Line Loop(ll14) = {c6, l34, -c10, -l32};
  ll15 = newll; Line Loop(ll15) = {l8, l35, -l13, -l34};
  ll16 = newll; Line Loop(ll16) = {c7, l37, -c11, -l35};
  ll17 = newll; Line Loop(ll17) = {l9, l38, -l14, -l37};
  ll18 = newll; Line Loop(ll18) = {l10, l39, -l15, -l38};
  ll19 = newll; Line Loop(ll19) = {c8, l28, -c12, -l39};
  ll20 = newll; Line Loop(ll20) = {c9, l11, l12, c10, l13, c11, l14, l15, c12};


  // Surfaces
  s1 = news; Plane Surface(s1) = {ll1};
  s2 = news; Ruled Surface(s2) = {ll2};
  s3 = news; Plane Surface(s3) = {ll3};
  s4 = news; Plane Surface(s4) = {ll4};
  s5 = news; Ruled Surface(s5) = {ll5};
  s6 = news; Plane Surface(s6) = {ll6};
  s7 = news; Ruled Surface(s7) = {ll7};
  s8 = news; Plane Surface(s8) = {ll8};
  s9 = news; Plane Surface(s9) = {ll9};
  s10 = news; Ruled Surface(s10) = {ll10};

  s11 = news; Ruled Surface(s11) = {ll11};
  s12 = news; Plane Surface(s12) = {ll12};
  s13 = news; Plane Surface(s13) = {ll13};
  s14 = news; Ruled Surface(s14) = {ll14};
  s15 = news; Plane Surface(s15) = {ll15};
  s16 = news; Ruled Surface(s16) = {ll16};
  s17 = news; Plane Surface(s17) = {ll17};
  s18 = news; Plane Surface(s18) = {ll18};
  s19 = news; Ruled Surface(s19) = {ll19};
  s20 = news; Plane Surface(s20) = {ll20};

  sl = newsl;
  Surface Loop(sl) = {s1, s2, s3, s4, s5, s6, s7, s8, s9, s10,
                       s11, s12, s13, s14, s15,s16, s17, s18, s19, s20};

  v = newv; Volume(v) = {sl};


  Return
