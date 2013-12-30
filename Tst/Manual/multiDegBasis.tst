LIB "tst.lib"; tst_init(); /* tst_ignore("4ti2"); */
LIB "multigrading.lib";
ring R = 0, (x, y), dp;
intmat g1[2][2]=1,0,0,1;
intmat l[2][1]=2,0;
intmat g2[2][2]=1,1,1,1;
intvec v1=4,0;
intvec v2=4,4;
intmat g3[1][2]=1,1;
setBaseMultigrading(g3);
intvec v3=4:1;
v3;
multiDegBasis(v3);
setBaseMultigrading(g1,l);
multiDegBasis(v1);
setBaseMultigrading(g2);
multiDegBasis(v2);
intmat M[2][2] = 1, -1, -1, 1;
intvec d = -2, 2;
setBaseMultigrading(M);
multiDegBasis(d);
attrib(_, "ZeroPart");
kill R, M, d;
ring R = 0, (x, y, z), dp;
intmat M[2][3] = 1, -2, 1,     1, 1, 0;
intmat L[2][1] = 0, 2;
intvec d = 4, 1;
setBaseMultigrading(M, L);
multiDegBasis(d);
attrib(_, "ZeroPart");
kill R, M, d;
ring R = 0, (x, y, z), dp;
qring Q = std(ideal( y^6+ x*y^3*z-x^2*z^2 ));
intmat M[2][3] = 1, 1, 2,     2, 1, 1;
//  intmat T[2][1] = 0, 2;
setBaseMultigrading(M); // BUG????
intvec d = 6, 6;
multiDegBasis(d);
attrib(_, "ZeroPart");
kill R, Q, M, d;
ring R = 0, (x, y, z), dp;
qring Q = std(ideal( x*z^3 - y *z^6, x*y*z  - x^4*y^2 ));
intmat M[2][3] = 1, -2, 1,     1, 1, 0;
intmat T[2][1] = 0, 2;
intvec d = 4, 1;
setBaseMultigrading(M, T); // BUG????
multiDegBasis(d);
attrib(_, "ZeroPart");
tst_status(1);$
