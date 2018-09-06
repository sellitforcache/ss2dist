turtle rssa test
1   0   1 -2 3 -4 5 -1234
2   0   1 -2 3 -4 1234 -6
99  0   (-1:2:-3:4:-5:6)

1     px  -20
2     px   20
3     py  -20
4     py   20
5     pz  -20
6     pz   20
1234  pz    0

mode n p h
imp:n,p,h 1 1 0
ssr
print
phys:n 600.0 600.0 j j j j j 20.0
phys:p 600.02 $  0 0 1
phys:h 600.0 j 0.0
mphys on
c  MESH TALLIES FOR DOSE
TMESH
 rmesh101:h
  cora101   -20  99i 20
  corb101   -20  99i 20
  corc101   -20  99i 20
ENDMD
