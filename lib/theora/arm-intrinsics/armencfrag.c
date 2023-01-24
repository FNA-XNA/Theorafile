#include "armencfrag.h"

#ifdef OC_ARM_ASM_NEON
#include "neon_a64_compat.h"
#include "neon_transpose.h"

static inline unsigned oc_hadamard_satd_neon_8x8(int *_dc,const int16x8_t _rows[8]) {
    int16x8_t p[8], q[8];
    // in: 9 bit [-255..255]
    // Hadamard stage 1: 4x2
    p[0] = vaddq_s16(_rows[0], _rows[4]);
    p[1] = vaddq_s16(_rows[1], _rows[5]);
    p[2] = vaddq_s16(_rows[2], _rows[6]);
    p[3] = vaddq_s16(_rows[3], _rows[7]);
    p[4] = vsubq_s16(_rows[0], _rows[4]);
    p[5] = vsubq_s16(_rows[1], _rows[5]);
    p[6] = vsubq_s16(_rows[2], _rows[6]);
    p[7] = vsubq_s16(_rows[3], _rows[7]);
    // 10 bit

    // Hadamard stage 2: 2x4
    q[0] = vaddq_s16(p[0], p[2]);
    q[1] = vaddq_s16(p[1], p[3]);
    q[2] = vsubq_s16(p[0], p[2]);
    q[3] = vsubq_s16(p[1], p[3]);
    q[4] = vaddq_s16(p[4], p[6]);
    q[5] = vaddq_s16(p[5], p[7]);
    q[6] = vsubq_s16(p[4], p[6]);
    q[7] = vsubq_s16(p[5], p[7]);
    // 11 bit

    // Hadamard stage 3: 1x8
    p[0] = vaddq_s16(q[0], q[1]);
    p[1] = vsubq_s16(q[0], q[1]);
    p[2] = vaddq_s16(q[2], q[3]);
    p[3] = vsubq_s16(q[2], q[3]);
    p[4] = vaddq_s16(q[4], q[5]);
    p[5] = vsubq_s16(q[4], q[5]);
    p[6] = vaddq_s16(q[6], q[7]);
    p[7] = vsubq_s16(q[6], q[7]);
    // 12 bit

    *_dc = vaddlvq_s16(p[0]);
    v_transpose8x8(p, q);

    // Hadamard stage 1: 4x2
    p[0] = vaddq_s16(q[0], q[4]);
    p[1] = vaddq_s16(q[1], q[5]);
    p[2] = vaddq_s16(q[2], q[6]);
    p[3] = vaddq_s16(q[3], q[7]);
    p[4] = vsubq_s16(q[0], q[4]);
    p[5] = vsubq_s16(q[1], q[5]);
    p[6] = vsubq_s16(q[2], q[6]);
    p[7] = vsubq_s16(q[3], q[7]);
    // 13 bit

    // Hadamard stage 2: 2x4
    q[0] = vaddq_s16(p[0], p[2]);
    q[1] = vaddq_s16(p[1], p[3]);
    q[2] = vsubq_s16(p[0], p[2]);
    q[3] = vsubq_s16(p[1], p[3]);
    q[4] = vaddq_s16(p[4], p[6]);
    q[5] = vaddq_s16(p[5], p[7]);
    q[6] = vsubq_s16(p[4], p[6]);
    q[7] = vsubq_s16(p[5], p[7]);
    // 14 bit

    // Hadamard stage 3: 1x8, ABS
    p[0] = vabsq_s16(vaddq_s16(q[0], q[1]));
    p[2] = vabsq_s16(vaddq_s16(q[2], q[3]));
    p[4] = vabsq_s16(vaddq_s16(q[4], q[5]));
    p[6] = vabsq_s16(vaddq_s16(q[6], q[7]));
    // 15->14 bit (ABS)

    // SUM
    uint16x8_t v0 = vreinterpretq_u16_s16(vaddq_s16(p[0], p[4]));
    uint16x8_t v2 = vreinterpretq_u16_s16(vaddq_s16(p[2], p[6]));
    // 15 bit

    v0 = vaddq_u16(v0, v2);
    v2 = vreinterpretq_u16_s16(vabaq_s16(vabaq_s16(vabaq_s16(vabdq_s16(q[0], q[1]), q[2], q[3]), q[4], q[5]), q[6], q[7]));
    // 16 bit full

    return vaddlvq_u16(v0) + vaddlvq_u16(v2) - vgetq_lane_s16(p[0], 0);
}

unsigned oc_enc_frag_satd_neon(int *_dc,const unsigned char *_src,
                               const unsigned char *_ref,int _ystride){
    int16x8_t buf[8];
    for (int i = 0; i < 8; i++)
        buf[i] = vreinterpretq_s16_u16(vsubl_u8(
                vld1_u8(&_src[i*_ystride]),
                vld1_u8(&_ref[i*_ystride])));

    return oc_hadamard_satd_neon_8x8(_dc,buf);
}

unsigned oc_enc_frag_satd2_neon(int *_dc,const unsigned char *_src,
                                const unsigned char *_ref1,const unsigned char *_ref2,int _ystride){
    int16x8_t buf[8];
    for (int i = 0; i < 8; i++)
        buf[i] = vreinterpretq_s16_u16(vsubl_u8(
                vld1_u8(&_src[i*_ystride]),
                vhadd_u8(vld1_u8(&_ref1[i*_ystride]), vld1_u8(&_ref2[i*_ystride]))));

    return oc_hadamard_satd_neon_8x8(_dc,buf);
}

unsigned oc_enc_frag_intra_satd_neon(int *_dc,
                                     const unsigned char *_src,int _ystride){
    int16x8_t buf[8];
    for (int i = 0; i < 8; i++)
        buf[i] = vreinterpretq_s16_u16(vmovl_u8(vld1_u8(&_src[i*_ystride])));

    return oc_hadamard_satd_neon_8x8(_dc,buf);
}

unsigned oc_enc_frag_sad_neon(const unsigned char *_src,
                              const unsigned char *_ref,int _ystride){
    uint16x8_t a;

    for(int i = 0; i < 8; i++) {
        uint8x8_t src = vld1_u8(&_src[i * _ystride]);
        uint8x8_t ref = vld1_u8(&_ref[i * _ystride]);
        a = i ? vabal_u8(a, src, ref) : vabdl_u8(src, ref);
    }

    return vaddlvq_u16(a);
}

unsigned oc_enc_frag_sad_thresh_neon(const unsigned char *_src,
                                     const unsigned char *_ref,int _ystride,unsigned _thresh){
    return oc_enc_frag_sad_neon(_src, _ref, _ystride);
}

unsigned oc_enc_frag_sad2_thresh_neon(const unsigned char *_src,
                                      const unsigned char *_ref1, const unsigned char *_ref2,
                                      int _ystride, unsigned _thresh){
    uint16x8_t a;

    for(int i = 0; i < 8; i++) {
        uint8x8_t src = vld1_u8(&_src[i * _ystride]);
        uint8x8_t ref = vhadd_u8(vld1_u8(&_ref1[i * _ystride]), vld1_u8(&_ref2[i * _ystride]));
        a = i ? vabal_u8(a, src, ref) : vabdl_u8(src, ref);
    }

    return vaddlvq_u16(a);
}

unsigned oc_enc_frag_ssd_neon(const unsigned char *_src,
                              const unsigned char *_ref,int _ystride){
    uint32x4_t buf[8];

    for(int i=0;i<8;i++){
        uint8x8_t src = vld1_u8(&_src[i * _ystride]);
        uint8x8_t ref = vld1_u8(&_ref[i * _ystride]);
        uint8x8_t dif = vabd_u8(src, ref);
        uint16x8_t squ = vmull_u8(dif, dif);
        buf[i] = vpaddlq_u16(squ);
    }

    buf[0] = vaddq_u32(buf[0], buf[4]);
    buf[1] = vaddq_u32(buf[1], buf[5]);
    buf[2] = vaddq_u32(buf[2], buf[6]);
    buf[3] = vaddq_u32(buf[3], buf[7]);

    buf[0] = vaddq_u32(buf[0], buf[2]);
    buf[1] = vaddq_u32(buf[1], buf[3]);

    buf[0] = vaddq_u32(buf[0], buf[1]);

    return vaddvq_u32(buf[0]);
}

void oc_enc_frag_copy2_neon(unsigned char *_dst,const unsigned char *_src1,const unsigned char *_src2,int _ystride) {
  for (int i = 0; i < 8; i++)
    vst1_u8(&_dst[i * _ystride],
            vhadd_u8(vld1_u8(&_src1[i * _ystride]),
                     vld1_u8(&_src2[i * _ystride])));
}

void oc_enc_frag_sub_neon(int16_t _diff[64],const unsigned char *_src,const unsigned char *_ref,int _ystride) {
  for (int i = 0; i < 8; i++)
    vst1q_s16(&_diff[i * 8],
              vreinterpretq_s16_u16(vsubl_u8(vld1_u8(&_src[i * _ystride]), vld1_u8(&_ref[i * _ystride]))));
}

void oc_enc_frag_sub_128_neon(int16_t _diff[64],const unsigned char *_src,int _ystride) {
  for (int i = 0; i < 8; i++)
    vst1q_s16(&_diff[i * 8],
              vreinterpretq_s16_u16(vsubl_u8(vld1_u8(&_src[i * _ystride]), vdup_n_u8(128))));
}
#endif
