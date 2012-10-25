#define KUTIL2_CC

/* Preamble
 * See kutil2.h
 * Last Change: 2012-03-02 18:00
 * Maintainers: Grischa Studzinski and Benjamin Schnitzler 
 */

/* TODO:
 * a lot (see kutil2.h)
 * - loop
 *     search for (semantic and syntax) bugs
 *   until all bugs are cleared
 * (unfortunatly the above statement does not compile)
 */

#include <climits>

//Needed because of include order; should rather be included in
//ring.h
#include <kernel/mod2.h>

#include <kernel/kutil.h>
#include <kernel/kutil2.h> //(already included in kutil.h)
#include <polys/monomials/p_polys.h> //For p_Totaldegree and the like
#include <kernel/polys.h> //For pTotaldegree and the like
#include <kernel/febase.h> //For Print stuff

#if DEBOGRI > 0
#if 0
#include <fstream>
#endif
#endif



//member functions of ShiftDVec::sTobject



uint ShiftDVec::CreateDVec
  (poly p, ring r, uint*& dvec)
{
  initDeBoGri
    ( ShiftDVec::indent, 
      "Entering CreateDVec", "Leaving CreateDVec", 512 );
  deBoGriPrint(++ShiftDVec::debugCounter, "debugCounter: ", 512);

  if(p == NULL){dvec = NULL; return 0;}

  //We should test here, if p is in r. (see sTObject::Set ?)

  //I hope this is the right way to get the total deg. of lm(p)
  uint dvSize = p_Totaldegree(p, r);
  assume(dvSize < 1000);
  if(!dvSize){dvec = NULL; return 0;}
  dvec = (uint *)omAlloc0(dvSize*sizeof(uint));
  loGriToFile("omAlloc0 for dvec in CreateDVec ",dvSize*sizeof(uint),1024, (void*)dvec);

  uint * it = dvec;

  /* transform lm(p) to shift invariant distance vector
   * j is the distance to the next entry in the block
   * representation of the letterplace monomial lm(p)   
   * "it" is a pointer to the next free entry in our distance
   * vector                                                   
   * TODO: Do that by getting the exponent vector (see mail from
   * H. Schoenemann)
   */
  for(int j=1, i=1, l=0; l < dvSize; ++i)
    if(p_GetExp(p,i,r)){*it=j;++it;j=1;++l;} else{++j;}

  return dvSize;
}


uint* ShiftDVec::sTObject::GetDVec()
{
  return dvec;
}


uint ShiftDVec::sTObject::GetDVsize()
{
  if(!dvec) SetDVec();
  if(!dvec) return 0; //Be careful! p does not exist!
  return dvSize;
}


int ShiftDVec::sTObject::cmpDVec(ShiftDVec::sTObject* T)
{
  if(GetDVsize() == T->GetDVsize())
    return memcmp(dvec,T->dvec,dvSize*sizeof(uint));
  return -1;
}

/* Compares part of this object's DVec, beginning at begin, to 
 * part of T1's DVec, beginning at beginT1. Comparison ends
 * after comparing size entries. This function does not check, 
 * if begin + size, respectivly beginT1 + size is larger than
 * this object's DVec size, respectivly the size of T1's DVec.
 * Returns 0 if these parts match, or non-zero value if not
 * (see memcmp for more informations).                        */
int ShiftDVec::sTObject::cmpDVec
  (ShiftDVec::sTObject* T1, uint begin, uint beginT1, uint size)
{
  assume(begin + size <= dvSize && beginT1 + size <= T1->dvSize);
  initDeBoGri
    ( ShiftDVec::indent, 
      "Entering cmpDVec", "Leaving cmpDVec", 128 );
  deBoGriPrint(begin + size, "begin + size: ", 128);
  deBoGriPrint(dvSize, "dvSize: ", 128);
  deBoGriPrint(beginT1 + size, "beginT1 + size: ", 128);
  deBoGriPrint(T1->dvSize, "T1->dvSize: ", 128);

    return 
      memcmp( dvec+begin, T1->dvec+beginT1, size*sizeof(uint) );
}

/* Does what cmpDVec should do. size > 0 should hold.*/
int ShiftDVec::sTObject::cmpDVecProper
  ( ShiftDVec::sTObject* T1, 
    uint begin, uint beginT1, uint size, int lV )
{
  assume(size > 0);
  assume(begin >= 0 && beginT1 >= 0);
  assume(begin + size <= dvSize && beginT1 + size <= T1->dvSize);

  initDeBoGri
    ( ShiftDVec::indent, 
      "Entering cmpDVecProper", "Leaving cmpDVecProper", 128 );

  deBoGriInitCounter();
  deBoGriIncCounter();
  deBoGriPrintCounter("Counter: ", 128);

  deBoGriPrint(begin, "begin: ", 128);
  deBoGriPrint(dvSize, "dvSize: ", 128);
  deBoGriPrint(beginT1, "beginT1: ", 128);
  deBoGriPrint(T1->dvSize, "T1->dvSize: ", 128);
  deBoGriPrint(size, "size: ", 128);

  if(begin == 0 && beginT1 != 0)
  {
    deBoGriPrint("begin == 0", 128);
    int first = 0, i = 0;
    for(; i <= beginT1; ++i) first += T1->dvec[i];
    if(first - beginT1*lV != dvec[begin]) return -1;
    size -= 1; begin += 1; beginT1 += 1;
  } 
  else if(beginT1 == 0)
  {
    deBoGriPrint("beginT1 == 0", 128);
    int first = 0, i = 0;
    for(; i <= begin; ++i) first += dvec[i];
    if(first - begin*lV != T1->dvec[beginT1]) return -1;
    size -= 1; begin += 1; beginT1 += 1;
  }

  return 
    memcmp( dvec+begin, T1->dvec+beginT1, size*sizeof(uint) );
}

void ShiftDVec::sTObject::freeDVec()
{
  if(dvec){ 
    loGriToFile("omFreeSize of (ADDRESS)dvec in freeDVec ", sizeof(uint)*dvSize, 1024, (void*)dvec ); 
    omFreeSize((ADDRESS)dvec, sizeof(uint)*dvSize); 
    dvec = NULL;
    dvSize = 0;
  }
}

ShiftDVec::sTObject& ShiftDVec::sTObject::operator=
  (const ShiftDVec::sTObject& t)
{
  this->::sTObject::operator=(t);

  //dvec = NULL; //or should we copy, or deep copy??? - only user knows
  //dvSize = 0;
  //Be careful: we copy the dvec shallow!
  dvec = t.dvec;
  dvSize = t.dvSize;
}



//member functions of ShiftDVec::sLobject



//Get the size of the lcms dvec. Initialize it, if not set - the
//latter requires p1 and p2 to be set and shifted correctly.
uint ShiftDVec::sLObject::getLcmDvSize(ring r)
{
  assume( p1 != NULL && p2 != NULL );

  if( !lcmDvSize ) {
    //I hope, this will get me the letterplace degree of lm(p1) 
    //(totaldeg(x(4)*y(5)*z(6)) for example should be 6).
    //BOCO: IMPORTANT: TODO: This was a wrong assuption - maybe
    //use getShift somehow.
    int degP1 = p_Totaldegree(p1, r);
    int degP2 = p_Totaldegree(p2, r);
    lcmDvSize = degP1 > degP2 ? degP1 : degP2; 
  }

  return lcmDvSize;
}


void ShiftDVec::sLObject::SetLcmDVec(ring r)
{
  initDeBoGri
    ( ShiftDVec::indent, 
      "Entering SetLcmDVec", "Leaving SetLcmDVec", 16 );

#ifdef USE_DVEC_LCM
  //We want to create the lcm directly from p1, p2 .
  //This is an adapted version of the CreateDVec function.
  //TODO: This has to be testet!
  deBoGriPrint("USE_DVEC_LCM is set.", 16);
  deBoGriPrint
    ("Using p1, p2 to create the lcm's DVec directly.", 16);
  
  assume(p1 != NULL && p2 != NULL);

  deBoGriPrint(p1, "p1 is: ", 16);
  deBoGriPrint(p2, "p2 is: ", 16);

  //I hope, this will get me the letterplace degree of lm(p1) 
  //(totaldeg(x(4)*y(5)*z(6)) for example should be 6).
  //BOCO: IMPORTANT: TODO: This was a wrong assuption - maybe
  //use getShift somehow.
  uint dvSize1 = p_Totaldegree(p1, r);
  uint dvSize2 = p_Totaldegree(p2, r);

  //One of the polys should be shifted in letterplace case. This
  //one overlaps the other one.
  poly shifted; poly notShifted;
  if(dvSize1 > dvSize2)
    {poly shifted=p1; poly notShifted=p2; 
      lcmDvSize=dvSize1; dvSize1 = dvSize2;}
  else
    {poly shifted=p2; poly notShifted=p1; lcmDvSize=dvSize2;}

  assume(lcmDvSize > 0);

 loGriToFile("omAlloc0 for lcmDvec in SetLcmDVec ",lcmDvSize*sizeof(uint),1024);
  lcmDvec = (uint *)omAlloc0(lcmDvSize*sizeof(uint));

  uint * it = lcmDvec;

  /* The creation of the shift invariant distance vector is
   * similar to its creation in CreateDVec; we will begin with
   * the not shifted poly. After we have created it's dvec we
   * will continue to finish the lcmDvec by beginning at the
   * right postion of the second polynomial.                   */
  int j = 1, i = 1, l = 0;
  for(;l < dvSize1; ++i)
    if(p_GetExp(notShifted,i,r)){*it=j;++it;j=1;++l;} else{++j;}
  for(;l < lcmDvSize; ++i)
    if(p_GetExp(shifted,i,r)){*it=j;++it;j=1;++l;} else{++j;}
#else
  deBoGriPrint(lcm, "The lcm is: ", 16);
  SetLcmDVec(lcm, r);
#endif

  deBoGriPrint(lcmDvec, lcmDvSize, "New dvec is: ", 16);
  deBoGriPrint(lcmDvSize, "It's size is: ", 16);
}


void ShiftDVec::sLObject::freeLcmDVec()
{
  if(lcmDvec){ 
    loGriToFile("omFreeSize for (ADDRESS)lcmDvec in freeLcmDVec",sizeof(uint)*dvSize,1024);
    omFreeSize((ADDRESS)lcmDvec, sizeof(uint)*dvSize); 
    dvec = NULL;
    dvSize = 0;
    lcmDvec = NULL;
  }
}


/* returns true, if the DVec of this objects lcm is equal to the
 * DVec of the other objects lcm. Thus, if both lcm do not have
 * the same shift, but would otherwise be equal, true will be
 * returned, too.  In every other case, false is returned.    */
bool ShiftDVec::sLObject::compareLcmTo( sLObject* other, ring r )
{
  if( this->getLcmDvSize(r) != other->getLcmDvSize(r) ) 
    return false;

  assume(this->p1 != NULL && this->p2 != NULL);
  assume(other->p1 != NULL && other->p2 != NULL);

  //now we need to create the dvecs
  if(!this->lcmDvec) this->SetLcmDVec();
  if(!other->lcmDvec) other->SetLcmDVec();

  if(memcmp((void*)(lcmDvec),(void*)other->lcmDvec,lcmDvSize))
    return false;

  return true;
}


/* This function returns true, if the objects lcm is equal to
 * a shift of lcm(p1, p2).                                    */
bool ShiftDVec::sLObject::compareLcmTo(poly p1, poly p2, ring r)
{
  //We want to obtain the lcm directly from p1, p2 .
  //This is an adapted version of the SetLcmDVec function.
  //TODO: This has to be testet!
  
  assume(p1 != NULL && p2 != NULL);

  //I hope, this will get me the letterplace degree of lm(p1) 
  //(totaldeg(x(4)*y(5)*z(6)) for example should be 6).
  uint dvSize1 = p_Totaldegree(p1, r);
  uint dvSize2 = p_Totaldegree(p2, r);

  //We need to sort out, which poly has the higher degree
  poly hiDeg; poly lowDeg; uint pLcmDvSize;
  if(dvSize1 > dvSize2)
    {poly shifted=p1; poly notShifted=p2; 
      lcmDvSize=dvSize1; dvSize1 = dvSize2;}
  else
    {poly hiDeg=p2; poly lowDeg=p1; pLcmDvSize=dvSize2;}

  if(pLcmDvSize != lcmDvSize){return false;}

  if( compareDVec(lcmDvec, lowDeg, 0, dvSize1, r) &&
      compareDVec(lcmDvec, lowDeg, dvSize1, pLcmDvSize, r) ) 
    return true;

  return false;
}


/* This function expects two letterplace polynomials and will
 * return true, if p1 and p2 correspond to a valid overlap and
 * their lcm is equal to this objects lcm. p1 is assumed to be
 * not shifted, p2 is assumed to be shifted. The lcm is assumed
 * to be not shifted. All polynomials are assumed to be
 * correctly formatted letterplace polynomials (no gaps, no
 * exponents greater than 1).
 * Examples:
 *  Ex 1 :  x(1)y(2) and y(2)z(3) correspond to a valid overlap 
 *  their lcm is x(1)y(2)z(3)
 *  Ex 2 :  x(1)y(2) and y(3)z(4) do not correspond to a valid 
 *  overlap
 *  Ex 3 :  x(1)y(2)z(3) and y(2)x(3)z(4) do not correspond to 
 *  a valid overlap, though they have indeed gcd != 1 .
 *  Ex 4 :  x(1)y(2)z(3) and y(2)z(3) do not correspond to a 
 *  valid overlap, since we do only enter pairs, such that p2
 *  does not divide p1.
 * This function was somehow inspired by the pCompareChain
 * function. See that function for more info.                 */
bool ShiftDVec::sLObject::gm3LcmUnEqualToLcm
  (poly p1, poly p2, int lV, ring r /* = currRing */ )
{
  bool unequal;

  //Number of variables in our letterplace ring
  int numVars = r->N;

  //p2 is shifted, we will loop, until we discover the first
  //variable not equal to 0.
  int j;
  for (j=1; j <= numVars && !pGetExp(p2, j); ++j)
    unequal = unequal || pGetExp(p1, j) != pGetExp(this->lcm,j);

  //We looped, until pGetExp(p2, j) != 0. From here on, p1 and
  //p2 should be equal, until the end of p1.
  assume(lV < j);
  for (; j <= numVars; ++j){
    unequal = unequal || pGetExp(p2, j) != pGetExp(this->lcm,j);
    if( pGetExp(p1, j) != pGetExp(p2, j) ) break;
  }

  if( j > numVars - lV ) return false;
  //Everything was equal up to the last letterplace variable,
  //this is like in Ex 4, p2 divides p1 - not a valid pair.

  //position right before current pos. in letterplace polynomial
  int lpPos = j / lV;
  /* p1 and p2 were equal up to the previous position in the
   * letterplace ring. Now p1 should be 0, if not, this would
   * either mean, that p2 is 0 and this implies that p2 divides
   * p1, or p1 != p2 at the current letterplace position, which
   * means, that we have something like Ex 3 . Both cases imply,
   * that we have not a valid pair.                           */
  for(j = lpPos * lV + 1; j <= lpPos * (lV + 1); ++j)
    if( pGetExp(p1, j) != 0 ) return false;

  //Now our polynomials correspond to a valid overlap, we can
  //stop, as soon, as we know, that the lcms are unequal

  if(unequal) return true;

  for (; j <= numVars; ++j)
    if( pGetExp(p2, j) != pGetExp(this->lcm,j) ) return true;

  //The lcms were equal
  return false;
}

uint ShiftDVec::sLObject::lcmDivisibleBy
  ( ShiftDVec::sTObject * T, int numVars )
{
  initDeBoGri
    ( ShiftDVec::indent, 
      "Entering lcmDivisibleBy", "Leaving lcmDivisibleBy", 16 );

  this->SetLcmDvecIfNULL();
  T->SetDVecIfNULL();

  deBoGriPrint(this->lcm, "this->lcm: ", 16);
  deBoGriPrint
    (this->lcmDvec, this->lcmDvSize, "this->lcmDvec: ", 16);
  deBoGriPrint(this->lcmDvSize, "this->lcmDvSize: ", 16);
  deBoGriPrint(T->p, "T->p: ", 16);
  deBoGriPrint(T->dvec, T->dvSize, "T->dvec: ", 16);
  deBoGriPrint(T->dvSize, "T->dvSize: ", 16);

  return ShiftDVec::divisibleBy
    ( lcmDvec, lcmDvSize, T->dvec, T->dvSize, numVars );
}

uint ShiftDVec::sLObject::lcmDivisibleBy
  ( ShiftDVec::sTObject * T, 
    uint minShift, uint maxShift, int numVars )
{
  initDeBoGri
    ( ShiftDVec::indent, 
      "Entering lcmDivisibleBy(sTObject, int, uint, uint, int)", 
      "Leaving lcmDivisibleBy", 16                              );

  this->SetLcmDvecIfNULL();
  T->SetDVecIfNULL();

  deBoGriPrint(this->lcm, "this->lcm: ", 16);
  deBoGriPrint
    (this->lcmDvec, this->lcmDvSize, "this->lcmDvec: ", 16);
  deBoGriPrint(this->lcmDvSize, "this->lcmDvSize: ", 16);
  deBoGriPrint(T->p, "T->p: ", 16);
  deBoGriPrint(T->dvec, T->dvSize, "T->dvec: ", 16);
  deBoGriPrint(T->dvSize, "T->dvSize: ", 16);

  return ShiftDVec::divisibleBy
    ( lcmDvec, lcmDvSize, 
      T->dvec, T->dvSize, minShift, maxShift, numVars );
}



//other functions, which do not have no counterpart in normal bba



//This should get the shift of a letterplace polynomial.
//numFirstVar should be the index of the variable in the first
//visual block of p (first index should be 1!). TODO: It would
//be more efficient to store the shifts for all shifted
//polynomials somehow. p should not be NULL!
uint ShiftDVec::getShift
  ( poly p, uint numFirstVar, ring r )
{
  for( uint i = numFirstVar, sh=0; ; i += r->N, ++sh )
    if( p_GetExp(p, i, r) ) return sh;
}


bool ShiftDVec::compareDVec
  ( const uint* dvec, poly p, uint offset, uint maxSize, ring r )
{

#if DEBOGRI > 0 
if( deBoGri == 4 )
{
  PrintS("\nIn compareDVec\nComparing\n");
  dvecWrite(dvec, maxSize);
  PrintS("\nWith\n");
  pWrite(p);
  PrintS("Which has dvec:\n");
  dvecWrite(p);
  Print("\nUp to size %d\n", maxSize);
}
#endif

  for(uint j = 1, i = 1, l = offset; l < maxSize; ++i)
  {
    if( p_GetExp(p,i,r) )
      if( dvec[l] != j ){ return false; }
      else { j = 1; ++l; }
    else{ ++j; }
  }

#if DEBOGRI > 0
if( deBoGri == 4 )
  PrintS("compareDVec returns true!\n");
#endif

  return true;
}


/* Returns the first shift s, where the polynomial corresponding
 * to dvec1, is divisible by the shifted polynomial corresponding
 * to dvec2. If no such shift exists, UINT_MAX is returned. If
 * dvSize2 == 0, UINT_MAX-1 is returned.
 *
 * Has to be tested!
 */
uint ShiftDVec::divisibleBy
  ( const uint* dvec1, uint dvSize1, 
    const uint* dvec2, uint dvSize2, int numVars )
{
  initDeBoGri
    ( ShiftDVec::indent, 
      "Entering divisibleBy", "Leaving divisibleBy", 256 );
  deBoGriPrint(dvec1, dvSize1, "dvec1: ", 256);
  deBoGriPrint(dvec2, dvSize2, "dvec2: ", 256);
  assume(dvSize2 > 0);
  assume(dvSize1 > 0);
  if(dvSize1 < dvSize2) return UINT_MAX;
  if(!dvSize2) return UINT_MAX-1;

 loGriToFile("omAlloc0 for tmpVec in divBy ",dvSize1*sizeof(uint),1024);
  uint * tmpVec = (uint *)omAlloc0(dvSize1*sizeof(uint));
  memcpy((void*)tmpVec,(void*)dvec1,dvSize1*sizeof(uint));

  //overflow can not happen since dvSize1 <= dvSize2
  uint maxSh = dvSize1 - dvSize2;
  size_t cmpLen = dvSize2 * sizeof(uint);
  for(int shift=0; shift <= maxSh; ++shift){
    if(!memcmp((void*)(tmpVec+shift),(void*)dvec2,cmpLen)){
      loGriToFile("omFreeSize for (ADDRESS)tmpVec in divBy ",dvSize1*sizeof(uint),1024);
      omFreeSize((ADDRESS)tmpVec, sizeof(uint)*dvSize1);
      return shift;
    }
    tmpVec[shift+1] = tmpVec[shift] + tmpVec[shift+1] - numVars;
  }
  loGriToFile("omFreeSize for (ADDRESS)tmpVec in divBy ",dvSize1*sizeof(uint),1024);
  omFreeSize((ADDRESS)tmpVec, sizeof(uint)*dvSize1);
  return UINT_MAX;
}


/* Returns the first shift s >= minShift, with s <= maxShift
 * where the polynomial corresponding to dvec1, is divisible by 
 * the shifted polynomial corresponding to dvec2. If no such 
 * shift exists, UINT_MAX is returned.  
 *
 * Assumes: 
 * minShift <= maxShift
 * and
 * maxShift + dvSize2 < dvSize1
 * and
 * dvSize2 > 0
 * and
 * dvSize1 > 0 ,
 * but does not test these!
 *
 * TODO:
 * - Has to be tested!
 * - Maybe Code could be commented a bit more.
 */
uint ShiftDVec::divisibleBy
  ( const uint* dvec1, uint dvSize1, 
    const uint* dvec2, uint dvSize2, 
    uint minShift, uint maxShift, int numVars )
{
  assume(dvSize2 > 0);
  assume(dvSize1 > 0);
  assume(minShift <= maxShift);
  assume(maxShift + dvSize2 < dvSize1);

  uint tmpSize = (maxShift - minShift + dvSize2);
 loGriToFile("omAlloc0 in divBy for tmpVec ",tmpSize*sizeof(uint),1024);
  uint * tmpVec = (uint *) omAlloc0( tmpSize*sizeof(uint) );

  memcpy
    ((void*)tmpVec,(void*)(dvec1+minShift),tmpSize*sizeof(uint));

  for(int i = 0; i < minShift; ++i) tmpVec[0] += dvec1[i];
  tmpVec[0] -= (numVars * minShift);

  size_t cmpLen = dvSize2 * sizeof(uint);
  for(int shift=0; shift <= maxShift - minShift; ++shift){
    if(!memcmp((void*)(tmpVec+shift),(void*)dvec2,cmpLen)){
      loGriToFile("omFreeSize for (ADDRESS)tmpVec in divBy ",tmpSize*sizeof(uint),1024);
      omFreeSize((ADDRESS)tmpVec, sizeof(uint)*tmpSize);
      return shift;
    }
    tmpVec[shift+1] = tmpVec[shift] + tmpVec[shift+1] - numVars;
  }

  loGriToFile("omFreeSize for (ADDRESS)tmpVec in divBy ",tmpSize*sizeof(uint),1024);
  omFreeSize((ADDRESS)tmpVec, sizeof(uint)*tmpSize);
  return UINT_MAX;
}


/* Returns the first shift s, where lm(p1) is divided by the 
 * shifted monomial s*lm(p2) (,that is the dvec of lm(p1) has a
 * central overlap with the dvec of lm(p2)). If no such shift
 * exists, UINT_MAX is returned. If lm(p2) is a Field Element,
 * UINT_MAX-1 is returned (In this case the dvec of p2 should be 
 * a NULL-Vector). Sets the dvecs, if they are NULL.
 *
 * Has to be tested!
 */
uint ShiftDVec::divisibleBy
  ( ShiftDVec::sTObject * p1, 
    ShiftDVec::sTObject * p2, int numVars )
{
  p1->SetDVecIfNULL();
  p2->SetDVecIfNULL();

  return divisibleBy
    (p1->dvec, p1->dvSize, p2->dvec, p2->dvSize, numVars );
}


#if 0 //in progress
/* Stores the first shift s greater than s0 into s0, where
 * lm(p1) is divided by the shifted monomial s*lm(p2) (,that is
 * the dvec of lm(p1) has a central overlap with the dvec of 
 * lm(p2)). If no such shift exists, false is returned,
 * otherwise true.
 *
 * Has to be tested!
 */
bool ShiftDVec::divisibleBy
  ( ShiftDVec::sTObject * p1, 
    ShiftDVec::sTObject * p2, int& s0, int numVars )
{
  p1->SetDVecIfNULL();
  p2->SetDVecIfNULL();

  return divisibleBy
    (p1->dvec, p1->dvSize, p2->dvec, p2->dvSize, numVars );
}
#endif


/* Tests if the LObject's polynomial lcm is divisible by a
 * shift s of TObject's polynomial p. Returns this shift s.
 * Sets the dvecs, if they are NULL.
 *
 * Has to be tested!
 */
uint ShiftDVec::lcmDivisibleBy
  ( LObject * lcm, ShiftDVec::sTObject * p, 
    int numVars                             )
{
  initDeBoGri
    ( ShiftDVec::indent, 
      "Entering lcmDivisibleBy", "Leaving lcmDivisibleBy", 16 );
  lcm->SetLcmDvecIfNULL();
  p->SetDVecIfNULL();

  deBoGriPrint(lcm->dvec, lcm->dvSize, "lcm->dvec: ", 16);
  deBoGriPrint(lcm->dvSize, "lcm->lcmdvSize: ", 16);
  deBoGriPrint(p->dvec, p->dvSize, "p->dvec: ", 16);
  deBoGriPrint(p->dvSize, "p->dvSize: ", 16);

  return divisibleBy
    ( lcm->lcmDvec, lcm->lcmDvSize, p->dvec, p->dvSize, numVars );
}


/* Finds all shifts sh, where the letterplace objects t1 and
 * st2 := sh * t2 have a common divisor.
 * into shifts. Any previous contents of shifts will be 
 * deleted. Central Overlaps, i.e. shifts where st2 divides t1,
 * are not considered. The found shifts are stored in uint **
 * overlaps. The number of the found shifts, thus the size of
 * uint ** overlaps is returned.
 * Do we only get overlaps where sh * t2 is not violating the
 * maxDeg? How do we test that?
 * The current implementation does not yet care for maxDeg.
 * We should have to change maxElCompared.
 * overlaps have to be freed manually (with omFreeSize)!
 */
uint ShiftDVec::findRightOverlaps
  ( ShiftDVec::sTObject * t1, ShiftDVec::sTObject * t2, 
    int numVars, int maxDeg, uint ** overlaps )
{
  t1->SetDVecIfNULL(); t2->SetDVecIfNULL();
  assume(t1->dvSize > 0 && t2->dvSize > 0);
  //TODO: t1->dvSize-1 should be sufficient because we dont want
  //central overlaps
  *overlaps = (uint*)omAlloc0((t1->dvSize+1)*sizeof(uint));
  loGriToFile
    ("omAlloc0 for overlaps in findRightOverlaps ",
     (t1->dvSize+1)*sizeof(uint), 1024, (void*) (*overlaps) );

  memcpy
    ((void*)(*overlaps), (void*)t1->dvec, t1->dvSize*sizeof(uint));



  //uint maxElCompared = min(t1.dvSize, maxDeg); //not correct
  uint maxElCompared = t1->dvSize; //eventuell zuviele shifts

  /* We want only to find right overlappings, not central
   * overlappings.                                        */
  int shift =
    t2->dvSize > t1->dvSize ? 1 : t1->dvSize - t2->dvSize + 1;

  int cmpLen = (t1->dvSize - shift) * sizeof(uint);
  uint numberOfShifts = 0;
  for(uint i = 0; i < shift; ++i)
    (*overlaps)[shift] += (*overlaps)[i];
  (*overlaps)[shift] -= (shift * numVars);
  for(uint i = 0; shift < maxElCompared; ++shift, cmpLen -= sizeof(uint)){
    if(!memcmp((void*)((*overlaps)+shift),(void*)t2->dvec,cmpLen)){
      (*overlaps)[shift+1] = 
        (*overlaps)[shift] + (*overlaps)[shift+1] - numVars;
      (*overlaps)[i] = shift; i = ++numberOfShifts;
    } else {
      (*overlaps)[shift+1] = 
        (*overlaps)[shift] + (*overlaps)[shift+1] - numVars;
    }
  }

  if( numberOfShifts > 0 ){
    (*overlaps) = (uint*) omrealloc
      ( (*overlaps), numberOfShifts*sizeof(uint) );
    loGriToFile
      ( "Reallocation for overlaps in findRightOverlaps ",
        numberOfShifts*sizeof(uint), 1024, (*overlaps)     );
  }else{
    loGriToFile
      ( "omFreeSize for overlaps in findRightOverlaps ",
        (t1->dvSize+1)*sizeof(uint), 1024, (void*) *overlaps );
    omFreeSize
      ((ADDRESS)(*overlaps), sizeof(uint)*(t1->dvSize+1));
  }

  return numberOfShifts;
}


/* Returns true, if reduction of poly a with poly b would
 * violate the the degree bound. (to be more exact: We test, if
 * the total degree of b*lm(a)/lm(b) is greater than uptodeg.)
 * Will always return false, if uptodeg == 0.
 * WARNING/TODO:
 * - Recently changed! Has to be tested again. Is it right, how
 *   we handle Rings?
 * - This is not very perfect; Even if redViolatesDeg returns
 *   true, the polynomial after reduction may not violate the
 *   degree bound.
 */
BOOLEAN ShiftDVec::redViolatesDeg
  ( poly a, poly b, int uptodeg,
    ring aLmRing, ring bLmRing, ring bTailRing )
{
  initDeBoGri
    ( ShiftDVec::indent, 
      "Entering redViolatesDeg.", "Leaving redViolatesDeg.", 1 );
  if(!uptodeg || !b) return FALSE;
  //or should we return true, if !b ?
  
  int tg_lm_a = p_Totaldegree(a, aLmRing);
  int tg_lm_b = p_Totaldegree(b, bLmRing);

  int tg_b    = tg_lm_b;
  while(pIter(b))
  {
    int tg_lm_itb = p_Totaldegree(b, bTailRing);
    tg_b = tg_b > tg_lm_itb ? tg_b : tg_lm_itb;
  }

  deBoGriPrint
    ( "Reduction viol. deg. bound.", 1, 
      !(tg_b + tg_lm_a - tg_lm_b > uptodeg) );
  deBoGriPrint
    ( a, "poly a: ", 1, 
      !(tg_b + tg_lm_a - tg_lm_b > uptodeg) );
  deBoGriPrint
    ( b, "poly b: ", 1, 
      !(tg_b + tg_lm_a - tg_lm_b > uptodeg) );

  return tg_b + tg_lm_a - tg_lm_b > uptodeg;
}


/* Returns true, if reduction of poly a with poly b would
 * violate the the degree bound. (to be more exact: We test, if
 * the total degree of b*lm(a)/lm(b) is greater than uptodeg.)
 * Will always return false, if uptodeg == 0.
 * WARNING/TODO:
 * - Recently changed! Has to be tested again. Is it right, how
 *   we handle Rings?
 * - This is not very perfect; Even if redViolatesDeg returns
 *   true, the polynomial after reduction may not violate the
 *   degree bound.
 */
BOOLEAN ShiftDVec::redViolatesDeg
  ( TObject* a, TObject* b, int uptodeg, ring lmRing )
{
  initDeBoGri
    ( ShiftDVec::indent, 
      "Entering redViolatesDeg v2.", 
      "Leaving redViolatesDeg v2.", 1 );

  int tg_a, tg_b;
  poly it_b;
  ring r_a, r_b;

  if( a->p == NULL )
  {
    if( a->t_p == NULL ) return FALSE;
    tg_a = p_Totaldegree(a->t_p, a->tailRing);
  }
  else
    tg_a = p_Totaldegree(a->p, lmRing);

  if( b->p == NULL)
  {
    if( b->t_p == NULL ) return FALSE;
    tg_b = p_Totaldegree(b->t_p, b->tailRing);
    it_b = b->t_p;
    r_b = b->tailRing;
  }
  else
  {
    tg_b = p_Totaldegree(b->p, lmRing);
    it_b = b->p;
    r_b = b->t_p == NULL ? lmRing : b->tailRing;
  }

  while(pIter(it_b))
  {
    int tg_lm_it_b = p_Totaldegree(it_b, r_b);
    tg_b = tg_b > tg_lm_it_b ? tg_b : tg_lm_it_b;
  }

  return tg_b + tg_a - tg_b > uptodeg;
}


/* Returns true, if the creation of the s-poly of poly a and
 * poly b would violate the degree bound, false otherwise.
 * shift has to be the shift of a, such that b and shift a
 * have overlap, example with shift = 3 (x^2yz and y^3x^2)
 *    xxyz | poly a
 * yyyxx   | poly b
 * Will always return false, if uptodeg == 0.
 * WARNING/TODO:
 * Recently changed! Has to be tested again. Is it right, how we
 * handle Rings?
 */
bool ShiftDVec::createSPviolatesDeg
  ( poly a, poly b, uint shifta, int uptodeg,
    ring aLmRing, ring bLmRing, ring aTailRing, ring bTailRing )
{
  initDeBoGri
    ( ShiftDVec::indent, 
      "Entering createSPviolatesDeg", 
      "Leaving createSPviolatesDeg", 1 );
  if(!uptodeg) return false;

  int tg_lm_b = p_Totaldegree(b, bLmRing);
  uint degOverlap = tg_lm_b - shifta + 1;

#if 0 //replaced
  poly it_a = a;
#else //replacement
  poly it_a = pNext(a); //TODO: Does that work with Singular?
#endif

  while(it_a){
    int tg_lm_it_a = p_Totaldegree(it_a, aTailRing);
    if(tg_lm_b + tg_lm_it_a - degOverlap > uptodeg) 
    {
      deBoGriPrint("Creation of s-poly violates deg. bound.", 1);
      deBoGriPrint(a, "poly a: ", 1);
      deBoGriPrint(shifta, "Shift for poly a: ", 1);
      deBoGriPrint(b, "poly b: ", 1);
      deBoGriPrint(uptodeg, "Uptodeg: ", 1);
      deBoGriPrint(tg_lm_b, "Totaldeg of lm(b): ", 1);
      deBoGriPrint(degOverlap, "Degree of overlap: ", 1);
      deBoGriPrint(tg_lm_it_a, "Degree of lm(it_a): ", 1);
      return true;
    }
    pIter(it_a);
  }

  int tg_lm_a = p_Totaldegree(a, aLmRing);

#if 0 //replaced
  poly it_b = b;
#else //replacement
  poly it_b = pNext(b); //TODO: Does that work with Singular?
#endif

  while(it_b){
    int tg_lm_it_b = p_Totaldegree(it_b, bTailRing);
    if(tg_lm_a + tg_lm_it_b - degOverlap > uptodeg)
    {
      deBoGriPrint("Creation of s-poly violates deg. bound.", 1);
      deBoGriPrint(a, "poly a: ", 1);
      deBoGriPrint(shifta, "Shift for poly a: ", 1);
      deBoGriPrint(b, "poly b: ", 1);
      deBoGriPrint(uptodeg, "Uptodeg: ", 1);
      deBoGriPrint(tg_lm_a, "Totaldeg of lm(a): ", 1);
      deBoGriPrint(degOverlap, "Degree of overlap: ", 1);
      deBoGriPrint(tg_lm_it_b, "Degree of lm(it_b): ", 1);
      return true;
    }
    pIter(it_b);
  }

  return false;
}


bool ShiftDVec::shiftViolatesDeg
  (poly p, uint shift, int uptodeg, ring pLmRing, ring pTailRing)
{
  assume(p != NULL);
  int tg_lm_p = p_Totaldegree(p, pLmRing);
  loop{
    if(tg_lm_p + shift > uptodeg) return true;
    if( pIter(p) == NULL ) break;
    tg_lm_p = p_Totaldegree(p, pTailRing);
  }

  return false;
}



//functions for (debug) output



void ShiftDVec::dvecWrite(const poly p, ring r)
{
  uint dvSize = p_Totaldegree(p, r);
  for(int j=1, i=1, l=0; l < dvSize; ++i)
    if(p_GetExp(p,i,r)){Print(" %d ",j);j=1;++l;} else{++j;}
}


void ShiftDVec::dvecWrite(const uint* dvec, uint dvSize)
{
  PrintS("( ");
  for(int i = 0; i < dvSize; ++i){ Print(" %d ", dvec[i]); }
  PrintS(" )");
}


void ShiftDVec::dvecWrite(const TObject* t)
{ dvecWrite(t->dvec, t->dvSize); }


void ShiftDVec::lcmDvecWrite(const LObject* t)
{ dvecWrite(t->lcmDvec, t->lcmDvSize); }


int ShiftDVec::lpDVCase = 0;


//class deBoGriInitializer functions



#if DEBOGRI > 0
int ShiftDVec__deBoGriCnt = 0;
int ShiftDVec::deBoGri = 0;
int ShiftDVec::indent = 0;
int ShiftDVec::indentInc = 2;
int ShiftDVec::isSilenced = false;
int ShiftDVec::debugCounter = 0;
#endif

#if DEBOGRI > 0
ShiftDVec::DeBoGri::DeBoGri
  ( int& indent_ ) : indent(indent_), leaveMsg(NULL)
{ ShiftDVec::indent = indent += ShiftDVec::indentInc; }
#endif

#if DEBOGRI > 0
ShiftDVec::DeBoGri::DeBoGri
  ( int& indent_, const char* startMsg, uint flag_ ) : 
  indent(indent_), leaveMsg(NULL), flag(flag_)
{
  deBoGriPrint( startMsg, flag_ );
  ShiftDVec::indent = indent += ShiftDVec::indentInc;
}
#endif

#if DEBOGRI > 0
ShiftDVec::DeBoGri::DeBoGri
  ( int& indent_, 
    const char* startMsg, const char* leaveMsg_, uint flag_ ) : 
  indent(indent_), leaveMsg(leaveMsg_), flag(flag_)
{
  ShiftDVec__deBoGriCnt++;
  deBoGriPrint( startMsg, flag_ );
  ShiftDVec::indent = indent += ShiftDVec::indentInc;
}
#endif

#if DEBOGRI > 0
ShiftDVec::DeBoGri::~DeBoGri() 
{
  ShiftDVec__deBoGriCnt++;
  ShiftDVec::indent = indent - ShiftDVec::indentInc; 
  deBoGriPrint( leaveMsg, flag );
}
#endif



//debogri debug functions



#if 0  //This has to be changed to make it work again
/* Test if degree violation checks works as expected. This
 * function will do nothing, if DEBOGRI & 1 == 0 .         */
void ShiftDVec::debugPrintDegViolation
  ( const char* where, poly p1, uint shift, 
    poly p2, kStrategy strat                )
{
  namespace SD = ShiftDVec;
  PrintS("\nAt "); Print(where); PrintLn();
  if( shiftViolatesDeg(p1, shift, strat->uptodeg) )
  {
    PrintS("Shift of polynomial\n" );
    pWrite(p1);
    PrintS("would violate the degree.\n");
    Print("Shift: %d\n", shift);
    PrintLn();
  } else if( SD::createSPviolatesDeg
               (p1, p2, shift, strat->uptodeg) )
  {
    poly pTemp = p_LPshift
      ( p1, shift, strat->uptodeg, strat->lV, currRing );

    PrintS("Creation of s-poly would violate the degree:\n");
    PrintS("Poly 1:"); pWrite(p2);
    PrintS("Poly 2:"); pWrite(pTemp);
    PrintLn();

    pDelete(&pTemp);
  } else {
    PrintS("No degree violation\n");
    poly pTemp = p_LPshift
      ( p1, shift, strat->uptodeg, strat->lV, currRing );
    deBoGriPrint("Not shiftet Poly in pair: ", p2, 1);
    deBoGriPrint("Shifted Poly in pair: ", pTemp, 1);
  }
}
#endif

#if DEBOGRI > 0
bool ShiftDVec::deBoGriPrint
  ( const char* description, uint flag, bool assume, int indent )
{
  if(indent < 0){ indent = ShiftDVec::indent; }
  if( ShiftDVec::deBoGri & flag && 
      !assume && !ShiftDVec::isSilenced )
  { 
    for(int i = indent; i; --i) PrintS(" ");
    PrintS(description); PrintLn();
  }
  return assume;
}
#endif

#if DEBOGRI > 0
bool ShiftDVec::deBoGriPrint
  ( const poly p, 
    const char* description, uint flag, bool assume, int indent )
{
  if(indent < 0){ indent = ShiftDVec::indent; }
  if( ShiftDVec::deBoGri & flag && 
      !assume && !ShiftDVec::isSilenced )
  {
    for(int i = indent; i; --i) PrintS(" ");
    PrintS(description); pWrite(p);
  }
  return assume;
}
#endif

#if DEBOGRI > 0
bool ShiftDVec::deBoGriPrint
  ( const poly p, int shift, kStrategy strat,
    const char* description, uint flag, bool assume, int indent )
{
  if(indent < 0){ indent = ShiftDVec::indent; }
  if( ShiftDVec::deBoGri & flag && 
      !assume && !ShiftDVec::isSilenced )
  {
    poly pTemp = p;
    if( shiftViolatesDeg(p, shift, strat->uptodeg) )
    {
      PrintS("DEBOGRIPRINT: Shift would violate degree bound.\n"); 
      PrintS("DEBOGRIPRINT: Will print the polynomial unshifted.\n"); 
    }
    else if(shift != 0)
     {
      pTemp = p_LPshift
        ( p, shift, strat->uptodeg, strat->lV, currRing );
      loGriToFile("p_LPshift in deBoGriPrint",0 ,1024, (void*)pTemp);
     }
    for(int i = indent; i; --i) PrintS(" ");
    PrintS(description); pWrite(pTemp);

    if(p != pTemp)
    { 
     loGriToFile("pDelete in deBoGriPrint",0 ,1024, (void*)pTemp);
     pDelete(&pTemp);
    }
  }
  return assume;
}
#endif

#if DEBOGRI > 0
bool ShiftDVec::deBoGriPrint
  ( const uint* dvec, uint size, 
    const char* description, uint flag, bool assume, int indent )
{
  if(indent < 0){ indent = ShiftDVec::indent; }
  if( ShiftDVec::deBoGri & flag && 
      !assume && !ShiftDVec::isSilenced )
  {
    for(int i = indent; i; --i) PrintS(" ");
    PrintS(description); dvecWrite(dvec, size); PrintLn();
  }
  return assume;
}
#endif

#if DEBOGRI > 0
bool ShiftDVec::deBoGriPrint
  ( int number, 
    const char* description, uint flag, bool assume, int indent )
{
  if(indent < 0){ indent = ShiftDVec::indent; }
  if( ShiftDVec::deBoGri & flag && 
      !assume && !ShiftDVec::isSilenced )
  {
    for(int i = indent; i; --i) PrintS(" ");
    PrintS(description); Print("%d\n", number);
  }
  return assume;
}
#endif

#if DEBOGRI > 0
bool ShiftDVec::deBoGriPrint
  ( void* address, 
    const char* description, uint flag, bool assume, int indent )
{
  if(indent < 0){ indent = ShiftDVec::indent; }
  if( ShiftDVec::deBoGri & flag && 
      !assume && !ShiftDVec::isSilenced )
  {
    for(int i = indent; i; --i) PrintS(" ");
    PrintS(description); Print("%p\n", address);
  }
  return assume;
}
#endif

#if DEBOGRI > 0
bool ShiftDVec::loGriToFile
  ( const char* message, size_t number, uint flag,
    const void* addr, 
    const char* logfile, bool assume, int indent   )
{
  if(indent < 0){ indent = ShiftDVec::indent; }
  if( ShiftDVec::deBoGri & flag 
      && !assume && !ShiftDVec::isSilenced )
  {
#if 0 //Replaced
    std::ofstream toFile;
    toFile.open(logfile, std::ios::out | std::ios::app);
    for(int i = indent; i; --i) toFile << " ";
    toFile << message << number;
    if(addr){ toFile << " Adresse: " << addr << "\n";}
    else    { toFile << "\n"; }
    toFile.close();
#else //Replacement
    //TODO: addr als Adresse printen
    FILE* log = fopen(logfile, "a");
    for(int i = indent; i; --i) fprintf(log, " ");
    fprintf(log, "%s%d Adresse: %p\n", message, number, addr);
    fclose(log);
#endif
  }
  return assume;
}
#endif

#if DEBOGRI > 0 && HAVE_DEBOGRITEST
void ShiftDVec::deBoGriTTest(kStrategy strat)
{
  if (strat->T != NULL)
  {
    for (int i=0; i<=strat->tl; i++)
    {
      kTest_T(&(strat->T[i]), strat->tailRing, i, 'T');
    }
  }
}
#endif