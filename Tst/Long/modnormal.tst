LIB "tst.lib";
tst_init();

LIB "modnormal.lib";
// plane curves

ring r24 = 0,(x,y,z),dp;
int k = 2;
poly f = (x^(k+1)+y^(k+1)+z^(k+1))^2-4*(x^(k+1)*y^(k+1)+y^(k+1)*z^(k+1)+z^(k+1)*x^(k+1));
f = subst(f,z,2x-y+1);
ring s24 = 0,(x,y),dp;
poly f = imap(r24,f);
ideal i = f;

//locNormal(i);
modNormal(i,1);


ring r24 = 0,(x,y,z),dp;
int k = 3;
poly f = (x^(k+1)+y^(k+1)+z^(k+1))^2-4*(x^(k+1)*y^(k+1)+y^(k+1)*z^(k+1)+z^(k+1)*x^(k+1));
f = subst(f,z,2x-y+1);
ring s24 = 0,(x,y),dp;
poly f = imap(r24,f);
ideal i = f;

//locNormal(i);
modNormal(i,1,"noVerification");


ring r24 = 0,(x,y,z),dp;
int k = 4;
poly f = (x^(k+1)+y^(k+1)+z^(k+1))^2-4*(x^(k+1)*y^(k+1)+y^(k+1)*z^(k+1)+z^(k+1)*x^(k+1));
f = subst(f,z,2x-y+1);
ring s24 = 0,(x,y),dp;
poly f = imap(r24,f);
ideal i = f;

//locNormal(i);
modNormal(i,1,"noVerification");


ring s24 = 0,(x,y),dp;
int a=7;
ideal i = ((x-1)^a-y^3)*((x+1)^a-y^3)*((x)^a-y^3)*((x-2)^a-y^3)*((x+2)^a-y^3)+y^15;

//locNormal(i);
modNormal(i,1);


ring s24 = 0,(x,y),dp;
int a=8;
ideal i = ((x-1)^a-y^3)*((x+1)^a-y^3)*((x)^a-y^3)*((x-2)^a-y^3)*((x+2)^a-y^3)+y^15;

//locNormal(i);
modNormal(i,1);

ring s24 = 0,(x,y),dp;
int a=9;
ideal i = ((x-1)^a-y^3)*((x+1)^a-y^3)*((x)^a-y^3)*((x-2)^a-y^3)*((x+2)^a-y^3)+y^15;

//locNormal(i);
modNormal(i,1,"noVerification");




ring r=0,(x,y),dp;
ideal i=9127158539954x10+3212722859346x8y2+228715574724x6y4-34263110700x4y6
-5431439286x2y8-201803238y10-134266087241x8-15052058268x6y2+12024807786x4y4
+506101284x2y6-202172841y8+761328152x6-128361096x4y2+47970216x2y4-6697080y6
-2042158x4+660492x2y2-84366y4+2494x2-474y2-1;

//locNormal(i);
modNormal(i,1);


// surfaces in A3


ring r7 = 0,(x,y,t),dp;
int a=11;
ideal i = x*y*(x-y)*(x+y)*(y-1)*t+(x^a-y^2)*(x^10-(y-1)^2);
//locNormal(i);
modNormal(i,1,"noVerification");

ring r7 = 0,(x,y,t),dp;
int a=12;
ideal i = x*y*(x-y)*(x+y)*(y-1)*t+(x^a-y^2)*(x^10-(y-1)^2);
//locNormal(i);
modNormal(i,1,"noVerification");


ring r7 = 0,(x,y,t),dp;
int a=13;
ideal i = x*y*(x-y)*(x+y)*(y-1)*t+(x^a-y^2)*(x^10-(y-1)^2);

//locNormal(i);
modNormal(i,1,"noVerification");


ring r22 = 0,(x,y,z),dp;
ideal i = z2-(y2-1234x3)^2*(15791x2-y3)*(1231y2-x2*(x+158))*(1357y5-3x11);

//locNormal(i);
modNormal(i,1,"noVerification");


ring r22 = 0,(x,y,z),dp;
ideal i = z2-(y2-1234x3)^3*(15791x2-y3)*(1231y2-x2*(x+158))*(1357y5-3x11);

//locNormal(i);
modNormal(i,1,"noVerification");


ring r23 = 0,(x,y,z),dp;
ideal i = z5-((13x-17y)*(5x2-7y3)*(3x3-2y2)*(19y2-23x2*(x+29)))^2;

//locNormal(i);
modNormal(i,1,"noVerification");


// curve in A3

ring r23 = 0,(x,y,z),dp;
ideal i = z3-(19y2-23x2*(x+29))^2,x3-(11y2-13z2*(z+1));

//locNormal(i);
modNormal(i,1,"noVerification");


// surface in A4

ring r23 = 0,(x,y,z,w),dp;
ideal i = z2-(y3-123456w2)*(15791x2-y3)^2, w*z-(1231y2-x*(111x+158));


//locNormal(i);
modNormal(i,1,"noVerification");

tst_status(1);$
