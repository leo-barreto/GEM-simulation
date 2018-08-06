

// Parameters in mm
RADIUS = 0.035;
DISTANCE_HOLES = 0.140;
RADIUS_DIE = 0.010;
THICKNESS_DIE = 0.05;
THICKNESS_PLA = 0.005;
DISTANCE_PAD = 1.;
DISTANCE_ELE = 1.;

Function gf_gem
  H = Sqrt(3) * DISTANCE_HOLES / 2;
  
  // Characteristic Distances
  lc_D = DISTANCE_HOLES / 15;
  lc_P = DISTANCE_HOLES / 15;
  lc_M = (DISTANCE_ELE + DISTANCE_PAD) / 10;


  // Defining Points
  // Superior Chamber
  x0 = -DISTANCE_HOLES / 4; y0 = 0;
  z0 = DISTANCE_ELE / 2 + THICKNESS_PLA;
  h = DISTANCE_ELE; w = H; l = DISTANCE_HOLES / 2;
  lc = lc_M;

  // Points
  Point(1) = {x0-l/2, y0-w/2, z0-h/2, lc};
  Point(2) = {x0-l/2, y0-w/2, z0+h/2, lc};
  Point(3) = {x0-l/2, y0+w/2, z0-h/2, lc};
  Point(4) = {x0-l/2, y0+w/2, z0+h/2, lc};
  Point(5) = {x0+l/2, y0-w/2, z0-h/2, lc};
  Point(6) = {x0+l/2, y0-w/2, z0+h/2, lc};
  Point(7) = {x0+l/2, y0+w/2, z0-h/2, lc};
  Point(8) = {x0+l/2, y0+w/2, z0+h/2, lc};


  // Upper Plate
  x0 = 0; y0 = 0; z0 = 0;
  R = RADIUS; D = DISTANCE_HOLES; R_M = R; T = THICKNESS_PLA;
  z0 = THICKNESS_PLA;
  lc = lc_P;

  p1 = newp; Point(p1) = {x0, y0 - R + H / 2, z0, lc};
  p3 = newp; Point(p3) = {x0 - R, y0 + H / 2, z0, lc};
  p4 = newp; Point(p4) = {x0 - D / 2, y0 + H / 2, z0, lc};
  p5 = newp; Point(p5) = {x0 - D / 2, y0 - H / 2+ R, z0, lc};
  p7 = newp; Point(p7) = {x0 - D / 2 + R, y0 - H / 2, z0, lc};
  p8 = newp; Point(p8) = {x0, y0 - H / 2, z0, lc};

  p17 = newp; Point(p17) = {x0, y0 - R + H / 2, z0 - T, lc};
  p18 = newp; Point(p18) = {x0, y0 + H / 2, z0 - T, lc};
  p19 = newp; Point(p19) = {x0 - R, y0 + H / 2, z0 - T, lc};
  p20 = newp; Point(p20) = {x0 - D / 2, y0 + H / 2, z0 - T, lc};
  p21 = newp; Point(p21) = {x0 - D / 2, y0 - H / 2+ R, z0 - T, lc};
  p22 = newp; Point(p22) = {x0 - D / 2, y0 - H / 2, z0 - T, lc};
  p23 = newp; Point(p23) = {x0 - D / 2 + R, y0 - H / 2, z0 - T, lc};
  p24 = newp; Point(p24) = {x0, y0 - H / 2, z0 - T, lc};


  // Dieletric
  x0 = 0; y0 = 0; z0 = 0;
  R = RADIUS; D = DISTANCE_HOLES; R_M = R - RADIUS_DIE; T = THICKNESS_DIE;
  lc = lc_D;

  p9 = newp; Point(p9) = {x0, y0 - R_M + H / 2, z0 - T / 2, lc};
  p10 = newp; Point(p10) = {x0, y0 + H / 2, z0 - T / 2, lc};
  p11 = newp; Point(p11) = {x0 - R_M, y0 + H / 2, z0 - T / 2, lc};
  p12 = newp; Point(p12) = {x0 - D / 2, y0 + H / 2, z0 - T / 2, lc};
  p13 = newp; Point(p13) = {x0 - D / 2, y0 - H / 2+ R_M, z0 - T / 2, lc};
  p14 = newp; Point(p14) = {x0 - D / 2, y0 - H / 2, z0 - T / 2, lc};
  p15 = newp; Point(p15) = {x0 - D / 2 + R_M, y0 - H / 2, z0 - T / 2, lc};
  p16 = newp; Point(p16) = {x0, y0 - H / 2, z0 - T / 2, lc};

  p17 = newp; Point(p17) = {x0, y0 - R + H / 2, z0 - T, lc};
  p18 = newp; Point(p18) = {x0, y0 + H / 2, z0 - T, lc};
  p19 = newp; Point(p19) = {x0 - R, y0 + H / 2, z0 - T, lc};
  p20 = newp; Point(p20) = {x0 - D / 2, y0 + H / 2, z0 - T, lc};
  p21 = newp; Point(p21) = {x0 - D / 2, y0 - H / 2+ R, z0 - T, lc};
  p22 = newp; Point(p22) = {x0 - D / 2, y0 - H / 2, z0 - T, lc};
  p23 = newp; Point(p23) = {x0 - D / 2 + R, y0 - H / 2, z0 - T, lc};
  p24 = newp; Point(p24) = {x0, y0 - H / 2, z0 - T, lc};


  // Lower Plate
  R = RADIUS; D = DISTANCE_HOLES; R_M = R; T = THICKNESS_PLA;
  z0 = -THICKNESS_DIE;
  lc = lc_P;

  p17 = newp; Point(p17) = {x0, y0 - R + H / 2, z0 - T, lc};
  p18 = newp; Point(p18) = {x0, y0 + H / 2, z0 - T, lc};
  p19 = newp; Point(p19) = {x0 - R, y0 + H / 2, z0 - T, lc};
  p20 = newp; Point(p20) = {x0 - D / 2, y0 + H / 2, z0 - T, lc};
  p21 = newp; Point(p21) = {x0 - D / 2, y0 - H / 2+ R, z0 - T, lc};
  p22 = newp; Point(p22) = {x0 - D / 2, y0 - H / 2, z0 - T, lc};
  p23 = newp; Point(p23) = {x0 - D / 2 + R, y0 - H / 2, z0 - T, lc};
  p24 = newp; Point(p24) = {x0, y0 - H / 2, z0 - T, lc};


  // Inferior Chamber
  x0 = -DISTANCE_HOLES / 4; y0 = 0;
  z0 = -DISTANCE_PAD / 2 - THICKNESS_PLA - THICKNESS_DIE;
  h = DISTANCE_PAD; w = H; l = DISTANCE_HOLES / 2;
  lc = lc_M;

  p1 = newp; Point(p1) = {x0-l/2, y0-w/2, z0-h/2, lc};
  p3 = newp; Point(p3) = {x0-l/2, y0+w/2, z0-h/2, lc};
  p5 = newp; Point(p5) = {x0+l/2, y0-w/2, z0-h/2, lc};
  p7 = newp; Point(p7) = {x0+l/2, y0+w/2, z0-h/2, lc};


  Line(1) = {2, 4};
  Line(2) = {4, 8};
  Line(3) = {8, 6};
  Line(4) = {6, 2};
  Line(5) = {8, 7};
  Line(6) = {4, 3};
  Line(7) = {2, 1};
  Line(8) = {6, 5};
  Line(9) = {3, 12};
  Line(10) = {12, 1};
  Line(11) = {1, 13};
  Line(12) = {13, 5};
  Line(13) = {5, 9};
  Line(14) = {9, 7};
  Line(15) = {7, 10};
  Line(16) = {10, 3};
  Line(17) = {5, 22};
  Line(18) = {9, 15};
  Line(19) = {7, 16};
  Line(20) = {10, 17};
  Line(21) = {3, 18};
  Line(22) = {12, 19};
  Line(23) = {1, 20};
  Line(24) = {13, 21};
  Line(25) = {22, 15};
  Line(26) = {15, 16};
  Line(27) = {16, 17};
  Line(28) = {17, 18};
  Line(29) = {18, 19};
  Line(30) = {19, 20};
  Line(31) = {20, 21};
  Line(32) = {21, 22};
  Circle(33) = {9, 7, 10};
  Circle(34) = {12, 1, 13};
  Circle(35) = {15, 16, 17};
  Circle(36) = {19, 20, 21};
  Line(37) = {22, 30};
  Line(38) = {16, 24};
  Line(39) = {18, 26};
  Line(40) = {20, 28};
  Line(41) = {30, 23};
  Line(42) = {23, 24};
  Line(43) = {24, 25};
  Line(44) = {25, 26};
  Line(45) = {26, 27};
  Line(46) = {27, 28};
  Line(47) = {28, 29};
  Line(48) = {29, 30};
  Line(49) = {24, 32};
  Line(50) = {32, 33};
  Line(51) = {33, 34};
  Line(52) = {34, 35};
  Line(53) = {35, 36};
  Line(54) = {36, 37};
  Line(55) = {37, 38};
  Line(56) = {38, 31};
  Line(57) = {31, 32};
  Line(58) = {26, 34};
  Line(59) = {28, 36};
  Line(60) = {30, 38};
  Line(61) = {19, 27};
  Line(62) = {27, 35};
  Line(63) = {21, 29};
  Line(64) = {29, 37};
  Line(65) = {15, 23};
  Line(66) = {23, 31};
  Line(67) = {17, 25};
  Line(68) = {25, 33};
  Circle(69) = {27, 28, 29};
  Circle(70) = {35, 36, 37};
  Circle(71) = {23, 24, 25};
  Circle(72) = {31, 32, 33};
  Line(73) = {34, 42};
  Line(74) = {35, 43};
  Line(75) = {36, 44};
  Line(76) = {37, 45};
  Line(77) = {38, 46};
  Line(78) = {31, 39};
  Line(79) = {32, 40};
  Line(80) = {33, 41};
  Line(81) = {42, 43};
  Line(82) = {43, 44};
  Line(83) = {44, 45};
  Line(84) = {45, 46};
  Line(85) = {46, 39};
  Line(86) = {39, 40};
  Line(87) = {40, 41};
  Line(88) = {41, 42};
  Circle(89) = {43, 44, 45};
  Circle(90) = {39, 40, 41};
  Line(91) = {44, 47};
  Line(92) = {42, 48};
  Line(93) = {46, 49};
  Line(94) = {40, 50};
  Line(95) = {48, 47};
  Line(96) = {47, 49};
  Line(97) = {49, 50};
  Line(98) = {50, 48};



  // Surfaces
  Line Loop(99) = {1, 2, 3, 4};
  Plane Surface(100) = {99};
  Line Loop(101) = {1, 6, 9, 10, -7};
  Plane Surface(102) = {101};
  Line Loop(103) = {2, 5, 15, 16, -6};
  Plane Surface(104) = {103};
  Line Loop(105) = {4, 7, 11, 12, -8};
  Plane Surface(106) = {105};
  Line Loop(107) = {3, 8, 13, 14, -5};
  Plane Surface(108) = {107};
  Line Loop(109) = {9, 34, 12, 13, 33, 16};
  Plane Surface(110) = {109};
  Line Loop(111) = {28, 29, 36, 32, 25, 35};
  Plane Surface(112) = {111};
  Line Loop(113) = {16, 21, -28, -20};
  Plane Surface(114) = {113};
  Line Loop(115) = {9, 22, -29, -21};
  Plane Surface(116) = {115};
  Line Loop(117) = {12, 17, -32, -24};
  Plane Surface(118) = {117};
  Line Loop(119) = {25, -18, -13, 17};
  Plane Surface(120) = {119};
  Line Loop(121) = {34, 24, -36, -22};
  Ruled Surface(122) = {121};
  Line Loop(123) = {20, -35, -18, 33};
  Ruled Surface(124) = {123};
  Line Loop(125) = {10, 23, -30, -22};
  Ruled Surface(126) = {125};
  Line Loop(127) = {11, 24, -31, -23};
  Ruled Surface(128) = {127};
  Line Loop(129) = {18, 26, -19, -14};
  Ruled Surface(130) = {129};
  Line Loop(131) = {15, 20, -27, -19};
  Ruled Surface(132) = {131};
  Line Loop(133) = {29, 61, -45, -39};
  Ruled Surface(134) = {133};
  Line Loop(135) = {30, 40, -46, -61};
  Ruled Surface(136) = {135};
  Line Loop(137) = {47, -63, -31, 40};
  Ruled Surface(138) = {137};
  Line Loop(139) = {48, -37, -32, 63};
  Ruled Surface(140) = {139};
  Line Loop(141) = {41, -65, -25, 37};
  Ruled Surface(142) = {141};
  Line Loop(143) = {65, 42, -38, -26};
  Ruled Surface(144) = {143};
  Line Loop(145) = {27, 67, -43, -38};
  Ruled Surface(146) = {145};
  Line Loop(147) = {35, 67, -71, -65};
  Ruled Surface(148) = {147};
  Line Loop(149) = {36, 63, -69, -61};
  Ruled Surface(150) = {149};
  Line Loop(151) = {45, 62, -52, -58};
  Plane Surface(152) = {151};
  Line Loop(153) = {46, 59, -53, -62};
  Plane Surface(154) = {153};
  Line Loop(155) = {59, 54, -64, -47};
  Plane Surface(156) = {155};
  Line Loop(157) = {64, 55, -60, -48};
  Plane Surface(158) = {157};
  Line Loop(159) = {41, 66, -56, -60};
  Plane Surface(160) = {159};
  Line Loop(161) = {42, 49, -57, -66};
  Plane Surface(162) = {161};
  Line Loop(163) = {43, 68, -50, -49};
  Plane Surface(164) = {163};
  Line Loop(165) = {44, 58, -51, -68};
  Plane Surface(166) = {165};
  Line Loop(167) = {44, -39, -28, 67};
  Plane Surface(168) = {167};
  Line Loop(169) = {69, 64, -70, -62};
  Ruled Surface(170) = {169};
  Line Loop(171) = {71, 68, -72, -66};
  Ruled Surface(172) = {171};
  Line Loop(173) = {52, 74, -81, -73};
  Plane Surface(174) = {173};
  Line Loop(175) = {74, 82, -75, -53};
  Plane Surface(176) = {175};
  Line Loop(177) = {75, 83, -76, -54};
  Plane Surface(178) = {177};
  Line Loop(179) = {55, 77, -84, -76};
  Plane Surface(180) = {179};
  Line Loop(181) = {56, 78, -85, -77};
  Plane Surface(182) = {181};
  Line Loop(183) = {86, -79, -57, 78};
  Plane Surface(184) = {183};
  Line Loop(185) = {50, 80, -87, -79};
  Plane Surface(186) = {185};
  Line Loop(187) = {51, 73, -88, -80};
  Plane Surface(188) = {187};
  Line Loop(189) = {74, 89, -76, -70};
  Ruled Surface(190) = {189};
  Line Loop(191) = {72, 80, -90, -78};
  Ruled Surface(192) = {191};
  Line Loop(193) = {56, 72, 51, 52, 70, 55};
  Plane Surface(194) = {193};
  Line Loop(195) = {85, 90, 88, 81, 89, 84};
  Plane Surface(196) = {195};
  Line Loop(197) = {91, 96, -93, -84, -83};
  Plane Surface(198) = {197};
  Line Loop(199) = {91, -95, -92, 81, 82};
  Plane Surface(200) = {199};
  Line Loop(201) = {93, 97, -94, -86, -85};
  Plane Surface(202) = {201};
  Line Loop(203) = {87, 88, 92, -98, -94};
  Plane Surface(204) = {203};
  Line Loop(205) = {95, 96, 97, 98};
  Plane Surface(206) = {205};



  // Volumes
  // Gas
  Surface Loop(207) = {100, 102, 104, 108, 106, 128, 138, 156, 154, 136, 126, 176, 200, 198, 206, 202, 204, 186, 164, 146, 132, 130, 144, 162, 184, 178, 110, 122, 124, 148, 172, 170, 150, 190, 196, 192};
  Volume(208) = {207};

  // Dieletric
  Surface Loop(209) = {140, 158, 160, 142, 112, 148, 172, 168, 166, 152, 134, 150, 170, 194};
  Volume(210) = {209};

  // Upper Plate
  Surface Loop(211) = {110, 122, 124, 114, 116, 118, 120, 112};
  Volume(212) = {211};

  // Lower Plate
  Surface Loop(213) = {190, 196, 192, 182, 180, 174, 188, 194};
  Volume(214) = {213};



  // Boudanries
  Physical Surface(215) = {102, 116, 126, 134, 136, 152, 154, 174, 176, 200}; // -X
  Physical Surface(216) = {106, 128, 118, 138, 140, 156, 158, 178, 180, 198}; // -Y
  Physical Surface(217) = {108, 130, 120, 142, 144, 160, 162, 182, 184, 202}; // +X
  Physical Surface(218) = {104, 132, 114, 168, 146, 166, 164, 188, 186, 204}; // +Y



  // Physical Surfaces (Potential)
  Physical Surface(219) = {100}; // Electrode
  Physical Surface(220) = {110}; // Upper Plate
  Physical Surface(221) = {196}; // Lower Plate
  Physical Surface(222) = {206}; // Pad



  // Physical Volumes
  Physical Volume(223) = {208};  // Gas
  Physical Volume(224) = {210};  // Dieletric
  Physical Volume(225) = {212};  // Upper Plate
  Physical Volume(226) = {214};  // Lower Plate


  Return
