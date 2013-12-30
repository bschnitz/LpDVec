LIB "tst.lib"; tst_init();
LIB "elim.lib";
ring R=(0,a),x(1..3),dp;
minpoly = a2+1;

int n = 3;
int m = 2;
  list rl = ringlist(R);
   int k;
   for(k = 1; k <= n; k++)
     {
        rl[2][k] = "x("+string(k)+")";
     }
   for(k = 1; k <= m; k++)
     {
        rl[2][k+n] = "y("+string(k)+")";
     }
   rl[3]= list(list("dp",1:(n+m)),list("C",0));
   /*Use Hilbert driven Buchberger*/
   def Rbig0 = ring(rl);
   setring Rbig0;


matrix HM[2][2]=y(1),
(85487396464080*a-672629646755040)*x(1)^4+(1486235684289200*a-221204675596320)*x(1)^3*x(2)+(392179468524480*a+140976390779120)*x(1)^2*x(2)^2+(1486235684289200*a-221204675596320)*x(1)*x(2)^3+(306692072060400*a+813606037534160)*x(2)^4+(417625502236920*a-62157512916432)*x(1)^3*x(3)+(270683195894300*a-40287276890280)*x(1)^2*x(2)*x(3)+(-989927116413440*a+147336326913024)*x(1)*x(2)^2*x(3)+(-232014167909400*a+34531951620240)*x(2)^3*x(3)+(-5037031828686240*a+1904524949647680)*x(1)^2*x(3)^2+(-1643951241259920*a+3502964647809600)*x(1)*x(2)*x(3)^2+(920076216181200*a+2440818112602480)*x(2)^2*x(3)^2+(-1392085007456400*a+207191709721440)*x(1)*x(3)^3+(-696042503728200*a+103595854860720)*x(2)*x(3)^3,
y(2),
-655628472*x(1)^3*x(3)-424944380*x(1)^2*x(2)*x(3)+1554082304*x(1)*x(2)^2*x(3)+364238040*x(2)^3*x(3)+2185428240*x(1)*x(3)^3+1092714120*x(2)*x(3)^3;

ideal I =x(1)^6+3*x(1)^4*x(2)^2+3*x(1)^2*x(2)^4+x(2)^6-4*x(1)^4*x(3)^2-34*x(1)^3*x(2)*x(3)^2-7*x(1)^2*x(2)^2*x(3)^2+12*x(1)*x(2)^3*x(3)^2+6*x(2)^4*x(3)^2+36*x(1)^2*x(3)^4+36*x(1)*x(2)*x(3)^4+9*x(2)^2*x(3)^4;

ideal phi=(85487396464080*a-672629646755040)*x(1)^4+(1486235684289200*a-221204675596320)*x(1)^3*x(2)+(392179468524480*a+140976390779120)*x(1)^2*x(2)^2+(1486235684289200*a-221204675596320)*x(1)*x(2)^3+(306692072060400*a+813606037534160)*x(2)^4+(417625502236920*a-62157512916432)*x(1)^3*x(3)+(270683195894300*a-40287276890280)*x(1)^2*x(2)*x(3)+(-989927116413440*a+147336326913024)*x(1)*x(2)^2*x(3)+(-232014167909400*a+34531951620240)*x(2)^3*x(3)+(-5037031828686240*a+1904524949647680)*x(1)^2*x(3)^2+(-1643951241259920*a+3502964647809600)*x(1)*x(2)*x(3)^2+(920076216181200*a+2440818112602480)*x(2)^2*x(3)^2+(-1392085007456400*a+207191709721440)*x(1)*x(3)^3+(-696042503728200*a+103595854860720)*x(2)*x(3)^3,
-655628472*x(1)^3*x(3)-424944380*x(1)^2*x(2)*x(3)+1554082304*x(1)*x(2)^2*x(3)+364238040*x(2)^3*x(3)+2185428240*x(1)*x(3)^3+1092714120*x(2)*x(3)^3;
quotient(I+minor(HM,2),phi);
tst_status(1);$

