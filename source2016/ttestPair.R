# ttestPair.R
x = scan("dataA.dat")
y = scan("dataB.dat")
t.test(x,y,paired=T)
