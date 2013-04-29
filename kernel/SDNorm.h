#include <mod2.h>
#include <SDkutil.h>

#include <SDBase.h>

#ifndef SDNORM_H
#define SDNORM_H


namespace ShiftDVec
{
  namespace SD = ShiftDVec;

  poly NormalForm
   (poly p, ideal I, int uptodeg, int nVars);

  poly initNF
   (poly p, ideal F, ideal Q, tHomog h, intvec ** w, intvec *hilb, 
    int syzComp, int newIdeal, intvec *vw, int uptodeg, int lV );

}

#endif
