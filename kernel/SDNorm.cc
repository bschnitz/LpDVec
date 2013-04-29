
#include <kernel/SDNorm.h>
#include <kernel/SDBase.cc>

namespace ShiftDVec
{
  class sTObject;

  uint divisibleBy
    ( const ShiftDVec::sTObject * t1,
      const ShiftDVec::sTObject * t2, int numVars );

  uint findRightOverlaps
    ( const ShiftDVec::sTObject * t1,
      const ShiftDVec::sTObject * t2,
      int numVars, int maxDeg, uint ** overlaps );

//  bool redViolatesDeg
//    ( poly a, poly b, int uptodeg, ring ar, ring br );
  bool createSPviolatesDeg
    (poly a, poly b, uint shift, int uptodeg);

  typedef skStrategy* kStrategy;
};

//this is a comentary

poly ShiftDVec::NormalForm(poly p, ideal I, int uptodeg, int nVars)
{
  namespace SD = ShiftDVec;
  
  poly n;
  ideal pI = p,I;
  
  if (! ideal_isInV(pI,lVblock) )
  {
    WerrorS("The input ideal contains incorrectly encoded elements! ");
    return(NULL);
  }

  kill pI;
  
  n = SD::initNF(p,I,NULL, testHomog, NULL,NULL,0,0,NULL, uptodeg, lVblock);
  
  n = p_Cleardenom(n,currRing);
  
  return n;

}

poly ShiftDVec::initNF(poly p, ideal F, ideal Q, tHomog h, intvec ** w, intvec *hilb, int syzComp, int newIdeal, intvec *vw, int uptodeg, int lV )
{
  namespace SD = ShiftDVec;

  ideal r;
  BOOLEAN b=currRing->pLexOrder,toReset=FALSE;
  BOOLEAN delete_w=(w==NULL);
  kStrategy strat=new skStrategy;

  #ifdef HAVE_SHIFTBBADVEC //BOCO: added code
  strat->init_lV(lV);
  strat->init_uptodeg(uptodeg);
  #endif

  if(!TEST_OPT_RETURN_SB)
    strat->syzComp = syzComp;
  if (TEST_OPT_SB_1)
    strat->newIdeal = newIdeal;
  if (rField_has_simple_inverse(currRing))
    strat->LazyPass=20;
  else
    strat->LazyPass=2;
  strat->LazyDegree = 1;

  strat->enterOnePair=NULL;
  strat->chainCrit=NULL;

    strat->ak = id_RankFreeModule(F,currRing);
  strat->kModW=kModW=NULL;
  strat->kHomW=kHomW=NULL;
  if (vw != NULL)
  {
    currRing->pLexOrder=FALSE;
    strat->kHomW=kHomW=vw;
    strat->pOrigFDeg = currRing->pFDeg;
    strat->pOrigLDeg = currRing->pLDeg;
    pSetDegProcs(currRing,kHomModDeg);
    toReset = TRUE;
  }
  if (h==testHomog)
  {
    if (strat->ak == 0)
    {
      h = (tHomog)idHomIdeal(F,Q);
      w=NULL;
    }
    else if (!TEST_OPT_DEGBOUND)
    {
      h = (tHomog)idHomModule(F,Q,w);
    }
  }
  currRing->pLexOrder=b;

  if (h==isHomog)
  {
    if (strat->ak > 0 && (w!=NULL) && (*w!=NULL))
    {
      strat->kModW = kModW = *w;
      if (vw == NULL)
      {
        strat->pOrigFDeg = currRing->pFDeg;
        strat->pOrigLDeg = currRing->pLDeg;
        pSetDegProcs(currRing,kModDeg);
        toReset = TRUE;
      }
    }
    currRing->pLexOrder = TRUE;
    if (hilb==NULL) strat->LazyPass*=2;
  }
  strat->homog=h;

  SD::LObject L(p, currRing);
  
  int i = ShiftDVec::redHomog (@L, strat);
  
  p = L.p;

  return p;
}
