#ifndef _arm_intrinsics_armencfrag_H
#define _arm_intrinsics_armencfrag_H 1
#include <stdint.h>
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef OC_ARM_ASM_NEON
unsigned oc_enc_frag_satd_neon(int *_dc,const unsigned char *_src,
                               const unsigned char *_ref,int _ystride);
unsigned oc_enc_frag_satd2_neon(int *_dc,const unsigned char *_src,
                                const unsigned char *_ref1,const unsigned char *_ref2,int _ystride);
unsigned oc_enc_frag_intra_satd_neon(int *_dc,
                                     const unsigned char *_src,int _ystride);
unsigned oc_enc_frag_sad_neon(const unsigned char *_src,
                              const unsigned char *_ref,int _ystride);
unsigned oc_enc_frag_sad_thresh_neon(const unsigned char *_src,
                                     const unsigned char *_ref,int _ystride,unsigned _thresh);
unsigned oc_enc_frag_sad2_thresh_neon(const unsigned char *_src,
                                      const unsigned char *_ref1, const unsigned char *_ref2,
                                      int _ystride, unsigned _thresh);
unsigned oc_enc_frag_ssd_neon(const unsigned char *_src,
                              const unsigned char *_ref,int _ystride);
void oc_enc_frag_copy2_neon(unsigned char *_dst,const unsigned char *_src1,const unsigned char *_src2,int _ystride);
void oc_enc_frag_sub_neon(int16_t _diff[64],const unsigned char *_src,const unsigned char *_ref,int _ystride);
void oc_enc_frag_sub_128_neon(int16_t _diff[64],const unsigned char *_src,int _ystride);
#endif

#endif //_arm_intrinsics_armencfrag_H
