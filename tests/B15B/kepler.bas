10 REM    KEPLER'S EQUATION
20 REM
25 PI=ATN(1)*4
30 P1=PI : D7=P1/180 : R1=1/D7
40 K=.01720209895
50 GOSUB 1060
60 REM
70 PRINT "PERIHELION DISTANCE Q: "; : INPUT Q
90 PRINT "ECCENTRICITY           "; : INPUT E0
92 PRINT "STARTING AND ENDING T  "; : INPUT T7,T8
95 FOR T=T7 TO T8 STEP .1
100 A1=Q/(1-E0) : IF A1<0 THEN M=0 : GOTO 120
110 N0=K*A1^(-1.5) : M=N0*T
120 PRINT "MEAN ANOMALY IS ";M*R1;" DEGREES"
122 PRINT  : PRINT 
123 PRINT "METHOD         I         V         R"
124 PRINT 
130 IF A1<0 THEN 205
140 IF E0>.99 THEN 170
150 PRINT "SIMPLE   ";
160 GOSUB 490 : GOSUB 400
170 PRINT "ENCKE    ";
180 GOSUB 560 : GOSUB 400
190 PRINT "BINARY   ";
200 GOSUB 260 : GOSUB 400
205 REM
210 PRINT "HERRICK  ";
220 GOSUB 640 : GOSUB 440
230 PRINT "GAUSS    ";
240 GOSUB 850 : GOSUB 440
245 NEXT T
250 GOTO 2000
260 REM    BINARY METHOD
270 REM
280 F=SGN(M) : M=ABS(M)/(2*P1) : M=(M-INT(M))*2*P1*F
290 IF M<0 THEN M=M+2*P1
300 F=1 : IF M>P1 THEN M=2*P1-M : F=-1
310 E=P1/2 : D=P1/4
320 FOR I1=1 TO 47
330 M1=E-E0*SIN(E)
340 E=E+SGN(M-M1)*D : D=D/2
350 E1=E
360 NEXT I1
370 E=E*F : I=I1-1
380 RETURN
390 REM
400 REM   TRUE ANOMALY FROM ECCENTRIC ANOMALY
410 REM
420 V=2*ATN(SQR((1+E0)/(1-E0))*SIN(E/2)/COS(E/2))
430 R=A1*(1-E0*COS(E))
440 PRINT USING "   ####    ";I;USING"###.######   ";V*R1;USING"##.#######";R
470 RETURN
480 REM
490 REM   SIMPLE ITERATION
500 REM
510 E=M : I=0
520 E1=M+E0*SIN(E) : I=I+1
530 IF ABS(E-E1)>1.0000000000000E-10 THEN E=E1 : GOTO 520
540 RETURN
550 REM
560 REM   ENCKE ITERATION
570 REM
580 E=M : I=0
590 E1=E+(M+E0*SIN(E)-E)/(1-E0*COS(E)) : I=I+1
600 IF ABS(E-E1)>1.0000000000000E-10 THEN E=E1 : GOTO 590
610 RETURN
620 REM
640 REM    HERRICK'S METHOD
650 REM
660 REM    ENTER WITH T,E0,Q
670 REM    EXIT WITH TRUE ANOMALY
680 REM    
682 I=0
690 P=Q*(1+E0)
700 C=K*(1+E0)*(1+E0)/(2*P^1.5)
710 X=SGN(T) : REM  ASSUME V +90 OR -90 TO START
720 L=(1-E0)/(1+E0)
730 D2=C*(1+L*X*X)/(1+X*X)
740 C2=X/(1+L*X*X) : K1=X*X*X/(1+E0)
750 C2=C2+K1*(1/(1+L*X*X)-1/3)
760 N=0 : S=-1
770 N=N+1
775 I=I+1
780 S=-S
790 F=K1*S*(L*X*X)^N/(2*N+3)
800 C2=C2+F : IF ABS(F)>1.0000000000000E-12 THEN 770
810 D3=T-C2/C : IF ABS(D3)<1.0000000000000E-10 THEN 830
820 X=X+D2*D3 : GOTO 730
830 V=2*ATN(X) : R=Q*(1+E0)/(1+E0*COS(V))
840 RETURN
850 REM   GAUSS METHOD
860 REM
870 REM   ENTER WITH T,E0,Q; EXIT WITH V,R
880 REM
882 I=0
890 A=SQR((1+9*E0)/10) : B=5*(1-E0)/(1+9*E0)
900 C=SQR(5*(1+E0)/(1+9*E0))
910 B1=3*A*K*T/SQR(2*Q*Q*Q)
920 B2=1 : REM   INITIAL ASSUMPTION
930 W1=B2*B1
940 B3=ATN(2/W1) : T1=SIN(B3/2)/COS(B3/2)
950 S1=SGN(T1) : T1=ABS(T1) : T2=T1^(1/3)*S1
960 G=ATN(T2) : S=2/(SIN(2*G)/COS(2*G))
970 A2=B*S*S
980 B0=B2 : B2=0
982 FOR J=0 TO 7 : B2=B2+B(J)*A2^J : NEXT J
984 I=I+1
990 IF ABS(B2-B0)>1.0000000000000E-13 THEN 930
1000 C1=0 : FOR J=0 TO 7 : C1=C1+G(J)*A2^J : NEXT J
1010 V1=C*C1*S
1020 D1=1/(1+A2*C1*C1) : R=Q*D1*(1+V1*V1)
1030 V=2*ATN(V1)
1040 RETURN
1050 REM
1060 REM   COEFFICIENTS
1080 DIM B(7):FOR J=0 TO 7 : READ B(J) : NEXT J
1090 DIM G(7):FOR J=0 TO 7 : READ G(J) : NEXT J
1100 DIM S(7):FOR J=0 TO 7 : READ S(J) : NEXT J
1110 RETURN
1120 DATA 1,0,-0.017142857,-0.003809524,-0.001104267
1130 DATA -0.000367358,-0.000131675,-0.000049577
1140 DATA 1,0.4,0.21714286,0.12495238,0.07339814
1150 DATA 0.04351610,0.02592289,0.01548368
1160 DATA 1,-0.8,0.04571429,0.01523810,0.00562820
1170 DATA 0.00218783,0.00087905,0.00036155
2000 END
2010 REM  ------------------------------------------
2020 REM  APPEARED IN ASTRONOMICAL COMPUTING, SKY &
2030 REM  TELESCOPE, AUGUST, 1985
2040 REM  ------------------------------------------
