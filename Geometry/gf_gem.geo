Include "gf_gplane.geo";
Include "gf_rectangle.geo";
Include "gf_hole.geo";


// Parameters in mm
RADIUS = 0.035;
DISTANCE_HOLES = 0.140;
RADIUS_DIE = 0.010;
THICKNESS_DIE = 0.050;
THICKNESS_PLA = 0.005;
DISTANCE_PAD = 1.;
DISTANCE_ELE = 1.;

Function gf_gem

  // Characteristic Distances
  lc_D = DISTANCE_HOLES / 15;
  lc_P = DISTANCE_HOLES / 15;
  lc_M = (DISTANCE_ELE + DISTANCE_PAD) / 10;


  // Dieletric
  x0 = 0; y0 = 0; z0 = 0;
  R = RADIUS; D = DISTANCE_HOLES; R_O = RADIUS_DIE; T = THICKNESS_DIE;
  lc = lc_D;
  Call gf_gplane;
  vol_die = v;
  s_die_1 = s1; s_die_2 = s2; s_die_3 = s3; s_die_4 = s4; s_die_5 = s5;
  s_die_6 = s6; s_die_7 = s7; s_die_8 = s8; s_die_9 = s9; s_die_10 = s10;
  s_die_11 = s11; s_die_12 = s12; s_die_13 = s13; s_die_14 = s14; s_die_15 = s15;
  s_die_16 = s16; s_die_17 = s17; s_die_18 = s18; s_die_19 = s19; s_die_20 = s20;

  // Upper Plate
  R = RADIUS; D = DISTANCE_HOLES; R_O = 0; T = THICKNESS_PLA;
  z0 = THICKNESS_PLA;
  lc = lc_P;
  Call gf_gplane;
  vol_up = v;
  s_up_1 = s1; s_up_2 = s2; s_up_3 = s3; s_up_4 = s4; s_up_5 = s5;
  s_up_6 = s6; s_up_7 = s7; s_up_8 = s8; s_up_9 = s9; s_up_10 = s10;
  s_up_11 = s11; s_up_12 = s12; s_up_13 = s13; s_up_14 = s14; s_up_15 = s15;
  s_up_16 = s16; s_up_17 = s17; s_up_18 = s18; s_up_19 = s19; s_up_20 = s20;

  // Lower Plate
  R = RADIUS; D = DISTANCE_HOLES; R_O = 0; T = THICKNESS_PLA;
  z0 = -THICKNESS_DIE;
  Call gf_gplane;
  vol_lo = v;
  s_lo_1 = s1; s_lo_2 = s2; s_lo_3 = s3; s_lo_4 = s4; s_lo_5 = s5;
  s_lo_6 = s6; s_lo_7 = s7; s_lo_8 = s8; s_lo_9 = s9; s_lo_10 = s10;
  s_lo_11 = s11; s_lo_12 = s12; s_lo_13 = s13; s_lo_14 = s14; s_lo_15 = s15;
  s_lo_16 = s16; s_lo_17 = s17; s_lo_18 = s18; s_lo_19 = s19; s_lo_20 = s20;


  // Chamber
  lc = lc_M;

  x0 = -DISTANCE_HOLES / 4; y0 = 0;
  z0 = DISTANCE_ELE / 2 + THICKNESS_PLA;
  h = DISTANCE_ELE; w = H; l = DISTANCE_HOLES / 2;
  Call gf_rectangle;
  s1_up1 = s1; s2_up1 = s2; s3_up1 = s3;
  s4_up1 = s4; s5_up1 = s5; s6_up1 = s6;
  vol_upper_cha1 = vol;

  x0 = DISTANCE_HOLES / 4; y0 = 0;
  z0 = DISTANCE_ELE / 2 + THICKNESS_PLA;
  h = DISTANCE_ELE; w = H; l = DISTANCE_HOLES / 2;
  Call gf_rectangle;
  s1_up2 = s1; s2_up2 = s2; s3_up2 = s3;
  s4_up2 = s4; s5_up2 = s5; s6_up2 = s6;
  vol_upper_cha2 = vol;

  x0 = -DISTANCE_HOLES / 4; y0 = 0;
  z0 = -DISTANCE_PAD / 2 - THICKNESS_PLA - THICKNESS_DIE;
  h = DISTANCE_PAD; w = H; l = DISTANCE_HOLES / 2;
  Call gf_rectangle;
  s1_lo1 = s1; s2_lo1 = s2; s3_lo1 = s3;
  s4_lo1 = s4; s5_lo1 = s5; s6_lo1 = s6;
  vol_bottom_cha1 = vol;

  x0 = DISTANCE_HOLES / 4; y0 = 0;
  z0 = -DISTANCE_PAD / 2 - THICKNESS_PLA - THICKNESS_DIE;
  h = DISTANCE_PAD; w = H; l = DISTANCE_HOLES / 2;
  Call gf_rectangle;
  s1_lo2 = s1; s2_lo2 = s2; s3_lo2 = s3;
  s4_lo2 = s4; s5_lo2 = s5; s6_lo2 = s6;
  vol_bottom_cha2 = vol;


  // Holes
  lc = lc_D;

  // First Hole
  x0 = -DISTANCE_HOLES / 2; y0 = -H / 2;
  z0 = THICKNESS_PLA; R_O = 0; T = THICKNESS_PLA;
  Call gf_hole;
  vh1_1 = v;
  s1_h11 = s1; s2_h11 = s2; s3_h11 = s3;
  s4_h11 = s4; s5_h11 = s5; s6_h11 = s6;

  z0 = 0; R_O = RADIUS_DIE; T = THICKNESS_DIE;
  Call gf_hole;
  vh1_2 = v;
  s1_h12 = s1; s2_h12 = s2; s3_h12 = s3;
  s4_h12 = s4; s5_h12 = s5; s6_h12 = s6;

  z0 = -THICKNESS_DIE; R_O = 0; T = THICKNESS_PLA;
  Call gf_hole;
  vh1_3 = v;
  s1_h13 = s1; s2_h13 = s2; s3_h13 = s3;
  s4_h13 = s4; s5_h13 = s5; s6_h13 = s6;

  // Second Hole
  x0 = 0; y0 = H / 2; A = Pi;
  z0 = THICKNESS_PLA; R_O = 0; T = THICKNESS_PLA;
  Call gf_hole;
  vh2_1 = v;
  s1_h21 = s1; s2_h21 = s2; s3_h21 = s3;
  s4_h21 = s4; s5_h21 = s5; s6_h21 = s6;

  z0 = 0; R_O = RADIUS_DIE; T = THICKNESS_DIE;
  Call gf_hole;
  vh2_2 = v;
  s1_h22 = s1; s2_h22 = s2; s3_h22 = s3;
  s4_h22 = s4; s5_h22 = s5; s6_h22 = s6;

  z0 = -THICKNESS_DIE; R_O = 0; T = THICKNESS_PLA;
  Call gf_hole;
  vh2_3 = v;
  s1_h23 = s1; s2_h23 = s2; s3_h23 = s3;
  s4_h23 = s4; s5_h23 = s5; s6_h23 = s6;

  // Third Hole
  x0 = 0; y0 = H / 2; A =  -Pi / 2;
  z0 = THICKNESS_PLA; R_O = 0; T = THICKNESS_PLA;
  Call gf_hole;
  vh3_1 = v;
  s1_h31 = s1; s2_h31 = s2; s3_h31 = s3;
  s4_h31 = s4; s5_h31 = s5; s6_h31 = s6;

  z0 = 0; R_O = RADIUS_DIE; T = THICKNESS_DIE;
  Call gf_hole;
  vh3_2 = v;
  s1_h32 = s1; s2_h32 = s2; s3_h32 = s3;
  s4_h32 = s4; s5_h32 = s5; s6_h32 = s6;

  z0 = -THICKNESS_DIE; R_O = 0; T = THICKNESS_PLA;
  Call gf_hole;
  vh3_3 = v;
  s1_h33 = s1; s2_h33 = s2; s3_h33 = s3;
  s4_h33 = s4; s5_h33 = s5; s6_h33 = s6;

  // Forth Hole
  x0 = DISTANCE_HOLES / 2; y0 = -H / 2; A = Pi / 2;
  z0 = THICKNESS_PLA; R_O = 0; T = THICKNESS_PLA;
  Call gf_hole;
  vh4_1 = v;
  s1_h41 = s1; s2_h41 = s2; s3_h41 = s3;
  s4_h41 = s4; s5_h41 = s5; s6_h41 = s6;

  z0 = 0; R_O = RADIUS_DIE; T = THICKNESS_DIE;
  Call gf_hole;
  vh4_2 = v;
  s1_h42 = s1; s2_h42 = s2; s3_h42 = s3;
  s4_h42 = s4; s5_h42 = s5; s6_h42 = s6;

  z0 = -THICKNESS_DIE; R_O = 0; T = THICKNESS_PLA;
  Call gf_hole;
  vh4_3 = v;
  s1_h43 = s1; s2_h43 = s2; s3_h43 = s3;
  s4_h43 = s4; s5_h43 = s5; s6_h43 = s6;


  // Boudanry
  b1 = newreg; Physical Surface(b1) = {s1_up1, s6_h11, s6_h12, s6_h13,
                                       s_up_4, s_up_13, s_die_4, s_die_13,
                                       s_lo_4, s_lo_13, s1_lo1};

  b2 = newreg; Physical Surface(b2) = {s2_up1, s5_h21, s5_h22, s5_h23,
                                       s_up_3, s_up_12, s_die_3, s_die_12,
                                       s_lo_3, s_lo_12, s2_lo1};

  b3 = newreg; Physical Surface(b3) = {s4_up1, s5_h11, s5_h12, s5_h13, s4_lo1};

  b4 = newreg; Physical Surface(b4) = {s2_up2, s6_h31, s6_h32, s6_h33,
                                       s_up_9, s_up_18, s_die_9, s_die_18,
                                       s_lo_9, s_lo_18, s2_lo2};

  b5 = newreg; Physical Surface(b5) = {s3_up2, s5_h41, s5_h42, s5_h43,
                                       s_up_8, s_up_17, s_die_8, s_die_17,
                                       s_lo_8, s_lo_17, s3_lo2};

  b6 = newreg; Physical Surface(b6) = {s4_up2, s6_h41, s6_h42, s6_h43, s4_lo2};



  // Physical Surfaces
  up_pla = newreg; Physical Surface(up_pla) = {s_up_1};

  lo_pla = newreg; Physical Surface(lo_pla) = {s_lo_20};


  ele = newreg; Physical Surface(ele) = {s5_up1, s5_up2};
  pad = newreg; Physical Surface(pad) = {s6_lo1, s6_lo2};


  // Physical Volumes
  v_gas = newreg; Physical Volume(v_gas) = {vol_upper_cha1, vol_upper_cha2,
                                            vh1_1, vh1_2, vh1_3,
                                            vh2_1, vh2_2, vh2_3,
                                            vh3_1, vh3_2, vh3_3,
                                            vh4_1, vh4_2, vh4_3,
                                            vol_bottom_cha2, vol_bottom_cha1};
  v_die = newreg; Physical Volume(v_die) = {vol_die};
  v_up = newreg; Physical Volume(v_up) = {vol_up};
  v_lo = newreg; Physical Volume(v_lo) = {vol_lo};

  Return
