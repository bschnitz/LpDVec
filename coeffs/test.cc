#include "config.h"

#include <auxiliary.h>

#include <coeffs.h>
#include <numbers.h>
#include <reporter.h>
#include <omalloc.h>

#include <longrat.h>
#include <gnumpfl.h>
#include <gnumpc.h>
#include <shortfl.h>
#include <ffields.h>
#include <modulop.h>
#include <rmodulon.h>
#include <rmodulo2m.h>
#include <rintegers.h>

#include <iostream>
using namespace std;


bool Test(const coeffs r)
{
  number a = n_Init(666, r); 
  n_Test(a,r);
  
  number b = n_Add(a, a, r);
  n_Test(b,r);

  StringSetS("a: "); n_Write(a, r);PrintS(StringAppend("\n"));
  StringSetS("b: "); n_Write(b, r);PrintS(StringAppend("\n"));

  n_Delete(&a, r);
  n_Delete(&b, r);

  return false;
}



bool Test(const n_coeffType type, void* p = NULL)
{

  cout  << endl << "----------------------- Testing coeffs: [" << type <<
                "]: -----------------------" << endl;

  const coeffs r = nInitChar( type, p );

  assume( r != NULL );

  nSetChar( r );

  assume( getCoeffType(r) == type );

  assume( r->cfInit != NULL );
  assume( r->cfWrite != NULL );
  assume( r->cfAdd != NULL );
  assume( r->cfDelete != NULL );


  if( type == n_Q )
  {
    assume( r->cfInit == nlInit );
    assume( r->cfWrite == nlWrite );
    assume( r->cfAdd == nlAdd );
    assume( r->cfDelete == nlDelete );    
  }
  else if( type == n_long_R )
  {
    assume( r->cfInit == ngfInit );
    assume( r->cfWrite == ngfWrite );
    assume( r->cfAdd == ngfAdd );
    assume( r->cfDelete == ngfDelete );
  }
  else if( type == n_long_C )
  {
    assume( r->cfInit == ngcInit );
    assume( r->cfWrite == ngcWrite );
    assume( r->cfAdd == ngcAdd );
    assume( r->cfDelete == ngcDelete );    
  }
  else if( type == n_R )
  {
    assume( r->cfInit == nrInit );
    assume( r->cfWrite == nrWrite );
    assume( r->cfAdd == nrAdd );
//    assume( r->cfDelete == nrDelete ); // No?
  }
#ifdef HAVE_RINGS
  else if( type == n_Z2m )
  {
    assume( r->cfInit == nr2mInit );
    assume( r->cfWrite == nr2mWrite );
    assume( r->cfAdd == nr2mAdd );
    assume( r->cfDelete == ndDelete );
  }
  else if( type == n_Zn )
  {
    assume( r->cfInit == nrnInit );
    assume( r->cfWrite == nrnWrite );
    assume( r->cfAdd == nrnAdd );
    assume( r->cfDelete == nrnDelete );
  }
#endif  
  else
  {
    // ...
  }

  bool ret = Test( r );

  nKillChar( r );

  return ret;
}




int main()
{
  int c = 0;
  
  n_coeffType type;

  // rings needed for: n_Zp_a, n_Q_a ?

  type = nRegister( n_Zp, npInitChar); assume( type == n_Zp );
  if( Test(type, (void*) 11) )
    c ++;

  
  type = nRegister( n_Q, nlInitChar); assume( type == n_Q );
  if( Test(type) )
    c ++;

  type = nRegister( n_R, nrInitChar); assume( type == n_R );
  if( Test(type) )
    c ++;

  type = nRegister( n_long_R, ngfInitChar); assume( type == n_long_R );
  if( Test(type) )
    c ++;

  type = nRegister( n_long_C, ngcInitChar); assume( type == n_long_C );
  if( Test(type) )
    c ++;


#ifdef HAVE_RINGS
  type = nRegister( n_Z, nrzInitChar); assume( type == n_Z );
  if( Test(type) )
    c ++;

  type = nRegister( n_Z2m, nr2mInitChar); assume( type == n_Z2m );
  if( Test(type, (void*) 2) )
    c ++;

  type = nRegister( n_Zn, nrnInitChar); assume( type == n_Zn );

  // BUG here!
  // TODO: Frank (cmp. with the previous code)
  // Note: the parameter 'm' is ignored now in nrnSetExp!!!
  if( Test(type, (void*) 3) )
    c ++;

#endif

/*  
  // TODO: Hans (the following needs resources, e.g. feFopen)
  type = nRegister( n_GF, nfInitChar); assume( type == n_GF );
  if( Test(type) )
    c ++;
*/
  
  return c;

}