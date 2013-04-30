
#include <kernel/febase.h>
#include <kernel/ideals.h>
#include <misc/options.h>
#include <kernel/kstd1.h>
#include <kernel/khstd.h>
#include <polys/weight.h>
#include <kernel/timer.h>
#include <kernel/polys.h>
#include <misc/intvec.h>

#include <kernel/shiftgb.h>

#include <kernel/kutil.h>




#include <kernel/SDNorm.h>
#include <kernel/SDBase.h>
#include <kernel/SDDebug.h>

#include <kernel/SDMultiplication.h>



  typedef skStrategy* kStrategy;


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

  //Grico: removed this assumption because maybe its not defined
  //but we want it
  //#ifdef HAVE_SHIFTBBADVEC //BOCO: added code
  strat->init_lV(lV);
  strat->init_uptodeg(uptodeg);
  //#endif

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
  
  int i = ShiftDVec::redHomog (&L, strat);
  
  p = L.p;

  return p;
}
