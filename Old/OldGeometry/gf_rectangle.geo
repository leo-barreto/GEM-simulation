// Default parameters
x0 = 0;
y0 = 0;
z0 = 0;
l = 1;
w = 1;
h = 1;
lc = 0.3;
V_cond = 1;

Function gf_rectangle


  // Points
  p1 = newp; Point(p1) = {x0-l/2, y0-w/2, z0-h/2, lc};
  p2 = newp; Point(p2) = {x0-l/2, y0-w/2, z0+h/2, lc};
  p3 = newp; Point(p3) = {x0-l/2, y0+w/2, z0-h/2, lc};
  p4 = newp; Point(p4) = {x0-l/2, y0+w/2, z0+h/2, lc};
  p5 = newp; Point(p5) = {x0+l/2, y0-w/2, z0-h/2, lc};
  p6 = newp; Point(p6) = {x0+l/2, y0-w/2, z0+h/2, lc};
  p7 = newp; Point(p7) = {x0+l/2, y0+w/2, z0-h/2, lc};
  p8 = newp; Point(p8) = {x0+l/2, y0+w/2, z0+h/2, lc};


  // Lines
  l1 = newl; Line(l1) = {p1,p2};
  l2 = newl; Line(l2) = {p2,p4};
  l3 = newl; Line(l3) = {p4,p3};
  l4 = newl; Line(l4) = {p3,p1};
  l5 = newl; Line(l5) = {p5,p6};
  l6 = newl; Line(l6) = {p6,p8};
  l7 = newl; Line(l7) = {p8,p7};
  l8 = newl; Line(l8) = {p7,p5};
  l9 = newl; Line(l9) = {p1,p5};
  l10 = newl; Line(l10) = {p2,p6};
  l11 = newl; Line(l11) = {p3,p7};
  l12 = newl; Line(l12) = {p4,p8};


  // Surfaces
  lp1 = newll; Line Loop(lp1) = {l1, l2, l3, l4};
  s1 = news; Plane Surface(s1) = {lp1};       // Sup. Maior 1
  lp2 = newll; Line Loop(lp2) = {l12, l7, -l11, -l3};
  s2 = news; Plane Surface(s2) = {lp2};       // Encaixe
  lp3 = newll; Line Loop(lp3) = {-l5, -l6, -l7, -l8};
  s3 = news; Plane Surface(s3) = {lp3};       // Sup. Maior 2
  lp4 = newll; Line Loop(lp4) = {-l10, l5, l9, -l1};
  s4 = news; Plane Surface(s4) = {lp4};       // Oposto ao Encaixe
  lp5 = newll; Line Loop(lp5) = {-l2, -l12, l6, l10};
  s5 = news; Plane Surface(s5) = {lp5};       // Plano Superior
  lp6 = newll; Line Loop(lp6) = {-l9, l8, l11, -l4};
  s6 = news; Plane Surface(s6) = {lp6};       // Plano Inferior


  // Volumes
  If (V_cond == 1)
    sl = newsl; Surface Loop(sl) = {s5, s1, s4, s3, s2, s6};
    vol = newv; Volume(vol) = {sl};
  EndIf


Return
