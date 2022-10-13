// Copyright 2021 ETH Zurich and University of Bologna.
// Solderpad Hardware License, Version 0.51, see LICENSE for details.
// SPDX-License-Identifier: SHL-0.51
//
// Author: Matteo Perotti <mperotti@iis.ee.ethz.ch>
//         Basile Bougenot <bbougenot@student.ethz.ch>

#include "float_macros.h"
#include "vector_macros.h"

// Simple random test with similar values
void TEST_CASE1(void) {
  VSET(16, e16, m2);
  //               0.6821,  0.7749,  0.0299,  0.0299, -0.5410, -0.1865,  0.1885,
  //               0.0228, -0.5410,  0.1902, -0.9160, -0.3511, -0.9287, -0.9961,
  //               -0.6509, -0.3940
  VLOAD_16(v4, 0x3975, 0x3a33, 0x27a5, 0x27a5, 0xb854, 0xb1f8, 0x3208, 0x25d8,
           0xb854, 0x3216, 0xbb54, 0xb59e, 0xbb6e, 0xbbf8, 0xb935, 0xb64e);
  //              -0.9048,  0.1698,  0.2411,  0.2411, -0.6147,  0.9580,  0.5117,
  //              0.8330, -0.6147,  0.5591,  0.7031, -0.1556,  0.2397,  0.8154,
  //              0.1646,  0.2693
  VLOAD_16(v6, 0xbb3d, 0x316f, 0x33b7, 0x33b7, 0xb8eb, 0x3baa, 0x3818, 0x3aaa,
           0xb8eb, 0x3879, 0x39a0, 0xb0fb, 0x33ac, 0x3a86, 0x3144, 0x344f);
  //              -0.2292,  0.7578, -0.7427, -0.7427,  0.1119,  0.2939, -0.2983,
  //              -0.3608,  0.1119, -0.5562, -0.0629, -0.1968, -0.8638, -0.2700,
  //              -0.7690,  0.4216
  VLOAD_16(v2, 0xb356, 0x3a10, 0xb9f1, 0xb9f1, 0x2f29, 0x34b4, 0xb4c6, 0xb5c6,
           0x2f29, 0xb873, 0xac06, 0xb24c, 0xbae9, 0xb452, 0xba27, 0x36bf);
  asm volatile("vfmsub.vv v2, v4, v6");
  //               0.7485,  0.4175, -0.2632, -0.2632,  0.5542, -1.0127, -0.5679,
  //               -0.8413,  0.5542, -0.6650, -0.6455,  0.2247,  0.5625,
  //               -0.5464,  0.3359, -0.4355
  VCMP_U16(1, v2, 0x39fd, 0x36ae, 0xb436, 0xb436, 0x386f, 0xbc0d, 0xb88b,
           0xbabb, 0x386f, 0xb952, 0xb92a, 0x3331, 0x3880, 0xb85f, 0x3560,
           0xb6f8);

  VSET(16, e32, m2);
  //              -0.74553698, -0.16736358, -0.11869104, -0.85860848,
  //              -0.66138542, -0.68386567, -0.45389724, -0.12761629,
  //              -0.95652348,  0.71083277,  0.24187960,  0.01609672,
  //              -0.58867335, -0.55222940, -0.67417240, -0.06725668
  VLOAD_32(v4, 0xbf3edb83, 0xbe2b615c, 0xbdf3144a, 0xbf5bcdc4, 0xbf29508e,
           0xbf2f11d2, 0xbee86538, 0xbe02add8, 0xbf74deb9, 0x3f35f923,
           0x3e77af49, 0x3c83dd45, 0xbf16b34c, 0xbf0d5ee8, 0xbf2c9690,
           0xbd89bddf);
  //              -0.17500710, -0.81537211, -0.31956050,  0.22762603,
  //              0.49659184, -0.09389434,  0.05757815, -0.13087828,
  //              -0.73042232, -0.79662275, -0.96801740,  0.03017101,
  //              0.70759267, -0.35606241,  0.18037270, -0.35372722
  VLOAD_32(v6, 0xbe333510, 0xbf50bc3a, 0xbea39d6f, 0x3e6916cc, 0x3efe4149,
           0xbdc04bad, 0x3d6bd711, 0xbe0604f5, 0xbf3afcf5, 0xbf4bef78,
           0xbf77cffd, 0x3cf72932, 0x3f3524cb, 0xbeb64dd0, 0x3e38b39f,
           0xbeb51bbc);
  //               0.92876774,  0.18572871, -0.42147154, -0.79289448,
  //               0.90907055,  0.07037155,  0.07339484,  0.17415307,
  //               -0.61978233, -0.04939311,  0.56138068, -0.51601994,
  //               -0.80625385, -0.31227911,  0.91474551,  0.78424871
  VLOAD_32(v2, 0x3f6dc3b9, 0x3e3e2fab, 0xbed7cb1e, 0xbf4afb22, 0x3f68b8d9,
           0x3d901ef6, 0x3d965009, 0x3e32552f, 0xbf1eaa0e, 0xbd4a506e,
           0x3f0fb6a5, 0xbf0419e2, 0xbf4e66a7, 0xbe9fe30c, 0x3f6a2cc3,
           0x3f48c486);
  asm volatile("vfmsub.vv v2, v4, v6");
  //              -0.51742357,  0.78428787,  0.36958539,  0.45315993,
  //              -1.09783781,  0.04576965, -0.09089187,
  //              0.10865352,  1.32325864,  0.76151252,  1.10380387,
  //              -0.03847724, -0.23297250,  0.52851212, -0.79706889, 0.30098125
  VCMP_U32(2, v2, 0xbf0475df, 0x3f48c717, 0x3ebd3a4c, 0x3ee80493, 0xbf8c85f3,
           0x3d3b78f5, 0xbdba2584, 0x3dde85bc, 0x3fa9608a, 0x3f42f27c,
           0x3f8d4973, 0xbd1d9a4f, 0xbe6e9058, 0x3f074c92, 0xbf4c0cb5,
           0x3e9a1a37);
};

// Simple random test with similar values (masked)
// The numbers are the same of TEST_CASE1
void TEST_CASE2(void) {
  VSET(16, e16, m2);
  //               0.6821,  0.7749,  0.0299,  0.0299, -0.5410, -0.1865,  0.1885,
  //               0.0228,  0.7217,  0.1902, -0.9160, -0.3511, -0.9287, -0.9961,
  //               -0.6509, -0.3940
  VLOAD_16(v4, 0x3975, 0x3a33, 0x27a5, 0x27a5, 0xb854, 0xb1f8, 0x3208, 0x25d8,
           0x39c6, 0x3216, 0xbb54, 0xb59e, 0xbb6e, 0xbbf8, 0xb935, 0xb64e);
  //              -0.9048,  0.1698,  0.2411,  0.2411, -0.6147,  0.9580,  0.5117,
  //              0.8330, -0.8584,  0.5591,  0.7031, -0.1556,  0.2397,  0.8154,
  //              0.1646,  0.2693
  VLOAD_16(v6, 0xbb3d, 0x316f, 0x33b7, 0x33b7, 0xb8eb, 0x3baa, 0x3818, 0x3aaa,
           0xbade, 0x3879, 0x39a0, 0xb0fb, 0x33ac, 0x3a86, 0x3144, 0x344f);
  VLOAD_8(v0, 0xAA, 0xAA);
  //              -0.2292,  0.7578, -0.7427, -0.7427,  0.1119,  0.2939, -0.2983,
  //              -0.3608,  0.3169, -0.5562, -0.0629, -0.1968, -0.8638, -0.2700,
  //              -0.7690,  0.4216
  VLOAD_16(v2, 0xb356, 0x3a10, 0xb9f1, 0xb9f1, 0x2f29, 0x34b4, 0xb4c6, 0xb5c6,
           0x3512, 0xb873, 0xac06, 0xb24c, 0xbae9, 0xb452, 0xba27, 0x36bf);
  asm volatile("vfmsub.vv v2, v4, v6, v0.t");
  //              -0.2292,  0.4175, -0.7427, -0.2632,  0.1119, -1.0127, -0.2983,
  //              -0.8413,  0.3169, -0.6650, -0.0629,  0.2247, -0.8638, -0.5464,
  //              -0.7690, -0.4355
  VCMP_U16(3, v2, 0xb356, 0x36ae, 0xb9f1, 0xb436, 0x2f29, 0xbc0d, 0xb4c6,
           0xbabb, 0x3512, 0xb952, 0xac06, 0x3331, 0xbae9, 0xb85f, 0xba27,
           0xb6f8);

  VSET(16, e32, m2);
  //              -0.74553698, -0.16736358, -0.11869104, -0.85860848,
  //              -0.66138542, -0.68386567, -0.45389724, -0.12761629,
  //              -0.95652348,  0.71083277,  0.24187960,  0.01609672,
  //              -0.58867335, -0.55222940, -0.67417240, -0.06725668
  VLOAD_32(v4, 0xbf3edb83, 0xbe2b615c, 0xbdf3144a, 0xbf5bcdc4, 0xbf29508e,
           0xbf2f11d2, 0xbee86538, 0xbe02add8, 0xbf74deb9, 0x3f35f923,
           0x3e77af49, 0x3c83dd45, 0xbf16b34c, 0xbf0d5ee8, 0xbf2c9690,
           0xbd89bddf);
  //              -0.17500710, -0.81537211, -0.31956050,  0.22762603,
  //              0.49659184, -0.09389434,  0.05757815, -0.13087828,
  //              -0.73042232, -0.79662275, -0.96801740,  0.03017101,
  //              0.70759267, -0.35606241,  0.18037270, -0.35372722
  VLOAD_32(v6, 0xbe333510, 0xbf50bc3a, 0xbea39d6f, 0x3e6916cc, 0x3efe4149,
           0xbdc04bad, 0x3d6bd711, 0xbe0604f5, 0xbf3afcf5, 0xbf4bef78,
           0xbf77cffd, 0x3cf72932, 0x3f3524cb, 0xbeb64dd0, 0x3e38b39f,
           0xbeb51bbc);
  VLOAD_8(v0, 0xAA, 0xAA);
  //               0.92876774,  0.18572871, -0.42147154, -0.79289448,
  //               0.90907055,  0.07037155,  0.07339484,  0.17415307,
  //               -0.61978233, -0.04939311,  0.56138068, -0.51601994,
  //               -0.80625385, -0.31227911,  0.91474551,  0.78424871
  VLOAD_32(v2, 0x3f6dc3b9, 0x3e3e2fab, 0xbed7cb1e, 0xbf4afb22, 0x3f68b8d9,
           0x3d901ef6, 0x3d965009, 0x3e32552f, 0xbf1eaa0e, 0xbd4a506e,
           0x3f0fb6a5, 0xbf0419e2, 0xbf4e66a7, 0xbe9fe30c, 0x3f6a2cc3,
           0x3f48c486);
  asm volatile("vfmsub.vv v2, v4, v6, v0.t");
  //               0.92876774,  0.78428787, -0.42147154,  0.45315993,
  //               0.90907055,  0.04576965,  0.07339484,  0.10865352,
  //               -0.61978233,  0.76151252,  0.56138068, -0.03847724,
  //               -0.80625385,  0.52851212,  0.91474551,  0.30098125
  VCMP_U32(4, v2, 0x3f6dc3b9, 0x3f48c717, 0xbed7cb1e, 0x3ee80493, 0x3f68b8d9,
           0x3d3b78f5, 0x3d965009, 0x3dde85bc, 0xbf1eaa0e, 0x3f42f27c,
           0x3f0fb6a5, 0xbd1d9a4f, 0xbf4e66a7, 0x3f074c92, 0x3f6a2cc3,
           0x3e9a1a37);
};

// Simple random test with similar values (vector-scalar)
void TEST_CASE3(void) {
  VSET(16, e16, m2);
  float fscalar_16;
  //                              0.1489
  BOX_HALF_IN_FLOAT(fscalar_16, 0x30c4);
  //               0.8530, -0.3298, -0.1814, -0.2385,  0.9946,  0.6553,  0.8711,
  //               -0.6377,  0.4224, -0.1814,  0.7026,  0.2852,  0.0553, 0.7349,
  //               -0.8105,  0.0033
  VLOAD_16(v4, 0x3ad3, 0xb547, 0xb1ce, 0xb3a2, 0x3bf5, 0x393e, 0x3af8, 0xb91a,
           0x36c2, 0xb1ce, 0x399f, 0x3490, 0x2b15, 0x39e1, 0xba7c, 0x1abd);
  //              -0.2338, -0.2512,  0.0069,  0.0613, -0.1733,  0.8560, -0.2766,
  //              -0.0028, -0.1803,  0.0069,  0.7856, -0.0243, -0.1974,  0.6416,
  //              0.7109,  0.0817
  VLOAD_16(v2, 0xb37b, 0xb405, 0x1f06, 0x2bd8, 0xb18c, 0x3ad9, 0xb46d, 0x99d2,
           0xb1c5, 0x1f06, 0x3a49, 0xa639, 0xb251, 0x3922, 0x39b0, 0x2d3a);
  asm volatile("vfmsub.vf v2, %[A], v4" ::[A] "f"(fscalar_16));
  //              -0.8877,  0.2925,  0.1824,  0.2477, -1.0205, -0.5278, -0.9121,
  //              0.6372, -0.4492,  0.1824, -0.5854, -0.2888, -0.0847, -0.6392,
  //              0.9165,  0.0089
  VCMP_U16(5, v2, 0xbb1a, 0x34ae, 0x31d6, 0x33ed, 0xbc15, 0xb839, 0xbb4c,
           0x3919, 0xb730, 0x31d6, 0xb8af, 0xb49f, 0xad6c, 0xb91d, 0x3b55,
           0x208b);

  VSET(16, e32, m2);
  float fscalar_32;
  //                              -0.12857932
  BOX_FLOAT_IN_FLOAT(fscalar_32, 0xbe03aa4c);
  //               0.31050768, -0.13843875,  0.23405042, -0.30545133,
  //               -0.28880060,  0.46233574, -0.51105869, -0.11776974,
  //               -0.39969075,  0.51141965,  0.88750082, -0.22310242,
  //               0.60111052,  0.58466393, -0.14306845, -0.01826003
  VLOAD_32(v4, 0x3e9efadd, 0xbe0dc2e3, 0x3e6faaea, 0xbe9c641e, 0xbe93ddac,
           0x3eecb745, 0xbf02d4be, 0xbdf13143, 0xbecca444, 0x3f02ec66,
           0x3f633341, 0xbe6474f6, 0x3f19e261, 0x3f15ac89, 0xbe128089,
           0xbc95960e);
  //              -0.51789892,  0.77328473, -0.88433731,  0.40865302,
  //              -0.50454420,  0.30827177, -0.25503114,  0.07736996,
  //              0.20596179, -0.42633566,  0.89622146,  0.03779412, 0.50878429,
  //              0.67896879, -0.17667305,  0.06984760
  VLOAD_32(v2, 0xbf049506, 0x3f45f5fd, 0xbf6263ee, 0x3ed13af8, 0xbf0129cf,
           0x3e9dd5cc, 0xbe829371, 0x3d9e7424, 0x3e52e7a6, 0xbeda48ab,
           0x3f656ec5, 0x3d1ace01, 0x3f023fb0, 0x3f2dd0e6, 0xbe34e9c8,
           0x3d8f0c42);
  asm volatile("vfmsub.vf v2, %[A], v4" ::[A] "f"(fscalar_32));
  //              -0.24391660,  0.03901032, -0.12034293,  0.25290701,
  //              0.35367453, -0.50197309,  0.54385042,  0.10782156, 0.37320831,
  //              -0.45660171, -1.00273633,  0.21824288, -0.66652966,
  //              -0.67196524,  0.16578496,  0.00927907
  VCMP_U32(6, v2, 0xbe79c546, 0x3d1fc94a, 0xbdf6765b, 0x3e817d07, 0x3eb514d5,
           0xbf00814f, 0x3f0b39c8, 0x3ddcd18d, 0x3ebf1529, 0xbee9c7b3,
           0xbf8059aa, 0x3e5f7b10, 0xbf2aa1b0, 0xbf2c05eb, 0x3e29c388,
           0x3c18073f);
};

// Simple random test with similar values (vector-scalar) (masked)
void TEST_CASE4(void) {
  VSET(16, e16, m2);
  float fscalar_16;
  //                              0.1489
  BOX_HALF_IN_FLOAT(fscalar_16, 0x30c4);
  //                0.8530, -0.3298, -0.1814, -0.2385,  0.9946,  0.6553, 0.8711,
  //                -0.6377,  0.4224, -0.1814,  0.7026,  0.2852,  0.0553,
  //                0.7349, -0.8105,  0.0033
  VLOAD_16(v4, 0x3ad3, 0xb547, 0xb1ce, 0xb3a2, 0x3bf5, 0x393e, 0x3af8, 0xb91a,
           0x36c2, 0xb1ce, 0x399f, 0x3490, 0x2b15, 0x39e1, 0xba7c, 0x1abd);
  VLOAD_8(v0, 0xAA, 0xAA);
  //               -0.2338, -0.2512,  0.0069,  0.0613, -0.1733,  0.8560,
  //               -0.2766, -0.0028, -0.1803,  0.0069,  0.7856, -0.0243,
  //               -0.1974,  0.6416,  0.7109,  0.0817
  VLOAD_16(v2, 0xb37b, 0xb405, 0x1f06, 0x2bd8, 0xb18c, 0x3ad9, 0xb46d, 0x99d2,
           0xb1c5, 0x1f06, 0x3a49, 0xa639, 0xb251, 0x3922, 0x39b0, 0x2d3a);
  asm volatile("vfmsub.vf v2, %[A], v4, v0.t" ::[A] "f"(fscalar_16));
  //               -0.2338,  0.2925,  0.0069,  0.2477, -0.1733, -0.5278,
  //               -0.2766,  0.6372, -0.1803,  0.1824,  0.7856, -0.2888,
  //               -0.1974, -0.6392,  0.7109,  0.0089
  VCMP_U16(7, v2, 0xb37b, 0x34ae, 0x1f06, 0x33ed, 0xb18c, 0xb839, 0xb46d,
           0x3919, 0xb1c5, 0x31d6, 0x3a49, 0xb49f, 0xb251, 0xb91d, 0x39b0,
           0x208b);

  VSET(16, e32, m2);
  float fscalar_32;
  //                              -0.12857932
  BOX_FLOAT_IN_FLOAT(fscalar_32, 0xbe03aa4c);
  //                0.31050768, -0.13843875,  0.23405042, -0.30545133,
  //                -0.28880060,  0.46233574, -0.51105869, -0.11776974,
  //                -0.39969075,  0.51141965,  0.88750082, -0.22310242,
  //                0.60111052,  0.58466393, -0.14306845, -0.01826003
  VLOAD_32(v4, 0x3e9efadd, 0xbe0dc2e3, 0x3e6faaea, 0xbe9c641e, 0xbe93ddac,
           0x3eecb745, 0xbf02d4be, 0xbdf13143, 0xbecca444, 0x3f02ec66,
           0x3f633341, 0xbe6474f6, 0x3f19e261, 0x3f15ac89, 0xbe128089,
           0xbc95960e);
  VLOAD_8(v0, 0xAA, 0xAA);
  //               -0.51789892,  0.77328473, -0.88433731,  0.40865302,
  //               -0.50454420,  0.30827177, -0.25503114,  0.07736996,
  //               0.20596179, -0.42633566,  0.89622146,  0.03779412,
  //               0.50878429,  0.67896879, -0.17667305,  0.06984760
  VLOAD_32(v2, 0xbf049506, 0x3f45f5fd, 0xbf6263ee, 0x3ed13af8, 0xbf0129cf,
           0x3e9dd5cc, 0xbe829371, 0x3d9e7424, 0x3e52e7a6, 0xbeda48ab,
           0x3f656ec5, 0x3d1ace01, 0x3f023fb0, 0x3f2dd0e6, 0xbe34e9c8,
           0x3d8f0c42);
  asm volatile("vfmsub.vf v2, %[A], v4, v0.t" ::[A] "f"(fscalar_32));
  //               -0.51789892,  0.03901032, -0.88433731,  0.25290701,
  //               -0.50454420, -0.50197309, -0.25503114,  0.10782156,
  //               0.20596179, -0.45660171,  0.89622146,  0.21824288,
  //               0.50878429, -0.67196524, -0.17667305,  0.00927907
  VCMP_U32(8, v2, 0xbf049506, 0x3d1fc94a, 0xbf6263ee, 0x3e817d07, 0xbf0129cf,
           0xbf00814f, 0xbe829371, 0x3ddcd18d, 0x3e52e7a6, 0xbee9c7b3,
           0x3f656ec5, 0x3e5f7b10, 0x3f023fb0, 0xbf2c05eb, 0xbe34e9c8,
           0x3c18073f);
};

int main(void) {
  INIT_CHECK();
  enable_vec();
  enable_fp();

  TEST_CASE1();
  //TEST_CASE2();
  TEST_CASE3();
  //TEST_CASE4();

  EXIT_CHECK();
}
