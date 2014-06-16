/*
 * (C) Gražvydas "notaz" Ignotas, 2014
 *
 * This work is licensed under the terms of GNU GPL version 2 or later.
 * See the COPYING file in the top-level directory.
 */

#include "arm_features.h"

.syntax unified
.text
.align 3

.macro do_mac_flags rr1 rr2 rr3
    cmp         \rr1, #1
.endm


/*
 * ProcessVertexData register map:
 *
 *  q | d | c code
 *  0   0   
 *      1   
 *  1   2   
 *      3   
 *  2   4   
 *      5   
 *  3   6   
 *      7   
 *  4   8   
 *      9   
 *  5  10   
 *     11   
 *  6  12   
 *     13   
 *  7  14   g_vecProjected
 *     15   
 *  8  16   
 *     17   
 * ...      
 * 12  24   gRSPworldProject _11,_21,_31,_41
 *     25   
 * 13  26   gRSPworldProject _12,_22,_32,_42
 *     27   
 * 14  28   gRSPworldProject _13,_23,_33,_43
 *     29   
 * 15  30   gRSPworldProject _14,_24,_34,_44
 *     31   
 *
 * r4 vtx[], 16 bytes:
 * short y, x, flag, z, tv, tu;
 * / uint8 a, b, g, r;
 * \ char  a, z, y, x;
 *
 *  outputs:
 * r0        - XVECTOR4 *g_vtxTransformed
 * r1        - XVECTOR4 *g_vecProjected
 * r2        - uint32   *g_dwVtxDifColor
 * r3        - VECTOR2  *g_fVtxTxtCoords
 *     sp+00 - float    *g_fFogCoord
 *     sp+04 - uint32   *g_clipFlag2
 *  inputs:
 * r11 sp+08 - uint32      dwNum
 *     sp+0c - int         neon_flags
 * r4  sp+10 - FiddledVtx  vtx[]
 *     sp+14 - Light      *gRSPlights
 *     sp+18 - float      *fRSPAmbientLightRGBA
 *     sp+1c - XMATRIX    *gRSPworldProjectTransported
 *     sp+20 - XMATRIX    *gRSPmodelViewTop
 *     sp+24 - uint32      gRSPnumLights
 *     sp+28 - float       gRSPfFogMin
 */
FUNCTION(pv_neon): @ 
    ldr         r12, [sp, #0x10]
    pld         [r12]

    push        {r4-r11,lr}
    vpush       {q4-q7}

    mov         r4, r12               @ vtx
    ldr         r12, [sp, #0x64+0x1c]
    ldr		r11, [sp, #0x64+0x08]
    vld1.32     {q12,q13}, [r12, :128]! @ load gRSPworldProject
    vld1.32     {q14,q15}, [r12, :128]

0:
    vld1.16     d4[1], [r4]!          @ y
    vld1.16     d4[0], [r4]!          @ x
    vld1.16     d4[3], [r4]!          @ flag
    vld1.16     d4[2], [r4]!          @ z
    vld1.16     d5[1], [r4]!          @ v
    vld1.16     d5[0], [r4]!          @ u
    vmovl.s16   q0, d4
    vmovl.s16   q1, d5
    mov         r12, #0x3f800000      @ 1.0f
    vcvt.f32.s32 q2, q0               @ q2 = vtx_raw
    vcvt.f32.s32 q3, q1               @ d6 = float u, v
    vmov.32     d5[1], r12            @ q2 = { x, y, x, 1.0f }
    vmul.f32    q4, q2, q12
    vmul.f32    q5, q2, q13
    vmul.f32    q6, q2, q14
    vmul.f32    q7, q2, q15
    /* wrt u,v to g_fVtxTxtCoords */  vst1.32     {d6}, [r3]!
    vpadd.f32   d7, d14, d15
    vpadd.f32   d6, d12, d13
    vpadd.f32   d5, d10, d11
    vpadd.f32   d4, d8, d9
    vpadd.f32   d1, d6, d7            @ g_vtxTransformed .z .w
    vpadd.f32   d0, d4, d5            @ g_vtxTransformed .x .y

#if 1
    vrecpe.f32  d2, d1                @ inv .z(unused) .w
    vrecps.f32  d3, d1, d2            @ step
    /* wrt g_vtxTransformed */        vst1.32     {q0}, [r0]!
    vmul.f32    d2, d3, d2            @ better inv
    vrecps.f32  d3, d1, d2            @ step
    vmul.f32    d2, d3, d2            @ better inv
 #if 0
    vrecps.f32  d3, d1, d2            @ step
    vmul.f32    d2, d3, d2            @ better inv
 #endif
#else
    vmov.f32    s5, r12
    /* wrt g_vtxTransformed */        vst1.32     {q0}, [r0]!
    vdiv.f32    s5, s5, s3
#endif

    vmul.f32    q7, q0, d2[1]
    vshr.u64    d2, #32
    vsli.64     d15, d2, #32
    /* wrt g_vecProjected */          vst1.32     {q7}, [r1]!

    vpop        {q4-q7}
    pop         {r4-r11,pc}
    .size	pv_neon, .-pv_neon
 

@ vim:filetype=armasm
