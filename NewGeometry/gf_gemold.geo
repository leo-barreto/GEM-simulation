Include "gf_functions.geo";
SetFactory("OpenCASCADE");

// All parameters in mm
X = 0; Y = 0; Z = 0; // Center of GEM
DIST = 0.14;               // Distance between holes

// Copper plates
R1 = 0.035;             // Radius of upper plate
R2 = 0.035;             // Radius of lower plate
TPLA1 = 0.005;          // Thickness of upper plate
TPLA2 = 0.005;          // Thickness of lower plate

// Dieletric
NDIE = 3;                      // Number of dieletric planes
POSDIE = {0.025, 0, -0.025};   // Positions of dieletric planes
RDIE = {0.035, 0.025, 0.035};  // Radius of each dieletric plane
TDIE = POSDIE[0] - POSDIE[NDIE - 1];  // Thickness of dieletric

// Chamber
DRI = .3;                      // Size of drift region
IND = .1;                      // Size of induction region


Function gf_gem

  X0 = X; Y0 = Y;


  // Up Plate
  pmin = newp; lmin = newl;
  Z0 = TDIE / 2 + TPLA1; R = R1; D = DIST; Call gf_plane;
  Z0 = TDIE / 2; Call gf_plane;
  Call VerticalLines;
  Call Loops;
  slup = loops;
  vup = gemv;


  // Dieletric
  sldie_a = {};
  vdie_a = {};

  For j In {0 : NDIE - 2}
    pmin = newp; lmin = newl;
    Z0 = POSDIE[j]; R = RDIE[j]; //D = DIST + 2 * (R1 - R);
    Call gf_plane;
    Z0 = POSDIE[j + 1]; R = RDIE[j + 1]; //D = DIST + 2 * (R1 - R);
    Call gf_plane;
    Call VerticalLines;
    Call Loops;
    sldie_a += {sl};
    vdie_a += {gemv};
  EndFor


  // Low Plate
  pmin = newp; lmin = newl;
  Z0 = -TDIE / 2; R = R2; D = DIST + 2 * (R1 - R2); Call gf_plane;
  Z0 = -TDIE / 2 - TPLA2; Call gf_plane;
  Call VerticalLines;
  Call Loops;
  sllo = loops;
  vlo = gemv;

  // Before defining the chamber, we apply coherence so any duplicated entity
  // won't affect the generation of the volume mesh
  Coherence;

  // Chamber
  ZTOP = TDIE / 2 + TPLA1 + DRI; ZBOT = -TDIE / 2 - TPLA2 - IND;
  pC1 = newp; Point(pC1) = {X1, Y1, ZTOP, lc}; // Mid Up-Left
  pC2 = newp; Point(pC2) = {X4, Y1, ZTOP, lc}; // Mid Up-Right
  pC3 = newp; Point(pC3) = {X4, Y4, ZTOP, lc}; // Mid Down-Right
  pC4 = newp; Point(pC4) = {X1, Y4, ZTOP, lc}; // Mid Down-Left
  pC5 = newp; Point(pC5) = {X1, Y1, ZBOT, lc}; // Mid Up-Left
  pC6 = newp; Point(pC6) = {X4, Y1, ZBOT, lc}; // Mid Up-Right
  pC7 = newp; Point(pC7) = {X4, Y4, ZBOT, lc}; // Mid Down-Right
  pC8 = newp; Point(pC8) = {X1, Y4, ZBOT, lc}; // Mid Down-Left

  lC1 = newl; Line(lC1) = {pC1, pC2};
  lC2 = newl; Line(lC2) = {pC2, pC3};
  lC3 = newl; Line(lC3) = {pC3, pC4};
  lC4 = newl; Line(lC4) = {pC4, pC1};
  lC5 = newl; Line(lC5) = {pC5, pC6};
  lC6 = newl; Line(lC6) = {pC6, pC7};
  lC7 = newl; Line(lC7) = {pC7, pC8};
  lC8 = newl; Line(lC8) = {pC8, pC5};
  lC9 = newl; Line(lC9) = {pC1, pC5};
  lC10 = newl; Line(lC10) = {pC2, pC6};
  lC11 = newl; Line(lC11) = {pC3, pC7};
  lC12 = newl; Line(lC12) = {pC4, pC8};

  lp1 = newll; Line Loop(lp1) = {lC1, lC2, lC3, lC4};
  s1 = news; Plane Surface(s1) = {lp1}; // +Z
  lp2 = newll; Line Loop(lp2) = {lC5, lC6, lC7, lC8};
  s2 = news; Plane Surface(s2) = {lp2}; // -Z
  lp3 = newll; Line Loop(lp3) = {lC1, lC10, -lC5, -lC9};
  s3 = news; Plane Surface(s3) = {lp3}; // +Y
  lp4 = newll; Line Loop(lp4) = {lC2, lC11, -lC6, -lC10};
  s4 = news; Plane Surface(s4) = {lp4}; // +X
  lp5 = newll; Line Loop(lp5) = {lC3, lC12, -lC7, -lC11};
  s5 = news; Plane Surface(s5) = {lp5}; // -Y
  lp6 = newll; Line Loop(lp6) = {lC4, lC9, -lC8, -lC12};
  s6 = news; Plane Surface(s6) = {lp6}; // -X

  sl = newsl; Surface Loop(sl) = {s1, s3, s4, s5, s6, s2};
  vtotal = newv; Volume(vtotal) = {sl}; // Total volume of cell


  // Boundaries
  bc1 = newreg; Physical Surface(bc1) = {s3}; // +Y
  bc2 = newreg; Physical Surface(bc2) = {s4}; // +X
  bc3 = newreg; Physical Surface(bc3) = {s5}; // -Y
  bc4 = newreg; Physical Surface(bc4) = {s6}; // -X


  // Physical Surfaces (Potential)
  b1 = newreg; Physical Surface(b1) = {s1};   // +Z
  b2 = newreg; Physical Surface(b2) = slup[]; // Upper Plate
  b3 = newreg; Physical Surface(b3) = sllo[]; // Lower Plate
  b4 = newreg; Physical Surface(b4) = {s2};   // -Z


  // Physical Volumes
  vdie = newreg; Physical Volume(vdie) = {vdie_a[]};
  vpla1 = newreg; Physical Volume(vpla1) = {vup};
  vpla2 = newreg; Physical Volume(vpla2) = {vlo};
  // Subtract plate and dieletric volumes from total to get the gas volume
  bd = newreg; BooleanDifference(bd) = { Volume{vtotal}; Delete; }{ Volume{vup, vdie_a[], vlo}; };
  vgas = newreg; Physical Volume(vgas) = {vtotal};

Return
