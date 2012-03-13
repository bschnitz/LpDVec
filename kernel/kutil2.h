/* Preamble
 * Definitions for the letterplace DVec implementations
 * Last Change: 2011-09-06 15:00
 * Maintainers: Grischa Studzinski and Benjamin Schnitzler 
 */

/* TODO:
 * a lot
 */

#ifndef KUTIL2_H
#define KUTIL2_H

#define HAVE_SHIFTBBADVEC

//BOCO: Our personal debugging output
//0 : RECOMMENDED for not-developers
//    no debugging output; removes all in DEBOGRI defined debug
//    functions from the code, which is recommended, if you do
//    not want to debug the code.
//1 : entering of pairs - check degree violation
//4 : debugging of Gebauer-Moeller
//8 : debugging of the initialization of LObjects
//16 : debugging of lcm related things
//32 : debugging of missing pairs
//64 : debugging of chainCrit
//128 : Which pairs are sorted out by some criterion?
//      - at the moment, this considers only GM3.
//256 : debugging divisiblility/ reduction related things
//512 : debugging Creation of DVecs
//1024 : Debugging memory corruption
#define DEBOGRI 0
//TODO: 
// - Introduce a singular option for setting debogri whilst in
//   debug mode.
// - Create a function, which can turn on/off debug output for
//   all debogris, local to the current function (works together
//   with initdebogri?)


//BOCO: This was an idea, but it is cancelled at the moment
//#define USE_DVEC_LCM


#include <kernel/mod2.h> //Needed because of include order

namespace ShiftDVec{
  uint CreateDVec
    (poly p, uint numberOfVariables, ring r, uint*& dvec);
}

#include <kernel/kutil.h>

typedef unsigned int uint;


namespace ShiftDVec{
  class sLObject;
  class sTObject;

  uint getShift
    (poly p, uint numFirstVar, ring r = currRing );

  bool compareDVec
      ( const uint* dvec, poly p, 
        uint offset, uint maxSize, ring r = currRing );

  uint divisibleBy
    ( const uint* dvec1, uint dvSize1, 
      const uint* dvec2, uint dvSize2, int numVars );

  uint divisibleBy
    ( const uint* dvec1, uint dvSize1, 
      const uint* dvec2, uint dvSize2, 
      uint minShift, uint maxShift, int numVars );

  uint divisibleBy
    ( ShiftDVec::sTObject * t1, 
      ShiftDVec::sTObject * t2, int numVars );

  uint findRightOverlaps
    ( ShiftDVec::sTObject * t1, ShiftDVec::sTObject * t2, 
      int numVars, int maxDeg, uint ** overlaps );

  bool redViolatesDeg(poly a, poly b, int uptodeg);
  bool createSPviolatesDeg
    (poly a, poly b, uint shift, int uptodeg);
  bool shiftViolatesDeg(poly p, uint shift, int uptodeg);

  void dvecWrite(const poly p, ring r = currRing);

  void dvecWrite(const uint* dvec, uint dvSize);

  void dvecWrite(const sTObject* t);

  void lcmDvecWrite(const sLObject* t);
};


#if DEBOGRI > 0

#if DEBOGRI & 1
//This has to be changed to make it work again
//void debugPrintDegViolation
//  (const char* where, poly p1, uint shift, poly p2, kStrategy);
#define debugPrintDegViolation(x1, x2, x3, x4, x5) ((void) 0)
#else
#define debugPrintDegViolation(x1, x2, x3, x4, x5) ((void) 0)
#endif

namespace ShiftDVec
{
  class DeBoGri;

  bool deBoGriPrint
    ( const char* description, 
      uint flag, bool assume = false, int indent = -1 );
  bool deBoGriPrint
    ( int number, const char* description, 
      uint flag, bool assume = false, int indent = -1 );
  bool deBoGriPrint
    ( void* address, const char* description, 
      uint flag, bool assume = false, int indent = -1 );
  bool deBoGriPrint
    ( const poly p, const char* description, 
      uint flag, bool assume = false, int indent = -1 );
  bool deBoGriPrint
    ( const uint* dvec, uint size, 
      const char* description, 
      uint flag, bool assume = false, int indent = -1 );
  bool deBoGriPrint
    ( const poly p, int shift, kStrategy strat,
      const char* description, 
      uint flag, bool assume = false, int indent = -1 );
  bool loGriToFile
    ( const char* message, size_t number, uint flag,
      const void* addr = NULL, 
      const char* logfile = "logri.log", 
      bool assume = false, int indent = 0            );

#ifdef HAVE_DEBOGRITEST
  void deBoGriTTest(kStrategy strat);
#endif

  extern int indent;
  extern int indentInc;
  extern int isSilenced;
  extern int debugCounter;
};

class ShiftDVec::DeBoGri
{
  private:
    bool silenced;
    int flag;
    int& indent;
    const char* leaveMsg;

  public:
    DeBoGri(int& indent_);
    DeBoGri(int& indent_, const char* startMsg, uint flag);
    DeBoGri
      (int& indent_, const char* startMsg, const char* leaveMsg, uint flag);
    ~DeBoGri();

    void silence(){ ShiftDVec::isSilenced = silenced = true; }
    void speak(){ ShiftDVec::isSilenced = silenced = false; }
    void silenceIf(bool test)
      { ShiftDVec::isSilenced = silenced = test; }
    void speakIf(bool test)
      { ShiftDVec::isSilenced = silenced = !test; }
    bool isSilenced(){ return silenced; }
};

#define initDeBoGri(indentRef, startMsg, leaveMsg, flag) \
  ShiftDVec::DeBoGri debogriDbgObj(indentRef, startMsg, leaveMsg, flag)

#define deBoGriSilence() \
  debogriDbgObj.silence()

#define deBoGriSpeak() \
  debogriDbgObj.speak()

#define deBoGriSilenceIf(test) \
  debogriDbgObj.silenceIf(test)

#define deBoGriSpeakIf(test) \
  debogriDbgObj.speakIf(test)

#define deBoGriInitCounter() \
  static int counter = 0

#define deBoGriIncCounter() \
  ++counter

#define deBoGriPrintCounter(description, flag) \
  deBoGriPrint(counter, description, flag)

#ifndef HAVE_DEBOTRITEST
#define deBoGriTTest(x1) ((void) 0)
#endif

#else //#if DEBOGRI > 0 i.e DEBOGRI <= 0

#define deBoGriTTest(x1) ((void) 0)
#define loGriToFile(...) ((void) 0)
#define initDeBoGri(...) ((void) 0)
#define deBoGriPrint(...) ((void) 0)
#define deBoGriSpeak() ((void) 0)
#define deBoGriSilence() ((void) 0)
#define deBoGriSpeakIf(test) ((void) 0)
#define deBoGriSilenceIf(test) ((void) 0)
#define deBoGriInitCounter() ((void) 0)
#define deBoGriPrintCounter(x2, x3) ((void) 0)
#define deBoGriIncCounter() ((void) 0)

#endif //#if DEBOGRI > 0



class sTObject;

class ShiftDVec::sTObject : public ::sTObject
{
public:
  uint * dvec; //Distance Vector of lm(p)
  uint dvSize; //size of the >>uint * dvec<< array
  //uint pIsInR; //already i_r in ::sTObject

  // constructors
  sTObject(ring r = currRing) : 
    ::sTObject(r), dvec(NULL){ t_p = NULL; }

  sTObject(poly p, ring tailRing = currRing) : 
    ::sTObject(p, tailRing),  dvec(NULL){ t_p = NULL; }

  sTObject(poly p, ring c_r, ring tailRing) : 
    ::sTObject(p, c_r, tailRing),  dvec(NULL){ t_p = NULL; }

  sTObject(sTObject* T, int copy) : 
    ::sTObject(T, copy),  dvec(NULL){ t_p = NULL; }

#if 0
  //destructor - 
  //freeing dvecs automatically was to problematic with singular
  ~sTObject()
  { this->freeDVec(); }
#endif
  
  // Initialize the distance vector of an ShiftDVec::sTObject
  void SetDVec
    (poly p, uint numberOfVariables = 0, ring r = currRing)
  { 
    freeDVec(); 
    dvSize = CreateDVec(p, numberOfVariables, r, dvec); 
  }

  //uses the TObjects polynomial; p has to be a valid polynomial!
  void SetDVec
    (uint numberOfVariables = 0, ring r=currRing)
  { 
    freeDVec();
    dvSize = CreateDVec(this->p, numberOfVariables, r, dvec); 
  }

  void SetDVec(uint* dv) {dvec = dv;}

  uint*  GetDVec(); 
  uint  getDVecAtIndex(uint index) {return dvec[index];}
  uint GetDVsize(); 
  void SetDVecIfNULL();

  int cmpDVec(ShiftDVec::sTObject* toCompare);

  int cmpDVec
    (ShiftDVec::sTObject* T1, uint begin, uint beginT1, uint size);

  int cmpDVecProper
    ( ShiftDVec::sTObject* T1, 
      uint begin, uint beginT1, uint size, int lV );

  void freeDVec(); 

  // may need adjustments
  ShiftDVec::sTObject& operator=
    (const ShiftDVec::sTObject& t);

  uint divisibleBy( ShiftDVec::sTObject * T, 
                    int numVars                    )
  { SetDVecIfNULL(); T->SetDVecIfNULL();
    return ShiftDVec::divisibleBy
      (dvec, dvSize, T->dvec, T->dvSize, numVars); }
};

#endif  //#ifndef KUTIL2_H
