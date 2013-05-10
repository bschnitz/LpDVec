#include <mod2.h>
#include <SDkutil.h>

#ifndef SDBASE_H
#define SDBASE_H

namespace ShiftDVec
{
  namespace SD = ShiftDVec;

  ideal freegbdvc
    (ideal I, int uptodeg, int lVblock, long deBoGriFlags);
  ideal kStd
    ( ideal F, ideal Q, tHomog h,
      intvec ** w, intvec *hilb,int syzComp,
      int newIdeal, intvec *vw, int uptodeg, int lV );
  ideal bba
    ( ideal F, ideal Q, ideal I,
      intvec *w, intvec *hilb, SD::kStrategy strat );
  void initBba( ideal F, SD::kStrategy strat );
  poly redtailBba
    ( SD::LObject* L, int pos, SD::kStrategy strat,
      BOOLEAN withT=FALSE, BOOLEAN normalize=FALSE );
  poly redBba
    ( SD::LObject * h, int maxIndex, SD::kStrategy strat );
  poly redtail( SD::LObject* L, int pos, SD::kStrategy strat );
  poly redtail( poly p, int pos, SD::kStrategy strat );
  int redHomog( SD::LObject* h,SD::kStrategy strat );
  uint p_LmShortDivisibleBy
    ( SD::TObject* t1,unsigned long sev_t1,
      SD::TObject* t2,unsigned long not_sev_t2, const ring r );
  uint p_LmDivisibleBy
    (SD::TObject * t1, SD::TObject * t2, const ring r, int lV);
  static inline uint _p_LmDivisibleByNoComp
    (SD::TObject * t1, SD::TObject * t2, const ring r, int lV);
  int kFindDivisibleByInT
    ( const SD::TSet& T,
      const unsigned long* sevT, SD::LObject* L, 
      uint& shift, SD::kStrategy strat, const int start=0 );
  TObject * kFindDivisibleByInS
    ( SD::kStrategy strat, int pos,
      SD::LObject* L, SD::TObject* T, uint & shift,
      int lV, int uptodeg=0, long ecart = LONG_MAX  );
  int kFindDivisibleByInS
    ( const SD::kStrategy strat,
      int* max_ind, SD::LObject* L, int shift);
  void updateSdVec(BOOLEAN toT,ShiftDVec::kStrategy strat);
  void completeReduce
    ( SD::kStrategy strat, BOOLEAN withT=FALSE );
  void updateResult( ideal r,ideal Q, SD::kStrategy strat );
  KINLINE poly redtailBba
    ( poly p, int pos, SD::kStrategy strat,BOOLEAN normalize );
  void updateS( BOOLEAN toT,SD::kStrategy strat );
  void clearS 
    ( SD::LObject* H, unsigned long p_sev,
      int* at, int* k, SD::kStrategy strat );
  void enterpairs 
    ( LObject* H, int k, int ecart,
      int pos, SD::kStrategy strat, int atR );
  SD::LObject* enterOnePair
    ( poly p1, int atR1,
      int isFromQ1, int ecart1,
      poly p2, uint shift, int atR2,
      int isFromQ2, int ecart2, SD::kStrategy strat );
  void initenterpairs
    ( SD::LObject* H, int k,
      int ecart,int isFromQ,
      SD::kStrategy strat, int atR = -1 );
  void enterRightOverlaps
    ( SD::LObject* H,
      SD::kStrategy strat, int k, uint** rightOvls,
      uint* sizesRightOvls, int isFromQ, int ecart, int atR );
  void enterLeftOverlaps
    ( SD::LObject* H,
      SD::kStrategy strat, int k, uint** leftOvls,
      uint* sizesLeftOvls, int isFromQ, int ecart, int atR );
  void enterSelfOverlaps
    ( SD::LObject* H,
      SD::kStrategy strat, uint* selfOvls,
      uint numSelfOvls, int isFromQ, int ecart, int atR );
  void GebauerMoeller
    ( uint* selfOvls, uint sizeSelfOvls,
      uint** rightOvls, uint* sizesRightOvls,
      uint** leftOvls, uint* sizesLeftOvls, 
      SD::LObject* H, int k, SD::kStrategy strat );
  bool GMTestRight
    ( SD::TObject* H1,
      SD::TObject* H2, SD::TObject* H3, uint shift2,
      uint shift3, SD::kStrategy strat, ring r = currRing );
  bool GMTestLeft
    ( SD::TObject* H1,
      SD::TObject* H2, SD::TObject* H3, uint shift2,
      uint shift3, SD::kStrategy strat, ring r = currRing );
  bool GMTest
    ( SD::TObject* H1,
      SD::TObject* H2,
      uint sH2, SD::TObject* H3, 
      uint* ovlH1H3, uint numOvlH1H3,
      uint* ovlH3H2, uint numOvlH3H2  );
  bool GM3
    ( SD::LObject* H, uint shift1,
      uint shift2, SD::kStrategy strat, ring r = currRing );
  bool GM3
    ( SD::LObject* H, SD::LObject* L,
      SD::kStrategy strat, ring r = currRing );
  void chainCrit
    ( SD::LObject *H, int ecart, SD::kStrategy strat );
  void initBuchMoraCrit( SD::kStrategy strat );
  SD::LObject* enterL
    ( SD::LSet *set,
      int *length, int *LSetmax,
      SD::LObject* p,int at, uint* dvec=NULL );
  void initSL( ideal F, ideal Q, SD::kStrategy strat );
  void initBuchMora( ideal F, ideal Q, SD::kStrategy strat );
  void kMergeBintoL( SD::kStrategy strat );
  poly p_mLPshift
    ( poly p, int sh, int uptodeg, int lV, const ring r );
  ideal initTestGM
    ( ideal I, poly p, int uptodeg, int lVblock );
  void DeBoGriTestGM
    ( SD::TSet tset, int k,
      SD::TObject* H, int uptodeg, int lVblock );
}

#endif