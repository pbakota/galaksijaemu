10 HOME
15 IF KEY(48) GOTO 15
20 PRINTAT10, "SZUNYOG"
30 PRINT "A FELADAT HOGY A NYIL BILLENYU-"
40 PRINT "KKEL IRANJITSD AZ EMBEREDET    "
50 PRINT "HOGY OSSZESZEDJE A SZUNYOGOKAT "
70 PRINT "A LAKASBAN":PRINT
80 PRINT "A KEZDESHEZ USD LE AZ <ENTER>"
90 PRINT "BILLENTYUT"
100 IF KEY(48) GOTO 110: ELSE GOTO 100
110 HOME:S=&2800:PRINTAT0,"PONT:"
115 A=16:B=8:P=0
120 X=INT(RND*32):Y=1+INT(RND*14)
125 BYTE S+Y*32+X,&2A
135 IF A=X GOTO 137:ELSE GOTO 139
137 IF B=Y GOTO 280
139 BYTE S+B*32+A,&2B:PRINTAT5,P
140 IF KEY(27):GOTO240
150 IF KEY(28):GOTO260
160 IF KEY(29):GOTO200
170 IF KEY(30):GOTO220
180 GOTO 140
200 BYTE S+B*32+A,&20:IF A>0 A=A-1
210 GOTO 135
220 BYTE S+B*32+A,&20:IF A<31 A=A+1
230 GOTO 135
240 BYTE S+B*32+A,&20:IF B>1 B=B-1
250 GOTO 135
260 BYTE S+B*32+A,&20:IF B<14 B=B+1
270 GOTO 135
280 P=P+1:GOTO120
