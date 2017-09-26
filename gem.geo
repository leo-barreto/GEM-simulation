Include "gf_gplane.geo";
Include "gf_rectangle.geo";
Include "gf_hole.geo";


// Parameters Sipaj in mm
RADIUS = 0.035;
DISTANCE_HOLES = 0.140;
RADIUS_DIE = 0.010;
THICKNESS_DIE = 0.050;
THICKNESS_PLA = 0.005;
DISTANCE_PAD = 1.;
DISTANCE_ELE = 1.;

// Characteristic Distances
lc_D = DISTANCE_HOLES / 20;
lc_P = DISTANCE_HOLES / 20;
lc_M = (DISTANCE_ELE + DISTANCE_PAD) / 100;


// Dieletric
x0 = 0; y0 = 0; z0 = 0;
R = RADIUS; D = DISTANCE_HOLES; R_O = RADIUS_DIE; T = THICKNESS_DIE;
lc = lc_D;
Call gf_gplane;
vol_die = v;

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
x0 = -DISTANCE_HOLES / 4; y0 = 0;
z0 = (DISTANCE_ELE - DISTANCE_PAD - THICKNESS_DIE) / 2;
h = DISTANCE_PAD + DISTANCE_ELE + THICKNESS_DIE + 2 * THICKNESS_PLA;
w = H; l = DISTANCE_HOLES / 2;
lc = lc_M;
Call gf_rectangle;
s1_left = s1; s2_left = s2; s3_left = s3;
s4_left = s4; s5_left = s5; s6_left = s6;
vol_left = vol;

x0 = DISTANCE_HOLES / 4; y0 = 0;
z0 = (DISTANCE_ELE - DISTANCE_PAD - THICKNESS_DIE) / 2;
h = DISTANCE_PAD + DISTANCE_ELE + THICKNESS_DIE + 2 * THICKNESS_PLA;
w = H; l = DISTANCE_HOLES / 2;
Call gf_rectangle;
s1_right = s1;s2_right = s2; s3_right = s3;
s4_right = s4; s5_right = s5; s6_right = s6;
vol_right = vol;

x0 = 0; y0 = 0; z0 = DISTANCE_ELE / 2 + THICKNESS_PLA;
h = DISTANCE_ELE; w = H; l = DISTANCE_HOLES;
Call gf_rectangle;
vol_upper_cha = vol;

x0 = 0; y0 = 0; z0 = DISTANCE_PAD / 2 - THICKNESS_PLA - THICKNESS_DIE;
h = DISTANCE_PAD; w = H; l = DISTANCE_HOLES;
Call gf_rectangle;
vol_bottom_cha = vol;


// Holes
// First Hole
lc = lc_D;
x0 = -DISTANCE_HOLES / 2; y0 = -H / 2;
z0 = THICKNESS_PLA; R_O = 0; T = THICKNESS_PLA;
Call gf_hole;
vh1_1 = v;

z0 = 0; R_O = RADIUS_DIE; T = THICKNESS_DIE;
Call gf_hole;
vh1_2 = v;

z0 = -THICKNESS_DIE; R_O = 0; T = THICKNESS_PLA;
Call gf_hole;
vh1_3 = v;

// Second Hole
x0 = 0; y0 = H / 2; A = Pi;
z0 = THICKNESS_PLA; R_O = 0; T = THICKNESS_PLA;
Call gf_hole;
vh2_1 = v;

z0 = 0; R_O = RADIUS_DIE; T = THICKNESS_DIE;
Call gf_hole;
vh2_2 = v;

z0 = -THICKNESS_DIE; R_O = 0; T = THICKNESS_PLA;
Call gf_hole;
vh2_3 = v;

// Third Hole
x0 = 0; y0 = H / 2; A =  -Pi / 2;
z0 = THICKNESS_PLA; R_O = 0; T = THICKNESS_PLA;
Call gf_hole;
vh3_1 = v;

z0 = 0; R_O = RADIUS_DIE; T = THICKNESS_DIE;
Call gf_hole;
vh3_2 = v;

z0 = -THICKNESS_DIE; R_O = 0; T = THICKNESS_PLA;
Call gf_hole;
vh3_3 = v;

// Forth Hole
x0 = DISTANCE_HOLES / 2; y0 = -H / 2; A = Pi / 2;
z0 = THICKNESS_PLA; R_O = 0; T = THICKNESS_PLA;
Call gf_hole;
vh4_1 = v;

z0 = 0; R_O = RADIUS_DIE; T = THICKNESS_DIE;
Call gf_hole;
vh4_2 = v;

z0 = -THICKNESS_DIE; R_O = 0; T = THICKNESS_PLA;
Call gf_hole;
vh4_3 = v;


// Boudanry
b1 = newreg; Physical Surface(b1) = {s1_left};
b2 = newreg; Physical Surface(b2) = {s2_left};
b3 = newreg; Physical Surface(b3) = {s4_left};
b4 = newreg; Physical Surface(b4) = {s2_right};
b5 = newreg; Physical Surface(b5) = {s3_right};
b6 = newreg; Physical Surface(b6) = {s4_right};


// Physical Surfaces
up_pla = newreg; Physical Surface(up_pla) = {s_up_1, s_up_2, s_up_3, s_up_4,
                                             s_up_5, s_up_6, s_up_7, s_up_8,
                                             s_up_9, s_up_10, s_up_11, s_up_12,
                                             s_up_13, s_up_14, s_up_15, s_up_16,
                                             s_up_17, s_up_18, s_up_19, s_up_20};

lo_pla = newreg; Physical Surface(lo_pla) = {s_lo_1, s_lo_2, s_lo_3, s_lo_4,
                                             s_lo_5, s_lo_6, s_lo_7, s_lo_8,
                                             s_lo_9, s_lo_10, s_lo_11, s_lo_12,
                                             s_lo_13, s_lo_14, s_lo_15, s_lo_16,
                                             s_lo_17, s_lo_18, s_lo_19, s_lo_20};

ele = newreg; Physical Surface(ele) = {s5_left, s5_right};

pad = newreg; Physical Surface(pad) = {s6_left, s6_right};


// Physical Volumes
//v_gas = newreg; Physical Volume(v_gas) = {vol_right, vol_left};
v_gas = newreg; Physical Volume(v_gas) = {vol_upper_cha,
                                          vh1_1, vh1_2, vh1_3,
                                          vh2_1, vh2_2, vh2_3,
                                          vh3_1, vh3_2, vh3_3,
                                          vh4_1, vh4_2, vh4_3,
                                          vol_bottom_cha};
v_die = newreg; Physical Volume(v_die) = {vol_die};
v_up = newreg; Physical Volume(v_up) = {vol_up};
v_lo = newreg; Physical Volume(v_lo) = {vol_lo};
