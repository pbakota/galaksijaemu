10 H.
15 IFKEY(48)G.15
20 P.AT10, "SZUNYOG"
30 P."A FELADAT HOGY A NYIL BILLENYU-"
40 P."KKEL IRANJITSD AZ EMBEREDET    "
50 P."HOGY OSSZESZEDJE A SZUNYOGOKAT "
70 P."A LAKASBAN":P.
80 P."A KEZDESHEZ USD LE AZ <ENTER>"
90 P."BILLENTYUT"
100 IFKEY(48)G.110:E.G.100
110 H.:S=&2800:P.AT0,"PONT:";:P.AT10,"IDO:";
115 A=16:B=8:P=0:I=1000
120 X=INT(RND*32):Y=1+INT(RND*14)
125 B.S+Y*32+X,&2A
135 IFA=XG.137:E.G.139
137 IFB=YG.280
139 B.S+B*32+A,&2B:P.AT5,P
140 I=I-1:P.AT14,I
145 IFKEY(27):G.240
150 IFKEY(28):G.260
160 IFKEY(29):G.200
170 IFKEY(30):G.220
180 G.140
200 B.S+B*32+A,&20:IFA>0A=A-1
210 G.135
220 B.S+B*32+A,&20:IFA<31A=A+1
230 G.135
240 B.S+B*32+A,&20:IFB>1B=B-1
250 G.135
260 B.S+B*32+A,&20:IFB<15B=B+1
270 G.135
280 P=P+1:G.120
