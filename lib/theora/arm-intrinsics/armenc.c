#include "armenc.h"

void oc_enc_accel_init_arm(oc_enc_ctx *_enc){
  ogg_uint32_t cpu_flags;
  cpu_flags=_enc->state.cpu_flags;
  oc_enc_accel_init_c(_enc);
#if defined(OC_ARM_ASM_NEON)
  if(cpu_flags & OC_CPU_ARM_NEON){
# if defined(OC_ENC_USE_VTABLE)
    _enc->opt_vtable.frag_satd=oc_enc_frag_satd_neon;
    _enc->opt_vtable.frag_satd2=oc_enc_frag_satd2_neon;
    _enc->opt_vtable.frag_intra_satd=oc_enc_frag_intra_satd_neon;
    _enc->opt_vtable.frag_sad=oc_enc_frag_sad_neon;
    _enc->opt_vtable.frag_sad_thresh=oc_enc_frag_sad_thresh_neon;
    _enc->opt_vtable.frag_sad2_thresh=oc_enc_frag_sad2_thresh_neon;
    _enc->opt_vtable.frag_ssd=oc_enc_frag_ssd_neon;
    _enc->opt_vtable.frag_copy2=oc_enc_frag_copy2_neon;
    _enc->opt_vtable.frag_sub=oc_enc_frag_sub_neon;
    _enc->opt_vtable.frag_sub_128=oc_enc_frag_sub_128_neon;
# endif
    _enc->opt_data.enquant_table_size=128*sizeof(ogg_uint16_t);
    _enc->opt_data.enquant_table_alignment=16;
  }
#endif
}
