1. compile
gcc DIY_SEOBNRE_main.c LALSimSEOBNRE_DIY.c -lgsl -lgslcblas -o SEOBNRE

mpicc DIY_SEOBNRE_main.c LALSimSEOBNRE_DIY.c -lgsl -lgslcblas -o SEOBNRE -I/share/home/zjcao/Lib/usr/local/include/ -L/share/home/zjcao/Lib/usr/local/lib -lm

2. example
export LD_LIBRARY_PATH=/share/home/zjcao/Lib/usr/local/lib

SEOBNRE --m1 10 --m2 10 --f-min 20.304334346643405 --e0 0.5

These parameters are for LIGO detector which is inherited from SEOBNR code. The above setting corresponds to fM = 0.002.
