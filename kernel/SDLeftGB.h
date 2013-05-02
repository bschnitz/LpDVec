#ifndef SDLeftGB_H
#define SDLeftGB_H

#include <kernel/SDkutil.h>

namespace ShiftDVec
{
  namespace SD = ShiftDVec;

  namespace LeftGB
  {
    void initenterpairs
      ( SD::LObject* J, int size_of_I, int ecart,
        int isFromQ, SD::kStrategy strat, int atR );

    void GebauerMoeller
      ( uint** overlaps, uint* sizesOvls,
        SD::LObject* J, SD::kStrategy strat );

    void GMFilter
      ( SD::LObject* J,
        SD::TObject* p_i,
        SD::TObject* p_k,
        uint* i_ovls_j, uint* k_ovls_j, 
        uint size_i_ovls_j, uint size_k_ovls_j );

    void GMFilter
      ( SD::LObject* L,
        SD::LObject* J,
        SD::LObject* p_k,
        uint* k_ovls_j,uint size_k_ovls_j, SD::kStrategy strat );

    void GMFilter
      ( int index_L,
        SD::LObject* J,
        SD::LObject* p_k,
        uint* k_ovls_j,uint size_k_ovls_j, SD::kStrategy strat );

    void enterOverlaps
      ( SD::LObject* J,
        SD::kStrategy strat, uint** overlaps,
        uint* ovl_sizes, int isFromQ, int ecart, int atR );

    typedef skStrategy* kStrategy;
  }
}

#endif
