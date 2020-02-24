// $Id: Panyicomm_elip.cpp,v 1.1.1.1 2017/09/25 02:35:55 zjcao Exp $
#include <gsl/gsl_vector.h>
#include <gsl/gsl_multiroots.h>
#include <gsl/gsl_integration.h>
#include <gsl/gsl_sf_gamma.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_deriv.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_interp.h>
#include <gsl/gsl_spline.h>
#include "DIY_SEOBNRE_ALL.h"

#ifdef newc
#include <complex>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <cstdarg>
#include <cstring>
#include <cmath>
using namespace std;
#else
#include <complex.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#endif

#define debugOutput 0
#define outputh22 1
#define outputOmega 1
/* Function Declaration */

const LALUnit lalStrainUnit        = {{ 0, 0, 0, 0, 0, 1, 0}, { 0, 0, 0, 0, 0, 0, 0} };    /**< Strain [1] */


void PNwaveformPRD544813rdotc_22mode(double *hr,double *hi,
                                     const double x1,const double x2,const double x3, // test particle position
                                     const double v1,const double v2,const double v3, // test particle velocity
                                     const double eta) // symmetric mass ratio
{
    const double dm = sqrt(1-4*eta);
    double r = sqrt(x1*x1+x2*x2+x3*x3);
    double n1 = x1/r,n2 = x2/r,n3 = x3/r;
    double rdot = v1*n1+v2*n2+v3*n3;
    double vsqr = v1*v1+v2*v2+v3*v3;
    
    double vn1,vn2,vn3;
    vn1 = rdot*n1;
    vn2 = rdot*n2;
    vn3 = rdot*n3;
    double lambda1,lambda2,lambda3;
    lambda1 = v1-vn1;
    lambda2 = v2-vn2;
    lambda3 = v3-vn3;
    double ln = sqrt(lambda1*lambda1+lambda2*lambda2+lambda3*lambda3);
    lambda1 = lambda1/ln;
    lambda2 = lambda2/ln;
    lambda3 = lambda3/ln;
    
    double complex hm22;
    
    double complex h11,h12,h13,h22,h23,h33;
    double complex Q11,Q12,Q13,Q22,Q23,Q33;
    
    ///////////////////////////////////////00 part///////////////////////////////////////
    Q11 = sqrt(M_PI/5)/3;
    Q12 = -sqrt(M_PI/5)/3*I;
    Q13 = 0;
    Q22 = -sqrt(M_PI/5)/3;
    Q23 = 8/sqrt(5*M_PI)/3;
    Q33 = 0;
    
    h11 = 0;
    h12 = 0;
    h13 = 0;
    h22 = 0;
    h23 = 0;
    h33 = 0;
    
    h11 += 1.0/3*(2/r*rdot*(5+3*eta)*(n1*v1+v1*n1)+(3*(1-3*eta)*rdot*rdot)/r*n1*n1);
    h12 += 1.0/3*(2/r*rdot*(5+3*eta)*(n1*v2+v1*n2)+(3*(1-3*eta)*rdot*rdot)/r*n1*n2);
    h13 += 1.0/3*(2/r*rdot*(5+3*eta)*(n1*v3+v1*n3)+(3*(1-3*eta)*rdot*rdot)/r*n1*n3);
    h22 += 1.0/3*(2/r*rdot*(5+3*eta)*(n2*v2+v2*n2)+(3*(1-3*eta)*rdot*rdot)/r*n2*n2);
    h23 += 1.0/3*(2/r*rdot*(5+3*eta)*(n2*v3+v2*n3)+(3*(1-3*eta)*rdot*rdot)/r*n2*n3);
    h33 += 1.0/3*(2/r*rdot*(5+3*eta)*(n3*v3+v3*n3)+(3*(1-3*eta)*rdot*rdot)/r*n3*n3);
    
    hm22 = Q11*h11+Q22*h22+Q33*h33+2.*(Q12*h12+Q13*h13+Q23*h23);
    
    ///////////////////////////////////////10 part///////////////////////////////////////
    Q11 = 0;
    Q12 = 0;
    Q13 = -sqrt(M_PI/5)*(x1 - I*x2)/(3.*r);
    Q22 = 0;
    Q23 = sqrt(M_PI/5)*(I*x1 + x2)/(3.*r);
    Q33 = 0;
    
    h11 = dm*3/r*(-rdot*n1*n1);
    h12 = dm*3/r*(-rdot*n1*n2);
    h13 = dm*3/r*(-rdot*n1*n2);
    h22 = dm*3/r*(-rdot*n2*n2);
    h23 = dm*3/r*(-rdot*n2*n3);
    h33 = dm*3/r*(-rdot*n3*n3);
    
    h11 += dm/12/r*((n1*v1+v1*n1)*(rdot*rdot*(63+54*eta))
                    +n1*n1*rdot*(rdot*rdot*(15-90*eta)-vsqr*(63-54*eta)+(242-24*eta)/r)-rdot*v1*v1*(186+24*eta));
    h12 += dm/12/r*((n1*v2+v1*n2)*(rdot*rdot*(63+54*eta))
                    +n1*n2*rdot*(rdot*rdot*(15-90*eta)-vsqr*(63-54*eta)+(242-24*eta)/r)-rdot*v1*v2*(186+24*eta));
    h13 += dm/12/r*((n1*v3+v1*n3)*(rdot*rdot*(63+54*eta))
                    +n1*n3*rdot*(rdot*rdot*(15-90*eta)-vsqr*(63-54*eta)+(242-24*eta)/r)-rdot*v1*v3*(186+24*eta));
    h22 += dm/12/r*((n2*v2+v2*n2)*(rdot*rdot*(63+54*eta))
                    +n2*n2*rdot*(rdot*rdot*(15-90*eta)-vsqr*(63-54*eta)+(242-24*eta)/r)-rdot*v2*v2*(186+24*eta));
    h23 += dm/12/r*((n2*v3+v2*n3)*(rdot*rdot*(63+54*eta))
                    +n2*n3*rdot*(rdot*rdot*(15-90*eta)-vsqr*(63-54*eta)+(242-24*eta)/r)-rdot*v2*v3*(186+24*eta));
    h33 += dm/12/r*((n3*v3+v3*n3)*(rdot*rdot*(63+54*eta))
                    +n3*n3*rdot*(rdot*rdot*(15-90*eta)-vsqr*(63-54*eta)+(242-24*eta)/r)-rdot*v3*v3*(186+24*eta));
    
    hm22 += Q11*h11+Q22*h22+Q33*h33+2.*(Q12*h12+Q13*h13+Q23*h23);
    
    ///////////////////////////////////////01 part///////////////////////////////////////
    Q11 = 0;
    Q12 = 0;
    Q13 = -sqrt(M_PI/5)*(v1 - I*v2)/3.;
    Q22 = 0;
    Q23 = sqrt(M_PI/5)*(I*v1 + v2)/3.;
    Q33 = 0;
    
    h11 = 0;
    h12 = 0;
    h13 = 0;
    h22 = 0;
    h23 = 0;
    h33 = 0;
    
    h11 += dm*(-(n1*v1+v1*n1)/2/r*rdot*(7+4*eta)
               -n1*n1/r*(0.75*(1-2*eta)*rdot*rdot));
    h12 += dm*(-(n1*v2+v1*n2)/2/r*rdot*(7+4*eta)
               -n1*n2/r*(0.75*(1-2*eta)*rdot*rdot));
    h13 += dm*(-(n1*v3+v1*n3)/2/r*rdot*(7+4*eta)
               -n1*n3/r*(0.75*(1-2*eta)*rdot*rdot));
    h22 += dm*(-(n2*v2+v2*n2)/2/r*rdot*(7+4*eta)
               -n2*n2/r*(0.75*(1-2*eta)*rdot*rdot));
    h23 += dm*(-(n2*v3+v2*n3)/2/r*rdot*(7+4*eta)
               -n2*n3/r*(0.75*(1-2*eta)*rdot*rdot));
    h33 += dm*(-(n3*v3+v3*n3)/2/r*rdot*(7+4*eta)
               -n3*n3/r*(0.75*(1-2*eta)*rdot*rdot));
    
    hm22 += Q11*h11+Q22*h22+Q33*h33+2.*(Q12*h12+Q13*h13+Q23*h23);
    
    ///////////////////////////////////////20 part///////////////////////////////////////
    Q11 = -sqrt(M_PI/5)*(x1*x1-8.*I*x1*x2-7*x2*x2-x3*x3)/21./r/r;
    Q12 = -I*sqrt(M_PI/5)*(3*(x1*x1+x2*x2)+x3*x3)/21./r/r;
    Q13 = 2*sqrt(M_PI/5)*(x1-I*x2)*x3/21./r/r;
    Q22 = sqrt(M_PI/5)*(-7*x1*x1+8.*I*x1*x2+x2*x2-x3*x3)/21./r/r;
    Q23 = -2.*I*sqrt(M_PI/5)*(x1-I*x2)*x3/21./r/r;
    Q33 = 8*sqrt(M_PI/5)*pow(x1-I*x2,2)/21./r/r;
    
    h11 = (1-3*eta)/3/r*((-15*rdot*rdot)*n1*n1+15*rdot*(n1*v1+v1*n1));
    h12 = (1-3*eta)/3/r*((-15*rdot*rdot)*n1*n2+15*rdot*(n1*v2+v1*n2));
    h13 = (1-3*eta)/3/r*((-15*rdot*rdot)*n1*n3+15*rdot*(n1*v3+v1*n3));
    h22 = (1-3*eta)/3/r*((-15*rdot*rdot)*n2*n2+15*rdot*(n2*v2+v2*n2));
    h23 = (1-3*eta)/3/r*((-15*rdot*rdot)*n2*n3+15*rdot*(n2*v3+v2*n3));
    h33 = (1-3*eta)/3/r*((-15*rdot*rdot)*n3*n3+15*rdot*(n3*v3+v3*n3));
    
    hm22 += Q11*h11+Q22*h22+Q33*h33+2.*(Q12*h12+Q13*h13+Q23*h23);
    
    ///////////////////////////////////////11 part///////////////////////////////////////
    Q11 = sqrt(M_PI/5)*(-v1*x1+4.*I*v2*x1+4.*I*v1*x2+7*v2*x2+v3*x3)/21./r;
    Q12 = -I*sqrt(M_PI/5)*(3*v1*x1+3*v2*x2+v3*x3)/21./r;
    Q13 = sqrt(M_PI/5)*(v3*(x1-I*x2)+x3*(v1-I*v2))/21./r;
    Q22 = sqrt(M_PI/5)*(-7*v1*x1+4.*I*v2*x1+4.*I*v1*x2+v2*x2-v3*x3)/21./r;
    Q23 = -sqrt(M_PI/5)*(v3*(I*x1+x2)+x3*(I*v1+v2))/21./r;
    Q33 = 8*sqrt(M_PI/5)*(v1-I*v2)*(x1-I*x2)/21./r;
    
    h11 = (1-3*eta)/3/r*(12*rdot*n1*n1);
    h12 = (1-3*eta)/3/r*(12*rdot*n1*n2);
    h13 = (1-3*eta)/3/r*(12*rdot*n1*n3);
    h22 = (1-3*eta)/3/r*(12*rdot*n2*n2);
    h23 = (1-3*eta)/3/r*(12*rdot*n2*n3);
    h33 = (1-3*eta)/3/r*(12*rdot*n3*n3);
    
    hm22 += Q11*h11+Q22*h22+Q33*h33+2.*(Q12*h12+Q13*h13+Q23*h23);
    
    ///////////////////////////////////////02 part///////////////////////////////////////
    Q11 = sqrt(M_PI/5)*(-v1*v1+8.*I*v1*v2+7*v2*v2+v3*v3)/21.;
    Q12 = -I*sqrt(M_PI/5)*(3*(v1*v1+v2*v2)+v3*v3)/21.;
    Q13 = 2*sqrt(M_PI/5)*(v1-I*v2)*v3/21.;
    Q22 = sqrt(M_PI/5)*(-7*v1*v1+8.*I*v1*v2+v2*v2-v3*v3)/21.;
    Q23 = -2.*I*sqrt(M_PI/5)*(v1-I*v2)*v3/21.;
    Q33 = 8*sqrt(M_PI/5)*pow(v1-I*v2,2)/21.;
    
    h11 = 0;
    h12 = 0;
    h13 = 0;
    h22 = 0;
    h23 = 0;
    h33 = 0;
    
    hm22 += Q11*h11+Q22*h22+Q33*h33+2.*(Q12*h12+Q13*h13+Q23*h23);
    
    ///////////////////////////////////////30 part///////////////////////////////////////
    Q11 = sqrt(M_PI/5)*pow(x1-I*x2,2)*x3/7./r/r/r;
    Q12 = 0;
    Q13 = -sqrt(M_PI/5)*(x1-I*x2)*(2.*x1*x1+5.*I*x1*x2+7*x2*x2+3*x3*x3)/21./r/r/r;
    Q22 = sqrt(M_PI/5)*pow(x1-I*x2,2)*x3/7./r/r/r;
    Q23 = sqrt(M_PI/5)*(I*x1+x2)*(7*x1*x1-5.*I*x1*x2+2*x2*x2+3*x3*x3)/21./r/r/r;
    Q33 = -2*sqrt(M_PI/5)*pow(x1-I*x2,2)*x3/7./r/r/r;
    
    h11 = dm*(1-2*eta)/r*(1.25*(3*vsqr-7*rdot*rdot+6/r)*rdot*n1*n1-8.5*rdot*v1*v1-(-105*rdot*rdot)/12*(n1*v1+v1*n1));
    h12 = dm*(1-2*eta)/r*(1.25*(3*vsqr-7*rdot*rdot+6/r)*rdot*n1*n2-8.5*rdot*v1*v2-(-105*rdot*rdot)/12*(n1*v2+v1*n2));
    h13 = dm*(1-2*eta)/r*(1.25*(3*vsqr-7*rdot*rdot+6/r)*rdot*n1*n3-8.5*rdot*v1*v3-(-105*rdot*rdot)/12*(n1*v3+v1*n3));
    h22 = dm*(1-2*eta)/r*(1.25*(3*vsqr-7*rdot*rdot+6/r)*rdot*n2*n2-8.5*rdot*v2*v2-(-105*rdot*rdot)/12*(n2*v2+v2*n2));
    h23 = dm*(1-2*eta)/r*(1.25*(3*vsqr-7*rdot*rdot+6/r)*rdot*n2*n3-8.5*rdot*v2*v3-(-105*rdot*rdot)/12*(n2*v3+v2*n3));
    h33 = dm*(1-2*eta)/r*(1.25*(3*vsqr-7*rdot*rdot+6/r)*rdot*n3*n3-8.5*rdot*v3*v3-(-105*rdot*rdot)/12*(n3*v3+v3*n3));
    
    hm22 += Q11*h11+Q22*h22+Q33*h33+2.*(Q12*h12+Q13*h13+Q23*h23);
    
    ///////////////////////////////////////21 part///////////////////////////////////////
    Q11 = sqrt(M_PI/5)*(x1-I*x2)*(v3*(x1-I*x2)+2.*(v1-I*v2)*x3)/21./r/r;
    Q12 = 0;
    Q13 = -sqrt(M_PI/5)*((x1-I*x2)*(2*v1*x1+I*x1*v2+4.*I*v1*x2+7*x2*v2)+2*v3*(x1-I*x2)*x3+(v1-I*v2)*x3*x3)/21./r/r;
    Q22 = sqrt(M_PI/5)*(x1-I*x2)*(v3*(x1-I*x2)+2.*(v1-I*v2)*x3)/21./r/r;
    Q23 = sqrt(M_PI/5)*((x1-I*x2)*(v2*(4*x1+2.*I*x2)+v1*(7.*I*x1+x2))+2*v3*(I*x1+x2)*x3+(I*v1+v2)*x3*x3)/21./r/r;
    Q33 = -2*sqrt(M_PI/5)*(x1-I*x2)*(v3*(x1-I*x2)+2.*(v1-I*v2)*x3)/21./r/r;
    
    h11 = dm*(1-2*eta)/4/r*((45*rdot*rdot)*n1*n1-54*rdot*(n1*v1+v1*n1));
    h12 = dm*(1-2*eta)/4/r*((45*rdot*rdot)*n1*n2-54*rdot*(n1*v2+v1*n2));
    h13 = dm*(1-2*eta)/4/r*((45*rdot*rdot)*n1*n3-54*rdot*(n1*v3+v1*n3));
    h22 = dm*(1-2*eta)/4/r*((45*rdot*rdot)*n2*n2-54*rdot*(n2*v2+v2*n2));
    h23 = dm*(1-2*eta)/4/r*((45*rdot*rdot)*n2*n3-54*rdot*(n2*v3+v2*n3));
    h33 = dm*(1-2*eta)/4/r*((45*rdot*rdot)*n3*n3-54*rdot*(n3*v3+v3*n3));
    
    hm22 += Q11*h11+Q22*h22+Q33*h33+2.*(Q12*h12+Q13*h13+Q23*h23);
    
    ///////////////////////////////////////12 part///////////////////////////////////////
    Q11 = sqrt(M_PI/5)*(v1-I*v2)*(2*v3*(x1-I*x2)+(v1-I*v2)*x3)/21./r;
    Q12 = 0;
    Q13 = -sqrt(M_PI/5)*(v3*v3*(x1-I*x2)+v1*v1*(2*x1+I*x2)+v2*v2*(4*x1-7.*I*x2)-2.*I*v2*v3*x3+2*v1*(I*v2*x1+4*v2*x2+v3*x3))/21./r;
    Q22 = sqrt(M_PI/5)*(v1-I*v2)*(2*v3*(x1-I*x2)+(v1-I*v2)*x3)/21./r;
    Q23 = sqrt(M_PI/5)*(v3*v3*(I*x1+x2)+v2*v2*(-I*x1+2*x2)+v1*v1*(7.*I*x1+4*x2)+2*v2*v3*x3+v1*(8*v2*x1-2.*I*v2*x2+2.*I*v3*x3))/21./r;
    Q33 = -2*sqrt(M_PI/5)*(v1-I*v2)*(2*v3*(x1-I*x2)+(v1-I*v2)*x3)/21./r;
    
    h11 = dm*(1-2*eta)*1.5/r*(-3*rdot*n1*n1);
    h12 = dm*(1-2*eta)*1.5/r*(-3*rdot*n1*n2);
    h13 = dm*(1-2*eta)*1.5/r*(-3*rdot*n1*n3);
    h22 = dm*(1-2*eta)*1.5/r*(-3*rdot*n2*n2);
    h23 = dm*(1-2*eta)*1.5/r*(-3*rdot*n2*n3);
    h33 = dm*(1-2*eta)*1.5/r*(-3*rdot*n3*n3);
    
    hm22 += Q11*h11+Q22*h22+Q33*h33+2.*(Q12*h12+Q13*h13+Q23*h23);
    
    ///////////////////////////////////////03 part///////////////////////////////////////
    Q11 = sqrt(M_PI/5)*pow(v1-I*v2,2)*v3/7.;
    Q12 = 0;
    Q13 = -sqrt(M_PI/5)*(v1-I*v2)*(2*v1*v1+5.*I*v1*v2+7*v2*v2+3*v3*v3)/21.;
    Q22 = sqrt(M_PI/5)*pow(v1-I*v2,2)*v3/7.;
    Q23 = sqrt(M_PI/5)*(I*v1+v2)*(7*v1*v1-5.*I*v1*v2+2*v2*v2+3*v3*v3)/21.;
    Q33 = -2*sqrt(M_PI/5)*pow(v1-I*v2,2)*v3/7.;
    
    h11 = 0;
    h12 = 0;
    h13 = 0;
    h22 = 0;
    h23 = 0;
    h33 = 0;
    
    hm22 += Q11*h11+Q22*h22+Q33*h33+2.*(Q12*h12+Q13*h13+Q23*h23);
    //-----------------------------------complete-----------------------------------------
    
    hm22 = hm22*2.*eta;
    *hr = creal(hm22);
    *hi = cimag(hm22);
    //exit(0);
}





/**主要的计算程序函数
 * This function generates spin-aligned SEOBNRv1 waveforms h+ and hx.
 * Currently, only the h22 harmonic is available.
 * STEP 0) Prepare parameters, including pre-computed coefficients
 *         for EOB Hamiltonian, flux and waveform
 * STEP 1) Solve for initial conditions
 * STEP 2) Evolve EOB trajectory until reaching the peak of orbital frequency
 * STEP 3) Step back in time by tStepBack and volve EOB trajectory again
 *         using high sampling rate, stop at 0.3M out of the "EOB horizon".
 * STEP 4) Locate the peak of orbital frequency for NQC and QNM calculations
 * STEP 5) Calculate NQC correction using hi-sampling data
 * STEP 6) Calculate QNM excitation coefficients using hi-sampling data
 * STEP 7) Generate full inspiral waveform using desired sampling frequency
 * STEP 8) Generate full IMR modes -- attaching ringdown to inspiral
 * STEP 9) Generate full IMR hp and hx waveforms
 */
int XLALSimSEOBNRE(
                   REAL8TimeSeries **hplus,     /**<< OUTPUT, +-polarization waveform */
                   REAL8TimeSeries **hcross,    /**<< OUTPUT, x-polarization waveform */
                   const REAL8     phiC,        /**<< coalescence orbital phase (rad) */
                   REAL8           deltaT,      /**<< sampling time step */
                   const REAL8     m1SI,        /**<< mass-1 in SI unit */
                   const REAL8     m2SI,        /**<< mass-2 in SI unit */
                   const REAL8     fMin,        /**<< starting frequency (Hz) */
                   const REAL8     e0,          /**<< eccentricity at starting GW frequency (Hz) */
                   const REAL8     r,           /**<< distance in SI unit */
                   const REAL8     inc,         /**<< inclination angle */
                   const REAL8     spin1z,      /**<< z-component of spin-1, dimensionless */
                   const REAL8     spin2z,      /**<< z-component of spin-2, dimensionless */
                   const char      *jobtag
)
{
    /* If either spin > 0.6, model not available, exit */
    if ( spin1z > 0.6 || spin2z > 0.6 )
    {
        fprintf(stderr, "XLAL Error -: Component spin larger than 0.6!\nSEOBNRv1 is only available for spins in the range -1 < a/M < 0.6.\n");
    }
    
    int i;
    
    REAL8Vector *values = NULL;
    REAL8 fini = fMin;
    /* elip correction for initial frequency */
    fini /= pow(1-e0*e0, 1.5);
    
    /* EOB spin vectors used in the Hamiltonian */
    REAL8Vector *sigmaStar = NULL;
    REAL8Vector *sigmaKerr = NULL;
    REAL8       a;
    REAL8       chiS, chiA;
    
    /* Wrapper spin vectors used to calculate sigmas */
    REAL8Vector s1Vec;
    REAL8Vector s2Vec;
    REAL8       spin1[3] = {0, 0, spin1z};
    REAL8       spin2[3] = {0, 0, spin2z};
    REAL8       s1Data[3], s2Data[3];
    
    /* Parameters of the system */
    REAL8 m1, m2, mTotal, eta, mTScaled;
    REAL8 amp0;
    REAL8 sSub = 0.0;
    LIGOTimeGPS tc = { 0, 0 };
    
    /* Dynamics of the system */
    REAL8Vector rVec, phiVec, prVec, pPhiVec;
    REAL8       omega, v, ham;
    
    /* Cartesian vectors needed to calculate Hamiltonian */
    REAL8Vector cartPosVec, cartMomVec;
    REAL8       cartPosData[3], cartMomData[3];
    
    /* Signal mode */
    COMPLEX16   hLM;
    REAL8Vector *sigReVec = NULL, *sigImVec = NULL;
    
    /* Non-quasicircular correction */
    EOBNonQCCoeffs nqcCoeffs;
    COMPLEX16      hNQC;
    REAL8Vector    *ampNQC = NULL, *phaseNQC = NULL;
    
    /* Ringdown freq used to check the sample rate */
    COMPLEX16Vector modefreqVec;
    COMPLEX16      modeFreq;
    
    /* Spin-weighted spherical harmonics */
    COMPLEX16  MultSphHarmP;
    COMPLEX16  MultSphHarmM;
    
    /* We will have to switch to a high sample rate for ringdown attachment */
    REAL8 deltaTHigh;
    UINT4 resampFac;
    UINT4 resampPwr;
    REAL8 resampEstimate;
    
    /* How far will we have to step back to attach the ringdown? */
    REAL8 tStepBack;
    int  nStepBack;
    
    /* Dynamics and details of the high sample rate part used to attach the ringdown */
    UINT4 hiSRndx;
    REAL8Vector timeHi, rHi, phiHi, prHi, pPhiHi;
    REAL8Vector *sigReHi = NULL, *sigImHi = NULL;
    REAL8Vector *omegaHi = NULL;
    
    /* Indices of peak frequency and final point */
    /* Needed to attach ringdown at the appropriate point */
    UINT4 peakIdx = 0, finalIdx = 0;
    
    /* (2,2) and (2,-2) spherical harmonics needed in (h+,hx) */
    REAL8 y_1, y_2, z1, z2;
    
    /* Variables for the integrator */
    ark4GSLIntegrator       *integrator = NULL;
    REAL8Array              *dynamics   = NULL;
    REAL8Array              *dynamicsHi = NULL;
    int                    retLen;
    
    
    /* Accuracies of adaptive Runge-Kutta integrator */
    const REAL8 EPS_ABS = 1.0e-10;
    const REAL8 EPS_REL = 1.0e-9;
    
    /**
     * STEP 0) Prepare parameters, including pre-computed coefficients
     *         for EOB Hamiltonian, flux and waveform
     */
    
    /* Parameter structures containing important parameters for the model */
    SpinEOBParams           seobParams;
    SpinEOBHCoeffs          seobCoeffs;
    EOBParams               eobParams;
    FacWaveformCoeffs       hCoeffs;
    NewtonMultipolePrefixes prefixes;
    
    /* Initialize parameters */
    m1 = m1SI / LAL_MSUN_SI;
    m2 = m2SI / LAL_MSUN_SI;
    mTotal = m1 + m2;
    mTScaled = mTotal * LAL_MTSUN_SI;
    eta    = m1 * m2 / (mTotal*mTotal);
    
    amp0 = mTotal * LAL_MRSUN_SI / r;
    
    /* TODO: Insert potentially necessary checks on the arguments */
    
    /* Calculate the time we will need to step back for ringdown */
    tStepBack = 50. * mTScaled;
    nStepBack = ceil( tStepBack / deltaT );
    
    /* Calculate the resample factor for attaching the ringdown */
    /* We want it to be a power of 2 */
    /* If deltaT > Mtot/50, reduce deltaT by the smallest power of two for which deltaT < Mtot/50 */
    resampEstimate = 50. * deltaT / mTScaled;
    resampFac = 1;
    //resampFac = 1 << (UINT4)ceil(MYlog2(resampEstimate));
    
    if ( resampEstimate > 1. )
    {
        resampPwr = (UINT4)ceil( MYlog2( resampEstimate ) );
        while ( resampPwr-- )
        {
            resampFac *= 2u;
        }
    }
    
    
    /* Allocate the values vector to contain the initial conditions */
    /* Since we have aligned spins, we can use the 4-d vector as in the non-spin case */
    if ( !(values = XLALCreateREAL8Vector( 4 )) )
    {
        return 0;
    }
    memset ( values->data, 0, values->length * sizeof( REAL8 ));
    
    /* Set up structures and calculate necessary PN parameters */
    /* Unlike the general case, we only need to calculate these once */
    memset( &seobParams, 0, sizeof(seobParams) );
    memset( &seobCoeffs, 0, sizeof(seobCoeffs) );
    memset( &eobParams, 0, sizeof(eobParams) );
    memset( &hCoeffs, 0, sizeof( hCoeffs ) );
    memset( &prefixes, 0, sizeof( prefixes ) );
    
    /* Before calculating everything else, check sample freq is high enough */
    modefreqVec.length = 1;
    modefreqVec.data   = &modeFreq;
    
    if ( XLALSimIMREOBGenerateQNMFreqV2_re( &modefreqVec, m1, m2, spin1, spin2, 2, 2, 1) == -1 )
    {
        XLALDestroyREAL8Vector( values );
        return -1;
    }
    
    
    /* If Nyquist freq < 220 QNM freq, exit */
    if ( deltaT > LAL_PI / creal(modeFreq) )
    {
        fprintf( stderr,"XLAL Error -: Ringdown frequency > Nyquist frequency = %f!\nAt present this situation is not supported.\n", LAL_PI / creal(modeFreq));
        XLALDestroyREAL8Vector( values );
        return -1;
    }
    
    if ( !(sigmaStar = XLALCreateREAL8Vector( 3 )) )
    {
        XLALDestroyREAL8Vector( values );
        return -1;
    }
    
    if ( !(sigmaKerr = XLALCreateREAL8Vector( 3 )) )
    {
        XLALDestroyREAL8Vector( sigmaStar );
        XLALDestroyREAL8Vector( values );
        return -1;
    }
    
    seobParams.alignedSpins = 1;
    seobParams.tortoise     = 1;
    seobParams.sigmaStar    = sigmaStar;
    seobParams.sigmaKerr    = sigmaKerr;
    seobParams.seobCoeffs   = &seobCoeffs;
    seobParams.eobParams    = &eobParams;
    seobParams.Spin1    = XLALCreateREAL8Vector( 3 );
    seobParams.Spin2    = XLALCreateREAL8Vector( 3 );
    eobParams.hCoeffs       = &hCoeffs;
    eobParams.prefixes      = &prefixes;
    
    eobParams.m1  = m1;
    eobParams.m2  = m2;
    eobParams.eta = eta;
    
    s1Vec.length = s2Vec.length = 3;
    s1Vec.data   = s1Data;
    s2Vec.data   = s2Data;
    
    /* copy the spins into the appropriate vectors, and scale them by the mass */
    memcpy( s1Data, spin1, sizeof( s1Data ) );
    memcpy( s2Data, spin2, sizeof( s2Data ) );
    
    /* Calculate chiS and chiA */
    
    
    chiS = 0.5 * (spin1[2] + spin2[2]);
    chiA = 0.5 * (spin1[2] - spin2[2]);
    
    for( i = 0; i < 3; i++ )
    {
        s1Data[i] *= m1*m1;
        s2Data[i] *= m2*m2;
    }
    
    cartPosVec.length = cartMomVec.length = 3;
    cartPosVec.data = cartPosData;
    cartMomVec.data = cartMomData;
    memset( cartPosData, 0, sizeof( cartPosData ) );
    memset( cartMomData, 0, sizeof( cartMomData ) );
    
    /* Populate the initial structures */
    if ( XLALSimIMRSpinEOBCalculateSigmaStar( sigmaStar, m1, m2, &s1Vec, &s2Vec ) == -1 )
    {
        XLALDestroyREAL8Vector( sigmaKerr );
        XLALDestroyREAL8Vector( sigmaStar );
        XLALDestroyREAL8Vector( values );
        return -1;
    }
    
    if ( XLALSimIMRSpinEOBCalculateSigmaKerr( sigmaKerr, m1, m2, &s1Vec, &s2Vec ) == -1 )
    {
        XLALDestroyREAL8Vector( sigmaKerr );
        XLALDestroyREAL8Vector( sigmaStar );
        XLALDestroyREAL8Vector( values );
        return -1;
    }
    
    /* Calculate the value of a */
    /* XXX I am assuming that, since spins are aligned, it is okay to just use the z component XXX */
    /* TODO: Check this is actually the way it works in LAL */
    a = 0.0;
    /*for ( i = 0; i < 3; i++ )
     {
     a += sigmaKerr->data[i]*sigmaKerr->data[i];
     }
     a = sqrt( a );*/
    seobParams.a = a = sigmaKerr->data[2];
    /* a set to zero in SEOBNRv1, didn't know yet a good mapping from two physical spins to the test-particle limit Kerr spin */
    if ( XLALSimIMREOBCalcSpinFacWaveformCoefficients( &hCoeffs, m1, m2, eta, /*a*/0.0, chiS, chiA ) == -1 )
    {
        XLALDestroyREAL8Vector( sigmaKerr );
        XLALDestroyREAL8Vector( sigmaStar );
        XLALDestroyREAL8Vector( values );
        return -1;
    }
    
    if ( XLALSimIMREOBComputeNewtonMultipolePrefixes( &prefixes, eobParams.m1, eobParams.m2 )
        == -1 )
    {
        XLALDestroyREAL8Vector( sigmaKerr );
        XLALDestroyREAL8Vector( sigmaStar );
        XLALDestroyREAL8Vector( values );
        return -1;
    }
    
    /**
     * STEP 1) Solve for initial conditions
     */
    
    /* Set the initial conditions. For now we use the generic case */
    /* Can be simplified if spin-aligned initial conditions solver available. The cost of generic code is negligible though. */
    REAL8Vector *tmpValues = XLALCreateREAL8Vector( 14 );
    if ( !tmpValues )
    {
        XLALDestroyREAL8Vector( sigmaKerr );
        XLALDestroyREAL8Vector( sigmaStar );
        XLALDestroyREAL8Vector( values );
        return -1;
    }
    
    memset( tmpValues->data, 0, tmpValues->length * sizeof( REAL8 ) );
    
    /* We set inc zero here to make it easier to go from Cartesian to spherical coords */
    /* No problem setting inc to zero in solving spin-aligned initial conditions. */
    /* inc is not zero in generating the final h+ and hx */
    if ( XLALSimIMRSpinEOBInitialConditions( tmpValues, m1, m2, fini, e0, 0, s1Data, s2Data, &seobParams ) == XLAL_FAILURE )
    {
        XLALDestroyREAL8Vector( tmpValues );
        XLALDestroyREAL8Vector( sigmaKerr );
        XLALDestroyREAL8Vector( sigmaStar );
        XLALDestroyREAL8Vector( values );
        return -1;
    }
    
    /*fprintf( stderr, "ICs = %.16e %.16e %.16e %.16e %.16e %.16e %.16e %.16e %.16e %.16e %.16e %.16e\n", tmpValues->data[0], tmpValues->data[1], tmpValues->data[2],
     tmpValues->data[3], tmpValues->data[4], tmpValues->data[5], tmpValues->data[6], tmpValues->data[7], tmpValues->data[8],
     tmpValues->data[9], tmpValues->data[10], tmpValues->data[11] );*/
    
    /* Taken from Andrea's code */
    /*  memset( tmpValues->data, 0, tmpValues->length*sizeof(tmpValues->data[0]));*/
    /*
     tmpValues->data[0] = 12.983599142327673;
     tmpValues->data[3] = -0.002383249720459786;
     tmpValues->data[4] = 4.3204065947459735/tmpValues->data[0];
     */
    /* Now convert to Spherical */
    /* The initial conditions code returns Cartesian components of four vectors x, p, S1 and S2,
     * in the special case that the binary starts on the x-axis and the two spins are aligned
     * with the orbital angular momentum along the z-axis.
     * Therefore, in spherical coordinates the initial conditions are
     * r = x; phi = 0.; pr = px; pphi = r * py.
     */
    values->data[0] = tmpValues->data[0];
    values->data[1] = 0.;
    values->data[2] = tmpValues->data[3];
    values->data[3] = tmpValues->data[0] * tmpValues->data[4];
    
    //fprintf( stderr, "Spherical initial conditions: %e %e %e %e\n", values->data[0], values->data[1], values->data[2], values->data[3] );
    
    /* Now compute the spinning H coefficients and store them in seobCoeffs */
    if ( XLALSimIMRCalculateSpinEOBHCoeffs( &seobCoeffs, eta, a ) == -1 )
    {
        XLALDestroyREAL8Vector( tmpValues );
        XLALDestroyREAL8Vector( sigmaKerr );
        XLALDestroyREAL8Vector( sigmaStar );
        XLALDestroyREAL8Vector( values );
        return -1;
    }
    
    /**
     * STEP 2) Evolve EOB trajectory until reaching the peak of orbital frequency
     */
    
    /* Now we have the initial conditions, we can initialize the adaptive integrator */
    /* XLALAdaptiveRungeKutta4Init is in Panyicomm_elip.cpp */
    /* XLALSpinAlignedHcapDerivative is in Panyicomm_elip.cpp */
    /* XLALEOBSpinAlignedStopCondition is in Panyicomm_elip.cpp */
    if (!(integrator = XLALAdaptiveRungeKutta4Init(4, XLALSpinAlignedHcapDerivative, XLALEOBSpinAlignedStopCondition, EPS_ABS, EPS_REL)))
    {
        XLALDestroyREAL8Vector( values );
    }
    
    integrator->stopontestonly = 1;
    integrator->retries = 1;
    
    retLen = XLALAdaptiveRungeKutta4( integrator, &seobParams, values->data, 0., 20./mTScaled, deltaT/mTScaled, &dynamics );
#if debugOutput
    fprintf(stderr, "DEBUG:retLen=%d\n", retLen);
#endif
    if ( retLen == XLAL_FAILURE )
    {
        fprintf(stderr,"Error!");
    }
    
    /* Set up pointers to the dynamics */
    rVec.length = phiVec.length = prVec.length = pPhiVec.length = retLen;
    rVec.data    = dynamics->data+retLen;
    phiVec.data  = dynamics->data+2*retLen;
    prVec.data   = dynamics->data+3*retLen;
    pPhiVec.data = dynamics->data+4*retLen;
    
    //printf( "We think we hit the peak at time %e\n", dynamics->data[retLen-1] );
    //fprintf(stderr, "r0 = %.16e pphi0 = %.16e\n", rVec.data[i], pPhiVec.data[i]);
    /* TODO : Insert high sampling rate / ringdown here */
#if 0
    FILE *out = fopen( "dynamics_v1_debug.txt", "w" );
    for ( i = 0; i < retLen - nStepBack; i++ )
    {
        fprintf( out, "%.16e %.16e %.16e %.16e %.16e\n", dynamics->data[i], rVec.data[i], phiVec.data[i], prVec.data[i], pPhiVec.data[i] );
    }
    fclose(out);
#endif
    /**
     * STEP 3) Step back in time by tStepBack and volve EOB trajectory again
     *         using high sampling rate, stop at 0.3M out of the "EOB horizon".
     */
    
    /* Set up the high sample rate integration */
    hiSRndx = retLen - nStepBack;
    deltaTHigh = deltaT / (REAL8)resampFac;
#if debugOutput
    fprintf(stderr, "DEBUG:hiSRndx=%d\n", hiSRndx);
    fprintf(stderr, "DEBUG:deltaTHigh=%f\n", deltaTHigh);
#endif
    /*fprintf( stderr, "Stepping back %d points - we expect %d points at high SR\n", nStepBack, nStepBack*resampFac );
     fprintf( stderr, "Commencing high SR integration... from %.16e %.16e %.16e %.16e %.16e\n",
     (dynamics->data)[hiSRndx],rVec.data[hiSRndx], phiVec.data[hiSRndx], prVec.data[hiSRndx], pPhiVec.data[hiSRndx] );*/
    
    values->data[0] = rVec.data[hiSRndx];
    values->data[1] = phiVec.data[hiSRndx];
    values->data[2] = prVec.data[hiSRndx];
    values->data[3] = pPhiVec.data[hiSRndx];
    /* For HiSR evolution, we stop at a radius 0.3M from the deformed Kerr singularity,
     * or when any derivative of Hamiltonian becomes nan */
    integrator->stop = XLALSpinAlignedHiSRStopCondition;
    
    retLen = XLALAdaptiveRungeKutta4( integrator, &seobParams, values->data, 0., 20./mTScaled, deltaTHigh/mTScaled, &dynamicsHi );
#if debugOutput
    fprintf(stderr, "DEBUG:retLen=%d\n", retLen);
#endif
    if ( retLen == XLAL_FAILURE )
    {
        fprintf(stderr,"Error!");
    }
    
    //fprintf( stderr, "We got %d points at high SR\n", retLen );
    
    /* Set up pointers to the dynamics */
    rHi.length = phiHi.length = prHi.length = pPhiHi.length = timeHi.length = retLen;
    timeHi.data = dynamicsHi->data;
    rHi.data    = dynamicsHi->data+retLen;
    phiHi.data  = dynamicsHi->data+2*retLen;
    prHi.data   = dynamicsHi->data+3*retLen;
    pPhiHi.data = dynamicsHi->data+4*retLen;

#if debugOutput
    FILE *out1 = fopen( "dynamicsHi_v1.txt", "w" );
    for ( i = 0; i < retLen; i++ )
    {
        fprintf( out1 , "%.16e %.16e %.16e %.16e %.16e\n", dynamics->data[hiSRndx]+timeHi.data[i], rHi.data[i], phiHi.data[i], prHi.data[i], pPhiHi.data[i] );
    }
    fclose(out1);
#endif
    //printf("retLen = %d\n", retLen);
    /* Allocate the high sample rate vectors */
    sigReHi  = XLALCreateREAL8Vector( retLen + (UINT4)ceil( 20 / ( cimag(modeFreq) * deltaTHigh )) );
    sigImHi  = XLALCreateREAL8Vector( retLen + (UINT4)ceil( 20 / ( cimag(modeFreq) * deltaTHigh )) );
    omegaHi  = XLALCreateREAL8Vector( retLen + (UINT4)ceil( 20 / ( cimag(modeFreq) * deltaTHigh )) );
    ampNQC   = XLALCreateREAL8Vector( retLen );
    phaseNQC = XLALCreateREAL8Vector( retLen );
    
    if ( !sigReHi || !sigImHi || !omegaHi || !ampNQC || !phaseNQC )
    {
        //XLAL_ERROR( XLAL_ENOMEM );
        fprintf(stderr,"Error!");
    }
    
    memset( sigReHi->data, 0, sigReHi->length * sizeof( sigReHi->data[0] ));
    memset( sigImHi->data, 0, sigImHi->length * sizeof( sigImHi->data[0] ));
    
    /* Populate the high SR waveform */
    REAL8 omegaOld = 0.0;
    INT4  phaseCounter = 0;

    for ( i = 0; i < retLen; i++ )
    {
        values->data[0] = rHi.data[i];
        values->data[1] = phiHi.data[i];
        values->data[2] = prHi.data[i];
        values->data[3] = pPhiHi.data[i];
        
        omegaHi->data[i] = omega = XLALSimIMRSpinAlignedEOBCalcOmega( values->data, &seobParams );

        v = MYcbrt( omega );

        /* Calculate the value of the Hamiltonian */
        cartPosVec.data[0] = values->data[0];
        cartMomVec.data[0] = values->data[2];
        cartMomVec.data[1] = values->data[3] / values->data[0];
        
        ham = XLALSimIMRSpinEOBHamiltonian( eta, &cartPosVec, &cartMomVec, sigmaKerr, sigmaStar, seobParams.tortoise, &seobCoeffs );
        
        if ( XLALSimIMRSpinEOBGetSpinFactorizedWaveform( &hLM, values, v, ham, 2, 2, &seobParams )
            == XLAL_FAILURE )
        {
            /* TODO: Clean-up */
            fprintf(stderr,"Error!");
        }
        
        ampNQC->data[i]  = cabs( hLM );
        sigReHi->data[i] = (REAL4)(amp0 * creal(hLM));
        sigImHi->data[i] = (REAL4)(amp0 * cimag(hLM));
        phaseNQC->data[i]= carg( hLM ) + phaseCounter * LAL_TWOPI;
        
        if ( i && phaseNQC->data[i] > phaseNQC->data[i-1] )
        {
            phaseCounter--;
            phaseNQC->data[i] -= LAL_TWOPI;
        }
        
        if ( omega <= omegaOld && !peakIdx )
        {

            peakIdx = i;
        }
        omegaOld = omega;
    }
    


    //printf( "We now think the peak is at %d\n", peakIdx );
    finalIdx = retLen - 1;
#if debugOutput
    fprintf(stderr, "DEBUG:peakIdx=%d\n", peakIdx);
#endif
    /**
     * STEP 4) Locate the peak of orbital frequency for NQC and QNM calculations
     */
    
    /* Stuff to find the actual peak time */
    gsl_spline    *spline = NULL;
    gsl_interp_accel *acc = NULL;
    REAL8 omegaDeriv1; //, omegaDeriv2;
    REAL8 time1, time2;
    REAL8 timePeak, timewavePeak = 0., omegaDerivMid;
    REAL8 sigAmpSqHi = 0., oldsigAmpSqHi = 0.;
    INT4  peakCount = 0;
    
    spline = gsl_spline_alloc( gsl_interp_cspline, retLen );
    acc    = gsl_interp_accel_alloc();
    
    time1 = dynamicsHi->data[peakIdx];
    
    gsl_spline_init( spline, dynamicsHi->data, omegaHi->data, retLen );
    omegaDeriv1 = gsl_spline_eval_deriv( spline, time1, acc );
    if ( omegaDeriv1 > 0. )
    {
        time2 = dynamicsHi->data[peakIdx+1];
        //omegaDeriv2 = gsl_spline_eval_deriv( spline, time2, acc );
    }
    else
    {
        //omegaDeriv2 = omegaDeriv1;
        time2 = time1;
        time1 = dynamicsHi->data[peakIdx-1];
        peakIdx--;
        omegaDeriv1 = gsl_spline_eval_deriv( spline, time1, acc );
    }
#if debugOutput
    fprintf(stderr, "DEBUG:time1=%f, time2=%f\n", time1, time2);
#endif
    do
    {
        timePeak = ( time1 + time2 ) / 2.;
        omegaDerivMid = gsl_spline_eval_deriv( spline, timePeak, acc );
        
        if ( omegaDerivMid * omegaDeriv1 < 0.0 )
        {
            //omegaDeriv2 = omegaDerivMid;
            time2 = timePeak;
        }
        else
        {
            omegaDeriv1 = omegaDerivMid;
            time1 = timePeak;
        }
    }
    while ( time2 - time1 > 1.0e-5 );
#if debugOutput
    fprintf(stderr, "DEBUG:omegaDeriv1=%f\n", omegaDeriv1);
    fprintf(stderr, "DEBUG:timePeak=%f\n", timePeak);
#endif
    /*gsl_spline_free( spline );
     gsl_interp_accel_free( acc );
     */


    //printf( "Estimation of the peak is now at time %.16e\n", timePeak );
    
    /* Having located the peak of orbital frequency, we set time and phase of coalescence */
    XLALGPSAdd( &tc, -mTScaled * (dynamics->data[hiSRndx] + timePeak));
    gsl_spline_init( spline, dynamicsHi->data, phiHi.data, retLen );
#if 1
    sSub = gsl_spline_eval( spline, timePeak, acc ) - phiC;
#else
    sSub = -phiC;
#endif
    gsl_spline_free( spline );
    gsl_interp_accel_free( acc );
    /* Apply phiC to hi-sampling waveforms */
    REAL8 thisReHi, thisImHi;
    REAL8 csSub2 = cos(2.0 * sSub);
    REAL8 ssSub2 = sin(2.0 * sSub);
    for ( i = 0; i < retLen; i++)
    {
        thisReHi = sigReHi->data[i];
        thisImHi = sigImHi->data[i];
        sigReHi->data[i] =   thisReHi * csSub2 - thisImHi * ssSub2; //set the phase of waveform at peak orbital frequancy as 0, C*exp(i 2*sSub)
        sigImHi->data[i] =   thisReHi * ssSub2 + thisImHi * csSub2;
    }
    
    /**
     * STEP 5) Calculate NQC correction using hi-sampling data
     */
    
    /* Calculate nonspin and amplitude NQC coefficients from fits and interpolation table */
#if debugOutput
    fprintf
    (stderr,"NQC should be 0 here: a1=%.16e, a2=%.16e\na3= %.16e,a3S= %.16e\na4= %.16e, a5= %.16e\nb1= %.16e,b2= %.16e\nb3= %.16e, b4= %.16e\n",
     nqcCoeffs.a1, nqcCoeffs.a2, nqcCoeffs.a3, nqcCoeffs.a3S, nqcCoeffs.a4,
     nqcCoeffs.a5, nqcCoeffs.b1, nqcCoeffs.b2, nqcCoeffs.b3, nqcCoeffs.b4);
    fprintf (stderr,"timePeak %.16e\n", timePeak);
#endif
    
    if ( XLALSimIMRGetEOBCalibratedSpinNQC( &nqcCoeffs, 2, 2, eta, a ) == XLAL_FAILURE )
    {
        fprintf(stderr,"Error(%d)", XLAL_EFUNC );
    }
    
    /* Calculate phase NQC coefficients */
    if ( XLALSimIMRSpinEOBCalculateNQCCoefficients( ampNQC, phaseNQC, &rHi, &prHi, omegaHi,
                                                   2, 2, timePeak, deltaTHigh/mTScaled, eta, a, &nqcCoeffs ) == XLAL_FAILURE )
    {
        fprintf(stderr,"Error(%d)", XLAL_EFUNC );
    }
#if debugOutput
    fprintf
    (stderr, "Only spin NQC should be 0 here:a1= %.16e, a2= %.16e\na3= %.16e, a3S= %.16e\na4= %.16e, a5= %.16e\nb1= %.16e, b2= %.16e\nb3= %.16e, b4= %.16e\n",
     nqcCoeffs.a1, nqcCoeffs.a2, nqcCoeffs.a3, nqcCoeffs.a3S, nqcCoeffs.a4,
     nqcCoeffs.a5, nqcCoeffs.b1, nqcCoeffs.b2, nqcCoeffs.b3, nqcCoeffs.b4);
#endif
    
    /* Calculate the time of amplitude peak. Despite the name, this is in fact the shift in peak time from peak orb freq time */
    timewavePeak = XLALSimIMREOBGetNRSpinPeakDeltaT(2, 2, eta,  a);
#if debugOutput
    fprintf(stderr, "DEBUG:timewavePeak=%f\n", timewavePeak);
#endif
    /* Apply to the high sampled part */
    //out = fopen( "saWavesHi.dat", "w" );
    for ( i = 0; i < retLen; i++ )
    {
        values->data[0] = rHi.data[i];
        values->data[1] = phiHi.data[i] - sSub;
        values->data[2] = prHi.data[i];
        values->data[3] = pPhiHi.data[i];
        
        //printf("NQCs entering hNQC: %.16e, %.16e, %.16e, %.16e, %.16e, %.16e\n", nqcCoeffs.a1, nqcCoeffs.a2,nqcCoeffs.a3, nqcCoeffs.a3S, nqcCoeffs.a4, nqcCoeffs.a5 );
        if ( XLALSimIMREOBNonQCCorrection( &hNQC, values, omegaHi->data[i], &nqcCoeffs ) == XLAL_FAILURE )
        {
            fprintf(stderr,"Error(%d)", XLAL_EFUNC );
        }
        
        hLM = sigReHi->data[i];
        hLM += I * sigImHi->data[i];
        //fprintf( out, "%.16e %.16e %.16e %.16e %.16e\n", timeHi.data[i], hLM.re, hLM.im, hNQC.re, hNQC.im );
        
        hLM *= hNQC;
        //if ( l == 2 && m == 2 ) elliptic correction
        {
            double r,phi,pr,pphi,rdot,phidot;
            values->data[1] += sSub;
            r = values->data[0];
            phi = values->data[1];
            pr = values->data[2];
            pphi = values->data[3];
            double dydt[4],ttmp=0;
            XLALSpinAlignedHcapDerivative(ttmp, values->data, dydt, &seobParams);
            values->data[1] -= sSub;
            rdot = dydt[0];
            phidot = dydt[1];
            
            double x1,x2,x3;
            x1 = r*cos(phi);
            x2 = r*sin(phi);
            x3 = 0;
            double v1,v2,v3;
            v1 = rdot*cos(phi)-r*phidot*sin(phi);
            v2 = rdot*sin(phi)+r*phidot*cos(phi);
            v3 = 0;
            
            double hp,hc;
            PNwaveformPRD544813rdotc_22mode(&hp,&hc,x1,x2,x3,v1,v2,v3,eta);
            
            hLM = hLM*exp(-I*2.0*sSub);
#if 1
            hLM += amp0*(hp+I*hc);
#else
            hLM += amp0*(hp+I*hc)/cabs(hNQC);  // relax by hNQC for late inspiral
#endif
            hLM = hLM*exp(I*2.0*sSub);
        }
        sigReHi->data[i] = (REAL4) creal(hLM);
        sigImHi->data[i] = (REAL4) cimag(hLM);
        sigAmpSqHi = creal(hLM)*creal(hLM)+cimag(hLM)*cimag(hLM);
        if (sigAmpSqHi < oldsigAmpSqHi && peakCount == 0 && (i-1)*deltaTHigh/mTScaled < timePeak - timewavePeak)
        {
            timewavePeak = (i-1)*deltaTHigh/mTScaled;
            peakCount += 1;
        }
        oldsigAmpSqHi = sigAmpSqHi;
    }
#if 0
    fclose(out);
#endif
#if debugOutput
    fprintf(stderr, "DEBUG:timewavePeak=%f, peakCount=%d\n", timewavePeak, peakCount);
#endif
    if (timewavePeak < 1.0e-16 || peakCount == 0 )
    {
#if debugOutput
            fprintf(stderr,"YP::warning: could not locate mode peak, use calibrated time shift of amplitude peak instead.\n");
#endif
        /* NOTE: instead of looking for the actual peak, use the calibrated value,    */
        /*       ignoring the error in using interpolated NQC instead of iterated NQC */
        timewavePeak = timePeak - timewavePeak;
    }
    
    /**
     * STEP 6) Calculate QNM excitation coefficients using hi-sampling data
     */
    
    /*out = fopen( "saInspWaveHi.dat", "w" );
     for ( i = 0; i < retLen; i++ )
     {
     fprintf( out, "%.16e %.16e %.16e\n", timeHi.data[i], sigReHi->data[i], sigImHi->data[i] );
     }
     fclose( out );*/
    
    /* Attach the ringdown at the time of amplitude peak */
    REAL8 combSize = 7.5; /* Eq. 34 */
    REAL8 timeshiftPeak;
    timeshiftPeak = timePeak - timewavePeak;
    
    //printf("YP::timePeak and timewavePeak: %.16e and %.16e\n",timePeak,timewavePeak);
    
    REAL8Vector *rdMatchPoint = XLALCreateREAL8Vector( 3 );
    if ( !rdMatchPoint )
    {
        fprintf(stderr,"Error(%d)", XLAL_ENOMEM );
    }
    
    if ( combSize > timePeak - timeshiftPeak )
    {
        fprintf( stderr,"The comb size looks to be too big!!!\n" );
    }
    
    rdMatchPoint->data[0] = combSize < timePeak - timeshiftPeak ? timePeak - timeshiftPeak - combSize : 0;
    rdMatchPoint->data[1] = timePeak - timeshiftPeak;
    rdMatchPoint->data[2] = dynamicsHi->data[finalIdx];
#if 0
    fprintf( out, "#match to inspiral at t = %.16e (%.16e)\n", dynamics->data[hiSRndx]+rdMatchPoint->data[1],rdMatchPoint->data[1]);
    fclose( out );
#endif
    //printf("Get combine range: [%.5f, %.5f], now let's attach ringdown...\n", rdMatchPoint->data[0], rdMatchPoint->data[1]);
    //XLALSimIMREOBHybridAttachRingdown locates in the current file
    if ( XLALSimIMREOBHybridAttachRingdown( sigReHi, sigImHi, 2, 2,
                                           deltaTHigh, m1, m2, spin1[0], spin1[1], spin1[2], spin2[0], spin2[1], spin2[2],
                                           &timeHi, rdMatchPoint)
        == XLAL_FAILURE )
    {
        
        fprintf(stderr,"Error(%d)", XLAL_EFUNC );
    }
    
    /**
     * STEP 7) Generate full inspiral waveform using desired sampling frequency
     */
    
    /* Now create vectors at the correct sample rate, and compile the complete waveform */
    sigReVec = XLALCreateREAL8Vector( rVec.length + (UINT4)ceil( sigReHi->length / (double)resampFac ) );
    sigImVec = XLALCreateREAL8Vector( sigReVec->length );
    
    memset( sigReVec->data, 0, sigReVec->length * sizeof( REAL8 ) );
    memset( sigImVec->data, 0, sigImVec->length * sizeof( REAL8 ) );
    
    /* Generate full inspiral waveform using desired sampling frequency */
    /* TODO - Check vectors were allocated */

    REAL8Vector *omegaPrint = NULL, *rVecPrint = NULL, *phiVecPrint = NULL, *prVecPrint = NULL, *pPhiVecPrint = NULL;
    REAL8Vector *timePrint = NULL;
    omegaPrint  = XLALCreateREAL8Vector( (INT4)rVec.length );
    rVecPrint = XLALCreateREAL8Vector( (INT4)rVec.length );
    phiVecPrint = XLALCreateREAL8Vector( (INT4)rVec.length );
    prVecPrint = XLALCreateREAL8Vector( (INT4)rVec.length );
    pPhiVecPrint = XLALCreateREAL8Vector( (INT4)rVec.length );
    timePrint = XLALCreateREAL8Vector( (INT4)rVec.length );
    double t_ini;
    t_ini = XLALGPSGetREAL8(&tc);

    for ( i = 0; i < (INT4)rVec.length; i++ )
    {
        rVecPrint->data[i] = values->data[0] = rVec.data[i];
        phiVecPrint->data[i] = values->data[1] = phiVec.data[i] - sSub;
        prVecPrint->data[i] = values->data[2] = prVec.data[i];
        pPhiVecPrint->data[i] = values->data[3] = pPhiVec.data[i];
        
        omegaPrint->data[i] = omega = XLALSimIMRSpinAlignedEOBCalcOmega( values->data, &seobParams );
        timePrint->data[i] = i * deltaT;
        v = MYcbrt( omega );
        
        /* Calculate the value of the Hamiltonian */
        cartPosVec.data[0] = values->data[0];
        cartMomVec.data[0] = values->data[2];
        cartMomVec.data[1] = values->data[3] / values->data[0];
        
        ham = XLALSimIMRSpinEOBHamiltonian( eta, &cartPosVec, &cartMomVec, sigmaKerr, sigmaStar, seobParams.tortoise, &seobCoeffs );
        
        //    printf("%f,%f,%f,%f\n",values->data[0],values->data[1],values->data[2],values->data[3]); exit(0);
        
        if ( XLALSimIMRSpinEOBGetSpinFactorizedWaveform( &hLM, values, v, ham, 2, 2, &seobParams )
            == XLAL_FAILURE )
        {
            /* TODO: Clean-up */
            fprintf(stderr,"Error(%d)", XLAL_EFUNC );
        }
        
        if ( XLALSimIMREOBNonQCCorrection( &hNQC, values, omega, &nqcCoeffs ) == XLAL_FAILURE )
        {
            fprintf(stderr,"Error(%d)", XLAL_EFUNC );
        }
        
        hLM *= hNQC;
        
        sigReVec->data[i] = amp0 * creal(hLM);
        sigImVec->data[i] = amp0 * cimag(hLM);
    }

     // Print out dynamics to file XXX_v1_jobtag.txt....
    if (strcmp(jobtag, "None") != 0)
    {
        char dynamicout[256];
        sprintf(dynamicout, "%s%s%s", "dynamic_v1_", jobtag, ".txt");
        FILE *out1 = fopen( dynamicout, "w" );
        for ( i = 0; i < (INT4)rVec.length; i++ )
        {
            // time, omega, r, pr, phi, pphi
            fprintf( out1 , "%.16e %.16e %.16e %.16e %.16e %.16e\n",timePrint->data[i] + t_ini , omegaPrint->data[i] ,rVecPrint->data[i], prVecPrint->data[i], phiVecPrint->data[i], pPhiVecPrint->data[i]);
        }
        fclose(out1);
    }
    XLALDestroyREAL8Vector(omegaPrint);
    XLALDestroyREAL8Vector(timePrint);
    XLALDestroyREAL8Vector(rVecPrint);
    XLALDestroyREAL8Vector(prVecPrint);
    XLALDestroyREAL8Vector(phiVecPrint);
    XLALDestroyREAL8Vector(pPhiVecPrint);
    //mTScaled*(timeHi.data[i]-(dynamics->data[hiSRndx] + timePeak))

    /**
     * STEP 8) Generate full IMR modes -- attaching ringdown to inspiral
     */
    
    /* Attach the ringdown part to the inspiral */
    for ( i = 0; i < (INT4)(sigReHi->length / resampFac); i++ )
    {
        sigReVec->data[i+hiSRndx] = sigReHi->data[i*resampFac];  // right hand side include high resolution inspiral and ringdown
        sigImVec->data[i+hiSRndx] = sigImHi->data[i*resampFac];
    }
    
    /**
     * STEP 9) Generate full IMR hp and hx waveforms
     */
    
    /* For now, let us just try to create a waveform */
    REAL8TimeSeries *hPlusTS  = XLALCreateREAL8TimeSeries( "H_PLUS", &tc, 0.0, deltaT, &lalStrainUnit, sigReVec->length );
    REAL8TimeSeries *hCrossTS = XLALCreateREAL8TimeSeries( "H_CROSS", &tc, 0.0, deltaT, &lalStrainUnit, sigImVec->length );
    
    /* TODO change to using XLALSimAddMode function to combine modes */
    /* For now, calculate -2Y22 * h22 + -2Y2-2 * h2-2 directly (all terms complex) */
    /* Compute spin-weighted spherical harmonics and generate waveform */
    REAL8 coa_phase = 0.0;
    
    MultSphHarmP = XLALSpinWeightedSphericalHarmonic( inc, coa_phase, -2, 2, 2 ); // -2Y22(inc, coa_phase)
    MultSphHarmM = XLALSpinWeightedSphericalHarmonic( inc, coa_phase, -2, 2, -2 ); // -2Y2-2(inc, coa_phase)
    
    y_1 =   creal(MultSphHarmP) + creal(MultSphHarmM);
    y_2 =   cimag(MultSphHarmM) - cimag(MultSphHarmP) ;
    z1 = -  cimag(MultSphHarmM) - cimag(MultSphHarmP) ;
    z2 =    creal(MultSphHarmM) - creal(MultSphHarmP);
    
    for ( i = 0; i < (INT4)sigReVec->length; i++ )
    {
        REAL8 x1 = sigReVec->data[i];
        REAL8 x2 = sigImVec->data[i];
#if outputh22
        // output h22
        hPlusTS->data->data[i]  = x1;
        hCrossTS->data->data[i] = x2;
#else
        // output h_+-i*h_x=-2Y22 * h22 + -2Y2-2 * h2-2 while h2-2 = h22^(*)
        // h_+ = y1*x1 + y2*x2
        // h_x = z1*x1 + z2*x2
        hPlusTS->data->data[i]  = y_1*x1 + y_2*x2;
        hCrossTS->data->data[i] = z1*x1 + z2*x2;
#endif
    }
    
    /* Point the output pointers to the relevant time series and return */
    (*hplus)  = hPlusTS;
    (*hcross) = hCrossTS;
    
    /* Free memory */
    XLALDestroyREAL8Vector( tmpValues );
    XLALDestroyREAL8Vector( sigmaKerr );
    XLALDestroyREAL8Vector( sigmaStar );
    XLALDestroyREAL8Vector( values );
    XLALDestroyREAL8Vector( rdMatchPoint );
    XLALDestroyREAL8Vector( ampNQC );
    XLALDestroyREAL8Vector( phaseNQC );
    XLALDestroyREAL8Vector( sigReVec );
    XLALDestroyREAL8Vector( sigImVec );
    XLALAdaptiveRungeKutta4Free( integrator );
    XLALDestroyREAL8Array( dynamics );
    XLALDestroyREAL8Array( dynamicsHi );
    XLALDestroyREAL8Vector( sigReHi );
    XLALDestroyREAL8Vector( sigImHi );
    XLALDestroyREAL8Vector( omegaHi );
    
    return XLAL_SUCCESS;
}


/**
 * The main workhorse function for performing the ringdown attachment for EOB
 * models EOBNRv2 and SEOBNRv1. This is the function which gets called by the
 * code generating the full IMR waveform once generation of the inspiral part
 * has been completed.
 * The ringdown is attached using the hybrid comb matching detailed in
 * The method is describe in Sec. II C of Pan et al. PRD 84, 124052 (2011),
 * specifically Eqs. 30 - 32.. Further details of the
 * implementation of the found in the DCC document T1100433.
 * In SEOBNRv1, the last physical overtone is replace by a pseudoQNM. See
 * Taracchini et al. PRD 86, 024011 (2012) for details.
 * STEP 1) Get mass and spin of the final black hole and the complex ringdown frequencies
 * STEP 2) Based on least-damped-mode decay time, allocate memory for rigndown waveform
 * STEP 3) Get values and derivatives of inspiral waveforms at matching comb points
 * STEP 4) Solve QNM coefficients and generate ringdown waveforms
 * STEP 5) Stitch inspiral and ringdown waveoforms
 */
INT4 XLALSimIMREOBHybridAttachRingdown(
                                       REAL8Vector *signal1,    /**<< OUTPUT, Real of inspiral waveform to which we attach ringdown */
                                       REAL8Vector *signal2,    /**<< OUTPUT, Imag of inspiral waveform to which we attach ringdown */
                                       const INT4   l,          /**<< Current mode l */
                                       const INT4   m,          /**<< Current mode m */
                                       const REAL8  dt,         /**<< Sample time step (in seconds) */
                                       const REAL8  mass1,      /**<< First component mass (in Solar masses) */
                                       const REAL8  mass2,      /**<< Second component mass (in Solar masses) */
                                       const REAL8  spin1x,     /**<<The spin of the first object; only needed for spin waveforms */
                                       const REAL8  spin1y,     /**<<The spin of the first object; only needed for spin waveforms */
                                       const REAL8  spin1z,     /**<<The spin of the first object; only needed for spin waveforms */
                                       const REAL8  spin2x,     /**<<The spin of the second object; only needed for spin waveforms */
                                       const REAL8  spin2y,     /**<<The spin of the second object; only needed for spin waveforms */
                                       const REAL8  spin2z,     /**<<The spin of the second object; only needed for spin waveforms */
                                       REAL8Vector *timeVec,    /**<< Vector containing the time values */
                                       REAL8Vector *matchrange /**<< Time values chosen as points for performing comb matching */
)
{
    
    COMPLEX16Vector *modefreqs;
    UINT4 Nrdwave;
    UINT4 j;
    
    UINT4 nmodes;
    REAL8Vector        *rdwave1;
    REAL8Vector        *rdwave2;
    REAL8Vector        *rinspwave;
    REAL8Vector        *dinspwave;
    REAL8Vector        *ddinspwave;
    REAL8VectorSequence    *inspwaves1;
    REAL8VectorSequence    *inspwaves2;
    REAL8 eta, a, NRPeakOmega22; /* To generate pQNM frequency */
    REAL8 mTot; /* In geometric units */
    REAL8 spin1[3] = { spin1x, spin1y, spin1z };
    REAL8 spin2[3] = { spin2x, spin2y, spin2z };
    REAL8 finalMass, finalSpin;
    
    mTot  = (mass1 + mass2) * LAL_MTSUN_SI;
    eta       = mass1 * mass2 / ( (mass1 + mass2) * (mass1 + mass2) );
    
    /**
     * STEP 1) Get mass and spin of the final black hole and the complex ringdown frequencies
     */
    
    /* Create memory for the QNM frequencies */
    nmodes = 8;
    modefreqs = XLALCreateCOMPLEX16Vector( nmodes );
    if ( !modefreqs )
    {
        fprintf(stderr,"Error(%d)", XLAL_ENOMEM );
    }
    
    if ( XLALSimIMREOBGenerateQNMFreqV2_re( modefreqs, mass1, mass2, spin1, spin2, l, m, nmodes ) == XLAL_FAILURE )
    {
        XLALDestroyCOMPLEX16Vector( modefreqs );
        
        fprintf(stderr,"Error(%d)", XLAL_EFUNC );
    }
    //printf("mode0 = %f + i%f\n", creal(modefreqs->data[0]), cimag(modefreqs->data[0]));
    
    /* Call XLALSimIMREOBFinalMassSpin_re() to get mass and spin of the final black hole */
    if ( XLALSimIMREOBFinalMassSpin_re(&finalMass, &finalSpin, mass1, mass2, spin1, spin2) == XLAL_FAILURE )
    {
        fprintf(stderr,"Error(%d)", XLAL_EFUNC );
    }
    
    //if ( approximant == SEOBNRv1 )
    //{
    /* Replace the last QNM with pQNM */
    /* We assume aligned/antialigned spins here */
    a  = (spin1[2] + spin2[2]) / 2. * (1.0 - 2.0 * eta) + (spin1[2] - spin2[2]) / 2. * (mass1 - mass2) / (mass1 + mass2);
    NRPeakOmega22 = GetNRSpinPeakOmega( l, m, eta, a ) / mTot;
    /*printf("a and NRomega in QNM freq: %.16e %.16e %.16e %.16e %.16e\n",spin1[2],spin2[2],
     mTot/LAL_MTSUN_SI,a,NRPeakOmega22*mTot);*/
    modefreqs->data[7] = (NRPeakOmega22/finalMass + creal(modefreqs->data[0])) / 2.;
    modefreqs->data[7] += I * 10./3. * cimag(modefreqs->data[0]);
    //}
    
    /*for (j = 0; j < nmodes; j++)
     {
     printf("QNM frequencies: %d %d %d %e %e\n",l,m,j,modefreqs->data[j].re*mTot,1./modefreqs->data[j].im/mTot);
     }*/
    
    /* Ringdown signal length: 10 times the decay time of the n=0 mode */
    Nrdwave = (INT4) (EOB_RD_EFOLDS / cimag(modefreqs->data[0]) / dt);
    
    /* Check the value of attpos, to prevent memory access problems later */
    if ( matchrange->data[0] * mTot / dt < 5 || matchrange->data[1]*mTot/dt > matchrange->data[2] *mTot/dt - 2 )
    {
        fprintf(stderr, "More inspiral points needed for ringdown matching.\n" );
        //printf("%.16e,%.16e,%.16e\n",matchrange->data[0] * mTot / dt, matchrange->data[1]*mTot/dt, matchrange->data[2] *mTot/dt - 2);
        XLALDestroyCOMPLEX16Vector( modefreqs );
        
        fprintf(stderr,"Error(%d)", XLAL_EFAILED );
    }
    
    /**
     * STEP 2) Based on least-damped-mode decay time, allocate memory for rigndown waveform
     */
    
    /* Create memory for the ring-down and full waveforms, and derivatives of inspirals */
    
    rdwave1 = XLALCreateREAL8Vector( Nrdwave );
    rdwave2 = XLALCreateREAL8Vector( Nrdwave );
    rinspwave = XLALCreateREAL8Vector( 6 );
    dinspwave = XLALCreateREAL8Vector( 6 );
    ddinspwave = XLALCreateREAL8Vector( 6 );
    inspwaves1 = XLALCreateREAL8VectorSequence( 3, 6 );
    inspwaves2 = XLALCreateREAL8VectorSequence( 3, 6 );
    
    /* Check memory was allocated */
    if ( !rdwave1 || !rdwave2 || !rinspwave || !dinspwave
        || !ddinspwave || !inspwaves1 || !inspwaves2 )
    {
        XLALDestroyCOMPLEX16Vector( modefreqs );
        if (rdwave1)    XLALDestroyREAL8Vector( rdwave1 );
        if (rdwave2)    XLALDestroyREAL8Vector( rdwave2 );
        if (rinspwave)  XLALDestroyREAL8Vector( rinspwave );
        if (dinspwave)  XLALDestroyREAL8Vector( dinspwave );
        if (ddinspwave) XLALDestroyREAL8Vector( ddinspwave );
        if (inspwaves1) XLALDestroyREAL8VectorSequence( inspwaves1 );
        if (inspwaves2) XLALDestroyREAL8VectorSequence( inspwaves2 );
        fprintf(stderr,"Error(%d)", XLAL_ENOMEM );
    }
    
    memset( rdwave1->data, 0, rdwave1->length * sizeof( REAL8 ) );
    memset( rdwave2->data, 0, rdwave2->length * sizeof( REAL8 ) );
    
    /**
     * STEP 3) Get values and derivatives of inspiral waveforms at matching comb points
     */
    
    /* Generate derivatives of the last part of inspiral waves */
    /* Get derivatives of signal1 */
    if ( XLALGenerateHybridWaveDerivatives( rinspwave, dinspwave, ddinspwave, timeVec, signal1,
                                           matchrange, dt, mass1, mass2 ) == XLAL_FAILURE )
    {
        XLALDestroyCOMPLEX16Vector( modefreqs );
        XLALDestroyREAL8Vector( rdwave1 );
        XLALDestroyREAL8Vector( rdwave2 );
        XLALDestroyREAL8Vector( rinspwave );
        XLALDestroyREAL8Vector( dinspwave );
        XLALDestroyREAL8Vector( ddinspwave );
        XLALDestroyREAL8VectorSequence( inspwaves1 );
        XLALDestroyREAL8VectorSequence( inspwaves2 );
        fprintf(stderr,"Error(%d)", XLAL_EFUNC );
    }
    for (j = 0; j < 6; j++)
    {
        inspwaves1->data[j] = rinspwave->data[j];
        inspwaves1->data[j + 6] = dinspwave->data[j];
        inspwaves1->data[j + 12] = ddinspwave->data[j];
    }
    
    /* Get derivatives of signal2 */
    if ( XLALGenerateHybridWaveDerivatives( rinspwave, dinspwave, ddinspwave, timeVec, signal2,
                                           matchrange, dt, mass1, mass2 ) == XLAL_FAILURE )
    {
        XLALDestroyCOMPLEX16Vector( modefreqs );
        XLALDestroyREAL8Vector( rdwave1 );
        XLALDestroyREAL8Vector( rdwave2 );
        XLALDestroyREAL8Vector( rinspwave );
        XLALDestroyREAL8Vector( dinspwave );
        XLALDestroyREAL8Vector( ddinspwave );
        XLALDestroyREAL8VectorSequence( inspwaves1 );
        XLALDestroyREAL8VectorSequence( inspwaves2 );
        fprintf(stderr,"Error(%d)", XLAL_EFUNC );
    }
    for (j = 0; j < 6; j++)
    {
        inspwaves2->data[j] = rinspwave->data[j];
        inspwaves2->data[j + 6] = dinspwave->data[j];
        inspwaves2->data[j + 12] = ddinspwave->data[j];
    }
    
    
    /**
     * STEP 4) Solve QNM coefficients and generate ringdown waveforms
     */
    
    /* Generate ring-down waveforms */
    //XLALSimIMREOBHybridRingdownWave locates in the current file
    if ( XLALSimIMREOBHybridRingdownWave( rdwave1, rdwave2, dt, mass1, mass2, inspwaves1, inspwaves2,
                                         modefreqs, matchrange ) == XLAL_FAILURE )
    {
        XLALDestroyCOMPLEX16Vector( modefreqs );
        XLALDestroyREAL8Vector( rdwave1 );
        XLALDestroyREAL8Vector( rdwave2 );
        XLALDestroyREAL8Vector( rinspwave );
        XLALDestroyREAL8Vector( dinspwave );
        XLALDestroyREAL8Vector( ddinspwave );
        XLALDestroyREAL8VectorSequence( inspwaves1 );
        XLALDestroyREAL8VectorSequence( inspwaves2 );
        fprintf(stderr,"Error(%d)", XLAL_EFUNC );
    }
    
    /**
     * STEP 5) Stitch inspiral and ringdown waveoforms
     */
    
    /* Generate full waveforms, by stitching inspiral and ring-down waveforms */
    UINT4 attachIdx = (UINT4)(matchrange->data[1] * mTot / dt);
    //printf("attachIdx = %d\n", attachIdx);
    //printf("len(signal1) = %d\n", signal1->length);
    //printf("Nrdwave = %d\n", Nrdwave);

    for (j = 1; j < Nrdwave; ++j)
    {
        signal1->data[j + attachIdx] = rdwave1->data[j];
        signal2->data[j + attachIdx] = rdwave2->data[j];
    }
    
    memset( signal1->data+Nrdwave+attachIdx, 0, (signal1->length - Nrdwave - attachIdx)*sizeof(REAL8) );
    memset( signal2->data+Nrdwave+attachIdx, 0, (signal2->length - Nrdwave - attachIdx)*sizeof(REAL8) );
    
    /* Free memory */
    XLALDestroyCOMPLEX16Vector( modefreqs );
    XLALDestroyREAL8Vector( rdwave1 );
    XLALDestroyREAL8Vector( rdwave2 );
    XLALDestroyREAL8Vector( rinspwave );
    XLALDestroyREAL8Vector( dinspwave );
    XLALDestroyREAL8Vector( ddinspwave );
    XLALDestroyREAL8VectorSequence( inspwaves1 );
    XLALDestroyREAL8VectorSequence( inspwaves2 );
    
    return XLAL_SUCCESS;
}

/**
 * Generates the ringdown wave associated with the given real
 * and imaginary parts of the inspiral waveform. The parameters of
 * the ringdown, such as amplitude and phase offsets, are determined
 * by solving the linear equations defined in the DCC document T1100433.
 * In the linear equations Ax=y,
 * A is a 16-by-16 matrix depending on QNM (complex) frequencies,
 * x is a 16-d vector of the 8 unknown complex QNM amplitudes,
 * y is a 16-d vector depending on inspiral-plunge waveforms and their derivatives near merger.
 */
INT4 XLALSimIMREOBHybridRingdownWave(
                                     REAL8Vector          *rdwave1,   /**<< OUTPUT, Real part of ringdown waveform */
                                     REAL8Vector          *rdwave2,   /**<< OUTPUT, Imag part of ringdown waveform */
                                     const REAL8           dt,        /**<< Sampling interval */
                                     const REAL8           mass1,     /**<< First component mass (in Solar masses) */
                                     const REAL8           mass2,     /**<< Second component mass (in Solar masses) */
                                     REAL8VectorSequence  *inspwave1, /**<< Values and derivs of real part inspiral waveform */
                                     REAL8VectorSequence  *inspwave2, /**<< Values and derivs of imag part inspiral waveform */
                                     COMPLEX16Vector      *modefreqs, /**<< Complex freqs of ringdown (scaled by total mass) */
                                     REAL8Vector          *matchrange /**<< Times which determine the comb of ringdown attachment */
)
{
#if debugOutput
    fprintf(stderr,"matchrange = %f, %f, %f\n",matchrange->data[0],matchrange->data[1],matchrange->data[2]);
#endif
    /* XLAL error handling */
    INT4 errcode = XLAL_SUCCESS;
    
    /* For checking GSL return codes */
    INT4 gslStatus;
    
    UINT4 i, j, k, nmodes = 8;
    
    /* Sampling rate from input */
    REAL8 t1, t2, t3, t4, t5, rt;
    gsl_matrix *coef;
    gsl_vector *hderivs;
    gsl_vector *x;
    gsl_permutation *p;
    REAL8Vector *modeamps;
    int s;
    REAL8 tj;
    REAL8 m;
    
    /* mass in geometric units */
    m  = (mass1 + mass2) * LAL_MTSUN_SI;
    t5 = (matchrange->data[0] - matchrange->data[1]) * m;
    rt = -t5 / 5.;
    
    t4 = t5 + rt;
    t3 = t4 + rt;
    t2 = t3 + rt;
    t1 = t2 + rt;
    
    if ( inspwave1->length != 3 || inspwave2->length != 3 ||
        modefreqs->length != nmodes )
    {
        fprintf(stderr,"Error(%d)", XLAL_EBADLEN );
    }
    
    /* Solving the linear system for QNMs amplitude coefficients using gsl routine */
    /* Initiate matrices and supporting variables */
    //XLAL_CALLGSL( coef = (gsl_matrix *) gsl_matrix_alloc(2 * nmodes, 2 * nmodes) );
    coef = (gsl_matrix *) gsl_matrix_alloc(2 * nmodes, 2 * nmodes);
    //XLAL_CALLGSL( hderivs = (gsl_vector *) gsl_vector_alloc(2 * nmodes) );
    hderivs = (gsl_vector *) gsl_vector_alloc(2 * nmodes);
    //XLAL_CALLGSL( x = (gsl_vector *) gsl_vector_alloc(2 * nmodes) );
    x = (gsl_vector *) gsl_vector_alloc(2 * nmodes);
    //XLAL_CALLGSL( p = (gsl_permutation *) gsl_permutation_alloc(2 * nmodes) );
    p = (gsl_permutation *) gsl_permutation_alloc(2 * nmodes);
    
    /* Check all matrices and variables were allocated */
    if ( !coef || !hderivs || !x || !p )
    {
        if (coef)    gsl_matrix_free(coef);
        if (hderivs) gsl_vector_free(hderivs);
        if (x)       gsl_vector_free(x);
        if (p)       gsl_permutation_free(p);
        fprintf(stderr,"Error(%d)", XLAL_ENOMEM );
    }
    
    /* Define the linear system Ax=y */
    /* Matrix A (2*n by 2*n) has block symmetry. Define half of A here as "coef" */
    /* The half of A defined here corresponds to matrices M1 and -M2 in the DCC document T1100433 */
    /* Define y here as "hderivs" */
    for (i = 0; i < nmodes; ++i)
    {
        gsl_matrix_set(coef, 0, i, 1);
        gsl_matrix_set(coef, 1, i, - cimag(modefreqs->data[i]));
        gsl_matrix_set(coef, 2, i, exp(-cimag(modefreqs->data[i])*t1) * cos(creal(modefreqs->data[i])*t1));
        gsl_matrix_set(coef, 3, i, exp(-cimag(modefreqs->data[i])*t2) * cos(creal(modefreqs->data[i])*t2));
        gsl_matrix_set(coef, 4, i, exp(-cimag(modefreqs->data[i])*t3) * cos(creal(modefreqs->data[i])*t3));
        gsl_matrix_set(coef, 5, i, exp(-cimag(modefreqs->data[i])*t4) * cos(creal(modefreqs->data[i])*t4));
        gsl_matrix_set(coef, 6, i, exp(-cimag(modefreqs->data[i])*t5) * cos(creal(modefreqs->data[i])*t5));
        gsl_matrix_set(coef, 7, i, exp(-cimag(modefreqs->data[i])*t5) *
                       (-cimag(modefreqs->data[i]) * cos(creal(modefreqs->data[i])*t5)
                        -creal(modefreqs->data[i]) * sin(creal(modefreqs->data[i])*t5)));
        gsl_matrix_set(coef, 8, i, 0);
        gsl_matrix_set(coef, 9, i, - creal(modefreqs->data[i]));
        gsl_matrix_set(coef, 10, i, -exp(-cimag(modefreqs->data[i])*t1) * sin(creal(modefreqs->data[i])*t1));
        gsl_matrix_set(coef, 11, i, -exp(-cimag(modefreqs->data[i])*t2) * sin(creal(modefreqs->data[i])*t2));
        gsl_matrix_set(coef, 12, i, -exp(-cimag(modefreqs->data[i])*t3) * sin(creal(modefreqs->data[i])*t3));
        gsl_matrix_set(coef, 13, i, -exp(-cimag(modefreqs->data[i])*t4) * sin(creal(modefreqs->data[i])*t4));
        gsl_matrix_set(coef, 14, i, -exp(-cimag(modefreqs->data[i])*t5) * sin(creal(modefreqs->data[i])*t5));
        gsl_matrix_set(coef, 15, i, exp(-cimag(modefreqs->data[i])*t5) *
                       (cimag(modefreqs->data[i]) * sin(creal(modefreqs->data[i])*t5)
                        -creal(modefreqs->data[i]) * cos(creal(modefreqs->data[i])*t5)));
    }
    for (i = 0; i < 2; ++i)
    {
        gsl_vector_set(hderivs, i, inspwave1->data[(i + 1) * inspwave1->vectorLength - 1]);
        gsl_vector_set(hderivs, i + nmodes, inspwave2->data[(i + 1) * inspwave2->vectorLength - 1]);
        gsl_vector_set(hderivs, i + 6, inspwave1->data[i * inspwave1->vectorLength]);
        gsl_vector_set(hderivs, i + 6 + nmodes, inspwave2->data[i * inspwave2->vectorLength]);
    }
    gsl_vector_set(hderivs, 2, inspwave1->data[4]);
    gsl_vector_set(hderivs, 2 + nmodes, inspwave2->data[4]);
    gsl_vector_set(hderivs, 3, inspwave1->data[3]);
    gsl_vector_set(hderivs, 3 + nmodes, inspwave2->data[3]);
    gsl_vector_set(hderivs, 4, inspwave1->data[2]);
    gsl_vector_set(hderivs, 4 + nmodes, inspwave2->data[2]);
    gsl_vector_set(hderivs, 5, inspwave1->data[1]);
    gsl_vector_set(hderivs, 5 + nmodes, inspwave2->data[1]);
    
    /* Complete the definition for the rest half of A */
    for (i = 0; i < nmodes; ++i)
    {
        for (k = 0; k < nmodes; ++k)
        {
            gsl_matrix_set(coef, i, k + nmodes, - gsl_matrix_get(coef, i + nmodes, k));
            gsl_matrix_set(coef, i + nmodes, k + nmodes, gsl_matrix_get(coef, i, k));
        }
    }
    
#if 0
    /* print ringdown-matching linear system: coefficient matrix and RHS vector */
    fprintf(stderr,"\nRingdown matching matrix:\n");
    for (i = 0; i < 16; ++i)
    {
        for (j = 0; j < 16; ++j)
        {
            fprintf(stderr,"%.12e ",gsl_matrix_get(coef,i,j));
        }
        fprintf(stderr, "\n");
    }
    fprintf(stderr,"RHS:  ");
    for (i = 0; i < 16; ++i)
    {
        fprintf(stderr,"%.12e   ",gsl_vector_get(hderivs,i));
    }
    fprintf(stderr,"\n");
#endif
    
    /* Call gsl LU decomposition to solve the linear system */
    //XLAL_CALLGSL( gslStatus = gsl_linalg_LU_decomp(coef, p, &s) );
    gslStatus = gsl_linalg_LU_decomp(coef, p, &s) ;
    if ( gslStatus == GSL_SUCCESS )
    {
        //XLAL_CALLGSL( gslStatus = gsl_linalg_LU_solve(coef, p, hderivs, x) );
        gslStatus = gsl_linalg_LU_solve(coef, p, hderivs, x);
    }
    if ( gslStatus != GSL_SUCCESS )
    {
        gsl_matrix_free(coef);
        gsl_vector_free(hderivs);
        gsl_vector_free(x);
        gsl_permutation_free(p);
        
        fprintf(stderr,"Error(%d)", XLAL_EFUNC );
    }
    
    /* Putting solution to an XLAL vector */
    modeamps = XLALCreateREAL8Vector(2 * nmodes);
    
    if ( !modeamps )
    {
        gsl_matrix_free(coef);
        gsl_vector_free(hderivs);
        gsl_vector_free(x);
        gsl_permutation_free(p);
        
        fprintf(stderr,"Error(%d)", XLAL_ENOMEM );
    }
    
    for (i = 0; i < nmodes; ++i)
    {
        modeamps->data[i] = gsl_vector_get(x, i);
        modeamps->data[i + nmodes] = gsl_vector_get(x, i + nmodes);
    }
#if debugOutput
    fprintf(stderr,"using 20Msun to do scaling.\n");

    for (i = 0; i < nmodes; ++i)
    {
        fprintf(stderr,"%d-th amps = %e+i %e, freq = %e+i %e\n",i,modeamps->data[i],
               modeamps->data[i + nmodes],creal(modefreqs->data[i])*20*4.92549095e-6,cimag(modefreqs->data[i])*20*4.92549095e-6);
    }
    
    fprintf(stderr,"using Mtotal to do scaling.\n");
    for (i = 0; i < nmodes; ++i)
    {
        fprintf(stderr,"%d-th amps = %e+i %e, freq = %e+i %e\n",i,modeamps->data[i],
               modeamps->data[i + nmodes],creal(modefreqs->data[i])*m,cimag(modefreqs->data[i])*m);
    }
#endif
    /* Free all gsl linear algebra objects */
    gsl_matrix_free(coef);
    gsl_vector_free(hderivs);
    gsl_vector_free(x);
    gsl_permutation_free(p);
    
    /* Build ring-down waveforms */
    
    REAL8 timeOffset = fmod( matchrange->data[1], dt/m) * dt;
    
    for (j = 0; j < rdwave1->length; ++j)
    {
        tj = j * dt - timeOffset;
        rdwave1->data[j] = 0;
        rdwave2->data[j] = 0;
        for (i = 0; i < nmodes; ++i)
        {
            rdwave1->data[j] += exp(- tj * cimag(modefreqs->data[i]))
            * ( modeamps->data[i] * cos(tj * creal(modefreqs->data[i]))
               +   modeamps->data[i + nmodes] * sin(tj * creal(modefreqs->data[i])) );
            rdwave2->data[j] += exp(- tj * cimag(modefreqs->data[i]))
            * (- modeamps->data[i] * sin(tj * creal(modefreqs->data[i]))
               +   modeamps->data[i + nmodes] * cos(tj * creal(modefreqs->data[i])) );
        }
    }
    
    XLALDestroyREAL8Vector(modeamps);
    return errcode;
}



 REAL8 XLALInspiralSpinFactorizedFlux_elip(
                      REAL8Vector           *values, /**< dynamical variables */
                      const REAL8   rphivalues[],   /**< dynamical varables, r, phi, pr, pphi */
                      const REAL8   drphivalues[],   /**< dot dynamical varables, r, phi, pr, pphi */
                      const REAL8           omega,   /**< orbital frequency */
                      SpinEOBParams         *ak,     /**< physical parameters */
                      const REAL8            H,      /**< real Hamiltonian */
                      const int             lMax    /**< upper limit of the summation over l */
                     )

{

  REAL8 flux = 0.0;
  REAL8 v;
  REAL8 omegaSq;
  COMPLEX16 hLM;
  int l, m;

  EOBNonQCCoeffs nqcCoeffs;


  if ( lMax < 2 )
  {
    return -1;
  }

  /* Omegs is the derivative of phi */
  omegaSq = omega*omega;

  v = MYcbrt( omega );
//  printf( "v = %.16e\n", v );
  for ( l = 2; l <= lMax; l++ )
  {
    for ( m = 1; m <= l; m++ )
    {

      if ( XLALSimIMRSpinEOBGetSpinFactorizedWaveform( &hLM, values, v, H, l, m, ak )
             == -1 )
      {
        fprintf(stderr,"Error!(%d)", XLAL_EFUNC );
      }
      /* For the 2,2 mode, we apply NQC correction to the flux */
      if ( l == 2 && m == 2 )
      {
        COMPLEX16 hNQC;
        XLALSimIMRGetEOBCalibratedSpinNQC( &nqcCoeffs, l, m, ak->eobParams->eta, ak->a );    
        XLALSimIMREOBNonQCCorrection( &hNQC, values, omega, &nqcCoeffs );
        /* Eq. 16 */
        hLM *= hNQC;

        double r,phi,prt,pphi,rdot,phidot;
        r = rphivalues[0];
        prt = rphivalues[2];
        phi = rphivalues[1];
        phidot = drphivalues[3];
        pphi = rphivalues[3];
        rdot = drphivalues[0];

        double x1,x2,x3;
        x1 = r*cos(phi);
        x2 = r*sin(phi);
        x3 = 0;
        double v1,v2,v3;
        v1 = rdot*cos(phi)-r*phidot*sin(phi);
        v2 = rdot*sin(phi)+r*phidot*cos(phi);
        v3 = 0;

        double hp,hc;
        PNwaveformPRD544813rdotc_22mode(&hp,&hc,x1,x2,x3,v1,v2,v3,ak->eobParams->eta);

        const double complex CI = I;
#if 1
        hLM += hp+CI*hc;
#else
        hLM += (hp+CI*hc)/cabs(hNQC); // relax by hNQC for late inspiral
#endif
      }
      // printf( "l = %d, m = %d, mag(hLM) = %.17e\n", l, m,  XLALCOMPLEX16Abs2( hLM ) );
      /* Eq. 13, Eq.(12) of PRD 84, 124052 (2011)*/
      flux += (REAL8)(m * m) * omegaSq * ( creal(hLM)*creal(hLM) + cimag(hLM)*cimag(hLM));
    }
  }
  return flux * LAL_1_PI / 8.0;
}
/*
double EOBPNflux(const double m1,const double m2,         // we assume m1>m2
          const double s1x,const double s1y,const double s1z,
          const double s2x,const double s2y,const double s2z,
	  const double x1,const double x2,const double x3, // relative position
	  const double v1,const double v2,const double v3, // relative velocity
	  double *EOBPNfluxwoNQC=0);
*/
double EOBPNflux(const double m1,const double m2,         // we assume m1>m2
          const double s1x,const double s1y,const double s1z,
          const double s2x,const double s2y,const double s2z,
	  const double x1,const double x2,const double x3, // relative position
	  const double v1,const double v2,const double v3, // relative velocity
	  double *EOBPNfluxwoNQC)
{
   double tpt;
   if(EOBPNfluxwoNQC == 0) EOBPNfluxwoNQC = &tpt;
   const double chi1x = s1x/m1/m1;
   const double chi1y = s1y/m1/m1;
   const double chi1z = s1z/m1/m1;
   const double chi2x = s2x/m2/m2;
   const double chi2y = s2y/m2/m2;
   const double chi2z = s2z/m2/m2;
   const double m=m1+m2;
   const double eta=m1*m2/m/m;

   const double r=sqrt(x1*x1+x2*x2+x3*x3);
   const double v=sqrt(v1*v1+v2*v2+v3*v3);
   const double n1 = x1/r,n2 = x2/r,n3 = x3/r;
   const double rd = v1*n1+v2*n2+v3*n3;
   const double dm = m1-m2;
   const double ncv1=n2*v3-n3*v2;
   const double ncv2=n3*v1-n1*v3;
   const double ncv3=n1*v2-n2*v1;

   const double chis1=(chi1x+chi2x)/2;
   const double chis2=(chi1y+chi2y)/2;
   const double chis3=(chi1z+chi2z)/2;
   const double chia1=(chi1x-chi2x)/2;
   const double chia2=(chi1y-chi2y)/2;
   const double chia3=(chi1z-chi2z)/2;
   const double chissqr = chis1*chis1+chis2*chis2+chis3*chis3;
   const double chiasqr = chia1*chia1+chia2*chia2+chia3*chia3;
   const double ndchis = chis1*n1+chis2*n2+chis3*n3;
   const double ndchia = chia1*n1+chia2*n2+chia3*n3;
   const double vdchis = chis1*v1+chis2*v2+chis3*v3;
   const double vdchia = chia1*v1+chia2*v2+chia3*v3;

   double flux;
#if 0   
   flux = 8./15*eta*eta*pow(m/r,4)*(
         (12*v*v-11*rd*rd)   // (6.13a) of PRD 54, 4813 (1996); (3.25a) of PRD 52, 821 (1995)
	+((785-852*eta)*pow(v,4)-2*(1487-1392*eta)*v*v*rd*rd  
          +3*(687-620*eta)*pow(rd,4)-160*(17-eta)*m/r*v*v
	  +8*(367-15*eta)*m/r*rd*rd+16*(1-4*eta)*pow(m/r,2))/28  // (6.13b) of PRD 54, 4813 (1996); (3.25b) of PRD 52, 821 (1995)
	+(18*(1692-5497*eta+4430*eta*eta)*pow(v,6)-54*(1719-10278*eta+6292*eta*eta)*pow(v,4)*rd*rd
	 +54*(2018-15207*eta+7572*eta*eta)*v*v*pow(rd,4)-18*(2501-20234*eta+8404*eta*eta)*pow(rd,6)
	 -12*(33510-60971*eta+14290*eta*eta)*m/r*pow(rd,4)-36*(4446-5237*eta+1393*eta*eta)*m/r*pow(v,4)
	 +108*(4987-8513*eta+2165*eta*eta)*m/r*rd*rd*v*v-3*(106319+9798*eta+5376*eta*eta)*m/r*m/r*rd*rd
	 +(281473+81828*eta+4368*eta*eta)*m/r*m/r*v*v-24*(253-1026*eta+56*eta*eta)*pow(m/r,3))/756    // (6.13d) of PRD 54, 4813 (1996)
	+m/r*((27*rd*rd-37*v*v-12*m/r)*(ncv1*(chis1+dm/m*chia1)+ncv2*(chis2+dm/m*chia2)+ncv3*(chis3+dm/m*chia3))
	      +4*eta*(12*rd*rd-3*v*v+8*m/r)*(ncv1*chis1+ncv2*chis2+ncv3*chis3))                       // (F16) of PRD 54, 4813 (1996)
	+m*m/r/r*eta*(3*(chissqr-chiasqr)*(47*v*v-55*rd*rd)
		     -3*(ndchis*ndchis+ndchia*ndchia)*(168*v*v-269*rd*rd)
		     +71*(vdchis*vdchis+vdchia*vdchia)
		     -342*rd*(vdchis*ndchis-vdchia*ndchia))               // (F17) of PRD 54, 4813 (1996)
	);
#else
   const double omega=sqrt(ncv1*ncv1+ncv2*ncv2+ncv3*ncv3)/r;
   const double x=pow(MYcbrt( omega ),2);
   const double a0 = 153.8803;
   const double a1 = -55.13;
   const double a2 = 588;
   const double a3 = -1144;
   const double EulerGamma = 0.57721566490153286060651209008240243104215933593992;
   flux = 32.0/5*eta*eta*pow(x,5)*(1
		                  -(1247+980*eta)/336*x
				  +4*M_PI*pow(x,1.5)
				  +(-4.928461199294532 + (9271*eta)/504. + (65*pow(eta,2))/18.)*pow(x,2)
				  -(8191/672. + 583*eta/24.)*M_PI*pow(x,2.5)
				  +(95.10839000836025 - (1712*EulerGamma)/105. + (16*pow(M_PI,2))/3.
			            - (134543*eta)/7776. + (41*pow(M_PI,2)*eta)/48. - (94403*pow(eta,2))/3024. 
				    - (775*pow(eta,3))/324. - (856*log(16))/105. - (856*log(x))/105.)*pow(x,3)
				  +(-16285/504. + 214745*eta/1728. + 27755*pow(eta,2)/432.)*M_PI*pow(x,3.5)
				  +(-257.36874820698483 + (856*EulerGamma)/35. - 8*pow(M_PI,2) 
				    - (59292668653*eta)/8.382528e8 + 5*a0*eta + (856*EulerGamma*eta)/315. 
				    + (31495*pow(M_PI,2)*eta)/8064. - (54732199*pow(eta,2))/93312. 
				    + (3157*pow(M_PI,2)*pow(eta,2))/144. + (18929389*pow(eta,3))/435456. 
				    + (97*pow(eta,4))/3888. + (428*log(16))/35. + (428*eta*log(16))/315.
				    + (428*log(x))/35. + (47468*eta*log(x))/315.)*pow(x,4)
				  +(-104.44401041666667 + (51438847*eta)/48384. - (205*pow(M_PI,2)*eta)/6. 
				    - (42745411*pow(eta,2))/145152. - (4199*pow(eta,3))/576.)*M_PI*pow(x,4.5)
				  +(-1466.6353720117309 + (5778*EulerGamma)/35. - 54*pow(M_PI,2) 
				    + (4820443583363*eta)/1.2573792e9 - (3715*a0*eta)/336. + 6*a1*eta
				    - (4066*EulerGamma*eta)/35. - (31869941*pow(M_PI,2)*eta)/435456. 
				    - (2006716046219*pow(eta,2))/3.3530112e9 - (55*a0*pow(eta,2))/4. 
				    + (214*EulerGamma*pow(eta,2))/105. + (406321*pow(M_PI,2)*pow(eta,2))/48384. 
				    + (2683003625*pow(eta,3))/3.359232e6 - (100819*pow(M_PI,2)*pow(eta,3))/3456. 
				    - (192478799*pow(eta,4))/5.225472e6 + (33925*pow(eta,5))/186624. 
				    + (2889*log(16))/35. - (2033*eta*log(16))/35. + (107*pow(eta,2)*log(16))/105. 
				    + (2889*log(x))/35. - (391669*eta*log(x))/315. - (122981*pow(eta,2)*log(x))/105.)*pow(x,5)
				  +(-347.97154017857144 - (235274549*eta)/241920. + 20*a0*eta + (852595*pow(M_PI,2)*eta)/16128.
				    - (187219705*pow(eta,2))/32256. + (12915*pow(M_PI,2)*pow(eta,2))/64. 
				    + (503913815*pow(eta,3))/870912. - (24065*pow(eta,4))/3456. + (1792*eta*log(x))/3.)*M_PI*pow(x,5.5)
				  +(-5876.793546111627 + (4815*EulerGamma)/7. - 225*pow(M_PI,2) + (45811843687349*eta)/1.14960384e9 
				    + (170515*a0*eta)/18144. - (743*a1*eta)/56. + 7*a2*eta + a3*eta - (737123*EulerGamma*eta)/189. 
				    - (84643435883*pow(M_PI,2)*eta)/6.7060224e8 + (8774*EulerGamma*pow(M_PI,2)*eta)/63. 
				    - (410*pow(M_PI,4)*eta)/9. - (37516325949517*pow(eta,2))/6.03542016e8 
				    + (68305*a0*pow(eta,2))/2016. - (33*a1*pow(eta,2))/2. + (6634*EulerGamma*pow(eta,2))/63. 
				    + (23084972185*pow(M_PI,2)*pow(eta,2))/5.225472e6 - (92455*pow(M_PI,4)*pow(eta,2))/1152.
				    + (6069288163291*pow(eta,3))/2.58660864e9 + (295*a0*pow(eta,3))/18. + (107*EulerGamma*pow(eta,3))/243.
				    - (114930545*pow(M_PI,2)*pow(eta,3))/1.741824e6 - (145089945295*pow(eta,4))/2.82175488e8 
				    + (141655*pow(M_PI,2)*pow(eta,4))/7776. + (6942085*pow(eta,5))/497664. + (196175*pow(eta,6))/3.359232e6 
				    + (4815*log(16))/14. - (737123*eta*log(16))/378. + (4387*pow(M_PI,2)*eta*log(16))/63. 
				    + (3317*pow(eta,2)*log(16))/63. + (107*pow(eta,3)*log(16))/486. + (4815*log(x))/14. 
				    + (13185899*eta*log(x))/59535. + 7*a3*eta*log(x) + (4387*pow(M_PI,2)*eta*log(x))/63. 
				    + (963937*pow(eta,2)*log(x))/189. + (6279367*pow(eta,3)*log(x))/2430.)*pow(x,6));

   // NQC correction
   flux += (8*pow(m,4)*pow(eta,2)*((64*pow(m,8)*pow(rd,6)*pow(eta,4)*(44 - 25*eta - 67*pow(eta,2)))/21. + (64*pow(m,8)*pow(rd,4)*pow(eta,4)*
          (-1965*m - 288*r + 65*m*eta + 162*r*eta + 1016*m*pow(eta,2) + 1224*pow(r,3)*pow(omega,2) - 666*pow(r,3)*eta*pow(omega,2) 
	   - 2619*pow(r,3)*pow(eta,2)*pow(omega,2)))/(63.*r) + (32*pow(m,8)*pow(rd,2)*pow(eta,4)*(-18742*pow(m,2) + 2916*m*r 
	   + 378*pow(r,2) + 26217*pow(m,2)*eta + 540*m*r*eta - 5880*pow(m,2)*pow(eta,2) + 109242*m*pow(r,3)*pow(omega,2) 
	   - 18954*pow(r,4)*pow(omega,2) - 271170*m*pow(r,3)*eta*pow(omega,2) + 14580*pow(r,4)*eta*pow(omega,2) 
	   + 66762*m*pow(r,3)*pow(eta,2)*pow(omega,2) - 729*pow(r,6)*pow(omega,4) 
	   + 129087*pow(r,6)*eta*pow(omega,4) - 50274*pow(r,6)*pow(eta,2)*pow(omega,4)))/(189.*pow(r,2))))/(15.*pow(r,4));
#endif

   return flux;
}
/** Returns the GPS time as a REAL8. */
REAL8 XLALGPSGetREAL8( const LIGOTimeGPS *epoch )
{
  return epoch->gpsSeconds + (epoch->gpsNanoSeconds / 1e9);
}
COMPLEX16Vector* XLALCreateCOMPLEX16Vector(UINT4 length)
{
	COMPLEX16Vector* vector;
	vector = (COMPLEX16Vector*)malloc(sizeof(*vector));
	vector->length = length;
	if ( ! length ) /* zero length: set data pointer to be NULL */
	{
		vector->data = NULL;
	}
	else /* non-zero length: allocate memory for data */
	{
		vector->data = (COMPLEX16 *)malloc(length * sizeof( *vector->data));
	}
	
	return vector;
}

void XLALDestroyCOMPLEX16Vector(COMPLEX16Vector* v)
{
	if(NULL == v)
	{
		return;
	}
	if(v->data)
	{
		v->length = 0;
		free(v->data);
	}
	v->data = NULL;
	free(v);
	
	return;
}

REAL8Vector* XLALCreateREAL8Vector(UINT4 length)
{
	REAL8Vector* vector;
	vector = (REAL8Vector*)malloc(sizeof(*vector));
	vector->length = length;
	if ( ! length ) /* zero length: set data pointer to be NULL */
	{
		vector->data = NULL;
	}
	else /* non-zero length: allocate memory for data */
	{
		vector->data = (REAL8 *)malloc(length * sizeof( *vector->data));
	}
	
	return vector;
}

void XLALDestroyREAL8Vector(REAL8Vector* v)
{
	if(NULL == v)
	{
		return;
	}
	if(v->data)
	{
		v->length = 0;
		free(v->data);
	}
	v->data = NULL;
	free(v);
	
	return;
}
UINT4Vector* XLALCreateUINT4Vector(UINT4 length)
{
	UINT4Vector* vector;
	vector = (UINT4Vector*)malloc(sizeof(*vector));
	vector->length = length;
	if ( ! length ) /* zero length: set data pointer to be NULL */
	{
		vector->data = NULL;
	}
	else /* non-zero length: allocate memory for data */
	{
		vector->data = (UINT4 *)malloc(length * sizeof( *vector->data));
	}
	
	return vector;
}

void XLALDestroyUINT4Vector(UINT4Vector* v)
{
	if(NULL == v)
	{
		return;
	}
	if(v->data)
	{
		v->length = 0;
		free(v->data);
	}
	v->data = NULL;
	free(v);
	
	return;
}

REAL8Array* XLALCreateREAL8ArrayL(UINT4 ndim,...)
{
	
	enum { maxdim = 16 };
	va_list ap;
	REAL8Array *arr;
	UINT4 dims[maxdim];
	UINT4 dim;
	
	UINT4 size = 1;
	
	UINT4Vector dimLength;

	//即还有两个uint4参数
	va_start( ap, ndim );
	for ( dim = 0; dim < ndim; ++dim )
	  dims[dim] = va_arg( ap, UINT4 );
	va_end( ap );

	//向量长度为维数，数据为这个参数数组
	dimLength.length = ndim;
	dimLength.data	 = dims;

	//Array的数据存储空间的大小为后面n个参数的乘积
	for ( dim = 0; dim < ndim; ++dim )
	  size *= dimLength.data[dim];
	
	/* create array */
	arr = (REAL8Array *)malloc( sizeof( *arr ) );//为指针变量分配空间
	/* create array dimensions */
	arr->dimLength = XLALCreateUINT4Vector( ndim );
	if ( ! arr->dimLength )
	 {
	   free( arr );
	 }
	/* copy dimension lengths */
	memcpy( arr->dimLength->data, dimLength.data,
		ndim * sizeof( *arr->dimLength->data ) );
	/* allocate data storage */
	arr->data = (REAL8 *)malloc( size * sizeof( *arr->data ) );
	if ( ! arr->data )
	{
	  XLALDestroyUINT4Vector( arr->dimLength );
	  free( arr );
	}
	
	return arr;
}

void XLALDestroyREAL8Array(REAL8Array* array)
{
	XLALDestroyUINT4Vector( array->dimLength );
  	free( array->data );
  	free( array );
}

REAL8VectorSequence * XLALCreateREAL8VectorSequence ( UINT4 length, UINT4 veclen )
{
	
	REAL8VectorSequence *seq;
		
	seq = (REAL8VectorSequence *)malloc( sizeof( *seq ) );
	if ( ! seq )
	  fprintf(stderr,"Error(%d)", XLAL_ENOMEM );
	
	seq->length = length;
	seq->vectorLength = veclen;
	
	if ( ! length || ! veclen )
	  seq->data = NULL;
	else
	{
	  seq->data = (REAL8 *)malloc( length * veclen * sizeof( *seq->data ) );
	  if ( ! seq )
	  {
		free( seq );
		fprintf(stderr,"Error(%d)",XLAL_ENOMEM );
	  }
	}
	
	return seq;
}
void XLALDestroyREAL8VectorSequence ( REAL8VectorSequence * vseq )
{
	if ( ! vseq )
    return;
  if ( ( ! vseq->length || ! vseq->vectorLength ) && vseq->data )
	  fprintf(stderr,"Error(%d)", XLAL_EINVAL );
  if ( ! vseq->data && ( vseq->length || vseq->vectorLength ) )
	  fprintf(stderr,"Error(%d)" ,XLAL_EINVAL );
  if ( vseq->data )
    free( vseq->data );
  vseq->data = NULL; /* leave lengths as they are to indicate freed vector */
  free( vseq );
  return;
}

REAL8TimeSeries *XLALCreateREAL8TimeSeries ( const char *name, const LIGOTimeGPS *epoch, REAL8 f0, REAL8 deltaT, const LALUnit *sampleUnits, int length )
{
	REAL8TimeSeries *r8ts;
	REAL8Vector *r8sequence;

	r8ts = (REAL8TimeSeries *)malloc(sizeof(*r8ts));
	r8sequence = XLALCreateREAL8Vector(length);
	if(!r8ts || !r8sequence) {
		free(r8ts);
		XLALDestroyREAL8Vector (r8sequence);
	}

	if(name)
		strncpy(r8ts->name, name, LALNameLength);
	else
		r8ts->name[0] = '\0';
	r8ts->epoch = *epoch;
	r8ts->f0 = f0;
	r8ts->deltaT = deltaT;
	r8ts->sampleUnits = *sampleUnits;
	r8ts->data = r8sequence;

	return r8ts;
}

void XLALDestroyREAL8TimeSeries( REAL8TimeSeries * series )
{
	if(series)
		XLALDestroyREAL8Vector(series->data);
	free(series);
}


int XLALSimIMREOBGenerateQNMFreqV2_re(
  COMPLEX16Vector *modefreqs, /**<< OUTPUT, complex freqs of overtones in unit of Hz */
  const REAL8      mass1,     /**<< The mass of the 1st component (in Solar masses) */
  const REAL8      mass2,     /**<< The mass of the 2nd component (in Solar masses) */
  const REAL8      spin1[3],  /**<< The spin of the 1st object; only needed for spin waveforms */
  const REAL8      spin2[3],  /**<< The spin of the 2nd object; only needed for spin waveforms */
  UINT4            l,         /**<< The l value of the mode in question */
  UINT4            m,         /**<< The m value of the mode in question */
  UINT4            nmodes    /**<< The number of overtones that should be included (max 8) */
  )
{

  /* Data for interpolating the quasinormal mode frequencies is taken from 
   * The webpage of Vitor Cardoso, http://centra.ist.utl.pt/~vitor/?page=ringdown 
   * In the spin range of (-0.995, 0.999), 
   * interpolation error within 5e-5 for the first 3 overtones of the 22 mode 
   * interpolation error within 0.005 for the first 8 overtones of the 22 mode 
   * On the data points, interpolation error is within 1e-5, except for the 
   * highest overtone of the (4,4) and (5,5) modes.
   */

// a/M with M is the mass of the final black hole	
  const double afinallist[107] = {-0.9996, -0.9995, -0.9994, -0.9992, -0.999, -0.9989, -0.9988, 
  -0.9987, -0.9986, -0.9985, -0.998, -0.9975, -0.997, -0.996, -0.995, -0.994, -0.992, -0.99, -0.988, 
  -0.986, -0.984, -0.982, -0.98, -0.975, -0.97, -0.96, -0.95, -0.94, -0.92, -0.9, -0.88, -0.86, -0.84, 
  -0.82, -0.8, -0.78, -0.76, -0.74, -0.72, -0.7, -0.65, -0.6, -0.55, -0.5, -0.45, -0.4, -0.35, -0.3, 
  -0.25, -0.2, -0.15, -0.1, -0.05, 0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 
  0.65, 0.7, 0.72, 0.74, 0.76, 0.78, 0.8, 0.82, 0.84, 0.86, 0.88, 0.9, 0.92, 0.94, 0.95, 0.96, 0.97, 
  0.975, 0.98, 0.982, 0.984, 0.986, 0.988, 0.99, 0.992, 0.994, 0.995, 0.996, 0.997, 0.9975, 0.998, 
  0.9985, 0.9986, 0.9987, 0.9988, 0.9989, 0.999, 0.9992, 0.9994, 0.9995, 0.9996};

  /* 2, 0 mode */
// M\omega_R with M is the mass of the final black hole
  const double reomegaqnm20[8][107] = {{0.4247721439,0.4247863113,0.4247985972,0.4248325877,0.4248584259,0.4248659433,0.4248700764,
0.4248712306,0.4248698326,0.4248662798,0.4248267198,0.4247676547,0.4247005042,0.4245577530,0.4244119723,0.4242658712,
0.4239745379,0.4236846241,0.4233960302,0.4231086991,0.4228226189,0.4225377956,0.4222542405,0.4215509794,0.4208558870,0.4194906865,
0.4181592598,0.4168617799,0.4143677184,0.4120044663,0.4097658509,0.4076448500,0.4056343086,0.4037273098,0.4019173509,
0.4001984114,0.3985649653,0.3970119631,0.3955348022,0.3941292897,0.3909047276,0.3880539184,0.3855360484,0.3833182760,
0.3813740024,0.3796815867,0.3782233906,0.3769850625,0.3759550001,0.3751239445,0.3744846758,0.3740317881,0.3737615278,
0.3736716844,0.3737615278,0.3740317881,0.3744846758,0.3751239445,0.3759550001,0.3769850625,0.3782233906,0.3796815867,
0.3813740024,0.3833182760,0.3855360484,0.3880539184,0.3909047276,0.3941292897,0.3955348022,0.3970119631,0.3985649653,
0.4001984114,0.4019173509,0.4037273098,0.4056343086,0.4076448500,0.4097658509,0.4120044663,0.4143677184,0.4168617799,
0.4181592598,0.4194906865,0.4208558870,0.4215509794,0.4222542405,0.4225377956,0.4228226189,0.4231086991,0.4233960302,
0.4236846241,0.4239745379,0.4242658712,0.4244119723,0.4245577530,0.4247005042,0.4247676547,0.4248267198,0.4248662798,
0.4248698326,0.4248712306,0.4248700764,0.4248659433,0.4248584259,0.4248325877,0.4247985972,0.4247863113,0.4247721439},
	  {0.4022595044,0.4025176372,0.4026385253,0.4027146550,0.4027157267,0.4027081834,0.4026990446,0.4026895027,
0.4026801424,0.4026712204,0.4026338048,0.4026034279,0.4025749860,0.4025177609,0.4024587579,0.4023983315,0.4022740572,
0.4021454680,0.4020126089,0.4018755539,0.4017344007,0.4015892521,0.4014402120,0.4010512734,0.4006403236,0.3997591492,
0.3988102405,0.3978068020,0.3956841928,0.3934728792,0.3912313515,0.3889989525,0.3868012743,0.3846545743,0.3825688955,
0.3805501632,0.3786015741,0.3767245157,0.3749191785,0.3731849675,0.3691526314,0.3655335386,0.3623007731,0.3594283374,
0.3568927266,0.3546733772,0.3527526583,0.3511156895,0.3497501098,0.3486458530,0.3477949500,0.3471913675,0.3468308827,
0.3467109969,0.3468308827,0.3471913675,0.3477949500,0.3486458530,0.3497501098,0.3511156895,0.3527526583,0.3546733772,
0.3568927266,0.3594283374,0.3623007731,0.3655335386,0.3691526314,0.3731849675,0.3749191785,0.3767245157,0.3786015741,
0.3805501632,0.3825688955,0.3846545743,0.3868012743,0.3889989525,0.3912313515,0.3934728792,0.3956841928,0.3978068020,
0.3988102405,0.3997591492,0.4006403236,0.4010512734,0.4014402120,0.4015892521,0.4017344007,0.4018755539,0.4020126089,
0.4021454680,0.4022740572,0.4023983315,0.4024587579,0.4025177609,0.4025749860,0.4026034279,0.4026338048,0.4026712204,
0.4026801424,0.4026895027,0.4026990446,0.4027081834,0.4027157267,0.4027146550,0.4026385253,0.4025176372,0.4022595044},
	  {0.3587631202,0.3586562755,0.3585571621,0.3584767648,0.3584871698,0.3585031359,0.3585201960,0.3585364516,
0.3585511443,0.3585641498,0.3586123314,0.3586525789,0.3586945293,0.3587812110,0.3588663108,0.3589493444,0.3591108827,
0.3592664061,0.3594155854,0.3595583252,0.3596945387,0.3598241205,0.3599469676,0.3602240117,0.3604570682,0.3607872733,
0.3609323176,0.3608910420,0.3602774401,0.3590639204,0.3574099949,0.3554578534,0.3533158689,0.3510620393,0.3487514511,
0.3464228554,0.3441035438,0.3418127732,0.3395641339,0.3373671870,0.3321397710,0.3273292258,0.3229529922,0.3190100679,
0.3154913069,0.3123844448,0.3096766084,0.3073555756,0.3054103975,0.3038316900,0.3026117559,0.3017446212,0.3012260348,
0.3010534545,0.3012260348,0.3017446212,0.3026117559,0.3038316900,0.3054103975,0.3073555756,0.3096766084,0.3123844448,
0.3154913069,0.3190100679,0.3229529922,0.3273292258,0.3321397710,0.3373671870,0.3395641339,0.3418127732,0.3441035438,
0.3464228554,0.3487514511,0.3510620393,0.3533158689,0.3554578534,0.3574099949,0.3590639204,0.3602774401,0.3608910420,
0.3609323176,0.3607872733,0.3604570682,0.3602240117,0.3599469676,0.3598241205,0.3596945387,0.3595583252,0.3594155854,
0.3592664061,0.3591108827,0.3589493444,0.3588663108,0.3587812110,0.3586945293,0.3586525789,0.3586123314,0.3585641498,
0.3585511443,0.3585364516,0.3585201960,0.3585031359,0.3584871698,0.3584767648,0.3585571621,0.3586562755,0.3587631202},
	  {0.2955931981,0.2956911763,0.2959013663,0.2960902804,0.2960805341,0.2960706030,0.2960715835,0.2960841755,
0.2961057593,0.2961328408,0.2962715691,0.2963767257,0.2964791366,0.2967001479,0.2969184558,0.2971346512,0.2975687544,
0.2980006488,0.2984308575,0.2988593119,0.2992856405,0.2997097563,0.3001315265,0.3011744976,0.3021986114,0.3041793062,
0.3060420460,0.3077622800,0.3106733357,0.3125253856,0.3131974762,0.3128916813,0.3118658660,0.3103333453,0.3084523863,
0.3063370790,0.3040694520,0.3017088670,0.2992986826,0.2968708711,0.2908660077,0.2851210461,0.2797541132,0.2748239607,
0.2703586352,0.2663699371,0.2628610658,0.2598308118,0.2572759173,0.2551924300,0.2535764879,0.2524247751,0.2517347842,
0.2515049622,0.2517347842,0.2524247751,0.2535764879,0.2551924300,0.2572759173,0.2598308118,0.2628610658,0.2663699371,
0.2703586352,0.2748239607,0.2797541132,0.2851210461,0.2908660077,0.2968708711,0.2992986826,0.3017088670,0.3040694520,
0.3063370790,0.3084523863,0.3103333453,0.3118658660,0.3128916813,0.3131974762,0.3125253856,0.3106733357,0.3077622800,
0.3060420460,0.3041793062,0.3021986114,0.3011744976,0.3001315265,0.2997097563,0.2992856405,0.2988593119,0.2984308575,
0.2980006488,0.2975687544,0.2971346512,0.2969184558,0.2967001479,0.2964791366,0.2963767257,0.2962715691,0.2961328408,
0.2961057593,0.2960841755,0.2960715835,0.2960706030,0.2960805341,0.2960902804,0.2959013663,0.2956911763,0.2955931981},
	  {0.2265759655,0.2263134666,0.2259201107,0.2260332610,0.2263091114,0.2263241376,0.2262966437,0.2262653635,
0.2262534991,0.2262677342,0.2264910301,0.2266045970,0.2267226381,0.2270204493,0.2272816677,0.2275628966,0.2281119757,
0.2286682789,0.2292233708,0.2297821291,0.2303431133,0.2309055008,0.2314706692,0.2328917020,0.2343288764,0.2372425752,
0.2401516712,0.2432064357,0.2490028794,0.2551643774,0.2609821536,0.2649179918,0.2670263535,0.2677578325,0.2675054998,
0.2665561482,0.2651128405,0.2633200598,0.2612820783,0.2590753625,0.2531607164,0.2470884185,0.2411635985,0.2355561825,
0.2303648137,0.2256486007,0.2214438733,0.2177734415,0.2146518936,0.2120886995,0.2100900575,0.2086599993,0.2078010452,
0.2075145798,0.2078010452,0.2086599993,0.2100900575,0.2120886995,0.2146518936,0.2177734415,0.2214438733,0.2256486007,
0.2303648137,0.2355561825,0.2411635985,0.2470884185,0.2531607164,0.2590753625,0.2612820783,0.2633200598,0.2651128405,
0.2665561482,0.2675054998,0.2677578325,0.2670263535,0.2649179918,0.2609821536,0.2551643774,0.2490028794,0.2432064357,
0.2401516712,0.2372425752,0.2343288764,0.2328917020,0.2314706692,0.2309055008,0.2303431133,0.2297821291,0.2292233708,
0.2286682789,0.2281119757,0.2275628966,0.2272816677,0.2270204493,0.2267226381,0.2266045970,0.2264910301,0.2262677342,
0.2262534991,0.2262653635,0.2262966437,0.2263241376,0.2263091114,0.2260332610,0.2259201107,0.2263134666,0.2265759655},
	  {0.1624677140,0.1634934248,0.1638277421,0.1630968281,0.1635010049,0.1636759976,0.1636803676,0.1635845741,
0.1634883150,0.1634553864,0.1638050393,0.1637918910,0.1639844275,0.1642094594,0.1645146073,0.1647460864,0.1652882910,
0.1658098185,0.1663401778,0.1668757036,0.1674133259,0.1679475306,0.1685023684,0.1698847398,0.1712487246,0.1741108365,
0.1768267467,0.1804631771,0.1863051500,0.1935771765,0.1987452707,0.2093226733,0.2181547856,0.2237317269,0.2269036596,
0.2284118203,0.2287563990,0.2282723907,0.2271898972,0.2256715181,0.2206696442,0.2147729564,0.2085926425,0.2024747303,
0.1966287474,0.1911890101,0.1862461282,0.1818640842,0.1780897950,0.1749585446,0.1724970437,0.1707251041,0.1696565048,
0.1692994034,0.1696565048,0.1707251041,0.1724970437,0.1749585446,0.1780897950,0.1818640842,0.1862461282,0.1911890101,
0.1966287474,0.2024747303,0.2085926425,0.2147729564,0.2206696442,0.2256715181,0.2271898972,0.2282723907,0.2287563990,
0.2284118203,0.2269036596,0.2237317269,0.2181547856,0.2093226733,0.1987452707,0.1935771765,0.1863051500,0.1804631771,
0.1768267467,0.1741108365,0.1712487246,0.1698847398,0.1685023684,0.1679475306,0.1674133259,0.1668757036,0.1663401778,
0.1658098185,0.1652882910,0.1647460864,0.1645146073,0.1642094594,0.1639844275,0.1637918910,0.1638050393,0.1634553864,
0.1634883150,0.1635845741,0.1636803676,0.1636759976,0.1635010049,0.1630968281,0.1638277421,0.1634934248,0.1624677140},
	  {0.1150947175,0.1136995811,0.1144317617,0.1145108329,0.1143223733,0.1147075752,0.1148529865,0.1147211083,
0.1145101899,0.1144146127,0.1148757124,0.1147623558,0.1150527078,0.1152162011,0.1153522163,0.1156670599,0.1160937605,
0.1165085059,0.1169521137,0.1174121457,0.1178333408,0.1183089991,0.1188779907,0.1199614936,0.1212661221,0.1229079880,
0.1252696237,0.1300527498,0.1363714850,0.1365743504,0.1502937555,0.1499042247,0.1576063359,0.1737999474,0.1840565193,
0.1901954481,0.1937178657,0.1954878977,0.1960322476,0.1956928755,0.1923495252,0.1869716004,0.1806432357,0.1739650338,
0.1673020380,0.1608944901,0.1549126532,0.1494856455,0.1447169546,0.1406923509,0.1374833382,0.1351476926,0.1337284518,
0.1332523443,0.1337284518,0.1351476926,0.1374833382,0.1406923509,0.1447169546,0.1494856455,0.1549126532,0.1608944901,
0.1673020380,0.1739650338,0.1806432357,0.1869716004,0.1923495252,0.1956928755,0.1960322476,0.1954878977,0.1937178657,
0.1901954481,0.1840565193,0.1737999474,0.1576063359,0.1499042247,0.1502937555,0.1365743504,0.1363714850,0.1300527498,
0.1252696237,0.1229079880,0.1212661221,0.1199614936,0.1188779907,0.1183089991,0.1178333408,0.1174121457,0.1169521137,
0.1165085059,0.1160937605,0.1156670599,0.1153522163,0.1152162011,0.1150527078,0.1147623558,0.1148757124,0.1144146127,
0.1145101899,0.1147211083,0.1148529865,0.1147075752,0.1143223733,0.1145108329,0.1144317617,0.1136995811,0.1150947175},
	  {0.07824594797,0.07897765236,0.07781656751,0.07894919224,0.07811549687,0.07852555182,0.07887349260,0.07877853969,
0.07846824390,0.07832233515,0.07872416680,0.07887758159,0.07876636539,0.07904172864,0.07931185989,0.07935962619,0.07970963786,
0.08015321455,0.08049831171,0.08098647273,0.08113113975,0.08171084479,0.08245975407,0.08198058103,0.08310489532,0.08636947463,
0.08742026462,0.09301825285,0.09708260702,0.10449211510,0.09410791221,0.1163296909,0.1194610774,0.1071758642,0.1310599660,
0.1479210288,0.1575029326,0.1631326584,0.1662931627,0.1677797021,0.1669344646,0.1623398390,0.1558069565,0.1482827082,
0.1403267639,0.1323069592,0.1244931384,0.1171075385,0.1103537661,0.1044314061,0.09954024628,0.09587287090,0.09359533547,
0.09282239791,0.09359533547,0.09587287090,0.09954024628,0.1044314061,0.1103537661,0.1171075385,0.1244931384,0.1323069592,
0.1403267639,0.1482827082,0.1558069565,0.1623398390,0.1669344646,0.1677797021,0.1662931627,0.1631326584,0.1575029326,
0.1479210288,0.1310599660,0.1071758642,0.1194610774,0.1163296909,0.09410791221,0.10449211510,0.09708260702,0.09301825285,
0.08742026462,0.08636947463,0.08310489532,0.08198058103,0.08245975407,0.08171084479,0.08113113975,0.08098647273,0.08049831171,
0.08015321455,0.07970963786,0.07935962619,0.07931185989,0.07904172864,0.07876636539,0.07887758159,0.07872416680,0.07832233515,
0.07846824390,0.07877853969,0.07887349260,0.07852555182,0.07811549687,0.07894919224,0.07781656751,0.07897765236,0.07824594797}};

// M\omega_I with M is the mass of the final black hole
  const double imomegaqnm20[8][107] = {{0.06956111286,0.07057665227,0.07109516199,0.07156667263,0.07176004502,0.07181519360,
0.07185564256,0.07188639200,0.07191058835,0.07193027264,0.07199608801,0.07204342275,0.07208725348,0.07217445117,
0.07226259362,0.07235103048,0.07252698509,0.07270070919,0.07287193971,0.07304067834,0.07320697034,0.07337086430,
0.07353240627,0.07392625431,0.07430631954,0.07502754477,0.07570073722,0.07633017500,0.07747297842,0.07848269543,
0.07938083725,0.08018464701,0.08090799941,0.08156214052,0.08215626528,0.08269795953,0.08319353674,0.08364829544,
0.08406671695,0.08445261881,0.08529549690,0.08599466937,0.08657885062,0.08706905134,0.08748097560,0.08782655405,
0.08811495632,0.08835327799,0.08854701612,0.08870040374,0.08881664656,0.08889809023,0.08894633586,0.08896231569,
0.08894633586,0.08889809023,0.08881664656,0.08870040374,0.08854701612,0.08835327799,0.08811495632,0.08782655405,
0.08748097560,0.08706905134,0.08657885062,0.08599466937,0.08529549690,0.08445261881,0.08406671695,0.08364829544,
0.08319353674,0.08269795953,0.08215626528,0.08156214052,0.08090799941,0.08018464701,0.07938083725,0.07848269543,
0.07747297842,0.07633017500,0.07570073722,0.07502754477,0.07430631954,0.07392625431,0.07353240627,0.07337086430,
0.07320697034,0.07304067834,0.07287193971,0.07270070919,0.07252698509,0.07235103048,0.07226259362,0.07217445117,
0.07208725348,0.07204342275,0.07199608801,0.07193027264,0.07191058835,0.07188639200,0.07185564256,0.07181519360,
0.07176004502,0.07156667263,0.07109516199,0.07057665227,0.06956111286},
   {0.2185411094,0.2185862168,0.2185760597,0.2185502469,0.2185551556,0.2185670597,0.2185832072,0.2186023833,
0.2186236698,0.2186463949,0.2187689601,0.2188938400,0.2190172960,0.2192613118,0.2195038699,0.2197457780,0.2202276534,
0.2207065981,0.2211824488,0.2216551307,0.2221245734,0.2225907052,0.2230534566,0.2241951244,0.2253143564,0.2274827082,
0.2295545126,0.2315282566,0.2351860943,0.2384795534,0.2414444658,0.2441189781,0.2465387707,0.2487354182,0.2507361819,
0.2525643629,0.2542398214,0.2557795018,0.2571979034,0.2585074823,0.2613723978,0.2637535915,0.2657465547,0.2674214198,
0.2688307276,0.2700144584,0.2710033792,0.2718213228,0.2724867699,0.2730139630,0.2734136955,0.2736938697,0.2738598819,
0.2739148753,0.2738598819,0.2736938697,0.2734136955,0.2730139630,0.2724867699,0.2718213228,0.2710033792,0.2700144584,
0.2688307276,0.2674214198,0.2657465547,0.2637535915,0.2613723978,0.2585074823,0.2571979034,0.2557795018,0.2542398214,
0.2525643629,0.2507361819,0.2487354182,0.2465387707,0.2441189781,0.2414444658,0.2384795534,0.2351860943,0.2315282566,
0.2295545126,0.2274827082,0.2253143564,0.2241951244,0.2230534566,0.2225907052,0.2221245734,0.2216551307,0.2211824488,
0.2207065981,0.2202276534,0.2197457780,0.2195038699,0.2192613118,0.2190172960,0.2188938400,0.2187689601,0.2186463949,
0.2186236698,0.2186023833,0.2185832072,0.2185670597,0.2185551556,0.2185502469,0.2185760597,0.2185862168,0.2185411094},
   {0.3746283234,0.3744760286,0.3744655082,0.3745713459,0.3746804416,0.3747239651,0.3747616125,0.3747951976,
0.3748262688,0.3748559959,0.3750036033,0.3751595179,0.3753190966,0.3756378976,0.3759558332,0.3762746082,0.3769149521,
0.3775582079,0.3782042318,0.3788529456,0.3795041978,0.3801578285,0.3808136809,0.3824619707,0.3841204813,0.3874566765,
0.3907983942,0.3941199491,0.4005962971,0.4067113438,0.4123795696,0.4175861754,0.4223525880,0.4267144430,0.4307106786,
0.4343787455,0.4377527783,0.4408631174,0.4437364171,0.4463959791,0.4522370945,0.4571173234,0.4612213527,0.4646855141,
0.4676122255,0.4700795615,0.4721476900,0.4738632779,0.4752625561,0.4763734789,0.4772172535,0.4778094206,0.4781606008,
0.4782769831,0.4781606008,0.4778094206,0.4772172535,0.4763734789,0.4752625561,0.4738632779,0.4721476900,0.4700795615,
0.4676122255,0.4646855141,0.4612213527,0.4571173234,0.4522370945,0.4463959791,0.4437364171,0.4408631174,0.4377527783,
0.4343787455,0.4307106786,0.4267144430,0.4223525880,0.4175861754,0.4123795696,0.4067113438,0.4005962971,0.3941199491,
0.3907983942,0.3874566765,0.3841204813,0.3824619707,0.3808136809,0.3801578285,0.3795041978,0.3788529456,0.3782042318,
0.3775582079,0.3769149521,0.3762746082,0.3759558332,0.3756378976,0.3753190966,0.3751595179,0.3750036033,0.3748559959,
0.3748262688,0.3747951976,0.3747616125,0.3747239651,0.3746804416,0.3745713459,0.3744655082,0.3744760286,0.3746283234},
   {0.5488977566,0.5492725578,0.5493796167,0.5493030521,0.5492800515,0.5493081779,0.5493493533,0.5493946353,
0.5494384190,0.5494780738,0.5496229054,0.5497549686,0.5499025591,0.5501989882,0.5504901744,0.5507853393,0.5513807914,
0.5519818189,0.5525897750,0.5532041749,0.5538251797,0.5544529382,0.5550874672,0.5567036937,0.5583633611,0.5618146957,
0.5654450115,0.5692386679,0.5773386388,0.5859421423,0.5945472028,0.6027877630,0.6105064194,0.6176644895,0.6242770530,
0.6303804075,0.6360171698,0.6412297490,0.6460576806,0.6505367243,0.6604082105,0.6686938222,0.6756913514,0.6816215439,
0.6866504101,0.6909045159,0.6944813958,0.6974567098,0.6998892223,0.7018243015,0.7032963922,0.7043307557,0.7049446682,
0.7051482024,0.7049446682,0.7043307557,0.7032963922,0.7018243015,0.6998892223,0.6974567098,0.6944813958,0.6909045159,
0.6866504101,0.6816215439,0.6756913514,0.6686938222,0.6604082105,0.6505367243,0.6460576806,0.6412297490,0.6360171698,
0.6303804075,0.6242770530,0.6176644895,0.6105064194,0.6027877630,0.5945472028,0.5859421423,0.5773386388,0.5692386679,
0.5654450115,0.5618146957,0.5583633611,0.5567036937,0.5550874672,0.5544529382,0.5538251797,0.5532041749,0.5525897750,
0.5519818189,0.5513807914,0.5507853393,0.5504901744,0.5501989882,0.5499025591,0.5497549686,0.5496229054,0.5494780738,
0.5494384190,0.5493946353,0.5493493533,0.5493081779,0.5492800515,0.5493030521,0.5493796167,0.5492725578,0.5488977566},
   {0.7501379784,0.7495636806,0.7496503080,0.7501258019,0.7500613683,0.7499927446,0.7499667401,0.7499862570,
0.7500353707,0.7500943083,0.7502378208,0.7502955686,0.7504262429,0.7506329733,0.7508454096,0.7510636293,0.7514901194,
0.7519267005,0.7523658758,0.7528118500,0.7532615237,0.7537170118,0.7541777199,0.7553533494,0.7565672969,0.7591011388,
0.7618191441,0.7646738706,0.7712404750,0.7783465723,0.7875759366,0.7980345082,0.8085705185,0.8186647248,0.8281387322,
0.8369567431,0.8451396344,0.8527290961,0.8597724928,0.8663164947,0.8807666598,0.8929239587,0.9032139277,0.9119531958,
0.9193796679,0.9256743556,0.9309764881,0.9353939337,0.9390104311,0.9418906437,0.9440837035,0.9456256811,0.9465412730,
0.9468448909,0.9465412730,0.9456256811,0.9440837035,0.9418906437,0.9390104311,0.9353939337,0.9309764881,0.9256743556,
0.9193796679,0.9119531958,0.9032139277,0.8929239587,0.8807666598,0.8663164947,0.8597724928,0.8527290961,0.8451396344,
0.8369567431,0.8281387322,0.8186647248,0.8085705185,0.7980345082,0.7875759366,0.7783465723,0.7712404750,0.7646738706,
0.7618191441,0.7591011388,0.7565672969,0.7553533494,0.7541777199,0.7537170118,0.7532615237,0.7528118500,0.7523658758,
0.7519267005,0.7514901194,0.7510636293,0.7508454096,0.7506329733,0.7504262429,0.7502955686,0.7502378208,0.7500943083,
0.7500353707,0.7499862570,0.7499667401,0.7499927446,0.7500613683,0.7501258019,0.7496503080,0.7495636806,0.7501379784},
   {0.9742529776,0.9747874524,0.9741123398,0.9741101898,0.9745054107,0.9743713094,0.9742131924,0.9741459996,
0.9741878531,0.9742927333,0.9744241227,0.9744440584,0.9745998745,0.9746798313,0.9748508069,0.9749764636,0.9752738201,
0.9755817532,0.9758699637,0.9761776339,0.9764736452,0.9767861418,0.9770899129,0.9778706747,0.9786413703,0.9802309584,
0.9821505190,0.9839171716,0.9872274650,0.9938892699,0.9973348177,1.0029600920,1.0137082960,1.0258619280,1.0379031380,
1.0493556480,1.0600886610,1.0700928400,1.0794025970,1.0880666360,1.1072318520,1.1233872540,1.1370869350,1.1487453930,
1.1586731910,1.1671054460,1.1742220930,1.1801619460,1.1850324510,1.1889164550,1.1918768980,1.1939600320,1.1951975740,
1.1956080540,1.1951975740,1.1939600320,1.1918768980,1.1889164550,1.1850324510,1.1801619460,1.1742220930,1.1671054460,
1.1586731910,1.1487453930,1.1370869350,1.1233872540,1.1072318520,1.0880666360,1.0794025970,1.0700928400,1.0600886610,
1.0493556480,1.0379031380,1.0258619280,1.0137082960,1.0029600920,0.9973348177,0.9938892699,0.9872274650,0.9839171716,
0.9821505190,0.9802309584,0.9786413703,0.9778706747,0.9770899129,0.9767861418,0.9764736452,0.9761776339,0.9758699637,
0.9755817532,0.9752738201,0.9749764636,0.9748508069,0.9746798313,0.9745998745,0.9744440584,0.9744241227,0.9742927333,
0.9741878531,0.9741459996,0.9742131924,0.9743713094,0.9745054107,0.9741101898,0.9741123398,0.9747874524,0.9742529776},
   {1.2115268570,1.2119224830,1.2127581510,1.2117269920,1.2124824650,1.2124753940,1.2122054860,1.2120007160,
1.2120183490,1.2121990080,1.2122129970,1.2124447280,1.2123706080,1.2125862000,1.2126197320,1.2127468970,1.2129846840,
1.2132075340,1.2134023920,1.2136367370,1.2138479250,1.2141571580,1.2143110530,1.2150808200,1.2157732190,1.2167214260,
1.2186909060,1.2196006930,1.2213948240,1.2231508920,1.2289533870,1.2368231740,1.2309780760,1.2383599140,1.2512352410,
1.2647465460,1.2777995720,1.2901129870,1.3016351760,1.3123895010,1.3362428600,1.3564067550,1.3735550870,1.3881973760,
1.4007134530,1.4113878280,1.4204352180,1.4280186030,1.4342618660,1.4392588030,1.4430794470,1.4457744710,1.4473782250,
1.4479106320,1.4473782250,1.4457744710,1.4430794470,1.4392588030,1.4342618660,1.4280186030,1.4204352180,1.4113878280,
1.4007134530,1.3881973760,1.3735550870,1.3564067550,1.3362428600,1.3123895010,1.3016351760,1.2901129870,1.2777995720,
1.2647465460,1.2512352410,1.2383599140,1.2309780760,1.2368231740,1.2289533870,1.2231508920,1.2213948240,1.2196006930,
1.2186909060,1.2167214260,1.2157732190,1.2150808200,1.2143110530,1.2141571580,1.2138479250,1.2136367370,1.2134023920,
1.2132075340,1.2129846840,1.2127468970,1.2126197320,1.2125862000,1.2123706080,1.2124447280,1.2122129970,1.2121990080,
1.2120183490,1.2120007160,1.2122054860,1.2124753940,1.2124824650,1.2117269920,1.2127581510,1.2119224830,1.2115268570},
   {1.4571663840,1.4557744370,1.4561406500,1.4561633880,1.4563989150,1.4567024440,1.4564520290,1.4561110480,
1.4560841830,1.4563431260,1.4562032670,1.4566118340,1.4564050080,1.4564957940,1.4567084750,1.4568561130,1.4570324500,
1.4571992550,1.4572168310,1.4577583970,1.4580789520,1.4583441320,1.4581619640,1.4585416390,1.4580717000,1.4621403950,
1.4642010920,1.4637245060,1.4685741490,1.4643467300,1.4727222970,1.4673766030,1.4833905610,1.4800758950,1.4701776130,
1.4825655120,1.4971099840,1.5114169920,1.5249950640,1.5377449720,1.5661545750,1.5902726720,1.6108788240,1.6285796450,
1.6438274760,1.6569573710,1.6682159440,1.6777810640,1.6857758350,1.6922778140,1.6973286180,1.7009421340,1.7031156000,
1.7038413270,1.7031156000,1.7009421340,1.6973286180,1.6922778140,1.6857758350,1.6777810640,1.6682159440,1.6569573710,
1.6438274760,1.6285796450,1.6108788240,1.5902726720,1.5661545750,1.5377449720,1.5249950640,1.5114169920,1.4971099840,
1.4825655120,1.4701776130,1.4800758950,1.4833905610,1.4673766030,1.4727222970,1.4643467300,1.4685741490,1.4637245060,
1.4642010920,1.4621403950,1.4580717000,1.4585416390,1.4581619640,1.4583441320,1.4580789520,1.4577583970,1.4572168310,
1.4571992550,1.4570324500,1.4568561130,1.4567084750,1.4564957940,1.4564050080,1.4566118340,1.4562032670,1.4563431260,
1.4560841830,1.4561110480,1.4564520290,1.4567024440,1.4563989150,1.4561633880,1.4561406500,1.4557744370,1.4571663840}};

  /* 2, 2 mode */

  const double reomegaqnm22[8][107] = {{0.270228, 0.276562, 0.280636, 0.285234, 0.287548, 0.288282,
  0.288845, 0.289287, 0.289639, 0.289924, 0.290781, 0.291189, 0.291418, 0.291658, 0.291785, 0.29187, 
  0.291998, 0.292111, 0.292221, 0.292331, 0.292441, 0.292552, 0.292664, 0.292943, 0.293223, 0.293787, 0.294354, 0.294925, 0.296077, 0.297244, 0.298426, 0.299624, 0.300837, 0.302067, 0.303313, 0.304577, 0.305857, 0.307156, 0.308473, 0.309808, 0.313232, 0.316784, 0.320473, 0.324307, 0.328299, 0.332458, 0.336798, 0.341333, 0.346079, 0.351053, 0.356275, 0.361768, 0.367557, 0.373672, 0.380146, 0.387018, 0.394333, 0.402145, 0.410518, 0.419527, 0.429264, 0.439842, 0.451402, 0.464123, 0.478235, 0.494045, 0.511969, 0.5326, 0.541794, 0.55163, 0.562201, 0.573616, 0.586017, 0.59958, 0.614539, 0.631206, 0.650018, 0.671614, 0.696995, 0.727875, 0.74632, 0.767674, 0.793208, 0.808235, 0.825429, 0.8331, 0.841343, 0.850272, 0.860046, 0.870893, 0.883162, 0.897446, 0.905664, 0.914902, 0.925581, 0.931689, 0.938524, 0.946385, 0.948123, 0.949929, 0.951813, 0.953784, 0.955854, 0.960358, 0.965514, 0.968438, 0.97169}, 
{0.243689, 0.247076, 0.248937, 0.250577, 0.251041, 0.251088, 0.251073, 0.251023, 0.250952, 0.250874,
	0.25052, 0.250339, 0.250292, 0.250359, 0.250463, 0.250549, 0.250686, 0.250814, 0.250945, 0.251079, 0.251214, 0.251349, 0.251484, 0.25182, 0.252158, 0.252836, 0.253519, 0.254205, 0.255591, 0.256994, 0.258413, 0.259851, 0.261306, 0.26278, 0.264273, 0.265785, 0.267317, 0.268869, 0.270442, 0.272036, 0.276117, 0.280342, 0.284721, 0.289262, 0.293978, 0.29888, 0.303981, 0.309295, 0.314838, 0.320629, 0.326687, 0.333036, 0.339701, 0.346711, 0.354101, 0.36191, 0.370183, 0.378976, 0.388353, 0.39839, 0.409183, 0.420847, 0.433527, 0.447407, 0.462728, 0.479807, 0.499079, 0.521161, 0.53097, 0.541447, 0.552684, 0.564795, 0.577922, 0.592247, 0.608001, 0.625499, 0.645173, 0.667658, 0.693938, 0.725708, 0.744582, 0.766349, 0.792272, 0.807482, 0.824852, 0.832591, 0.840901, 0.849896, 0.859735, 0.870645, 0.882977, 0.897322, 0.905568, 0.914834, 0.925538, 0.931657, 0.938502, 0.946371, 0.94811, 0.949919, 0.951804, 0.953776, 0.955847, 0.960353, 0.96551, 0.968435, 0.971688}, 
{0.191626, 0.188311, 0.185222, 0.182107, 0.182532, 0.183173, 0.183784, 0.184279, 0.184634, 0.18486,
	0.184811, 0.184353, 0.184263, 0.184581, 0.184713, 0.18472, 0.184862, 0.185055, 0.185205, 0.185357, 0.185521, 0.185687, 0.18585, 0.18625, 0.186658, 0.187473, 0.188294, 0.189121, 0.190789, 0.192478, 0.194189, 0.195923, 0.19768, 0.19946, 0.201263, 0.203092, 0.204945, 0.206823, 0.208728, 0.210659, 0.215608, 0.220737, 0.226056, 0.231577, 0.237312, 0.243272, 0.249472, 0.255926, 0.262651, 0.269665, 0.276988, 0.284642, 0.292654, 0.301053, 0.309873, 0.319153, 0.328939, 0.339285, 0.350255, 0.361927, 0.374396, 0.387779, 0.402225, 0.417925, 0.43513, 0.454179, 0.475545, 0.499906, 0.510701, 0.522217, 0.534557, 0.547847, 0.56224, 0.577926, 0.595148, 0.614222, 0.635576, 0.659827, 0.687923, 0.721496, 0.741239, 0.763831, 0.790516, 0.806079, 0.823779, 0.831643, 0.840075, 0.849189, 0.859144, 0.870168, 0.882612, 0.897067, 0.905368, 0.914687, 0.925444, 0.931587, 0.938454, 0.946343, 0.948085, 0.949897, 0.951785, 0.95376, 0.955833, 0.960343, 0.965503, 0.96843, 0.971683}, 
{0.127766, 0.134925, 0.137314, 0.135026, 0.132545, 0.132613, 0.133213, 0.133927, 0.134502, 0.134846, 0.134197, 
0.133839, 0.134383, 0.134311, 0.134305, 0.134548, 0.134538, 0.134764, 0.134855, 0.13499, 0.135153, 0.135276, 0.135401, 0.13576, 0.136094, 0.136791, 0.137494, 0.138199, 0.139634, 0.141097, 0.142587, 0.144106, 0.145656, 0.147236, 0.148849, 0.150493, 0.152171, 0.153884, 0.155632, 0.157416, 0.162043, 0.16692, 0.172065, 0.177495, 0.183228, 0.18928, 0.19567, 0.202416, 0.209537, 0.217053, 0.224984, 0.233352, 0.242183, 0.251505, 0.261348, 0.271749, 0.282749, 0.294397, 0.306753, 0.319886, 0.333884, 0.348856, 0.364938, 0.382309, 0.40121, 0.421977, 0.445099, 0.471336, 0.482957, 0.495375, 0.508729, 0.523187, 0.538956, 0.556285, 0.575464, 0.59683, 0.620788, 0.647869, 0.678894, 0.715333, 0.736426, 0.760279, 0.788101, 0.804173, 0.822338, 0.830374, 0.83897, 0.848242, 0.858347, 0.869515, 0.8821, 0.896696, 0.905068, 0.91446, 0.925289, 0.931468, 0.938371, 0.946292, 0.948041, 0.949858, 0.951752, 0.953732, 0.955811, 0.960329, 0.965496, 0.968424, 0.971679}, 
{0.111845, 0.106167, 0.107777, 0.111206, 0.108936, 0.108364, 0.108639, 0.109304, 0.109887, 0.110167,
	0.109102, 0.109714, 0.109696, 0.109638, 0.109676, 0.109676, 0.109779, 0.109961, 0.109986, 0.110136, 0.11021, 0.110304, 0.110427, 0.110658, 0.110906, 0.111408, 0.111916, 0.112428, 0.113458, 0.114508, 0.115576, 0.116663, 0.117771, 0.118902, 0.120054, 0.12123, 0.122431, 0.123659, 0.124914, 0.126199, 0.12955, 0.133123, 0.136951, 0.141068, 0.145514, 0.150331, 0.155561, 0.161252, 0.167448, 0.174197, 0.18154, 0.189518, 0.198166, 0.207515, 0.217594, 0.228433, 0.24006, 0.252511, 0.265825, 0.280057, 0.295269, 0.311546, 0.328992, 0.347741, 0.367967, 0.389902, 0.413872, 0.440385, 0.451887, 0.464034, 0.476972, 0.490925, 0.506263, 0.523592, 0.543861, 0.568205, 0.597077, 0.629836, 0.666171, 0.707148, 0.730203, 0.755806, 0.785154, 0.801892, 0.82065, 0.828901, 0.837698, 0.847157, 0.857436, 0.868766, 0.881504, 0.896248, 0.904696, 0.914167, 0.925079, 0.931302, 0.938248, 0.946214, 0.947971, 0.949797, 0.951699, 0.953687, 0.955773, 0.960306, 0.965484, 0.968417, 0.971675}, 
{0.0973518, 0.0987518, 0.0952128, 0.097231, 0.0968051, 0.0959061, 0.0958383, 0.0963991, 0.0969597, 0.0971588, 0.0962907, 0.0968828, 0.0964446, 0.0967173, 0.0968133, 0.0967445, 0.0968961, 0.0970152, 0.0970355, 0.0971754, 0.0972259, 0.0973507, 0.097408, 0.0976397, 0.0978753, 0.0983115, 0.0987594, 0.099209, 0.100108, 0.101005, 0.101904, 0.102805, 0.103707, 0.104611, 0.10552, 0.106431, 0.107346, 0.108266, 0.109192, 0.110125, 0.112493, 0.114931, 0.117463, 0.120121, 0.122943, 0.125975, 0.129278, 0.132931, 0.137032, 0.14171, 0.147116, 0.153415, 0.160767, 0.169299, 0.179085, 0.19014, 0.202441, 0.215947, 0.230623, 0.246457, 0.263464, 0.281691, 0.301218, 0.322157, 0.344655, 0.368896, 0.39509, 0.42345, 0.43543, 0.447764, 0.460418, 0.473311, 0.486283, 0.499037,
0.511101, 0.522095, 0.532461, 0.536917, 0.527548, 0.521465, 0.518438, 0.515417, 0.51241, 0.510911, 0.509415, 0.508818, 0.508221, 0.507624, 0.507028, 0.506432, 0.505837, 0.505242, 0.504944, 0.504647, 0.50435, 0.504202, 0.504054, 0.503904, 0.503874, 0.503845, 0.503817, 0.503789, 0.50376, 0.503692, 0.503638, 0.503632, 0.503585},
{0.084098, 0.0868741, 0.0879148, 0.0858816, 0.0871769, 0.0862173, 0.0858779, 0.0863779, 0.0869438, 0.0870509, 0.0866021, 0.0864788, 0.0867758, 0.0866752, 0.0866613, 0.0868868, 0.086965, 0.0870574, 0.0871075, 0.0872251, 0.0873545, 0.0874293, 0.087563, 0.0878271, 0.088086, 0.0886037, 0.0891314, 0.0896576, 0.0906978, 0.0917299, 0.0927446, 0.093744, 0.0947274, 0.0956935, 0.0966414, 0.0975719, 0.0984848, 0.0993786, 0.100255, 0.101113, 0.103185, 0.105155, 0.107036, 0.108838, 0.110571, 0.112244, 0.113866, 0.115452, 0.117034, 0.118689, 0.120596, 0.123138, 0.127029, 0.133252, 0.142539, 0.154841, 0.169536, 0.185971, 0.203721, 0.222585, 0.242517, 0.263571, 0.285865, 0.309569, 0.334899, 0.362133, 0.391629, 0.423854, 0.437641, 0.45201, 0.467014, 0.482713, 0.499165, 0.516428, 0.534543, 0.553525, 0.573596, 0.602227, 0.649295, 0.697231, 0.722841, 0.750611, 0.781797, 0.799333,
0.818797, 0.827303, 0.836336, 0.846012, 0.856487, 0.867993, 0.880887, 0.895775, 0.904293, 0.913837, 0.92483, 0.931097, 0.938091, 0.946107, 0.947875, 0.949711, 0.951623, 0.953621, 0.955717, 0.960269, 0.965464, 0.968404, 0.971669}, 
{0.0783357, 0.0733796, 0.0766204, 0.0744319, 0.0764107, 0.0754793, 0.0749396, 0.0755031, 0.0761004,
	0.0760294, 0.0759874, 0.075511, 0.0757765, 0.0758322, 0.0759714, 0.0759232, 0.0761145, 0.0762289, 0.0763205, 0.0764184, 0.0765972, 0.0767207, 0.0768409, 0.0771736, 0.0775187, 0.07821, 0.0789117, 0.0796137, 0.0810158, 0.0824117, 0.0837964, 0.0851528, 0.0864792, 0.0877698, 0.0890208, 0.0902295, 0.0913937, 0.0925132, 0.0935883, 0.0946171, 0.0969926, 0.0990908, 0.100921, 0.102489, 0.103795, 0.104821, 0.10553, 0.105854, 0.105679, 0.104817, 0.102968, 0.0997112, 0.094996, 0.0928224, 0.102996, 0.122468, 0.144233,  0.166117, 0.187881, 0.209753, 0.232042, 0.255048, 0.279057, 0.304354, 0.331244, 0.360084, 0.391319, 0.425537, 0.440234, 0.455604, 0.471729, 0.488705, 0.506652, 0.52572, 0.546107, 0.568081, 0.592057, 0.618799, 0.650217, 0.691189, 0.716737, 0.745623, 0.778341, 0.796657, 0.816858, 0.825638, 0.834931, 0.844846, 0.855539, 0.867237, 0.880294, 0.895318, 0.903898, 0.913504, 0.924565, 0.930871, 0.93791, 0.945976, 0.947755, 0.949602, 0.951525, 0.953535, 0.955643, 0.960217, 0.965434, 0.968355, 0.971639}};

   const double imomegaqnm22[8][107] = {{0.0784607, 0.0801007, 0.0816472, 0.0840037, 0.0854954,
  	0.0860129, 0.0864215, 0.0867455, 0.0870035, 0.08721, 0.0877802, 0.0879878, 0.088065, 0.0880965,
  	0.0880878, 0.0880757, 0.0880612, 0.0880574, 0.0880589, 0.0880627, 0.0880675, 0.0880727, 0.088078, 0.0880913, 0.0881045, 0.0881304, 0.088156, 0.0881813, 0.0882315, 0.0882807, 0.0883289, 0.0883763, 0.0884226, 0.0884679, 0.0885122, 0.0885555, 0.0885976, 0.0886386, 0.0886785, 0.0887172, 0.0888085, 0.0888917, 0.0889663, 0.0890315, 0.0890868, 0.0891313, 0.0891643, 0.0891846, 0.0891911, 0.0891825, 0.0891574, 0.0891138, 0.0890496, 0.0889623, 0.0888489, 0.0887057, 0.0885283, 0.0883112, 0.0880477, 0.0877293, 0.0873453, 0.086882, 0.0863212, 0.0856388, 0.0848021, 0.0837652, 0.0824618, 0.0807929, 0.0799908, 0.0790927, 0.0780817, 0.0769364, 0.0756296, 0.0741258, 0.072378, 0.0703215, 0.0678642, 0.0648692, 0.0611186, 0.0562313, 0.053149, 0.0494336, 0.0447904, 0.0419586, 0.0386302, 0.0371155, 0.0354676, 0.033659, 0.0316516, 0.0293904, 0.0267908, 0.0237095, 0.0219107, 0.0198661, 0.0174737, 0.0160919, 0.014534, 0.0127274, 0.0123259, 0.0119077, 0.0114708, 0.0110127, 0.0105306, 0.00947799, 0.00826689, 0.00757704, 0.0068074}, 
{0.284478, 0.282567, 0.281162, 0.279245, 0.278072, 0.27767, 0.277358, 0.277118, 0.276936, 0.2768,
	0.276536, 0.276565, 0.276647, 0.276753, 0.276778, 0.276775, 0.276764, 0.276768, 0.276776, 0.276784, 0.276791, 0.276797, 0.276803, 0.276818, 0.276834, 0.276864, 0.276893, 0.276922, 0.276977, 0.277028, 0.277076, 0.27712, 0.27716, 0.277197, 0.27723, 0.277259, 0.277283, 0.277304, 0.277321, 0.277333, 0.277344, 0.277326, 0.277278, 0.277196, 0.27708, 0.276926, 0.276732, 0.276495, 0.276212, 0.275877, 0.275486, 0.275033, 0.274512, 0.273915, 0.273232, 0.272452, 0.271562, 0.270546, 0.269383, 0.268049, 0.266512, 0.264733, 0.262661, 0.260225, 0.257331, 0.253847, 0.24958, 0.244238, 0.241705, 0.238888, 0.235736, 0.232183, 0.228149, 0.223524, 0.218165, 0.211876, 0.204376, 0.195252, 0.183847, 0.169019, 0.159687, 0.148458, 0.134453, 0.125927, 0.115917, 0.111365, 0.106415, 0.100985, 0.0949599, 0.0881754, 0.080377, 0.0711342, 0.0657383, 0.0596046, 0.0524264, 0.04828, 0.043605, 0.0381837, 0.0369789, 0.0357239, 0.0344128, 0.0330384, 0.0315916, 0.0284334, 0.0247999, 0.0227304, 0.0204216}, 
{0.504534, 0.501693, 0.501238, 0.503711, 0.506007, 0.50649, 0.506633, 0.506559, 0.506365, 0.50612, 
	0.505173, 0.50512, 0.505381, 0.505519, 0.505371, 0.505318, 0.505354, 0.505311, 0.505262, 0.505236, 0.50521, 0.505177, 0.505141, 0.505059, 0.504978, 0.504812, 0.504645, 0.504476, 0.504135, 0.503786, 0.503432, 0.50307, 0.502702, 0.502326, 0.501944, 0.501554, 0.501156, 0.500751, 0.500338, 0.499917, 0.498828, 0.497686, 0.496488, 0.49523, 0.49391, 0.492523, 0.491065, 0.489531, 0.487913, 0.486206, 0.484399, 0.482484, 0.480448, 0.478277, 0.475955, 0.473463, 0.470778, 0.467872, 0.464713, 0.46126, 0.457463, 0.453259, 0.448569, 0.443287, 0.437269, 0.430315, 0.422131, 0.412262, 0.407689, 0.402664, 0.397103, 0.390894, 0.383895, 0.375919, 0.366715, 0.35594, 0.34311, 0.327518, 0.308058, 0.282827, 0.266998, 0.248008, 0.22441, 0.210086, 0.193307, 0.18569, 0.177413, 0.16834, 0.158283, 0.146966, 0.133965, 0.118563, 0.109572, 0.0993515, 0.0873891, 0.0804781, 0.0726848, 0.0636465, 0.0616378, 0.0595453, 0.0573593, 0.0550676, 0.0526555, 0.0473899, 0.0413327, 0.037883, 0.0340348}, 
{0.777682, 0.77817, 0.774907, 0.771079, 0.772456, 0.773571, 0.774249, 0.774423, 0.774236, 0.773865,
	0.772678, 0.773376, 0.773489, 0.77299, 0.773229, 0.773121, 0.773005, 0.772942, 0.772818, 0.772776, 0.77268, 0.772579, 0.772506, 0.772294, 0.772084, 0.771654, 0.771224, 0.770787, 0.7699, 0.768995, 0.768072, 0.767129, 0.766168, 0.765187, 0.764186, 0.763165, 0.762123, 0.761061, 0.759977, 0.758872, 0.756015, 0.75302, 0.749885, 0.746606, 0.743182, 0.739611, 0.735891, 0.732017, 0.727985, 0.723788, 0.719419, 0.714865, 0.710114, 0.705148, 0.699946, 0.694481, 0.688721, 0.682628, 0.676155, 0.669243, 0.661823, 0.653808, 0.645091, 0.635536, 0.624969, 0.613154, 0.599764, 0.584301, 0.577364, 0.569885, 0.561756, 0.552827, 0.542888, 0.531648, 0.518699, 0.50348, 0.485224, 0.462864, 0.434823, 0.398473, 0.375741, 0.348572, 0.314974, 0.294663, 0.270945, 0.2602, 0.248542, 0.235779, 0.221649, 0.205769, 0.187547, 0.16598, 0.153396, 0.139094, 0.122354, 0.112682, 0.101773, 0.0891181, 0.0863053, 0.0833752, 0.0803139, 0.0771045, 0.0737264, 0.0663517, 0.0578682, 0.053037, 0.0476481}, 
{1.04431, 1.0466, 1.05065, 1.04828, 1.04719, 1.04809, 1.04894, 1.04925, 1.04906, 1.04863, 1.04812,
	1.04862, 1.04806, 1.04843, 1.04808, 1.04824, 1.04803, 1.04799, 1.04788, 1.04781, 1.04768, 1.04762, 1.04752, 1.04729, 1.04704, 1.04657, 1.04609, 1.0456, 1.0446, 1.04357, 1.04251, 1.04141, 1.04028, 1.03911, 1.03791, 1.03667, 1.03539, 1.03406, 1.0327, 1.03129, 1.02756, 1.02353, 1.01918, 1.01448, 1.00943, 1.004, 0.9982, 0.992012, 0.985437, 0.978476, 0.971133, 0.963412, 0.955317, 0.946845, 0.937987, 0.928725, 0.919026, 0.908845, 0.898118, 0.886764, 0.874681, 0.861738, 0.847776, 0.832595, 0.815947, 0.797527, 0.776957, 0.7538, 0.74371, 0.733106, 0.721967, 0.710268, 0.697962, 0.684902, 0.670601, 0.65375, 0.632042, 0.603295, 0.56587, 0.517102, 0.486783, 0.450771, 0.406552, 0.379969, 0.349048, 0.33508, 0.319949, 0.303412, 0.285135, 0.264631, 0.241142, 0.213382, 0.197201, 0.178818, 0.157307, 0.144878, 0.130859, 0.114594, 0.110978, 0.107211, 0.103275, 0.099148, 0.0948041, 0.0853202, 0.0744092, 0.0681953, 0.0612642}, 
{1.32188, 1.31695, 1.31668, 1.31947, 1.31731, 1.3177, 1.31852, 1.31894, 1.31877, 1.31831, 1.31844,
	1.31807, 1.31822, 1.318, 1.3182, 1.318, 1.31792, 1.31791, 1.31782, 1.3177, 1.31764, 1.31754, 1.31745, 1.31723, 1.31701, 1.31656, 1.31612, 1.31566, 1.31474, 1.31378, 1.3128, 1.31179, 1.31075, 1.30967, 1.30856, 1.3074, 1.3062, 1.30495, 1.30366, 1.30231, 1.2987, 1.2947, 1.29025, 1.2853, 1.2798, 1.27368, 1.26687, 1.25931, 1.25091, 1.24163, 1.23143, 1.22032, 1.20835, 1.19561, 1.18221, 1.16825, 1.15377, 1.13878, 1.1232, 1.10691, 1.08976, 1.07153, 1.05194, 1.03067, 1.0073, 0.981265, 0.951825, 0.91795, 0.902851, 0.886706, 0.869389, 0.850781, 0.830803, 0.809534, 0.787531, 0.766572, 0.750871, 0.748723, 0.741744, 0.732374, 0.728044, 0.723765, 0.719565, 0.717492, 0.715438, 0.714621, 0.713807, 0.712996, 0.712188, 0.711383, 0.710581, 0.709781, 0.709382, 0.708984, 0.708587, 0.708388, 0.70819, 0.707992, 0.707952, 0.707911, 0.707871, 0.707832, 0.707796, 0.70772, 0.70762, 0.707592, 0.707598}, 
{1.58204, 1.58635, 1.58318, 1.58511, 1.58355, 1.58343, 1.58418, 1.58468, 1.58448, 1.58398, 1.58441, 
	1.58383, 1.58417, 1.5841, 1.58387, 1.58384, 1.58373, 1.58367, 1.58357, 1.58342, 1.58336, 1.58323, 1.58315, 1.58289, 1.58264, 1.58213, 1.58164, 1.58114, 1.58016, 1.57919, 1.57821, 1.57723, 1.57624, 1.57524, 1.57422, 1.57317, 1.5721, 1.57099, 1.56984, 1.56865, 1.56545, 1.56189, 1.55789, 1.55337, 1.54825, 1.54244, 1.53582, 1.52823, 1.51949, 1.50932, 1.49738, 1.48328, 1.46673, 1.44791, 1.42768, 1.40711, 1.38682, 1.36683, 1.34689, 1.32662, 1.30561, 1.28347, 1.25976, 1.234, 1.20558, 1.17376, 1.13753, 1.09541, 1.07645, 1.05602, 1.03387, 1.0097, 0.983084, 0.953463, 0.919986, 0.881204, 0.833927, 0.770337, 0.707988, 0.641106, 0.601525, 0.555418, 0.499613, 0.466355, 0.427874, 0.41055, 0.391822, 0.371395, 0.348864, 0.323638, 0.2948, 0.260789, 0.240992, 0.218519, 0.192238, 0.177057, 0.159933, 0.140065, 0.135647, 0.131044, 0.126235, 0.121193, 0.115884, 0.104293, 0.0909554, 0.0833586, 0.0748845}, 
{1.84957, 1.84968, 1.85134, 1.85007, 1.84983, 1.84929, 1.85004, 1.8506, 1.85027, 1.84968, 1.85008, 
	1.85008, 1.84964, 1.84961, 1.84963, 1.84966, 1.84941, 1.84923, 1.84906, 1.84884, 1.84864, 1.84851, 1.84829, 1.84788, 1.84746, 1.84662, 1.84583, 1.84506, 1.84361, 1.84225, 1.84097, 1.83975, 1.83859, 1.83747, 1.83638, 1.8353, 1.83424, 1.83317, 1.8321, 1.831, 1.82815, 1.82502, 1.82153, 1.81757, 1.81306, 1.8079, 1.80199, 1.79518, 1.78728, 1.77801, 1.76686, 1.75278, 1.73323, 1.70384, 1.66882, 1.63893, 1.61423, 1.59206, 1.57064, 1.54892, 1.52619, 1.5019, 1.47555, 1.44657, 1.41428, 1.37781, 1.33598, 1.28712, 1.26509, 1.24133, 1.21558, 1.18751, 1.15671, 1.12263, 1.08453, 1.04141, 0.991742, 0.933178, 0.861906, 0.773444, 0.721837, 0.663505, 0.594647, 0.554166, 0.507666, 0.486825, 0.464348, 0.439888, 0.412969, 0.382898, 0.348601, 0.308246, 0.284798, 0.25821, 0.227147, 0.209213, 0.188989, 0.165524, 0.160306, 0.15487, 0.149189, 0.143232, 0.136961, 0.123266, 0.107504, 0.113694, 0.102135}};

  /* 2, 1 mode */

  const double reomegaqnm21[8][107] = {{0.336609, 0.339386, 0.340852, 0.342219, 0.342818, 0.343002, 
  0.343143, 0.343254, 0.343344, 0.343417, 0.343641, 0.343748, 
  0.343804, 0.343853, 0.343871, 0.343879, 0.343886, 0.343891, 
  0.343896, 0.343902, 0.343909, 0.343915, 0.343922, 0.343941, 0.34396,
   0.344002, 0.344049, 0.344101, 0.34422, 0.344359, 0.344517, 
  0.344696, 0.344896, 0.345115, 0.345356, 0.345617, 0.345899, 
  0.346201, 0.346525, 0.34687, 0.347824, 0.348911, 0.350132, 0.351491,
   0.35299, 0.354633, 0.356423, 0.358366, 0.360469, 0.362738, 
  0.365183, 0.367812, 0.370637, 0.373672, 0.376931, 0.380432, 
  0.384197, 0.388248, 0.392615, 0.39733, 0.402436, 0.407979, 0.41402, 
  0.420632, 0.427909, 0.435968, 0.444968, 0.455121, 0.459569, 
  0.464271, 0.469259, 0.474564, 0.480231, 0.486308, 0.492859, 
  0.499965, 0.507729, 0.516291, 0.525845, 0.536673, 0.542693, 
  0.549213, 0.556329, 0.560146, 0.564155, 0.565814, 0.567505, 
  0.569227, 0.570976, 0.572749, 0.574535, 0.576322, 0.577208, 
  0.578084, 0.578948, 0.579374, 0.579795, 0.580212, 0.580295, 
  0.580377, 0.58046, 0.580541, 0.580623, 0.580784, 0.580942, 0.581018,
   0.581093}, {0.313486, 0.314984, 0.315748, 0.31636, 0.316504, 
  0.316512, 0.316502, 0.316483, 0.31646, 0.316436, 0.316347, 0.316311,
   0.316305, 0.316315, 0.316325, 0.31633, 0.316338, 0.316345, 
  0.316352, 0.31636, 0.316368, 0.316375, 0.316383, 0.316401, 0.316419,
   0.316455, 0.316491, 0.316527, 0.316602, 0.316682, 0.316772, 
  0.316874, 0.316989, 0.317122, 0.317273, 0.317444, 0.317636, 
  0.317851, 0.31809, 0.318353, 0.319124, 0.320064, 0.321179, 0.322476,
   0.32396, 0.325636, 0.327508, 0.329582, 0.331865, 0.334364, 
  0.337088, 0.340045, 0.343249, 0.346711, 0.350448, 0.354478, 
  0.358823, 0.363506, 0.368558, 0.374014, 0.379916, 0.386313, 
  0.393268, 0.400856, 0.409172, 0.418337, 0.428509, 0.439897, 
  0.444855, 0.450076, 0.455587, 0.46142, 0.467612, 0.474208, 0.481261,
   0.488834, 0.497004, 0.505862, 0.515506, 0.526011, 0.531572, 
  0.537263, 0.542874, 0.545479, 0.547723, 0.548438, 0.548984, 0.54929,
   0.549241, 0.548642, 0.54714, 0.544185, 0.542606, 0.542325, 
  0.541626, 0.540962, 0.540617, 0.540156, 0.540066, 0.539986, 
  0.539906, 0.539818, 0.539726, 0.539553, 0.539377, 0.539288, 
  0.539198}, {0.270281, 0.269419, 0.268655, 0.267935, 0.267923, 
  0.268009, 0.268102, 0.268185, 0.268251, 0.268297, 0.268334, 
  0.268285, 0.268273, 0.268312, 0.268342, 0.268359, 0.268398, 
  0.268442, 0.268483, 0.268523, 0.268562, 0.268601, 0.26864, 0.268733,
   0.268822, 0.268989, 0.269141, 0.26928, 0.26952, 0.269718, 0.269881,
   0.270016, 0.270132, 0.270235, 0.270333, 0.270431, 0.270536, 
  0.270653, 0.270785, 0.270937, 0.271426, 0.2721, 0.272992, 0.274121, 
  0.275504, 0.277153, 0.279078, 0.281288, 0.283792, 0.286598, 
  0.289716, 0.293156, 0.296931, 0.301053, 0.30554, 0.31041, 0.315684, 
  0.321389, 0.327554, 0.334218, 0.341421, 0.349217, 0.357668, 
  0.366852, 0.376864, 0.387825, 0.39989, 0.413261, 0.419032, 0.425075,
   0.431414, 0.438075, 0.445088, 0.452488, 0.46031, 0.468593, 
  0.477369, 0.48665, 0.49639, 0.506348, 0.511184, 0.515607, 0.51903, 
  0.519992, 0.5201, 0.51983, 0.519361, 0.51871, 0.517943, 0.517204, 
  0.516728, 0.516696, 0.516098, 0.513376, 0.509813, 0.508083, 
  0.506412, 0.504789, 0.504468, 0.504148, 0.50383, 0.503511, 0.503194,
   0.502559, 0.501923, 0.501605, 0.501286}, {0.21269, 0.214089, 
  0.215208, 0.215364, 0.21474, 0.214584, 0.21456, 0.214625, 0.21473, 
  0.214837, 0.214992, 0.214885, 0.214922, 0.215045, 0.215071, 
  0.215139, 0.215267, 0.215383, 0.215507, 0.215624, 0.215741, 
  0.215858, 0.215974, 0.216256, 0.216533, 0.217064, 0.217566, 0.21804,
   0.218903, 0.219653, 0.220295, 0.220836, 0.221285, 0.22165, 
  0.221945, 0.22218, 0.222368, 0.22252, 0.222647, 0.222759, 0.223032, 
  0.223383, 0.2239, 0.224647, 0.225672, 0.22701, 0.228689, 0.23073, 
  0.233151, 0.235969, 0.239199, 0.242854, 0.246951, 0.251505, 
  0.256534, 0.262058, 0.268101, 0.274689, 0.281852, 0.289628, 
  0.298059, 0.307197, 0.317104, 0.327855, 0.339542, 0.352278, 
  0.366207, 0.38151, 0.388065, 0.394895, 0.402017, 0.409454, 0.417227,
   0.425361, 0.433878, 0.442797, 0.452127, 0.461855, 0.471918, 
  0.482195, 0.48739, 0.49271, 0.498495, 0.501813, 0.5056, 0.507251, 
  0.508939, 0.510585, 0.512031, 0.513028, 0.513289, 0.512758, 
  0.512367, 0.512091, 0.511921, 0.51135, 0.5096, 0.50715, 0.50665, 
  0.506154, 0.505662, 0.505175, 0.504694, 0.503744, 0.502809, 
  0.502344, 0.50188}, {0.171239, 0.169224, 0.168634, 0.170053, 
  0.169938, 0.169613, 0.169418, 0.169408, 0.169531, 0.169696, 
  0.169825, 0.169724, 0.16992, 0.169916, 0.170075, 0.17014, 0.170344, 
  0.170511, 0.17071, 0.170886, 0.171073, 0.171258, 0.171437, 0.171892,
   0.172337, 0.173213, 0.174064, 0.174889, 0.176453, 0.177895, 
  0.179203, 0.180372, 0.181395, 0.182273, 0.183007, 0.183606, 
  0.184081, 0.184444, 0.18471, 0.184896, 0.185105, 0.18512, 0.185113, 
  0.185222, 0.18555, 0.186176, 0.187164, 0.188562, 0.190413, 0.192751,
   0.195604, 0.199, 0.202962, 0.207515, 0.212679, 0.21848, 0.224939, 
  0.232083, 0.23994, 0.248543, 0.257928, 0.268139, 0.279228, 0.291258,
   0.304303, 0.318456, 0.33383, 0.350563, 0.357675, 0.365045, 
  0.372687, 0.380617, 0.388855, 0.397422, 0.406346, 0.41567, 0.425462,
   0.435857, 0.447149, 0.45999, 0.467349, 0.475546, 0.484594, 
  0.489367, 0.494295, 0.496339, 0.498459, 0.500692, 0.50308, 0.505635,
   0.508214, 0.510197, 0.510572, 0.510387, 0.509864, 0.509635, 
  0.509456, 0.50879, 0.508443, 0.507999, 0.50747, 0.506881, 0.506257, 
  0.504974, 0.503704, 0.503082, 0.502466}, {0.136159, 0.137914, 
  0.137016, 0.136626, 0.137329, 0.137055, 0.136755, 0.136667, 
  0.136793, 0.136997, 0.136954, 0.137117, 0.137154, 0.137279, 0.1373, 
  0.13747, 0.137649, 0.137842, 0.138069, 0.138258, 0.138476, 0.138671,
   0.138881, 0.139389, 0.139898, 0.14091, 0.14191, 0.142898, 0.144829,
   0.146684, 0.148447, 0.150099, 0.151622, 0.153001, 0.154221, 
  0.15527, 0.156142, 0.156839, 0.157368, 0.15774, 0.158094, 0.157845, 
  0.15725, 0.156526, 0.155847, 0.155354, 0.155162, 0.155369, 0.156061,
   0.157316, 0.159206, 0.161795, 0.165143, 0.169299, 0.174306, 
  0.180192, 0.186977, 0.194671, 0.203282, 0.212811, 0.223263, 
  0.234649, 0.246985, 0.260297, 0.274622, 0.290009, 0.306518, 
  0.324226, 0.331666, 0.339322, 0.347205, 0.355332, 0.363727, 
  0.372432, 0.381518, 0.391111, 0.401435, 0.412868, 0.425995, 
  0.441563, 0.45052, 0.460463, 0.471717, 0.477974, 0.484665, 0.487438,
   0.490248, 0.493085, 0.49596, 0.498923, 0.502084, 0.505527, 
  0.507222, 0.508526, 0.508875, 0.508627, 0.508251, 0.507926, 
  0.507863, 0.507785, 0.50767, 0.507484, 0.507181, 0.506113, 0.504624,
   0.503839, 0.50306}, {0.114342, 0.113963, 0.115141, 0.114074, 
  0.11486, 0.114789, 0.114485, 0.114338, 0.114452, 0.114665, 0.114512,
   0.114807, 0.11467, 0.114821, 0.114995, 0.115025, 0.115231, 
  0.115438, 0.115661, 0.115846, 0.11605, 0.116249, 0.116452, 0.116951,
   0.117454, 0.118454, 0.11946, 0.120464, 0.122464, 0.124443, 
  0.126383, 0.128266, 0.130073, 0.131781, 0.133368, 0.134813, 0.13609,
   0.137179, 0.138065, 0.138742, 0.139567, 0.139335, 0.138333, 
  0.136834, 0.135064, 0.133208, 0.131418, 0.129826, 0.128564, 
  0.127775, 0.127629, 0.128336, 0.130133, 0.133252, 0.137863, 
  0.144021, 0.151672, 0.160684, 0.170902, 0.182186, 0.194429, 
  0.207556, 0.221528, 0.236327, 0.251955, 0.268421, 0.285744, 0.30395,
   0.311492, 0.319195, 0.327076, 0.335168, 0.343522, 0.352232, 
  0.361443, 0.371382, 0.382361, 0.394771, 0.409089, 0.426092, 
  0.436065, 0.4474, 0.460415, 0.467675, 0.475563, 0.478922, 0.482402, 
  0.485993, 0.489665, 0.493377, 0.497113, 0.500995, 0.503086, 
  0.505273, 0.507186, 0.507677, 0.507628, 0.507177, 0.507077, 
  0.506981, 0.506892, 0.506806, 0.506714, 0.506383, 0.505391, 
  0.504576, 0.503665}, {0.0996362, 0.098809, 0.0988228, 0.0989612, 
  0.0991812, 0.0993225, 0.0990912, 0.0989133, 0.0990066, 0.0992056, 
  0.0990844, 0.0992013, 0.0993046, 0.0993698, 0.0994135, 0.0995406, 
  0.0997102, 0.0999057, 0.100109, 0.100292, 0.100464, 0.100668, 
  0.100843, 0.10132, 0.101793, 0.102742, 0.103705, 0.104671, 0.106617,
   0.108576, 0.11054, 0.112492, 0.114417, 0.116296, 0.118106, 
  0.119822, 0.121422, 0.122876, 0.12415, 0.125214, 0.12686, 0.127085, 
  0.126123, 0.124264, 0.121765, 0.118824, 0.115576, 0.112102, 0.10844,
   0.104598, 0.100602, 0.0966436, 0.0934548, 0.0928224, 0.0967608, 
  0.105267, 0.116748, 0.129817, 0.143719, 0.158124, 0.17292, 0.188094,
   0.203673, 0.219697, 0.236201, 0.253208, 0.270727, 0.288784, 
  0.296181, 0.303707, 0.311404, 0.319342, 0.327628, 0.336416, 
  0.345907, 0.35633, 0.367923, 0.380968, 0.395965, 0.413866, 0.424377,
   0.436366, 0.450442, 0.458506, 0.467375, 0.471171, 0.475126, 
  0.479254, 0.483568, 0.488052, 0.492639, 0.497232, 0.499557, 
  0.501993, 0.504598, 0.505838, 0.506697, 0.506742, 0.506649, 
  0.506536, 0.50641, 0.506281, 0.506156, 0.505924, 0.505541, 0.505047,
   0.504219}};

  const double imomegaqnm21[8][107] = {{0.0743116, 0.0772659, 0.0791824, 0.0812815, 0.0822652, 0.082556, 
  0.0827684, 0.082926, 0.0830445, 0.0831347, 0.0833596, 0.0834304, 
  0.0834555, 0.0834714, 0.0834806, 0.0834917, 0.0835195, 0.0835507, 
  0.0835831, 0.0836156, 0.0836481, 0.0836804, 0.0837126, 0.083792, 
  0.0838704, 0.0840241, 0.0841737, 0.0843193, 0.0845994, 0.084865, 
  0.085117, 0.0853561, 0.0855831, 0.0857987, 0.0860035, 0.086198, 
  0.0863828, 0.0865584, 0.0867253, 0.086884, 0.087247, 0.0875662, 
  0.0878462, 0.0880906, 0.0883025, 0.088484, 0.088637, 0.0887628, 
  0.0888621, 0.0889354, 0.0889828, 0.0890037, 0.0889973, 0.0889623, 
  0.0888968, 0.0887983, 0.0886636, 0.0884885, 0.0882679, 0.0879952, 
  0.0876618, 0.0872571, 0.086767, 0.086173, 0.0854501, 0.0845642, 
  0.0834665, 0.0820852, 0.0814304, 0.0807035, 0.0798926, 0.0789827, 
  0.077955, 0.0767847, 0.0754394, 0.0738744, 0.072027, 0.0698043, 
  0.067061, 0.0635492, 0.0613721, 0.058791, 0.0556436, 0.0537759, 
  0.0516427, 0.0506978, 0.0496908, 0.0486136, 0.0474565, 0.0462084, 
  0.0448564, 0.0433879, 0.0426068, 0.0417939, 0.0409501, 0.0405173, 
  0.0400777, 0.0396319, 0.039542, 0.0394519, 0.0393616, 0.039271, 
  0.0391802, 0.0389974, 0.0388111, 0.038714, 0.0386091}, {0.258785, 
  0.258526, 0.258215, 0.257703, 0.257385, 0.257284, 0.25721, 0.257158,
   0.257123, 0.257101, 0.257089, 0.257133, 0.257178, 0.257248, 
  0.257304, 0.257357, 0.257465, 0.257574, 0.257683, 0.257791, 
  0.257898, 0.258005, 0.258112, 0.258376, 0.258637, 0.259149, 
  0.259649, 0.260137, 0.261076, 0.261967, 0.262812, 0.263613, 
  0.264372, 0.265089, 0.265768, 0.26641, 0.267016, 0.267589, 0.26813, 
  0.26864, 0.269792, 0.270782, 0.271629, 0.272346, 0.272945, 0.273434,
   0.273821, 0.274112, 0.274309, 0.274415, 0.27443, 0.274354, 
  0.274183, 0.273915, 0.273543, 0.273058, 0.272452, 0.271711, 
  0.270819, 0.269754, 0.268489, 0.266992, 0.265218, 0.263108, 
  0.260587, 0.257547, 0.253838, 0.249238, 0.247078, 0.244692, 
  0.242045, 0.239089, 0.235766, 0.231999, 0.227686, 0.222686, 0.2168, 
  0.209731, 0.201006, 0.189803, 0.18282, 0.174484, 0.164205, 0.158029,
   0.150891, 0.1477, 0.14428, 0.140606, 0.136657, 0.132444, 0.128112, 
  0.124511, 0.123925, 0.123321, 0.12167, 0.121162, 0.120732, 0.12014, 
  0.120048, 0.119951, 0.119844, 0.119734, 0.119631, 0.119429, 
  0.119227, 0.119127, 0.119027}, {0.451082, 0.450126, 0.449946, 
  0.450393, 0.450836, 0.450954, 0.451016, 0.451037, 0.451032, 
  0.451015, 0.450927, 0.450953, 0.451028, 0.451151, 0.451242, 
  0.451336, 0.451538, 0.451736, 0.451932, 0.452129, 0.452326, 
  0.452521, 0.452716, 0.453201, 0.453683, 0.454634, 0.45557, 0.456489,
   0.458275, 0.459989, 0.461626, 0.463185, 0.464665, 0.466067, 
  0.46739, 0.468636, 0.469808, 0.470906, 0.471935, 0.472897, 0.475022,
   0.476783, 0.478216, 0.479352, 0.48022, 0.48084, 0.481231, 0.481407,
   0.481377, 0.481149, 0.480726, 0.480109, 0.479294, 0.478277, 
  0.477047, 0.475592, 0.473893, 0.471929, 0.469669, 0.467076, 
  0.464104, 0.460691, 0.456761, 0.45221, 0.446903, 0.440653, 0.433196,
   0.424144, 0.419951, 0.415358, 0.4103, 0.404693, 0.398434, 0.391383,
   0.383355, 0.374094, 0.363227, 0.350188, 0.334044, 0.313093, 
  0.299808, 0.283583, 0.262719, 0.249472, 0.233119, 0.225306, 
  0.216483, 0.206365, 0.194563, 0.180538, 0.16345, 0.141483, 0.12731, 
  0.111596, 0.0954876, 0.0868316, 0.0774193, 0.066857, 0.0645541, 
  0.0621712, 0.0596988, 0.057125, 0.0544355, 0.0486318, 0.0420655, 
  0.0383765, 0.0343031}, {0.670736, 0.672092, 0.671812, 0.670663, 
  0.670549, 0.67073, 0.670927, 0.671078, 0.671164, 0.671189, 0.671021,
   0.671075, 0.671219, 0.67131, 0.671427, 0.671572, 0.671807, 
  0.672062, 0.672309, 0.672557, 0.672808, 0.673057, 0.673305, 
  0.673929, 0.674552, 0.675799, 0.677043, 0.678283, 0.680743, 
  0.683166, 0.685536, 0.68784, 0.690067, 0.692205, 0.694247, 0.696187,
   0.69802, 0.699744, 0.701359, 0.702865, 0.706165, 0.708833, 
  0.710915, 0.71246, 0.71351, 0.714105, 0.714277, 0.714053, 0.713452, 
  0.71249, 0.711176, 0.709516, 0.707509, 0.705148, 0.702423, 0.699317,
   0.695803, 0.691851, 0.687416, 0.682445, 0.676867, 0.670592, 
  0.663504, 0.655451, 0.646227, 0.635554, 0.623036, 0.60809, 0.601244,
   0.593791, 0.58563, 0.576638, 0.566651, 0.555455, 0.54276, 0.528156,
   0.511038, 0.490465, 0.464847, 0.431189, 0.409563, 0.382936, 
  0.348824, 0.32773, 0.302947, 0.291759, 0.279694, 0.26659, 0.252176, 
  0.235931, 0.216835, 0.192946, 0.178198, 0.160681, 0.138951, 
  0.125667, 0.110911, 0.095128, 0.0917732, 0.0883219, 0.0847576, 
  0.0810612, 0.0772103, 0.0689279, 0.0595841, 0.0543422, 
  0.048559}, {0.910592, 0.910063, 0.91138, 0.911832, 0.911012, 
  0.911003, 0.911199, 0.911439, 0.911606, 0.91166, 0.911352, 0.911585,
   0.911648, 0.911704, 0.911854, 0.911933, 0.912192, 0.912425, 
  0.912663, 0.912897, 0.913141, 0.913376, 0.913616, 0.91422, 0.91483, 
  0.916061, 0.917309, 0.918574, 0.921148, 0.923769, 0.92642, 0.929083,
   0.931738, 0.934365, 0.936941, 0.939446, 0.941862, 0.944173, 
  0.946367, 0.948436, 0.953025, 0.956761, 0.959656, 0.961747, 0.96308,
   0.963701, 0.963651, 0.962967, 0.96168, 0.959815, 0.957389, 
  0.954417, 0.950902, 0.946845, 0.942236, 0.937057, 0.931283, 
  0.924873, 0.917775, 0.909918, 0.901208, 0.891524, 0.880707, 
  0.868545, 0.854753, 0.838939, 0.820547, 0.798756, 0.788822, 
  0.778034, 0.76625, 0.75329, 0.738922, 0.722836, 0.704612, 0.683652, 
  0.659084, 0.629562, 0.592913, 0.54542, 0.515651, 0.480063, 0.436051,
   0.409334, 0.377864, 0.363486, 0.347803, 0.330563, 0.311454, 
  0.290066, 0.265802, 0.237412, 0.220683, 0.201065, 0.17693, 0.162373,
   0.145371, 0.124789, 0.120176, 0.115422, 0.110541, 0.105527, 
  0.100358, 0.0893955, 0.0771778, 0.0703585, 0.0628494}, {1.16313, 
  1.16205, 1.16102, 1.16236, 1.1618, 1.16155, 1.16165, 1.16191, 
  1.16212, 1.16217, 1.16185, 1.16214, 1.162, 1.16222, 1.16228, 1.1624,
   1.16261, 1.16279, 1.163, 1.16321, 1.16342, 1.16362, 1.16384, 
  1.16436, 1.16489, 1.16598, 1.1671, 1.16824, 1.1706, 1.17306, 
  1.17562, 1.17826, 1.18098, 1.18376, 1.18658, 1.18941, 1.19222, 
  1.19499, 1.19769, 1.20029, 1.20624, 1.21127, 1.21528, 1.21824, 
  1.22017, 1.2211, 1.22105, 1.22006, 1.21814, 1.21533, 1.21164, 
  1.20711, 1.20176, 1.19561, 1.18868, 1.18098, 1.17251, 1.16324, 
  1.15314, 1.14213, 1.13009, 1.11686, 1.10224, 1.08592, 1.06753, 
  1.04652, 1.02214, 0.993243, 0.980058, 0.965724, 0.950047, 0.932779, 
  0.913601, 0.892095, 0.867699, 0.839644, 0.806856, 0.767797, 
  0.720146, 0.659944, 0.622848, 0.578732, 0.524237, 0.491317, 
  0.452927, 0.435542, 0.416661, 0.395943, 0.372912, 0.346898, 
  0.316949, 0.28167, 0.261342, 0.238436, 0.211307, 0.195088, 0.176156,
   0.153337, 0.148132, 0.142654, 0.136869, 0.130747, 0.124273, 
  0.110319, 0.0949385, 0.0864639, 0.0771851}, {1.41428, 1.41604, 
  1.4156, 1.41536, 1.4156, 1.41524, 1.41519, 1.41543, 1.41565, 
  1.41568, 1.41551, 1.41554, 1.41563, 1.41566, 1.41579, 1.41591, 
  1.41608, 1.41624, 1.41643, 1.41663, 1.4168, 1.417, 1.41717, 1.41765,
   1.41813, 1.41911, 1.42011, 1.42114, 1.42328, 1.42554, 1.42792, 
  1.43041, 1.43303, 1.43576, 1.4386, 1.44152, 1.44453, 1.44758, 
  1.45065, 1.45369, 1.46099, 1.46752, 1.473, 1.47733, 1.48042, 
  1.48226, 1.48282, 1.48207, 1.47997, 1.47646, 1.4715, 1.46505, 
  1.45715, 1.44791, 1.43752, 1.42619, 1.41412, 1.4014, 1.38801, 
  1.37384, 1.35872, 1.34237, 1.32445, 1.30456, 1.28213, 1.25643, 
  1.22645, 1.19065, 1.17422, 1.15628, 1.13659, 1.11481, 1.09053, 
  1.06323, 1.03225, 0.996712, 0.955487, 0.906959, 0.848541, 0.775494, 
  0.730792, 0.678016, 0.613349, 0.574379, 0.528885, 0.508284, 
  0.485946, 0.461517, 0.434493, 0.404108, 0.369135, 0.327547, 
  0.303278, 0.275875, 0.244209, 0.225953, 0.204982, 0.179727, 
  0.173965, 0.167908, 0.161519, 0.154752, 0.147552, 0.131555, 
  0.113035, 0.102763, 0.09161}, {1.6695, 1.6684, 1.66944, 1.66866, 
  1.66927, 1.66897, 1.66882, 1.66899, 1.6692, 1.66921, 1.66919, 
  1.66906, 1.66925, 1.66933, 1.66938, 1.66941, 1.6696, 1.66977, 
  1.66995, 1.67014, 1.67031, 1.67048, 1.67066, 1.6711, 1.67155, 
  1.67246, 1.6734, 1.67436, 1.67635, 1.67846, 1.68069, 1.68303, 
  1.68552, 1.68814, 1.69089, 1.69379, 1.69681, 1.69996, 1.70322, 
  1.70655, 1.71494, 1.72293, 1.73008, 1.73612, 1.7409, 1.74437, 
  1.74645, 1.74707, 1.74612, 1.74337, 1.73845, 1.73073, 1.71933, 
  1.70384, 1.68568, 1.6674, 1.65022, 1.63402, 1.61822, 1.60219, 
  1.58534, 1.56716, 1.54712, 1.52462, 1.49894, 1.46917, 1.43397, 
  1.39142, 1.37171, 1.35009, 1.32623, 1.29974, 1.27014, 1.23683, 
  1.19912, 1.15609, 1.10649, 1.04841, 0.97878, 0.892317, 0.839828, 
  0.778168, 0.702974, 0.657884, 0.605411, 0.581667, 0.555906, 
  0.527711, 0.496517, 0.46151, 0.421398, 0.373867, 0.346042, 0.314367,
   0.277467, 0.25637, 0.232725, 0.204918, 0.198597, 0.191947, 
  0.184925, 0.177485, 0.169568, 0.151985, 0.131247, 0.119312, 
  0.106197}};

  /* 3, 3 mode */
  const double reomegaqnm33[8][107] = {{0.445768, 0.452799, 0.456948, 0.460943, 0.462462, 0.462842, 
  0.463095, 0.463269, 0.463394, 0.463488, 0.463746, 0.463886, 
  0.463989, 0.464144, 0.464267, 0.464374, 0.464572, 0.464763, 
  0.464952, 0.46514, 0.465329, 0.465518, 0.465707, 0.466182, 0.466657,
   0.467612, 0.468573, 0.46954, 0.471491, 0.473465, 0.475464, 
  0.477487, 0.479535, 0.481609, 0.483709, 0.485837, 0.487991, 
  0.490174, 0.492386, 0.494627, 0.500363, 0.5063, 0.512449, 0.518826, 
  0.525445, 0.532323, 0.539479, 0.546934, 0.55471, 0.562834, 0.571335,
   0.580244, 0.5896, 0.599443, 0.609823, 0.620796, 0.632425, 0.644787,
   0.657972, 0.672086, 0.68726, 0.70365, 0.721455, 0.740921, 0.762369,
   0.786223, 0.813057, 0.843687, 0.857254, 0.871717, 0.887201, 
  0.90386, 0.921885, 0.941521, 0.963088, 0.987016, 1.01391, 1.04464, 
  1.08058, 1.1241, 1.14998, 1.17986, 1.21547, 1.23637, 1.26023, 
  1.27086, 1.28227, 1.29462, 1.30812, 1.32308, 1.33999, 1.35965, 
  1.37094, 1.38363, 1.39829, 1.40666, 1.41603, 1.42679, 1.42917, 
  1.43164, 1.43422, 1.43692, 1.43975, 1.44591, 1.45295, 1.45695, 
  1.46139}, {0.428357, 0.432238, 0.434351, 0.436362, 0.437184, 
  0.437404, 0.437552, 0.437654, 0.437725, 0.437773, 0.437868, 
  0.437898, 0.437929, 0.438015, 0.438115, 0.438217, 0.438421, 
  0.438623, 0.438826, 0.43903, 0.439233, 0.439437, 0.439641, 0.440152,
   0.440665, 0.441694, 0.442729, 0.443771, 0.445872, 0.447999, 
  0.45015, 0.452328, 0.454532, 0.456764, 0.459023, 0.461311, 0.463627,
   0.465974, 0.46835, 0.470758, 0.476918, 0.483289, 0.489884, 
  0.496717, 0.503805, 0.511164, 0.518814, 0.526776, 0.535074, 
  0.543734, 0.552785, 0.562261, 0.5722, 0.582644, 0.593642, 0.60525, 
  0.617535, 0.630573, 0.644453, 0.659285, 0.675198, 0.692352, 
  0.710943, 0.731221, 0.753507, 0.778225, 0.805952, 0.837504, 
  0.851449, 0.866295, 0.882167, 0.89922, 0.917645, 0.937687, 0.959667,
   0.984015, 1.01133, 1.0425, 1.07889, 1.12285, 1.14897, 1.17907, 
  1.21491, 1.23591, 1.25988, 1.27055, 1.28201, 1.2944, 1.30794, 
  1.32294, 1.33989, 1.35958, 1.37089, 1.38359, 1.39826, 1.40664, 
  1.41601, 1.42678, 1.42916, 1.43163, 1.43421, 1.43691, 1.43974, 
  1.4459, 1.45295, 1.45695, 1.46139}, {0.390162, 0.390933, 0.390855, 
  0.39013, 0.389593, 0.389452, 0.389377, 0.389349, 0.389352, 0.389374,
   0.389533, 0.389627, 0.38968, 0.38978, 0.389896, 0.390013, 0.390243,
   0.390473, 0.390704, 0.390935, 0.391167, 0.391398, 0.39163, 
  0.392211, 0.392793, 0.393963, 0.395139, 0.396321, 0.398706, 
  0.401118, 0.403557, 0.406025, 0.408522, 0.411048, 0.413604, 
  0.416191, 0.41881, 0.42146, 0.424144, 0.426861, 0.433807, 0.44098, 
  0.448397, 0.456071, 0.464019, 0.47226, 0.480815, 0.489704, 0.498954,
   0.50859, 0.518645, 0.529153, 0.540151, 0.551685, 0.563804, 
  0.576567, 0.590041, 0.604302, 0.619445, 0.635578, 0.652833, 
  0.671371, 0.691391, 0.713145, 0.736956, 0.763249, 0.792606, 
  0.825846, 0.840483, 0.856032, 0.872619, 0.890397, 0.90956, 0.930355,
   0.953102, 0.978234, 1.00636, 1.03835, 1.07558, 1.12042, 1.14697, 
  1.17752, 1.21379, 1.23502, 1.2592, 1.26995, 1.28149, 1.29396, 
  1.30758, 1.32266, 1.33967, 1.35943, 1.37078, 1.38351, 1.3982, 
  1.4066, 1.41598, 1.42676, 1.42914, 1.43162, 1.4342, 1.4369, 1.43973,
   1.44589, 1.45294, 1.45694, 1.46139}, {0.332321, 0.329612, 0.329447,
   0.330998, 0.331533, 0.331485, 0.331367, 0.331244, 0.331151, 
  0.331101, 0.331226, 0.331354, 0.331387, 0.331504, 0.331643, 
  0.331766, 0.332024, 0.33228, 0.332537, 0.332795, 0.333053, 0.333311,
   0.333569, 0.334216, 0.334865, 0.336168, 0.337478, 0.338796, 
  0.341454, 0.344143, 0.346863, 0.349614, 0.352398, 0.355215, 
  0.358066, 0.360951, 0.363872, 0.366829, 0.369823, 0.372855, 
  0.380605, 0.388612, 0.39689, 0.405455, 0.414327, 0.423525, 0.43307, 
  0.442986, 0.4533, 0.464039, 0.475237, 0.486928, 0.499155, 0.511962, 
  0.525402, 0.539534, 0.554428, 0.570163, 0.586836, 0.604556, 0.62346,
   0.643711, 0.665511, 0.689115, 0.71485, 0.743145, 0.774586, 
  0.809998, 0.82553, 0.841991, 0.859506, 0.878231, 0.898362, 0.920146,
   0.943908, 0.970086, 0.999286, 1.0324, 1.0708, 1.11686, 1.14404, 
  1.17523, 1.21215, 1.23369, 1.25818, 1.26906, 1.28072, 1.2933, 
  1.30704, 1.32223, 1.33936, 1.35922, 1.37061, 1.38339, 1.39812, 
  1.40653, 1.41593, 1.42673, 1.42911, 1.43159, 1.43418, 1.43688, 
  1.43971, 1.44588, 1.45294, 1.45694, 1.46138}, {0.279748, 0.281425, 
  0.280003, 0.278496, 0.279479, 0.279701, 0.279679, 0.279534, 0.27938,
   0.279284, 0.279511, 0.279556, 0.279569, 0.279745, 0.279852, 
  0.279994, 0.280246, 0.280507, 0.280766, 0.281026, 0.281285, 
  0.281546, 0.281806, 0.282459, 0.283115, 0.284431, 0.285757, 
  0.287091, 0.289787, 0.292519, 0.295288, 0.298095, 0.30094, 0.303825,
   0.306751, 0.309717, 0.312725, 0.315776, 0.318871, 0.322011, 
  0.330062, 0.338415, 0.347087, 0.356096, 0.365463, 0.37521, 0.385359,
   0.395936, 0.406968, 0.418488, 0.430528, 0.443126, 0.456326, 
  0.470174, 0.484725, 0.500042, 0.516195, 0.533269, 0.551359, 
  0.570583, 0.591077, 0.61301, 0.636588, 0.662069, 0.689786, 0.720173,
   0.753822, 0.791564, 0.808064, 0.825515, 0.844044, 0.863808, 
  0.885005, 0.907885, 0.932779, 0.960131, 0.990559, 1.02497, 1.06475, 
  1.11229, 1.14026, 1.17226, 1.21, 1.23195, 1.25686, 1.26789, 1.2797, 
  1.29245, 1.30633, 1.32167, 1.33893, 1.35893, 1.37038, 1.38322, 
  1.39801, 1.40645, 1.41587, 1.42669, 1.42908, 1.43156, 1.43415, 
  1.43685, 1.43969, 1.44587, 1.45293, 1.45693, 1.46137}, {0.239992, 
  0.240995, 0.242946, 0.24156, 0.241854, 0.242213, 0.242269, 0.242117,
   0.241939, 0.241855, 0.242174, 0.24208, 0.242226, 0.242298, 
  0.242438, 0.242546, 0.242783, 0.243024, 0.243261, 0.243498, 
  0.243736, 0.243974, 0.244213, 0.244811, 0.245412, 0.246621, 0.24784,
   0.249069, 0.251558, 0.254089, 0.256662, 0.259279, 0.26194, 
  0.264647, 0.2674, 0.2702, 0.273049, 0.275948, 0.278897, 0.281899, 
  0.289636, 0.297725, 0.306186, 0.315041, 0.324312, 0.334027, 0.34421,
   0.354891, 0.366101, 0.377873, 0.390245, 0.403257, 0.416954, 
  0.431386, 0.446612, 0.462696, 0.479713, 0.49775, 0.516906, 0.537303,
   0.559082, 0.582416, 0.607517, 0.634649, 0.66415, 0.696462, 
  0.732182, 0.77215, 0.789586, 0.807999, 0.827516, 0.848298, 0.870543,
   0.894505, 0.920521, 0.949042, 0.980704, 1.01643, 1.05767, 1.10684, 
  1.1357, 1.16864, 1.20737, 1.22983, 1.25522, 1.26646, 1.27846, 
  1.29139, 1.30546, 1.32098, 1.33842, 1.35857, 1.3701, 1.38301, 
  1.39787, 1.40634, 1.41579, 1.42664, 1.42903, 1.43152, 1.43411, 
  1.43682, 1.43966, 1.44585, 1.45291, 1.45692, 1.46137}, {0.21884, 
  0.216298, 0.216633, 0.217244, 0.216888, 0.217233, 0.217329, 
  0.217189, 0.217034, 0.217006, 0.217217, 0.217255, 0.217293, 
  0.217414, 0.217505, 0.217599, 0.217807, 0.218016, 0.218223, 
  0.218427, 0.218635, 0.218841, 0.21905, 0.21957, 0.220094, 0.221148, 
  0.222212, 0.223288, 0.22547, 0.227696, 0.229966, 0.232282, 0.234646,
   0.237057, 0.239517, 0.242028, 0.244591, 0.247207, 0.249877, 
  0.252604, 0.259674, 0.267128, 0.274991, 0.283291, 0.292057, 
  0.301319, 0.31111, 0.321465, 0.332419, 0.344012, 0.356285, 0.369284,
   0.383057, 0.39766, 0.413152, 0.429604, 0.447094, 0.465712, 
  0.485563, 0.506772, 0.529486, 0.553883, 0.580183, 0.608655, 
  0.639647, 0.673609, 0.711146, 0.753105, 0.771386, 0.790674, 
  0.811095, 0.832808, 0.856013, 0.880965, 0.908002, 0.937583, 
  0.970359, 1.00729, 1.04987, 1.10064, 1.13044, 1.16442, 1.20427, 
  1.22731, 1.2533, 1.26476, 1.27699, 1.29015, 1.30444, 1.32016, 
  1.3378, 1.35815, 1.36977, 1.38276, 1.3977, 1.40621, 1.4157, 1.42658,
   1.42898, 1.43147, 1.43407, 1.43678, 1.43963, 1.44582, 1.4529, 
  1.45691, 1.46135}, {0.199814, 0.201353, 0.200132, 0.200956, 
  0.200467, 0.200722, 0.200829, 0.200716, 0.200604, 0.200626, 
  0.200705, 0.200826, 0.200822, 0.200914, 0.201004, 0.201103, 
  0.201273, 0.201448, 0.201625, 0.2018, 0.201975, 0.202153, 0.202329, 
  0.202773, 0.20322, 0.204121, 0.205032, 0.205953, 0.207826, 0.209742,
   0.211702, 0.213706, 0.215757, 0.217854, 0.220001, 0.222198, 
  0.224447, 0.226749, 0.229105, 0.231518, 0.237809, 0.244492, 
  0.251597, 0.25916, 0.267216, 0.275804, 0.284965, 0.294742, 0.305178,
   0.316322, 0.328221, 0.340928, 0.354499, 0.368992, 0.384475, 
  0.401019, 0.418708, 0.437635, 0.45791, 0.479661, 0.50304, 0.528232, 
  0.555461, 0.585008, 0.617226, 0.652577, 0.691676, 0.735379, 
  0.754411, 0.774477, 0.795706, 0.818253, 0.842317, 0.868149, 
  0.896082, 0.926573, 0.960272, 0.998162, 1.04182, 1.09394, 1.1246, 
  1.15963, 1.20071, 1.22441, 1.25107, 1.2628, 1.2753, 1.28872, 
  1.30326, 1.31923, 1.3371, 1.35766, 1.36938, 1.38248, 1.39751, 
  1.40606, 1.41559, 1.4265, 1.42891, 1.43141, 1.43401, 1.43673, 
  1.43959, 1.44579, 1.45288, 1.45689, 1.46134}};

  const double imomegaqnm33[8][107] = {{0.068612, 0.0735351, 0.0774984, 0.0829193, 0.0860369, 0.0870572, 
  0.0878377, 0.0884398, 0.0889085, 0.0892767, 0.0902685, 0.0906386, 
  0.0907974, 0.0909101, 0.0909414, 0.090952, 0.0909603, 0.0909664, 
  0.0909729, 0.0909798, 0.090987, 0.0909942, 0.0910015, 0.0910197, 
  0.0910378, 0.091074, 0.09111, 0.0911457, 0.0912166, 0.0912867, 
  0.091356, 0.0914243, 0.0914917, 0.0915582, 0.0916236, 0.091688, 
  0.0917514, 0.0918136, 0.0918746, 0.0919344, 0.0920784, 0.0922137, 
  0.0923394, 0.0924547, 0.0925583, 0.0926492, 0.0927258, 0.0927867, 
  0.0928302, 0.0928541, 0.0928562, 0.0928338, 0.092784, 0.092703, 
  0.0925869, 0.0924305, 0.0922281, 0.0919726, 0.0916556, 0.0912666, 
  0.0907928, 0.0902179, 0.0895213, 0.0886763, 0.087647, 0.0863849, 
  0.0848213, 0.0828557, 0.0819248, 0.0808922, 0.0797413, 0.0784512, 
  0.0769953, 0.075339, 0.0734361, 0.0712234, 0.0686106, 0.0654629, 
  0.0615646, 0.056537, 0.0533881, 0.0496087, 0.0449046, 0.0420439, 
  0.0386881, 0.0371631, 0.0355053, 0.0336873, 0.0316714, 0.0294027, 
  0.0267968, 0.023711, 0.0219107, 0.0198652, 0.0174725, 0.0160908, 
  0.014533, 0.0127266, 0.0123252, 0.011907, 0.0114701, 0.011012, 
  0.0105299, 0.00947732, 0.00826624, 0.0075764, 
  0.00680679}, {0.277496, 0.278402, 0.278809, 0.278955, 0.278824, 
  0.278742, 0.278664, 0.278594, 0.278534, 0.278482, 0.278334, 
  0.278289, 0.278281, 0.278294, 0.278307, 0.278318, 0.278336, 
  0.278354, 0.278373, 0.278391, 0.278409, 0.278427, 0.278446, 
  0.278491, 0.278537, 0.278627, 0.278716, 0.278805, 0.27898, 0.279153,
   0.279323, 0.279489, 0.279653, 0.279813, 0.27997, 0.280123, 
  0.280273, 0.280419, 0.280561, 0.280699, 0.281026, 0.281324, 
  0.281591, 0.281825, 0.28202, 0.282175, 0.282285, 0.282345, 0.28235, 
  0.282293, 0.282168, 0.281967, 0.281681, 0.281298, 0.280807, 
  0.280191, 0.279434, 0.278515, 0.277407, 0.27608, 0.274494, 0.272602,
   0.27034, 0.26763, 0.264363, 0.260394, 0.255518, 0.249435, 0.246568,
   0.243397, 0.239871, 0.235928, 0.231489, 0.226451, 0.220675, 
  0.213971, 0.206071, 0.19657, 0.184822, 0.169692, 0.160224, 0.148867,
   0.134739, 0.12615, 0.116076, 0.111499, 0.106524, 0.101069, 
  0.0950193, 0.088212, 0.0803933, 0.0711348, 0.0657335, 0.0595967, 
  0.0524182, 0.0482728, 0.0435993, 0.03818, 0.0369756, 0.0357211, 
  0.0344103, 0.0330362, 0.0315898, 0.028432, 0.0247987, 0.0227292, 
  0.0204204}, {0.486993, 0.484741, 0.483427, 0.482389, 0.482301, 
  0.482372, 0.48246, 0.482542, 0.482611, 0.482664, 0.482744, 0.482718,
   0.482702, 0.482709, 0.482722, 0.482729, 0.482743, 0.482759, 
  0.482774, 0.482789, 0.482804, 0.482819, 0.482834, 0.482871, 
  0.482907, 0.48298, 0.483051, 0.483121, 0.483256, 0.483386, 0.483511,
   0.48363, 0.483742, 0.483849, 0.483949, 0.484043, 0.48413, 0.48421, 
  0.484283, 0.484348, 0.484477, 0.484555, 0.484575, 0.484535, 
  0.484428, 0.484247, 0.483987, 0.483638, 0.483192, 0.48264, 0.481968,
   0.481164, 0.480211, 0.479093, 0.477786, 0.476267, 0.474506, 
  0.472466, 0.470105, 0.467369, 0.464195, 0.4605, 0.456181, 0.451103, 
  0.44509, 0.437899, 0.42919, 0.418467, 0.413455, 0.407937, 0.401829, 
  0.39503, 0.387407, 0.37879, 0.36895, 0.357573, 0.344212, 0.328195, 
  0.308447, 0.283082, 0.267235, 0.248247, 0.224647, 0.210309, 
  0.193501, 0.185866, 0.177568, 0.168469, 0.158382, 0.147032, 
  0.133998, 0.118564, 0.10956, 0.0993313, 0.0873659, 0.0804562, 
  0.0726665, 0.0636339, 0.0616265, 0.0595355, 0.0573509, 0.0550606, 
  0.0526499, 0.0473868, 0.0413313, 0.0378821, 0.034034}, {0.709974, 
  0.711601, 0.713499, 0.714385, 0.713785, 0.713546, 0.713418, 
  0.713383, 0.713411, 0.713468, 0.713655, 0.713595, 0.713563, 
  0.713584, 0.713571, 0.713561, 0.71355, 0.713535, 0.713522, 0.713507,
   0.713493, 0.713479, 0.713465, 0.713428, 0.713392, 0.713317, 
  0.713241, 0.713162, 0.712999, 0.712828, 0.712649, 0.712461, 
  0.712264, 0.712058, 0.711843, 0.711618, 0.711383, 0.711138, 
  0.710882, 0.710616, 0.709899, 0.709105, 0.708229, 0.707264, 
  0.706201, 0.705033, 0.703748, 0.702336, 0.700785, 0.699079, 
  0.697202, 0.695135, 0.692855, 0.690337, 0.68755, 0.684459, 0.68102, 
  0.677183, 0.672887, 0.668056, 0.662599, 0.656401, 0.649315, 
  0.641153, 0.631664, 0.620511, 0.607221, 0.591101, 0.583641, 
  0.575473, 0.566484, 0.556529, 0.545428, 0.532943, 0.518759, 
  0.502439, 0.48336, 0.460589, 0.432625, 0.396831, 0.37452, 0.347819, 
  0.314676, 0.294558, 0.270986, 0.260282, 0.248651, 0.235902, 
  0.221769, 0.20587, 0.187614, 0.166001, 0.153393, 0.13907, 0.122317, 
  0.112642, 0.101735, 0.0890889, 0.0862783, 0.0833507, 0.080292, 
  0.0770855, 0.0737103, 0.0663418, 0.057864, 0.053035, 
  0.0476477}, {0.972477, 0.969229, 0.96766, 0.969029, 0.969442, 
  0.969155, 0.968886, 0.968746, 0.968738, 0.968815, 0.969029, 
  0.968878, 0.968919, 0.968877, 0.968857, 0.968828, 0.968768, 
  0.968706, 0.968649, 0.968589, 0.968529, 0.96847, 0.96841, 0.96826, 
  0.96811, 0.967806, 0.9675, 0.96719, 0.966561, 0.96592, 0.965265, 
  0.964597, 0.963916, 0.96322, 0.962509, 0.961784, 0.961043, 0.960287,
   0.959514, 0.958725, 0.956678, 0.954518, 0.952237, 0.949827, 
  0.947277, 0.944577, 0.941715, 0.938677, 0.935446, 0.932005, 
  0.928331, 0.924401, 0.920185, 0.915649, 0.910755, 0.905456, 
  0.899695, 0.893406, 0.88651, 0.878908, 0.870479, 0.861073, 0.850501,
   0.838516, 0.824795, 0.808901, 0.790225, 0.767883, 0.757639, 
  0.746483, 0.73427, 0.720819, 0.7059, 0.689212, 0.670355, 0.648776, 
  0.623682, 0.593881, 0.557455, 0.511016, 0.482143, 0.44764, 0.404868,
   0.378929, 0.348555, 0.334768, 0.319791, 0.303378, 0.285189, 
  0.264731, 0.241246, 0.213447, 0.197233, 0.178815, 0.157271, 
  0.144831, 0.130807, 0.114545, 0.110932, 0.107167, 0.103234, 
  0.0991113, 0.0947715, 0.0852972, 0.0743968, 0.068188, 
  0.0612614}, {1.23488, 1.2385, 1.23757, 1.23638, 1.23735, 1.23719, 
  1.23689, 1.23671, 1.23672, 1.23684, 1.23689, 1.23686, 1.23687, 
  1.23681, 1.23674, 1.23671, 1.2366, 1.23651, 1.23641, 1.23631, 
  1.23622, 1.23612, 1.23602, 1.23578, 1.23553, 1.23504, 1.23454, 
  1.23403, 1.23301, 1.23196, 1.23088, 1.22979, 1.22867, 1.22753, 
  1.22637, 1.22518, 1.22396, 1.22272, 1.22145, 1.22016, 1.21681, 
  1.21328, 1.20956, 1.20564, 1.20152, 1.19717, 1.19258, 1.18775, 
  1.18264, 1.17724, 1.17153, 1.16546, 1.15902, 1.15215, 1.14481, 
  1.13695, 1.12848, 1.11934, 1.10942, 1.09859, 1.08672, 1.07361, 
  1.05902, 1.04265, 1.02409, 1.00282, 0.978074, 0.948772, 0.935434, 
  0.920971, 0.905209, 0.887929, 0.868853, 0.847622, 0.823753, 
  0.796583, 0.765161, 0.728048, 0.682917, 0.625639, 0.590118, 
  0.547732, 0.49525, 0.463448, 0.42623, 0.409344, 0.391005, 0.370913, 
  0.348652, 0.323623, 0.294897, 0.260904, 0.241081, 0.218564, 
  0.192229, 0.177023, 0.159881, 0.140004, 0.135587, 0.130985, 
  0.126178, 0.121138, 0.115834, 0.104253, 0.09093, 0.0833412, 
  0.0748751}, {1.50812, 1.50714, 1.50886, 1.50765, 1.50833, 1.50831, 
  1.50805, 1.5079, 1.50794, 1.50808, 1.50796, 1.50805, 1.50794, 
  1.50791, 1.50787, 1.50779, 1.50768, 1.50756, 1.50745, 1.50733, 
  1.50721, 1.50709, 1.50697, 1.50667, 1.50637, 1.50577, 1.50515, 
  1.50453, 1.50326, 1.50195, 1.50061, 1.49924, 1.49784, 1.49639, 
  1.49491, 1.4934, 1.49185, 1.49025, 1.48862, 1.48695, 1.4826, 
  1.47798, 1.47308, 1.46789, 1.46239, 1.45657, 1.45042, 1.44391, 
  1.43702, 1.42973, 1.42201, 1.41383, 1.40515, 1.39591, 1.38607, 
  1.37555, 1.36428, 1.35215, 1.33904, 1.32481, 1.30928, 1.29221, 
  1.27333, 1.25226, 1.22852, 1.20145, 1.17016, 1.13335, 1.11668, 
  1.09865, 1.07906, 1.05766, 1.03413, 1.00803, 0.978819, 0.945725, 
  0.907651, 0.862934, 0.808869, 0.7406, 0.698384, 0.648072, 0.585828, 
  0.54813, 0.50403, 0.484027, 0.462309, 0.43852, 0.412171, 0.382553, 
  0.348572, 0.308374, 0.284937, 0.258319, 0.227191, 0.209218, 
  0.188957, 0.165464, 0.160243, 0.154805, 0.149123, 0.143167, 
  0.136898, 0.123211, 0.107464, 0.0984948, 0.102103}, {1.77947, 
  1.77829, 1.77828, 1.77845, 1.7786, 1.77868, 1.7785, 1.77838, 
  1.77844, 1.77854, 1.77842, 1.77841, 1.77841, 1.77834, 1.77827, 
  1.7782, 1.77808, 1.77795, 1.77783, 1.7777, 1.77757, 1.77744, 
  1.77732, 1.77699, 1.77667, 1.77601, 1.77534, 1.77466, 1.77327, 
  1.77183, 1.77035, 1.76883, 1.76726, 1.76564, 1.76398, 1.76226, 
  1.7605, 1.75868, 1.75682, 1.75489, 1.74985, 1.74445, 1.73867, 
  1.73249, 1.72589, 1.71886, 1.71137, 1.7034, 1.69493, 1.68592, 
  1.67635, 1.66618, 1.65536, 1.64384, 1.63157, 1.61846, 1.60441, 
  1.58931, 1.57303, 1.55539, 1.53617, 1.5151, 1.49185, 1.46597, 
  1.43689, 1.40384, 1.36575, 1.32111, 1.30093, 1.27916, 1.25555, 
  1.22981, 1.20156, 1.17032, 1.13545, 1.09609, 1.05101, 0.998352, 
  0.935078, 0.85569, 0.806777, 0.748562, 0.676569, 0.632966, 0.58196, 
  0.558828, 0.533715, 0.506213, 0.475757, 0.441533, 0.40228, 0.355861,
   0.328804, 0.29808, 0.262156, 0.241415, 0.218035, 0.190926, 
  0.184902, 0.178626, 0.17207, 0.165197, 0.157962, 0.142169, 0.123998,
   0.113649, 0.115718}};


  /* 4, 4 mode */
  const double reomegaqnm44[8][107] = {{0.603485, 0.613847, 0.619636, 0.623952, 0.624219, 0.623894, 
  0.623504, 0.623122, 0.62278, 0.622487, 0.621648, 0.621375, 0.621309,
   0.621365, 0.621483, 0.621613, 0.621877, 0.622141, 0.622404, 
  0.622667, 0.62293, 0.623194, 0.623458, 0.624119, 0.624781, 0.626113,
   0.627452, 0.628799, 0.631518, 0.634269, 0.637054, 0.639872, 
  0.642726, 0.645615, 0.648541, 0.651503, 0.654504, 0.657544, 
  0.660623, 0.663743, 0.671728, 0.679989, 0.688543, 0.697411, 
  0.706611, 0.716168, 0.726107, 0.736455, 0.747243, 0.758508, 
  0.770286, 0.782624, 0.795569, 0.809178, 0.823517, 0.83866, 0.854693,
   0.871718, 0.889853, 0.909242, 0.930054, 0.9525, 0.976839, 1.0034, 
  1.03259, 1.06498, 1.10131, 1.14265, 1.16092, 1.18036, 1.20114, 
  1.22345, 1.24755, 1.27374, 1.30245, 1.33422, 1.36984, 1.41042, 
  1.45773, 1.51478, 1.54862, 1.58759, 1.6339, 1.66102, 1.69194, 
  1.7057, 1.72045, 1.73641, 1.75384, 1.77314, 1.79492, 1.82022, 
  1.83474, 1.85104, 1.86985, 1.8806, 1.8926, 1.9064, 1.90945, 1.91261,
   1.91592, 1.91937, 1.92299, 1.93088, 1.93989, 1.945, 
  1.95068}, {0.59231, 0.596523, 0.598412, 0.599977, 0.600617, 
  0.600806, 0.600947, 0.601053, 0.601136, 0.601202, 0.601389, 
  0.601485, 0.601558, 0.601693, 0.601828, 0.601964, 0.602238, 
  0.602511, 0.602785, 0.60306, 0.603334, 0.603609, 0.603884, 0.604574,
   0.605265, 0.606653, 0.60805, 0.609455, 0.61229, 0.615158, 0.618061,
   0.620998, 0.623972, 0.626982, 0.630029, 0.633115, 0.63624, 
  0.639405, 0.64261, 0.645858, 0.654166, 0.662758, 0.671651, 0.680865,
   0.69042, 0.700341, 0.710653, 0.721383, 0.732565, 0.744232, 
  0.756425, 0.769188, 0.782571, 0.796632, 0.811434, 0.827055, 
  0.843581, 0.861114, 0.879773, 0.899703, 0.921074, 0.944098, 
  0.969034, 0.996209, 1.02604, 1.05909, 1.09611, 1.13816, 1.15671, 
  1.17644, 1.19752, 1.22013, 1.24453, 1.27103, 1.30005, 1.33214, 
  1.36806, 1.40896, 1.45658, 1.51396, 1.54795, 1.58707, 1.63353, 
  1.66073, 1.69172, 1.7055, 1.72028, 1.73626, 1.75372, 1.77304, 
  1.79485, 1.82017, 1.8347, 1.85101, 1.86983, 1.88058, 1.89259, 
  1.90639, 1.90944, 1.91261, 1.91591, 1.91936, 1.92299, 1.93087, 
  1.93989, 1.945, 1.95068}, {0.559386, 0.561881, 0.562985, 0.563673, 
  0.563747, 0.563732, 0.563712, 0.563695, 0.563683, 0.563678, 
  0.563718, 0.563794, 0.563872, 0.564021, 0.564168, 0.564316, 
  0.564612, 0.564908, 0.565204, 0.565501, 0.565798, 0.566096, 
  0.566394, 0.567139, 0.567887, 0.56939, 0.570901, 0.57242, 0.575484, 
  0.578584, 0.58172, 0.584892, 0.588102, 0.59135, 0.594637, 0.597965, 
  0.601333, 0.604743, 0.608195, 0.611692, 0.62063, 0.629865, 0.639415,
   0.649299, 0.65954, 0.670161, 0.681189, 0.692653, 0.704586, 
  0.717023, 0.730006, 0.743579, 0.757794, 0.77271, 0.788392, 0.804918,
   0.822375, 0.840868, 0.860518, 0.88147, 0.903897, 0.928012, 
  0.954076, 0.982419, 1.01346, 1.04777, 1.08608, 1.12947, 1.14857, 
  1.16887, 1.19051, 1.21369, 1.23867, 1.26576, 1.29538, 1.32806, 
  1.36459, 1.4061, 1.45434, 1.51232, 1.54662, 1.58604, 1.6328, 
  1.66014, 1.69127, 1.70511, 1.71994, 1.73598, 1.75348, 1.77286, 
  1.79471, 1.82008, 1.83463, 1.85096, 1.8698, 1.88055, 1.89257, 
  1.90638, 1.90943, 1.9126, 1.9159, 1.91935, 1.92298, 1.93087, 
  1.93989, 1.945, 1.95068}, {0.514416, 0.513669, 0.512866, 0.5122, 
  0.512256, 0.512336, 0.512404, 0.512455, 0.512488, 0.51251, 0.512565,
   0.51264, 0.512726, 0.512889, 0.513051, 0.513214, 0.51354, 0.513866,
   0.514193, 0.51452, 0.514848, 0.515175, 0.515503, 0.516325, 
  0.517149, 0.518804, 0.520467, 0.522139, 0.525511, 0.52892, 0.532367,
   0.535852, 0.539377, 0.542943, 0.546549, 0.550198, 0.553889, 
  0.557625, 0.561406, 0.565233, 0.575009, 0.585097, 0.595517, 
  0.606289, 0.617438, 0.628986, 0.640963, 0.653399, 0.666327, 
  0.679785, 0.693815, 0.708464, 0.723784, 0.739837, 0.75669, 0.774421,
   0.793122, 0.812899, 0.833875, 0.856197, 0.880043, 0.905628, 
  0.933216, 0.96314, 0.995828, 1.03184, 1.07193, 1.11717, 1.13703, 
  1.1581, 1.18052, 1.20451, 1.2303, 1.25821, 1.28866, 1.32219, 
  1.35958, 1.40195, 1.45106, 1.50993, 1.54467, 1.58453, 1.63172, 
  1.65927, 1.69061, 1.70453, 1.71944, 1.73555, 1.75313, 1.77258, 
  1.79451, 1.81994, 1.83452, 1.85088, 1.86975, 1.88051, 1.89254, 
  1.90636, 1.90941, 1.91258, 1.91589, 1.91934, 1.92297, 1.93086, 
  1.93988, 1.94499, 1.95068}, {0.454601, 0.454746, 0.455842, 0.456343,
   0.456046, 0.455998, 0.456018, 0.456071, 0.456127, 0.456171, 
  0.456246, 0.456327, 0.456423, 0.456596, 0.456773, 0.456949, 
  0.457302, 0.457655, 0.458009, 0.458363, 0.458717, 0.459072, 
  0.459427, 0.460317, 0.461208, 0.462999, 0.464799, 0.466609, 
  0.470257, 0.473946, 0.477675, 0.481446, 0.485259, 0.489115, 
  0.493016, 0.496962, 0.500954, 0.504993, 0.509081, 0.513219, 
  0.523786, 0.534689, 0.545948, 0.557585, 0.569625, 0.582093, 0.59502,
   0.608436, 0.622378, 0.636885, 0.652, 0.667772, 0.684256, 0.701516, 
  0.71962, 0.738651, 0.758702, 0.779882, 0.802317, 0.82616, 0.851591, 
  0.878829, 0.908143, 0.939873, 0.974449, 1.01244, 1.05461, 1.10202, 
  1.12279, 1.14477, 1.16814, 1.19308, 1.21984, 1.24875, 1.28021, 
  1.31477, 1.35322, 1.39666, 1.44687, 1.50684, 1.54214, 1.58256, 
  1.63031, 1.65813, 1.68974, 1.70376, 1.71878, 1.73499, 1.75267, 
  1.77222, 1.79424, 1.81975, 1.83438, 1.85077, 1.86967, 1.88046, 
  1.8925, 1.90633, 1.90939, 1.91256, 1.91587, 1.91932, 1.92296, 
  1.93085, 1.93987, 1.94499, 1.95067}, {0.405546, 0.404573, 0.403366, 
  0.404025, 0.404115, 0.403989, 0.40394, 0.403971, 0.404037, 0.404098,
   0.40416, 0.404265, 0.404355, 0.404535, 0.404715, 0.404897, 0.40526,
   0.405624, 0.405988, 0.406353, 0.406718, 0.407083, 0.407449, 
  0.408365, 0.409284, 0.411129, 0.412986, 0.414853, 0.418619, 0.42243,
   0.426286, 0.430188, 0.434137, 0.438134, 0.44218, 0.446277, 
  0.450425, 0.454625, 0.458878, 0.463187, 0.474206, 0.485594, 
  0.497376, 0.509573, 0.522212, 0.535321, 0.54893, 0.563075, 0.577791,
   0.593121, 0.609109, 0.625808, 0.643274, 0.661572, 0.680777, 
  0.700971, 0.722252, 0.744732, 0.768542, 0.793839, 0.820807, 
  0.849672, 0.880708, 0.914263, 0.950774, 0.990817, 1.03516, 1.0849, 
  1.10664, 1.12961, 1.15399, 1.17998, 1.20782, 1.23782, 1.27041, 
  1.30613, 1.34576, 1.39042, 1.44188, 1.50314, 1.53909, 1.58017, 
  1.62858, 1.65673, 1.68866, 1.70281, 1.71796, 1.7343, 1.7521, 
  1.77176, 1.7939, 1.81952, 1.83419, 1.85064, 1.86958, 1.88039, 
  1.89245, 1.9063, 1.90936, 1.91253, 1.91584, 1.9193, 1.92294, 
  1.93083, 1.93986, 1.94498, 1.95067}, {0.35976, 0.362048, 0.362088, 
  0.361405, 0.361838, 0.361724, 0.361635, 0.361653, 0.361731, 
  0.361798, 0.361829, 0.361951, 0.362022, 0.362204, 0.362384, 
  0.362559, 0.362914, 0.363269, 0.363624, 0.36398, 0.364336, 0.364693,
   0.36505, 0.365945, 0.366843, 0.368648, 0.370465, 0.372293, 
  0.375986, 0.379729, 0.383522, 0.387367, 0.391264, 0.395214, 0.39922,
   0.403281, 0.4074, 0.411576, 0.415813, 0.42011, 0.431129, 0.44256, 
  0.454427, 0.466756, 0.479575, 0.492914, 0.506807, 0.521289, 
  0.536401, 0.552185, 0.568691, 0.585972, 0.604089, 0.623109, 
  0.643109, 0.664176, 0.686412, 0.709933, 0.734875, 0.761399, 
  0.789696, 0.819999, 0.85259, 0.887824, 0.92615, 0.968153, 1.01462, 
  1.06665, 1.08936, 1.11334, 1.13875, 1.16581, 1.19475, 1.22589, 
  1.25966, 1.29659, 1.33748, 1.38345, 1.43625, 1.49892, 1.53559, 
  1.57741, 1.62657, 1.6551, 1.6874, 1.7017, 1.717, 1.73348, 1.75142, 
  1.77123, 1.79349, 1.81924, 1.83398, 1.85048, 1.86948, 1.8803, 
  1.89239, 1.90626, 1.90932, 1.9125, 1.91581, 1.91928, 1.92291, 
  1.93082, 1.93985, 1.94497, 1.95066},
{0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72, 0.72}};

  const double imomegaqnm44[8][107] = {{0.0472887, 0.0570846, 0.0652857, 0.0769202, 0.0835313, 0.0855743, 
  0.0870596, 0.0881437, 0.0889418, 0.0895359, 0.0909619, 0.0914196, 
  0.0916056, 0.091741, 0.0917845, 0.0918026, 0.0918186, 0.0918281, 
  0.0918366, 0.0918448, 0.0918531, 0.0918614, 0.0918696, 0.0918903, 
  0.0919109, 0.0919521, 0.0919931, 0.092034, 0.0921151, 0.0921956, 
  0.0922753, 0.0923543, 0.0924324, 0.0925097, 0.0925861, 0.0926616, 
  0.0927361, 0.0928096, 0.0928821, 0.0929535, 0.0931268, 0.093292, 
  0.0934484, 0.0935949, 0.0937302, 0.0938532, 0.0939623, 0.0940559, 
  0.0941321, 0.0941887, 0.0942233, 0.094233, 0.0942145, 0.094164, 
  0.0940768, 0.0939478, 0.0937705, 0.0935374, 0.0932393, 0.092865, 
  0.0924006, 0.0918287, 0.0911274, 0.0902679, 0.0892124, 0.0879094, 
  0.086287, 0.0842401, 0.0832692, 0.0821917, 0.0809905, 0.0796442, 
  0.0781255, 0.0763992, 0.0744185, 0.0721192, 0.0694103, 0.066156, 
  0.0621396, 0.056982, 0.0537633, 0.0499111, 0.0451315, 0.0422323, 
  0.0388376, 0.037297, 0.0356236, 0.0337901, 0.0317587, 0.0294747, 
  0.0268536, 0.0237529, 0.0219453, 0.0198926, 0.0174928, 0.0161075, 
  0.0145463, 0.0127365, 0.0123344, 0.0119155, 0.0114779, 0.0110192, 
  0.0105364, 0.00948248, 0.00827008, 0.00757959, 
  0.00680933}, {0.267496, 0.27243, 0.275117, 0.277412, 0.278166, 
  0.278332, 0.278431, 0.278489, 0.278523, 0.278542, 0.278554, 
  0.278543, 0.278539, 0.278544, 0.278555, 0.278567, 0.27859, 0.278613,
   0.278636, 0.278659, 0.278682, 0.278704, 0.278727, 0.278784, 
  0.278841, 0.278955, 0.279068, 0.27918, 0.279403, 0.279623, 0.279841,
   0.280057, 0.28027, 0.28048, 0.280687, 0.280892, 0.281093, 0.281291,
   0.281485, 0.281676, 0.282137, 0.282572, 0.282979, 0.283355, 
  0.283696, 0.283998, 0.284257, 0.284467, 0.284624, 0.284721, 
  0.284749, 0.284701, 0.284567, 0.284334, 0.28399, 0.283519, 0.282901,
   0.282113, 0.281129, 0.279913, 0.278426, 0.276614, 0.274412, 
  0.271733, 0.268465, 0.264452, 0.25948, 0.253233, 0.250278, 0.247003,
   0.243357, 0.239276, 0.234678, 0.229458, 0.223476, 0.216539, 
  0.208375, 0.198576, 0.186494, 0.170992, 0.161324, 0.149756, 
  0.135408, 0.126707, 0.116519, 0.111896, 0.106875, 0.101374, 
  0.0952787, 0.0884259, 0.0805622, 0.0712595, 0.0658364, 0.0596782, 
  0.0524786, 0.0483228, 0.043639, 0.0382095, 0.0370031, 0.0357465, 
  0.0344338, 0.0330576, 0.0316092, 0.0284475, 0.0248103, 0.0227388, 
  0.020428}, {0.476992, 0.476552, 0.476059, 0.475354, 0.475023, 
  0.474945, 0.474902, 0.47488, 0.474873, 0.474873, 0.474904, 0.474923,
   0.474932, 0.474945, 0.47496, 0.474975, 0.475006, 0.475036, 
  0.475066, 0.475097, 0.475127, 0.475157, 0.475187, 0.475263, 
  0.475338, 0.475487, 0.475635, 0.475782, 0.476073, 0.476359, 
  0.476641, 0.476918, 0.47719, 0.477456, 0.477718, 0.477973, 0.478223,
   0.478467, 0.478704, 0.478936, 0.479483, 0.479984, 0.480434, 
  0.480828, 0.48116, 0.481423, 0.48161, 0.481712, 0.481721, 0.481624, 
  0.48141, 0.481064, 0.48057, 0.479908, 0.479056, 0.477986, 0.476666, 
  0.47506, 0.47312, 0.470791, 0.468003, 0.464669, 0.460679, 0.45589, 
  0.450113, 0.44309, 0.434465, 0.423712, 0.41865, 0.413055, 0.406842, 
  0.399905, 0.392108, 0.383276, 0.373175, 0.361487, 0.347759, 
  0.331313, 0.311071, 0.28514, 0.268985, 0.249669, 0.225726, 0.21121, 
  0.194221, 0.186512, 0.17814, 0.168968, 0.158807, 0.147383, 0.134275,
   0.118768, 0.109729, 0.099465, 0.0874652, 0.0805385, 0.072732, 
  0.0636828, 0.0616721, 0.0595777, 0.0573898, 0.0550962, 0.0526822, 
  0.0474125, 0.0413505, 0.037898, 0.0340467}, {0.688887, 0.687276, 
  0.68687, 0.687194, 0.687484, 0.687529, 0.687535, 0.687523, 0.687504,
   0.687487, 0.687464, 0.68748, 0.687488, 0.687497, 0.687509, 
  0.687521, 0.687544, 0.687567, 0.68759, 0.687613, 0.687636, 0.687658,
   0.687681, 0.687738, 0.687794, 0.687905, 0.688015, 0.688122, 
  0.688333, 0.688537, 0.688734, 0.688923, 0.689105, 0.689278, 
  0.689444, 0.689601, 0.689749, 0.689889, 0.690018, 0.690138, 
  0.690394, 0.690579, 0.69069, 0.690717, 0.690652, 0.690487, 0.690211,
   0.689812, 0.689276, 0.688589, 0.687732, 0.686685, 0.685425, 
  0.683924, 0.682151, 0.680068, 0.677629, 0.674782, 0.671462, 
  0.667589, 0.663066, 0.657772, 0.651551, 0.644203, 0.635463, 
  0.624972, 0.612229, 0.596503, 0.589146, 0.581044, 0.572077, 
  0.562097, 0.550915, 0.538287, 0.523888, 0.507273, 0.48781, 0.464555,
   0.436002, 0.399509, 0.376808, 0.349693, 0.316109, 0.295761, 
  0.271954, 0.261154, 0.249426, 0.236578, 0.222347, 0.206349, 
  0.187993, 0.166281, 0.153625, 0.139254, 0.122453, 0.112755, 
  0.101826, 0.0891563, 0.0863413, 0.0834091, 0.080346, 0.0771349, 
  0.0737552, 0.0663776, 0.0578907, 0.0530572, 0.0476654}, {0.918235, 
  0.920515, 0.920895, 0.920137, 0.920012, 0.920094, 0.920167, 
  0.920207, 0.920214, 0.920202, 0.920149, 0.920166, 0.920164, 0.92016,
   0.920158, 0.920156, 0.920152, 0.920148, 0.920144, 0.92014, 
  0.920136, 0.920132, 0.920127, 0.920116, 0.920104, 0.920079, 
  0.920051, 0.920022, 0.919956, 0.919881, 0.919796, 0.919702, 
  0.919597, 0.919482, 0.919356, 0.919218, 0.919069, 0.918908, 
  0.918734, 0.918547, 0.91802, 0.917401, 0.916681, 0.915851, 0.9149, 
  0.913815, 0.912584, 0.91119, 0.909617, 0.907843, 0.905847, 0.903603,
   0.901078, 0.898239, 0.895043, 0.891442, 0.887378, 0.88278, 
  0.877566, 0.871633, 0.864855, 0.857075, 0.848091, 0.837647, 0.8254, 
  0.81089, 0.79347, 0.772205, 0.762324, 0.751485, 0.739534, 0.72628, 
  0.711484, 0.694831, 0.675907, 0.654142, 0.628729, 0.598457, 0.5614, 
  0.514171, 0.484847, 0.449863, 0.406581, 0.380377, 0.349729, 
  0.335831, 0.32074, 0.304211, 0.285904, 0.265327, 0.24172, 0.213799, 
  0.197524, 0.179045, 0.157442, 0.144973, 0.13092, 0.11463, 0.111011, 
  0.107241, 0.103302, 0.0991739, 0.0948285, 0.0853429, 0.0744311, 
  0.0682165, 0.0612841}, {1.17243, 1.17025, 1.17072, 1.17153, 1.17109,
   1.17107, 1.17115, 1.17122, 1.17125, 1.17124, 1.17116, 1.17118, 
  1.17115, 1.17114, 1.17112, 1.17109, 1.17105, 1.171, 1.17096, 
  1.17092, 1.17087, 1.17083, 1.17078, 1.17067, 1.17055, 1.17033, 
  1.17009, 1.16986, 1.16938, 1.16889, 1.16838, 1.16786, 1.16733, 
  1.16679, 1.16622, 1.16565, 1.16506, 1.16445, 1.16382, 1.16318, 
  1.1615, 1.15969, 1.15776, 1.15567, 1.15344, 1.15103, 1.14843, 
  1.14562, 1.14259, 1.1393, 1.13573, 1.13184, 1.12761, 1.12298, 
  1.1179, 1.11232, 1.10615, 1.09933, 1.09174, 1.08325, 1.07371, 
  1.06294, 1.05067, 1.0366, 1.0203, 1.00121, 0.97854, 0.951144, 
  0.9385, 0.92468, 0.909499, 0.892724, 0.874063, 0.853135, 0.829435, 
  0.802272, 0.770663, 0.733138, 0.687353, 0.629184, 0.593147, 
  0.550213, 0.497162, 0.465071, 0.427558, 0.410551, 0.392089, 
  0.371872, 0.349482, 0.32432, 0.295457, 0.261323, 0.241428, 0.218839,
   0.192433, 0.177192, 0.160015, 0.140105, 0.135681, 0.131073, 
  0.126259, 0.121213, 0.115902, 0.104308, 0.0909715, 0.0833759, 
  0.0749029}, {1.43411, 1.43505, 1.43374, 1.43433, 1.43411, 1.43401, 
  1.43406, 1.43415, 1.43418, 1.43415, 1.4341, 1.43407, 1.43405, 
  1.43401, 1.43397, 1.43393, 1.43384, 1.43376, 1.43367, 1.43359, 
  1.4335, 1.43342, 1.43333, 1.43312, 1.4329, 1.43247, 1.43203, 
  1.43159, 1.43069, 1.42978, 1.42884, 1.42789, 1.42691, 1.42592, 
  1.4249, 1.42387, 1.42281, 1.42173, 1.42062, 1.41949, 1.41656, 
  1.41345, 1.41017, 1.4067, 1.40302, 1.39911, 1.39496, 1.39054, 
  1.38582, 1.38079, 1.3754, 1.36961, 1.36339, 1.35669, 1.34943, 
  1.34155, 1.33296, 1.32356, 1.31324, 1.30183, 1.28915, 1.27497, 
  1.259, 1.24085, 1.22004, 1.19589, 1.16745, 1.13339, 1.11775, 
  1.10072, 1.08207, 1.06154, 1.03877, 1.01331, 0.984584, 0.951766, 
  0.913705, 0.868675, 0.81392, 0.744589, 0.701738, 0.650764, 0.587868,
   0.549856, 0.505448, 0.485323, 0.46348, 0.439564, 0.413085, 
  0.383331, 0.349205, 0.308853, 0.285337, 0.258637, 0.227427, 
  0.209413, 0.189112, 0.16558, 0.160352, 0.154906, 0.149217, 0.143253,
   0.136976, 0.123274, 0.107512, 0.0985354, 0.0885218}, 
{0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 
	0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28}};

  /* 5, 5 mode */
  const double reomegaqnm55[8][107] = {{0.752532, 0.769133, 0.779179, 0.78709, 0.786149, 0.784329, 0.782338,
   0.780485, 0.778903, 0.777615, 0.774286, 0.773282, 0.772966, 
  0.77289, 0.772999, 0.773149, 0.773476, 0.77381, 0.774145, 0.77448, 
  0.774816, 0.775152, 0.775488, 0.776331, 0.777176, 0.778874, 
  0.780582, 0.782299, 0.785766, 0.789274, 0.792825, 0.79642, 0.800059,
   0.803743, 0.807474, 0.811253, 0.81508, 0.818956, 0.822883, 
  0.826863, 0.837046, 0.847582, 0.858493, 0.869802, 0.881536, 
  0.893724, 0.906398, 0.919594, 0.93335, 0.947712, 0.962728, 0.978454,
   0.994953, 1.0123, 1.03056, 1.04985, 1.07027, 1.09194, 1.11502, 
  1.13968, 1.16614, 1.19467, 1.22558, 1.25928, 1.29631, 1.33734, 
  1.38332, 1.43555, 1.45861, 1.48313, 1.50932, 1.53742, 1.56774, 
  1.60067, 1.63671, 1.67655, 1.72115, 1.77188, 1.83092, 1.90197, 
  1.94403, 1.99239, 2.04977, 2.08333, 2.12154, 2.13852, 2.15673, 
  2.17641, 2.1979, 2.22168, 2.24849, 2.27961, 2.29746, 2.31749, 
  2.34058, 2.35377, 2.3685, 2.38542, 2.38915, 2.39303, 2.39708, 
  2.40131, 2.40575, 2.41541, 2.42645, 2.43271, 2.43967}, {0.753001, 
  0.757208, 0.757533, 0.756804, 0.756468, 0.756414, 0.756397, 
  0.756401, 0.756417, 0.756438, 0.756561, 0.756667, 0.756761, 
  0.756937, 0.757109, 0.75728, 0.757623, 0.757967, 0.758311, 0.758656,
   0.759001, 0.759346, 0.759692, 0.760558, 0.761427, 0.763172, 
  0.764927, 0.766693, 0.770255, 0.77386, 0.777508, 0.7812, 0.784938, 
  0.788722, 0.792553, 0.796433, 0.800362, 0.804341, 0.808372, 
  0.812456, 0.822904, 0.833711, 0.844898, 0.85649, 0.868513, 0.880997,
   0.893974, 0.90748, 0.921555, 0.936243, 0.951594, 0.967664, 
  0.984515, 1.00222, 1.02086, 1.04054, 1.06135, 1.08343, 1.10693, 
  1.13203, 1.15894, 1.18793, 1.21932, 1.25352, 1.29105, 1.33262, 
  1.37915, 1.43196, 1.45525, 1.48001, 1.50644, 1.53478, 1.56534, 
  1.59852, 1.63481, 1.67491, 1.71976, 1.77074, 1.83004, 1.90134, 
  1.94352, 1.992, 2.0495, 2.08311, 2.12137, 2.13838, 2.15661, 2.17631,
   2.19781, 2.22161, 2.24844, 2.27957, 2.29743, 2.31747, 2.34057, 
  2.35376, 2.36849, 2.38541, 2.38915, 2.39303, 2.39708, 2.40131, 
  2.40575, 2.41541, 2.42645, 2.43271, 2.43967}, {0.7196, 0.722567, 
  0.723924, 0.725024, 0.725386, 0.725467, 0.725518, 0.725552, 
  0.725576, 0.725595, 0.725675, 0.72576, 0.725849, 0.72603, 0.726211, 
  0.726392, 0.726754, 0.727117, 0.72748, 0.727843, 0.728207, 0.728571,
   0.728936, 0.729849, 0.730765, 0.732605, 0.734456, 0.736316, 
  0.740071, 0.743868, 0.747711, 0.751599, 0.755534, 0.759516, 
  0.763547, 0.767627, 0.771759, 0.775942, 0.780178, 0.784469, 
  0.795442, 0.806783, 0.818514, 0.830662, 0.843253, 0.856316, 
  0.869886, 0.883998, 0.898692, 0.914015, 0.930016, 0.946752, 
  0.964288, 0.982696, 1.00206, 1.02247, 1.04405, 1.06692, 1.09123, 
  1.11716, 1.14494, 1.17482, 1.20713, 1.24229, 1.28082, 1.32342, 
  1.37101, 1.42493, 1.44867, 1.47389, 1.50078, 1.5296, 1.56064, 
  1.5943, 1.63108, 1.67167, 1.71702, 1.7685, 1.82829, 1.90008, 1.9425,
   1.99122, 2.04895, 2.08267, 2.12104, 2.13809, 2.15636, 2.1761, 
  2.19764, 2.22147, 2.24834, 2.27951, 2.29738, 2.31743, 2.34054, 
  2.35374, 2.36848, 2.3854, 2.38914, 2.39302, 2.39707, 2.4013, 
  2.40575, 2.41541, 2.42645, 2.43271, 2.43966}, {0.680361, 0.681655, 
  0.681927, 0.681788, 0.681669, 0.681657, 0.681664, 0.681682, 
  0.681705, 0.681729, 0.681838, 0.681934, 0.682031, 0.682225, 0.68242,
   0.682614, 0.683004, 0.683394, 0.683784, 0.684175, 0.684566, 
  0.684957, 0.685349, 0.686331, 0.687316, 0.689293, 0.69128, 0.693279,
   0.69731, 0.701387, 0.705509, 0.709679, 0.713897, 0.718164, 
  0.722481, 0.72685, 0.731271, 0.735746, 0.740276, 0.744863, 0.756582,
   0.768683, 0.781187, 0.794122, 0.807514, 0.821395, 0.835799, 
  0.850762, 0.866327, 0.88254, 0.899452, 0.917122, 0.935614, 0.955004,
   0.975376, 0.996827, 1.01947, 1.04344, 1.06888, 1.09599, 1.12497, 
  1.1561, 1.18971, 1.22621, 1.26614, 1.31018, 1.35928, 1.41476, 
  1.43915, 1.46502, 1.49258, 1.52208, 1.5538, 1.58816, 1.62564, 
  1.66694, 1.713, 1.76521, 1.82572, 1.89823, 1.941, 1.99007, 2.04813, 
  2.08202, 2.12055, 2.13766, 2.15599, 2.17578, 2.19738, 2.22127, 
  2.24819, 2.2794, 2.2973, 2.31737, 2.34051, 2.35371, 2.36846, 
  2.38539, 2.38913, 2.39301, 2.39706, 2.40129, 2.40574, 2.4154, 
  2.42645, 2.43271, 2.43966}, {0.630475, 0.629154, 0.628652, 0.628812,
   0.628995, 0.62902, 0.629028, 0.629035, 0.629045, 0.629061, 
  0.629172, 0.629277, 0.629381, 0.629592, 0.629802, 0.630012, 
  0.630433, 0.630855, 0.631277, 0.631699, 0.632122, 0.632545, 
  0.632968, 0.634029, 0.635093, 0.637229, 0.639376, 0.641534, 
  0.645886, 0.650284, 0.65473, 0.659225, 0.66377, 0.668366, 0.673014, 
  0.677716, 0.682472, 0.687284, 0.692153, 0.69708, 0.709663, 0.722641,
   0.73604, 0.749885, 0.764206, 0.779035, 0.794407, 0.81036, 0.826938,
   0.844188, 0.862164, 0.880925, 0.900539, 0.921082, 0.94264, 
  0.965314, 0.989218, 1.01449, 1.04128, 1.06978, 1.1002, 1.13284, 
  1.168, 1.20613, 1.24774, 1.29355, 1.34449, 1.4019, 1.42708, 1.45377,
   1.48215, 1.51248, 1.54506, 1.58029, 1.61866, 1.66085, 1.70782, 
  1.76095, 1.82238, 1.89581, 1.93904, 1.98856, 2.04706, 2.08116, 
  2.1199, 2.13709, 2.1555, 2.17537, 2.19704, 2.221, 2.24799, 2.27927, 
  2.29719, 2.31729, 2.34045, 2.35367, 2.36843, 2.38537, 2.38911, 
  2.39299, 2.39704, 2.40128, 2.40573, 2.41539, 2.42644, 2.4327, 
  2.43966}, {0.571708, 0.573132, 0.573635, 0.573297, 0.573329, 
  0.573398, 0.573443, 0.573465, 0.573477, 0.573488, 0.5736, 0.573712, 
  0.573823, 0.574048, 0.574272, 0.574496, 0.574944, 0.575393, 
  0.575843, 0.576292, 0.576743, 0.577194, 0.577645, 0.578775, 
  0.579908, 0.582183, 0.58447, 0.586768, 0.591402, 0.596085, 0.600818,
   0.605603, 0.61044, 0.61533, 0.620276, 0.625278, 0.630337, 0.635455,
   0.640632, 0.645871, 0.659247, 0.673038, 0.687272, 0.701975, 
  0.717178, 0.732914, 0.74922, 0.766136, 0.783707, 0.801983, 0.821018,
   0.840873, 0.86162, 0.883336, 0.90611, 0.930046, 0.955261, 0.981893,
   1.0101, 1.04008, 1.07205, 1.10629, 1.14315, 1.18304, 1.22651, 
  1.27427, 1.32727, 1.38685, 1.41293, 1.44053, 1.46986, 1.50115, 
  1.53471, 1.57094, 1.61033, 1.65357, 1.70161, 1.75581, 1.81834, 
  1.89287, 1.93665, 1.98671, 2.04575, 2.08011, 2.1191, 2.13638, 
  2.15489, 2.17486, 2.19662, 2.22067, 2.24774, 2.2791, 2.29706, 
  2.31719, 2.34039, 2.35362, 2.36839, 2.38534, 2.38909, 2.39297, 
  2.39703, 2.40126, 2.40571, 2.41538, 2.42643, 2.4327, 
  2.43966}, {0.522196, 0.520579, 0.52057, 0.521087, 0.52093, 0.520981,
   0.521047, 0.521087, 0.521102, 0.52111, 0.521228, 0.521338, 
  0.521456, 0.521686, 0.521918, 0.522149, 0.522613, 0.523077, 
  0.523541, 0.524006, 0.524471, 0.524937, 0.525404, 0.526572, 
  0.527743, 0.530095, 0.53246, 0.534837, 0.539631, 0.544478, 0.549379,
   0.554335, 0.559348, 0.564418, 0.569547, 0.574737, 0.579987, 
  0.585301, 0.590679, 0.596123, 0.61003, 0.624382, 0.639206, 0.654532,
   0.670391, 0.686819, 0.703853, 0.721535, 0.739912, 0.759036, 
  0.778963, 0.799758, 0.821493, 0.844248, 0.868117, 0.893205, 
  0.919633, 0.947544, 0.977101, 1.0085, 1.04198, 1.07781, 1.11634, 
  1.15802, 1.20338, 1.25315, 1.30829, 1.37015, 1.39719, 1.42578, 
  1.45611, 1.48844, 1.52307, 1.56039, 1.6009, 1.64529, 1.69451, 
  1.74992, 1.81368, 1.88946, 1.93386, 1.98455, 2.0442, 2.07886, 
  2.11815, 2.13555, 2.15417, 2.17425, 2.19612, 2.22028, 2.24745, 
  2.2789, 2.29691, 2.31708, 2.34031, 2.35356, 2.36835, 2.38531, 
  2.38906, 2.39295, 2.39701, 2.40125, 2.4057, 2.41537, 2.42643, 
  2.43269, 2.43965}, 
{0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9,
	0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9}};

  const double imomegaqnm55[8][107] = {{0.0151971, 0.02987, 0.0432352, 0.064849, 0.0789971, 0.0834521, 
  0.0865117, 0.0885261, 0.0898184, 0.090638, 0.0919286, 0.0920896, 
  0.0921222, 0.0921485, 0.0921652, 0.0921765, 0.092191, 0.0922013, 
  0.0922106, 0.0922195, 0.0922283, 0.0922371, 0.0922459, 0.0922679, 
  0.0922898, 0.0923335, 0.092377, 0.0924205, 0.0925068, 0.0925926, 
  0.0926777, 0.092762, 0.0928457, 0.0929286, 0.0930106, 0.0930918, 
  0.0931722, 0.0932516, 0.09333, 0.0934074, 0.0935961, 0.0937773, 
  0.09395, 0.0941133, 0.094266, 0.0944066, 0.0945339, 0.094646, 
  0.0947411, 0.0948169, 0.0948709, 0.0949002, 0.0949014, 0.0948705, 
  0.094803, 0.0946932, 0.0945348, 0.0943198, 0.0940389, 0.0936804, 
  0.0932301, 0.09267, 0.0919774, 0.0911228, 0.0900671, 0.0887575, 
  0.0871199, 0.0850466, 0.0840612, 0.0829665, 0.0817451, 0.0803751, 
  0.0788289, 0.0770706, 0.0750526, 0.0727103, 0.0699513, 0.0666388, 
  0.0625545, 0.0573174, 0.054054, 0.0501531, 0.0453206, 0.0423931, 
  0.038969, 0.0374162, 0.0357305, 0.0338844, 0.0318402, 0.0295431, 
  0.0269089, 0.0237947, 0.0219803, 0.0199207, 0.017514, 0.0161252, 
  0.0145605, 0.0127472, 0.0123443, 0.0119247, 0.0114865, 0.011027, 
  0.0105435, 0.00948819, 0.00827437, 0.00758316, 
  0.00681219}, {0.25166, 0.264251, 0.270831, 0.275931, 0.277491, 
  0.277846, 0.278073, 0.278222, 0.278323, 0.278391, 0.278527, 
  0.278556, 0.278565, 0.278576, 0.278588, 0.2786, 0.278625, 0.27865, 
  0.278676, 0.278701, 0.278726, 0.278751, 0.278776, 0.278839, 
  0.278901, 0.279026, 0.27915, 0.279273, 0.279519, 0.279763, 0.280005,
   0.280244, 0.280481, 0.280716, 0.280948, 0.281177, 0.281404, 
  0.281627, 0.281847, 0.282065, 0.282593, 0.283097, 0.283575, 
  0.284023, 0.284438, 0.284817, 0.285154, 0.285444, 0.285682, 
  0.285862, 0.285974, 0.286011, 0.285963, 0.285817, 0.28556, 0.285175,
   0.284643, 0.28394, 0.283038, 0.281902, 0.280489, 0.278746, 
  0.276603, 0.273974, 0.27074, 0.266744, 0.261763, 0.255474, 0.252491,
   0.249179, 0.245488, 0.24135, 0.236685, 0.231383, 0.225304, 
  0.218251, 0.20995, 0.199989, 0.187715, 0.171984, 0.162185, 0.150475,
   0.135971, 0.127186, 0.116912, 0.112252, 0.107194, 0.101656, 
  0.0955223, 0.0886307, 0.0807275, 0.0713845, 0.0659411, 0.0597624, 
  0.0525421, 0.0483758, 0.0436815, 0.0382415, 0.037033, 0.0357743, 
  0.0344594, 0.0330811, 0.0316306, 0.0284646, 0.0248231, 0.0227495, 
  0.0204366}, {0.467579, 0.470028, 0.470942, 0.471348, 0.471322, 
  0.47129, 0.471261, 0.471239, 0.471223, 0.471213, 0.471201, 0.47121, 
  0.471221, 0.47124, 0.471258, 0.471277, 0.471314, 0.471351, 0.471388,
   0.471425, 0.471462, 0.471499, 0.471536, 0.471628, 0.47172, 
  0.471904, 0.472086, 0.472268, 0.472628, 0.472984, 0.473336, 
  0.473684, 0.474028, 0.474367, 0.474701, 0.475031, 0.475355, 
  0.475674, 0.475987, 0.476294, 0.477036, 0.477735, 0.478387, 
  0.478987, 0.479529, 0.480006, 0.480411, 0.480735, 0.480968, 
  0.481101, 0.481119, 0.481008, 0.480751, 0.480328, 0.479717, 
  0.478889, 0.477812, 0.476446, 0.474745, 0.472651, 0.470091, 
  0.466978, 0.463197, 0.4586, 0.452993, 0.446112, 0.437588, 0.426883, 
  0.421821, 0.416213, 0.409971, 0.402987, 0.395124, 0.386201, 
  0.375984, 0.364147, 0.350231, 0.333554, 0.313026, 0.286746, 
  0.270386, 0.250845, 0.226651, 0.212, 0.194868, 0.1871, 0.178668, 
  0.169434, 0.15921, 0.147722, 0.134549, 0.118976, 0.109903, 
  0.0996047, 0.0875705, 0.0806266, 0.0728027, 0.063736, 0.0617217, 
  0.0596238, 0.0574324, 0.0551353, 0.0527177, 0.047441, 0.0413719, 
  0.0379158, 0.034061}, {0.676801, 0.675633, 0.67493, 0.67443, 0.6744,
   0.674422, 0.674445, 0.674462, 0.674474, 0.674481, 0.67449, 
  0.674497, 0.674508, 0.674528, 0.674548, 0.674568, 0.674609, 
  0.674649, 0.674689, 0.674729, 0.674769, 0.674809, 0.674849, 
  0.674948, 0.675048, 0.675245, 0.675441, 0.675635, 0.67602, 0.676399,
   0.676771, 0.677137, 0.677496, 0.677847, 0.678192, 0.678529, 
  0.678858, 0.679179, 0.679492, 0.679795, 0.680514, 0.681168, 
  0.681753, 0.68226, 0.682681, 0.683008, 0.683228, 0.683332, 0.683303,
   0.683128, 0.682788, 0.682263, 0.681529, 0.680557, 0.679315, 
  0.677766, 0.675862, 0.673549, 0.67076, 0.667416, 0.663415, 0.658633,
   0.65291, 0.64604, 0.637751, 0.627674, 0.615292, 0.599856, 0.592588,
   0.584557, 0.575639, 0.565684, 0.554498, 0.541831, 0.527355, 
  0.510615, 0.490971, 0.467468, 0.438585, 0.401662, 0.378702, 
  0.351292, 0.317377, 0.296847, 0.272846, 0.261966, 0.250155, 
  0.237224, 0.222906, 0.20682, 0.188374, 0.16657, 0.153867, 0.139448, 
  0.1226, 0.112878, 0.101924, 0.0892306, 0.0864106, 0.0834736, 
  0.0804055, 0.0771896, 0.0738049, 0.0664175, 0.0579207, 0.0530822, 
  0.0476854}, {0.891895, 0.89147, 0.891863, 0.892311, 0.89227, 
  0.892236, 0.892217, 0.892213, 0.892217, 0.892223, 0.892242, 
  0.892246, 0.892254, 0.892269, 0.892283, 0.892298, 0.892327, 
  0.892357, 0.892386, 0.892415, 0.892444, 0.892473, 0.892502, 
  0.892574, 0.892646, 0.892788, 0.892927, 0.893065, 0.893335, 
  0.893597, 0.89385, 0.894094, 0.894329, 0.894554, 0.89477, 0.894975, 
  0.89517, 0.895353, 0.895526, 0.895686, 0.896031, 0.896291, 0.896458,
   0.896521, 0.89647, 0.896292, 0.895974, 0.895501, 0.894855, 
  0.894014, 0.892957, 0.891657, 0.890083, 0.888198, 0.88596, 0.88332, 
  0.880219, 0.876586, 0.872336, 0.867364, 0.86154, 0.854705, 0.846653,
   0.83712, 0.825754, 0.812083, 0.795442, 0.774871, 0.765236, 
  0.754619, 0.742864, 0.729775, 0.715106, 0.698537, 0.679644, 
  0.657847, 0.632324, 0.601851, 0.564477, 0.516789, 0.487173, 
  0.451846, 0.408167, 0.381741, 0.350856, 0.336857, 0.321664, 0.30503,
   0.286614, 0.265926, 0.242205, 0.214167, 0.197833, 0.179293, 
  0.15763, 0.14513, 0.131046, 0.114725, 0.1111, 0.107324, 0.103379, 
  0.099244, 0.0948923, 0.085394, 0.0744695, 0.0682486, 
  0.0613098}, {1.12614, 1.12697, 1.12642, 1.12612, 1.12629, 1.12629, 
  1.12627, 1.12625, 1.12624, 1.12624, 1.12626, 1.12625, 1.12626, 
  1.12626, 1.12626, 1.12626, 1.12626, 1.12626, 1.12627, 1.12627, 
  1.12627, 1.12627, 1.12628, 1.12628, 1.12628, 1.12629, 1.12629, 
  1.1263, 1.12629, 1.12628, 1.12625, 1.12622, 1.12617, 1.12611, 
  1.12604, 1.12595, 1.12586, 1.12574, 1.12562, 1.12548, 1.12505, 
  1.12453, 1.12388, 1.1231, 1.12219, 1.12111, 1.11986, 1.11841, 
  1.11675, 1.11485, 1.11267, 1.1102, 1.10738, 1.10418, 1.10054, 
  1.09641, 1.0917, 1.08634, 1.08021, 1.07319, 1.06513, 1.05582, 
  1.04501, 1.03238, 1.0175, 0.999787, 0.978439, 0.952276, 0.940091, 
  0.926705, 0.911928, 0.895521, 0.877184, 0.856528, 0.833035, 0.806, 
  0.774421, 0.736806, 0.690779, 0.632177, 0.595837, 0.552533, 
  0.499038, 0.466693, 0.428905, 0.411781, 0.393199, 0.372856, 
  0.350338, 0.325043, 0.296043, 0.261768, 0.241802, 0.219141, 
  0.192661, 0.177383, 0.160169, 0.140221, 0.135789, 0.131174, 
  0.126352, 0.121299, 0.11598, 0.104371, 0.0910184, 0.083415, 
  0.0749342}, {1.3749, 1.37446, 1.37529, 1.37504, 1.37506, 1.37511, 
  1.37511, 1.37508, 1.37506, 1.37506, 1.37507, 1.37505, 1.37505, 
  1.37503, 1.37501, 1.37499, 1.37496, 1.37492, 1.37488, 1.37485, 
  1.37481, 1.37478, 1.37474, 1.37465, 1.37456, 1.37437, 1.37418, 
  1.37399, 1.37359, 1.37319, 1.37277, 1.37233, 1.37188, 1.37142, 
  1.37094, 1.37045, 1.36993, 1.36941, 1.36886, 1.36829, 1.3668, 
  1.36517, 1.36339, 1.36145, 1.35934, 1.35704, 1.35452, 1.35177, 
  1.34875, 1.34544, 1.3418, 1.33779, 1.33338, 1.3285, 1.32309, 
  1.31709, 1.3104, 1.30292, 1.29452, 1.28507, 1.27436, 1.26216, 
  1.24818, 1.23202, 1.2132, 1.19101, 1.1645, 1.13228, 1.11735, 
  1.10101, 1.08301, 1.06309, 1.04089, 1.01595, 0.987663, 0.955194, 
  0.917365, 0.872418, 0.817554, 0.747869, 0.704729, 0.653377, 
  0.590005, 0.551715, 0.507, 0.486743, 0.464764, 0.440707, 0.41408, 
  0.384173, 0.34989, 0.309375, 0.285775, 0.25899, 0.227694, 0.209637, 
  0.189292, 0.165716, 0.160479, 0.155024, 0.149326, 0.143353, 
  0.137067, 0.123347, 0.107567, 0.0985815, 0.0885587}, 
{0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 
	0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28, 0.28}};

  /* To actually calculate the ringdown, we will use the following pointers to point to the correct mode */
  const double (*reomegaqnm)[107] = NULL;
  const double (*imomegaqnm)[107] = NULL;

  REAL8 totalMass, finalMass, finalSpin;

  /* Stuff for interpolating the data */
  gsl_spline    *spline = NULL;
  gsl_interp_accel *acc = NULL;

  UINT4 i;

  /* Check we do not require more overtones than we have */
#if debugOutput
  if ( nmodes > 8 )
  {
    fprintf( stderr,"Requesting more overtones than we have data to generate!\n");
  }
#endif

  /* Choose the appropriate data bases on the user requested l an m */
  switch ( l )
  {
    case 2:
      if ( m == 2 )
      {
        reomegaqnm = reomegaqnm22;
        imomegaqnm = imomegaqnm22;
      }
      else if ( m == 1 )
      {
        reomegaqnm = reomegaqnm21;
        imomegaqnm = imomegaqnm21;
      }
      else if ( m == 0 )
      {
        reomegaqnm = reomegaqnm20;
        imomegaqnm = imomegaqnm20;
      }
      else
      {
        fprintf( stderr,"Unsupported combination of l, m (%d, %d)\n", l, m );
		return -1;
      }
      break;
    case 3:
      if ( l == 3 )
      {
        reomegaqnm = reomegaqnm33;
        imomegaqnm = imomegaqnm33;
      }
      else
      {
        fprintf( stderr,"Unsupported combination of l, m (%d, %d)\n", l, m );
        return -1;
      }
      break;
    case 4:
      if ( l == 4 )
      {
        reomegaqnm = reomegaqnm44;
        imomegaqnm = imomegaqnm44;
      }
      else
      {
        fprintf( stderr,"Unsupported combination of l, m (%d, %d)\n", l, m );
        return -1;
      }
      break;
    case 5:
      if ( l == 5 )
      {
        reomegaqnm = reomegaqnm55;
        imomegaqnm = imomegaqnm55;
      }
      else
      {
        fprintf( stderr,"Unsupported combination of l, m (%d, %d)\n", l, m );
        return -1;
      }
      break;
    default:
      fprintf( stderr,"Unsupported combination of l, m (%d, %d)\n", l, m );
      return -1;
      break;
  }

  spline = gsl_spline_alloc( gsl_interp_cspline, 107 );
  acc    = gsl_interp_accel_alloc();

  totalMass = mass1 + mass2;

  /* Call XLALSimIMREOBFinalMassSpin_re() to get mass and spin of the final black hole, locate in current file */
  if ( XLALSimIMREOBFinalMassSpin_re(&finalMass, &finalSpin, mass1, mass2, spin1, spin2) == -1 )
  {
    	return -1;
  }
#if debugOutput
  fprintf(stderr,"finalMass = %e, finalSpin = %e\n",finalMass, finalSpin);
#endif
  COMPLEX16 ci = I;
  /* Now get the QNM frequencies from interpolating the above data */
  for ( i = 0; i < nmodes; i++ )
  {
    gsl_spline_init( spline, afinallist, reomegaqnm[i], 107 );
    gsl_interp_accel_reset( acc );
    
    modefreqs->data[i] = gsl_spline_eval( spline, finalSpin, acc );

    gsl_spline_init( spline, afinallist, imomegaqnm[i], 107 );
    gsl_interp_accel_reset( acc );

    modefreqs->data[i] += ci*gsl_spline_eval( spline, finalSpin, acc );
#if debugOutput
    fprintf(stderr,"QNM(%d,%d,%d) = %e+i %e\n",l,m,i,
		  creal(modefreqs->data[i]),cimag(modefreqs->data[i]));
#endif
    /* Scale by the appropriate mass factors */
    modefreqs->data[i] *= 1./ finalMass / (totalMass * LAL_MTSUN_SI);
  }
  /* Free memory and exit */
  gsl_spline_free( spline );
  gsl_interp_accel_free( acc );

  return 0;
}
  
int XLALSimIMREOBFinalMassSpin_re(
  REAL8    *finalMass,	/**<< OUTPUT, the final mass (scaled by original total mass) */
  REAL8    *finalSpin,	/**<< OUTPUT, the final spin (scaled by final mass) */
const REAL8 	mass1,		/**<< The mass of the 1st component of the system */
const REAL8 	mass2,		/**<< The mass of the 2nd component of the system */
const REAL8 	spin1[3],	/**<< The spin of the 1st object; only needed for spin waveforms */
const REAL8 	spin2[3]	/**<< The spin of the 2nd object; only needed for spin waveforms */
)
{
static const REAL8 root9ovr8minus1 = -0.057190958417936644;
static const REAL8 root12		   = 3.4641016151377544;

/* Constants used for the spinning EOB model */
static const REAL8 p0 = 0.04826;
static const REAL8 p1 = 0.01559;
static const REAL8 p2 = 0.00485;
static const REAL8 t0 = -2.8904;
static const REAL8 t2 = -3.5171;
static const REAL8 t3 = 2.5763;
static const REAL8 s4 = -0.1229;
static const REAL8 s5 = 0.4537;

REAL8 totalMass;
REAL8 eta, eta2, eta3;
REAL8 a1, a2, chiS, q;
REAL8 tmpVar;

/* get a local copy of the intrinsic parameters */
totalMass = mass1 + mass2;
eta = mass1 * mass2 / (totalMass*totalMass);
eta2 = eta * eta;
eta3 = eta2 * eta;


	/* Final mass/spin comes from Eq. 8 of Tichy and Marronetti PRD78, 081501 
	   and from Eqs. 1 and 3 of Barausse and Rezzolla ApJ704, L40 */
a1 = spin1[2];
a2 = spin2[2];

chiS = 0.5 * ( a1 + a2 );
q	 = mass1 / mass2;

*finalMass = 1. - p0 + 2.*p1 * chiS  + 4.*p2*chiS*chiS;
*finalMass = 1. + (0.9515 - 1.0)*4.*eta - 0.013*16.*eta2*(a1+a2);
tmpVar	   = ( a1 + a2 /q/q) / ( 1. + 1/q/q);
*finalSpin = tmpVar + tmpVar * eta * (s4 * tmpVar + s5 * eta + t0 ) + eta * (2. * sqrt(3.) + t2*eta + t3*eta*eta );


/*printf( "Final mass = %e, Final spin = %e\n", *finalMass, *finalSpin );*/
return 0;
}
/**
 * Function to calculate normalized spin of the test particle in SEOBNRv1.
 * Eq. 5.3 of Barausse and Buonanno PRD 81, 084024 (2010).
 */
 int XLALSimIMRSpinEOBCalculateSigmaStar( 
                  REAL8Vector *sigmaStar, /**<< OUTPUT, normalized (to total mass) spin of test particle */
                  REAL8        mass1,     /**<< mass 1 */
                  REAL8        mass2,     /**<< mass 2 */
                  REAL8Vector *s1,        /**<< spin vector 1 */
                  REAL8Vector *s2         /**<< spin vector 2 */)
{

  UINT4 i;
  REAL8 totalMass = mass1 + mass2;

  for ( i = 0; i < 3; i++ )
  {
    sigmaStar->data[i] = (mass2/mass1 * s1->data[i] + mass1/mass2 * s2->data[i])/(totalMass*totalMass);
  }

  return 0;
}
/**
 * Function to calculate normalized spin of the deformed-Kerr background in SEOBNRv1.
 * Eq. 5.2 of Barausse and Buonanno PRD 81, 084024 (2010).
 */
 int XLALSimIMRSpinEOBCalculateSigmaKerr( 
                  REAL8Vector *sigmaKerr, /**<< OUTPUT, normalized (to total mass) spin of deformed-Kerr */
                  REAL8        mass1,     /**<< mass 1 */
                  REAL8        mass2,     /**<< mass 2 */
                  REAL8Vector *s1,        /**<< spin vector 1 */
                  REAL8Vector *s2         /**<< spin vector 2 */)
{

  UINT4 i;
  REAL8 totalMass = mass1 + mass2;

  for ( i = 0; i < 3; i++ )
  {
    sigmaKerr->data[i] = (s1->data[i] + s2->data[i])/(totalMass*totalMass);
  }

  return 0;
}
 int XLALSimIMREOBCalcSpinFacWaveformCoefficients(
          FacWaveformCoeffs * const coeffs, /**< OUTPUT, pre-computed waveform coefficients */
          const REAL8               m1,     /**< mass 1 */
          const REAL8               m2,     /**< mass 2 */
          const REAL8               eta,    /**< symmetric mass ratio */
          const REAL8               a,      /**< Kerr spin parameter for test-particle terms */
          const REAL8               chiS,   /**< (chi1+chi2)/2 */
          const REAL8               chiA    /**< (chi1-chi2)/2 */
          )
{
  REAL8 eta2 = eta*eta;
  REAL8 eta3 = eta2 * eta;

  REAL8 dM, dM2; //dM3;
  REAL8 a2, a3;

  /* Combination which appears a lot */
  REAL8 m1Plus3eta, m1Plus3eta2, m1Plus3eta3;

  dM2 = 1. - 4.*eta;
 
  //printf( "****************************** a = %e *********************************\n", a );
 
  /* Check that deltaM has a reasonable value */
  if ( dM2 < 0 )
  {
    fprintf( stderr,"XLAL Error : eta seems to be < 0.25 - this isn't allowed!\n" );
    return -1;
  }

  dM  = sqrt( dM2 );
  if (m1 < m2)
  {
    dM = -dM;
  }
  //dM3 = dM2 * dM;

  a2 = a*a;
  a3 = a2*a;

  m1Plus3eta  = - 1. + 3.*eta;
  m1Plus3eta2 = m1Plus3eta * m1Plus3eta;
  m1Plus3eta3 = m1Plus3eta * m1Plus3eta2;

  /* Initialize all coefficients to zero */
  /* This is important, as we will not set some if dM is zero */
  memset( coeffs, 0, sizeof( FacWaveformCoeffs ) );


  /* l = 2, Eqs. A8a and A8b for rho, Eq. A15a for f, 
     Eqs. 20 and 21 of DIN and Eqs. 27a and 27b of PBFRT for delta */

  coeffs->delta22vh3 = 7./3.;
  coeffs->delta22vh6 = (-4.*a)/3. + (428.*LAL_PI)/105.;
  coeffs->delta22v8 = (20.*a)/63.;
  coeffs->delta22vh9 = -2203./81. + (1712.*LAL_PI*LAL_PI)/315.;
  coeffs->delta22v5  = - 24.*eta;

  coeffs->rho22v2   = -43./42. + (55.*eta)/84.;
  coeffs->rho22v3   = (-2.*(chiS + chiA*dM - chiS*eta))/3.;
  coeffs->rho22v4   = -20555./10584. + 0.5*a2 
       - (33025.*eta)/21168. + (19583.*eta2)/42336.;
  coeffs->rho22v5   = (-34.*a)/21.;
  coeffs->rho22v6   = 1556919113./122245200. + (89.*a2)/252. - (48993925.*eta)/9779616. 
       - (6292061.*eta2)/3259872. + (10620745.*eta3)/39118464.
       + (41.*eta*LAL_PI*LAL_PI)/192.;
  coeffs->rho22v6l  = - 428./105.;
  coeffs->rho22v7   = (18733.*a)/15876. + a*a2/3.;
  coeffs->rho22v8   = eta*(-5.6-117.6*eta) -387216563023./160190110080. + (18353.*a2)/21168. - a2*a2/8.;
  coeffs->rho22v8l  =  9202./2205.;
  coeffs->rho22v10  = -16094530514677./533967033600.;
  coeffs->rho22v10l =  439877./55566.;

  /*printf( "v2 = %.16e, v3 = %.16e, v4 = %.16e, v5 = %.16e\n"
    "v6 = %.16e, v6l = %.16e v7 = %.16e v8 = %.16e, v8l = %.16e v10 = %.16e v10l = %.16e\n",
      coeffs->rho22v2, coeffs->rho22v3, coeffs->rho22v4, coeffs->rho22v5, coeffs->rho22v6,
      coeffs->rho22v6l, coeffs->rho22v7, coeffs->rho22v8, coeffs->rho22v8l, coeffs->rho22v10,
      coeffs->rho22v10l );*/

  if ( dM2 )
  {
    coeffs->delta21vh3 = 2./3.;
    coeffs->delta21vh6 = (-17.*a)/35. + (107.*LAL_PI)/105.;
    coeffs->delta21vh7 = (3.*a2)/140.;
    coeffs->delta21vh9 = -272./81. + (214.*LAL_PI*LAL_PI)/315.;
    coeffs->delta21v5  = - 493. * eta /42.;

    //coeffs->rho21v1   = (-3.*(chiS+chiA/dM))/(4.);
    coeffs->rho21v1   = 0.0;
    //coeffs->rho21v2   = -59./56 - (9.*chiAPlusChiSdM*chiAPlusChiSdM)/(32.*dM2) + (23.*eta)/84.;
    coeffs->rho21v2   = -59./56 + (23.*eta)/84. - 9./32.*a2;
    /*coeffs->rho21v3   = (-567.*chiA*chiA*chiA - 1701.*chiA*chiA*chiS*dM
                        + chiA*(-4708. + 1701.*chiS*chiS - 2648.*eta)*(-1. + 4.*eta)
                        + chiS* dM3 *(4708. - 567.*chiS*chiS
                        + 1816.*eta))/(2688.*dM3);*/
    coeffs->rho21v3   = 1177./672.*a - 27./128.*a3;
    coeffs->rho21v4   = -47009./56448.- (865.*a2)/1792. - (405.*a2*a2)/2048. - (10993.*eta)/14112.
                        + (617.*eta2)/4704.;
    coeffs->rho21v5   = (-98635.*a)/75264. + (2031.*a*a2)/7168. - (1701.*a2*a3)/8192.;
    coeffs->rho21v6   = 7613184941./2607897600.+ (9032393.*a2)/1806336. + (3897.*a2*a2)/16384.
                        - (15309.*a3*a3)/65536.; 
    coeffs->rho21v6l  = - 107./105.;
    coeffs->rho21v7   = (-3859374457.*a)/1159065600. - (55169.*a3)/16384.
                        + (18603.*a2*a3)/65536. - (72171.*a2*a2*a3)/262144.;
    coeffs->rho21v7l  =  107.*a/140.;
    coeffs->rho21v8   = -1168617463883./911303737344.;
    coeffs->rho21v8l  = 6313./5880.;
    coeffs->rho21v10  = -63735873771463./16569158860800.; 
    coeffs->rho21v10l = 5029963./5927040.;

    coeffs->f21v1     = (-3.*(chiS+chiA/dM))/(2.);
  }
  else
  {
    coeffs->f21v1     = -3.*chiA/2.;
  }

  /* l = 3, Eqs. A9a - A9c for rho, Eqs. A15b and A15c for f,
     Eqs. 22 - 24 of DIN and Eqs. 27c - 27e of PBFRT for delta */
  if ( dM2 )
  {
    coeffs->delta33vh3 = 13./10.;
    coeffs->delta33vh6 = (-81.*a)/20. + (39.*LAL_PI)/7.;
    coeffs->delta33vh9 = -227827./3000. + (78.*LAL_PI*LAL_PI)/7.;
    coeffs->delta33v5  = - 80897.*eta / 2430.;

    coeffs->rho33v2 = -7./6. + (2.*eta)/3.;
    //coeffs->rho33v3 = (chiS*dM*(-4. + 5.*eta) + chiA*(-4. + 19.*eta))/(6.*dM);
    coeffs->rho33v3 = 0.0;
    coeffs->rho33v4 = -6719./3960. + a2/2. - (1861.*eta)/990. + (149.*eta2)/330.;
    coeffs->rho33v5 = (-4.*a)/3.;
    coeffs->rho33v6 = 3203101567./227026800. + (5.*a2)/36.;
    coeffs->rho33v6l = - 26./7.;
    coeffs->rho33v7 = (5297.*a)/2970. + a*a2/3.;
    coeffs->rho33v8 = -57566572157./8562153600.;
    coeffs->rho33v8l = 13./3.;

    coeffs->f33v3 = (chiS*dM*(-4. + 5.*eta) + chiA*(-4. + 19.*eta))/(2.*dM);
  }
  else
  {
    coeffs->f33v3 = chiA * 3./8.;
  }

  coeffs->delta32vh3 = (10. + 33.*eta)/(-15.*m1Plus3eta);
  coeffs->delta32vh4 = 4.*a;
  coeffs->delta32vh6 = (-136.*a)/45. + (52.*LAL_PI)/21.;
  coeffs->delta32vh9 = -9112./405. + (208.*LAL_PI*LAL_PI)/63.;

  coeffs->rho32v   = (4.*chiS*eta)/(-3.*m1Plus3eta);
  coeffs->rho32v2  = (-4.*a2*eta2)/(9.*m1Plus3eta2) + (328. - 1115.*eta
                        + 320.*eta2)/(270.*m1Plus3eta);
  coeffs->rho32v3  = (2.*(45.*a*m1Plus3eta3
                        - a*eta*(328. - 2099.*eta + 5.*(733. + 20.*a2)*eta2
                        - 960.*eta3)))/(405.*m1Plus3eta3);
  coeffs->rho32v4  = a2/3. + (-1444528.
                        + 8050045.*eta - 4725605.*eta2 - 20338960.*eta3
                        + 3085640.*eta2*eta2)/(1603800.*m1Plus3eta2);
  coeffs->rho32v5  = (-2788.*a)/1215.;
  coeffs->rho32v6  = 5849948554./940355325. + (488.*a2)/405.;
  coeffs->rho32v6l =  - 104./63.;
  coeffs->rho32v8  = -10607269449358./3072140846775.;
  coeffs->rho32v8l = 17056./8505.;

  if ( dM2 )
  {
    coeffs->delta31vh3 = 13./30.;
    coeffs->delta31vh6 = (61.*a)/20. + (13.*LAL_PI)/21.;
    coeffs->delta31vh7 = (-24.*a2)/5.;
    coeffs->delta31vh9 = -227827./81000. + (26.*LAL_PI*LAL_PI)/63.;
    coeffs->delta31v5  = - 17.*eta/10.; 
 
    coeffs->rho31v2  = -13./18. - (2.*eta)/9.;
    //coeffs->rho31v3  = (chiA*(-4. + 11.*eta) + chiS*dM*(-4. + 13.*eta))/(6.*dM);
    coeffs->rho31v3  = 0.0;
    coeffs->rho31v4  = 101./7128.
                        - (5.*a2)/6. - (1685.*eta)/1782. - (829.*eta2)/1782.;
    coeffs->rho31v5  = (4.*a)/9.;
    coeffs->rho31v6  = 11706720301./6129723600. - (49.*a2)/108.;
    coeffs->rho31v6l =  - 26./63.;
    coeffs->rho31v7  = (-2579.*a)/5346. + a*a2/9.;
    coeffs->rho31v8  = 2606097992581./4854741091200.;
    coeffs->rho31v8l = 169./567.;

    coeffs->f31v3  = (chiA*(-4. + 11.*eta) + chiS*dM*(-4. + 13.*eta))/(2.*dM);
  }
  else
  {
    coeffs->f31v3  = - chiA * 5./8.;
  }

  /* l = 4, Eqs. A10a - A10d for delta, Eq. A15d for f
     Eqs. 25 - 28 of DIN and Eqs. 27f - 27i of PBFRT for delta */
  
  coeffs->delta44vh3 = (112. + 219.*eta)/(-120.*m1Plus3eta);
  coeffs->delta44vh6 = (-464.*a)/75. + (25136.*LAL_PI)/3465.;

  coeffs->rho44v2 = (1614. - 5870.*eta + 2625.*eta2)/(1320.*m1Plus3eta);
  coeffs->rho44v3 = (chiA*(10. - 39.*eta)*dM + chiS*(10. - 41.*eta
                        + 42.*eta2))/(15.*m1Plus3eta);
  coeffs->rho44v4 = a2/2. + (-511573572.
                        + 2338945704.*eta - 313857376.*eta2 - 6733146000.*eta3
                        + 1252563795.*eta2*eta2)/(317116800.*m1Plus3eta2);
  coeffs->rho44v5 = (-69.*a)/55.;
  coeffs->rho44v6 = 16600939332793./1098809712000. + (217.*a2)/3960.;
  coeffs->rho44v6l = - 12568./3465.;

  if ( dM2 )
  {
    coeffs->delta43vh3 = (486. + 4961.*eta)/(810.*(1. - 2.*eta));
    coeffs->delta43vh4 = (11.*a)/4.;
    coeffs->delta43vh6 = 1571.*LAL_PI/385.;

    //coeffs->rho43v   = (5.*(chiA - chiS*dM)*eta)/(8.*dM*(-1. + 2.*eta));
    coeffs->rho43v   = 0.0;
    coeffs->rho43v2  = (222. - 547.*eta + 160.*eta2)/(176.*(-1. + 2.*eta));
    coeffs->rho43v4  = -6894273./7047040. + (3.*a2)/8.;
    coeffs->rho43v5  = (-12113.*a)/6160.;
    coeffs->rho43v6  = 1664224207351./195343948800.;
    coeffs->rho43v6l = - 1571./770.;

    coeffs->f43v   = (5.*(chiA - chiS*dM)*eta)/(2.*dM*(-1. + 2.*eta));
  }
  else
  {
    coeffs->f43v   = - 5.*chiA/4.;
  }

  coeffs->delta42vh3 = (7.*(1. + 6.*eta))/(-15.*m1Plus3eta);
  coeffs->delta42vh6 = (212.*a)/75. + (6284.*LAL_PI)/3465.;

  coeffs->rho42v2  = (1146. - 3530.*eta + 285.*eta2)/(1320.*m1Plus3eta);
  coeffs->rho42v3  = (chiA*(10. - 21.*eta)*dM + chiS*(10. - 59.*eta
                        + 78.*eta2))/(15.*m1Plus3eta);
  coeffs->rho42v4  = a2/2. + (-114859044. + 295834536.*eta + 1204388696.*eta2 - 3047981160.*eta3
                        - 379526805.*eta2*eta2)/(317116800.*m1Plus3eta2);
  coeffs->rho42v5  = (-7.*a)/110.;
  coeffs->rho42v6  = 848238724511./219761942400. + (2323.*a2)/3960.;
  coeffs->rho42v6l = - 3142./3465.;

  if ( dM2 )
  {
    coeffs->delta41vh3 = (2. + 507.*eta)/(10.*(1. - 2.*eta));
    coeffs->delta41vh4 = (11.*a)/12.;
    coeffs->delta41vh6 = 1571.*LAL_PI/3465.;

    //coeffs->rho41v   = (5.*(chiA - chiS*dM)*eta)/(8.*dM*(-1. + 2.*eta));
    coeffs->rho41v   = 0.0;
    coeffs->rho41v2  = (602. - 1385.*eta + 288.*eta2)/(528.*(-1. + 2.*eta));
    coeffs->rho41v4  = -7775491./21141120. + (3.*a2)/8.;
    coeffs->rho41v5  = (-20033.*a)/55440. - (5*a*a2)/6.;
    coeffs->rho41v6  = 1227423222031./1758095539200.;
    coeffs->rho41v6l = - 1571./6930.;

    coeffs->f41v = (5.*(chiA - chiS*dM)*eta)/(2.*dM*(-1. + 2.*eta));
  }
  else
  {
    coeffs->f41v = - 5.*chiA/4.;
  }

  /* l = 5, Eqs. A11a - A11e for rho, 
     Eq. 29 of DIN and Eqs. E1a and E1b of PBFRT for delta */
  if ( dM2 )
  {
    coeffs->delta55vh3 = (96875. + 857528.*eta)/(131250.*(1 - 2*eta));

    coeffs->rho55v2 = (487. - 1298.*eta + 512.*eta2)/(390.*(-1. + 2.*eta));
    coeffs->rho55v3 = (-2.*a)/3.;
    coeffs->rho55v4 = -3353747./2129400. + a2/2.;
    coeffs->rho55v5 = - 241. * a / 195.;
  }

  coeffs->delta54vh3 = 8./15.;
  coeffs->delta54vh4 = 12.*a/5.;

  coeffs->rho54v2 = (-17448. + 96019.*eta - 127610.*eta2
                        + 33320.*eta3)/(13650.*(1. - 5.*eta + 5.*eta2));
  coeffs->rho54v3 = (-2.*a)/15.;
  coeffs->rho54v4 = -16213384./15526875. + (2.*a2)/5.;

  if ( dM2 )
  {
    coeffs->delta53vh3 = 31./70.;

    coeffs->rho53v2 = (375. - 850.*eta + 176.*eta2)/(390.*(-1. + 2.*eta));
    coeffs->rho53v3 = (-2.*a)/3.;
    coeffs->rho53v4 = -410833./709800. + a2/2.;
    coeffs->rho53v5 = - 103.*a/325.;
  }

  coeffs->delta52vh3 = 4./15.;
  coeffs->delta52vh4 = 6.*a/5.;

  coeffs->rho52v2 = (-15828. + 84679.*eta - 104930.*eta2
                        + 21980.*eta3)/(13650.*(1. - 5.*eta + 5.*eta2));
  coeffs->rho52v3 = (-2.*a)/15.;
  coeffs->rho52v4 = -7187914./15526875. + (2.*a2)/5.;

  if ( dM2 )
  {
    coeffs->delta51vh3 = 31./210.;

    coeffs->rho51v2 = (319. - 626.*eta + 8.*eta2)/(390.*(-1. + 2.*eta));
    coeffs->rho51v3 = (-2.*a)/3.;
    coeffs->rho51v4 = -31877./304200. + a2/2.;
    coeffs->rho51v5 = 139.*a/975.;
  }

  /* l = 6, Eqs. A12a - A12f for rho, Eqs. E1c and E1d of PBFRT for delta */

  coeffs->delta66vh3 = 43./70.;
  
  coeffs->rho66v2 = (-106. + 602.*eta - 861.*eta2
                        + 273.*eta3)/(84.*(1. - 5.*eta + 5.*eta2));
  coeffs->rho66v3 = (-2.*a)/3.;
  coeffs->rho66v4 = -1025435./659736. + a2/2.;

  if ( dM2 )
  {
    coeffs->delta65vh3 = 10./21.;
    
    coeffs->rho65v2 = (-185. + 838.*eta - 910.*eta2
                        + 220.*eta3)/(144.*(dM2 + 3.*eta2));
    coeffs->rho65v3 = - 2.*a/9.;
  }

  coeffs->delta64vh3 = 43./105.;
  
  coeffs->rho64v2 = (-86. + 462.*eta - 581.*eta2
                        + 133.*eta3)/(84.*(1. - 5.*eta + 5.*eta2));
  coeffs->rho64v3 = (-2.*a)/3.;
  coeffs->rho64v4 = -476887./659736. + a2/2.;

  if ( dM2 )
  {
    coeffs->delta63vh3 = 2./7.;

    coeffs->rho63v2 = (-169. + 742.*eta - 750.*eta2
                        + 156.*eta3)/(144.*(dM2 + 3.*eta2));
    coeffs->rho63v3 = - 2.*a/9.;
  }

  coeffs->delta62vh3 = 43./210.;

  coeffs->rho62v2 = (-74. + 378.*eta - 413.*eta2
                        + 49.*eta3)/(84.*(1. - 5.*eta + 5.*eta2));
  coeffs->rho62v3 = (-2.*a)/3.;
  coeffs->rho62v4 = -817991./3298680. + a2/2.;

  if ( dM2 )
  {
    coeffs->delta61vh3 = 2./21.;

    coeffs->rho61v2 = (-161. + 694.*eta - 670.*eta2
                        + 124.*eta3)/(144.*(dM2 + 3.*eta2));
    coeffs->rho61v3 = - 2. * a / 9.;
  }

  /* l = 7, Eqs. A13a - A13g for rho, Eqs. E1e and E1f of PBFRT for delta */
  if ( dM2 )
  {
    coeffs->delta77vh3 = 19./36.;

    coeffs->rho77v2 = (-906. + 4246.*eta - 4963.*eta2
                        + 1380.*eta3)/(714.*(dM2 + 3.*eta2));
    coeffs->rho77v3 = - 2.*a/3.;
  }

  coeffs->rho76v2 = (2144. - 16185.*eta + 37828.*eta2 - 29351.*eta3
                        + 6104.*eta2*eta2) / (1666.*(-1 + 7*eta - 14*eta2
                        + 7*eta3));

  if ( dM2 )
  {
    coeffs->delta75vh3 = 95./252.;

    coeffs->rho75v2 = (-762. + 3382.*eta - 3523.*eta2
                        + 804.*eta3)/(714.*(dM2 + 3.*eta2));
    coeffs->rho75v3 = - 2.*a/3.;
  }

  coeffs->rho74v2 = (17756. - 131805.*eta + 298872.*eta2 - 217959.*eta3
                        + 41076.*eta2*eta2) / (14994.*(-1. + 7.*eta - 14.*eta2
                        + 7.*eta3));

  if ( dM2 )
  {
    coeffs->delta73vh3 = 19./84.;

    coeffs->rho73v2 = (-666. + 2806.*eta - 2563.*eta2
                        + 420.*eta3)/(714.*(dM2 + 3.*eta2));
    coeffs->rho73v3 = - 2.*a/3.;
  }

  coeffs->rho72v2 = (16832. - 123489.*eta + 273924.*eta2 - 190239.*eta3
                        + 32760.*eta2*eta2) /(14994.*(-1. + 7.*eta - 14.*eta2
                        + 7.*eta3));

  if ( dM2 )
  {
    coeffs->delta71vh3 = 19./252.;

    coeffs->rho71v2 = (-618. + 2518.*eta - 2083.*eta2
                        + 228.*eta3)/(714.*(dM2 + 3.*eta2));
    coeffs->rho71v3 = - 2.*a/3.;
  }

  /* l = 8, Eqs. A14a - A14h */
  
  coeffs->rho88v2 = (3482. - 26778.*eta + 64659.*eta2 - 53445.*eta3
                        + 12243.*eta2*eta2) / (2736.*(-1. + 7.*eta - 14.*eta2
                        + 7.*eta3));

  if ( dM2 )
  {
    coeffs->rho87v2 = (23478. - 154099.*eta + 309498.*eta2 - 207550.*eta3
                        + 38920*eta2*eta2) / (18240.*(-1 + 6*eta - 10*eta2
                        + 4*eta3));
  }

  coeffs->rho86v2 = (1002. - 7498.*eta + 17269.*eta2 - 13055.*eta3
                        + 2653.*eta2*eta2) / (912.*(-1. + 7.*eta - 14.*eta2
                        + 7.*eta3));

  if ( dM2 )
  {
    coeffs->rho85v2 = (4350. - 28055.*eta + 54642.*eta2 - 34598.*eta3
                        + 6056.*eta2*eta2) / (3648.*(-1. + 6.*eta - 10.*eta2
                        + 4.*eta3));
  }

  coeffs->rho84v2 = (2666. - 19434.*eta + 42627.*eta2 - 28965.*eta3
                        + 4899.*eta2*eta2) / (2736.*(-1. + 7.*eta - 14.*eta2
                        + 7.*eta3));

  if ( dM2 )
  {
    coeffs->rho83v2 = (20598. - 131059.*eta + 249018.*eta2 - 149950.*eta3
                        + 24520.*eta2*eta2) / (18240.*(-1. + 6.*eta - 10.*eta2
                        + 4.*eta3));
  }

  coeffs->rho82v2 = (2462. - 17598.*eta + 37119.*eta2 - 22845.*eta3
                        + 3063.*eta2*eta2) / (2736.*(-1. + 7.*eta - 14.*eta2
                        + 7.*eta3));

  if ( dM2 )
  {
    coeffs->rho81v2 = (20022. - 126451.*eta + 236922.*eta2 - 138430.*eta3
                        + 21640.*eta2*eta2) / (18240.*(-1. + 6.*eta - 10.*eta2
                        + 4.*eta3));
  }

  /* All relevant coefficients should be set, so we return */

  return 0;
}

 int XLALSimIMREOBComputeNewtonMultipolePrefixes(
                NewtonMultipolePrefixes *prefix, /**<< OUTPUT Structure containing the coeffs */
                const REAL8             m1,      /**<< Mass of first component */
                const REAL8             m2       /**<< Nass of second component */
                )
{

  int l, m;

  memset( prefix, 0, sizeof( NewtonMultipolePrefixes ) );

  for ( l = 2; l <= 8; l++ )
  {
    for ( m = 1; m <= l; m++ )
    {
      CalculateThisMultipolePrefix( &(prefix->values[l][m]), m1, m2, l, m );
    }
  }
  return 0;
}
/**
 * Function to calculate the numerical prefix in the Newtonian amplitude. Eqs. 5 - 7.
 */
 int
CalculateThisMultipolePrefix(
                 COMPLEX16 *prefix, /**<< OUTPUT, Prefix value */
                 const REAL8 m1,    /**<< mass 1 */
                 const REAL8 m2,    /**<< mass 2 */
                 int l,      /**<< Mode l */
                 int m       /**<< Mode m */
                 )
{
   COMPLEX16 n;
   REAL8 c;

   REAL8 x1, x2; /* Scaled versions of component masses */

   REAL8 mult1, mult2;

   REAL8 totalMass;
   REAL8 eta;

   int epsilon;
   int sign; /* To give the sign of some additive terms */
   COMPLEX16 ci = I;


   //n = 0.0;

   totalMass = m1 + m2;
 
   epsilon = ( l + m )  % 2;
 
   x1 = m1 / totalMass;
   x2 = m2 / totalMass;

   eta = m1*m2/(totalMass*totalMass);

   if  ( abs( m % 2 ) == 0 )
   {
     sign = 1;
   }
   else
   {
     sign = -1;
   }
   /** 
    * Eq. 7 of Damour, Iyer and Nagar 2008. 
    * For odd m, c is proportional to dM = m1-m2. In the equal-mass case, c = dM = 0. 
    * In the equal-mass unequal-spin case, however, when spins are different, the odd m term is generally not zero.
    * In this case, c can be written as c0 * dM, while spins terms in PN expansion may take the form chiA/dM.
    * Although the dM's cancel analytically, we can not implement c and chiA/dM with the possibility of dM -> 0.
    * Therefore, for this case, we give numerical values of c0 for relevant modes, and c0 is calculated as
    * c / dM in the limit of dM -> 0. Consistently, for this case, we implement chiA instead of chiA/dM
    * in LALSimIMRSpinEOBFactorizedWaveform.c. 
    */
   if  ( m1 != m2 || sign == 1 )
   {
     // Eq.(B8) of Y Pan, et al, PRD 84, 124052 (2011)	   
     c = pow( x2, l + epsilon - 1 ) + sign * pow(x1, l + epsilon - 1 );
   }
   else
   {
     switch( l )
     {
       case 2:
         c = -1.0;
         break;
       case 3:
         c = -1.0;
         break;
       case 4:
         c = -0.5;
         break;
       default:
         c = 0.0;
         break;
     }
   }
  
   /* Eqs 5 and 6. Dependent on the value of epsilon (parity), we get different n */
   if ( epsilon == 0 )
   {
     // Eq.(B7a) of Y Pan et al, PRD 84, 124052 (2011) 
     n = ci * (double)m;
    
     n = MYcpow( n, l );
  
     mult1 = 8.0 * LAL_PI / gsl_sf_doublefact(2u*l + 1u);  // 1u means unsigned int
     mult2 = (REAL8)((l+1) * (l+2)) / (REAL8)(l * ((int)l - 1));
     mult2 = sqrt(mult2);

     n *= mult1;
     n *= mult2;
  }
  else if ( epsilon == 1 )
  {
     // Eq.(B7b) of Y Pan et al, PRD 84, 124052 (2011)   
     n = ci * (double)m;
     n = MYcpow( n,l );
     n = -n;

     mult1 = 16.*LAL_PI / gsl_sf_doublefact( 2u*l + 1u );

     mult2  = (REAL8)( (2*l + 1) * (l+2) * (l*l - m*m) );
     mult2 /= (REAL8)( (2*l - 1) * (l+1) * l * (l-1) );
     mult2  = sqrt(mult2);

     n *= ci * mult1;
     n *= mult2;
  }
  else
  {
    fprintf( stderr,"Epsilon must be 0 or 1.\n");
    return -1;
  }

  *prefix = n * eta * c;

  return 0;
}
 int XLALSimIMRSpinEOBInitialConditions(
                      REAL8Vector   *initConds, /**<< OUTPUT, Initial dynamical variables */
                      const REAL8    mass1,     /**<< mass 1 */
                      const REAL8    mass2,     /**<< mass 2 */
                      const REAL8    fMin,      /**<< Initial frequency (given) */
                      const REAL8    e0,        /**<< eccentricity at starting GW frequency (Hz) */
                      const REAL8    inc,       /**<< Inclination */
                      const REAL8    spin1[],   /**<< Initial spin vector 1 */
                      const REAL8    spin2[],   /**<< Initial spin vector 2 */
                      SpinEOBParams *params     /**<< Spin EOB parameters */
                      )
{
#if debugOutput
  fprintf(stderr, "input parameters:\n");
  fprintf( stderr,"(m1,m2)=(%.16e,%.16e)\n(fMin,inc)=(%.16e,%.16e)\n(S1x,S1y,S1z)=(%.16e,%.16e,%.16e)\n(S2x,S2y,S2z)=(%.16e,%.16e,%.16e)\n", mass1, mass2, fMin,inc,
          spin1[0], spin1[1], spin1[2], spin2[0], spin2[1], spin2[2]);
#endif

  static const int lMax = 8;

  int i;

  /* Variable to keep track of whether the user requested the tortoise */
  int tmpTortoise;

  REAL8 mTotal;
  REAL8 eta;
  REAL8 omega, v0;   /* Initial velocity and angular frequency */

  REAL8 ham;      /* Hamiltonian */

  REAL8 LnHat[3]; /* Initial orientation of angular momentum */
  REAL8 rHat[3];  /* Initial orientation of radial vector */
  REAL8 vHat[3];  /* Initial orientation of velocity vector */
  REAL8 Lhat[3];  /* Direction of relativistic ang mom */
  REAL8 qHat[3];
  REAL8 pHat[3];

  /* q and p vectors in Cartesian and spherical coords */
  REAL8 qCart[3], pCart[3];
  REAL8 qSph[3], pSph[3];

  /* We will need to manipulate the spin vectors */
  /* We will use temporary vectors to do this */
  REAL8 tmpS1[3];
  REAL8 tmpS2[3];

  REAL8Vector qCartVec, pCartVec;
  REAL8Vector s1Vec, s2Vec;
  REAL8Vector sKerr, sStar;
  REAL8       sKerrData[3], sStarData[3];
  REAL8       a = 0., chiS, chiA;
  REAL8       chi1, chi2;

  /* We will need a full values vector for calculating derivs of Hamiltonian */
  REAL8 sphValues[12];
  REAL8 cartValues[12];

  /* Matrices for rotating to the new basis set. */
  /* It is more convenient to calculate the ICs in a simpler basis */
  gsl_matrix *rotMatrix  = NULL;
  gsl_matrix *invMatrix  = NULL;
  gsl_matrix *rotMatrix2 = NULL;
  gsl_matrix *invMatrix2 = NULL;

  /* Root finding stuff for finding the spherical orbit */
  SEOBRootParams rootParams;
  const gsl_multiroot_fsolver_type *T = gsl_multiroot_fsolver_hybrids;
  gsl_multiroot_fsolver *rootSolver = NULL;

  gsl_multiroot_function rootFunction;
  gsl_vector *initValues  = NULL;
  gsl_vector *finalValues = NULL;
  int gslStatus;
  const int maxIter = 100;

  memset( &rootParams, 0, sizeof( rootParams ) );

  memcpy( tmpS1, spin1, sizeof(tmpS1) );
  memcpy( tmpS2, spin2, sizeof(tmpS2) );

  mTotal = mass1 + mass2;
  eta    = mass1 * mass2 / (mTotal * mTotal);

  /* We compute the ICs for the non-tortoise p, and convert at the end */
  tmpTortoise      = params->tortoise;
  params->tortoise = 0;

  /* STEP 1) Rotate to LNhat0 along z-axis and N0 along x-axis frame, where LNhat0 and N0 are initial normal to 
   *         orbital plane and initial orbital separation;
   */
 
  /* Set the initial orbital ang mom direction. Taken from STPN code */
  LnHat[0] = sin(inc);
  LnHat[1] = 0.;
  LnHat[2] = cos(inc);

  /* Set the radial direction - need to take care to avoid singularity if L is along z axis */
  if ( LnHat[2] > 0.9999 )
  {
    rHat[0] = 1.;
    rHat[1] = rHat[2] = 0.;
  }
  else
  {
    REAL8 theta0 = atan( - LnHat[2] / LnHat[0] ); /* theta0 is between 0 and Pi */
    rHat[0] = sin( theta0 );
    rHat[1] = 0;
    rHat[2] = cos( theta0 );
  }

  /* Now we can complete the triad */
  vHat[0] = CalculateCrossProduct( 0, LnHat, rHat );
  vHat[1] = CalculateCrossProduct( 1, LnHat, rHat );
  vHat[2] = CalculateCrossProduct( 2, LnHat, rHat );

  NormalizeVector( vHat );

  /* XXX Test code XXX */
  /*for ( i = 0; i < 3; i++ )
  {
    printf ( " LnHat[%d] = %.16e, rHat[%d] = %.16e, vHat[%d] = %.16e\n", i, LnHat[i], i, rHat[i], i, vHat[i] );
  }

  printf("\n\n" );
  for ( i = 0; i < 3; i++ )
  {
    printf ( " s1[%d] = %.16e, s2[%d] = %.16e\n", i, tmpS1[i], i, tmpS2[i] );
  }*/

  /* Allocate and compute the rotation matrices */
  rotMatrix = gsl_matrix_alloc( 3, 3 );
  invMatrix = gsl_matrix_alloc( 3, 3 );
  if ( !rotMatrix || !invMatrix )
  {
    if ( rotMatrix ) gsl_matrix_free( rotMatrix );
    if ( invMatrix ) gsl_matrix_free( invMatrix );
    return -1;
  }

  if ( CalculateRotationMatrix( rotMatrix, invMatrix, rHat, vHat, LnHat ) == -1 )
  {
    gsl_matrix_free( rotMatrix );
    gsl_matrix_free( invMatrix );
    return -1;
  }

  /* Rotate the orbital vectors and spins */
  ApplyRotationMatrix( rotMatrix, rHat );
  ApplyRotationMatrix( rotMatrix, vHat );
  ApplyRotationMatrix( rotMatrix, LnHat );
  ApplyRotationMatrix( rotMatrix, tmpS1 );
  ApplyRotationMatrix( rotMatrix, tmpS2 );

  /* XXX Test code XXX */
  /*printf( "\nAfter applying rotation matrix:\n\n" );
  for ( i = 0; i < 3; i++ )
  {
    printf ( " LnHat[%d] = %.16e, rHat[%d] = %.16e, vHat[%d] = %.16e\n", i, LnHat[i], i, rHat[i], i, vHat[i] );
  }

  printf("\n\n" );
  for ( i = 0; i < 3; i++ )
  {
    printf ( " s1[%d] = %.16e, s2[%d] = %.16e\n", i, tmpS1[i], i, tmpS2[i] );
  }*/

  /* STEP 2) After rotation in STEP 1, in spherical coordinates, phi0 and theta0 are given directly in Eq. (4.7) of PRD 74, 104005,
   *         r0, pr0, ptheta0 and pphi0 are obtained by solving Eqs. (4.8) and (4.9) of PRD 74, 104005 (using gsl_multiroot_fsolver).
   *         At this step, we find initial conditions for a spherical orbit without radiation reaction.
   */

  /* Calculate the initial velocity from the given initial frequency */
  omega = LAL_PI * mTotal * LAL_MTSUN_SI * fMin;
  v0    = MYcbrt( omega );
  /* Given this, we can start to calculate the initial conditions */
  /* for spherical coords in the new basis */
  rootParams.omega  = omega;
  rootParams.params = params;
  
  /* To start with, we will just assign Newtonian-ish ICs to the system */
  rootParams.values[0] = 1./(v0*v0);  /* Initial r */
  rootParams.values[4] = v0;    /* Initial p */
  memcpy( rootParams.values+6, tmpS1, sizeof( tmpS1 ) );
  memcpy( rootParams.values+9, tmpS2, sizeof( tmpS2 ) );

  //printf( "ICs guess: r = %.16e, p = %.16e\n", rootParams.values[0], rootParams.values[4] );

  /* Initialise the gsl stuff */
  rootSolver = gsl_multiroot_fsolver_alloc( T, 3 );
  if ( !rootSolver )
  {
    gsl_matrix_free( rotMatrix );
    gsl_matrix_free( invMatrix );
    return -1;
  }

  initValues = gsl_vector_calloc( 3 );
  if ( !initValues )
  {
    gsl_multiroot_fsolver_free( rootSolver );
    gsl_matrix_free( rotMatrix );
    gsl_matrix_free( invMatrix );
    return -1;
  }

  gsl_vector_set( initValues, 0, rootParams.values[0] );
  gsl_vector_set( initValues, 1, rootParams.values[4] );

  rootFunction.f      = XLALFindSphericalOrbit;
  rootFunction.n      = 3;
  rootFunction.params = &rootParams;

  gsl_multiroot_fsolver_set( rootSolver, &rootFunction, initValues );

  /* We are now ready to iterate to find the solution */
  i = 0;

  do
  {
    gslStatus = gsl_multiroot_fsolver_iterate( rootSolver ) ; 
    if ( gslStatus != GSL_SUCCESS )
    {
      fprintf(stderr, "Error in GSL iteration function!\n" );
      gsl_multiroot_fsolver_free( rootSolver );
      gsl_vector_free( initValues );
      gsl_matrix_free( rotMatrix );
      gsl_matrix_free( invMatrix );
      return -1;
    }

    gslStatus = gsl_multiroot_test_residual( rootSolver->f, 1.0e-10 );
    i++;
  }
  while ( gslStatus == GSL_CONTINUE && i <= maxIter );

  if ( i > maxIter && gslStatus != GSL_SUCCESS )
  {
    gsl_multiroot_fsolver_free( rootSolver );
    gsl_vector_free( initValues );
    gsl_matrix_free( rotMatrix );
    gsl_matrix_free( invMatrix );
    return -1;
  }

  finalValues = gsl_multiroot_fsolver_root( rootSolver );

  /*printf( "Spherical orbit conditions here given by the following:\n" );
  printf( " x = %.16e, py = %.16e, pz = %.16e\n", gsl_vector_get( finalValues, 0 ), 
      gsl_vector_get( finalValues, 1 ), gsl_vector_get( finalValues, 2 ) );*/

  memset( qCart, 0, sizeof(qCart) );
  memset( pCart, 0, sizeof(pCart) );

  qCart[0] = gsl_vector_get( finalValues, 0 );
  pCart[1] = gsl_vector_get( finalValues, 1 );
  pCart[2] = gsl_vector_get( finalValues, 2 );

  /* Free the GSL root finder, since we're done with it */
  gsl_multiroot_fsolver_free( rootSolver );
  gsl_vector_free( initValues );

  /* STEP 3) Rotate to L0 along z-axis and N0 along x-axis frame, where L0 is the initial orbital angular momentum
   *         and L0 is calculated using initial position and linear momentum obtained in STEP 2.
   */

  /* Now we can calculate the relativistic L and rotate to a new basis */
  memcpy( qHat, qCart, sizeof(qCart) );
  memcpy( pHat, pCart, sizeof(pCart) );

  NormalizeVector( qHat );
  NormalizeVector( pHat );

  Lhat[0] = CalculateCrossProduct( 0, qHat, pHat );
  Lhat[1] = CalculateCrossProduct( 1, qHat, pHat );
  Lhat[2] = CalculateCrossProduct( 2, qHat, pHat );  

  NormalizeVector( Lhat );

  rotMatrix2 = gsl_matrix_alloc( 3, 3 );
  invMatrix2 = gsl_matrix_alloc( 3, 3 );

  if ( CalculateRotationMatrix( rotMatrix2, invMatrix2, qHat, pHat, Lhat ) == -1 )
  {
    gsl_matrix_free( rotMatrix );
    gsl_matrix_free( invMatrix );
    return -1;
  }

  ApplyRotationMatrix( rotMatrix2, rHat );
  ApplyRotationMatrix( rotMatrix2, vHat );
  ApplyRotationMatrix( rotMatrix2, LnHat );
  ApplyRotationMatrix( rotMatrix2, tmpS1 );
  ApplyRotationMatrix( rotMatrix2, tmpS2 );
  ApplyRotationMatrix( rotMatrix2, qCart );
  ApplyRotationMatrix( rotMatrix2, pCart );

  /* STEP 4) In the L0-N0 frame, we calculate (dE/dr)|sph using Eq. (4.14) of PRD 74, 104005, then initial dr/dt using Eq. (4.10) of PRD 74, 104005,
   *         and finally pr0 using Eq. (4.15) of PRD 74, 104005.
   */

  /* Now we can calculate the flux. Change to spherical co-ords */
  CartesianToSpherical( qSph, pSph, qCart, pCart );
  memcpy( sphValues, qSph, sizeof( qSph ) );
  memcpy( sphValues+3, pSph, sizeof( pSph ) );
  memcpy( sphValues+6, tmpS1, sizeof(tmpS1) );
  memcpy( sphValues+9, tmpS2, sizeof(tmpS2) );

  memcpy( cartValues, qCart, sizeof(qCart) );
  memcpy( cartValues+3, pCart, sizeof(pCart) );
  memcpy( cartValues+6, tmpS1, sizeof(tmpS1) );
  memcpy( cartValues+9, tmpS2, sizeof(tmpS2) );

  REAL8 dHdpphi, d2Hdr2, d2Hdrdpphi;
  REAL8 rDot, dHdpr, flux, dEdr;

  d2Hdr2     = XLALCalculateSphHamiltonianDeriv2( 0, 0, sphValues, params );
  d2Hdrdpphi = XLALCalculateSphHamiltonianDeriv2( 0, 5, sphValues, params );

  //printf( "d2Hdr2 = %.16e, d2Hdrdpphi = %.16e\n", d2Hdr2, d2Hdrdpphi );

  dHdpphi  = XLALSpinHcapNumDerivWRTParam( 4, cartValues, params ) / sphValues[0];
  
  dEdr  = - dHdpphi * d2Hdr2 / d2Hdrdpphi;

  //printf( "d2Hdr2 = %.16e d2Hdrdpphi = %.16e dHdpphi = %.16e\n", d2Hdr2, d2Hdrdpphi, dHdpphi );

  if ( d2Hdr2 != 0.0 )
  {
      
    /* We will need to calculate the Hamiltonian to get the flux */
    s1Vec.length = s2Vec.length = sKerr.length = sStar.length = 3;
    s1Vec.data = tmpS1;
    s2Vec.data = tmpS2;
    sKerr.data = sKerrData;
    sStar.data = sStarData;

    qCartVec.length = pCartVec.length = 3;
    qCartVec.data   = qCart;
    pCartVec.data   = pCart;

    chi1 = tmpS1[0]*LnHat[0] + tmpS1[1]*LnHat[1] + tmpS1[2]*LnHat[2];
    chi2 = tmpS2[0]*LnHat[0] + tmpS2[1]*LnHat[1] + tmpS2[2]*LnHat[2];

    //printf( "magS1 = %.16e, magS2 = %.16e\n", chi1, chi2 );

    chiS = 0.5 * ( chi1 / (mass1*mass1) + chi2 / (mass2*mass2) );
    chiA = 0.5 * ( chi1 / (mass1*mass1) - chi2 / (mass2*mass2) );

    XLALSimIMRSpinEOBCalculateSigmaKerr( &sKerr, mass1, mass2, &s1Vec, &s2Vec );
    XLALSimIMRSpinEOBCalculateSigmaStar( &sStar, mass1, mass2, &s1Vec, &s2Vec );

    /* The a in the flux has been set to zero, but not in the Hamiltonian */
    a = sqrt(sKerr.data[0]*sKerr.data[0] + sKerr.data[1]*sKerr.data[1] + sKerr.data[2]*sKerr.data[2]);
    XLALSimIMREOBCalcSpinFacWaveformCoefficients( params->eobParams->hCoeffs, mass1, mass2, eta, /*a*/0.0, chiS, chiA );
    XLALSimIMRCalculateSpinEOBHCoeffs( params->seobCoeffs, eta, a );

    ham = XLALSimIMRSpinEOBHamiltonian( eta, &qCartVec, &pCartVec, &sKerr, &sStar, params->tortoise, params->seobCoeffs );

    //printf( "hamiltonian at this point is %.16e\n", ham );

    /* And now, finally, the flux */
    REAL8Vector polarDynamics;
    REAL8       polarData[4];

    polarDynamics.length = 4;
    polarDynamics.data = polarData;

    polarData[0] = qSph[0];
    polarData[1] = 0.;
    polarData[2] = pSph[0];
    polarData[3] = pSph[2];

    flux  = XLALInspiralSpinFactorizedFlux( &polarDynamics, omega, params, ham, lMax );
    flux  = flux / eta;

    rDot  = - flux / dEdr;

    /* We now need dHdpr - we take it that it is safely linear up to a pr of 1.0e-3 */
    cartValues[3] = 1.0e-3;
    dHdpr         = XLALSpinHcapNumDerivWRTParam( 3, cartValues, params );


    /*printf( "Ingredients going into prDot:\n" );
    printf( "flux = %.16e, dEdr = %.16e, dHdpr = %.16e\n", flux, dEdr, dHdpr );*/

    /* We can now calculate what pr should be taking into account the flux */
    pSph[0] = rDot / (dHdpr / cartValues[3] );
  }
  else
  {
    /* Since d2Hdr2 has evaluated to zero, we cannot do the above. Just set pr to zero */
    //printf( "d2Hdr2 is zero!\n" );
    pSph[0] = 0;
  }

  /* Now we are done - convert back to cartesian coordinates ) */
  SphericalToCartesian( qCart, pCart, qSph, pSph );

  /* STEP 5) Rotate back to the original inertial frame by inverting the rotation of STEP 3 and then  
   *         inverting the rotation of STEP 1.
   */

  /* Undo rotations to get back to the original basis */
  /* Second rotation */
  ApplyRotationMatrix( invMatrix2, rHat );
  ApplyRotationMatrix( invMatrix2, vHat );
  ApplyRotationMatrix( invMatrix2, LnHat );
  ApplyRotationMatrix( invMatrix2, tmpS1 );
  ApplyRotationMatrix( invMatrix2, tmpS2 );
  ApplyRotationMatrix( invMatrix2, qCart );
  ApplyRotationMatrix( invMatrix2, pCart );

  /* First rotation */
  ApplyRotationMatrix( invMatrix, rHat );
  ApplyRotationMatrix( invMatrix, vHat );
  ApplyRotationMatrix( invMatrix, LnHat );
  ApplyRotationMatrix( invMatrix, tmpS1 );
  ApplyRotationMatrix( invMatrix, tmpS2 );
  ApplyRotationMatrix( invMatrix, qCart );
  ApplyRotationMatrix( invMatrix, pCart );

  /* If required, apply the tortoise transform */
  if ( tmpTortoise )
  {
    REAL8 r = sqrt(qCart[0]*qCart[0] + qCart[1]*qCart[1] + qCart[2]*qCart[2] );
    REAL8 deltaR = XLALSimIMRSpinEOBHamiltonianDeltaR( params->seobCoeffs, r, eta, a );
    REAL8 deltaT = XLALSimIMRSpinEOBHamiltonianDeltaT( params->seobCoeffs, r, eta, a );
    REAL8 csi    = sqrt( deltaT * deltaR )/(r*r + a*a);

    REAL8 pr = (qCart[0]*pCart[0] + qCart[1]*pCart[1] + qCart[2]*pCart[2])/r;

    params->tortoise = tmpTortoise;
#if debugOutput
    fprintf(stderr, "Applying the tortoise to p (csi = %.26e)\n", csi );
#endif
    for ( i = 0; i < 3; i++ )
    {
      pCart[i] = pCart[i] + qCart[i] * pr * (csi - 1.) / r;
    }
  }

    //fprintf(stderr, "DEBUG: r *= 1-e0\n");
    qCart[0] /= 1+e0;
    pCart[1] *= (1+e0);

  /* Now copy the initial conditions back to the return vector */
  memcpy( initConds->data, qCart, sizeof(qCart) );
  memcpy( initConds->data+3, pCart, sizeof(pCart) );
  memcpy( initConds->data+6, tmpS1, sizeof(tmpS1) );
  memcpy( initConds->data+9, tmpS2, sizeof(tmpS2) );
#if debugOutput
  fprintf( stderr,"THE FINAL INITIAL CONDITIONS:\n");
  fprintf( stderr,"(x,y,z)=(%.16e,%.16e,%.16e)\n(px,py,pz)=(%.16e,%.16e,%.16e)\n(S1x,S1y,S1z)=(%.16e,%.16e,%.16e)\n(S2x,S2y,S2z)=(%.16e,%.16e,%.16e)\n", initConds->data[0], initConds->data[1], initConds->data[2],
          initConds->data[3], initConds->data[4], initConds->data[5], initConds->data[6], initConds->data[7], initConds->data[8],
          initConds->data[9], initConds->data[10], initConds->data[11] );
#endif
  return 0;
}

 int
CalculateRotationMatrix(
                gsl_matrix *rotMatrix,  /**< OUTPUT, rotation matrix */
                gsl_matrix *rotInverse, /**< OUTPUT, rotation matrix inversed */
                REAL8       r[],        /**< position vector */
                REAL8       v[],        /**< velocity vector */
                REAL8       L[]         /**< orbital angular momentum */
                )
{

  /** a, b, g are the angles alpha, beta and gamma */
  /* Use a, b and g to avoid shadowing gamma and getting a warning */
  REAL8 a, b, g;
  REAL8 cosa, sina, cosb, sinb, cosg, sing;

  /* Calculate the Euclidean Euler angles */

  /* We need to avoid a singularity if L is along z axis */
  if ( L[2] > 0.9999 )
  {
    a = b = g = 0.0;
  }
  else
  {
    a = atan2( L[0], -L[1] );
    b = atan2( sqrt( L[0]*L[0] + L[1]*L[1] ), L[2] );
    g = atan2( r[2], v[2] );
  }

  if ( ( cosa = cos( a ) ) < 1.0e-16 ) cosa = 0.0;
  if ( ( sina = sin( a ) ) < 1.0e-16 ) sina = 0.0;
  if ( ( cosb = cos( b ) ) < 1.0e-16 ) cosb = 0.0;
  if ( ( sinb = sin( b ) ) < 1.0e-16 ) sinb = 0.0;
  if ( ( cosg = cos( g ) ) < 1.0e-16 ) cosg = 0.0;
  if ( ( sing = sin( g ) ) < 1.0e-16 ) sing = 0.0;

  /** Implement the Rotation Matrix following the "x-convention"
   *  1. rotate about the z-axis by an angle a, rotation matrix Rz(a);
   *  2. rotate about the former x-axis (now x') by an angle b, rotation matrix Rx(b);
   *  3. rotate about the former z-axis (now z') by an algle g, rotation matrix Rz(g); 
   */
  /* populate the matrix */
  gsl_matrix_set( rotMatrix, 0, 0, cosg*cosa - cosb*sina*sing );
  gsl_matrix_set( rotMatrix, 0, 1, cosg*sina + cosb*cosa*sing );
  gsl_matrix_set( rotMatrix, 0, 2, sing*sinb );
  gsl_matrix_set( rotMatrix, 1, 0, -sing*cosa - cosb*sina*cosg );
  gsl_matrix_set( rotMatrix, 1, 1, -sing*sina + cosb*cosa*cosg );
  gsl_matrix_set( rotMatrix, 1, 2, cosg*sinb );
  gsl_matrix_set( rotMatrix, 2, 0, sinb*sina );
  gsl_matrix_set( rotMatrix, 2, 1, -sinb*cosa );
  gsl_matrix_set( rotMatrix, 2, 2, cosb );

  /* Now populate the transpose (which should be the inverse) */
  gsl_matrix_transpose_memcpy( rotInverse, rotMatrix );

  return 0;
}

 int
ApplyRotationMatrix(
             gsl_matrix *rotMatrix, /**< rotation matrix */
             REAL8      a[]         /**< OUTPUT, vector rotated */
                   )
{

  gsl_vector *tmpVec1 = gsl_vector_alloc( 3 );
  gsl_vector *tmpVec2 = gsl_vector_calloc( 3 );  

  gsl_vector_set( tmpVec1, 0, a[0] );
  gsl_vector_set( tmpVec1, 1, a[1] );
  gsl_vector_set( tmpVec1, 2, a[2] );

  gsl_blas_dgemv( CblasNoTrans, 1.0,  rotMatrix, tmpVec1, 0.0, tmpVec2 );

  a[0] = gsl_vector_get( tmpVec2, 0 );
  a[1] = gsl_vector_get( tmpVec2, 1 );
  a[2] = gsl_vector_get( tmpVec2, 2 );

  gsl_vector_free( tmpVec1 );
  gsl_vector_free( tmpVec2 );

  return 0;
}

 int
XLALFindSphericalOrbit( const gsl_vector *x, /**<< Parameters requested by gsl root finder */
                        void *params,        /**<< Spin EOB parameters */
                        gsl_vector *f        /**<< Function values for the given parameters */
                      )
{

  SEOBRootParams *rootParams = (SEOBRootParams *) params;

  REAL8 py, pz, r, ptheta, pphi;

  /* Numerical derivative of Hamiltonian wrt given value */
  REAL8 dHdx, dHdpy, dHdpz;
  REAL8 dHdr, dHdptheta, dHdpphi;

  /* Populate the appropriate values */
  /* In the special theta=pi/2 phi=0 case, r is x */
  rootParams->values[0] = r  = gsl_vector_get( x, 0 );
  rootParams->values[4] = py = gsl_vector_get( x, 1 );
  rootParams->values[5] = pz = gsl_vector_get( x, 2 );

  //printf( "Values r = %.16e, py = %.16e, pz = %.16e\n", r, py, pz );

  ptheta = - r * pz;
  pphi   = r * py;

  /* dHdR */
  dHdx = XLALSpinHcapNumDerivWRTParam( 0, rootParams->values, rootParams->params );
  if ( XLAL_IS_REAL8_FAIL_NAN( dHdx ) )
  {
    	return -1;
  }
  //printf( "dHdx = %.16e\n", dHdx );

  /* dHdPphi (I think we can use dHdPy in this coord system) */
  /* TODO: Check this is okay */
  dHdpy = XLALSpinHcapNumDerivWRTParam( 4, rootParams->values, rootParams->params );
  if ( XLAL_IS_REAL8_FAIL_NAN( dHdpy ) )
  {
    return -1;
  }

  /* dHdPtheta (I think we can use dHdPz in this coord system) */
  /* TODO: Check this is okay */
  dHdpz = XLALSpinHcapNumDerivWRTParam( 5, rootParams->values, rootParams->params );
  if ( XLAL_IS_REAL8_FAIL_NAN( dHdpz ) )
  {
    return -1;
  }

  /* Now convert to spherical polars */
  dHdr      = dHdx - dHdpy * pphi / (r*r) + dHdpz * ptheta / (r*r);
  dHdptheta = - dHdpz / r;
  dHdpphi   = dHdpy / r;

  /* populate the function vector */
  gsl_vector_set( f, 0, dHdr );   // Eq.(4.9) of PRD 74, 104005 (2006)
  gsl_vector_set( f, 1, dHdptheta ); // Eq.(4.8) of PRD 74, 104005 (2006)
  gsl_vector_set( f, 2, dHdpphi - rootParams->omega ); // Eq.(4.8) of PRD 74, 104005 (2006)

  //printf( "Current funcvals = %.16e %.16e %.16e\n", gsl_vector_get( f, 0 ), gsl_vector_get( f, 1 ),
  //   /*gsl_vector_get( f, 2 )*/dHdpphi );

  return 0;
}

 REAL8 XLALSpinHcapNumDerivWRTParam(
                 const INT4 paramIdx,      /**<< Index of the parameters */
                 const REAL8 values[],     /**<< Dynamical variables */
                 SpinEOBParams *funcParams /**<< EOB Parameters */
                 )
{
  static const REAL8 STEP_SIZE = 1.0e-3;

  HcapDerivParams params; // HcapDerivParams is in Panyidatatypes.h

  REAL8 result;

  gsl_function F;
  int         gslStatus;

  REAL8 mass1, mass2;

  /* The error in a derivative as measured by GSL */
  REAL8 absErr;

  /* Set up pointers for GSL */
  params.values  = values;
  params.params  = funcParams;

  F.function       = &GSLSpinHamiltonianWrapper;
  F.params         = &params;
  params.varyParam = paramIdx;

  mass1 = params.params->eobParams->m1;
  mass2 = params.params->eobParams->m2;

  /* Now calculate derivatives w.r.t. the required parameter */
  if ( paramIdx >=6 && paramIdx < 9 )
  {
   	gslStatus = gsl_deriv_central( &F, values[paramIdx],
                    STEP_SIZE*mass1*mass1, &result, &absErr );
  }
  else if ( paramIdx >= 9 )
  {
    	gslStatus = gsl_deriv_central( &F, values[paramIdx],
                    STEP_SIZE*mass2*mass2, &result, &absErr );
  }
  else
  {
    	gslStatus = gsl_deriv_central( &F, values[paramIdx],
                    STEP_SIZE, &result, &absErr );
  }
  if ( gslStatus != 0 )
  {
    fprintf(stderr, "XLAL Error - Failure in GSL function\n" );
    return -1;
  }

  //printf( "Abserr = %e\n", absErr );

  return result;
}

 double GSLSpinHamiltonianWrapper( double x, void *params )
{
  HcapDerivParams *dParams = (HcapDerivParams *)params;

  EOBParams *eobParams = dParams->params->eobParams;

  REAL8 tmpVec[12];
  REAL8 sKerrData[3], sStarData[3];

  /* These are the vectors which will be used in the call to the Hamiltonian */
  REAL8Vector r, p, spin1, spin2;
  REAL8Vector sigmaKerr, sigmaStar;

  REAL8 a;

  /* Use a temporary vector to avoid corrupting the main function */
  memcpy( tmpVec, dParams->values, 
               sizeof(tmpVec) );

  /* Set the relevant entry in the vector to the correct value */
  tmpVec[dParams->varyParam] = x;

  /* Set the LAL-style vectors to point to the appropriate things */
  r.length = p.length = spin1.length = spin2.length = 3;
  sigmaKerr.length = sigmaStar.length = 3;
  r.data     = tmpVec;
  p.data     = tmpVec+3;
  spin1.data = tmpVec+6;
  spin2.data = tmpVec+9;
  sigmaKerr.data = sKerrData;
  sigmaStar.data = sStarData;

  /* Calculate various spin parameters */
  XLALSimIMRSpinEOBCalculateSigmaKerr( &sigmaKerr, eobParams->m1, eobParams->m2, &spin1, &spin2 );
  XLALSimIMRSpinEOBCalculateSigmaStar( &sigmaStar, eobParams->m1, eobParams->m2, &spin1, &spin2 );

  a = sqrt( sigmaKerr.data[0]*sigmaKerr.data[0] + sigmaKerr.data[1]*sigmaKerr.data[1]
              + sigmaKerr.data[2]*sigmaKerr.data[2] );
  //printf( "a = %e\n", a );
  //printf( "aStar = %e\n", sqrt( sigmaStar.data[0]*sigmaStar.data[0] + sigmaStar.data[1]*sigmaStar.data[1] + sigmaStar.data[2]*sigmaStar.data[2]) );
  /*if ( isnan( a ) )
  {
    printf( "a is nan!!\n");
  }*/
  XLALSimIMRCalculateSpinEOBHCoeffs( dParams->params->seobCoeffs, eobParams->eta, a );

  //printf( "Hamiltonian = %e\n", XLALSimIMRSpinEOBHamiltonian( eobParams->eta, &r, &p, &sigmaKerr, &sigmaStar, dParams->params->seobCoeffs ) );
  return XLALSimIMRSpinEOBHamiltonian( eobParams->eta, &r, &p, &sigmaKerr, &sigmaStar, dParams->params->tortoise, dParams->params->seobCoeffs ) / eobParams->eta;
}

 int XLALSimIMRCalculateSpinEOBHCoeffs(
        SpinEOBHCoeffs *coeffs, /**<< OUTPUT, EOB parameters including pre-computed coefficients */
        const REAL8    eta,     /**<< symmetric mass ratio */
        const REAL8    a        /**<< Normalized deformed Kerr spin */
        )
{

  REAL8 KK, k0, k1, k2, k3, k4;
  REAL8 m1PlusEtaKK;
   
  /* Constants are fits taken from Eq. 37 */
  static const REAL8 c0  = 1.4467; /* needed to get the correct self-force results */
  static const REAL8 c1  = -1.7152360250654402;
  static const REAL8 c2  = -3.246255899738242;

  if ( !coeffs )
  {
    return -1;
  }


  coeffs->b3  = 0.;
  coeffs->bb3 = 0.;
  coeffs->KK = KK  = c0 + c1*eta + c2*eta*eta;

  m1PlusEtaKK = -1. + eta*KK;
  /* Eqs. 5.77 - 5.81 of BB1, i.e. PRD 81, 084024 (2010) */
  coeffs->k0 = k0 = KK*(m1PlusEtaKK - 1.);
  coeffs->k1 = k1 = - 2.*(k0 + KK)*m1PlusEtaKK;
  coeffs->k2 = k2 = (k1 * (k1 - 4.*m1PlusEtaKK)) / 2. - a*a*k0*m1PlusEtaKK*m1PlusEtaKK;
  coeffs->k3 = k3 = -k1*k1*k1/3. + k1*k2 + k1*k1*m1PlusEtaKK - 2.*(k2 - m1PlusEtaKK)*m1PlusEtaKK - a*a*k1*m1PlusEtaKK*m1PlusEtaKK;
  coeffs->k4 = k4 = (24.*k1*k1*k1*k1 - 96.*k1*k1*k2 + 48.*k2*k2 - 64.*k1*k1*k1*m1PlusEtaKK
      + 48.*a*a*(k1*k1 - 2.*k2)*m1PlusEtaKK*m1PlusEtaKK +
      96.*k1*(k3 + 2.*k2*m1PlusEtaKK) - m1PlusEtaKK*(192.*k3 + m1PlusEtaKK*(-3008. + 123.*LAL_PI*LAL_PI)))/96.;

  /*printf( "a = %.16e, k0 = %.16e, k1 = %.16e, k2 = %.16e, k3 = %.16e, k4 = %.16e, b3 = %.16e, bb3 = %.16e, KK = %.16e\n",
            a, coeffs->k0, coeffs->k1, coeffs->k2, coeffs->k3, coeffs->k4, coeffs->b3, coeffs->bb3, coeffs->KK );
  */
  return 0;
}


REAL8 XLALSimIMRSpinEOBHamiltonian( 
               const REAL8    eta,                  /**<< Symmetric mass ratio */
               REAL8Vector    *x,         /**<< Position vector */
               REAL8Vector    *p,	    /**<< Momentum vector (tortoise radial component pr*) */
               REAL8Vector    *sigmaKerr, /**<< Spin vector sigma_kerr */
               REAL8Vector    *sigmaStar, /**<< Spin vector sigma_star */
               INT4                      tortoise,  /**<< flag to state whether the momentum is the tortoise co-ord */
	           SpinEOBHCoeffs *coeffs               /**<< Structure containing various coefficients */
               )
{
  REAL8 r, r2, nx, ny, nz;
  REAL8 sKerr_x, sKerr_y, sKerr_z, a, a2;
  REAL8 sStar_x, sStar_y, sStar_z;
  REAL8 e3_x, e3_y, e3_z;
  REAL8 costheta; /* Cosine of angle between Skerr and r */
  REAL8 xi2, xi_x, xi_y, xi_z; /* Cross product of unit vectors in direction of Skerr and r */
  REAL8 vx, vy, vz, pxir, pvr, pn, prT, pr, pf, ptheta2; /*prT is the tortoise pr */
  REAL8 w2, rho2;
  REAL8 u, u2, u3, u4;
  REAL8 bulk, deltaT, deltaR, Lambda;
  REAL8 D, qq, ww, B, w, MU, nu, BR, wr, nur, mur;
  REAL8 wcos, nucos, mucos, ww_r, Lambda_r;
  REAL8 logTerms, deltaU, deltaU_u, Q, deltaT_r, pn2, pp;
  REAL8 deltaSigmaStar_x, deltaSigmaStar_y, deltaSigmaStar_z;
  REAL8 sx, sy, sz, sxi, sv, sn, s3;
  REAL8 H, Hns, Hs, Hss, Hreal, Hwcos, Hwr, HSOL, HSONL;
  REAL8 m1PlusetaKK;

  /* Terms which come into the 3.5PN mapping of the spins */
  //REAL8 aaa, bbb, a13P5, a23P5, a33P5, b13P5, b23P5, b33P5;
  REAL8 sMultiplier1, sMultiplier2;

  /*Temporary p vector which we will make non-tortoise */
  REAL8 tmpP[3];

  REAL8 csi;

  /* Spin gauge parameters. (YP) simplified, since both are zero. */
  // static const double aa=0., bb=0.;

  /* Calibrated coefficient in the 4.5PN spin mapping, Eq. 39 */
  static const REAL8 d1 = -69.5;
  static const REAL8 dheffSS = 2.75;

  //printf( "In Hamiltonian:\n" );
  //printf( "x = %.16e\t%.16e\t%.16e\n", x->data[0], x->data[1], x->data[2] );
  //printf( "p = %.16e\t%.16e\t%.16e\n", p->data[0], p->data[1], p->data[2] );

  r2 = x->data[0]*x->data[0] + x->data[1]*x->data[1] + x->data[2]*x->data[2];
  r  = sqrt(r2);
  nx = x->data[0] / r;
  ny = x->data[1] / r;
  nz = x->data[2] / r;   
     
  sKerr_x = sigmaKerr->data[0];
  sKerr_y = sigmaKerr->data[1];
  sKerr_z = sigmaKerr->data[2];

  sStar_x = sigmaStar->data[0];
  sStar_y = sigmaStar->data[1];
  sStar_z = sigmaStar->data[2];
     
  a2 = sKerr_x*sKerr_x + sKerr_y*sKerr_y + sKerr_z*sKerr_z;
  a  = sqrt( a2 );

  // e3 correspondes to the \hat{S}_{Kerr} in (45) of BB2, i.e., PRD 84, 104027 (2011)
  if(a != 0.) 
  {
    e3_x = sKerr_x / a;
    e3_y = sKerr_y / a;
    e3_z = sKerr_z / a;
  }
  else 
  {
    e3_x = 0.;
    e3_y = 0.;
    e3_z = 1.;	  
  }
       
  // simple spherical coordinate (r,theta,phi) view to (x,y,z)
  costheta = e3_x*nx + e3_y*ny + e3_z*nz; 
    
  //correspondes to sin^2\theta
  xi2=1. - costheta*costheta; 

  // xi correspondes to the \xi in (45) of BB2, i.e., PRD 84, 104027 (2011)
  xi_x = -e3_z*ny + e3_y*nz;
  xi_y =  e3_z*nx - e3_x*nz;
  xi_z = -e3_y*nx + e3_x*ny;

  // v correspondes to the v in (45) of BB2, i.e., PRD 84, 104027 (2011)
  vx = -nz*xi_y + ny*xi_z;
  vy =  nz*xi_x - nx*xi_z;
  vz = -ny*xi_x + nx*xi_y;

  w2 = r2 + a2;
  // (36) of BB2, i.e., PRD 84, 104027 (2011)
  rho2 = r2 + a2*costheta*costheta;

  u  = 1./r;
  u2 = u*u;
  u3 = u2*u;
  u4 = u2*u2;

  //printf( "KK = %.16e\n", coeffs->KK );
  //coeffs->KK correspondes to the K in (5.75) of BB1, i.e. PRD 81, 084024 (2010)
  m1PlusetaKK = -1. + eta * coeffs->KK;
  /* Eq. 5.75 of BB1, i.e. PRD 81, 084024 (2010) with assumed M=1, correspondes to the \bar{Delta}_u */
  bulk = 1./(m1PlusetaKK*m1PlusetaKK) + (2.*u)/m1PlusetaKK + a2*u2;
  /* Eq. 5.73 of BB1, i.e. PRD 81, 084024 (2010) */
  //coeffs->k0...k4 corresponde to Delta0...4 of 5.73 of BB1, i.e. PRD 81, 084024 (2010)
  logTerms = 1. + eta*coeffs->k0 + eta*log(1. + coeffs->k1*u + coeffs->k2*u2 + coeffs->k3*u3 + coeffs->k4*u4);
  //printf( "bulk = %.16e, logTerms = %.16e\n", bulk, logTerms );
  /* Eq. 5.73 of BB1, i.e. PRD 81, 084024 (2010), correspondes to the Delta_u */
  deltaU = bulk*logTerms;
  /* Eq. 5.71 of BB1, i.e. PRD 81, 084024 (2010) */
  deltaT = r2*deltaU;
  /* ddeltaU/du */
  deltaU_u = 2.*(1./m1PlusetaKK + a2*u)*logTerms + 
	  bulk * (eta*(coeffs->k1 + u*(2.*coeffs->k2 + u*(3.*coeffs->k3 + 4.*coeffs->k4*u))))
          / (1. + coeffs->k1*u + coeffs->k2*u2 + coeffs->k3*u3 + coeffs->k4*u4);
  /* ddeltaT/dr */
  deltaT_r = 2.*r*deltaU - deltaU_u;
  /* Eq. 5.39 of BB1, i.e. PRD 81, 084024 (2010), (35) of BB2, i.e., PRD 84, 104027 (2011) */
  Lambda = w2*w2 - a2*deltaT*xi2;
  /* Eq. 5.83 of BB1, i.e. PRD 81, 084024 (2010), inverse */
  D = 1. + log(1. + 6.*eta*u2 + 2.*(26. - 3.*eta)*eta*u3);
  /* Eq. 5.38 of BB1, i.e. PRD 81, 084024 (2010) */
  deltaR = deltaT*D;
  /* See Hns below, Eq. 4.34 of Damour et al. PRD 62, 084011 (2000) */
  qq = 2.*eta*(4. - 3.*eta);
  /* See Hns below. In Sec. II D of BB2, i.e., PRD 84, 104027 (2011),
   * b3 and bb3 coeffs are chosen to be zero. */
  //Eq.(5.40) of BB1, i.e. PRD 81, 084024 (2010) which is different to 
  //Eq.(37) of BB2, i.e., PRD 84, 104027 (2011)
  ww=2.*a*r + coeffs->b3*eta*a2*a*u + coeffs->bb3*eta*a*u;

  /* We need to transform the momentum to get the tortoise co-ord */
  if ( tortoise )
  {
    /* Eq. 28 of Pan et al. PRD 81, 084041 (2010) */
    // correspondes to the xi_a there	  
    csi = sqrt( deltaT * deltaR )/ w2; 
  }
  else
  {
    csi = 1.0;
  }
  //printf( "csi(miami) = %.16e\n", csi );

  prT = p->data[0]*nx + p->data[1]*ny + p->data[2]*nz;
  /* p->data is BL momentum vector; tmpP is tortoise momentum vector */ 
  // Eq.(A3) of PRD 81, 084041 (2010), 
  // but it seems csi here correspondes to 1/xi_a there
  tmpP[0] = p->data[0] - nx * prT * (csi - 1.)/csi;
  tmpP[1] = p->data[1] - ny * prT * (csi - 1.)/csi;
  tmpP[2] = p->data[2] - nz * prT * (csi - 1.)/csi;

  pxir = (tmpP[0]*xi_x + tmpP[1]*xi_y + tmpP[2]*xi_z) * r;
  pvr  = (tmpP[0]*vx + tmpP[1]*vy + tmpP[2]*vz) * r;
  pn   = tmpP[0]*nx + tmpP[1]*ny + tmpP[2]*nz;
          
  pr = pn;
  pf = pxir;
  ptheta2 = pvr * pvr / xi2;

  //printf( "pr = %.16e, prT = %.16e\n", pr, prT );

  //printf( " a = %.16e, r = %.16e\n", a, r );
  //printf( "D = %.16e, ww = %.16e, rho = %.16e, Lambda = %.16e, xi = %.16e\npr = %.16e, pf = %.16e, deltaR = %.16e, deltaT = %.16e\n", 
      //D, ww, sqrt(rho2), Lambda, sqrt(xi2), pr, pf, deltaR, deltaT );
  /* Eqs. 5.36 - 5.46 of BB1, i.e. PRD 81, 084024 (2010)  */
  /* Note that the tortoise prT appears only in the quartic term, explained in
   * Eqs. 14 and 15 of Tarrachini et al. PRD 86, 024011 (2012) */
  //according to the (5.45) of BB1, i.e. PRD 81, 084024 (2010)
  //beta^\phi = ww/Lambda 
  //Q4 = prT^4/r^2*z3, z3 is in (4.34) of Damour et al. PRD 62, 084011 (2000)
  Hns = sqrt(1. + prT*prT*prT*prT*qq*u2 + ptheta2/rho2 + pf*pf*rho2/(Lambda*xi2) + pr*pr*deltaR/rho2)
      / sqrt(Lambda/(rho2*deltaT)) + pf*ww/Lambda;
  
  //printf( "term 1 in Hns: %.16e\n",  prT*prT*prT*prT*qq*u2 );
  //printf( "term 2 in Hns: %.16e\n", ptheta2/rho2 );
  //printf( "term 3 in Hns = %.16e\n", pf*pf*rho2/(Lambda*xi2) );
  //printf( "term 4 in Hns = %.16e\n", pr*pr*deltaR/rho2 );
  //printf( "term 5 in Hns = %.16e\n", Lambda/(rho2*deltaT) );
  //printf( "term 6 in Hns = %.16e\n", pf*ww/Lambda );
  /* Eqs. 5.30 - 5.33 of BB1, i.e. PRD 81, 084024 (2010) */
  B = sqrt(deltaT);
  w = ww/Lambda;
  nu = 0.5 * log(deltaT*rho2/Lambda);
  MU = 0.5 * log(rho2);  
  /* dLambda/dr */
  Lambda_r = 4.*r*w2 - a2*deltaT_r*xi2;
     
  //dww/dr, ww is the \tilde{\omega}_{fd} of Eq.(5.40) of BB1, i.e. PRD 81, 084024 (2010)
  ww_r=2.*a - (a2*a*coeffs->b3*eta)*u2 - coeffs->bb3*eta*a*u2;
  /* Eqs. 5.47a - 5.47d of BB1, i.e. PRD 81, 084024 (2010) */
  BR = (-2.*deltaT + sqrt(deltaR)*deltaT_r)/(2.*sqrt(deltaR*deltaT));
  wr = (-Lambda_r*ww + Lambda*ww_r)/(Lambda*Lambda);
  nur = (r/rho2 + (w2 * (-4.*r*deltaT + w2*deltaT_r) ) / (2.*deltaT*Lambda) );
  mur = (r/rho2 - 1./sqrt(deltaR));
  /* Eqs. 5.47f - 5.47h of BB1, i.e. PRD 81, 084024 (2010) */
  wcos  = -2.*a2*costheta*deltaT*ww/(Lambda*Lambda);  
  nucos = a2*costheta*w2*(w2-deltaT)/(rho2*Lambda);  
  mucos = a2*costheta/rho2;
  /* Eq. 5.52 of BB1, i.e. PRD 81, 084024 (2010), (YP) simplified */
  //Q = 1. + pvr*pvr/(exp(2.*MU)*xi2) + exp(2.*nu)*pxir*pxir/(B*B*xi2) + pn*pn*deltaR/exp(2.*MU);
  Q = 1. + pvr*pvr/(rho2*xi2) + deltaT*rho2/Lambda*pxir*pxir/(B*B*xi2) + pn*pn*deltaR/rho2;
      
  pn2 = pr * pr * deltaR / rho2;
  pp  = Q - 1.;

  //printf( "pn2 = %.16e, pp = %.16e\n", pn2, pp );
  //printf( "sigmaKerr = %.16e, sigmaStar = %.16e\n", sKerr_z, sStar_z );
  /* expanded Eq. 5.68 of BB1, i.e. PRD 81, 084024 (2010), (YP) simplified for aa=bb=0. */
  /*
  deltaSigmaStar_x=(- 8.*aa*(1. + 3.*pn2*r - pp*r)*sKerr_x - 8.*bb*(1. + 3.*pn2*r - pp*r)*sStar_x + 
        eta*(-8.*sKerr_x - 36.*pn2*r*sKerr_x + 3.*pp*r*sKerr_x + 14.*sStar_x - 30.*pn2*r*sStar_x + 4.*pp*r*sStar_x))/(12.*r);

  deltaSigmaStar_y=(-8.*aa*(1. + 3.*pn2*r - pp*r)*sKerr_y - 8.*bb*(1. + 3.*pn2*r - pp*r)*sStar_y + 
        eta*(-8.*sKerr_y - 36.*pn2*r*sKerr_y + 3.*pp*r*sKerr_y + 14.*sStar_y - 30.*pn2*r*sStar_y + 4.*pp*r*sStar_y))/(12.*r);

  deltaSigmaStar_z=(-8.*aa*(1. + 3.*pn2*r - pp*r)*sKerr_z - 8.*bb*(1. + 3.*pn2*r - pp*r)*sStar_z + 
	eta*(-8.*sKerr_z - 36.*pn2*r*sKerr_z + 3.*pp*r*sKerr_z + 14.*sStar_z - 30.*pn2*r*sStar_z + 4.*pp*r*sStar_z))/(12.*r);
  */
  deltaSigmaStar_x=eta*(-8.*sKerr_x - 36.*pn2*r*sKerr_x + 3.*pp*r*sKerr_x + 14.*sStar_x - 30.*pn2*r*sStar_x + 4.*pp*r*sStar_x)/(12.*r);

  deltaSigmaStar_y=eta*(-8.*sKerr_y - 36.*pn2*r*sKerr_y + 3.*pp*r*sKerr_y + 14.*sStar_y - 30.*pn2*r*sStar_y + 4.*pp*r*sStar_y)/(12.*r);

  deltaSigmaStar_z=eta*(-8.*sKerr_z - 36.*pn2*r*sKerr_z + 3.*pp*r*sKerr_z + 14.*sStar_z - 30.*pn2*r*sStar_z + 4.*pp*r*sStar_z)/(12.*r);


  /* Now compute the additional 3.5PN terms. */
  /* The following gauge parameters correspond to those given by 
   * Eqs. (69) and (70) of BB2, i.e., PRD 84, 104027 (2011) (aaa -> a0, bbb -> b0).
   * In SEOBNRv1 model, we chose to set all of them to zero,
   * described between Eqs. (3) and (4).
   */
  /*
  aaa = -3./2.*eta;
  bbb = -5./4.*eta;
  a1 = eta*eta/2.;
  a2 = -(1./8.)*eta*(-7. + 8.*eta);
  a3 = -((9.*eta*eta)/16.);
  b1 = 1./16.*eta*(9. + 5.*eta);
  b2 = -(1./8.)*eta*(-17. + 5.*eta);
  b3 = -3./8.*eta*eta;
  */
  /*aaa = 0.;
  bbb = 0.;
  a13P5 = 0.;
  a23P5 = 0.;
  a33P5 = 0.;
  b13P5 = 0.;
  b23P5 = 0.;
  b33P5 = 0.;
  */
  /* Eq. 52 of BB2, i.e., PRD 84, 104027 (2011), (YP) simplified for zero gauge parameters */    
  /* 
  sMultiplier1 =-(2.*(24.*b23P5 + eta*(-353. + 27.*eta) + bbb*(56. + 60.*eta)) +
      2.*(24.*b13P5 - 24.*b23P5 + bbb*(14. - 66.*eta) + 103.*eta - 60.*eta*eta)*pp*
      r + 120.*(2.*b33P5 - 3.*eta*(bbb + eta))*pn2*pn2*r*r +
      (-48.*b13P5 + 4.*bbb*(1. + 3.*eta) + eta*(23. + 3.*eta))*pp*pp*
      r*r + 6.*pn2*r*(16.*b13P5 + 32.*b23P5 + 24.*b33P5 - 47.*eta +
      54.*eta*eta + 24.*bbb*(1. + eta) +
     (24.*b13P5 - 24.*b33P5 - 16.*eta + 21.*eta*eta + bbb*(-2. + 30.*eta))*pp*
     r))/(72.*r*r);
  */
  sMultiplier1 = -(2.*eta*(-353. + 27.*eta) + 2.*(103.*eta - 60.*eta*eta)*pp*r 
               + 120.*(-3.*eta*eta)*pn2*pn2*r*r + (eta*(23. + 3.*eta))*pp*pp*r*r 
               + 6.*pn2*r*(- 47.*eta + 54.*eta*eta + (- 16.*eta + 21.*eta*eta)*pp*r))
               / (72.*r*r);                        
  /* Eq. 52 of BB2, i.e., PRD 84, 104027 (2011), (YP) simplified for zero gauge parameters */       
  /*
  sMultiplier2 = (-16.*(6.*a23P5 + 7.*eta*(8. + 3.*eta) + aaa*(14. + 15.*eta)) +
      4.*(-24.*a13P5 + 24.*a23P5 - 109.*eta + 51.*eta*eta + 2.*aaa*(-7. + 33.*eta))*
      pp*r + 30.*(-16.*a33P5 + 3.*eta*(8.*aaa + 9.*eta))*pn2*pn2*r*r +
      (96.*a13P5 - 45.*eta - 8.*aaa*(1. + 3.*eta))*pp*pp*r*r -
      6.*pn2*r*(32.*a13P5 + 64.*a23P5 + 48.*a33P5 + 16.*eta + 147.*eta*eta +
      48.*aaa*(1. + eta) + (48.*a13P5 - 48.*a33P5 - 6.*eta + 39.*eta*eta +
      aaa*(-4. + 60.*eta))*pp*r))/(144.*r*r);
  */
  sMultiplier2 = (-16.*(7.*eta*(8. + 3.*eta)) + 4.*(- 109.*eta + 51.*eta*eta)*pp*r 
               + 810.*eta*eta*pn2*pn2*r*r - 45.*eta*pp*pp*r*r 
               - 6.*pn2*r*(16.*eta + 147.*eta*eta + (- 6.*eta + 39.*eta*eta)*pp*r))
               / (144.*r*r);
  /* Eq. 52 of BB2, i.e., PRD 84, 104027 (2011) */                     
  deltaSigmaStar_x += sMultiplier1*sigmaStar->data[0] + sMultiplier2*sigmaKerr->data[0];
  deltaSigmaStar_y += sMultiplier1*sigmaStar->data[1] + sMultiplier2*sigmaKerr->data[1];
  deltaSigmaStar_z += sMultiplier1*sigmaStar->data[2] + sMultiplier2*sigmaKerr->data[2];

  /* And now the (calibrated) 4.5PN term */
  deltaSigmaStar_x += d1 * eta * sigmaStar->data[0] / (r*r*r);
  deltaSigmaStar_y += d1 * eta * sigmaStar->data[1] / (r*r*r);
  deltaSigmaStar_z += d1 * eta * sigmaStar->data[2] / (r*r*r);

  //printf( "deltaSigmaStar_x = %.16e, deltaSigmaStar_y = %.16e, deltaSigmaStar_z = %.16e\n", 
  //   deltaSigmaStar_x, deltaSigmaStar_y, deltaSigmaStar_z );
	
  sx = sStar_x + deltaSigmaStar_x;
  sy = sStar_y + deltaSigmaStar_y;
  sz = sStar_z + deltaSigmaStar_z;     
     
     
  sxi = sx*xi_x + sy*xi_y + sz*xi_z;
  sv  = sx*vx + sy*vy + sz*vz;
  sn  = sx*nx + sy*ny + sz*nz; 
     
  s3 = sx*e3_x + sy*e3_y + sz*e3_z;  
  /* Eq. 3.45 of BB1, i.e. PRD 81, 084024 (2010), second term */        
  Hwr = (exp(-3.*MU - nu)*sqrt(deltaR)*(exp(2.*(MU + nu))*pxir*pxir*sv - B*exp(MU + nu)*pvr*pxir*sxi + 
        B*B*xi2*(exp(2.*MU)*(sqrt(Q) + Q)*sv + pn*pvr*sn*sqrt(deltaR) - pn*pn*sv*deltaR)))/(2.*B*(1. + sqrt(Q))*sqrt(Q)*xi2);
  /* Eq. 3.45 of BB1, i.e. PRD 81, 084024 (2010), third term */     
  Hwcos = (exp(-3.*MU - nu)*(sn*(-(exp(2.*(MU + nu))*pxir*pxir) + B*B*(pvr*pvr - exp(2.*MU)*(sqrt(Q) + Q)*xi2)) - 
        B*pn*(B*pvr*sv - exp(MU + nu)*pxir*sxi)*sqrt(deltaR)))/(2.*B*(1. + sqrt(Q))*sqrt(Q));
  /* Eq. 3.44 of BB1, i.e. PRD 81, 084024 (2010), leading term */     
  HSOL = (exp(-MU + 2.*nu)*(-B + exp(MU + nu))*pxir*s3)/(B*B*sqrt(Q)*xi2);
  /* Eq. 3.44 of BB1, i.e. PRD 81, 084024 (2010), next-to-leading term */
  HSONL = (exp(-2.*MU + nu)*(-(B*exp(MU + nu)*nucos*pxir*(1. + 2.*sqrt(Q))*sn*xi2) + 
        (-(BR*exp(MU + nu)*pxir*(1. + sqrt(Q))*sv) + B*(exp(MU + nu)*nur*pxir*(1. + 2.*sqrt(Q))*sv + B*mur*pvr*sxi + 
        B*sxi*(-(mucos*pn*xi2) + sqrt(Q)*(mur*pvr - nur*pvr + (-mucos + nucos)*pn*xi2))))*sqrt(deltaR)))/(B*B*(sqrt(Q) + Q)*xi2);   
  /* Eq. 3.43 and 3.45 of BB1, i.e. PRD 81, 084024 (2010) */
  Hs = w*s3 + Hwr*wr + Hwcos*wcos + HSOL + HSONL;
  /* Eq. 5.70 of BB1, i.e. PRD 81, 084024 (2010), last term */   
  Hss = -0.5*u3 * (sx*sx + sy*sy + sz*sz - 3.*sn*sn);
  /* Eq. 5.70 of BB1, i.e. PRD 81, 084024 (2010) */
  H = Hns + Hs + Hss;

  /* Add the additional calibrated term */
  H += dheffSS * eta * (sKerr_x*sStar_x + sKerr_y*sStar_y + sKerr_z*sStar_z) / (r*r*r*r);

  //printf( "Hns = %.16e, Hs = %.16e, Hss = %.16e, other = %.16e\n", Hns, Hs, Hss, dheffSS * eta * (sKerr_x*sStar_x + sKerr_y*sStar_y + sKerr_z*sStar_z) / (r*r*r*r) );
  //printf( "H = %.16e\n", H );
  /* Real Hamiltonian given by Eq. 2, ignoring the constant -1. */
  /* Eq. 5.69 of BB1, i.e. PRD 81, 084024 (2010) */
  Hreal = sqrt(1. + 2.*eta *(H - 1.));

  return Hreal;
}

 int CartesianToSpherical(
                 REAL8 qSph[],        /**<< OUTPUT, position vector in spherical coordinates */
                 REAL8 pSph[],        /**<< OUTPUT, momentum vector in Cartesean coordinates */
                 const REAL8 qCart[], /**<< position vector in spherical coordinates */
                 const REAL8 pCart[]  /**<< momentum vector in Cartesean coordinates */
                 )
{

  REAL8 r;
  REAL8 pr, pTheta, pPhi;
  
  REAL8 x; //, y, z;
  REAL8 pX, pY, pZ;

  x  = qCart[0];
  //y  = qCart[1];
  //z  = qCart[2];
  pX = pCart[0];
  pY = pCart[1];
  pZ = pCart[2];

  r  = x;
  pr = pX;
  pTheta = - r * pZ;
  pPhi   =   r * pY;

  /* Fill the output vectors */
  qSph[0] = r;
  qSph[1] = LAL_PI_2; //pi/2
  qSph[2] = 0.0;
  pSph[0] = pr;
  pSph[1] = pTheta;
  pSph[2] = pPhi;

  return 0;
}

/** Tests if a value is an XLAL <tt>REAL8</tt> failure NaN. */
 int XLALIsREAL8FailNaN(REAL8 val)
{
	volatile const union { LAL_INT8_C i; unsigned char s[8]; } a = { 417 }; //XLAL_REAL8_FAIL_NAN_INT } ;
	//volatile const union { int i; unsigned char s[8]; } a = { XLAL_REAL8_FAIL_NAN_INT } ;
	volatile union { REAL8 x; unsigned char s[8]; } b;
	int n;
	b.x = val;
	for (n = 0; n < sizeof(val); ++n)
		if (a.s[n] != b.s[n])
		return 0;
	return 1;
}

 REAL8 XLALCalculateSphHamiltonianDeriv2(
                 const int      idx1,     /**<< Derivative w.r.t. index 1 */
                 const int      idx2,     /**<< Derivative w.r.t. index 2 */
                 const REAL8    values[], /**<< Dynamical variables in spherical coordinates */
                 SpinEOBParams *params    /**<< Spin EOB Parameters */
                 )
{

  static const REAL8 STEP_SIZE = 1.0e-5;

  REAL8 result;
  REAL8 absErr;

  HcapSphDeriv2Params dParams;

  gsl_function F;
  int gslStatus;

  dParams.sphValues  = values;
  dParams.varyParam1 = idx1;
  dParams.varyParam2 = idx2;
  dParams.params     = params;

  /*printf( " In second deriv function: values\n" );
  for ( int i = 0; i < 12; i++ )
  {
    printf( "%.16e ", values[i] );
  }
  printf( "\n" );
*/
  F.function = GSLSpinHamiltonianDerivWrapper;
  F.params   = &dParams;

  /* GSL seemed to give weird answers - try my own code */
  /*result = GSLSpinHamiltonianDerivWrapper( values[idx1] + STEP_SIZE, &dParams )
         - GSLSpinHamiltonianDerivWrapper( values[idx1] - STEP_SIZE, &dParams );
  printf( "%.16e - %.16e / 2h\n", GSLSpinHamiltonianDerivWrapper( values[idx1] + STEP_SIZE, &dParams ), GSLSpinHamiltonianDerivWrapper( values[idx1] - STEP_SIZE, &dParams ) );

  result = result / ( 2.*STEP_SIZE );
*/
 
  gslStatus = gsl_deriv_central( &F, values[idx1], 
                      STEP_SIZE, &result, &absErr );

  if ( gslStatus != GSL_SUCCESS )
  {
    fprintf(stderr, "XLAL Error %d - Failure in GSL function\n", gslStatus );
    return -1;
  }

  //printf( "Second deriv abs err = %.16e\n", absErr );

  //printf( "RESULT = %.16e\n", result );
  return result;
}

 double GSLSpinHamiltonianDerivWrapper( double x,    /**<< Derivative at x */
                                              void  *params /**<< Function parameters */)
{

  HcapSphDeriv2Params *dParams = (HcapSphDeriv2Params *) params;

  REAL8 sphValues[12];
  REAL8 cartValues[12];

  REAL8 dHdr, dHdx, dHdpy, dHdpz;
  REAL8 r, ptheta, pphi;

  memcpy( sphValues, dParams->sphValues, sizeof( sphValues ) );
  sphValues[dParams->varyParam1] = x;

  SphericalToCartesian( cartValues, cartValues+3, sphValues, sphValues+3 );
  memcpy( cartValues+6, sphValues+6, 6*sizeof(REAL8) );

  r      = sphValues[0];
  ptheta = sphValues[4];
  pphi   = sphValues[5];

  /* Return the appropriate derivative according to varyParam2 */
  switch ( dParams->varyParam2 )
  {
    case 0:
      /* dHdr */
      dHdx  = XLALSpinHcapNumDerivWRTParam( 0, cartValues, dParams->params );
      dHdpy = XLALSpinHcapNumDerivWRTParam( 4, cartValues, dParams->params );
      dHdpz = XLALSpinHcapNumDerivWRTParam( 5, cartValues, dParams->params );

      dHdr      = dHdx - dHdpy * pphi / (r*r) + dHdpz * ptheta / (r*r);
      //printf( "dHdr = %.16e\n", dHdr );
      return dHdr;

      break;
    case 4:
      /* dHdptheta */
      dHdpz = XLALSpinHcapNumDerivWRTParam( 5, cartValues, dParams->params );
      return - dHdpz / r;
      break;
    case 5:
      /* dHdpphi */
      dHdpy = XLALSpinHcapNumDerivWRTParam( 4, cartValues, dParams->params );
      return dHdpy / r;
      break;
    default:
      fprintf( stderr,"This option is not supported in the second derivative function!\n" );
      return -1;
      break;
  }
}

 int SphericalToCartesian(
                 REAL8 qCart[],      /**<< OUTPUT, position vector in Cartesean coordinates */
                 REAL8 pCart[],      /**<< OUTPUT, momentum vector in Cartesean coordinates */
                 const REAL8 qSph[], /**<< position vector in spherical coordinates */
                 const REAL8 pSph[]  /**<< momentum vector in spherical coordinates */
                 )
{

  REAL8 r;
  REAL8 pr, pTheta, pPhi;

  REAL8 x, y, z;
  REAL8 pX, pY, pZ;

  r      = qSph[0];
  pr     = pSph[0];
  pTheta = pSph[1];
  pPhi   = pSph[2];

  x = r;
  y = 0.0;
  z = 0.0;

  pX = pr;
  pY = pPhi / r;
  pZ = - pTheta / r;

  /* Copy these values to the output vectors */
  qCart[0] = x;
  qCart[1] = y;
  qCart[2] = z;
  pCart[0] = pX;
  pCart[1] = pY;
  pCart[2] = pZ;

  return 0;
}

 REAL8 XLALInspiralSpinFactorizedFlux(
                      REAL8Vector           *values, /**< dynamical variables */
                      const REAL8           omega,   /**< orbital frequency */
                      SpinEOBParams         *ak,     /**< physical parameters */
                      const REAL8            H,      /**< real Hamiltonian */
                      const int             lMax    /**< upper limit of the summation over l */
                     )

{

  REAL8 flux = 0.0;
  REAL8 v;
  REAL8 omegaSq;
  COMPLEX16 hLM;
  int l, m;

  EOBNonQCCoeffs nqcCoeffs;


  if ( lMax < 2 )
  {
    return -1;
  }

  /* Omegs is the derivative of phi */
  omegaSq = omega*omega;

  v = MYcbrt( omega );
//  printf( "v = %.16e\n", v );
  for ( l = 2; l <= lMax; l++ )
  {
    for ( m = 1; m <= l; m++ )
    {

      if ( XLALSimIMRSpinEOBGetSpinFactorizedWaveform( &hLM, values, v, H, l, m, ak )
             == -1 )
      {
        fprintf(stderr,"Error!(%d)", XLAL_EFUNC );
      }
      /* For the 2,2 mode, we apply NQC correction to the flux */
      if ( l == 2 && m == 2 )
      {
        COMPLEX16 hNQC;
        XLALSimIMRGetEOBCalibratedSpinNQC( &nqcCoeffs, l, m, ak->eobParams->eta, ak->a );    
        XLALSimIMREOBNonQCCorrection( &hNQC, values, omega, &nqcCoeffs );
        /* Eq. 16 */
        hLM *= hNQC;
      }
      // printf( "l = %d, m = %d, mag(hLM) = %.17e\n", l, m,  XLALCOMPLEX16Abs2( hLM ) );
      /* Eq. 13, Eq.(12) of PRD 84, 124052 (2011)*/
      flux += (REAL8)(m * m) * omegaSq * ( creal(hLM)*creal(hLM) + cimag(hLM)*cimag(hLM));
    }
  }
  return flux * LAL_1_PI / 8.0;
}

// The resummed factorized waveform was proposed by first time in
// Eq.(1) of PRD 79, 064004 (2009)
 int XLALSimIMRSpinEOBGetSpinFactorizedWaveform( 
                 COMPLEX16         * hlm,    /**< OUTPUT, hlm waveforms */
                 REAL8Vector       * values, /**< dyanmical variables */
                 const REAL8         v,               /**< velocity */
                 const REAL8         Hreal,           /**< real Hamiltonian */
                 const int          l,               /**< l mode index */
                 const int          m,               /**< m mode index */
                 SpinEOBParams     * params  /**< Spin EOB parameters */
                 )
{
    /* Status of function calls */
    int status;
    int i;

    REAL8 eta;	
	REAL8 r, pp, Omega, v2, vh, vh3, k, hathatk, eulerlogxabs; //pr
	REAL8 Slm, deltalm, rholm, rholmPwrl;
        REAL8 auxflm = 0.0;
	COMPLEX16 Tlm;
    COMPLEX16 hNewton;
	gsl_sf_result lnr1, arg1, z2;

    /* Non-Keplerian velocity */
    REAL8 vPhi, vPhi2;

    /* Pre-computed coefficients */
    FacWaveformCoeffs *hCoeffs = params->eobParams->hCoeffs;
	COMPLEX16 ci = I;

	if ( abs(m) > (int) l )
	{
	  return -1;
	}
        if ( m == 0 )
	{
	  return -1;
	}	

        eta = params->eobParams->eta;

        /* Check our eta was sensible */
        if ( eta > 0.25 )
        {
          fprintf(stderr,"XLAL Error: Eta seems to be > 0.25 - this isn't allowed!\n" );
         return -1;
        }
        /*else if ( eta == 0.25 && m % 2 )
        {
          // If m is odd and dM = 0, hLM will be zero 
          memset( hlm, 0, sizeof( COMPLEX16 ) );
          return XLAL_SUCCESS;
        }*/
        
	r	= values->data[0];
	//pr	= values->data[2];
	pp	= values->data[3];

	v2	= v * v;
        Omega   = v2 * v;
        vh3     = Hreal * Omega;
	vh	= MYcbrt(vh3);
	eulerlogxabs = LAL_GAMMA + log( 2.0 * (REAL8)m * v );

        /* Calculate the non-Keplerian velocity */
        if ( params->alignedSpins )
        {
          vPhi = XLALSimIMRSpinAlignedEOBNonKeplerCoeff( values->data, params );

          if ( XLAL_IS_REAL8_FAIL_NAN( vPhi ) )
          {
            return -1;
          }

          vPhi  = r * MYcbrt(vPhi);
          vPhi *= Omega;
          vPhi2 = vPhi*vPhi;
        }
        else
        {
          vPhi = v;
          vPhi2 = v2;
        }

        /* Calculate the newtonian multipole, 1st term in Eq. 17, given by Eq. A1 */
        status = XLALSimIMRSpinEOBCalculateNewtonianMultipole( &hNewton, vPhi2, r,
                         values->data[1], (UINT4)l, m, params->eobParams );
        if ( status == -1 )
        {
          return -1;
        }

        /* Calculate the source term, 2nd term in Eq. 17, given by Eq. A5 */
	// Eq.(20) of PRD 84, 124052
	if ( ( (l+m)%2 ) == 0)
	{ 
	  Slm = (Hreal*Hreal - 1.)/(2.*eta) + 1.;
	}
	else
	{
	  Slm = v * pp;
	}
        //printf( "Hreal = %e, Slm = %e, eta = %e\n", Hreal, Slm, eta );

        /* Calculate the Tail term, 3rd term in Eq. 17, given by Eq. A6 */	
	k	= m * Omega;
	hathatk = Hreal * k;
	status = gsl_sf_lngamma_complex_e( l+1.0, -2.0*hathatk, &lnr1, &arg1 );
	if (status != GSL_SUCCESS)
	{
	  fprintf(stderr,"XLAL Error -: Error in GSL function\n" );
	  return -1;
	}
	status = gsl_sf_fact_e( l, &z2 );
	if ( status != GSL_SUCCESS)
	{
	  fprintf(stderr,"XLAL Error - : Error in GSL function\n" );
	  return -1;
	}
	
	
	Tlm = cexp( ( lnr1.val + LAL_PI * hathatk ) + ci * ( 
				arg1.val + 2.0 * hathatk * log(4.0*k/sqrt(LAL_E)) ) );
	Tlm /= z2.val;


        /* Calculate the residue phase and amplitude terms */
        /* deltalm is the 4th term in Eq. 17, delta 22 given by Eq. A15, others  */
        /* rholm is the 5th term in Eq. 17, given by Eqs. A8 - A14 */
        /* auxflm is a special part of the 5th term in Eq. 17, given by Eq. A15 */
        /* Actual values of the coefficients are defined in the next function of this file */
	switch( l )
	{
	  case 2:
	    switch( abs(m) )
	    {
	      case 2:
	        deltalm = vh3*(hCoeffs->delta22vh3 + vh3*(hCoeffs->delta22vh6 
                    + vh*vh*(hCoeffs->delta22vh9*vh)))
                    + hCoeffs->delta22v5 *v*v2*v2 + hCoeffs->delta22v8 *v2*v2*v2*v2;
                rholm   = 1. + v2*(hCoeffs->rho22v2 + v*(hCoeffs->rho22v3
                    + v*(hCoeffs->rho22v4
                    + v*(hCoeffs->rho22v5 + v*(hCoeffs->rho22v6 
                    + hCoeffs->rho22v6l*eulerlogxabs + v*(hCoeffs->rho22v7 
                    + v*(hCoeffs->rho22v8 + hCoeffs->rho22v8l*eulerlogxabs 
                    + (hCoeffs->rho22v10 + hCoeffs->rho22v10l * eulerlogxabs)*v2)))))));
	        break;
	      case 1:
                {
	        deltalm = vh3*(hCoeffs->delta21vh3 + vh3*(hCoeffs->delta21vh6
			+ vh*(hCoeffs->delta21vh7 + (hCoeffs->delta21vh9)*vh*vh))) 
			+ hCoeffs->delta21v5*v*v2*v2 + hCoeffs->delta21v7*v2*v2*v2*v;
		rholm	= 1. + v*(hCoeffs->rho21v1
			+ v*( hCoeffs->rho21v2 + v*(hCoeffs->rho21v3 + v*(hCoeffs->rho21v4 
			+ v*(hCoeffs->rho21v5 + v*(hCoeffs->rho21v6 + hCoeffs->rho21v6l*eulerlogxabs 
			+ v*(hCoeffs->rho21v7 + hCoeffs->rho21v7l * eulerlogxabs 
			+ v*(hCoeffs->rho21v8 + hCoeffs->rho21v8l * eulerlogxabs 
			+ (hCoeffs->rho21v10 + hCoeffs->rho21v10l * eulerlogxabs)*v2))))))));
                auxflm = v*hCoeffs->f21v1;
                }
	        break;
	      default:
                break;
	    }
	    break;
	  case 3:
	    switch (m)
	    {
	      case 3:
	        deltalm = vh3*(hCoeffs->delta33vh3 + vh3*(hCoeffs->delta33vh6 + hCoeffs->delta33vh9*vh3)) 
                        + hCoeffs->delta33v5*v*v2*v2 + hCoeffs->delta33v7*v2*v2*v2*v;
		rholm	= 1. + v2*(hCoeffs->rho33v2 + v*(hCoeffs->rho33v3 + v*(hCoeffs->rho33v4 
			+ v*(hCoeffs->rho33v5 + v*(hCoeffs->rho33v6 + hCoeffs->rho33v6l*eulerlogxabs
			+ v*(hCoeffs->rho33v7 + (hCoeffs->rho33v8 + hCoeffs->rho33v8l*eulerlogxabs)*v))))));
                auxflm = v*v2*hCoeffs->f33v3;
	        break;
	      case 2:
		deltalm = vh3*(hCoeffs->delta32vh3 + vh*(hCoeffs->delta32vh4 + vh*vh*(hCoeffs->delta32vh6
			+ hCoeffs->delta32vh9*vh3)));
		rholm	= 1. + v*(hCoeffs->rho32v 
			+ v*(hCoeffs->rho32v2 + v*(hCoeffs->rho32v3 + v*(hCoeffs->rho32v4 + v*(hCoeffs->rho32v5
			+ v*(hCoeffs->rho32v6 + hCoeffs->rho32v6l*eulerlogxabs
			+ (hCoeffs->rho32v8 + hCoeffs->rho32v8l*eulerlogxabs)*v2))))));
		break;
	      case 1:
		deltalm = vh3*(hCoeffs->delta31vh3 + vh3*(hCoeffs->delta31vh6
			+ vh*(hCoeffs->delta31vh7 + hCoeffs->delta31vh9*vh*vh))) 
			+ hCoeffs->delta31v5*v*v2*v2;
		rholm	= 1. + v2*(hCoeffs->rho31v2 + v*(hCoeffs->rho31v3 + v*(hCoeffs->rho31v4 
			+ v*(hCoeffs->rho31v5 + v*(hCoeffs->rho31v6 + hCoeffs->rho31v6l*eulerlogxabs 
			+ v*(hCoeffs->rho31v7 + (hCoeffs->rho31v8 + hCoeffs->rho31v8l*eulerlogxabs)*v))))));
                auxflm = v*v2*hCoeffs->f31v3;
		break;
              default:
                break;
	    }
	    break;
	  case 4:
	    switch (m)
	    {
	      case 4:
                
	        deltalm = vh3*(hCoeffs->delta44vh3 + hCoeffs->delta44vh6 *vh3)
                          + hCoeffs->delta44v5*v2*v2*v;
		rholm	= 1. + v2*(hCoeffs->rho44v2
			+ v*( hCoeffs->rho44v3 + v*(hCoeffs->rho44v4
			+ v*(hCoeffs->rho44v5 + (hCoeffs->rho44v6
			+ hCoeffs->rho44v6l*eulerlogxabs)*v))));
	        break;
	      case 3:
	        deltalm = vh3*(hCoeffs->delta43vh3 + vh*(hCoeffs->delta43vh4 
			+ hCoeffs->delta43vh6*vh*vh));
		rholm	= 1. + v*(hCoeffs->rho43v
			+ v*(hCoeffs->rho43v2
			+ v2*(hCoeffs->rho43v4 + v*(hCoeffs->rho43v5
			+ (hCoeffs->rho43v6 + hCoeffs->rho43v6l*eulerlogxabs)*v))));
                auxflm = v*hCoeffs->f43v;
	        break;
	      case 2:
		deltalm = vh3*(hCoeffs->delta42vh3 + hCoeffs->delta42vh6*vh3);
		rholm	= 1. + v2*(hCoeffs->rho42v2
			+ v*(hCoeffs->rho42v3 + v*(hCoeffs->rho42v4 + v*(hCoeffs->rho42v5
			+ (hCoeffs->rho42v6 + hCoeffs->rho42v6l*eulerlogxabs)*v))));
		break;
	      case 1:
		deltalm = vh3*(hCoeffs->delta41vh3 + vh*(hCoeffs->delta41vh4
			+ hCoeffs->delta41vh6*vh*vh));
		rholm	= 1. + v*(hCoeffs->rho41v 
			+ v*(hCoeffs->rho41v2
			+ v2*(hCoeffs->rho41v4 + v*(hCoeffs->rho41v5 
			+ (hCoeffs->rho41v6 +  hCoeffs->rho41v6l*eulerlogxabs)*v))));
                auxflm = v*hCoeffs->f41v;
		break;
	      default:
                break;
	    }
	    break;
	  case 5:
	    switch (m)
	    {
	      case 5:
	        deltalm = hCoeffs->delta55vh3*vh3 + hCoeffs->delta55v5*v2*v2*v;
		rholm	= 1. + v2*( hCoeffs->rho55v2 
			+ v*(hCoeffs->rho55v3 + v*(hCoeffs->rho55v4 
                        + v*(hCoeffs->rho55v5 + hCoeffs->rho55v6*v))));
	        break;
	      case 4:
		deltalm = vh3*(hCoeffs->delta54vh3 + hCoeffs->delta54vh4*vh);
		rholm	= 1. + v2*(hCoeffs->rho54v2 + v*(hCoeffs->rho54v3
			+ hCoeffs->rho54v4*v));
		break;
	      case 3:
	        deltalm = hCoeffs->delta53vh3 * vh3;
		rholm	= 1. + v2*(hCoeffs->rho53v2 
			+ v*(hCoeffs->rho53v3 + v*(hCoeffs->rho53v4 + hCoeffs->rho53v5*v)));
	        break;
	      case 2:
		deltalm = vh3*(hCoeffs->delta52vh3 + hCoeffs->delta52vh4*vh);
		rholm	= 1. + v2*(hCoeffs->rho52v2 + v*(hCoeffs->rho52v3
			+ hCoeffs->rho52v4*v));
		break;
	      case 1:
		deltalm = hCoeffs->delta51vh3 * vh3;
		rholm	= 1. + v2*(hCoeffs->rho51v2 
			+ v*(hCoeffs->rho51v3 + v*(hCoeffs->rho51v4 + hCoeffs->rho51v5*v)));
		break;
	      default:
                break;
	    }
	    break;
	  case 6:
	    switch (m)
	    {
	      case 6:
	        deltalm = hCoeffs->delta66vh3*vh3;
		rholm	= 1. + v2*(hCoeffs->rho66v2 + v*(hCoeffs->rho66v3
			+ hCoeffs->rho66v4*v));
	        break;
	      case 5:
		deltalm = hCoeffs->delta65vh3*vh3;
		rholm	= 1. + v2*(hCoeffs->rho65v2 + hCoeffs->rho65v3*v);
		break;
	      case 4:
		deltalm = hCoeffs->delta64vh3 * vh3;
		rholm	= 1. + v2*(hCoeffs->rho64v2 + v*(hCoeffs->rho64v3
			+ hCoeffs->rho64v4*v));
		break;
	      case 3:
	        deltalm = hCoeffs->delta63vh3 * vh3;
		rholm	= 1. + v2*(hCoeffs->rho63v2 + hCoeffs->rho63v3*v);
	        break;
	      case 2:
		deltalm = hCoeffs->delta62vh3 * vh3;
		rholm	= 1. + v2*(hCoeffs->rho62v2 + v*(hCoeffs->rho62v3
			+ hCoeffs->rho62v4 * v));
		break;
	      case 1:
		deltalm = hCoeffs->delta61vh3 * vh3;
		rholm	= 1. + v2*(hCoeffs->rho61v2 + hCoeffs->rho61v3*v);
		break;
	      default:
                break;
	    }
	    break;
	  case 7:
	    switch (m)
	    {
	      case 7:
	        deltalm = hCoeffs->delta77vh3 * vh3;
		rholm	= 1. + v2*(hCoeffs->rho77v2 + hCoeffs->rho77v3 * v);
	        break;
	      case 6:
	        deltalm = 0.0;
		rholm	= 1. + hCoeffs->rho76v2 * v2;
	        break;
	      case 5:
		deltalm = hCoeffs->delta75vh3 * vh3;
		rholm	= 1. + v2*(hCoeffs->rho75v2 + hCoeffs->rho75v3*v);
		break;
	      case 4:
		deltalm = 0.0;
		rholm	= 1. + hCoeffs->rho74v2 * v2;
		break;
	      case 3:
	        deltalm = hCoeffs->delta73vh3 *vh3;
		rholm	= 1. + v2*(hCoeffs->rho73v2 + hCoeffs->rho73v3 * v);
	        break;
	      case 2:
		deltalm = 0.0;
		rholm	= 1. + hCoeffs->rho72v2 * v2;
		break;
	      case 1:
		deltalm = hCoeffs->delta71vh3 * vh3;
		rholm	= 1. + v2*(hCoeffs->rho71v2 +hCoeffs->rho71v3 * v);
		break;
	      default:
                break;
	    }
	    break;
	  case 8:
	    switch (m)
	    {
	      case 8:
	        deltalm = 0.0;
		rholm	= 1. + hCoeffs->rho88v2 * v2;
	        break;
	      case 7:
		deltalm = 0.0;
		rholm	= 1. + hCoeffs->rho87v2 * v2;
		break;
	      case 6:
	        deltalm = 0.0;
		rholm	= 1. + hCoeffs->rho86v2 * v2;
	        break;
	      case 5:
		deltalm = 0.0;
		rholm	= 1. + hCoeffs->rho85v2 * v2;
		break;
	      case 4:
		deltalm = 0.0;
		rholm	= 1. + hCoeffs->rho84v2 * v2;
		break;
	      case 3:
	        deltalm = 0.0;
		rholm	= 1. + hCoeffs->rho83v2 * v2;
	        break;
	      case 2:
		deltalm = 0.0;
		rholm	= 1. + hCoeffs->rho82v2 * v2;
		break;
	      case 1:
		deltalm = 0.0;
		rholm	= 1. + hCoeffs->rho81v2 * v2;
		break;
	      default:
                break;
	    }
	    break;
	  default:
            break; 
	}

        /* Raise rholm to the lth power */
        rholmPwrl = 1.0;
        i = l;
        while ( i-- )
        {
          rholmPwrl *= rholm;
        }
        /* In the equal-mass odd m case, there is no contribution from nonspin terms,  
         * and the only contribution comes from the auxflm term that is proportional to chiA (asymmetric spins). 
         * In this case, we must ignore the nonspin terms directly, since the leading term defined by 
         * CalculateThisMultipolePrefix in LALSimIMREOBNewtonianMultipole.c is not zero (see comments there).
         */ 
        if (eta == 0.25 && m % 2)
        {
          rholmPwrl = auxflm;
        }
        else
        {
          rholmPwrl += auxflm;
        }

        /*if (r > 8.5)
	{
	  printf("YP::dynamics variables in waveform: %i, %i, %e, %e\n",l,m,r,pp); 
	  printf( "rholm^l = %.16e, Tlm = %.16e + i %.16e, \nSlm = %.16e, hNewton = %.16e + i %.16e, delta = %.16e\n", rholmPwrl, Tlm.re, Tlm.im, Slm, hNewton.re, hNewton.im, deltalm );}*/
        /* Put all factors in Eq. 17 together, Eq.(14) of PRD 84, 124052 (2011) */
		
	*hlm = Tlm * cexp(ci * deltalm) * Slm * rholmPwrl;
	//printf("multiplied factor (l=%i,m=%i) = %e\n",l,m,abs(*hlm)); 
        *hlm *= hNewton;
	/*if (r > 8.5)
	{
	  printf("YP::FullWave: %.16e,%.16e, %.16e\n",hlm->re,hlm->im,sqrt(hlm->re*hlm->re+hlm->im*hlm->im));
	}*/
	return 0;
}

// Eq.(16) of Y. Pan et al, PRD 84, 124052 (2011)
 int
XLALSimIMRSpinEOBCalculateNewtonianMultipole(
                 COMPLEX16 *multipole, /**<< OUTPUT, Newtonian multipole */
                 REAL8 x,              /**<< Dimensionless parameter \f$\equiv v^2\f$ */
                 REAL8 r,       /**<< Orbital separation (units of total mass M */
                 REAL8 phi,            /**<< Orbital phase (in radians) */
                 UINT4  l,             /**<< Mode l */
                 int  m,              /**<< Mode m */
                 EOBParams *params     /**<< Pre-computed coefficients, parameters, etc. */
                 )
{
   int xlalStatus;

   COMPLEX16 y;

   int epsilon = (l + m) % 2;

   y = 0.0;

  /* Calculate the necessary Ylm */
  xlalStatus = XLALScalarSphHarmThetaPiBy2( &y, l - epsilon, - m, phi );
  if (xlalStatus != -1 )
  {
  }
  // a little different to the factor V^l_\Phi in Eq.(16) of Y Pan et al, PRD 84, 124052 (2011)
  // Eq.(1) and (4) of PRD 79, 064004 (2009)
  *multipole = params->prefixes->values[l][m] * pow( x, (REAL8)(l+epsilon)/2.0) ;
  *multipole *= y;

  return 0;
}

 int
XLALScalarSphHarmThetaPiBy2(
                 COMPLEX16 *y, /**<< OUTPUT, Ylm(pi/2,phi) */
                 int l,       /**<< Mode l */
                 int  m,      /**<< Mode m */
                 REAL8 phi     /**<< Orbital phase (in radians) */
                 )
{

  REAL8 legendre;
  int absM = abs( m );

  if ( l < 0 || absM > (INT4) l )
  {
  }

  /* For some reason GSL will not take negative m */
  /* We will have to use the relation between sph harmonics of +ve and -ve m */
  legendre = XLALAssociatedLegendreXIsZero( l, absM );
  if ( XLAL_IS_REAL8_FAIL_NAN( legendre ))
  {
  }

  /* Compute the values for the spherical harmonic */
  *y = legendre * cos(m * phi);
  *y += I * legendre * sin(m * phi);

  /* If m is negative, perform some jiggery-pokery */
  if ( m < 0 && absM % 2  == 1 )
  {
    *y *= -1.0;
  }

  return 0;
}

/**
 * Function to calculate associated Legendre function used by the spherical harmonics function
 */
 REAL8
XLALAssociatedLegendreXIsZero( const int l,
                               const int m )
{

  REAL8 legendre;

  if ( l < 0 )
  {
  }
  
  if ( m < 0 || m > l )
  {
  }

  /* we will switch on the values of m and n */
  switch ( l )
  {
    case 1:
      switch ( m )
      {
        case 1:
          legendre = - 1.;
          break;
        default:
			break;
      }
      break;
    case 2:
      switch ( m )
      {
        case 2:
          legendre = 3.;
          break;
        case 1:
          legendre = 0.;
          break;
        default:
			break;
      }
      break;
    case 3:
      switch ( m )
      {
        case 3:
          legendre = -15.;
          break;
        case 2:
          legendre = 0.;
          break;
        case 1:
          legendre = 1.5;
          break;
        default:
			break;
      }
      break;
    case 4:
      switch ( m )
      {
        case 4:
          legendre = 105.;
          break;
        case 3:
          legendre = 0.;
          break;
        case 2:
          legendre = - 7.5;
          break;
        case 1:
          legendre = 0;
          break;
        default:
			break;
      }
      break;
    case 5:
      switch ( m )
      {
        case 5:
          legendre = - 945.;
          break;
        case 4:
          legendre = 0.;
          break;
        case 3:
          legendre = 52.5;
          break;
        case 2:
          legendre = 0;
          break;
        case 1:
          legendre = - 1.875;
          break;
        default:
			break;
      }
      break;
    case 6:
      switch ( m )
      {
        case 6:
          legendre = 10395.;
          break;
        case 5:
          legendre = 0.;
          break;
        case 4:
          legendre = - 472.5;
          break;
        case 3:
          legendre = 0;
          break;
        case 2:
          legendre = 13.125;
          break;
        case 1:
          legendre = 0;
          break;
        default:
			break;
      }
      break;
    case 7:
      switch ( m )
      {
        case 7:
          legendre = - 135135.;
          break;
        case 6:
          legendre = 0.;
          break;
        case 5:
          legendre = 5197.5;
          break;
        case 4:
          legendre = 0.;
          break;
        case 3:
          legendre = - 118.125;
          break;
        case 2:
          legendre = 0.;
          break;
        case 1:
          legendre = 2.1875;
          break;
        default:
			break;
      }
      break;
    case 8:
      switch ( m )
      {
        case 8:
          legendre = 2027025.;
          break;
        case 7:
          legendre = 0.;
          break;
        case 6:
          legendre = - 67567.5;
          break;
        case 5:
          legendre = 0.;
          break;
        case 4:
          legendre = 1299.375;
          break;
        case 3:
          legendre = 0.;
          break;
        case 2:
          legendre = - 19.6875;
          break;
        case 1:
          legendre = 0.;
          break;
        default:
			break;
      }
      break;
    default:
      fprintf( stderr,"Unsupported (l, m): %d, %d\n", l, m );
  }

  legendre *= sqrt( (REAL8)(2*l+1)*gsl_sf_fact( l-m ) / (4.*LAL_PI*gsl_sf_fact(l+m)));

  return legendre;
}

 REAL8 XLALSimIMRSpinEOBHamiltonianDeltaR(
        SpinEOBHCoeffs *coeffs, /**<< Pre-computed coefficients which appear in the function */
        const REAL8    r,       /**<< Current orbital radius (in units of total mass) */
        const REAL8    eta,     /**<< Symmetric mass ratio */
        const REAL8    a        /**<< Normalized deformed Kerr spin */
        )
{

  
  REAL8 u2, u3;
  REAL8 D;
  REAL8 deltaT; /* The potential function, not a time interval... */
  REAL8 deltaR;

  u2 = 1./(r*r);
  u3 = u2 / r;

  D = 1. + log(1. + 6.*eta*u2 + 2.*(26. - 3.*eta)*eta*u3);

  deltaT = XLALSimIMRSpinEOBHamiltonianDeltaT( coeffs, r, eta, a );

  deltaR = deltaT*D;
  return deltaR;
}

 REAL8 XLALSimIMRSpinEOBHamiltonianDeltaT( 
        SpinEOBHCoeffs *coeffs, /**<< Pre-computed coefficients which appear in the function */
        const REAL8    r,       /**<< Current orbital radius (in units of total mass) */
        const REAL8    eta,     /**<< Symmetric mass ratio */
        const REAL8    a        /**<< Normalized deformed Kerr spin */
        )

{

  REAL8 a2;
  REAL8 u, u2, u3, u4;
  REAL8 m1PlusetaKK;

  REAL8 bulk;
  REAL8 logTerms;
  REAL8 deltaU;
  REAL8 deltaT;

  u  = 1./r;
  u2 = u*u;
  u3 = u2*u;
  u4 = u2*u2;

  a2 = a*a;

  m1PlusetaKK = -1. + eta * coeffs->KK;

  bulk = 1./(m1PlusetaKK*m1PlusetaKK) + (2.*u)/m1PlusetaKK + a2*u2;

  logTerms = 1. + eta*coeffs->k0 + eta*log(1. + coeffs->k1*u + coeffs->k2*u2 + coeffs->k3*u3 + coeffs->k4*u4);
  //printf( "bulk = %.16e, logTerms = %.16e\n", bulk, logTerms );
  deltaU = bulk*logTerms;

  deltaT = r*r*deltaU;

  return deltaT;
}

/* ark4GSLIntegrator is in Panyidatatypes.h */
ark4GSLIntegrator *XLALAdaptiveRungeKutta4Init( int dim,
                             int (* dydt) (double t, const double y[], double dydt[], void * params),  /* These are XLAL functions! */
                             int (* stop) (double t, const double y[], double dydt[], void * params),
                             double eps_abs, double eps_rel
                  				 )
{
  ark4GSLIntegrator *integrator;

	/* allocate our custom integrator structure */
  if (!(integrator = (ark4GSLIntegrator *) calloc(1, sizeof(ark4GSLIntegrator))))
	{
	}

  /* allocate the GSL ODE components */
  integrator->step    = gsl_odeiv_step_alloc(gsl_odeiv_step_rkf45,dim) ;
  integrator->control = gsl_odeiv_control_y_new(eps_abs,eps_rel);
  integrator->evolve  = gsl_odeiv_evolve_alloc(dim);

  /* allocate the GSL system (functions, etc.) */
  integrator->sys = (gsl_odeiv_system *)calloc(1, sizeof(gsl_odeiv_system));

  /* if something failed to be allocated, bail out */
  if ( !(integrator->step) || !(integrator->control) || !(integrator->evolve) || !(integrator->sys) )
  {
    XLALAdaptiveRungeKutta4Free(integrator);
  }

  integrator->dydt = dydt;
  integrator->stop = stop;

  integrator->sys->function  = dydt;
  integrator->sys->jacobian  = NULL;
  integrator->sys->dimension = dim;
  integrator->sys->params    = NULL;

  integrator->retries = 6;
  integrator->stopontestonly = 0;

  return integrator;
}

void XLALAdaptiveRungeKutta4Free( ark4GSLIntegrator *integrator )
{
  if (!integrator) return;

  if (integrator->evolve)  gsl_odeiv_evolve_free(integrator->evolve);
  if (integrator->control) gsl_odeiv_control_free(integrator->control);
  if (integrator->step)    gsl_odeiv_step_free(integrator->step);

  LALFree( integrator->sys );
  LALFree( integrator );

  return;
}

 int XLALSpinAlignedHcapDerivative(
                  double t,          /**< UNUSED */
                  const REAL8   values[],   /**< dynamical varables, r, phi, pr, pphi */
                  REAL8         dvalues[],  /**< time derivative of dynamical variables */
                  void         *funcParams  /**< EOB parameters */
                  )
{
  //printf("%.8f %.8f %.8f %.8f\n",values[0],values[1],values[2],values[3]);
  //exit(0); 

  static const REAL8 STEP_SIZE = 1.0e-4;

  static const INT4 lMax = 8;

  HcapDerivParams params;

  /* Since we take numerical derivatives wrt dynamical variables */
  /* but we want them wrt time, we use this temporary vector in  */
  /* the conversion */
  REAL8           tmpDValues[6];

  /* Cartesian values for calculating the Hamiltonian */
  REAL8           cartValues[6];

  REAL8           H; //Hamiltonian
  REAL8           flux;

  gsl_function F;
  INT4         gslStatus;

  UINT4 i;

  REAL8Vector rVec, pVec;
  REAL8 rData[3], pData[3];

  /* We need r, phi, pr, pPhi to calculate the flux */
  REAL8       r;
  REAL8Vector polarDynamics;
  REAL8       polData[4];

  REAL8 mass1, mass2, eta;

  /* Spins */
  REAL8Vector *sKerr = NULL;
  REAL8Vector *sStar = NULL;
  REAL8Vector *Spin1 = NULL;
  REAL8Vector *Spin2 = NULL;

  REAL8 a;

  REAL8 omega;

  /* EOB potential functions */
  REAL8 DeltaT, DeltaR;
  REAL8 csi;

  /* The error in a derivative as measured by GSL */
  REAL8 absErr;

  /* Set up pointers for GSL */ 
  params.values  = cartValues;
  params.params  = (SpinEOBParams *)funcParams;

  sKerr = params.params->sigmaKerr;
  sStar = params.params->sigmaStar;

  Spin1 = params.params->Spin1;
  Spin2 = params.params->Spin2;
    
  F.function = &GSLSpinAlignedHamiltonianWrapper;
  F.params   = &params;

  mass1 = params.params->eobParams->m1;
  mass2 = params.params->eobParams->m2;
  eta   = params.params->eobParams->eta;

  r = values[0];

  /* Since this is spin aligned, I make the assumption */
  /* that the spin vector is along the z-axis.         */
  a  = sKerr->data[2];

  /* Calculate the potential functions and the tortoise coordinate factor csi,
     given by Eq. 28 of Pan et al. PRD 81, 084041 (2010) */
  DeltaT = XLALSimIMRSpinEOBHamiltonianDeltaT( params.params->seobCoeffs, r, eta, a );
  DeltaR = XLALSimIMRSpinEOBHamiltonianDeltaR( params.params->seobCoeffs, r, eta, a );
  csi    = sqrt( DeltaT * DeltaR ) / (r*r + a*a);
  //printf( "csi in derivatives function = %.16e\n", csi );

  /* Populate the Cartesian values vector, using polar coordinate values */
  /* We can assume phi is zero wlog */
  memset( cartValues, 0, sizeof( cartValues ) );
  cartValues[0] = values[0];
  cartValues[3] = values[2];
  cartValues[4] = values[3] / values[0];

  /* Now calculate derivatives w.r.t. each Cartesian variable */
  for ( i = 0; i < 6; i++ )
  {
    params.varyParam = i;
    gslStatus = gsl_deriv_central( &F, cartValues[i], 
                    STEP_SIZE, &tmpDValues[i], &absErr );

    if ( gslStatus != GSL_SUCCESS )
    {
      fprintf( stderr,"XLAL Error : Failure in GSL function\n" );
    }
  }

  /* Calculate the Cartesian vectors rVec and pVec */
  polarDynamics.length = 4;
  polarDynamics.data   = polData;

  memcpy( polData, values, sizeof( polData ) );

  rVec.length = pVec.length = 3;
  rVec.data   = rData;
  pVec.data   = pData;

  memset( rData, 0, sizeof(rData) );
  memset( pData, 0, sizeof(pData) );

  rData[0] = values[0];
  pData[0] = values[2];
  pData[1] = values[3] / values[0];
  /* Calculate Hamiltonian using Cartesian vectors rVec and pVec */
  H =  XLALSimIMRSpinEOBHamiltonian( eta, &rVec, &pVec, sKerr, sStar, params.params->tortoise, params.params->seobCoeffs ); 

  //printf( "csi = %.16e, ham = %.16e ( tortoise = %d)\n", csi, H, params.params->tortoise );
  //exit(1);
  //printf( "Hamiltonian = %e\n", H );
  H = H * (mass1 + mass2);

  //printf( "Cartesian derivatives:\n%.16e %.16e %.16e %.16e %.16e %.16e\n",
  //    dx/dt, dy/dt, dz/dt, dpx/dt, dpy/dt, dpz/dt
  //    tmpDValues[3], tmpDValues[4], tmpDValues[5], -tmpDValues[0], -tmpDValues[1], -tmpDValues[2] );
  //    r, phi, pr, pphi
  //    values[0], values[1], values[2], values[3]

  /* Now calculate omega, and hence the flux */
  omega = tmpDValues[4] / r;

  /* Now we can calculate the final (spherical) derivatives */
  /* csi is needed because we use the tortoise co-ordinate */
  /* Right hand side of Eqs. 10a - 10d of Pan et al. PRD 84, 124052 (2011) */
  dvalues[0] = csi * tmpDValues[3];
  dvalues[1] = omega;

  // Eq.(6.12) of PRD 54, 4813 (1996)
  REAL8 m=mass1+mass2;
  REAL8 v = MYcbrt( omega );
  REAL8 rd = dvalues[0];
#if 0 
  //compare to Wenbiao IJMPD
  flux = 0;  // works for mass ratio 1:1000
#elif 0
  // does not work for mass ratio 1:1000
  flux  = XLALInspiralSpinFactorizedFlux( &polarDynamics, omega, params.params, H/m, lMax );  
#elif 0
  //works for mass ratio 1:1000
  flux = 8./15*eta*eta*pow(m/r,4)*(
         (12*v*v-11*rd*rd)   // (6.13a) of PRD 54, 4813 (1996); (3.25a) of PRD 52, 821 (1995)
	+((785-852*eta)*pow(v,4)-2*(1487-1392*eta)*v*v*rd*rd  
          +3*(687-620*eta)*pow(rd,4)-160*(17-eta)*m/r*v*v
	  +8*(367-15*eta)*m/r*rd*rd+16*(1-4*eta)*pow(m/r,2))/28  // (6.13b) of PRD 54, 4813 (1996); (3.25b) of PRD 52, 821 (1995)
	+(18*(1692-5497*eta+4430*eta*eta)*pow(v,6)-54*(1719-10278*eta+6292*eta*eta)*pow(v,4)*rd*rd
	 +54*(2018-15207*eta+7572*eta*eta)*v*v*pow(rd,4)-18*(2501-20234*eta+8404*eta*eta)*pow(rd,6)
	 -12*(33510-60971*eta+14290*eta*eta)*m/r*pow(rd,4)-36*(4446-5237*eta+1393*eta*eta)*m/r*pow(v,4)
	 +108*(4987-8513*eta+2165*eta*eta)*m/r*rd*rd*v*v-3*(106319+9798*eta+5376*eta*eta)*m/r*m/r*rd*rd
	 +(281473+81828*eta+4368*eta*eta)*m/r*m/r*v*v-24*(253-1026*eta+56*eta*eta)*pow(m/r,3))/756    // (6.13d) of PRD 54, 4813 (1996)
	);
#elif 0  // SEOBNREv2, this flux does not work for almost equal mass cases
  //works for mass ratio 1:1000
  flux = EOBPNflux(mass1/(mass1+mass2),mass2/(mass1+mass2),Spin1->data[0],Spin1->data[1],Spin1->data[2],
                           Spin2->data[0],Spin2->data[1],Spin2->data[2],
		 	   cartValues[0], cartValues[1], cartValues[2],
		 	   dvalues[0],dvalues[1]*r,0);
#elif 0   // SEOBNREv1
  //compare flux

  double fluxwoQNC;
  flux = EOBPNflux(mass1/(mass1+mass2),mass2/(mass1+mass2),
                   Spin1->data[0],Spin1->data[1],Spin1->data[2],
                   Spin2->data[0],Spin2->data[1],Spin2->data[2],
                   cartValues[0], cartValues[1], cartValues[2],
                   dvalues[0],dvalues[1]*r,0,
                   &fluxwoQNC);
  //flux = EOBPNflux(1.,1.,
  //                 0.1,0.1,0.1,
  //                 0.1,0.1,0.1,
  //                 0.5,0.5,0.5,
  //                 0.5,0.5,0.5,
  //	   &fluxwoQNC);
#endif
    
#if 0
  static FILE *out = 0;
  if(!out) out= fopen( "compareflux.dat", "w" );
  fprintf( out, "%.16e %.16e %.16e ", t, fluxwoQNC, flux);
#endif
  //
  /* Note: in this special coordinate setting, namely y = z = 0, dpr/dt = dpx/dt + dy/dt * py/r, where py = pphi/r */ 
  dvalues[2] = - tmpDValues[0] + tmpDValues[4] * values[3] / (r*r);
  dvalues[2] = dvalues[2] * csi;
  dvalues[3] = 0;
  flux = XLALInspiralSpinFactorizedFlux_elip(&polarDynamics, values, dvalues, omega, params.params, H/m, lMax);
#if 0
  fprintf( out, "%.16e\n", flux);
#endif
    
  // because Eq.(8) of PRD 85, 024046 (2012)
  // compared to Eq.(23) of PRD 81, 084041 (2010), factor 1/eta is added
  flux = flux / eta;

  //printf( "Flux in derivatives function = %.16e\n", flux );

  //printf("%.8f %.8f %.8f %.8f %.8f %.8f\n",polData[0],polData[1],polData[2],polData[3],omega,H/m);
  //printf("%.8f %.8f\n",values[3],flux);
  //exit(0); 

  /* Note: in this special coordinate setting, namely y = z = 0, dpr/dt = dpx/dt + dy/dt * py/r, where py = pphi/r */ 
  dvalues[2] = - tmpDValues[0] + tmpDValues[4] * values[3] / (r*r);
  dvalues[2] = dvalues[2] * csi - ( values[2] / values[3] ) * flux / omega;
  dvalues[3] = - flux / omega;

  //printf("Values:\n%.16e %.16e %.16e %.16e\n", values[0], values[1], values[2], values[3] );

  //printf("Derivatives:\n%.16e %.16e %.16e %.16e\n", dvalues[0], dvalues[1], dvalues[2], dvalues[3] );

  /*if ( isnan( dvalues[0] ) || isnan( dvalues[1] ) || isnan( dvalues[2] ) || isnan( dvalues[3] ) )
  {
    //printf( "Deriv is nan: %e %e %e %e\n", dvalues[0], dvalues[1], dvalues[2], dvalues[3] );
    return 1;
  }*/

  return 0;
}

 double GSLSpinAlignedHamiltonianWrapper( double x, void *params )
{
  HcapDerivParams *dParams = (HcapDerivParams *)params;

  EOBParams *eobParams = dParams->params->eobParams;

  REAL8 tmpVec[6];

  /* These are the vectors which will be used in the call to the Hamiltonian */
  REAL8Vector r, p;
  REAL8Vector *sigmaKerr = dParams->params->sigmaKerr;
  REAL8Vector *sigmaStar = dParams->params->sigmaStar;

  /* Use a temporary vector to avoid corrupting the main function */
  memcpy( tmpVec, dParams->values, 
               sizeof(tmpVec) );

  /* Set the relevant entry in the vector to the correct value */
  tmpVec[dParams->varyParam] = x;

  /* Set the LAL-style vectors to point to the appropriate things */
  r.length = p.length = 3;
  r.data     = tmpVec;
  p.data     = tmpVec+3;

  return XLALSimIMRSpinEOBHamiltonian( eobParams->eta, &r, &p, sigmaKerr, sigmaStar, dParams->params->tortoise, dParams->params->seobCoeffs ) / eobParams->eta;
}

 int
XLALEOBSpinAlignedStopCondition(double  t,  /**< UNUSED */
                           const double values[], /**< dynamical variable values */
                           double dvalues[],      /**< dynamical variable time derivative values */
                           void *funcParams       /**< physical parameters */
                          )
{

  REAL8 omega, r;
  SpinEOBParams *params = (SpinEOBParams *)funcParams;

  r     = values[0];
  omega = dvalues[1];

  if ( r < 6. && omega < params->eobParams->omega )
  {
    return 1;
  }

  params->eobParams->omega = omega;
  return 0;
}

int XLALAdaptiveRungeKutta4( ark4GSLIntegrator *integrator,
                         void *params,
                         REAL8 *yinit,
                         REAL8 tinit, REAL8 tend, REAL8 deltat,
                         REAL8Array **yout
                         )
{
  int status; /* used throughout */

	/* needed for the integration */
  size_t dim, bufferlength, cnt, retries;
	REAL8 t, tnew, h0;
  REAL8Array *buffers = NULL;
  REAL8 *temp = NULL, *y, *y0, *dydt_in, *dydt_in0, *dydt_out, *yerr; /* aliases */

	/* needed for the final interpolation */
  gsl_spline *interp = NULL;
  gsl_interp_accel *accel = NULL;
  int outputlen = 0;
  REAL8Array *output = NULL;
  REAL8 *times, *vector;	/* aliases */

  /* allocate the buffers!
	   note: REAL8Array has a field dimLength (UINT4Vector) with dimensions, and a field data that points to a single memory block;
	         dimLength itself has fields length and data */
  dim = integrator->sys->dimension;
  bufferlength = (int)((tend - tinit)/deltat) + 2;    		/* allow for the initial value and possibly a final semi-step */
  buffers = XLALCreateREAL8ArrayL(2,dim+1,bufferlength);	/* 2-dimensional array, (dim+1) x bufferlength */
  temp = (REAL8 *)calloc(6*dim,sizeof(REAL8));

  if (!buffers || !temp) {
    status = XLAL_ENOMEM;
    goto bail_out;
  }

	y = temp; y0 = temp + dim; dydt_in = temp + 2*dim; dydt_in0 = temp + 3*dim; dydt_out = temp + 4*dim; yerr = temp + 5*dim; /* aliases */

  /* set up to get started */
  integrator->sys->params = params;

	integrator->returncode = 0;

  cnt = 0;
  retries = integrator->retries;

	t  = tinit; h0 = deltat;
  memcpy(y,yinit,dim*sizeof(REAL8));

  /* store the first data point */
  buffers->data[0] = t;
  unsigned int i;
  for(i=1;i<=dim;i++) buffers->data[i*bufferlength] = y[i-1];
  /* compute derivatives at the initial time (dydt_in), bail out if impossible */
  if ((status = integrator->dydt(t,y,dydt_in,params)) != GSL_SUCCESS) {
		integrator->returncode = status;
		goto bail_out;
	}

	/* note: for speed, this replaces the single CALLGSL wrapper applied before each GSL call */
	//XLAL_BEGINGSL;
  while(1) 
  {

	if (!integrator->stopontestonly && t >= tend)
	{
	    break;
	}

	if (integrator->stop)
	{
      	if ((status = integrator->stop(t,y,dydt_in,params)) != GSL_SUCCESS)
		{
			integrator->returncode = status;
			break;
		}
//		printf("\r\nr,phi,pr,pt=%.4f %.4f %.8f %.8f\n",y[0],y[1],y[2],y[3]);
	}

		/* ready to try stepping! */
    try_step:

		/* if we would be stepping beyond the final time, stop there instead... */
    if (!integrator->stopontestonly && t + h0 > tend)
		h0 = tend - t;

    memcpy(y0      ,y,      dim*sizeof(REAL8));   /* save y to y0, dydt_in to dydt_in0 */
	memcpy(dydt_in0,dydt_in,dim*sizeof(REAL8));

    /* call the GSL stepper function */
	status = gsl_odeiv_step_apply(integrator->step,t,h0,y,yerr,dydt_in,dydt_out,integrator->sys);
	/* note: If the user-supplied functions defined in the system dydt return a status other than GSL_SUCCESS,
	   the step will be aborted. In this case, the elements of y will be restored to their pre-step values,
	   and the error code from the user-supplied function will be returned. */

	/* did the stepper report a derivative-evaluation error? */
    if (status != GSL_SUCCESS) {
      if (retries--) {
        h0 = h0 / 10.0;               /* if we have singularity retries left, reduce the timestep and try again */
        goto try_step;
      } else {
				integrator->returncode = status;
				break;                        /* otherwise exit the loop */
      }
    } else {
      retries = integrator->retries;  /* we stepped successfully, reset the singularity retries */
    }

    tnew = t + h0;

    /* call the GSL error-checking function */
    status = gsl_odeiv_control_hadjust(integrator->control,integrator->step,y,yerr,dydt_out,&h0);

		/* did the error-checker reduce the stepsize?
		   note: other possible return codes are GSL_ODEIV_HADJ_INC if it was increased,
																						 GSL_ODEIV_HADJ_NIL if it was unchanged */
    if (status == GSL_ODEIV_HADJ_DEC) {
      memcpy(y,      y0,      dim*sizeof(REAL8));	/* if so, undo the step, and try again */
			memcpy(dydt_in,dydt_in0,dim*sizeof(REAL8));
			goto try_step;
    }

		/* update the current time and input derivatives */
    t = tnew;
    memcpy(dydt_in,dydt_out,dim*sizeof(REAL8));
    cnt++;

    /* check if interpolation buffers need to be extended */
    if (cnt >= bufferlength) {
			REAL8Array *rebuffers;

				/* sadly, we cannot use ResizeREAL8Array, because it would only work if we extended the first array dimension,
					 so we need to copy everything over and switch the buffers. Oh well. */
      if (!(rebuffers = XLALCreateREAL8ArrayL(2,dim+1,2*bufferlength))) {
        status = XLAL_ENOMEM;	/* ouch, that hurt */
        goto bail_out;
      } else {
				for(i=0;i<=dim;i++) memcpy(&rebuffers->data[i*2*bufferlength],&buffers->data[i*bufferlength],cnt*sizeof(REAL8));
				XLALDestroyREAL8Array(buffers); buffers = rebuffers;
				bufferlength *= 2;
			}
    }

    /* copy time and state into interpolation buffers */
    buffers->data[cnt] = t;
    for(i=1;i<=dim;i++) 
		buffers->data[i*bufferlength + cnt] = y[i-1]; /* y does not have time */
	
  }
	//XLAL_ENDGSL;

	/* copy the final state into yinit */

	memcpy(yinit,y,dim*sizeof(REAL8));

  /* if we have completed at least one step, allocate the GSL interpolation object and the output array */
  if (cnt == 0) goto bail_out;

  //XLAL_CALLGSL( interp = gsl_spline_alloc(gsl_interp_cspline,cnt+1) );
   interp = gsl_spline_alloc(gsl_interp_cspline,cnt+1);
  //XLAL_CALLGSL( accel  = gsl_interp_accel_alloc() );
  accel  = gsl_interp_accel_alloc();

  outputlen = (int)(t / deltat) + 1;
  output = XLALCreateREAL8ArrayL(2,dim+1,outputlen);

  if (!interp || !accel || !output) {
    status = XLAL_ENOMEM;	/* ouch again, ran out of memory */
	  if (output) XLALDestroyREAL8Array(output);
		outputlen = 0;
    goto bail_out;
  }

  /* make an array of times */
  times = output->data;
  unsigned int j;
  for(j=0;j<outputlen;j++) times[j] = tinit + deltat * j;

  /* interpolate! */
	//XLAL_BEGINGSL;
  for(i=1;i<=dim;i++) {
    gsl_spline_init(interp,&buffers->data[0],&buffers->data[bufferlength*i],cnt+1);

    vector = output->data + outputlen*i;
    for(j=0;j<outputlen;j++) {
      gsl_spline_eval_e(interp,times[j],accel, &(vector[j]));
    }
  }
	//XLAL_ENDGSL;

  /* deallocate stuff and return */
  bail_out:

  if (buffers) XLALDestroyREAL8Array(buffers);   /* let's be careful, although all these checks may not be needed */
  if (temp)    LALFree(temp);

  if (interp)  gsl_spline_free(interp);
  //if (interp)  XLAL_CALLGSL( gsl_spline_free(interp) );
  if (accel)   gsl_interp_accel_free(accel);
//  if (accel)   XLAL_CALLGSL( gsl_interp_accel_free(accel) );

  if (status == XLAL_ENOMEM)
  	//XLAL_ERROR(XLAL_ENOMEM);	/* TO DO: will this return? */
  	fprintf(stderr,"Error!");

	*yout = output;
  return outputlen; /* TO DO: check XLAL error reporting conventions */
}


int XLALSpinAlignedHiSRStopCondition(double t,  /**< UNUSED */
                           const double values[], /**< dynamical variable values */
                           double dvalues[],      /**< dynamical variable time derivative values */
                           void *funcParams       /**< physical parameters */
                          )
{
  SpinEOBParams *params = (SpinEOBParams *)funcParams;
  REAL8 K, eta;
  eta = params->eobParams->eta;
  K = 1.4467 -  1.7152360250654402 * eta - 3.246255899738242 * eta * eta;

  //if ( values[0] <= (1.+sqrt(1-params->a * params->a))*(1.-K*eta) + 0.3 || isnan( dvalues[3] ) || isnan (dvalues[2]) || isnan (dvalues[1]) || isnan (dvalues[0]) )
  if ( values[0] <= (1.+sqrt(1-params->a * params->a))*(1.-K*eta) + 0.3  )
  {
    return 1;
  }
  return GSL_SUCCESS;
}

/**
 * Function to calculate the value of omega for the spin-aligned EOB waveform.
 * Can NOT be used in precessing cases. This omega is defined as \f$\dot{y}/r\f$ by setting \f$y=0\f$.
 * The function calculates omega = v/r, by first converting (r,phi,pr,pphi) to Cartesian coordinates 
 * in which rVec={r,0,0} and pVec={0,pphi/r,0}, i.e. the effective-test-particle is positioned at x=r, 
 * and its velocity along y-axis. Then it computes omega, which is now given by dydt/r = (dH/dp_y)/r. 
 */
 REAL8
XLALSimIMRSpinAlignedEOBCalcOmega(
                      const REAL8           values[],   /**<< Dynamical variables */
                      SpinEOBParams         *funcParams /**<< EOB parameters */
                      )
{
  static const REAL8 STEP_SIZE = 1.0e-4;

  HcapDerivParams params;

  /* Cartesian values for calculating the Hamiltonian */
  REAL8 cartValues[6];

  gsl_function F;
  INT4         gslStatus;

  REAL8 omega;
  REAL8 r;

  /* The error in a derivative as measured by GSL */
  REAL8 absErr;

  /* Set up pointers for GSL */
  params.values  = cartValues;
  params.params  = funcParams;

  F.function = &GSLSpinAlignedHamiltonianWrapper;
  F.params   = &params;

  /* Populate the Cartesian values vector */
  /* We can assume phi is zero wlog */
  memset( cartValues, 0, sizeof( cartValues ) );
  cartValues[0] = r = values[0];
  cartValues[3] = values[2];
  cartValues[4] = values[3] / values[0];

  /* Now calculate omega. In the chosen co-ordinate system, */
  /* we need dH/dpy to calculate this, i.e. varyParam = 4   */
  params.varyParam = 4;
  gslStatus = gsl_deriv_central( &F, cartValues[4],
                  STEP_SIZE, &omega, &absErr );

  if ( gslStatus != GSL_SUCCESS )
  {
    fprintf(stderr, "XLAL Error - : Failure in GSL function\n" );
  }
  
  omega = omega / r;

  return omega;
}
 REAL8
XMYSimIMRSpinAlignedEOBCalcOmega(
                      const REAL8           values[],   /**<< Dynamical variables */
                      SpinEOBParams         *funcParams /**<< EOB parameters */
                      )
{
  static const REAL8 STEP_SIZE = 1.0e-4;

  HcapDerivParams params;

  /* Cartesian values for calculating the Hamiltonian */
  REAL8 cartValues[6];

  gsl_function F;
  INT4         gslStatus;

  REAL8 omega;
  REAL8 r;

  /* The error in a derivative as measured by GSL */
  REAL8 absErr;

  /* Set up pointers for GSL */
  params.values  = cartValues;
  params.params  = funcParams;

  F.function = &GSLSpinAlignedHamiltonianWrapper;
  F.params   = &params;

  /* Populate the Cartesian values vector */
  /* We can assume phi is zero wlog */
  memset( cartValues, 0, sizeof( cartValues ) );
  cartValues[0] = r = values[0];
  cartValues[3] = values[2];
  cartValues[4] = values[3] / values[0];

  /* Now calculate omega. In the chosen co-ordinate system, */
  /* we need dH/dpy to calculate this, i.e. varyParam = 4   */
  params.varyParam = 4;
  gslStatus = gsl_deriv_central( &F, cartValues[4],
                  STEP_SIZE, &omega, &absErr );

  if ( gslStatus != GSL_SUCCESS )
  {
    fprintf( stderr,"XLAL Error - : Failure in GSL function\n" );
  }
  
  /* Now calculate omega. In the chosen co-ordinate system, */
  /* we need dH/dpx to calculate this, i.e. varyParam = 3   */
  const REAL8 gamma=1;
  REAL8 vx;
  params.varyParam = 3;
  gslStatus = gsl_deriv_central( &F, cartValues[3],
                  STEP_SIZE, &vx, &absErr );

  if ( gslStatus != GSL_SUCCESS )
  {
    fprintf( stderr,"XLAL Error - : Failure in GSL function\n" );
  }
  //printf("factor = %f, vx = %f, px = %f\n",pow(omega/(omega+vx),gamma),vx,cartValues[3]);
  omega *=pow(omega/(omega+vx),gamma);

  omega = omega / r;

  return omega;
}

/** Adds a double to a GPS time. */
LIGOTimeGPS * XLALGPSAdd( LIGOTimeGPS *epoch, REAL8 dt )
{
  LIGOTimeGPS dt_gps;
  if(!XLALGPSSetREAL8(&dt_gps, dt))
    fprintf(stderr,"error!");
  return XLALGPSAddGPS(epoch, &dt_gps);
}


/** Sets GPS time given GPS seconds as a REAL8. */
LIGOTimeGPS * XLALGPSSetREAL8( LIGOTimeGPS *epoch, REAL8 t )
{
  INT4 gpssec = (INT4)floor(t);
  INT4 gpsnan = (INT4)floor((t - gpssec) * XLAL_BILLION_REAL8 + 0.5);
 /* if(isnan(t)) {
    printf("%s(): NaN", __func__);
  }*/
  if(fabs(t) > 0x7fffffff) {
    fprintf(stderr,"(): overflow %g", t);
  }
  /* use XLALGPSSet() to normalize the nanoseconds */
  return XLALGPSSet(epoch, gpssec, gpsnan);
}

/** Adds two GPS times. */
LIGOTimeGPS * XLALGPSAddGPS( LIGOTimeGPS *epoch, const LIGOTimeGPS *dt )
{
  return XLALINT8NSToGPS( epoch, XLALGPSToINT8NS( epoch ) + XLALGPSToINT8NS( dt ) );
}

/** Sets GPS time given GPS integer seconds and residual nanoseconds. */
LIGOTimeGPS * XLALGPSSet( LIGOTimeGPS *epoch, INT4 gpssec, INT8 gpsnan )
{
  //return XLALINT8NSToGPS( epoch, XLAL_BILLION_INT8 * gpssec + gpsnan );
  return XLALINT8NSToGPS( epoch, 1e9 * gpssec + gpsnan );
}

/** Converts nano seconds stored as an INT8 to GPS time. */
LIGOTimeGPS * XLALINT8NSToGPS( LIGOTimeGPS *epoch, INT8 ns )
{
//  epoch->gpsSeconds     = (int)(ns / XLAL_BILLION_INT8);
//  epoch->gpsNanoSeconds =(int)( ns % XLAL_BILLION_INT8);
  epoch->gpsSeconds     = (int)(ns / 1e9);
  epoch->gpsNanoSeconds =(int)( ns % 1000000000);
  return epoch;
}

/** Converts GPS time to nano seconds stored as an INT8. */
INT8 XLALGPSToINT8NS( const LIGOTimeGPS *epoch )
{
  //return XLAL_BILLION_INT8 * epoch->gpsSeconds + epoch->gpsNanoSeconds;
  return 1e9 * epoch->gpsSeconds + epoch->gpsNanoSeconds;
}


 int XLALSimIMRGetEOBCalibratedSpinNQC( EOBNonQCCoeffs *coeffs, 
                                    INT4  l, 
                                    INT4  m, 
                                    REAL8 eta, 
                                    REAL8 a )
{
  const unsigned int nsqdim = 100;
  const unsigned int   qdim = 50;
  const unsigned int   adim = 69;
  UINT4 i;
  /* REAL8 eta2 = eta*eta;*/
  REAL8 a3slist[qdim], a4list[qdim], a5list[qdim];

  memset( coeffs, 0, sizeof( *coeffs ) );
  const double nsetalist[100] = {0.0025, 0.005, 0.0075, 0.01, 0.0125, 0.015, 0.0175, 0.02, 0.0225, 0.025, 0.0275, 0.03, 0.0325, 0.035, 0.0375, 0.04, 0.0425, 0.045, 0.0475, 0.05, 0.0525, 0.055, 0.0575, 0.06, 0.0625, 0.065, 0.0675, 0.07, 0.0725, 0.075, 0.0775, 0.08, 0.0825, 0.085, 0.0875, 0.09, 0.0925, 0.095, 0.0975, 0.1, 0.1025, 0.105, 0.1075, 0.11, 0.1125, 0.115, 0.1175, 0.12, 0.1225, 0.125, 0.1275, 0.13, 0.1325, 0.135, 0.1375, 0.14, 0.1425, 0.145, 0.1475, 0.15, 0.1525, 0.155, 0.1575, 0.16, 0.1625, 0.165, 0.1675, 0.17, 0.1725, 0.175, 0.1775, 0.18, 0.1825, 0.185, 0.1875, 0.19, 0.1925, 0.195, 0.1975, 0.2, 0.2025, 0.205, 0.2075, 0.21, 0.2125, 0.215, 0.2175, 0.22, 0.2225, 0.225, 0.2275, 0.23, 0.2325, 0.235, 0.2375, 0.24, 0.2425, 0.245, 0.2475, 0.25};
  const double etalist[50] = {0.005, 0.01, 0.015, 0.02, 0.025, 0.03, 0.035, 0.04, 0.045, 0.05, 0.055, 0.06, 0.065, 0.07, 0.075, 0.08, 0.085, 0.09, 0.095, 0.1, 0.105, 0.11, 0.115, 0.12, 0.125, 0.13, 0.135, 0.14, 0.145, 0.15, 0.155, 0.16, 0.165, 0.17, 0.175, 0.18, 0.185, 0.19, 0.195, 0.2, 0.205, 0.21, 0.215, 0.22, 0.225, 0.23, 0.235, 0.24, 0.245, 0.25};
  const double alist[69]   = {-1., -0.975, -0.95, -0.925, -0.9, -0.875, -0.85, -0.825, -0.8, -0.775, -0.75, -0.725, -0.7, -0.675, -0.65, -0.625, -0.6, -0.575, -0.55, -0.525, -0.5, -0.475, -0.45, -0.425, -0.4, -0.375, -0.35, -0.325, -0.3, -0.275, -0.25, -0.225, -0.2, -0.175, -0.15, -0.125, -0.1, -0.075, -0.05, -0.025, 0., 0.025, 0.05, 0.075, 0.1, 0.125, 0.15, 0.175, 0.2, 0.225, 0.25, 0.275, 0.3, 0.325, 0.35, 0.375, 0.4, 0.425, 0.45, 0.475, 0.5, 0.525, 0.55, 0.575, 0.6, 0.625, 0.65, 0.675, 0.7};

  const double a1list[100] = {-21.648106754277453, -20.47585498319721, -19.530321799953853, -18.760004570030972, -18.079213572570588, -17.419521042795658, -16.859312290615286, -16.28756533894047, -15.772560753656803, -15.303367029222859, -14.847798623544204, -14.404050939670224, -13.976897526988164, -13.578815297784823, -13.200763216702892, -12.841976980076495, -12.501692284239752, -12.179144825526786, -11.865421415437213, -11.561721630442458, -11.267369305715642, -10.980678895252732, -10.699964853049693, -10.42829543384451, -10.167524207712544, -9.914674799750443, -9.669372681081281, -9.43124332282813, -9.199912196114067, -8.97500477206216, -8.756146521795488, -8.542962916437121, -8.335079427110134, -8.130867290476582, -7.9313984345769715, -7.736518055359128, -7.545988962963793, -7.35957396753171, -7.177035879203623, -6.998068318971551, -6.8224288840577945, -6.649961333309703, -6.480441962265538, -6.313647066463564, -6.149352941442041, -5.9873358827392344, -5.827372185893404, -5.669238146442815, -5.512513869477759, -5.347444298469859, -5.183197664196798, -5.019955297249104, -4.857898528217308, -4.697208687691938, -4.538067106263523, -4.380655114522592, -4.225154043059675, -4.071745222465302, -3.9206099833299994, -3.771929656244297, -3.6258855717987246, -3.4826590605838113, -3.342431453190086, -3.2053840802080775, -3.0798495456033685, -2.957143501994997, -2.837141996613968, -2.7197210766912865, -2.6047567894579564, -2.4921251821449832, -2.381702301983373, -2.273364196204128, -2.1669869120382543, -2.0624464967167566, -1.9596189974706397, -1.8495581080328225, -1.7409749049276515, -1.6340876818580403, -1.5291147325269023, -1.4262743506371505, -1.3257848298916983, -1.2278644639934593, -1.1327315466453465, -1.0406043715502744, -0.9517012324111543, -0.8662404229309005, -0.7844402368124264, -0.7065189677586451, -0.6326949094724702, -0.5631863556568149, -0.4982116000145925, -0.4379889362487165, -0.3827366580621, -0.3326730591576565, -0.28801643323829923, -0.24898507400694198, -0.21579727516649713, -0.18867133041987855, -0.1678255334699995, -0.15347817801977337};

  const double a2list[100] = {178.73204288078207, 167.7345170263427, 158.85457776976878, 151.63702661020528, 145.25886103554546, 139.08071349079492, 133.85300186061994, 128.49833153024582, 123.69581397508206, 119.34304449990263, 115.1225036726121, 111.02138715464824, 107.08437843884623, 103.42908442981195, 99.96941370234742, 96.69762600347904, 93.6059810802332, 90.68673867963636, 87.86042720725196, 85.13659365364732, 82.50827361644501, 79.95961751850369, 77.47477578268207, 75.08044751075573, 72.79299640785891, 70.58542621544011, 68.45392194344085, 66.39466860180258, 64.40385120046682, 62.47765474937501, 60.61226425846867, 58.80386473768926, 57.04864119697828, 55.32723154023535, 53.65375061411054, 52.0271070701423, 50.44518833065014, 48.905881817953585, 47.407074954372156, 45.94482445612376, 44.51687981462722, 43.123270309615144, 41.76224017946817, 40.43203366256695, 39.13089499729211, 37.85706842202429, 36.60879817514412, 35.384328495032264, 34.180608559988464, 32.932965816541305, 31.701640999519693, 30.48755199799916, 29.291616701055233, 28.11475299776345, 26.95787877719934, 25.821911928438432, 24.70777034055626, 23.616371902628366, 22.54863450373026, 21.50547603293748, 20.487814379325563, 19.496567431970035, 18.53265307994643, 17.596989212330282, 16.733055464585682, 15.895477726977264, 15.083579856295431, 14.296685709330593, 13.53411914287315, 12.795204013713512, 12.079264178642092, 11.385623494449275, 10.71360581792548, 10.06253500586111, 9.431734915046569, 8.755551062926262, 8.098379005319565, 7.462063310457031, 6.848448546569211, 6.259379281886656, 5.69670008463992, 5.162255523059555, 4.657890165376111, 4.1854485798201475, 3.7467753346222055, 3.3437149980128416, 2.978112138222609, 2.651811323482059, 2.366657122021744, 2.124494102072216, 1.9271668318640272, 1.77651987962773, 1.674397813593876, 1.6226452019930173, 1.6231066130557064, 1.6776266150124943, 1.7880497760939342, 1.956220664530578, 2.1839838485529777, 2.4731838963916855};

  const double a3list[100] = {-198.6684740510964, -185.71560983427335, -175.26102024642407, -166.7798147654891, -159.28986302859136, -152.03810090459336, -145.91734205946045, -139.6355596843493, -134.01653939524905, -128.93945143613453, -124.02034829337536, -119.24645127356649, -114.67037356959001, -110.43118685036107, -106.4264567068743, -102.6466423946065, -99.08220316903453, -95.72359828563523, -92.4798228915885, -89.36096908139594, -86.35837723133453, -83.45329697039512, -80.62697792756852, -77.90968987612793, -75.32016588735331, -72.82709457524146, -70.42579806564999, -68.11159848443651, -65.87981795745866, -63.72577861057402, -61.64480256964021, -59.63221196051487, -57.683328909055604, -55.7731074201788, -53.92035933426217, -52.12397499502319, -50.38150684315194, -48.69050731933857, -47.04852886427319, -45.45070004394427, -43.89439067703587, -42.37998904664536, -40.90552002179141, -39.46900847149271, -38.068479264767944, -36.70195727063581, -35.36746735811498, -34.063034396224154, -32.78536874375959, -31.468633548362497, -30.173781283368523, -28.901550623295833, -27.652680242662605, -26.427908815987003, -25.227975017787198, -24.053617522581362, -22.905575004887663, -21.78458613922428, -20.691389600109368, -19.6267240620611, -18.59132819959765, -17.585940687237187, -16.61130019949788, -15.668145410897901, -14.794309398041404, -13.950185357101319, -13.135122694543167, -12.348470816832473, -11.589579130434757, -10.857797041815543, -10.152473957440362, -9.47295928377472, -8.818602427284151, -8.188752794434174, -7.582759791690314, -6.932160232642741, -6.304213548518645, -5.700899806714059, -5.124199074625014, -4.576091419647542, -4.0585569091776765, -3.573575610611449, -3.123127591344892, -2.709192918774041, -2.3337516602949204, -1.9987838833035663, -1.706269655196011, -1.458189043368287, -1.2565221152164259, -1.1032489381364605, -1.0003495795244224, -0.9498041067763443, -0.953592587288258, -1.0136950884561957, -1.1320916776761898, -1.31076242234427, -1.5516873898564725, -1.8568466476088277, -2.2282202629973678, -2.667788303418125};

  const double b1list[100] = {-0.5693500504085347, -0.576434151837257, -0.5827940588889807, -0.5875005969333106, -0.5915255507494274, -0.5970658827548452, -0.6057016775611604, -0.6053160995270499, -0.6070988602490128, -0.6110941958474475, -0.6140262971912503, -0.6172989788502661, -0.6206089494421659, -0.6234488672149939, -0.6258813681301192, -0.6277498501118401, -0.628897711084455, -0.6291683489722621, -0.6308371388180571, -0.6331613723912005, -0.6359581432020665, -0.6393457874435003, -0.6434426413083475, -0.6478966252652467, -0.6524003236766366, -0.6571425380351175, -0.6620695856777987, -0.6671277839417898, -0.6722634501642004, -0.6774229016821397, -0.6825524558327174, -0.6875984299530429, -0.6925071413802255, -0.6961049747777461, -0.6996298196832709, -0.7032241904115, -0.7069570372346382, -0.7108973104248908, -0.7151139602544623, -0.720064149835206, -0.7258460785616149, -0.7320745022572427, -0.7387427060855165, -0.7458439752098636, -0.7533715947937112, -0.7613188500004865, -0.7696790259936167, -0.778445407936529, -0.7876035918776259, -0.7967733093076456, -0.8063159350249182, -0.8162406283658405, -0.8265565486668099, -0.8372728552642233, -0.8483987074944775, -0.8599432646939698, -0.871915686199097, -0.884325131346256, -0.8971807594718443, -0.9104917299122585, -0.9242672020038958, -0.938516335083153, -0.9532482884864274, -0.9684722215501159, -0.9837886712042127, -0.9996512322126136, -1.0160843677317775, -1.0331125409181625, -1.0507602149282274, -1.0690518529184303, -1.08801191804523, -1.1076648734650847, -1.1280351823344532, -1.1491473078097938, -1.171025713047565, -1.1939017528688474, -1.2175927015073384, -1.2421149961497486, -1.2674850739827876, -1.2937193721931661, -1.320834327967594, -1.3488463784927816, -1.377771960955439, -1.4076275125422764, -1.4384294704400042, -1.4701942718353325, -1.5029383539149717, -1.5366781538656316, -1.5714301088740226, -1.6072106561268549, -1.6440362328108387, -1.6819232761126843, -1.7208882232191016, -1.7609475113168012, -1.802117577592493, -1.8444148592328868, -1.8878557934246936, -1.9324568173546235, -1.9782343682093864, -2.0252048831756926};

  const double b2list[100] = {1.6514745488753086, 1.6733593678301482, 1.687838328174986, 1.7031979370575185, 1.712831020475929, 1.7266279186283089, 1.7581796869631672, 1.7499867318965905, 1.7518398412177276, 1.7634468469918447, 1.770740685014047, 1.779639998248617, 1.788893228893931, 1.7964389585725973, 1.8024779675983216, 1.8063408969988246, 1.8073583878018264, 1.8048610810350476, 1.8077536017385523, 1.8130592620946404, 1.8200051503317694, 1.8290042591854243, 1.8404695813910905, 1.8531136802761718, 1.8658884601302625, 1.8795206835759333, 1.8938451458175296, 1.908696642059398, 1.923909967505884, 1.9393199173613336, 1.9547612868300928, 1.9700688711165077, 1.9850774654249244, 1.9960816492286722, 2.0069990026439246, 2.0182845185376195, 2.0301606467081585, 2.042849836953944, 2.056574539073377, 2.0726094682881886, 2.0912563552368337, 2.111506538675078, 2.1333773752936596, 2.1568862217833127, 2.1820504348347747, 2.208887371138781, 2.2374143873860683, 2.2676488402673725, 2.299578947658098, 2.3318064402049257, 2.3657435940219895, 2.4014679228653333, 2.439056940491002, 2.478588160655038, 2.520139097113486, 2.56378726362239, 2.609610173937794, 2.657685341815741, 2.708090281012276, 2.760902505283443, 2.8161995283852854, 2.874058864073847, 2.9345580261051722, 2.9977745282353045, 3.0615596376827443, 3.128412230361185, 3.1984931979224718, 3.271963432018449, 3.3489838243009618, 3.429715266421855, 3.5143186500329735, 3.602954866786163, 3.6957848083332676, 3.792969366326133, 3.894669432416603, 4.002044979667201, 4.114256382857298, 4.23142577617022, 4.353675293789295, 4.48112706989785, 4.613903238679211, 4.752125934316707, 4.895917290993665, 5.04539944289341, 5.200694524199272, 5.361924669094577, 5.529212011762653, 5.702678686386827, 5.882446827150425, 6.068638568236777, 6.261376043829206, 6.460781388111044, 6.666976735265615, 6.880084219476247, 7.100225974926268, 7.327524135799001, 7.5621008362777795, 7.804078210545929, 8.053578392786775, 8.310723517183646};

  const double a3stab[50][69] = {
{1298.87913, 1261.29637, 1227.72908, 1197.25947, 1168.96973, 1141.94207, 1115.2587, 1088.00181, 1059.25362, 1022.90618, 985.307903, 947.617046, 910.991876, 881.346609, 853.181173, 825.751447, 798.313312, 767.87765, 736.843339, 705.36426, 673.594293, 642.125047, 610.497581, 578.690684, 546.683144, 514.497622, 482.051486, 449.305976, 416.222332, 382.495195, 348.459044, 314.181759, 279.731222, 245.411071, 210.959123, 176.348956, 141.554145, 106.628243, 71.43286, 35.9095836, 0., -36.1519021, -72.887899, -110.347365, -148.669675, -186.733245, -226.44279, -268.442067, -313.374835, -358.91977, -409.871742, -468.06054, -535.315953, -602.768787, -687.227409, -794.8012, -931.599543, -1073.78457, -1269.39181, -1536.50955, -1893.22609, -2326.65745, -2947.80866, -3741.68954, -4974.29064, -6365.10282, -9538.63496, -15643.1414, -25826.8766}, 
{1196.32002, 1167.06104, 1137.27475, 1107.12301, 1076.76769, 1046.37066, 1016.09379, 986.098937, 956.547978, 928.8469, 901.415801, 873.918898, 846.020409, 814.568078, 783.16919, 752.614552, 723.694973, 702.877684, 683.006503, 662.601671, 640.183429, 607.718815, 572.902552, 536.876163, 500.78117, 469.351999, 438.700105, 408.529845, 378.545579, 347.703945, 316.75611, 285.705522, 254.555628, 223.39562, 192.108901, 160.664621, 129.031928, 97.2453174, 65.1824526, 32.7863434, 0., -33.0547348, -66.6852171, -101.01997, -136.187517, -170.971669, -207.383546, -246.089556, -287.756108, -330.313665, -378.258958, -433.352771, -497.35589, -562.191692, -643.393334, -746.656564, -877.67713, -1013.79966, -1200.41146, -1454.54875, -1793.24772, -2204.40576, -2792.4755, -3542.23306, -4707.36116, -6024.93464, -9029.55323, -14806.4354, -24440.7998}, 
{1111.34089, 1088.04208, 1061.93086, 1033.65502, 1003.86233, 973.200551, 942.317471, 911.86086, 882.478492, 858.393797, 835.248631, 812.260507, 788.646935, 759.48951, 729.796029, 700.438372, 672.288418, 651.387883, 631.370875, 611.041341, 589.203225, 559.502697, 527.964588, 495.455951, 462.843844, 433.627925, 404.989603, 376.74289, 348.701797, 320.308767, 291.898012, 263.432172, 234.873889, 206.130982, 177.242843, 148.194039, 118.969142, 89.6533759, 60.0903922, 30.2244979, 0., -30.4005164, -61.3663335, -93.0484555, -125.597886, -157.893074, -191.866601, -228.178495, -267.488782, -307.871972, -353.607815, -406.390546, -467.914398, -530.2754, -608.605274, -708.437534, -835.305696, -967.651586, -1148.93709, -1395.53239, -1723.80769, -2122.58454, -2690.87904, -3414.09589, -4533.73512, -5812.62405, -8672.18555, -14119.1936, -23160.4223}, 
{1039.53251, 1020.2822, 997.603313, 972.198968, 944.772298, 916.02643, 886.664491, 857.389611, 828.904917, 804.925294, 781.93741, 759.439688, 736.930555, 712.636669, 687.836926, 662.538457, 636.748394, 609.788544, 582.625489, 555.540492, 528.814811, 504.169828, 479.870634, 455.622441, 431.13046, 404.996091, 378.469884, 351.698574, 324.8289, 298.520437, 272.201947, 245.815032, 219.301294, 192.43116, 165.385874, 138.17551, 110.810137, 83.4601202, 55.9111218, 28.1090968, 0., -28.1232207, -56.8004007, -86.2243824, -116.588008, -146.981092, -179.140716, -213.700934, -251.2958, -290.060701, -334.127824, -385.130693, -444.702827, -504.716717, -580.471327, -677.504592, -801.354447, -931.385611, -1109.77852, -1352.54038, -1675.67843, -2068.96254, -2627.11195, -3337.00139, -4427.97475, -5693.23265, -8428.0796, -13552.2941, -21985.6547}, 
{976.485655, 959.824144, 940.197989, 918.098323, 894.016279, 868.442993, 841.869596, 814.787222, 787.687004, 761.819814, 736.613152, 712.254257, 688.930369, 670.53532, 652.067116, 632.23036, 609.729651, 574.459963, 537.459377, 500.956344, 467.179318, 448.413342, 432.807639, 418.568025, 403.900314, 381.127188, 356.690849, 331.150363, 305.064801, 280.549729, 255.985115, 231.307428, 206.453135, 181.110154, 155.562924, 129.847331, 103.999264, 78.2732522, 52.3990841, 26.3251899, 0., -26.1568213, -52.856571, -80.3393122, -108.845108, -137.719354, -168.454647, -201.64892, -237.900101, -275.345863, -318.028498, -367.530041, -425.432527, -483.212445, -556.599593, -651.218222, -772.692584, -901.046982, -1077.7456, -1318.65266, -1639.63241, -2031.30849, -2585.26688, -3290.67293, -4364.64226, -5631.82205, -8258.783, -13076.6149, -20916.4077}, 
{917.791081, 902.710621, 885.620781, 866.696557, 846.112943, 824.044936, 800.66753, 776.155722, 750.684506, 722.455778, 694.406874, 667.502029, 642.705478, 629.711227, 617.261835, 602.829629, 583.886937, 541.782439, 496.561572, 452.146121, 412.457876, 398.926373, 390.962551, 385.485097, 379.412698, 359.691907, 337.202401, 312.851722, 287.547412, 264.607418, 241.564715, 218.362682, 194.944699, 170.981969, 146.79892, 122.447804, 97.9808711, 73.7004736, 49.308722, 24.7578271, 0., -24.435292, -49.4039968, -75.1848062, -102.056412, -129.591492, -159.057156, -191.014502, -226.024627, -262.193469, -303.519346, -351.545419, -407.814847, -463.459392, -534.598171, -626.938905, -746.189313, -872.680952, -1047.64817, -1286.94915, -1606.44207, -1997.39115, -2549.4365, -3254.83387, -4318.29988, -5593.45384, -8125.84343, -12663.0342, -19952.5918}, 
{859.039559, 844.98436, 829.777581, 813.337142, 795.580959, 776.426953, 755.793041, 733.597141, 709.757173, 680.211609, 650.449591, 621.980817, 596.314984, 586.690157, 578.196318, 567.651816, 551.875002, 508.136269, 460.621105, 413.967044, 372.811617, 362.402057, 358.522317, 357.566049, 355.926907, 338.360939, 317.554443, 294.556114, 270.414647, 248.904279, 227.257944, 205.434118, 183.391278, 160.860605, 138.11879, 115.215226, 92.1993096, 69.3494866, 46.3944783, 23.2920582, 0., -22.8926065, -46.3118306, -70.5524255, -95.9091441, -122.081137, -150.197001, -180.789729, -214.392317, -249.069531, -288.809882, -335.133657, -389.561139, -443.154361, -512.075161, -602.027124, -718.713836, -842.332775, -1014.29608, -1250.50977, -1566.87987, -1954.97924, -2503.71346, -3209.20757, -4263.50982, -5543.18962, -7990.80852, -12282.43, -19094.1177}, 
{795.821854, 782.688084, 768.57428, 753.36355, 736.939001, 719.18374, 699.980874, 679.213512, 656.764759, 628.465728, 599.872319, 572.488437, 547.817986, 537.997872, 529.6458, 520.012472, 506.348594, 469.901748, 430.32701, 391.276333, 356.401671, 345.533527, 339.673884, 336.003275, 331.702235, 314.804975, 295.296878, 274.017005, 251.804419, 231.651088, 211.382003, 190.97506, 170.40816, 149.560065, 128.547461, 107.387899, 86.098929, 64.8279929, 43.4107958, 21.8129331, 0., -21.4627385, -43.4492248, -66.2337315, -90.0905311, -114.671921, -141.12294, -169.966649, -201.726112, -234.440059, -272.109617, -316.251583, -368.382751, -419.994158, -486.638661, -573.843361, -687.135356, -806.047703, -972.49916, -1202.41444, -1511.71827, -1891.84153, -2432.19041, -3133.5174, -4174.83432, -5446.09099, -7815.22592, -11905.6805, -18340.896}, 
{723.728735, 711.864516, 697.916767, 682.119252, 664.705738, 645.909991, 625.965777, 605.106863, 583.567013, 560.596558, 537.806075, 515.822704, 495.273586, 480.160139, 466.385516, 453.227147, 439.962463, 423.459176, 406.36832, 388.931212, 371.389169, 355.013918, 338.6042, 321.989168, 304.997976, 286.694707, 267.979608, 248.987859, 229.854639, 211.058618, 192.254089, 173.438833, 154.610635, 135.894352, 117.109862, 98.2041202, 79.1240795, 59.7436944, 40.1121174, 20.2055015, 0., -20.0796619, -40.6853319, -62.0202854, -84.2877981, -106.847476, -131.083733, -157.53731, -186.748953, -216.771064, -251.628063, -292.856026, -341.991034, -391.675587, -455.896772, -539.748099, -648.323078, -759.870989, -917.067264, -1135.7431, -1431.72971, -1795.74674, -2318.96003, -3007.48672, -4026.8356, -5267.21955, -7560.6433, -11503.6636, -17692.8373}, 
{670.702392, 658.608735, 645.231211, 630.627242, 614.85425, 597.969656, 580.030884, 561.095353, 541.220486, 518.602027, 495.903746, 473.927738, 453.476095, 438.689549, 425.696101, 413.962387, 402.955046, 391.824136, 380.479507, 368.51443, 355.522175, 339.17332, 321.752905, 303.623275, 285.146778, 267.626973, 250.10851, 232.577251, 215.019058, 197.328079, 179.618576, 161.913096, 144.234187, 126.856941, 109.450342, 91.9359207, 74.2352057, 56.1190544, 37.7199375, 19.0196532, 0., -18.9413868, -38.4048807, -58.5750179, -79.636335, -100.928376, -123.818666, -148.82974, -176.48413, -204.795257, -237.798413, -277.019773, -323.985518, -371.643004, -433.528758, -514.600485, -619.815891, -726.601121, -878.458066, -1091.35706, -1381.26842, -1738.68566, -2256.0096, -2944.21016, -3956.54775, -5195.43966, -7424.38522, -11191.6133, -17045.3526}, 
{623.046755, 610.336599, 597.375254, 583.988785, 570.003256, 555.244732, 539.539276, 522.712953, 504.591828, 481.903784, 458.812338, 436.382826, 415.680585, 401.790876, 390.151139, 380.218743, 371.451052, 363.730969, 355.920111, 347.30563, 337.174678, 321.458633, 304.142732, 285.856435, 267.229206, 250.700686, 234.366085, 218.130789, 201.900189, 185.228258, 168.512366, 151.798468, 135.13252, 118.913825, 102.69365, 86.3766111, 69.8673234, 52.8685906, 35.5675654, 17.9495883, 0., -17.9060924, -36.3290217, -55.439354, -75.4076554, -95.5838756, -117.287444, -141.017173, -167.271875, -194.031478, -225.321234, -262.64751, -307.516673, -353.072705, -412.529312, -490.737815, -592.549535, -694.603535, -841.248297, -1048.62004, -1332.855, -1684.25015, -2196.45943, -2885.12536, -3891.89364, -5131.56784, -7300.31693, -10898.9685, -16428.3503}, 
{578.777874, 565.343763, 552.617764, 540.222136, 527.779136, 514.911024, 501.240056, 486.388491, 469.978587, 447.525103, 424.400795, 401.870923, 381.200742, 368.162933, 357.712364, 349.311322, 342.422098, 337.556737, 332.707869, 326.917882, 319.229161, 304.187697, 287.130832, 268.899514, 250.334688, 234.780471, 219.573371, 204.553063, 189.559225, 173.868813, 158.109315, 142.345496, 126.64212, 111.492196, 96.360951, 81.1418536, 65.7283731, 49.7810319, 33.5194242, 16.9301977, 0., -16.9275463, -34.3655585, -52.4701786, -71.3975492, -90.5102836, -111.075464, -133.566646, -158.457381, -183.715399, -213.322409, -248.754293, -291.486936, -334.842038, -391.711338, -466.832391, -564.942754, -661.929092, -802.920208, -1004.19402, -1282.02842, -1626.52282, -2132.4907, -2819.95411, -3819.61549, -5057.44347, -7167.66153, -10607.4294, -15833.9069}, 
{537.705661, 523.592259, 510.97538, 499.323946, 488.10688, 476.793102, 464.851536, 451.751104, 436.960726, 415.185168, 392.563172, 370.469324, 350.278208, 338.111834, 328.698393, 321.5135, 316.032768, 313.155404, 310.363995, 306.564723, 300.663765, 286.44217, 269.9813, 252.237386, 234.16666, 219.614044, 205.491601, 191.600088, 177.74026, 163.024266, 148.21691, 133.394391, 118.632905, 104.472791, 90.3404513, 76.1264269, 61.7212598, 46.7790037, 31.5212837, 15.9332368, 0., -15.974157, -32.4508828, -49.5726937, -67.4821057, -85.5612143, -105.017124, -126.29652, -149.846085, -173.651974, -201.605614, -235.1379, -275.67973, -316.701541, -370.778872, -442.526802, -536.56041, -628.112094, -762.932689, -957.39035, -1227.85323, -1564.25632, -2062.26726, -2745.81353, -3735.46447, -4967.31563, -7018.71577, -10306.7003, -15248.3048}, 
{499.640027, 485.044119, 472.464742, 461.290869, 450.911477, 440.715539, 430.092032, 418.42993, 405.118208, 384.603164, 363.193521, 342.255325, 323.154622, 311.94369, 303.427849, 297.098651, 292.447647, 290.380935, 288.409705, 285.45969, 280.456626, 267.30371, 251.958228, 235.354929, 218.428561, 204.949119, 191.88201, 179.027883, 166.187391, 152.469136, 138.642637, 124.785364, 110.97479, 97.7363464, 84.5203574, 71.2251094, 57.7488886, 43.7851316, 29.5189138, 14.9304613, 0., -15.0143326, -30.5213867, -46.652101, -63.5374142, -80.5902821, -98.9468193, -119.025158, -141.243429, -163.646157, -189.974524, -221.596107, -259.878479, -298.401753, -349.435951, -417.463636, -506.967366, -592.686844, -720.744632, -907.520433, -1169.39395, -1496.20329, -1983.95294, -2659.82073, -3635.19172, -4855.43337, -6845.77638, -9986.48562, -14657.826}, 
{464.390881, 449.661375, 437.102489, 426.119557, 416.117917, 406.502905, 396.679857, 386.054108, 374.030995, 355.498276, 336.185896, 317.306223, 300.071624, 289.964612, 282.219351, 276.34015, 271.831319, 269.087297, 266.366212, 262.816322, 257.585886, 245.853978, 232.325711, 217.737017, 202.823827, 190.533415, 178.505829, 166.592464, 154.644712, 141.977941, 129.193979, 116.358629, 103.537694, 91.1635985, 78.7888757, 66.3326796, 53.7141645, 40.722041, 27.4580845, 13.8936269, 0., -14.0164816, -28.5134619, -43.6136023, -59.439564, -75.4511011, -92.6989449, -111.57092, -132.45485, -153.502899, -178.232816, -207.926688, -243.866605, -279.69321, -327.386613, -391.285482, -475.728482, -555.187646, -675.814927, -853.895645, -1105.71512, -1421.11637, -1895.71161, -2559.09281, -3514.54842, -4716.04577, -6641.14011, -9636.48968, -14048.7528}, 
{431.768135, 417.406059, 404.905261, 393.806663, 383.651192, 373.979771, 364.333324, 354.252775, 343.279049, 327.589687, 311.434349, 295.699313, 281.270856, 272.480712, 265.391519, 259.511373, 254.348369, 249.128454, 243.754732, 237.848156, 231.029682, 221.17463, 210.347843, 198.868528, 187.055894, 176.114645, 165.124294, 154.049849, 142.856319, 131.325202, 119.678423, 107.954395, 96.1915321, 84.6352843, 73.0342128, 61.3439161, 49.5199927, 37.5123573, 25.2845654, 12.794489, 0., -12.9490123, -26.3635003, -40.3623994, -55.0646445, -69.9972857, -86.1078972, -103.752168, -123.285788, -143.027154, -166.184163, -193.927421, -227.427532, -260.326451, -304.334894, -363.634929, -442.40862, -515.148803, -627.602467, -795.827371, -1035.88127, -1337.74821, -1795.70711, -2440.7469, -3369.28571, -4543.40188, -6397.10368, -9246.41689, -13407.3673}, 
{397.382476, 384.161063, 372.406617, 361.789178, 351.978785, 342.645477, 333.459295, 324.090277, 314.208462, 301.130105, 287.820545, 274.891334, 262.954027, 255.327872, 248.833648, 242.999831, 237.354894, 230.264221, 222.884616, 215.20979, 207.233455, 198.951905, 190.355236, 181.436127, 172.187257, 162.452347, 152.432615, 142.180322, 131.747728, 121.278447, 110.696847, 100.018645, 89.2595619, 78.5197505, 67.6967231, 56.7724257, 45.7288044, 34.6183092, 23.3241805, 11.8001626, 0., -11.943738, -24.3276619, -37.2695573, -50.8872099, -64.7777681, -79.7879094, -96.2436743, -114.471103, -132.961668, -154.609806, -180.475383, -211.618268, -241.71635, -282.164266, -336.974677, -410.160241, -476.397825, -580.770198, -739.024339, -966.907224, -1254.16741, -1694.54713, -2321.20376, -3222.44556, -4362.79135, -6146.7668, -8868.89588, -12823.7025}, 
{359.974686, 348.352, 338.125236, 328.975588, 320.58425, 312.632416, 304.801281, 296.772038, 288.225882, 276.655174, 264.805473, 253.233508, 242.496004, 235.582587, 229.643928, 224.263593, 219.02515, 212.45994, 205.624648, 198.523735, 191.16166, 183.591405, 175.749496, 167.620982, 159.190914, 150.290229, 141.119734, 131.726121, 122.156087, 112.539351, 102.806371, 92.9706295, 83.0456113, 73.124859, 63.1097732, 52.9818134, 42.7224396, 32.3549175, 21.8021786, 11.0289603, 0., -11.1220613, -22.6495511, -34.6968934, -47.3785121, -60.3411594, -74.354, -89.7185264, -106.736232, -124.027317, -144.247083, -168.369539, -197.368694, -225.064687, -262.446946, -313.351026, -381.612486, -442.034343, -539.097709, -688.251154, -904.943253, -1178.0799, -1602.7377, -2215.93586, -3095.8159, -4202.28901, -5927.06542, -8553.90562, -12366.5701}, 
{324.284553, 314.363487, 305.717204, 298.046437, 291.051921, 284.434388, 277.894571, 271.133205, 263.851022, 253.758508, 243.342743, 233.100562, 223.528797, 217.260142, 211.801225, 206.794534, 201.882558, 195.779693, 189.427757, 182.840476, 176.031575, 169.10102, 161.9418, 154.533147, 146.854291, 138.731414, 130.358013, 121.774536, 113.021431, 104.2128, 95.2859787, 86.2519542, 77.1217145, 67.9818174, 58.7374528, 49.3693806, 39.8583605, 30.1989457, 20.3525856, 10.2945228, 0., -10.342512, -21.0562901, -32.2513829, -44.0378392, -56.1058801, -69.1533129, -83.4581171, -99.2982726, -115.426321, -134.255855, -156.675029, -183.572, -208.924082, -243.294606, -290.336061, -353.700939, -408.371479, -498.138522, -638.12266, -843.444484, -1102.14389, -1510.58355, -2109.2871, -2967.15339, -4038.77567, -5707.59445, -8249.83842, -11941.7363}, 
{290.624535, 282.418333, 275.336854, 269.105352, 263.449083, 258.093303, 252.763265, 247.184226, 241.08144, 232.406317, 223.367496, 214.39977, 205.937932, 200.246338, 195.198394, 190.497067, 185.845325, 180.150535, 174.229506, 168.103447, 161.793567, 155.43567, 148.890531, 142.133519, 135.140006, 127.739783, 120.11203, 112.290346, 104.308333, 96.2624774, 88.09834, 79.8243672, 71.4490055, 63.0513927, 54.5410077, 45.8980203, 37.1026006, 28.1249184, 18.959144, 9.58944784, 0., -9.60482061, -19.549136, -29.9368594, -40.8719042, -52.0789138, -64.1927792, -77.4691216, -92.1635624, -107.160693, -124.635576, -145.392245, -170.234732, -193.315202, -224.750304, -268.004818, -326.543524, -375.558951, -458.097032, -588.931449, -782.835883, -1027.0032, -1418.94953, -2002.20341, -2837.66181, -3874.62585, -5491.41154, -7959.08117, -11548.697}, 
{259.307093, 252.739349, 247.138517, 242.255957, 237.843023, 233.651073, 229.431464, 224.935552, 219.914693, 212.564812, 204.814872, 197.038402, 189.608933, 184.426978, 179.72829, 175.275605, 170.831659, 165.499518, 159.965458, 154.256084, 148.398001, 142.550276, 136.554067, 130.382995, 124.010678, 117.279218, 110.346361, 103.238334, 95.9813658, 88.6520651, 81.2061239, 73.6496167, 65.9886178, 58.2943521, 50.4816833, 42.5306256, 34.4211932, 26.1073599, 17.6055963, 8.90633292, 0., -8.90871776, -18.1293461, -27.7571564, -37.8874201, -48.2672439, -59.4793298, -71.758215, -85.3384368, -99.2324482, -115.385705, -134.521577, -157.363436, -178.258714, -206.857097, -246.432332, -300.258165, -343.746479, -419.177631, -540.970111, -723.542412, -953.301677, -1328.70045, -1895.63073, -2708.54494, -3712.21403, -5281.5743, -7684.02071, -11186.9482}, 
{230.644686, 225.549343, 221.276528, 217.601877, 214.301026, 211.14961, 207.923267, 204.397631, 200.348339, 194.200203, 187.620012, 180.923731, 174.427326, 169.687863, 165.283766, 161.034559, 156.759767, 151.753696, 146.571178, 141.241823, 135.795243, 130.399758, 124.890791, 119.24247, 113.428926, 107.313599, 101.025583, 94.5832827, 88.0051028, 81.3452456, 74.5719997, 67.6894509, 60.7016853, 53.6714628, 46.5207255, 39.2300897, 31.7801716, 24.1207947, 16.2756849, 8.23777562, 0., -8.25393396, -16.7981776, -25.7161074, -35.0911002, -44.6778543, -55.0198957, -66.3320723, -78.829232, -91.6435998, -106.505696, -124.063416, -144.964658, -163.775287, -189.658045, -225.69364, -274.962784, -313.083784, -381.584712, -494.531239, -665.989037, -881.683134, -1240.70113, -1790.51499, -2581.00657, -3553.91473, -5081.14038, -7427.04393, -10855.9858}, 
{204.949773, 201.071126, 197.905217, 195.246737, 192.890376, 190.630825, 188.262774, 185.580914, 182.379935, 177.278699, 171.718057, 165.963028, 160.278636, 155.914796, 151.757677, 147.678343, 143.547856, 138.840123, 133.982228, 129.004099, 123.935661, 118.939037, 113.859081, 108.672842, 103.357369, 97.8068086, 92.1142731, 86.2899726, 80.3441169, 74.3057015, 68.1586367, 61.9056181, 55.5493414, 49.1434918, 42.6193799, 35.9593056, 29.1455691, 22.1397473, 14.9531524, 7.57637353, 0., -7.6401998, -15.5568876, -23.817546, -32.4896574, -41.3177284, -50.8214078, -61.1973687, -72.6422837, -84.3961618, -97.9950056, -114.018154, -133.044944, -149.885588, -173.196203, -205.863779, -250.775306, -283.720583, -345.522668, -449.907425, -610.600723, -812.791405, -1155.8164, -1687.80212, -2456.25049, -3402.10245, -4893.16741, -7190.53768, -10555.3056}, 
{182.534813, 179.527507, 177.178917, 175.294163, 173.678361, 172.136629, 170.474086, 168.49585, 166.007037, 161.766511, 157.044147, 152.063566, 147.048389, 142.99358, 139.042878, 135.111368, 131.114134, 126.685851, 122.134173, 117.486347, 112.76962, 108.123033, 103.417319, 98.6350069, 93.7586259, 88.7227275, 83.5770077, 78.3231858, 72.9629811, 67.4971152, 61.9287041, 56.2598663, 50.4927201, 44.6712063, 38.738892, 32.6811664, 26.4834191, 20.1387421, 13.621741, 6.91472423, 0., -7.06724582, -14.4067335, -22.0653056, -30.0898049, -38.19385, -46.8907973, -56.3607792, -66.783928, -77.4921481, -89.8530911, -104.386181, -121.610841, -136.610285, -157.514631, -187.017785, -227.813654, -255.806599, -311.195892, -407.391261, -557.802432, -747.270316, -1074.91108, -1588.43805, -2335.48049, -3259.15168, -4720.71301, -6976.88882, -10284.4035}, 
{165.290507, 162.286376, 160.078255, 158.450848, 157.188853, 156.076972, 154.899907, 153.442357, 151.489025, 147.77082, 143.54775, 139.026033, 134.411887, 130.567225, 126.78029, 122.995023, 119.155364, 115.021951, 110.795345, 106.492805, 102.131591, 97.7874608, 93.395776, 88.9503958, 84.4451802, 79.8803286, 75.2408251, 70.5179937, 65.7031583, 60.7565616, 55.7130412, 50.5763533, 45.3502544, 40.0687128, 34.693188, 29.2153516, 23.6268752, 17.9944365, 12.2046983, 6.21932983, 0., -6.54855977, -13.3849676, -20.5247794, -27.983551, -35.3990376, -43.315716, -51.9002622, -61.3193524, -70.9677412, -82.0927948, -95.1699579, -110.674675, -124.050902, -142.81816800000001, -169.464514, -206.47798, -229.664784, -278.867519, -367.246955, -507.963863, -685.99481, -999.053177, -1492.11906, -2218.17943, -3129.51904, -4573.22466, -6797.68259, -10051.2791}, 
{152.137356, 148.560618, 146.028799, 144.288529, 143.086442, 142.169167, 141.283337, 140.175584, 138.592538, 135.133503, 131.151371, 126.851704, 122.440065, 118.735667, 115.084965, 111.448061, 107.78506, 103.948328, 100.048801, 96.0896769, 92.0741556, 87.9923766, 83.8658206, 79.7029095, 75.5120655, 71.371298, 67.1916066, 62.9535786, 58.6378009, 54.159387, 49.5905871, 44.9381778, 40.2089354, 35.4261284, 30.5734448, 25.6510642, 20.6591665, 15.7676707, 10.7391215, 5.50580328, 0., -6.0717388, -12.4611006, -19.1455073, -26.1023811, -32.8645712, -40.0319015, -47.7596232, -56.2029876, -64.7948193, -74.7017665, -86.3680509, -100.237894, -112.156793, -129.007185, -153.07278, -186.637291, -205.292071, -248.690132, -329.792132, -461.558725, -629.404881, -928.92831, -1400.89323, -2107.06889, -3013.74267, -4448.12437, -6647.19318, -9847.92831}, 
{140.781753, 136.682234, 133.820464, 131.915018, 130.684473, 129.847403, 129.122385, 128.227993, 126.882804, 123.587991, 119.766493, 115.623843, 111.365579, 107.785616, 104.265758, 100.776187, 97.2870864, 93.7156472, 90.1062419, 86.4502507, 82.7390537, 78.8895359, 74.9973708, 71.0837365, 67.1698115, 63.3987972, 59.6210392, 55.8089063, 51.9347676, 47.8783341, 43.7416956, 39.5342838, 35.2655307, 30.9463936, 26.5841688, 22.1876782, 17.7657435, 13.5945717, 9.30864537, 4.80983222, 0., -5.61379701, -11.5769522, -17.8296726, -24.3121648, -30.4556994, -36.9129944, -43.8278312, -51.3439913, -58.9189753, -67.6573579, -77.9774331, -90.2974945, -100.815114, -115.857596, -137.531524, -167.94348, -182.552124, -220.771131, -295.366252, -419.10324, -577.762732, -865.065819, -1317.76997, -2006.19433, -2910.75903, -4337.91853, -6512.61869, -9659.80539}, 
{130.494647, 126.16847, 123.140568, 121.124242, 119.832792, 118.979516, 118.277715, 117.440689, 116.181737, 112.994264, 109.299423, 105.29847, 101.192663, 97.752079, 94.3816266, 91.0550345, 87.7460315, 84.3994753, 81.0295143, 77.6214257, 74.1604867, 70.521654, 66.8446537, 63.1588911, 59.4937719, 56.0305044, 52.5859699, 49.1288527, 45.6278369, 41.946012, 38.1998949, 34.400408, 30.5584734, 26.6696049, 22.7662965, 18.8656338, 14.9847023, 11.5026577, 7.92968738, 4.13804898, 0., -5.16541025, -10.7114327, -16.5445265, -22.5711508, -28.1368435, -33.9333535, -40.0915086, -46.7421363, -53.3523053, -60.9821059, -70.0278696, -80.8859276, -90.0081075, -103.313046, -122.774876, -150.36773, -161.586559, -195.476351, -264.602911, -381.532045, -532.136331, -809.061255, -1246.06057, -1920.26372, -2824.73561, -4245.42361, -6395.05448, -9486.35496}, 
{120.218374, 116.363283, 113.606148, 111.699338, 110.395219, 109.44616, 108.604528, 107.622691, 106.253017, 103.165789, 99.6282923, 95.8257276, 91.9432956, 88.7062903, 85.5437823, 82.4249355, 79.3189135, 76.1411207, 72.935984, 69.6941707, 66.4063484, 62.9681364, 59.5032696, 56.0404345, 52.6083179, 49.374891, 46.1738422, 42.9781445, 39.7607707, 36.4023776, 33.0051808, 29.5790797, 26.1339737, 22.6437028, 19.1686496, 15.7331373, 12.361489, 9.52475412, 6.62183918, 3.4983769, 0., -4.71442103, -9.83778635, -15.2497584, -20.8299995, -25.8728603, -31.0774398, -36.5575249, -42.4269028, -48.1422156, -54.7372534, -62.5886611, -72.0730838, -79.7205999, -91.2930477, -108.705699, -133.873825, -142.639015, -173.419696, -238.560614, -350.406513, -494.295262, -763.592234, -1191.33966, -1857.42283, -2763.52395, -4177.2945, -6299.89362, -9332.48046}, 
{110.75343, 107.590506, 105.231651, 103.484465, 102.156549, 101.055506, 99.9889362, 98.7644409, 97.1896215, 94.1790465, 90.790563, 87.1889854, 83.539128, 80.5146295, 77.5679503, 74.660375, 71.7531881, 68.7101427, 65.6290678, 62.5102607, 59.3540188, 56.1009217, 52.8348715, 49.5800526, 46.3606494, 43.309762, 40.2990927, 37.3092595, 34.3208804, 31.2458379, 28.1609798, 25.0744183, 21.9942657, 18.872276, 15.7954629, 12.7944818, 9.89998836, 7.66367431, 5.38674428, 2.89143919, 0., -4.26469321, -8.96129429, -13.9478179, -19.0822789, -23.6261418, -28.2725914, -33.1182624, -38.2597896, -43.1364487, -48.7651769, -55.5055526, -63.7171541, -69.9228172, -79.8535597, -95.4036569, -118.467384, -125.376221, -153.812357, -215.895186, -323.744101, -462.041787, -725.217765, -1146.38373, -1806.37838, -2714.14396, -4118.48155, -6208.22748, -9172.21812}, 
{102.109544, 99.7569725, 97.8623984, 96.2978131, 94.9352071, 93.6465714, 92.3038964, 90.7791731, 88.9443922, 85.9985892, 82.7558921, 79.3574736, 75.9445064, 73.1306553, 70.3956043, 67.6915293, 64.9706062, 62.0343038, 59.0457882, 56.0175184, 52.9619535, 49.8802388, 46.8006721, 43.7402377, 40.7159199, 37.8110855, 34.9497834, 32.1224447, 29.3195008, 26.4924822, 23.6862811, 20.9068892, 18.1602981, 15.3777876, 12.6699457, 10.0726484, 7.62177209, 5.93500366, 4.23368424, 2.32096572, 0., -3.81327259, -8.07360397, -12.6229576, -17.3032971, -21.3601287, -25.4704559, -29.714825, -34.1737824, -38.2677164, -43.0013945, -48.7194263, -55.7664212, -60.5911985, -68.9924741, -82.8731736, -104.136223, -109.710419, -136.462466, -196.284943, -301.070425, -434.866515, -693.100714, -1109.41568, -1764.10962, -2672.48073, -4063.17178, -6111.48937, -8992.74014}, 
{94.2964493, 92.7695158, 91.3437153, 89.9575739, 88.5496178, 87.0583731, 85.422366, 83.5801226, 81.4701691, 78.5889691, 75.4939364, 72.300422, 69.1237773, 66.5079261, 63.9682178, 61.4485745, 58.8929184, 56.0413661, 53.1231674, 50.1637667, 47.1886082, 44.2663165, 41.3618836, 38.4834818, 35.6392833, 32.8548298, 30.1139763, 27.4179471, 24.7679666, 22.1583995, 19.6000738, 17.0969577, 14.6530196, 12.1827008, 9.81530731, 7.59061801, 5.54841194, 4.35432756, 3.17194063, 1.79068637, 0., -3.35720491, -7.16636284, -11.2594299, -15.4683622, -19.038262, -22.6226808, -26.2883164, -30.1018666, -33.4687308, -37.3814243, -42.1711641, -48.1691674, -51.7021831, -58.7076835, -71.1186729, -90.8681555, -95.5538487, -121.178158, -179.408204, -281.911104, -412.260057, -666.403945, -1078.65842, -1727.59583, -2634.41935, -4005.5522, -6001.11258, -8781.21869}, 
{87.7713906, 86.4960091, 85.1811683, 83.7968101, 82.3128763, 80.6993088, 78.9260494, 76.9630399, 74.7802222, 72.0511526, 69.1607128, 66.1973987, 63.2497064, 60.7494344, 58.3044555, 55.8659448, 53.3850774, 50.6152646, 47.784551, 44.9232173, 42.0615442, 39.301138, 36.5724235, 33.8771513, 31.2170719, 28.5794753, 25.9863566, 23.4452507, 20.9636922, 18.5573668, 16.2223978, 13.9630598, 11.783627, 9.60000198, 7.54017972, 5.64378337, 3.95043606, 3.11669301, 2.31847245, 1.34862468, 0., -2.86170759, -6.14562894, -9.68805125, -13.3252617, -16.3257928, -19.3207882, -22.3736369, -25.547728, -28.2713039, -31.4969588, -35.5421401, -40.7242953, -43.2310953, -49.1616752, -60.4853932, -79.1716077, -83.2430703, -108.194389, -165.573564, -266.928597, -396.429249, -647.75824, -1054.36803, -1693.5088, -2596.01584, -3934.58632, -5846.45193, -8468.84437}, 
{82.0309902, 80.8879567, 79.6096357, 78.1899457, 76.622805, 74.9021323, 73.0218458, 70.9758641, 68.7581056, 66.1932829, 63.5122026, 60.7764656, 58.0476726, 55.637402, 53.257286, 50.8689344, 48.4339568, 45.7340129, 42.9826428, 40.2134361, 37.4599828, 34.8511951, 32.2872114, 29.7634926, 27.2754996, 24.774658, 22.3180779, 19.918834, 17.5900013, 15.3641527, 13.2270657, 11.1840158, 9.24027873, 7.31904842, 5.54051468, 3.94278571, 2.56396973, 2.03191979, 1.55910133, 0.947724641, 0., -2.3603199, -5.10118416, -8.06899945, -11.1101725, -13.5280581, -15.9293355, -18.3776321, -20.9365754, -23.0678086, -25.6777369, -29.0707811, -33.5513625, -35.1556526, -40.1636214, -50.5869898, -68.4374784, -72.203814, -96.8299089, -153.736681, -254.34505, -383.750348, -632.350248, -1032.43624, -1659.18033, -2552.95931, -3850.45028, -5664.25527, -8106.97631}, 
{76.9707195, 75.8621394, 74.5611871, 73.0790334, 71.4268495, 69.6158063, 67.6570748, 65.5618261, 63.3412311, 60.9450827, 58.4704813, 55.9531493, 53.4288089, 51.087234, 48.7484747, 46.3866325, 43.9758091, 41.3365635, 38.6579569, 35.975508, 33.3247354, 30.8558433, 28.4437906, 26.0782215, 23.7487807, 21.3748538, 19.0444472, 16.775309, 14.585187, 12.5188412, 10.5562024, 8.70421353, 6.96981753, 5.28812718, 3.76664768, 2.44105406, 1.34702134, 1.06977331, 0.875616728, 0.580407118, 0., -1.85821402, -4.04652853, -6.42570206, -8.85649313, -10.6846322, -12.4919171, -14.3451174, -16.311003, -17.8933143, -19.9470619, -22.7642269, -26.6367909, -27.4525786, -31.6693904, -41.3408701, -58.5206616, -62.2399847, -86.7838771, -143.414952, -243.395824, -373.020461, -618.457409, -1010.56686, -1621.66818, -2501.0912, -3748.66476, -5451.59296, -7697.07988}, 
{72.4860498, 71.3353381, 69.967892, 68.4061262, 66.6724552, 64.7892935, 62.7790558, 60.6641567, 58.4670106, 56.2362749, 53.9576244, 51.6429764, 49.3042485, 47.0143357, 44.6997869, 42.3481283, 39.9468865, 37.3618686, 34.7510078, 32.1505183, 29.5966139, 27.2544382, 24.9797041, 22.7570538, 20.5711298, 18.3145381, 16.1007718, 13.9512876, 11.8875423, 9.96151604, 8.15193297, 6.46804054, 4.91908621, 3.45552525, 2.16891421, 1.09201745, 0.257599308, 0.200019021, 0.249808109, 0.239092969, 0., -1.36056209, -2.99516214, -4.78158666, -6.59762218, -7.83508972, -9.05212751, -10.3209083, -11.7136048, -12.7828905, -14.3282371, -16.629617, -19.9670027, -20.0985972, -23.6348501, -32.6644414, -49.2760512, -53.1554874, -77.7554513, -134.125772, -233.316278, -363.036691, -604.357163, -986.463671, -1578.03011, -2436.25295, -3624.75043, -5205.53533, -7240.62045}, 
{68.4724527, 67.2243335, 65.7618201, 64.1132768, 62.3070676, 60.3715567, 58.3351084, 56.2260867, 54.0728559, 51.9965824, 49.8957072, 47.7614734, 45.5851245, 43.3341123, 41.0329879, 38.682511, 36.2834413, 33.7488806, 31.2023098, 28.6795519, 26.21643, 23.9863352, 21.8324951, 19.7357053, 17.6767614, 15.5281868, 13.4223587, 11.3833816, 9.43536035, 7.63226104, 5.95638238, 4.41988452, 3.0349276, 1.76952966, 0.697649771, -0.150895095, -0.746287965, -0.607577646, -0.336535071, -0.0837969507, 0., -0.872536286, -1.96058503, -3.16008079, -4.36695815, -5.01900493, -5.65356114, -6.34982007, -7.18697498, -7.77160669, -8.84456592, -10.6740909, -13.5284199, -13.0704323, -16.0158687, -24.475111, -40.558541, -44.7542269, -69.4437895, -125.386536, -223.341773, -352.596143, -588.326948, -957.830483, -1525.32388, -2354.28602, -3474.22797, -4923.15275, -6739.06338}, 
{68.2547617, 64.4030038, 61.0624053, 58.1628557, 55.6342446, 53.4064616, 51.4093962, 49.572938, 47.8269765, 45.957039, 44.0951222, 42.2288605, 40.3458883, 38.4667234, 36.5329635, 34.5190896, 32.3995828, 29.9385344, 27.4049711, 24.8575296, 22.354847, 20.1021213, 17.9528037, 15.9069066, 13.9644425, 12.1438868, 10.4194037, 8.78362046, 7.22916423, 5.71567078, 4.28195543, 2.93384198, 1.67715426, 0.346381608, -0.812783878, -1.72598459, -2.3188629, -1.86839962, -1.20836336, -0.523861146, 0., -0.627400983, -1.46345147, -2.37105288, -3.21310661, -3.25085515, -3.18952241, -3.13267337, -3.18387301, -2.80547233, -2.9987359, -4.1237143, -6.54045811, -6.83371701, -10.6489628, -19.8563666, -36.3260991, -39.4859101, -62.6253602, -116.591589, -212.231736, -342.977808, -571.922343, -920.991834, -1451.388, -2230.85247, -3261.692, -4562.57208, -6152.15819}, 
{68.4539243, 61.4483189, 55.8628302, 51.4992593, 48.1594069, 45.6450741, 43.7580616, 42.3001706, 41.0732018, 39.3851077, 37.7290771, 36.1044501, 34.5105672, 33.1173068, 31.6852556, 30.1455386, 28.4292807, 26.0790926, 23.5700193, 20.9885914, 18.4213396, 16.0987041, 13.9057423, 11.871421, 10.0247072, 8.59280239, 7.32714494, 6.17740787, 5.09326418, 3.90357687, 2.727153, 1.56198962, 0.406083746, -1.08429602, -2.43273282, -3.50453824, -4.16502385, -3.33725149, -2.20568241, -1.01252808, 0., -0.497549335, -1.22525202, -1.99042368, -2.60037993, -2.04190726, -1.27106206, -0.423371592, 0.365636893, 1.81885925, 2.59897586, 2.22709019, 0.224305732, -1.26648705, -6.39468643, -16.6879037, -33.6737502, -35.6379855, -56.6468132, -107.524585, -199.095654, -329.609444, -547.615087, -868.664624, -1349.75899, -2057.6127, -2978.63703, -4119.31905, -5486.14585}, 
{65.4719479, 57.4208681, 51.1223649, 46.3288876, 42.792886, 40.2668095, 38.5031077, 37.2542302, 36.2726266, 34.6627386, 33.0842266, 31.5487434, 30.0679416, 28.9024083, 27.7152878, 26.4186592, 24.9246012, 22.6910859, 20.2659417, 17.7428902, 15.2156527, 12.9020086, 10.7219986, 8.71972156, 6.93927587, 5.70709501, 4.6720087, 3.76518156, 2.9177782, 1.89110459, 0.854127432, -0.194045212, -1.25430528, -2.72905117, -4.05706578, -5.07863845, -5.63405853, -4.49776766, -3.002242, -1.41410996, 0., -0.232086191, -0.689233061, -1.15585079, -1.41634954, -0.326326355, 1.02947019, 2.49510466, 3.91464163, 6.02483426, 7.41998307, 7.58707717, 6.01310568, 4.25526237, -1.09775015, -11.3870246, -27.9536538, -30.037432, -49.9212697, -97.7867791, -183.815572, -309.038656, -511.089458, -801.715921, -1230.32357, -1857.50504, -2661.44699, -3640.93761, -4794.76507}, 
{60.4371914, 52.8944439, 47.0061111, 42.5344545, 39.2417357, 36.8902165, 35.2421584, 34.0598231, 33.1054723, 31.4888045, 29.8856696, 28.3193545, 26.8131459, 25.6686144, 24.5194497, 23.2776252, 21.8551143, 19.7300162, 17.4217281, 15.0157733, 12.5976748, 10.34573, 8.215578, 6.25563199, 4.51430532, 3.33212144, 2.34853948, 1.49512871, 0.703458391, -0.282898639, -1.26917813, -2.24861224, -3.21443316, -4.5363881, -5.68058815, -6.48965946, -6.80622818, -5.41661253, -3.64226975, -1.74834914, 0., 0.144274795, 0.0900209157, 0.0494304766, 0.23469559, 1.80159795, 3.64130425, 5.58857079, 7.47815384, 9.93910759, 11.6941713, 12.260382, 11.1547768, 9.8495151, 5.12446293, -4.2853914, -19.6450596, -22.6973878, -42.0384195, -86.7420329, -165.882106, -281.689148, -463.767142, -721.985092, -1095.87966, -1636.34431, -2319.17767, -3138.9521, -4090.23996}, 
{54.4780137, 48.4428385, 43.6791707, 39.9986734, 37.21301, 35.1338437, 33.5728378, 32.3416556, 31.2519604, 29.5621783, 27.8585046, 26.1738973, 24.5413145, 23.2625116, 21.9941304, 20.6616102, 19.1903901, 17.1513851, 14.9663681, 12.7025882, 10.427294, 8.26356395, 6.20048559, 4.282976, 2.55595225, 1.31323844, 0.251281779, -0.684563488, -1.54894311, -2.57958539, -3.57482051, -4.51606121, -5.38472022, -6.45481104, -7.29810532, -7.77897552, -7.76179404, -6.16045051, -4.16999329, -2.03498794, 0., 0.606819966, 1.05792541, 1.54218478, 2.24846652, 4.24757601, 6.49367, 8.82284217, 11.0711862, 13.6883341, 15.6514259, 16.5511399, 15.9781543, 15.634255, 12.1545697, 4.28533388, -9.22721724, -13.6309911, -32.5879525, -73.754209, -144.785868, -247.984626, -407.069825, -631.311504, -949.22515, -1399.94534, -1960.88484, -2624.88686, -3384.79461}, 
{48.7227738, 44.6398442, 41.3066454, 38.604258, 36.4137625, 34.6162396, 33.0927698, 31.7244338, 30.3923122, 28.5817329, 26.7278303, 24.8699861, 23.0475818, 21.5306864, 20.0357196, 18.509788, 16.8999986, 14.9106943, 12.8288514, 10.6986821, 8.56439891, 6.48920575, 4.49072666, 2.60557732, 0.870373416, -0.504197236, -1.72521994, -2.82570787, -3.83867419, -4.96010825, -5.99485657, -6.91074186, -7.67558681, -8.43282424, -8.90442271, -8.98796086, -8.58101735, -6.79594598, -4.62964023, -2.2937687, 0., 1.13083567, 2.15989592, 3.23917679, 4.5206743, 6.91731822, 9.51579733, 12.1637342, 14.7087513, 17.3991688, 19.5216327, 20.7634863, 20.8120732, 21.7274658, 19.8751872, 13.9934892, 2.82062372, -2.85138016, -21.1595584, -58.1871699, -120.017474, -208.348795, -342.419193, -531.534524, -793.157935, -1154.12298, -1595.62429, -2110.26622, -2690.65314}, 
{44.2998304, 42.0592533, 40.0536373, 38.233922, 36.5510472, 34.9559528, 33.3995785, 31.832864, 30.2067492, 28.2463412, 26.2187454, 24.1652348, 22.1270823, 20.3197254, 18.5406064, 16.7613322, 14.9535099, 12.9634457, 10.9381676, 8.89940251, 6.86887778, 4.85635082, 2.90030642, 1.02725964, -0.73627446, -2.27482884, -3.6864212, -4.98011725, -6.16498272, -7.38561978, -8.46134313, -9.34700391, -9.99745327, -10.418932, -10.4943457, -10.1579897, -9.34415937, -7.38976337, -5.06543819, -2.54443376, 0., 1.69160825, 3.34134796, 5.05717119, 6.94702998, 9.71653493, 12.6369161, 15.5770622, 18.4058619, 21.1982666, 23.5346769, 25.2015565, 25.9853686, 28.2471315, 28.1689322, 24.5074123, 16.0192136, 9.6283068, -7.34292715, -39.4047781, -91.0675361, -163.20536, -271.236932, -424.493521, -630.475917, -904.692043, -1232.45178, -1606.61452, -2020.03966}, 
{31.3611692, 33.4138344, 34.6123875, 35.0543707, 34.837326, 34.0587958, 32.8163219, 31.2074467, 29.3297123, 27.404283, 25.3556298, 23.2318459, 21.0810247, 19.0533799, 17.0540359, 15.0902378, 13.1692306, 11.3600238, 9.58339213, 7.82187479, 6.05801097, 4.17746025, 2.29839333, 0.442101287, -1.37012481, -3.08825048, -4.73122545, -6.289256, -7.75254846, -9.29802506, -10.6544898, -11.7374626, -12.4624633, -12.5589911, -12.202995, -11.3844033, -10.0931443, -7.98083331, -5.51103668, -2.80900778, 0., 2.33855583, 4.71445469, 7.18331415, 9.80075179, 13.1193625, 16.4989955, 19.7964776, 22.8686353, 25.1976694, 27.1648828, 28.7769525, 30.0405554, 34.3745852, 37.0086155, 36.5844368, 31.7438393, 24.3660043, 8.56037482, -18.326215, -58.9469312, -111.659095, -192.003407, -312.956989, -466.821193, -649.230038, -863.162643, -1109.76185, -1390.17051}, 
{37.107078, 37.9494898, 38.1475584, 37.7748672, 36.9049999, 35.61154, 33.9680713, 32.0481773, 29.9254415, 27.7748251, 25.5279832, 23.2179484, 20.8777534, 18.5825868, 16.3064628, 14.0655515, 11.8760232, 9.83055734, 7.83821112, 5.88455095, 3.9551433, 1.96138975, -0.00731245436, -1.93573091, -3.80863323, -5.6288923, -7.35592834, -8.96726683, -10.4404333, -11.8918334, -13.1045604, -14.0005876, -14.5018883, -14.3327386, -13.6918882, -12.5803894, -10.9992944, -8.70169555, -6.03578926, -3.10181195, 0., 2.87088123, 5.82853593, 8.89213931, 12.0808666, 15.681342, 19.3383122, 22.9639726, 26.4705188, 29.5587418, 32.4368037, 35.1014621, 37.5494744, 42.4526156, 46.0626187, 47.3062344, 45.1102132, 39.9921181, 28.651562, 9.37897069, -19.5352305, -57.2665312, -113.12676, -196.55736, -298.235907, -413.117734, -537.811019, -668.510728, -801.411827}, 
{54.2496337, 50.3266662, 46.8385324, 43.7179324, 40.8975662, 38.3101336, 35.8883348, 33.5648695, 31.2724379, 28.7494751, 26.2006517, 23.6363737, 21.0670471, 18.525831, 15.9912767, 13.4646886, 10.9473713, 8.38916945, 5.86343143, 3.39204573, 0.99690084, -1.26416265, -3.41958917, -5.46187108, -7.3835007, -9.22601578, -10.9132451, -12.4180629, -13.7133433, -14.8183369, -15.6409909, -16.1356292, -16.2565754, -15.8913591, -15.0878157, -13.8269866, -12.089913, -9.575889, -6.66040207, -3.43719234, 0., 3.32808289, 6.77440878, 10.3369783, 14.0137919, 17.7915291, 21.6840401, 25.6938539, 29.8234995, 34.4683192, 39.0809036, 43.5066565, 47.5909816, 52.0892308, 55.5728806, 57.5233557, 57.4220807, 57.0603078, 52.6857029, 42.8557596, 26.1279713, 0.306807571, -33.7911656, -77.5102898, -128.97305, -194.828091, -250.841278, -280.499173, -267.288336}, 
{45.9632263, 43.5948047, 41.2944117, 39.0497927, 36.8486933, 34.6788586, 32.5280343, 30.3839656, 28.2343979, 26.0514214, 23.8446989, 21.6082379, 19.3360459, 16.9792869, 14.5919492, 12.1851779, 9.7701179, 7.3618403, 4.9659933, 2.59215129, 0.249888704, -2.11170034, -4.39836902, -6.57635081, -8.61187918, -10.3777734, -11.9710468, -13.3952985, -14.6541278, -15.978402, -17.0535448, -17.792248, -18.1072036, -17.711688, -16.7975747, -15.3573219, -13.3833878, -10.6397615, -7.43875759, -3.86422188, 0., 3.91966746, 8.02148764, 12.2817728, 16.6768352, 21.1786397, 25.7695849, 30.427722, 35.1311022, 39.788373, 44.4747508, 49.1960481, 53.9580778, 59.5390344, 64.8633958, 69.6280218, 73.5297724, 77.145323, 78.9397916, 78.2581119, 74.4452177, 65.9571768, 54.8055203, 38.1237308, 26.1576508, 25.2296282, 44.2436292, 91.4582154, 175.131949}, 
{51.3870855, 48.6794894, 46.1831565, 43.8403542, 41.59335, 39.3844114, 37.155806, 34.8498012, 32.4086645, 29.4555478, 26.3794804, 23.2503763, 20.1381492, 17.3421612, 14.6110986, 11.9230958, 9.25628726, 6.47079096, 3.70996456, 0.999149184, -1.63631406, -4.15046897, -6.54683554, -8.80831871, -10.9178234, -12.8574803, -14.6112782, -16.1624317, -17.4941555, -18.7345028, -19.6639141, -20.2076685, -20.2910453, -19.6603376, -18.491405, -16.7811208, -14.5263586, -11.5733769, -8.12991022, -4.25307808, 0., 4.55743405, 9.3827832, 14.4248361, 19.6323815, 24.7770149, 30.0555953, 35.4877886, 41.0932604, 46.9053614, 52.9245989, 59.1651649, 65.6412515, 72.5154609, 79.5942109, 86.8323292, 94.184644, 100.942288, 107.989262, 115.545873, 123.832425, 131.321797, 143.476579, 160.747982, 191.051251, 237.852606, 314.950336, 433.559716, 604.896017}, 
{52.5791609, 52.189412, 50.9809292, 49.0755733, 46.5952055, 43.6616866, 40.3968776, 36.9226396, 33.3608334, 30.1281937, 26.9337584, 23.7814391, 20.6751474, 17.664573, 14.689538, 11.7356427, 8.78848729, 5.69032192, 2.6274371, -0.357226835, -3.22072955, -5.77288536, -8.1768974, -10.4487235, -12.6043213, -14.8262476, -16.8972218, -18.766562, -20.3835865, -21.7939441, -22.8120902, -23.348811, -23.3148923, -22.3006067, -20.6654593, -18.4484415, -15.6885449, -12.4879459, -8.79717705, -4.62995592, 0., 5.28008011, 10.9150172, 16.810651, 22.8728212, 28.6611796, 34.566229, 40.6322844, 46.9036604, 53.1753873, 59.8407787, 67.0438633, 74.92867, 83.7004277, 93.417485, 104.199391, 116.165694, 127.53876, 141.094195, 157.710418, 178.265852, 202.213911, 234.708038, 276.259409, 335.688216, 421.713219, 535.1499, 678.789558, 855.423494}
};

  const double a4tab[50][69] = {
{-4795.25483, -4657.11547, -4533.57386, -4420.99082, -4315.72713, -4214.14359, -4112.601, -4007.46016, -3895.08186, -3749.86323, -3598.9142, -3447.38103, -3300.40998, -3183.58631, -3073.44169, -2966.94678, -2861.07226, -2743.59299, -2624.35375, -2504.00351, -2383.19127, -2264.35938, -2145.64608, -2026.98299, -1908.30176, -1789.65691, -1670.80798, -1551.63744, -1432.02772, -1310.85768, -1189.41483, -1067.98309, -946.846341, -827.165104, -707.996037, -589.272406, -470.927472, -353.153181, -235.520643, -117.859652, 0., 117.602012, 235.993705, 355.595891, 476.829381, 595.994952, 719.281464, 848.757742, 986.492612, 1125.29678, 1280.20043, 1456.97565, 1661.39451, 1865.75367, 2122.69078, 2451.36808, 2870.9478, 3306.89371, 3909.54591, 4735.54605, 5841.53576, 7186.8075, 9120.05042, 11591.8611, 15444.2924, 19758.4141, 29767.7712, 49167.7902, 81653.8973}, 
{-4386.2486, -4276.4998, -4166.87176, -4057.27842, -3947.63375, -3837.85169, -3727.8462, -3617.53124, -3506.82077, -3396.24985, -3284.86287, -3172.32536, -3058.30283, -2934.12142, -2811.12175, -2692.30507, -2580.67265, -2501.29049, -2426.26919, -2349.78411, -2266.0106, -2143.26267, -2011.92156, -1876.50719, -1741.53944, -1625.69861, -1513.68005, -1404.33951, -1296.53273, -1186.17111, -1076.23248, -966.750311, -857.758075, -749.581249, -641.844515, -534.46455, -427.358033, -320.645434, -213.958122, -107.131257, 0., 107.056887, 214.965285, 324.107473, 434.865731, 543.216635, 655.710448, 774.491733, 901.705051, 1031.01986, 1176.44587, 1343.51767, 1537.76987, 1733.97589, 1980.73599, 2295.88922, 2697.27467, 3113.77504, 3687.76832, 4472.67611, 5521.92004, 6796.91537, 8627.10278, 10961.4315, 14608.3709, 18698.1916, 28214.4395, 46677.342, 77607.1262}, 
{-4089.70007, -3999.83272, -3900.40845, -3793.52861, -3681.29454, -3565.8076, -3449.16915, -3333.48054, -3220.84311, -3124.77905, -3031.40055, -2938.24064, -2842.83236, -2728.08787, -2612.00939, -2497.97828, -2389.37592, -2309.8317, -2234.37973, -2158.30216, -2076.88115, -1964.99854, -1846.4969, -1724.8185, -1603.40559, -1496.11382, -1391.80673, -1289.76124, -1189.25426, -1088.05749, -987.55515, -887.626246, -788.149784, -688.774549, -589.701849, -490.902776, -392.348421, -294.336138, -196.38025, -98.321342, 0., 97.9874303, 196.858427, 297.074709, 399.097994, 499.259521, 603.803679, 714.844378, 834.49553, 956.8933, 1095.32044, 1255.08195, 1441.48285, 1629.7946, 1867.36916, 2171.52496, 2559.58043, 2963.77126, 3520.53168, 4281.21321, 5297.16736, 6532.52802, 8300.29955, 10550.9045, 14057.1258, 18028.6165, 27105.1878, 44569.1112, 73702.6582}, 
{-3869.29607, -3792.59815, -3700.71666, -3596.83571, -3484.13943, -3365.81194, -3245.03736, -3124.99981, -3008.88343, -2913.32449, -2822.67409, -2734.73548, -2647.31191, -2552.51261, -2456.11249, -2358.19241, -2258.83326, -2155.5432, -2052.00489, -1949.32827, -1848.62329, -1756.61105, -1666.5459, -1577.29332, -1487.71881, -1392.40141, -1296.20764, -1199.71758, -1103.51131, -1010.11081, -917.377477, -825.114634, -733.125587, -640.567837, -548.148831, -455.930204, -363.973591, -272.876208, -181.949879, -91.0420086, 0., 90.1866618, 181.269486, 273.857901, 368.561335, 462.482307, 561.139919, 666.546362, 780.71383, 897.948208, 1031.05052, 1185.11547, 1365.23778, 1545.93007, 1775.10198, 2070.08108, 2448.19491, 2844.61617, 3391.68922, 4139.60356, 5138.54867, 6355.55344, 8090.28923, 10297.6099, 13712.8828, 17642.831, 26328.2418, 42776.3866, 69994.5366}, 
{-3688.72344, -3620.28003, -3534.3291, -3434.29407, -3323.59833, -3205.66528, -3083.91832, -2961.78084, -2842.67626, -2739.75984, -2642.83036, -2551.41848, -2465.05485, -2394.42261, -2323.43893, -2247.17347, -2160.69588, -2024.75159, -1882.46418, -1742.633, -1614.05739, -1545.14423, -1489.24234, -1439.30806, -1388.29774, -1306.06025, -1217.90239, -1126.02348, -1032.62284, -945.925062, -859.694111, -773.719216, -687.789602, -600.783951, -513.766253, -426.889953, -340.308496, -254.916558, -169.829861, -84.9053578, 0., 83.447602, 167.794816, 253.817352, 342.290922, 431.243692, 525.297932, 626.328371, 736.209734, 849.215705, 977.862472, 1127.06518, 1301.73898, 1475.10257, 1696.44611, 1983.36334, 2353.44796, 2744.04363, 3285.09418, 4026.29337, 5017.33496, 6227.8996, 7947.72035, 10138.8771, 13497.9681, 17433.977, 25771.8272, 41232.4571, 66536.8049}, 
{-3511.669, -3448.36228, -3367.77851, -3272.99804, -3167.10117, -3053.16822, -2934.27953, -2813.5154, -2693.95617, -2581.95877, -2476.01625, -2377.89827, -2289.37451, -2240.84482, -2193.9966, -2139.14746, -2066.61499, -1903.78345, -1729.07712, -1557.98694, -1406.00384, -1357.64213, -1331.76003, -1316.23912, -1298.96098, -1228.58923, -1147.91061, -1060.49389, -969.90785, -889.094259, -808.499694, -727.943732, -647.245948, -565.245724, -483.134905, -401.125143, -319.42809, -239.108105, -159.183049, -79.523491, 0., 77.5632713, 156.030772, 236.313367, 319.321921, 403.902372, 493.856483, 590.921089, 696.833024, 805.726906, 929.982676, 1074.37806, 1243.69077, 1410.03238, 1623.91323, 1903.17752, 2265.66943, 2649.78746, 3184.59978, 3919.72886, 4904.79718, 6111.47446, 7823.24142, 10012.0358, 13334.7074, 17295.1967, 25324.1697, 39870.6114, 63383.5064}, 
{-3301.81958, -3242.32883, -3167.59761, -3080.04195, -2982.07786, -2876.12137, -2764.5885, -2649.89526, -2534.45768, -2417.79493, -2306.37861, -2203.78347, -2113.58426, -2078.80618, -2047.79337, -2008.34038, -1948.2418, -1778.96537, -1595.16323, -1415.16067, -1257.28299, -1221.1488, -1211.27277, -1213.4629, -1213.52717, -1151.48722, -1077.25191, -994.943767, -908.685307, -833.212394, -757.788871, -682.291925, -606.598741, -529.775993, -452.835581, -375.978895, -299.407326, -224.101762, -149.172293, -74.5085093, 0., 72.3266902, 145.573709, 220.706247, 298.689499, 378.817046, 464.394334, 557.055201, 658.433482, 762.51293, 881.637499, 1020.50106, 1183.79748, 1343.43977, 1550.015, 1821.32939, 2175.18916, 2549.58149, 3074.05924, 3798.35623, 4772.2063, 5968.186, 7667.50096, 9854.41554, 13145.4268, 17119.632, 24873.4951, 38624.1385, 60588.6845}, 
{-3022.862, -2967.66361, -2900.31911, -2822.52015, -2735.95834, -2642.32532, -2543.31273, -2440.61219, -2335.91534, -2225.14199, -2118.06432, -2018.68268, -1930.99743, -1895.33367, -1864.83711, -1828.97824, -1777.2275, -1636.62394, -1484.04202, -1333.92478, -1200.71524, -1162.70827, -1144.95436, -1136.35581, -1125.81492, -1066.2531, -996.945919, -921.188061, -842.274195, -771.873467, -701.556286, -631.267537, -560.952101, -490.197592, -419.449071, -348.794329, -278.321158, -208.548445, -138.960447, -69.472514, 0., 67.5308789, 136.01998, 206.356297, 279.428824, 354.34641, 434.49025, 521.461393, 616.860889, 714.604893, 827.05331, 958.88115, 1114.76342, 1268.04501, 1467.2631, 1729.62474, 2072.337, 2431.15956, 2937.32579, 3640.6217, 4590.83328, 5759.94219, 7431.14749, 9603.34594, 12852.4522, 16800.4251, 24308.029, 37426.3271, 58206.3824}, 
{-2638.48309, -2589.85055, -2532.47575, -2467.52697, -2396.17251, -2319.58066, -2238.91972, -2155.35797, -2070.06371, -1981.87362, -1895.22025, -1812.20453, -1734.92739, -1677.45422, -1625.13573, -1575.28704, -1525.22331, -1463.08574, -1399.03301, -1334.04983, -1269.12095, -1209.3646, -1149.97859, -1090.29423, -1029.64285, -964.385738, -898.012251, -831.041724, -763.993491, -698.671476, -633.796583, -569.374308, -505.410146, -442.333357, -379.556167, -316.914565, -254.24454, -191.09907, -127.710361, -64.0276063, 0., 62.9688578, 126.965942, 192.62382, 260.575062, 328.849163, 401.722993, 480.870349, 567.965028, 657.033913, 760.456481, 882.965295, 1029.29292, 1176.56839, 1368.1692, 1619.86934, 1947.44278, 2282.2555, 2758.25267, 3424.97148, 4331.9491, 5448.65099, 7064.82952, 9196.15657, 12378.1095, 16230.7179, 23515.9972, 36210.4661, 56290.6436}, 
{-2405.71268, -2357.37371, -2305.09333, -2248.92728, -2188.93131, -2125.16116, -2057.67258, -1986.52131, -1911.7631, -1825.78366, -1739.37678, -1655.66624, -1577.77578, -1521.53728, -1472.28315, -1428.05392, -1386.8901, -1345.84384, -1304.33938, -1260.8126, -1213.69937, -1153.95915, -1090.49479, -1024.73273, -958.099404, -895.75918, -833.905408, -772.469359, -711.382305, -650.167044, -589.326712, -528.955972, -469.149485, -410.875751, -353.00606, -295.28554, -237.459321, -178.716175, -119.580127, -60.018845, 0., 59.1806414, 119.414746, 181.265883, 245.297621, 309.449323, 377.958449, 452.438249, 534.501976, 618.058134, 715.506621, 831.542589, 970.861189, 1111.62937, 1295.68177, 1538.3248, 1854.86492, 2173.99376, 2632.27845, 3279.67134, 4166.12475, 5260.24742, 6856.02255, 8984.53783, 12142.789, 15991.1783, 23070.5741, 35201.7283, 54205.3928}, 
{-2204.0302, -2154.47867, -2105.90828, -2057.23597, -2007.37868, -1955.25335, -1899.77692, -1839.86634, -1774.43854, -1688.83889, -1600.98454, -1515.22105, -1435.89401, -1383.22182, -1339.32807, -1302.20921, -1269.86166, -1242.18385, -1214.50944, -1184.07407, -1148.11338, -1090.76646, -1027.60414, -961.10068, -893.730354, -835.115058, -777.722379, -721.167533, -665.065735, -607.620119, -550.422817, -493.653877, -437.493349, -383.360258, -329.700085, -276.19729, -222.536329, -167.664731, -112.298656, -56.4173358, 0., 55.7448171, 112.55891, 170.95477, 231.444888, 291.989405, 356.674102, 427.03241, 504.59776, 583.170483, 675.11035, 785.044031, 917.598198, 1051.62411, 1227.83402, 1461.16475, 1766.55314, 2070.16998, 2511.22455, 3140.16011, 4007.4199, 5080.84751, 6658.68514, 8786.8724, 11925.8417, 15777.5862, 22663.3632, 34249.6139, 52202.7796}, 
{-2019.11349, -1967.86498, -1922.03119, -1879.54631, -1838.34453, -1796.36004, -1751.52704, -1701.77973, -1645.05228, -1560.9017, -1472.99026, -1386.60303, -1307.02508, -1257.73185, -1218.54189, -1187.46414, -1162.50754, -1146.0165, -1129.93027, -1110.52355, -1084.07109, -1029.27314, -967.008674, -900.58221, -833.298258, -778.328451, -725.163331, -673.160561, -621.677803, -567.845774, -514.139863, -460.808512, -408.100162, -357.77398, -307.963395, -258.312561, -208.46563, -157.218748, -105.403279, -53.0025778, 0., 52.510405, 106.100252, 161.230464, 218.361962, 275.485215, 336.519778, 402.914754, 476.119245, 549.893005, 636.450223, 740.315742, 866.014403, 993.013679, 1160.91873, 1384.27734, 1677.6373, 1964.76951, 2387.3094, 2996.11552, 3842.0464, 4892.15254, 6448.71667, 8571.55319, 11686.9318, 15534.6254, 22233.9388, 33309.2935, 50285.1113}, 
{-1852.906, -1799.92433, -1755.7931, -1717.7438, -1683.00798, -1648.81714, -1612.4028, -1570.99649, -1521.82971, -1440.70191, -1354.84952, -1270.0769, -1192.18842, -1146.35433, -1111.26673, -1084.98361, -1065.56296, -1056.86211, -1048.81999, -1037.17485, -1017.66497, -965.979445, -905.925352, -841.260613, -775.743151, -724.511292, -675.390392, -627.586209, -580.304504, -530.041543, -479.796376, -429.858557, -380.517643, -333.704815, -287.411353, -241.270163, -194.914152, -147.113112, -98.7083065, -49.6778856, 0., 49.3678455, 99.8185964, 151.765844, 205.623179, 259.438588, 316.93751, 379.479777, 448.425223, 517.590152, 598.895338, 696.718027, 815.435464, 935.025994, 1094.02532, 1306.57025, 1586.79759, 1856.40165, 2258.93873, 2845.52263, 3667.26715, 4690.50381, 6220.69328, 8329.94908, 11413.3409, 15245.1024, 21759.8548, 32352.1221, 48416.4288}, 
{-1707.35118, -1653.0484, -1609.52501, -1573.71394, -1542.54814, -1512.96053, -1481.88405, -1446.25164, -1402.99624, -1326.96932, -1246.01786, -1165.90739, -1092.40343, -1050.37625, -1018.84473, -995.932496, -979.763174, -974.240986, -969.396721, -961.041762, -944.987495, -897.385648, -841.571127, -781.219183, -720.005065, -672.775514, -627.56569, -583.582244, -540.031828, -493.404962, -446.710883, -400.242696, -354.293506, -310.740658, -267.659322, -224.708907, -181.548824, -137.082706, -92.0280493, -46.3465739, 0., 46.2075789, 93.4937656, 142.233789, 192.802879, 243.351361, 297.36933, 356.121975, 420.874489, 485.626378, 561.814791, 653.61119, 765.18704, 876.888988, 1026.24324, 1226.95119, 1492.71423, 1743.67573, 2124.5183, 2686.36654, 3480.34503, 4472.24266, 5969.19114, 8053.42895, 11092.3504, 14891.8235, 21218.6652, 31349.4551, 46560.7729}, 
{-1584.39249, -1529.62886, -1485.55795, -1449.34221, -1418.14411, -1389.1261, -1359.45065, -1326.28022, -1286.77728, -1218.43376, -1145.95085, -1074.35921, -1008.68952, -971.084549, -942.618, -921.475684, -905.843417, -897.67345, -889.878586, -879.138066, -862.131131, -819.992009, -771.162956, -718.541216, -665.024034, -622.233049, -580.851352, -540.286431, -499.94577, -457.133564, -414.20191, -371.399611, -328.975471, -288.469407, -248.322665, -208.267601, -168.036574, -126.862414, -85.1768186, -42.9119574, 0., 42.9200457, 86.9055837, 132.30718, 179.4754, 226.72537, 277.257271, 332.235845, 392.825836, 453.366138, 524.577678, 610.355535, 714.594789, 817.830593, 956.661925, 1144.32784, 1394.06738, 1625.20104, 1982.45386, 2516.63232, 3278.54291, 4233.71041, 5688.78638, 7733.36167, 10711.2418, 14457.5951, 20587.924, 30272.6475, 44682.1844}, 
{-1485.97338, -1432.0574, -1386.22294, -1346.5141, -1310.975, -1277.64975, -1244.58246, -1209.81724, -1171.39821, -1113.82504, -1054.10405, -995.697115, -942.066106, -909.766207, -883.928673, -862.778061, -844.53893, -826.679817, -808.483705, -788.47756, -765.188346, -730.298793, -691.917793, -651.310007, -609.740092, -571.99583, -534.409508, -496.836536, -459.132322, -420.424886, -381.587985, -342.767985, -304.111253, -266.47896, -229.016746, -191.585055, -154.044331, -116.18712, -77.9689255, -39.2773507, 0., 39.3956861, 79.8338744, 121.658895, 165.21508, 209.062449, 256.043366, 307.215884, 363.638057, 420.173885, 486.553096, 566.311367, 662.984373, 757.078737, 884.370809, 1057.60788, 1289.53726, 1499.58688, 1831.15114, 2334.30506, 3059.12368, 3971.24838, 5374.05517, 7361.11611, 10257.2967, 13925.2237, 19845.1853, 29093.0545, 42744.7041}, 
{-1376.76382, -1326.68783, -1282.07861, -1241.75639, -1204.5414, -1169.25385, -1134.71398, -1099.742, -1063.15815, -1015.4217, -967.058202, -920.232267, -877.1085, -849.472809, -826.019977, -805.066087, -784.927224, -759.752074, -733.691079, -706.727285, -678.843735, -650.016673, -620.238666, -589.495479, -557.772878, -524.522938, -490.478592, -455.839083, -420.803651, -385.914211, -350.890265, -315.793989, -280.687555, -245.903895, -211.126125, -176.308115, -141.403738, -106.570871, -71.4777782, -35.9967311, 0., 36.0990347, 73.185657, 111.604041, 151.698359, 192.269829, 235.822765, 283.318523, 335.718461, 388.430658, 450.191058, 524.182331, 613.587143, 698.997506, 815.223007, 974.482576, 1188.99514, 1378.73642, 1685.06585, 2157.09963, 2843.95399, 3710.28866, 5058.5893, 6988.85511, 9802.94449, 13373.0307, 19088.1684, 27958.192, 40992.9357}, 
{-1237.58523, -1193.72216, -1155.05249, -1120.43609, -1088.73286, -1058.80268, -1029.50544, -999.701022, -968.249314, -926.239712, -883.41079, -841.730634, -803.167327, -778.318849, -757.071429, -737.941191, -719.444261, -696.334781, -672.39565, -647.647786, -622.112106, -595.968306, -569.015013, -541.209634, -512.509574, -482.320899, -451.372892, -419.843492, -387.910642, -356.060285, -324.03916, -291.902006, -259.703565, -227.751645, -195.74669, -163.642216, -131.391734, -99.0699117, -66.4606474, -33.4689929, 0., 33.4513572, 67.8059178, 103.394599, 140.548317, 178.214416, 218.660816, 262.771863, 311.431906, 360.448557, 417.813589, 486.442042, 569.248956, 647.275805, 754.06062, 901.267868, 1100.56201, 1272.16848, 1555.76639, 1999.59584, 2651.8969, 3474.58878, 4774.87422, 6664.71547, 9415.52636, 12887.0161, 18426.2608, 27008.4947, 39608.9522}, 
{-1105.58134, -1068.28477, -1035.7158, -1006.80631, -980.488224, -955.693435, -931.35385, -906.401371, -879.767902, -843.331603, -805.899619, -769.225353, -735.062207, -712.723629, -693.378956, -675.757571, -658.588856, -637.292406, -615.231308, -592.458861, -569.028363, -545.287037, -520.876688, -495.733044, -469.791833, -442.442738, -414.385952, -385.775624, -356.765902, -327.780257, -298.595783, -269.258899, -239.816024, -210.549023, -181.174687, -151.645255, -121.912965, -91.969331, -61.7116068, -31.0763203, 0., 30.9521313, 62.7237173, 95.6297075, 129.985051, 164.863675, 202.31796, 243.159265, 288.198947, 333.654277, 386.768336, 450.190117, 526.568614, 597.431846, 694.998168, 830.364963, 1014.62961, 1168.42203, 1429.49406, 1845.13006, 2462.6144, 3241.06015, 4492.26561, 6339.07397, 9025.31546, 12395.8862, 17770.4317, 26099.91, 38335.2793}, 
{-981.843359, -951.135893, -924.577433, -901.1915, -880.001617, -860.031308, -840.304093, -819.843497, -797.673041, -766.547528, -734.266689, -702.361533, -672.363072, -652.260502, -634.543371, -618.159416, -602.056372, -582.353316, -561.958105, -540.949937, -519.408011, -497.806608, -475.671811, -452.924788, -429.486703, -404.761057, -379.393749, -353.513012, -327.24708, -300.95003, -274.433913, -247.736624, -220.896058, -194.167337, -167.28424, -140.19777, -112.858933, -85.1877245, -57.178561, -28.7948502, 0., 28.5987857, 57.9394136, 88.3159941, 120.022638, 152.2321, 186.808389, 224.494156, 266.032056, 308.046801, 357.046158, 415.419955, 485.558019, 549.519157, 638.156626, 761.992664, 931.549505, 1067.95242, 1306.87941, 1694.61147, 2277.42964, 3011.70205, 4213.44837, 6014.88302, 8636.06863, 11907.0428, 17130.0951, 25239.4523, 37169.3408}, 
{-867.462487, -843.035749, -822.146291, -803.916104, -787.467177, -771.921499, -756.401059, -740.027848, -721.923855, -695.737645, -668.254002, -640.784284, -614.639853, -596.502818, -580.165486, -564.790917, -549.542169, -531.245875, -512.336089, -492.910439, -473.066553, -453.360756, -433.248503, -412.643943, -391.461228, -369.148461, -346.272259, -322.93319, -299.231825, -275.445509, -251.427325, -227.207134, -202.814794, -178.477895, -153.949475, -129.180301, -104.121142, -78.6436884, -52.8094149, -26.6007194, 0., 26.3887492, 53.4533645, 81.4600863, 110.675155, 140.334184, 172.146291, 206.789968, 244.943705, 283.625111, 328.637912, 382.124952, 446.229074, 503.591266, 583.656969, 696.36977, 851.673255, 971.215005, 1188.55301, 1548.94926, 2097.66575, 2788.51376, 3941.10738, 5695.09497, 8251.54269, 11427.8877, 16514.6653, 24434.1361, 36108.5609}, 
{-763.529923, -744.744569, -728.931269, -715.304576, -703.079038, -691.469208, -679.689636, -666.954874, -652.479471, -630.752109, -607.603558, -584.138715, -561.462481, -545.02393, -529.846115, -515.296264, -500.741608, -483.698449, -466.12531, -448.129792, -429.819492, -411.783222, -393.454883, -374.749589, -355.582454, -335.477555, -314.897458, -293.913693, -272.597788, -251.142599, -229.449798, -207.542384, -185.443357, -163.352002, -141.044517, -118.473386, -95.5910942, -72.2558187, -48.5520731, -24.4700646, 0., 24.3194502, 49.2659279, 75.0686116, 101.95668, 129.18442, 158.345858, 190.060128, 224.946366, 260.38819, 301.534457, 350.298506, 408.59368, 459.701698, 531.62017, 633.715083, 775.352427, 878.665118, 1075.14545, 1409.05263, 1924.64589, 2573.49455, 3677.92749, 5382.66222, 7875.49447, 10965.8228, 15933.5562, 23690.976, 35150.3634}, 
{-671.136869, -657.022582, -645.441263, -635.681364, -627.031337, -618.779636, -610.214713, -600.625022, -589.299015, -571.441079, -552.057359, -532.069936, -512.400888, -497.39719, -483.186069, -469.319648, -455.35005, -439.439403, -423.08582, -406.397423, -389.482331, -372.907744, -356.139071, -339.100802, -321.717425, -303.620942, -285.145324, -266.332054, -247.222617, -227.917203, -208.375107, -188.61433, -168.652875, -148.660967, -128.443493, -107.957565, -87.1602927, -65.9427114, -44.3544406, -22.3790227, 0., 22.3883175, 45.3774618, 69.1481977, 93.8812897, 118.797302, 145.421279, 174.318067, 206.052511, 238.33502, 275.726649, 319.934016, 372.66374, 417.903983, 482.167203, 574.247404, 702.938588, 790.758109, 967.287275, 1275.83075, 1759.69318, 2368.6437, 3426.5936, 5080.53715, 7511.68081, 10528.25, 15396.182, 23016.9864, 34292.1725}, 
{-591.374525, -580.630018, -572.185168, -565.370918, -559.518208, -553.957981, -548.021179, -541.038742, -532.341613, -517.654709, -501.357407, -484.223055, -467.025006, -453.195949, -439.786162, -426.505259, -413.062857, -398.197102, -382.977668, -367.502757, -351.870573, -336.568062, -321.149189, -305.556662, -289.733187, -273.451226, -256.891831, -240.065808, -222.983963, -205.645227, -188.07703, -170.294927, -152.314472, -134.276096, -116.020528, -97.5133744, -78.7202402, -59.6229627, -40.164422, -20.3037303, 0., 20.5927796, 41.7883242, 63.7054721, 86.463062, 109.187321, 133.386744, 159.577213, 188.274612, 217.464584, 251.205346, 291.024877, 338.451155, 378.251645, 435.419044, 518.185534, 634.783301, 707.949324, 865.609071, 1150.19281, 1604.13079, 2175.96048, 3189.79057, 4791.67214, 7163.85853, 10122.571, 14911.9569, 22419.182, 33531.4121}, 
{-531.224895, -520.626258, -512.797608, -506.999762, -502.493538, -498.539755, -494.399231, -489.332783, -482.601229, -469.841541, -455.387921, -439.950727, -424.240315, -411.213282, -398.435247, -385.718072, -372.873616, -359.085644, -345.04535, -330.815832, -316.46019, -302.247325, -287.952211, -273.555623, -259.038339, -244.396642, -229.589599, -214.591784, -199.377772, -183.808107, -168.017004, -152.02465, -135.851231, -119.609428, -103.189935, -86.5759385, -69.7506277, -52.91163, -35.741918, -18.1389039, 0., 18.9692432, 38.6028073, 58.9265352, 79.9662703, 100.621877, 122.495568, 146.063579, 171.802143, 197.878892, 228.002105, 263.571461, 305.986636, 341.038107, 391.978429, 466.450961, 572.099059, 631.266927, 771.016734, 1033.1115, 1459.31424, 1998.35806, 2971.09571, 4515.64123, 6831.24812, 9763.79368, 14511.5205, 21934.5798, 32893.1229}, 
{-486.583679, -473.979814, -465.030685, -458.861989, -454.599418, -451.368669, -448.295435, -444.505412, -439.124294, -427.323975, -413.765471, -399.155998, -384.202771, -371.710721, -359.450261, -347.28952, -335.096629, -322.375158, -309.503618, -296.495961, -283.36614, -270.088482, -256.732415, -243.32774, -229.904261, -216.721713, -203.487992, -190.140926, -176.618344, -162.629655, -148.432476, -134.056005, -119.529439, -104.937331, -90.2313794, -75.4186386, -60.5061629, -45.9950208, -31.2006471, -15.9324906, 0., 17.4807597, 35.7296022, 54.6597253, 74.1843266, 92.8924074, 112.55104, 133.603101, 156.491467, 179.486059, 206.071889, 237.561016, 275.265497, 306.105316, 351.541434, 418.642738, 514.478116, 560.661054, 683.898001, 925.440002, 1326.53811, 1837.0297, 2772.40681, 4258.3764, 6521.60732, 9452.61168, 14185.5666, 21543.733, 32350.3718}, 
{-448.814905, -434.351947, -424.23195, -417.480755, -413.1242, -410.188124, -407.698365, -404.680764, -400.161159, -388.964091, -375.997217, -361.966893, -347.579478, -335.550049, -323.772757, -312.150471, -300.586061, -288.808322, -276.963829, -265.025081, -252.964578, -240.504183, -227.967286, -215.426643, -202.955006, -191.03281, -179.162057, -167.252429, -155.213609, -142.637141, -129.878101, -116.973426, -103.960054, -90.8894741, -77.7782523, -64.6575056, -51.5583513, -39.2929482, -26.798955, -13.7950721, 0., 16.0603954, 32.9974454, 50.6153159, 68.7181726, 85.5958004, 103.172498, 121.858185, 142.062778, 162.114943, 185.338352, 212.975426, 246.268582, 273.110677, 313.433519, 373.819356, 460.850433, 495.64119, 604.4288, 827.982629, 1207.07205, 1692.46558, 2594.93511, 4028.40848, 6246.18361, 9183.8677, 13908.4607, 21204.1145, 31854.9808}, 
{-415.107526, -399.828275, -389.110135, -381.960008, -377.384796, -374.391399, -371.986718, -369.177655, -364.971111, -354.165601, -341.659766, -328.143861, -314.308141, -302.780952, -291.539221, -280.497968, -269.57221, -258.587577, -247.584233, -236.512952, -225.324507, -213.595855, -201.801116, -190.040593, -178.414589, -167.532131, -156.781306, -146.058926, -135.261805, -123.926816, -112.454684, -100.886198, -89.2621454, -77.5906511, -65.9582303, -54.4187358, -43.0260202, -32.8911881, -22.5879395, -11.7472262, 0., 14.6788678, 30.3399124, 46.6897497, 63.4349954, 78.6160763, 94.2722731, 110.776677, 128.50238, 145.787851, 165.854652, 189.889726, 219.080012, 241.990545, 277.478935, 331.780884, 411.132096, 436.62174, 533.690667, 742.633193, 1103.74364, 1567.96956, 2443.64553, 3835.9396, 6019.38928, 8970.19375, 13688.5361, 20918.6033, 31404.5825}, 
{-381.404703, -367.810516, -358.064516, -351.312694, -346.701042, -343.375552, -340.482213, -337.167018, -332.575957, -322.140584, -310.207104, -297.407282, -284.372885, -273.56541, -263.055001, -252.741533, -242.524879, -232.132597, -221.705805, -211.213306, -200.6239, -189.585152, -178.515596, -167.512531, -156.673252, -146.561658, -136.621805, -126.76435, -116.899948, -106.625098, -96.2902787, -85.9318111, -75.5860158, -65.1968036, -54.9298686, -44.858495, -35.0559672, -26.8956431, -18.630704, -9.81440464, 0., 13.2974927, 27.6712859, 42.7528299, 58.1735747, 71.8319533, 85.7856399, 100.359291, 115.877565, 130.624358, 147.781391, 168.489624, 193.890017, 212.684812, 243.429178, 292.239562, 365.232414, 384.336086, 473.530361, 672.606928, 1021.35747, 1469.13545, 2327.04725, 3698.40244, 5866.54264, 8835.96394, 13546.5125, 20704.1969, 31015.0258}, 
{-350.693994, -339.567532, -331.244199, -325.066325, -320.376238, -316.516268, -312.828743, -308.655992, -303.340345, -293.184706, -281.786597, -269.704117, -257.495364, -247.430419, -237.670605, -228.089227, -218.55959, -208.634708, -198.636295, -188.565772, -178.424563, -168.013895, -157.615462, -147.310764, -137.1813, -127.67297, -118.357114, -109.169472, -100.045784, -90.6878242, -81.3588831, -72.0882854, -62.9053565, -53.6877426, -44.6771193, -35.9634834, -27.6368316, -21.2984096, -14.9224656, -7.99449665, 0., 11.9307478, 25.0149408, 38.8249936, 52.933321, 65.1521334, 77.5181313, 90.3078109, 103.797669, 116.194025, 130.671622, 148.335027, 170.288808, 185.113374, 211.447112, 255.404249, 323.099016, 337.713944, 421.467639, 613.647007, 953.538957, 1388.60832, 2233.60823, 3592.34408, 5751.29326, 8739.15611, 13433.0691, 20498.0598, 30599.1557}, 
{-322.965118, -314.722469, -308.053627, -302.529432, -297.72072, -293.198329, -288.533096, -283.29586, -277.057457, -267.132496, -256.250535, -244.884904, -233.50893, -224.173201, -215.142885, -206.260408, -197.368196, -187.807977, -178.123154, -168.356433, -158.550519, -148.713216, -138.936092, -129.275811, -119.789041, -110.754011, -101.917197, -93.246636, -84.7103687, -76.143424, -67.7000552, -59.4015055, -51.2690183, -43.1182337, -35.2582393, -27.7925197, -20.8245593, -16.1400159, -11.4873314, -6.29712101, 0., 10.5704871, 22.3478263, 34.8625739, 47.6452865, 58.4725586, 69.3304936, 80.4512328, 92.0669177, 102.300478, 114.336951, 129.252163, 148.121941, 159.204636, 181.520537, 221.27246, 284.663221, 296.452606, 376.86367, 564.67644, 898.670942, 1324.54377, 2160.32525, 3511.71103, 5663.53469, 8666.0039, 13328.6919, 20271.3306, 30113.6518}, 
{-298.207795, -292.898473, -287.897243, -283.010547, -278.044824, -272.806515, -267.102061, -260.737901, -253.520476, -243.818486, -233.451208, -222.800179, -212.246936, -203.590978, -195.228694, -186.974434, -178.64255, -169.366469, -159.913835, -150.371369, -140.825792, -131.514249, -122.312866, -113.248194, -104.346783, -95.6927269, -87.2320163, -78.9681837, -70.9047623, -63.0203273, -55.353353, -47.9173557, -40.7258518, -33.5430427, -26.7314855, -20.4044225, -14.675096, -11.4609901, -8.34940844, -4.73189658, 0., 9.2085648, 19.6468917, 30.8219035, 42.2405231, 51.6891709, 61.0834733, 70.6185542, 80.4895374, 88.7473442, 98.5889822, 111.067257, 127.234972, 134.887001, 153.637255, 189.841712, 249.85635, 260.249361, 339.079625, 524.618236, 855.136285, 1275.09739, 2104.19508, 3450.4498, 5593.16045, 8602.74098, 13213.8667, 19995.1477, 29515.1939}, 
{-277.692073, -273.266665, -268.661563, -263.791764, -258.572268, -252.918071, -246.744171, -239.965567, -232.497256, -223.318657, -213.654579, -203.794253, -194.026909, -185.767148, -177.728681, -169.75059, -161.671955, -152.674565, -143.517713, -134.303398, -125.133621, -116.351293, -107.721136, -99.2487811, -90.9398631, -82.7556789, -74.7639327, -66.9879926, -59.4512265, -52.2008259, -45.2268059, -38.543005, -32.163262, -25.856573, -19.9795559, -14.6439865, -9.96164035, -7.81861146, -5.84262963, -3.43574307, 0., 7.74471995, 16.6413325, 26.2149221, 35.990573, 43.8290239, 51.5854432, 59.4506533, 67.6154769, 74.1745522, 82.25336, 92.8811966, 107.087358, 112.157665, 128.36228, 162.227891, 220.281186, 230.228588, 308.945153, 494.485671, 824.904937, 1246.65773, 2072.59887, 3408.55568, 5528.94543, 8536.44835, 13052.1028, 19567.3577, 28573.6617}, 
{-259.939683, -255.968472, -251.491441, -246.505072, -241.005847, -234.990248, -228.454756, -221.395854, -213.810024, -205.1907, -196.23863, -187.151517, -178.127059, -170.170813, -162.349484, -154.537632, -146.609818, -137.847271, -128.955213, -120.045537, -111.230135, -102.937193, -94.8357926, -86.9113071, -79.1491114, -71.3967003, -63.8324798, -56.4969763, -49.4307162, -42.7343354, -36.3642068, -30.3368131, -24.6686367, -19.1508851, -14.1154261, -9.66885215, -5.91775591, -4.66091588, -3.63586439, -2.27231968, 0., 6.27642281, 13.5921656, 21.5074915, 29.5826637, 35.7843028, 41.9037718, 48.1387909, 54.6870805, 59.7199925, 66.272163, 75.3518594, 87.9673494, 90.8661607, 105.021597, 137.146221, 193.952597, 204.451318, 284.137707, 470.805114, 802.246893, 1227.94069, 2050.62697, 3373.0398, 5462.06578, 8452.38814, 12843.3511, 19043.1335, 27459.9144}, 
{-244.613452, -240.742451, -236.178876, -230.976389, -225.188652, -218.869326, -212.072073, -204.850553, -197.258429, -189.207498, -180.950031, -172.596435, -164.257118, -156.526127, -148.836772, -141.106004, -133.250774, -124.687935, -116.034573, -107.407678, -98.9242389, -91.0767404, -83.4564783, -76.0302437, -68.7648278, -61.4070679, -54.2316906, -47.2934683, -40.6471739, -34.4312784, -28.5833766, -23.1247619, -18.0767276, -13.2663693, -8.98685713, -5.33716324, -2.41625999, -1.89649508, -1.67411532, -1.2187429, 0., 4.81904605, 10.5395963, 16.7694066, 23.1162328, 27.6727934, 32.1678966, 36.8153135, 41.8288147, 45.4844063, 50.7087301, 58.4906628, 69.8190813, 70.9233303, 83.4556316, 114.308675, 170.37515, 182.25048, 263.643527, 451.965888, 784.629157, 1214.99982, 2032.62481, 3336.30428, 5382.70068, 8336.57764, 12572.4318, 18412.3265, 26178.3252}, 
{-231.376208, -227.327158, -222.515866, -217.031636, -210.963774, -204.401586, -197.434378, -190.151455, -182.642124, -175.141937, -167.535452, -159.853477, -152.126817, -144.557247, -136.936217, -129.226146, -121.389454, -112.999904, -104.564031, -96.1997155, -88.0248379, -80.5747267, -73.3828348, -66.400063, -59.5773124, -52.5780585, -45.755598, -39.1758021, -32.9045422, -27.1020775, -21.7021359, -16.7328335, -12.2222863, -8.04341573, -4.44161024, -1.50706368, 0.670030087, 0.566059177, 0.0976149603, -0.252129215, 0., 3.38796234, 7.52383021, 12.0704624, 16.6907177, 19.6122814, 22.5072553, 25.6125674, 29.1651461, 31.5685348, 35.6264003, 42.3090248, 52.5866904, 52.2400158, 63.5048119, 93.4272263, 149.053406, 162.959, 246.448854, 436.357316, 769.518734, 1203.88868, 2012.93783, 3290.75124, 5281.02929, 8175.03416, 12224.1654, 17664.7881, 24733.2674}, 
{-219.890779, -215.461152, -210.294408, -204.496731, -198.174303, -191.433308, -184.37993, -177.120352, -169.760758, -162.766899, -155.741564, -148.647109, -141.445889, -133.988329, -126.393489, -118.6685, -110.820491, -102.586526, -94.3518247, -86.2315432, -78.3408367, -71.2359444, -64.414504, -57.8152372, -51.3768655, -44.7009486, -38.1982351, -31.9423113, -26.0067637, -20.5571553, -15.5383054, -10.9870099, -6.94006466, -3.32241444, -0.327446578, 1.96330266, 3.46829701, 2.81815514, 1.73432386, 0.650404896, 0., 1.99854432, 4.58507283, 7.48045392, 10.405556, 11.7205528, 13.0512852, 14.6628993, 16.8205414, 18.073119, 21.0885124, 26.8183632, 36.2143128, 34.727059, 45.009565, 74.2138497, 129.491933, 145.909806, 231.539927, 422.368724, 754.382627, 1190.66081, 1985.91147, 3228.78282, 5147.23075, 7953.77503, 11783.3722, 16790.3694, 23129.1142}, 
{-226.724899, -212.764992, -200.613432, -190.033165, -180.787138, -172.638298, -165.34959, -158.683961, -152.404358, -145.81283, -139.317579, -132.865911, -126.40513, -119.900384, -113.273999, -106.466146, -99.4169927, -91.5262098, -83.4906648, -75.4667261, -67.6107623, -60.4573783, -53.6334118, -47.1439369, -40.9940276, -35.2433287, -29.8205152, -24.7088327, -19.8915271, -15.2693682, -10.9410679, -6.92286233, -3.23098758, 0.61081133, 3.89681083, 6.41377843, 7.9484816, 6.40498557, 4.20584103, 1.89089637, 0., 1.36847131, 3.32249934, 5.4837441, 7.4738656, 7.10299269, 6.52892902, 6.09794707, 6.15631933, 4.78894311, 5.50801613, 9.56436095, 18.2088001, 19.6826667, 33.4500685, 65.965624, 123.683951, 138.560516, 221.348751, 408.302935, 735.677349, 1179.71831, 1956.70398, 3138.69873, 4942.56472, 7595.34243, 11138.5612, 15660.1283, 21247.9508}, 
{-236.84171, -211.295632, -190.740811, -174.495373, -161.877439, -152.205134, -144.796581, -138.969903, -134.043225, -127.670992, -121.500477, -115.515274, -109.698978, -104.443078, -99.1601186, -93.6705368, -87.7947709, -80.3881138, -72.6222059, -64.7035429, -56.8386203, -49.4919717, -42.5088397, -35.9925048, -30.0462478, -25.3821555, -21.2511794, -17.5130774, -14.0276074, -10.327812, -6.73085042, -3.2271669, 0.192794552, 4.54191817, 8.42510028, 11.4505655, 13.2265385, 10.5952775, 7.03736001, 3.26739713, 0., 1.10351971, 2.87733143, 4.77455029, 6.24829142, 4.25773493, 1.74750499, -0.831709239, -3.02921862, -7.34619724, -9.19934742, -6.95723472, 1.01157529, 6.82986723, 25.4411853, 62.2804239, 122.782477, 135.652615, 213.747207, 393.192996, 710.116727, 1156.14026, 1900.90499, 2996.52076, 4644.78844, 7069.78957, 10256.7919, 14250.7667, 19096.6851}, 
{-232.280967, -202.805662, -179.493136, -161.488734, -147.937801, -137.985681, -130.77772, -125.459263, -121.175653, -114.86966, -108.770235, -102.903752, -97.2965875, -92.6305548, -88.0144139, -83.2123638, -77.9886036, -70.9785561, -63.5267075, -55.8487674, -48.1604456, -40.8346679, -33.8670415, -27.4103897, -21.6175357, -17.5081853, -14.0215263, -10.963629, -8.14056397, -4.8928611, -1.67834712, 1.51069153, 4.68196843, 9.02829042, 12.8982405, 15.8254948, 17.3437298, 13.841766, 9.25607751, 4.37828269, 0., 0.430831866, 1.53721913, 2.70358662, 3.31435919, -0.0847916974, -4.13518649, -8.31689902, -12.1100031, -18.0667268, -21.3661281, -20.2594189, -12.9978114, -5.65089228, 14.4758508, 52.2585556, 112.57336, 127.359326, 203.604497, 375.359842, 676.676327, 1108.31316, 1804.19659, 2804.49135, 4284.70754, 6449.15256, 9251.59936, 12708.4545, 16836.1246}, 
{-217.723575, -189.98152, -168.069873, -151.165799, -138.446465, -129.089038, -122.270683, -117.168567, -112.959856, -106.589909, -100.360423, -94.3412868, -88.602391, -83.9929754, -79.4918381, -74.8571281, -69.8469946, -63.1281994, -55.9868336, -48.6176009, -41.2152054, -34.0498118, -27.2104788, -20.8617257, -15.168072, -11.1893742, -7.83668007, -4.91637443, -2.23484203, 0.921668114, 4.01858121, 7.04145819, 9.97586002, 13.921929, 17.3048121, 19.6642379, 20.5399346, 16.3430655, 10.9933504, 5.28194367, 0., -0.573489477, -0.530341535, -0.4745363, -1.0100539, -5.63864982, -10.9074187, -16.2612305, -21.1449552, -27.7808742, -31.7254815, -31.3126825, -24.8763827, -18.1852808, 0.835428433, 36.825757, 94.4257167, 113.608134, 189.54708, 352.749442, 633.722106, 1037.49814, 1671.00588, 2568.45021, 3871.51835, 5752.88672, 8153.79254, 11073.3328, 14510.6046}, 
{-197.850438, -175.509646, -157.670492, -143.67912, -132.881674, -124.624299, -118.253141, -113.114343, -108.55405, -102.012813, -95.5046078, -89.1378169, -83.0208222, -78.0605024, -73.2473444, -68.3703316, -63.2184475, -56.6677061, -49.7852481, -42.7252446, -35.6418668, -28.701748, -22.0416126, -15.8106468, -10.1580371, -5.99367853, -2.40176533, 0.772799518, 3.68511306, 7.00195942, 10.1620738, 13.1158783, 15.8137953, 19.0748347, 21.6333959, 23.0924661, 23.0550322, 18.2977904, 12.3805353, 6.03677068, 0., -1.83334156, -3.15785451, -4.50520788, -6.40707065, -12.1179021, -18.3574648, -24.5683118, -30.1929959, -36.8968677, -41.0105633, -41.0875167, -35.6811617, -31.1989674, -15.1987182, 16.907766, 69.7086655, 94.3265236, 170.201412, 323.307766, 579.620021, 944.956329, 1505.75997, 2294.23711, 3414.41715, 5000.44737, 6994.1804, 9385.54253, 12164.4601}, 
{-177.34246, -162.076478, -149.494462, -139.181246, -130.721668, -123.700563, -117.702768, -112.313118, -107.11645, -100.319447, -93.4363589, -86.6032825, -79.9563146, -74.3632984, -68.935886, -63.5174757, -57.9514658, -51.4277386, -44.704615, -37.8868996, -31.0793971, -24.3548212, -17.8629043, -11.7212873, -6.04761157, -1.48905451, 2.57809353, 6.24800591, 9.61485598, 13.2341966, 16.5542697, 19.4846971, 21.9351002, 24.3390085, 25.8725726, 26.2358508, 25.1289017, 19.9045551, 13.5489893, 6.70115434, 0., -3.27262161, -6.17782377, -9.13381751, -12.5588139, -19.2366111, -26.2735981, -33.1417511, -39.3130462, -45.8229357, -49.9545293, -50.5544126, -46.4691713, -45.1176209, -33.3452256, -6.56967931, 39.791324, 69.4419812, 144.193951, 284.980784, 512.73603, 831.948853, 1312.88596, 1987.69175, 2922.60026, 4211.28985, 5803.57192, 7685.22462, 9842.0261}, 
{-160.880546, -152.368455, -144.741251, -137.82473, -131.444689, -125.426926, -119.597237, -113.78142, -107.805271, -100.690886, -93.3892444, -86.0476237, -78.8133015, -72.4315259, -66.2124162, -60.0640621, -53.8945533, -47.2389595, -40.5275983, -33.8177671, -27.1667634, -20.5733762, -14.1768151, -8.05778129, -2.29697582, 2.75654163, 7.39777206, 11.6533579, 15.5499414, 19.5045633, 22.9973084, 25.8986598, 28.0791008, 29.5664513, 30.0109229, 29.2200637, 27.0014222, 21.3619739, 14.6300691, 7.33348529, 0., -4.81522689, -9.42275327, -14.1057546, -19.1474064, -26.7088395, -34.4440918, -41.8851566, -48.5640273, -54.9673069, -59.290535, -60.683861, -58.2974344, -60.3669104, -53.3227302, -32.6808411, 6.04281005, 38.8819919, 110.151155, 235.714466, 431.436092, 699.736845, 1096.81095, 1654.65389, 2405.26399, 3404.86948, 4612.77606, 6012.52008, 7587.63786}, 
{-117.464876, -123.918365, -127.492752, -128.518553, -127.326281, -124.246452, -119.609581, -113.746181, -106.986767, -100.040866, -92.7083752, -85.1682045, -77.5992633, -70.6372629, -63.8215906, -57.1484355, -50.6139865, -44.2722108, -38.038408, -31.8856556, -25.7870314, -19.5202853, -13.3319539, -7.27324596, -1.3953703, 4.18790751, 9.51295815, 14.5535955, 19.2836336, 24.2243411, 28.5830951, 32.1147276, 34.5740706, 35.1779179, 34.4343549, 32.3134286, 28.7851863, 22.7771943, 15.718973, 7.99756168, 0., -6.61155838, -13.2352339, -19.9940327, -27.010961, -35.9903708, -44.8413841, -53.054469, -60.1200932, -64.6781795, -67.4099591, -68.1461179, -66.7173418, -74.5060328, -75.1704745, -63.9206668, -35.9666093, 0.717528466, 68.7915847, 178.151228, 338.692126, 545.47663, 852.90036, 1304.26458, 1880.44809, 2572.04849, 3393.21069, 4352.19278, 5457.25286}, 
{-136.527966, -139.159539, -139.607421, -138.121857, -134.953088, -130.351358, -124.566911, -117.849989, -110.450837, -102.93602, -95.112929, -87.1052792, -79.0367843, -71.261443, -63.5805711, -56.0257688, -48.6286365, -41.5501343, -34.6407587, -27.880366, -21.2488126, -14.5876696, -8.07039264, -1.7321522, 4.3918813, 10.3326244, 15.962385, 21.2195577, 26.0425374, 30.7830152, 34.8007711, 37.8688812, 39.7604217, 39.674461, 38.1876865, 35.3027776, 31.0224139, 24.5660901, 17.0329442, 8.73892973, 0., -8.04750415, -16.2361718, -24.5782045, -33.0858038, -42.6581978, -52.0657511, -60.9658549, -69.0159002, -75.50637, -80.6083267, -84.1259248, -85.8633187, -94.7693226, -97.845567, -91.2383424, -71.0939391, -40.7141918, 13.7723711, 99.0816769, 221.929653, 380.179382, 607.105325, 934.522007, 1336.34321, 1800.19873, 2316.75508, 2875.91963, 3467.59973}, 
{-194.664249, -181.230941, -169.302868, -158.648225, -149.035212, -140.232023, -132.006856, -124.127907, -116.363375, -107.812862, -99.1805959, -90.502209, -81.8133354, -73.2086007, -64.6410495, -56.1227188, -47.6656453, -39.1592333, -30.7872051, -22.6106505, -14.6906592, -7.17655082, -0.00589349249, 6.79551487, 13.2018764, 19.3079585, 24.9191718, 29.9614923, 34.3608958, 38.1894679, 41.1686312, 43.1659177, 44.0488595, 43.4935761, 41.6355773, 38.4189602, 33.7878219, 26.7947351, 18.6319311, 9.6001171, 0., -9.24386883, -18.7041645, -28.3297179, -38.0693597, -47.9114983, -57.7495558, -67.516532, -77.1454268, -88.0404396, -98.0748906, -106.593299, -112.940186, -120.019097, -122.191915, -117.379548, -103.502907, -87.2876613, -54.3280535, 0.976913021, 84.2282345, 202.753802, 356.973927, 552.751261, 786.612743, 1083.52895, 1364.03162, 1574.7622, 1662.36213}, 
{-175.36164, -166.624748, -158.159801, -149.925768, -141.881618, -133.986319, -126.198841, -118.478152, -110.783222, -103.030633, -95.2386948, -87.3833331, -79.440472, -71.2174429, -62.9261997, -54.6101037, -46.3125163, -38.121428, -30.017719, -22.0268988, -14.1744769, -6.30376836, 1.30464509, 8.55237609, 15.3410373, 21.2894899, 26.6951987, 31.5728771, 35.9372383, 40.4691856, 44.2507664, 47.0302182, 48.5557784, 48.0057096, 45.9262139, 42.2935185, 37.0838506, 29.5326914, 20.6533127, 10.7182403, 0., -10.8380537, -22.0705555, -33.5813111, -45.2541263, -57.0379543, -68.7253946, -80.1741938, -91.2420989, -101.801961, -111.69038, -120.759063, -128.859713, -138.865711, -146.398417, -150.100866, -148.616093, -144.359406, -130.692659, -104.749978, -63.6654913, -2.1174713, 75.3923952, 179.345261, 277.973065, 352.592929, 374.43045, 317.234105, 154.752373}, 
{-191.788584, -182.390037, -173.740918, -165.639879, -157.885566, -150.276631, -142.611723, -134.68949, -126.308584, -116.144987, -105.569081, -94.8285798, -84.1712001, -74.6575672, -65.3973217, -56.3130147, -47.3271972, -37.9729249, -28.7180425, -19.6408993, -10.8198446, -2.39999017, 5.63378214, 13.2298281, 20.3365033, 26.8791912, 32.8384087, 38.1717007, 42.8366119, 47.2220462, 50.6816452, 53.00041, 53.9633413, 52.8498506, 50.1527642, 45.859319, 39.9567519, 31.9102235, 22.4378773, 11.7357803, 0., -12.5765892, -25.7934503, -39.453239, -53.3586108, -66.8576235, -80.38937, -93.9383452, -107.489044, -121.262378, -134.911858, -148.327413, -161.398972, -174.938597, -187.545228, -198.739941, -208.043809, -213.510736, -216.715838, -217.767058, -216.77234, -208.338342, -209.076867, -219.416934, -264.909434, -355.793203, -538.433422, -850.163683, -1328.31758}, 
{-198.71236, -197.308699, -193.101836, -186.510063, -177.951671, -167.844951, -156.608195, -144.659693, -132.417739, -121.285996, -110.303233, -99.4935919, -88.8812147, -78.707498, -68.6924271, -58.7732421, -48.8871829, -38.459504, -28.1442253, -18.0833812, -8.41900594, 0.229792022, 8.38888204, 16.1070594, 23.4331194, 30.88596, 37.8562326, 44.2046916, 49.7920911, 54.7738453, 58.5981843, 61.0079983, 61.7461775, 59.6423142, 55.7179154, 50.0811906, 42.840349, 34.2000096, 24.1334081, 12.7101898, 0., -14.5670711, -30.0260009, -46.0513217, -62.3175661, -77.5333094, -92.7254243, -107.954826, -123.28243, -138.151262, -153.487283, -169.598565, -186.793178, -205.872273, -226.45361, -248.648032, -272.566377, -292.582224, -316.83858, -347.741193, -387.695808, -434.404278, -504.384028, -598.974796, -746.35905, -974.171705, -1285.39517, -1689.17496, -2194.65663}
};

  const double a5tab[50][69] = {
{4296.48547, 4145.14906, 4028.39347, 3938.25617, 3866.77463, 3805.98633, 3747.92872, 3684.63929, 3608.15549, 3470.8602, 3320.30733, 3164.39619, 3011.02609, 2895.53131, 2787.40219, 2683.56406, 2580.94224, 2467.08826, 2352.05074, 2236.50453, 2121.12447, 2008.42576, 1896.50673, 1785.30605, 1674.76241, 1564.89358, 1455.52754, 1346.57134, 1237.93205, 1128.57981, 1019.73336, 911.67453, 804.685139, 699.870801, 596.360042, 494.10517, 393.058497, 293.389367, 194.746242, 96.9946212, 0., -95.8821865, -191.472354, -287.100981, -383.098545, -476.415091, -572.113704, -671.877035, -777.387735, -883.098866, -1000.81451, -1135.10914, -1290.55726, -1445.5122, -1641.25806, -1892.85779, -2215.37433, -2550.46591, -3015.96209, -3656.28769, -4515.86756, -5562.5241, -7070.48939, -8999.14401, -12015.737, -15367.8837, -23273.8659, -38709.2987, -64649.7973}, 
{3989.50138, 3890.74144, 3791.28858, 3691.26149, 3590.77885, 3489.95935, 3388.92168, 3287.78452, 3186.66657, 3088.65256, 2989.7087, 2888.76729, 2784.76057, 2664.7319, 2544.25806, 2427.02688, 2316.72622, 2240.60956, 2169.37286, 2097.27769, 2018.58566, 1902.04814, 1777.64101, 1649.82996, 1523.08064, 1415.81637, 1312.96214, 1213.40058, 1116.01432, 1016.79356, 918.670313, 821.68419, 725.874783, 631.536102, 538.351566, 446.259007, 355.196258, 265.266634, 176.17629, 87.7968658, 0., -86.9254916, -173.692018, -260.594811, -347.929102, -432.365887, -519.274328, -610.399354, -707.48589, -805.717825, -916.025541, -1042.77838, -1190.34568, -1339.01104, -1526.86385, -1767.90776, -2076.14641, -2395.68728, -2838.38865, -3446.21264, -4261.12134, -5252.34273, -6680.04137, -8501.55957, -11361.8428, -14541.9955, -22090.8829, -36865.43, -61722.5616}, 
{3757.56237, 3688.98233, 3602.74083, 3502.53272, 3392.05283, 3274.99599, 3155.05703, 3035.9308, 2921.31211, 2833.32287, 2749.86003, 2667.24761, 2581.80961, 2470.83397, 2357.29521, 2245.13178, 2138.28212, 2062.17793, 1990.6671, 1919.09078, 1842.79012, 1736.93836, 1625.1117, 1510.71846, 1397.16693, 1298.16665, 1202.70421, 1110.06741, 1019.54408, 928.906343, 839.563964, 751.411028, 664.341619, 578.018704, 492.659931, 408.25183, 324.780933, 242.505048, 161.030918, 80.2365616, 0., -79.1939341, -158.316759, -237.733181, -317.807904, -395.541068, -476.007772, -560.918545, -651.983919, -744.762196, -849.577029, -970.599841, -1112.00206, -1254.41911, -1434.97281, -1667.24897, -1964.8334, -2274.39487, -2703.20307, -3291.61063, -4079.9702, -5039.50056, -6417.95592, -8173.42849, -10924.8947, -14014.4248, -21230.8298, -35251.2358, -58752.7686}, 
{3573.43954, 3518.13048, 3441.64113, 3347.99242, 3241.20528, 3125.30065, 3004.29946, 2882.22265, 2763.09114, 2665.78538, 2573.523, 2484.38113, 2396.4369, 2303.02573, 2208.86315, 2113.92298, 2018.17905, 1918.81705, 1819.71417, 1721.95946, 1626.642, 1540.32011, 1456.42589, 1373.86068, 1291.52585, 1204.16451, 1116.49954, 1029.09559, 942.517315, 859.174238, 777.048176, 695.965823, 615.753875, 535.637321, 456.285245, 377.765026, 300.144041, 223.94418, 148.596504, 73.9865859, 0., -72.5271837, -145.040088, -218.033339, -292.001563, -364.645482, -440.371185, -520.79086, -607.516694, -696.21608, -796.823916, -913.330305, -1049.72535, -1186.00008, -1359.74331, -1584.54477, -1873.9942, -2177.10138, -2597.86799, -3175.71572, -3950.0663, -4894.3482, -6245.96286, -7966.33323, -10645.6449, -13703.476, -20613.072, -33834.2617, -55826.8739}, 
{3409.90395, 3356.44466, 3286.88038, 3203.56312, 3108.8449, 3005.07774, 2894.61363, 2779.80461, 2663.00268, 2546.95431, 2433.45928, 2324.71182, 2222.90615, 2150.49542, 2081.31138, 2009.44471, 1928.98609, 1797.5506, 1660.29474, 1525.89938, 1403.04545, 1339.4171, 1289.09066, 1245.14575, 1200.66196, 1126.02999, 1046.0939, 963.008853, 878.930002, 801.853306, 725.756809, 650.459352, 575.779776, 500.710664, 426.227618, 352.479982, 279.6171, 208.423598, 138.159426, 68.7198163, 0., -66.7649103, -133.555516, -201.012536, -269.776693, -338.38397, -410.421719, -487.372554, -570.71909, -656.063581, -753.121147, -865.726549, -997.714547, -1128.01761, -1295.3337, -1513.4585, -1796.18769, -2094.31954, -2509.84608, -3081.76197, -3849.06182, -4787.23624, -6125.79199, -7831.85622, -10464.8454, -13527.4537, -20156.9751, -32582.0535, -53031.3329}, 
{3239.72667, 3182.18361, 3117.34947, 3045.16737, 2965.58039, 2878.53165, 2783.96423, 2681.82123, 2572.04576, 2437.74389, 2302.43055, 2172.78365, 2055.48109, 2002.43127, 1956.9894, 1907.74119, 1843.27233, 1685.40227, 1516.18949, 1350.92621, 1204.90465, 1161.45301, 1140.61312, 1130.46279, 1119.07984, 1055.98318, 983.233088, 904.330944, 822.778121, 751.19961, 680.323725, 610.002396, 540.087553, 469.557447, 399.487158, 330.078085, 261.53163, 194.782874, 129.006064, 64.1091301, 0., -61.7467835, -123.556551, -186.188021, -250.399908, -315.46138, -384.216524, -458.019878, -538.225981, -620.2888, -713.823669, -822.545353, -950.168616, -1074.73535, -1235.90234, -1447.6535, -1723.97275, -2016.56203, -2426.60002, -2992.98342, -3754.60894, -4688.5153, -6019.17312, -7721.57993, -10323.2482, -13404.6625, -19781.9045, -31462.1566, -50452.6013}, 
{3035.67878, 2973.60609, 2911.93932, 2848.72771, 2782.02045, 2709.86678, 2630.31591, 2541.41704, 2441.2194, 2299.06835, 2153.19849, 2013.14059, 1888.4254, 1848.02151, 1818.24671, 1784.85662, 1733.60687, 1569.39574, 1391.17913, 1217.05561, 1065.12374, 1033.65155, 1028.50034, 1035.70092, 1041.28404, 986.24413, 919.662878, 845.585609, 768.05765, 701.469213, 635.382783, 569.705732, 504.34543, 438.496382, 373.063973, 308.240723, 244.21915, 181.861577, 120.422796, 59.8274049, 0., -57.3124734, -114.736706, -173.077041, -233.137821, -294.582554, -359.81275, -430.089086, -506.672238, -584.87584, -674.286427, -778.543494, -901.286533, -1020.41696, -1175.60758, -1380.79311, -1649.90829, -1934.34159, -2335.59247, -2892.61414, -3644.35983, -4568.53596, -5887.83607, -7587.08678, -10161.6055, -13253.4069, -19407.2257, -30442.1168, -48177.1347}, 
{2770.53136, 2708.97085, 2649.54083, 2590.16668, 2528.77379, 2463.28755, 2391.63335, 2311.73658, 2221.52263, 2091.84191, 1958.52477, 1830.32661, 1716.0028, 1676.45437, 1647.43281, 1616.83524, 1572.5588, 1436.55469, 1289.04433, 1144.30323, 1016.60689, 983.236401, 970.259435, 966.74926, 961.779143, 909.032918, 847.129054, 779.296591, 708.764568, 646.91818, 585.567846, 524.68014, 464.221637, 403.846181, 343.958173, 284.649282, 226.011177, 168.499276, 111.696, 55.5475182, 0., -53.3016495, -106.789491, -161.196848, -217.257045, -274.452337, -335.267548, -400.93643, -472.692737, -545.808803, -629.864368, -728.477751, -845.267274, -959.326113, -1108.60779, -1306.54069, -1566.55318, -1838.17091, -2224.28609, -2763.88818, -3495.96666, -4397.64884, -5693.51063, -7379.95924, -9920.66925, -12991.9915, -18952.3044, -29489.4796, -46291.3886}, 
{2417.05548, 2366.53666, 2309.0449, 2245.40682, 2176.44909, 2102.99835, 2025.88123, 1945.9244, 1863.95449, 1776.9788, 1691.17107, 1608.88567, 1532.477, 1476.91808, 1426.8972, 1379.72126, 1332.69721, 1273.9028, 1213.56578, 1152.68474, 1092.25826, 1037.43127, 983.397481, 929.496943, 875.06971, 816.569606, 757.3774, 697.987635, 638.894853, 581.802574, 525.512772, 470.036397, 415.384401, 361.925557, 309.169865, 256.985149, 205.239231, 153.535543, 102.112055, 50.9423473, 0., -49.5539819, -99.4084146, -150.064689, -202.024195, -253.775575, -308.638068, -367.918164, -432.922351, -499.071792, -575.912436, -667.104903, -776.309814, -885.726459, -1029.06132, -1218.55955, -1466.46631, -1718.56271, -2080.14355, -2590.0396, -3287.08162, -4146.20453, -5397.92663, -7051.77975, -9541.19154, -12538.7207, -18336.5059, -28571.7906, -44881.8187}, 
{2185.86243, 2129.66864, 2072.54176, 2014.18321, 1954.2944, 1892.57674, 1828.73164, 1762.46052, 1693.46479, 1613.81027, 1533.88821, 1456.45424, 1384.26402, 1331.09789, 1284.27693, 1242.1469, 1203.05357, 1164.97587, 1126.77315, 1086.93792, 1043.96269, 989.074843, 930.938071, 870.950935, 810.511996, 754.725473, 699.802008, 645.657899, 592.209444, 538.934767, 486.363612, 434.587547, 383.69814, 334.548323, 286.163756, 238.331466, 190.838476, 142.966997, 95.210792, 47.5588098, 0., -46.4100171, -93.1756315, -140.734562, -189.524529, -237.936684, -289.27394, -344.79264, -405.74913, -467.474595, -539.520605, -625.513568, -729.079894, -833.298458, -970.562217, -1152.71659, -1391.60701, -1630.84359, -1977.80117, -2471.61931, -3151.43754, -3991.40953, -5225.63246, -6875.89452, -9345.32147, -12339.851, -17972.8354, -27755.7736, -43200.1643}, 
{1981.4486, 1917.74747, 1860.59169, 1808.1705, 1758.6731, 1710.28871, 1661.20656, 1609.61586, 1553.70582, 1477.58662, 1399.15813, 1322.2412, 1250.65666, 1201.62972, 1160.21509, 1124.87184, 1094.05904, 1069.0927, 1044.43221, 1017.39389, 985.294068, 932.67365, 874.734515, 813.90314, 752.606002, 700.319407, 649.600069, 600.054533, 551.289343, 501.502803, 452.272994, 403.769755, 356.162928, 310.714537, 266.065364, 221.948375, 178.096535, 133.578698, 89.0575891, 44.5318191, 0., -43.5656426, -87.5295569, -132.282577, -178.215537, -223.723063, -271.99068, -324.207705, -381.563456, -439.300701, -506.933925, -588.031063, -686.16005, -784.990737, -915.94838, -1090.56015, -1320.35322, -1546.9207, -1879.69744, -2358.18424, -3021.88189, -3844.23415, -5062.91291, -6711.52012, -9164.32682, -12162.1323, -17638.9538, -26980.504, -41572.4958}, 
{1794.4152, 1724.40808, 1667.3188, 1620.06974, 1579.58327, 1542.78174, 1506.58753, 1467.923, 1423.71051, 1351.59344, 1275.48475, 1200.01839, 1129.82833, 1084.63602, 1047.95293, 1018.37801, 994.510209, 980.512175, 967.19371, 950.928297, 928.089424, 877.880279, 820.71276, 759.828472, 698.469016, 649.593536, 602.839079, 557.560233, 513.111583, 466.653192, 420.611983, 375.220355, 330.710706, 288.654201, 247.408964, 206.671883, 166.13985, 124.746758, 83.2576926, 41.6747436, 0., -40.8980775, -82.2303199, -124.341187, -167.575137, -210.340302, -255.692002, -304.749227, -358.630969, -412.54525, -475.886418, -552.13785, -644.782925, -738.021909, -862.334537, -1028.91743, -1248.96722, -1462.16565, -1779.83017, -2241.76334, -2887.76773, -3690.53634, -4891.20049, -6534.3606, -8967.55178, -11963.4481, -17291.233, -26220.9625, -40022.6926}, 
{1628.55148, 1554.65838, 1498.11445, 1454.99224, 1421.36432, 1393.30328, 1366.88168, 1338.17208, 1303.24707, 1235.66334, 1163.01568, 1090.38301, 1022.84422, 981.368108, 948.787774, 923.826191, 905.206329, 898.844712, 893.393335, 884.697746, 868.603494, 821.350866, 766.232776, 706.936878, 647.150825, 601.764906, 558.783084, 517.41196, 476.858131, 433.670357, 390.776215, 348.44544, 306.947768, 268.010996, 229.863574, 192.192013, 154.682824, 116.244139, 77.6521993, 38.9048655, 0., -38.3123138, -77.0873241, -116.628058, -157.237541, -197.364371, -239.907775, -285.912554, -336.423505, -386.690844, -445.871786, -517.328963, -604.425008, -691.796217, -809.022092, -966.955801, -1176.45051, -1375.54392, -1677.03085, -2120.89065, -2747.10266, -3527.644, -4706.50075, -6337.96158, -8745.4887, -11730.9564, -16913.4172, -25457.6291, -38528.3499}, 
{1487.64668, 1413.5063, 1358.36997, 1318.04925, 1288.35567, 1265.10079, 1244.09613, 1221.15325, 1192.0837, 1129.6289, 1061.89856, 993.932245, 930.769552, 893.077305, 864.016948, 842.37717, 826.946657, 823.700716, 821.366769, 815.858856, 803.091017, 759.741547, 708.65453, 653.438306, 597.701215, 556.050561, 516.696131, 478.846677, 441.710953, 401.838723, 362.161324, 322.951103, 284.480408, 248.428604, 213.098217, 178.198789, 143.439861, 107.843807, 72.0822061, 36.1394671, 0., -35.7133438, -71.9099732, -108.860855, -146.836957, -184.371235, -224.167871, -267.193037, -314.412902, -361.220084, -416.38373, -483.099433, -564.562788, -645.717902, -755.312447, -903.842611, -1101.80458, -1286.02099, -1570.131, -1994.10022, -2597.89427, -3352.88507, -4504.81926, -6115.86863, -8488.62993, -11451.815, -16489.2504, -24670.9838, -37067.063}, 
{1375.49005, 1305.95973, 1253.47674, 1214.35207, 1184.89673, 1161.42172, 1140.23803, 1117.65665, 1089.9886, 1033.32271, 972.280995, 911.263313, 854.669523, 821.014935, 794.937775, 775.19172, 760.530448, 754.690595, 749.449696, 741.568244, 727.806733, 689.708458, 645.337991, 597.542705, 549.16997, 511.667545, 475.842264, 441.101348, 406.852016, 370.442713, 334.162944, 298.243438, 262.914923, 229.550707, 196.781911, 164.382231, 132.125366, 99.3187258, 66.3888102, 33.2958309, 0., -33.0061597, -66.5076708, -100.757245, -136.007596, -170.936864, -208.00216, -248.086024, -292.070995, -335.615571, -386.915951, -448.944294, -524.672755, -599.191206, -700.507005, -838.745224, -1024.03094, -1192.56234, -1457.96212, -1859.92612, -2438.15015, -3163.58747, -4282.16159, -5861.62737, -8187.4678, -11113.1819, -16002.4765, -23841.5066, -35616.4271}, 
{1295.87085, 1237.02659, 1188.82609, 1149.01198, 1115.32691, 1085.51353, 1057.31449, 1028.47242, 996.729976, 946.577353, 894.310623, 842.97341, 795.609339, 766.432322, 742.847581, 723.430622, 706.756955, 691.424756, 675.977799, 658.982526, 639.005378, 607.907736, 573.643128, 537.46002, 500.606877, 467.832902, 435.48553, 403.412934, 371.463285, 338.766752, 306.176712, 273.828538, 241.857605, 211.020988, 180.583677, 150.432364, 120.453741, 90.4418588, 60.4131083, 30.291239, 0., -30.0957535, -60.6898206, -92.0348943, -124.383668, -156.637225, -190.940513, -228.086866, -268.869621, -309.359907, -356.962153, -414.358576, -484.231398, -551.62037, -643.907167, -770.830997, -942.131067, -1094.13344, -1339.35572, -1716.90239, -2265.87789, -2957.07913, -4034.53329, -5568.78338, -7832.49467, -10702.2149, -15436.8397, -22949.6777, -34154.0375}, 
{1204.65587, 1157.00006, 1114.47906, 1076.03051, 1040.59206, 1007.10134, 974.496, 941.713669, 907.69199, 863.90708, 819.742711, 777.121132, 737.964591, 712.778866, 691.469265, 672.524625, 654.433783, 631.935693, 608.769028, 584.922579, 560.385137, 535.126853, 509.162613, 482.488661, 455.101244, 426.520612, 397.409404, 367.954264, 338.341835, 309.081426, 279.907949, 250.878982, 222.052103, 193.704483, 165.586269, 137.667202, 109.917023, 82.4541578, 55.0401876, 27.5853794, 0., -27.3906496, -55.2573507, -83.8558505, -113.441896, -143.126009, -174.767252, -209.079461, -246.776473, -284.355627, -328.433857, -381.411599, -445.689291, -506.351102, -590.040242, -706.083651, -863.808272, -999.956678, -1225.47393, -1578.72072, -2098.05773, -2753.3297, -3788.44518, -5278.67022, -7480.52598, -10279.4618, -14863.9593, -22096.742, -32840.5336}, 
{1074.63988, 1033.0459, 996.321555, 963.439721, 933.373293, 905.095166, 877.57823, 849.795378, 820.719503, 782.391091, 743.488403, 705.757293, 670.943616, 648.479622, 629.350211, 612.226682, 595.780332, 575.305119, 554.200613, 532.489048, 510.192655, 487.464298, 464.143325, 440.199715, 415.603449, 389.833046, 363.546529, 336.910461, 310.091403, 283.54333, 257.03043, 230.6043, 204.316538, 178.421378, 152.686729, 127.083133, 101.581135, 76.2399296, 50.9059493, 25.5142782, 0., -25.256491, -50.9435458, -77.3042049, -104.581509, -131.993253, -161.217822, -192.908352, -227.717982, -262.453961, -303.153673, -352.008612, -411.210273, -466.199332, -542.618428, -649.359384, -795.314019, -917.313637, -1125.13479, -1456.4935, -1949.1058, -2570.59931, -3568.95528, -5028.68341, -7183.5431, -9910.74615, -14364.4451, -21379.5571, -31790.9996}, 
{952.045299, 916.813429, 886.000482, 858.645986, 833.789466, 810.47045, 787.728463, 764.603033, 740.133686, 707.0761, 673.26719, 640.260019, 609.607655, 589.582905, 572.331196, 556.717696, 541.607574, 522.902498, 503.616539, 483.800265, 463.504248, 443.03075, 422.07797, 400.595797, 378.534124, 355.357107, 331.694667, 307.69099, 283.49026, 259.485015, 235.471748, 211.495303, 187.600525, 164.018334, 140.533068, 117.115138, 93.7349577, 70.3914253, 47.0150752, 23.564927, 0., -23.2517332, -46.8877878, -71.1367257, -96.2271088, -121.468367, -148.375847, -177.545764, -209.574332, -241.585969, -279.037406, -323.913578, -378.199419, -427.713909, -497.074318, -594.73196, -729.138153, -837.32558, -1027.73164, -1337.3751, -1803.27474, -2390.79442, -3351.91758, -4779.57717, -6886.99293, -9541.19849, -13873.6851, -20699.8296, -30835.0089}, 
{837.812697, 808.936927, 783.919108, 761.884766, 741.959432, 723.268633, 704.937899, 686.092758, 665.858739, 637.79203, 608.815237, 580.281624, 553.544456, 535.682276, 520.03296, 505.659659, 491.625529, 474.469091, 456.787981, 438.655203, 420.143763, 401.668489, 382.823831, 363.546065, 343.771465, 322.977172, 301.742249, 280.186624, 258.430227, 236.798139, 215.123074, 193.4429, 171.795483, 150.388204, 129.021612, 107.665771, 86.2907419, 64.8428209, 43.325344, 21.7178806, 0., -21.3729548, -43.0874978, -65.3543693, -88.3843094, -111.556974, -136.246622, -162.996426, -192.349561, -221.745105, -256.071965, -297.114955, -346.658886, -390.927315, -453.490813, -542.358694, -665.540274, -760.3361, -933.747761, -1222.06808, -1661.58988, -2215.46107, -3139.40919, -4533.64928, -6593.803, -9176.58984, -13398.9314, -20062.6987, -29969.7628}, 
{732.882625, 710.050697, 690.4807, 673.391519, 658.002041, 643.53115, 629.197732, 614.220674, 597.81886, 574.368803, 549.868711, 525.47442, 502.341765, 486.371293, 472.076243, 458.714566, 445.544212, 429.746158, 413.486118, 396.852833, 379.935045, 363.21979, 346.238194, 328.919679, 311.19367, 292.577619, 273.577705, 254.288137, 234.803126, 215.37436, 195.875577, 176.337996, 156.792834, 137.423841, 118.04869, 98.6375877, 79.1607395, 59.5282925, 39.7945341, 19.9536936, 0., -19.6167344, -39.5400971, -59.9580924, -81.0587245, -102.2647, -124.83544, -149.265068, -176.047706, -202.924822, -234.244259, -271.6012, -316.590832, -355.872032, -411.950817, -492.396897, -604.779981, -686.688787, -843.66641, -1111.27496, -1525.07654, -2046.14531, -2933.5072, -4293.19749, -6306.90082, -8822.69125, -12947.4358, -19473.3033, -29192.4625}, 
{638.19564, 620.789034, 606.088527, 593.401702, 582.036144, 571.299435, 560.499161, 548.942905, 535.938251, 516.636341, 496.163778, 475.490721, 455.58733, 441.243514, 428.081786, 415.544409, 403.073644, 388.474958, 373.482125, 358.192127, 342.701942, 327.526931, 312.178341, 296.585799, 280.678933, 264.042825, 247.089466, 229.886301, 212.500777, 195.105336, 177.620429, 160.071501, 142.483999, 125.018098, 107.510627, 89.933143, 72.2572021, 54.3820161, 36.3804242, 18.2529207, 0., -17.9796505, -36.2430068, -54.9488516, -74.2559679, -93.5971696, -114.147596, -136.356417, -160.672804, -185.118576, -213.541195, -247.360771, -287.997414, -322.580542, -372.537235, -445.003884, -547.116876, -616.727231, -757.970853, -1005.69828, -1394.76004, -1884.39319, -2736.28873, -4060.51959, -6029.21391, -8485.27376, -12526.4503, -18936.7826, -28500.3094}, 
{554.692301, 541.786237, 531.145858, 522.150773, 514.180592, 506.614925, 498.833382, 490.215574, 480.14111, 464.424568, 447.436604, 429.98284, 412.8689, 399.892497, 387.670329, 375.811181, 363.923841, 350.396751, 336.547181, 322.472055, 308.268298, 294.432189, 280.501557, 266.413583, 252.105452, 237.257168, 222.165962, 206.871889, 191.415003, 175.882728, 160.248799, 144.53432, 128.760398, 113.063829, 97.3037508, 81.4549918, 65.4923811, 49.3381679, 33.0407928, 16.5961166, 0., -16.4582818, -33.193648, -50.3276035, -67.9816533, -85.560007, -104.188383, -124.275204, -146.228893, -168.319821, -193.949684, -224.382126, -260.880791, -291.085326, -335.332971, -400.336967, -492.810558, -550.795024, -677.144355, -906.040578, -1271.66572, -1731.75074, -2549.83088, -3837.91334, -5763.66979, -8170.1084, -12143.2269, -18458.2756, -27890.5049}, 
{483.313163, 473.676603, 466.055961, 459.874189, 454.554236, 449.519054, 444.191593, 437.994804, 430.351638, 417.563406, 403.423355, 388.60309, 373.77422, 361.911802, 350.462611, 339.176876, 327.804822, 315.252799, 302.452463, 289.491591, 276.457961, 263.777843, 251.065126, 238.272192, 225.351423, 212.105025, 198.695625, 185.135674, 171.437623, 157.598192, 143.651858, 129.617363, 115.513453, 101.453888, 87.3243877, 73.1056889, 58.7785281, 44.3309239, 29.7334185, 14.9638358, 0., -15.0492067, -30.3894418, -46.0953047, -62.2413945, -78.1588373, -94.9630951, -113.026156, -132.72001, -152.52201, -175.456634, -202.653723, -235.243119, -261.418867, -300.420927, -358.55346, -442.120629, -489.235757, -601.67018, -813.004394, -1156.81889, -1589.764, -2376.21074, -3627.6765, -5513.19596, -7882.96623, -11805.0174, -18042.9214, -27360.2502}, 
{430.484418, 421.113456, 414.158086, 408.98146, 404.94673, 401.417046, 397.755561, 393.325426, 387.489794, 376.483753, 364.049742, 350.802139, 337.355319, 326.256844, 315.414632, 304.669786, 293.863408, 282.296992, 270.567093, 258.73066, 246.844638, 235.147337, 223.441797, 211.712417, 199.9436, 188.128389, 176.239085, 164.256634, 152.161977, 139.83341, 127.395587, 114.870511, 102.280187, 89.7188522, 77.1073842, 64.4388941, 51.7064931, 39.0573716, 26.2689294, 13.2726458, 0., -13.7764105, -27.9026719, -42.3837523, -57.2246196, -71.5863824, -86.6554121, -102.77422, -120.285319, -137.798375, -158.081883, -182.171493, -211.102857, -233.791427, -268.24113, -320.335691, -395.958842, -432.859693, -532.310436, -727.448645, -1051.41189, -1460.81344, -2218.36381, -3429.83, -5277.77804, -7636.36413, -11536.5847, -17720.2261, -26929.0746}, 
{392.308466, 381.186439, 373.266059, 367.790599, 364.003329, 361.147518, 358.466437, 355.203357, 350.601548, 340.48549, 328.88476, 316.410145, 303.672432, 293.084004, 282.733413, 272.510809, 262.30634, 251.700434, 241.016849, 230.269621, 219.472788, 208.611231, 197.739805, 186.884208, 176.07014, 165.51361, 154.973883, 144.400534, 133.743138, 122.753431, 111.657963, 100.485445, 89.2645885, 78.0710907, 66.8678814, 55.664877, 44.4719937, 33.6607917, 22.7348862, 11.559536, 0., -12.6123, -25.6647331, -39.0785057, -52.774824, -65.6832532, -79.1132972, -93.3828187, -108.809681, -124.072008, -141.783297, -162.917303, -188.447785, -208.078692, -238.561512, -285.377927, -354.009618, -381.598833, -469.302458, -649.937946, -956.32275, -1345.70917, -2077.61012, -3248.67972, -5062.9382, -7430.12351, -11329.7366, -17473.8377, -26574.487}, 
{360.66773, 347.89344, 338.935119, 332.947164, 329.083975, 326.499951, 324.349491, 321.786994, 317.966858, 308.406182, 297.351585, 285.41239, 273.197914, 263.040569, 253.137348, 243.408333, 233.77361, 224.009152, 214.236795, 204.434269, 194.579301, 184.439194, 174.28627, 164.182425, 154.189557, 144.710428, 135.329724, 125.972995, 116.565793, 106.761765, 96.8671266, 86.916191, 76.9432708, 67.0016617, 57.0990999, 47.2623045, 37.5179949, 28.466852, 19.3320488, 9.91071997, 0., -11.5081985, -23.54943, -35.9644712, -48.5940986, -60.1461305, -72.0474852, -84.5921225, -98.0740026, -111.204635, -126.493411, -144.867268, -167.253148, -184.019583, -210.875282, -252.97055, -315.455689, -335.026177, -412.669073, -580.916609, -872.30102, -1244.6194, -1954.6094, -3090.28309, -4876.46181, -7258.87633, -11162.8104, -17268.6597, -26256.8197}, 
{332.848826, 319.339406, 309.84285, 303.496527, 299.437804, 296.804049, 294.732628, 292.360909, 288.82626, 279.6239, 268.990204, 257.519397, 245.805708, 236.102767, 226.681634, 217.472775, 208.406655, 199.343792, 190.312576, 181.27145, 172.178856, 162.676833, 153.16679, 143.733731, 134.462662, 125.865105, 117.42894, 109.068565, 100.698377, 91.9270899, 83.0970604, 74.2449607, 65.4074631, 56.6069349, 47.9000748, 39.3292765, 30.936934, 23.542257, 16.1000972, 8.34212235, 0., -10.4410908, -21.5043985, -32.9596599, -44.576612, -54.88014, -65.3827734, -76.3521899, -88.0560672, -99.2001149, -112.238766, -128.064485, -147.569738, -161.556655, -185.044169, -222.960877, -280.235378, -293.441953, -363.205243, -521.795574, -801.483269, -1160.10306, -1853.23206, -2962.50825, -4729.38986, -7132.2067, -11041.982, -17106.5658, -25973.8078}, 
{304.952152, 292.952218, 284.33093, 278.349426, 274.268842, 271.350315, 268.854983, 266.043982, 262.17845, 253.319746, 243.208695, 232.386348, 221.393753, 212.329699, 203.554397, 194.985801, 186.541861, 178.001582, 169.477443, 160.942978, 152.371717, 143.465933, 134.578921, 125.792715, 117.189349, 109.241916, 101.484971, 93.8441289, 86.2450012, 78.3468177, 70.4481283, 62.5810991, 54.7778966, 47.0124833, 39.3985105, 31.9914262, 24.8466778, 18.9719631, 13.08958, 6.87407634, 0., -9.38029515, -19.4611898, -29.9592584, -40.5910756, -49.7813155, -59.0552134, -68.6461041, -78.7873225, -88.1276716, -99.1188308, -112.627947, -129.522169, -140.631068, -160.874397, -195.134334, -248.293057, -257.393244, -322.292372, -475.008418, -747.559361, -1096.57811, -1780.23784, -2878.9862, -4641.40046, -7069.0396, -10983.4062, -17001.0171, -25738.3888}, 
{279.78566, 269.98824, 262.640456, 257.177906, 253.036189, 249.650905, 246.457651, 242.892026, 238.389629, 229.788609, 220.160994, 209.981364, 199.724297, 191.311845, 183.192125, 175.260728, 167.413243, 159.281274, 151.129992, 142.960581, 134.774227, 126.404438, 118.087145, 109.890602, 101.883063, 94.4378202, 87.1960756, 80.1040686, 73.1080386, 65.9558819, 58.8715185, 51.8805254, 45.0084794, 38.1787473, 31.56, 25.2186982, 19.2213027, 14.7377201, 10.2895871, 5.50198627, 0., -8.3384809, -17.4423055, -26.9875147, -36.6501492, -44.7971021, -52.9372211, -61.2702062, -69.9957574, -77.6869199, -86.8207102, -98.2474902, -112.817622, -121.191888, -138.48606, -169.626333, -219.538899, -226.031281, -287.995812, -437.206154, -705.435969, -1047.95037, -1726.04867, -2820.76157, -4583.33267, -7035.48163, -10946.811, -16899.9525, -25477.5381}, 
{257.302813, 250.074573, 244.204199, 239.330028, 235.090398, 231.123647, 227.068111, 222.562129, 217.244038, 208.851392, 199.683625, 190.139389, 180.617334, 172.839114, 165.351176, 158.02297, 150.723945, 142.906536, 135.024016, 127.112641, 119.208668, 111.321731, 103.525358, 95.8664567, 88.3919317, 81.3337627, 74.4797527, 67.8027781, 61.2757154, 54.7583607, 48.3819031, 42.1644511, 36.1241133, 30.1364421, 24.4191248, 19.0472927, 14.0960769, 10.8651988, 7.71536327, 4.23186531, 0., -7.30986874, -15.4315422, -24.013753, -32.7052334, -39.8530662, -46.9282936, -54.100308, -61.5385019, -67.7347666, -75.206996, -84.795583, -97.3409206, -103.184816, -117.865682, -146.423345, -193.897633, -199.101319, -259.792105, -407.500641, -673.757574, -1012.59923, -1688.03923, -2782.74657, -4546.80967, -7020.11695, -10915.9374, -16779.1949, -25154.8133}, 
{237.457073, 232.838319, 228.454933, 224.153853, 219.782019, 215.186369, 210.213843, 204.71138, 198.525917, 190.328997, 181.613115, 172.69537, 163.892858, 156.701414, 149.787905, 142.997937, 136.177112, 128.601035, 120.91331, 113.187537, 105.497321, 98.0471923, 90.727455, 83.5593415, 76.5640841, 69.8106881, 63.253503, 56.8946518, 50.7362571, 44.7583326, 38.993954, 33.4540878, 28.1497004, 22.9162824, 18.0104665, 13.5134097, 9.5062687, 7.38006997, 5.38215293, 3.0697266, 0., -6.28867938, -13.4126964, -21.0072975, -28.7077289, -34.8747742, -40.9279278, -47.0122213, -53.2726862, -58.1281187, -64.1402798, -72.1446952, -82.9768906, -86.555554, -98.9997838, -125.511841, -171.293985, -176.348612, -237.157795, -385.00374, -651.168655, -988.904083, -1663.58422, -2759.85339, -4523.45463, -7011.52972, -10874.5267, -16614.567, -24733.7721}, 
{221.112345, 217.273009, 213.245771, 208.971399, 204.390662, 199.444326, 194.07316, 188.21793, 181.819406, 174.07591, 165.967632, 157.732316, 149.607708, 142.759168, 136.12578, 129.574241, 122.971252, 115.636065, 108.201803, 100.754142, 93.3787588, 86.3649399, 79.5133073, 72.8280932, 66.31353, 59.9400399, 53.7591891, 47.7887339, 42.0464305, 36.5675385, 31.3453094, 26.3904981, 21.7138595, 17.1544906, 12.9634673, 9.22020788, 6.00413027, 4.68026363, 3.52817058, 2.11302482, 0., -5.20156086, -11.1883212, -17.612775, -24.1274164, -29.1515342, -34.0641099, -39.0109194, -44.1377391, -47.8775291, -52.7740078, -59.6580777, -69.3606412, -71.3464121, -82.3589571, -107.775654, -152.973881, -158.692157, -220.802263, -370.537118, -639.129644, -981.466477, -1657.81939, -2751.51569, -4503.96735, -6999.36377, -10792.7001, -16321.2699, -24022.367}, 
{207.178203, 203.732178, 199.820678, 195.45373, 190.64136, 185.393593, 179.720456, 173.631975, 167.138176, 159.874527, 152.375435, 144.800749, 137.310317, 130.721435, 124.273526, 117.863461, 111.388109, 104.253972, 97.0444365, 89.8525206, 82.7712422, 76.1562566, 69.7328892, 63.4891028, 57.4128605, 51.3843422, 45.5424068, 39.9181301, 34.5425882, 29.4933277, 24.7363656, 20.2841896, 16.1492874, 12.1875237, 8.63065835, 5.55382834, 3.03217056, 2.36462843, 1.91300975, 1.26292884, 0., -4.1202108, -8.95002157, -14.1717987, -19.4679084, -23.3384285, -27.1209295, -30.9706931, -35.0430011, -37.8103107, -41.7838582, -47.7920555, -56.6633142, -57.3923226, -67.3747053, -92.1723635, -137.347198, -144.531429, -208.788512, -361.25222, -633.056326, -981.531677, -1659.22083, -2747.46148, -4481.02972, -6970.9883, -10669.8149, -15944.6335, -23162.5681}, 
{195.375656, 192.004692, 188.015746, 183.466195, 178.413415, 172.914785, 167.027681, 160.809479, 154.317557, 147.537176, 140.626675, 133.672277, 126.760207, 120.360119, 114.021432, 107.676999, 101.25967, 94.2939214, 87.28433, 80.3270978, 73.5184264, 67.2620822, 61.2236764, 55.3763849, 49.6933835, 43.9759018, 38.4378405, 33.1211539, 28.0677965, 23.3846307, 19.0227396, 14.9981141, 11.3267455, 7.89149902, 4.89474193, 2.40571575, 0.493661944, 0.363790214, 0.494986523, 0.502105057, 0., -3.05610252, -6.72786903, -10.7365941, -14.8035724, -17.5234724, -20.1948652, -22.9896958, -26.0799092, -27.9994607, -31.213481, -36.549111, -44.8334916, -44.6143205, -53.9099591, -78.4593255, -124.001338, -133.312004, -200.278317, -355.824359, -630.8742119999999, -985.890452, -1663.18168, -2741.44858, -4446.50068, -6914.71693, -10493.0898, -15475.9586, -22157.6622}, 
{185.425713, 181.879415, 177.667063, 172.87414, 167.58613, 161.888515, 155.86678, 149.606408, 143.192881, 136.876184, 130.511499, 124.11851, 117.716901, 111.447124, 105.159785, 98.8262595, 92.4179225, 85.5950808, 78.7646057, 72.0222999, 65.4639662, 59.5233565, 53.8231446, 48.3239537, 42.9864068, 37.5470253, 32.2801747, 27.2361189, 22.4651217, 18.090378, 14.0600481, 10.3952234, 7.11699527, 4.1425339, 1.63842043, -0.332685234, -1.7081232, -1.39162518, -0.767583015, -0.186780138, 0., -2.0207093, -4.55193494, -7.35938705, -10.2087758, -11.7946811, -13.3823954, -15.166081, -17.3399, -18.5179766, -21.1065264, -25.9317269, -33.8197554, -32.9334411, -41.8276492, -66.3938965, -112.5237, -124.479458, -194.433453, -352.92885, -630.508812, -991.333571, -1665.09509, -2727.23475, -4392.23916, -6818.86329, -10249.7435, -14906.5457, -21010.9359}, 
{177.049383, 173.145211, 168.610719, 163.542914, 158.038804, 152.195397, 146.109701, 139.878725, 133.599477, 127.703878, 121.820057, 115.911057, 109.93992, 103.754355, 97.4788721, 91.1226459, 84.6948522, 77.9966166, 71.3283849, 64.7825529, 58.4515163, 52.7810191, 47.3687694, 42.1658237, 37.1232385, 31.9300195, 26.9040941, 22.1013388, 17.5776303, 13.4595, 9.70390802, 6.33846907, 3.39079796, 0.816745601, -1.25560381, -2.76992995, -3.66991249, -2.97099193, -1.91638277, -0.821060353, 0., -1.02550445, -2.45229068, -4.09240305, -5.75788596, -6.24006989, -6.77999864, -7.59800213, -8.91441027, -9.4388557, -11.5066445, -15.9423856, -23.5706878, -22.2707196, -30.9907063, -55.7334329, -102.501684, -117.479367, -190.415696, -351.241008, -629.885639, -994.651801, -1660.35421, -2698.57779, -4310.10409, -6671.741, -9926.99437, -14227.6955, -19725.6759}, 
{187.82453, 175.380638, 164.522254, 155.048751, 146.759502, 139.453881, 132.931261, 126.991014, 121.432515, 115.682444, 110.061944, 104.519465, 99.0034562, 93.4094581, 87.7599952, 82.0246817, 76.1731321, 69.8497126, 63.4793845, 57.1618613, 50.9968561, 45.3113449, 39.8868738, 34.732251, 29.8562852, 25.3107344, 21.0442775, 17.0485431, 13.3151599, 9.78749463, 6.52474233, 3.53783623, 0.837709548, -1.92123783, -4.22892593, -5.93180815, -6.87633789, -5.53361613, -3.67558966, -1.69885283, 0., -0.623134421, -1.65833802, -2.8432016, -3.91531598, -3.23888229, -2.47423689, -1.90832646, -1.82809767, -0.571915493, -1.15474102, -4.64295363, -12.1029327, -13.5154792, -25.4667823, -53.4574527, -102.988101, -117.986443, -190.155143, -349.623968, -626.522688, -999.557189, -1653.12889, -2646.10523, -4169.23759, -6411.62763, -9437.09199, -13340.7686, -18217.7952}, 
{202.594033, 179.751196, 161.257808, 146.529047, 134.980092, 126.026122, 119.082316, 113.563853, 108.885911, 103.059218, 97.4651852, 92.0807714, 86.8829361, 82.0677256, 77.3053773, 72.4852159, 67.4965658, 61.6813602, 55.6952715, 49.6465805, 43.6435684, 37.8332563, 32.2696889, 27.0456511, 22.2539279, 18.4601648, 15.0951418, 12.0624993, 9.26587801, 6.39380025, 3.65107215, 1.02738158, -1.48758357, -4.64361367, -7.41575128, -9.51851721, -10.6664323, -8.53514926, -5.69360421, -2.67186512, 0., -0.508669667, -1.50711222, -2.60515855, -3.41263955, -1.63513838, 0.451567238, 2.47594729, 4.06647178, 7.34644229, 8.45156461, 6.01237612, -1.34058582, -6.53591649, -22.7602928, -54.7595243, -107.279421, -121.97668, -191.921867, -347.096438, -617.481845, -993.994698, -1623.81099, -2550.28408, -3950.79094, -6012.26715, -8749.39028, -12219.9024, -16481.5456}, 
{202.282333, 175.871048, 154.829792, 138.423698, 125.9179, 116.577528, 109.667717, 104.453599, 100.200306, 94.3003516, 88.640535, 83.2350361, 78.0980346, 73.6586754, 69.3501876, 65.0207652, 60.5186022, 55.0509575, 49.3633342, 43.5603002, 37.7464236, 31.9534243, 26.3878575, 21.1834305, 16.4738504, 13.064521, 10.1487743, 7.59163891, 5.25814349, 2.701185, 0.222776282, -2.18720142, -4.53886688, -7.71996386, -10.5119361, -12.5738524, -13.5647814, -10.8155658, -7.24479091, -3.44281592, 0., -0.08641868, -0.680828277, -1.34170145, -1.62751084, 1.08043447, 4.17563281, 7.22874608, 9.81043621, 14.0900627, 16.0001108, 14.0717634, 6.83620339, -0.0235436439, -17.9888746, -51.3893436, -104.554505, -121.858854, -191.569027, -341.996602, -601.453156, -968.32838, -1560.69951, -2412.60149, -3678.80089, -5530.33367, -7952.42665, -10976.7589, -14635.0096}, 
{191.434042, 166.516391, 146.685199, 131.231755, 119.447344, 110.623254, 104.050772, 99.021184, 94.8257772, 88.8485881, 83.051054, 77.4873619, 72.211699, 67.8168808, 63.6030143, 59.4088351, 55.0730788, 49.8012396, 44.318591, 38.7171651, 33.088994, 27.4028755, 21.9233699, 16.7918031, 12.1495011, 8.82957003, 6.00484342, 3.539935, 1.29945857, -1.20513783, -3.61480841, -5.9236731, -8.12585181, -11.0434715, -13.5114422, -15.1926812, -15.7501056, -12.5223676, -8.42635531, -4.05469173, 0., 0.584774086, 0.691459186, 0.751561269, 1.1965863, 4.6894944, 8.53775573, 12.2802946, 15.4560354, 19.9833366, 22.0699146, 20.30292, 13.2695034, 6.39295062, -11.3101771, -43.9871831, -95.785371, -117.497179, -187.966722, -332.683249, -577.13601, -923.480797, -1467.20723, -2237.65049, -3360.76265, -4981.95802, -7072.18514, -9646.00898, -12717.9945}, 
{174.593769, 154.463423, 138.271025, 125.452267, 115.442846, 107.678454, 101.594787, 96.6275385, 92.2124029, 86.1466711, 80.1598024, 74.3428524, 68.7868768, 64.1769151, 59.7724458, 55.426931, 50.993833, 45.7749414, 40.3960599, 34.9313197, 29.4548519, 23.9126366, 18.5582165, 13.5169832, 8.91432833, 5.46107933, 2.46301766, -0.188639368, -2.60267448, -5.23995495, -7.71634509, -9.99979375, -12.0582498, -14.5073195, -16.4082314, -17.4698714, -17.4011254, -13.8030567, -9.33550295, -4.55047905, 0., 1.44606418, 2.48069555, 3.47902116, 4.81616805, 8.97369958, 13.3777319, 17.5608176, 21.055509, 25.3506551, 27.2399934, 25.4735579, 18.8013827, 13.0848776, -2.88184972, -33.1933156, -81.9440363, -108.755869, -179.985053, -317.515168, -543.229795, -860.374511, -1346.74691, -2030.02411, -3004.17146, -4383.27103, -6134.6498, -8262.3235, -10770.3079}, 
{156.306122, 142.488343, 131.034262, 121.584286, 113.778824, 107.258283, 101.66307, 96.6335931, 91.81026, 85.6373442, 79.4298407, 73.3066107, 67.3865153, 62.3733517, 57.5670705, 52.8525584, 48.1147021, 42.814798, 37.4307592, 32.0169088, 26.6275694, 21.2137341, 15.9743875, 11.0051847, 6.40178052, 2.66481611, -0.677034553, -3.69011116, -6.44075338, -9.31805307, -11.9364969, -14.2333237, -16.1457723, -18.0046911, -19.1962656, -19.5002906, -18.6965613, -14.805135, -10.0694394, -4.97316436, 0., 2.43860714, 4.5578262, 6.6450698, 8.98775053, 13.7147082, 18.5353573, 23.0005397, 26.6610969, 30.516409, 32.0893643, 30.3513891, 24.2739098, 20.4235486, 7.13845799, -19.6480136, -64.0025181, -95.4991373, -166.494121, -294.851148, -498.433899, -779.932084, -1202.73129, -1794.31537, -2616.52254, -3750.40353, -5165.80466, -6860.37338, -8831.75711}, 
{141.115714, 133.367348, 126.421904, 120.126861, 114.329698, 108.877894, 103.618928, 98.4002787, 93.0694261, 86.7633508, 80.3242294, 73.8837403, 67.5735619, 62.0407637, 56.6954766, 51.4632226, 46.2695237, 40.7635444, 35.2577072, 29.788077, 24.3907188, 19.0371946, 13.8538735, 8.90262193, 4.24530605, 0.146547664, -3.61564479, -7.06050731, -10.2072759, -13.3542189, -16.129927, -18.4420232, -20.1981307, -21.4287693, -21.8695065, -21.3788065, -19.8151337, -15.6761045, -10.7253702, -5.36573418, 0., 3.50355851, 6.79379651, 10.0540987, 13.4678499, 18.6941784, 23.8504278, 28.5296855, 32.3250388, 35.8049895, 37.1970445, 35.7041252, 30.5291532, 28.7802748, 18.5930966, -3.99155006, -42.9328337, -77.5911981, -146.364027, -263.049978, -441.44771, -683.076077, -1038.57315, -1535.11731, -2205.31111, -3099.48633, -4191.63378, -5474.82953, -6942.14962}, 
{103.940274, 109.246159, 112.100836, 112.786499, 111.585343, 108.779563, 104.651353, 99.482908, 93.556423, 87.4462621, 81.0255828, 74.4597118, 67.9139761, 62.0289608, 56.3046312, 50.7162112, 45.2389241, 39.780798, 34.4111305, 29.1320235, 23.945579, 18.7904806, 13.7576159, 8.87445415, 4.16846479, -0.307552597, -4.56159118, -8.57631379, -12.3343833, -16.2187059, -19.6516038, -22.4556421, -24.4533865, -25.0813324, -24.702543, -23.294012, -20.8327326, -16.4901945, -11.3710965, -5.77463332, 0., 4.75478168, 9.44887446, 14.1426142, 18.8963367, 25.0263267, 30.8345917, 35.8790878, 39.7177713, 41.4854709, 41.3325214, 38.9861302, 34.1735046, 36.3321689, 31.5948865, 15.8047379, -15.1951963, -53.3876082, -119.973336, -223.978989, -374.43118, -567.576215, -850.781612, -1259.91447, -1784.82538, -2422.06506, -3187.40028, -4091.7938, -5146.20839}, 
{119.696459, 121.904076, 122.249633, 120.94698, 118.209966, 114.25244, 109.288254, 103.531255, 97.1952948, 90.7420274, 84.0383753, 77.1990662, 70.3388277, 63.8365474, 57.4371289, 51.1496357, 44.9831315, 38.9566103, 33.0652331, 27.3140912, 21.708276, 16.2226188, 10.9045753, 5.77134083, 0.84011102, -3.93325859, -8.44569664, -12.6554717, -16.5208523, -20.3073567, -23.5431039, -26.0634625, -27.7038011, -27.8850365, -27.02277, -25.1181511, -22.1723292, -17.5689495, -12.1736673, -6.23463379, 0., 5.72320321, 11.4692564, 17.2135602, 22.9315156, 29.3276159, 35.3565322, 40.7020283, 45.0478681, 48.0136362, 49.3729471, 48.8352361, 46.1099386, 48.6623724, 45.3437374, 32.7611163, 7.52159136, -26.3360594, -82.6091273, -167.663208, -287.863898, -441.843924, -659.167485, -966.431705, -1346.42536, -1792.66723, -2299.56444, -2861.30205, -3472.06512}, 
{169.150239, 157.676836, 147.513519, 138.460033, 130.316123, 122.881534, 115.956011, 109.3393, 102.831146, 95.6550193, 88.4174499, 81.1486922, 73.8790011, 66.6745961, 59.5153815, 52.4172262, 45.3959992, 38.4031129, 31.5446755, 24.8623387, 18.3977542, 12.2411791, 6.36621736, 0.795078651, -4.45002757, -9.41572308, -13.9834346, -18.10342, -21.7259373, -24.9162069, -27.4635394, -29.2722079, -30.2464854, -30.1545278, -29.0911722, -27.0151386, -23.8851471, -18.9590206, -13.1767348, -6.77736834, 0., 6.50069279, 13.0674696, 19.6274911, 26.1079182, 32.5190318, 38.6716244, 44.4596088, 49.7768977, 55.8502074, 60.7075256, 63.7096435, 64.2173524, 64.9421617, 60.5538573, 49.0729433, 28.5199236, 5.13411772, -34.5703119, -95.8603875, -184.003131, -305.004472, -461.914711, -660.672514, -900.983227, -1205.90322, -1511.31808, -1774.90607, -1954.34547}, 
{157.588269, 149.87012, 142.397846, 135.138514, 128.05919, 121.126941, 114.308835, 107.571936, 100.883312, 94.184065, 87.4776118, 80.7414055, 73.952899, 66.9294522, 59.8726476, 52.8239751, 45.8249243, 38.9794848, 32.2416468, 25.6279002, 19.1547349, 12.7006072, 6.47525399, 0.550378712, -5.00231531, -9.89727766, -14.3621909, -18.4108905, -22.0572119, -25.8017927, -28.9769454, -31.4017847, -32.8954252, -32.8713233, -31.7165151, -29.4123786, -25.9402917, -20.6885535, -14.4688524, -7.49979804, 0., 7.57072875, 15.3314634, 23.1600759, 30.9344379, 38.6372245, 45.9995829, 52.8574638, 59.0468175, 64.5920658, 69.0652995, 72.2270807, 73.8379713, 76.5085846, 76.0094109, 70.9609919, 59.9838691, 45.5410236, 20.8735817, -16.934891, -70.8008287, -145.218014, -238.370836, -359.648158, -484.977861, -600.901629, -684.966921, -716.969752, -676.706137}, 
{170.100056, 162.077358, 154.716959, 147.843675, 141.282325, 134.857726, 128.394696, 121.718051, 114.652611, 106.038997, 97.0798998, 87.993815, 78.9992377, 71.0318317, 63.3060567, 55.7535409, 48.3059124, 40.5721135, 32.9355325, 25.4568718, 18.1968339, 11.2698263, 4.66136436, -1.58933151, -7.44304092, -12.8267762, -17.748591, -22.1827721, -26.1036058, -29.8062716, -32.815806, -34.9781382, -36.1391978, -35.7891867, -34.2720525, -31.5760155, -27.6892961, -22.1624404, -15.5964124, -8.15450233, 0., 8.75252273, 17.8715705, 27.174366, 36.478132, 45.3117409, 53.896106, 62.1637903, 70.0473565, 77.8136669, 84.9272654, 91.1869954, 96.3917, 101.4355, 104.58385, 105.197482, 102.63713, 95.6793504, 84.50272, 68.7016409, 47.8705152, 17.3576345, -10.504268, -36.2030578, -39.5162004, -13.1099957, 81.1941944, 273.863774, 595.366146}, 
{177.272638, 176.171898, 172.67048, 167.127495, 159.902055, 151.353269, 141.840249, 131.722107, 121.357952, 111.932907, 102.649669, 93.5369434, 84.6234388, 76.1690857, 67.8788777, 59.6890322, 51.5357667, 42.9013283, 34.3574928, 26.0220658, 18.0128529, 10.8353733, 4.06463329, -2.336647, -8.40574756, -14.5091726, -20.223288, -25.4536843, -30.1059515, -34.3111544, -37.6592191, -39.965546, -41.0455356, -40.0655795, -37.7496906, -34.1728732, -29.4101313, -23.5468726, -16.6435361, -8.77096437, 0., 10.1187145, 20.7860564, 31.7231031, 42.6509319, 52.6140477, 62.2807294, 71.6426832, 80.6916156, 89.0636486, 97.2483064, 105.379529, 113.591258, 122.654121, 131.810694, 140.94024, 149.922024, 154.37936, 160.149841, 168.815111, 181.956813, 197.339586, 227.99609, 274.768336, 359.778501, 504.715443, 711.721124, 987.824231, 1340.05345}
};
  
  /* Stuff for interpolating the NQC data */
  gsl_spline    *spline = NULL;
  gsl_interp_accel *acc = NULL;
  /* Interpolate the spin NQC data in 2-D parameter space -- spin and mass ratio */
  /* First, interpolate in spin dimension for all mass ratios */
  spline = gsl_spline_alloc( gsl_interp_cspline, adim );
  acc    = gsl_interp_accel_alloc();
  for (i = 0; i < qdim; i++)
  {
    gsl_spline_init( spline, alist, a3stab[i], adim );
    gsl_interp_accel_reset( acc );
    a3slist[i] = gsl_spline_eval( spline, a/(1.0-2.0*eta), acc );
    gsl_spline_init( spline, alist, a4tab[i], adim );
    gsl_interp_accel_reset( acc );
    a4list[i] = gsl_spline_eval( spline, a/(1.0-2.0*eta), acc );
    gsl_spline_init( spline, alist, a5tab[i], adim );
    gsl_interp_accel_reset( acc );
    a5list[i] = gsl_spline_eval( spline, a/(1.0-2.0*eta), acc );
//printf("%.15f\n",a3slist[i]);
  }
//printf("%.15f\n",a);
  gsl_spline_free(spline);
  gsl_interp_accel_free(acc);
  /* Second, interpolate in mass ratio dimension */
  spline = gsl_spline_alloc( gsl_interp_cspline, qdim );
  acc    = gsl_interp_accel_alloc();
  gsl_spline_init( spline, etalist, a3slist, qdim );
  gsl_interp_accel_reset( acc );
  coeffs->a3S = gsl_spline_eval( spline, eta, acc );
  gsl_spline_init( spline, etalist, a4list, qdim );
  gsl_interp_accel_reset( acc );
  coeffs->a4 = gsl_spline_eval( spline, eta, acc );
  gsl_spline_init( spline, etalist, a5list, qdim );
  gsl_interp_accel_reset( acc );
  coeffs->a5 = gsl_spline_eval( spline, eta, acc );
  gsl_spline_free(spline);
  gsl_interp_accel_free(acc);
 
  /* Interpolate nonspin NQC data in the mass ratio dimension */
  spline = gsl_spline_alloc( gsl_interp_cspline, nsqdim );
  acc    = gsl_interp_accel_alloc();
  gsl_spline_init( spline, nsetalist, a1list, nsqdim );
  gsl_interp_accel_reset( acc );
  coeffs->a1 = gsl_spline_eval( spline, eta, acc );
  gsl_spline_init( spline, nsetalist, a2list, nsqdim );
  gsl_interp_accel_reset( acc );
  coeffs->a2 = gsl_spline_eval( spline, eta, acc );
  gsl_spline_init( spline, nsetalist, a3list, nsqdim );
  gsl_interp_accel_reset( acc );
  coeffs->a3 = gsl_spline_eval( spline, eta, acc );
  gsl_spline_init( spline, nsetalist, b1list, nsqdim );
  gsl_interp_accel_reset( acc );
  coeffs->b1 = gsl_spline_eval( spline, eta, acc );
  gsl_spline_init( spline, nsetalist, b2list, nsqdim );
  gsl_interp_accel_reset( acc );
  coeffs->b2 = gsl_spline_eval( spline, eta, acc );
  gsl_spline_free(spline);
  gsl_interp_accel_free(acc);
  /* Andrea and I have different sign conventions, so I need to put a minus sign in front */
  coeffs->b1 = - coeffs->b1;
  coeffs->b2 = - coeffs->b2;
  /*coeffs->a1 = -6.292993135543338;
  coeffs->a2 = 40.26795686019975;
  coeffs->a3 = -39.29214347590138;
  coeffs->a3S= 226.3844538692294;
  coeffs->a4 = -747.9690610155075;
  coeffs->a5 = 623.9090070111784;
  coeffs->b1 = 0.7467610798775989;
  coeffs->b2 = -2.159938635119833;
  */
  /* Obsolete polynomial fitting of nonspin NQC coefficients a1, a2, a3, b1 and b2 */
  /*
  coeffs->a1 = -12.67955358602124 + 75.41927959573084 * eta - 106.15933052937714 * eta2;
  coeffs->a2 = 101.45522216901628 - 757.3158549733314 * eta + 1473.314771676588 * eta2;
  coeffs->a3 = -107.6647834845902 + 857.6219519536213 * eta - 1776.2776804623143 * eta2;
  // Andrea and I have different sign conventions, so I need to put a minus sign in front 
  coeffs->b1 = - (-1.464129495621165 + 12.81732978488213 * eta - 60.09957767247623 * eta2);
  coeffs->b2 = - ( 7.477426352542122 - 85.26122117590637 * eta + 353.3251639728075 * eta2);
  */

  return XLAL_SUCCESS;

}


 int XLALSimIMRSpinEOBCalculateNQCCoefficients(
                 REAL8Vector    *amplitude,   /**<< Waveform amplitude, func of time */
                 REAL8Vector    *phase,       /**<< Waveform phase(rad), func of time */
                 REAL8Vector    *rVec,        /**<< Position-vector, function of time */
                 REAL8Vector    *prVec,       /**<< Momentum vector, function of time */
                 REAL8Vector    *orbOmegaVec, /**<< Orbital frequency, func of time */
                 INT4                      l,           /**<< Mode index l */
                 INT4                      m,           /**<< Mode index m */
                 REAL8                     timePeak,    /**<< Time of peak orbital frequency */
                 REAL8                     deltaT,      /**<< Sampling interval */
                 REAL8                     eta,         /**<< Symmetric mass ratio */
                 REAL8                     a,           /**<< Normalized spin of deformed-Kerr */
                 EOBNonQCCoeffs *coeffs       /**<< OUTPUT, NQC coefficients */)
{

  /* For gsl permutation stuff */

  int signum;

  REAL8Vector *timeVec = NULL;

  /* Vectors which are used in the computation of the NQC coefficients */
  REAL8Vector *q3 = NULL, *q4 = NULL, *q5 = NULL;
  REAL8Vector *p3 = NULL, *p4 = NULL;

  REAL8Vector *qNS = NULL, *pNS = NULL;

  /* Since the vectors we actually want are q etc * A, we will have to generate them here */
  REAL8Vector *q3LM  = NULL;
  REAL8Vector *q4LM  = NULL;
  REAL8Vector *q5LM  = NULL; 
  REAL8Vector *qNSLM = NULL;

  REAL8 amp, aDot, aDDot;
  REAL8 omega, omegaDot;

  REAL8 qNSLMPeak, qNSLMDot, qNSLMDDot;
  REAL8 pNSLMDot, pNSLMDDot;

  REAL8 nra, nraDDot;
  REAL8 nromega, nromegaDot;

  REAL8 nrDeltaT, nrTimePeak;

  /* Stuff for finding numerical derivatives */
  gsl_spline    *spline = NULL;
  gsl_interp_accel *acc = NULL;

  /* Matrix stuff for calculating coefficients */
  gsl_matrix *qMatrix = NULL, *pMatrix = NULL;
  gsl_vector *aCoeff  = NULL, *bCoeff  = NULL;

  gsl_vector *amps = NULL, *omegaVec = NULL;

  gsl_permutation *perm1 = NULL, *perm2 = NULL;

  memset( coeffs, 0, sizeof( EOBNonQCCoeffs ) );

  /* Populate the time vector */
  /* It is okay to assume initial t = 0 */
  timeVec = XLALCreateREAL8Vector( rVec->length );
  q3    = XLALCreateREAL8Vector( rVec->length );
  q4    = XLALCreateREAL8Vector( rVec->length );
  q5    = XLALCreateREAL8Vector( rVec->length );
  p3    = XLALCreateREAL8Vector( rVec->length );
  p4    = XLALCreateREAL8Vector( rVec->length );
  qNS   = XLALCreateREAL8Vector( rVec->length );
  pNS   = XLALCreateREAL8Vector( rVec->length );
  q3LM  = XLALCreateREAL8Vector( rVec->length );
  q4LM  = XLALCreateREAL8Vector( rVec->length );
  q5LM  = XLALCreateREAL8Vector( rVec->length );
  qNSLM = XLALCreateREAL8Vector( rVec->length );

  if ( !timeVec || !q3 || !q4 || !q5 || !p3 || !p4 || !qNS || !pNS || !q3LM
          || !q4LM || !q5LM || !qNSLM )
  {
    XLALDestroyREAL8Vector( timeVec );
    XLALDestroyREAL8Vector( q3 );
    XLALDestroyREAL8Vector( q4 );
    XLALDestroyREAL8Vector( q5 );
    XLALDestroyREAL8Vector( p3 );
    XLALDestroyREAL8Vector( p4 );
    XLALDestroyREAL8Vector( qNS );
    XLALDestroyREAL8Vector( pNS );
    XLALDestroyREAL8Vector( q3LM );
    XLALDestroyREAL8Vector( q4LM );
    XLALDestroyREAL8Vector( q5LM );
    XLALDestroyREAL8Vector( qNSLM );
    fprintf(stderr,"Error(%d)",XLAL_EFUNC );
  }

  /* We need the calibrated non-spinning NQC coefficients */
  if ( XLALSimIMRGetEOBCalibratedSpinNQC( coeffs, l, m, eta, a ) == XLAL_FAILURE )
  {
    XLALDestroyREAL8Vector( timeVec );
    XLALDestroyREAL8Vector( q3 );
    XLALDestroyREAL8Vector( q4 );
    XLALDestroyREAL8Vector( q5 );
    XLALDestroyREAL8Vector( p3 );
    XLALDestroyREAL8Vector( p4 );
    XLALDestroyREAL8Vector( qNS );
    XLALDestroyREAL8Vector( pNS );
    XLALDestroyREAL8Vector( q3LM );
    XLALDestroyREAL8Vector( q4LM );
    XLALDestroyREAL8Vector( q5LM );
    XLALDestroyREAL8Vector( qNSLM );
    fprintf(stderr,"Error(%d)",XLAL_EFUNC );
  }

  /* Populate vectors as necessary. Eqs. 14 - 17 of the LIGO DCC document T1100433v2 */
  unsigned int i;
  for ( i = 0; i < timeVec->length; i++ )
  {
    
    REAL8 rootR  = sqrt(rVec->data[i]);
    REAL8 rOmega = rVec->data[i] * orbOmegaVec->data[i];

    /* We don't need these as vectors as their coefficients are calibrated */
    REAL8 q1, q2, p1, p2;

    timeVec->data[i] = i * deltaT;
    q1            = prVec->data[i]*prVec->data[i] / (rOmega*rOmega);
    q2            = q1 / rVec->data[i];
    q3->data[i]   = q2 / rootR;
    q4->data[i]   = q2 / rVec->data[i];
    q5->data[i]   = q3->data[i] / rVec->data[i];

    p1          = prVec->data[i] / rOmega;
    p2          = p1 * prVec->data[i] * prVec->data[i];
    p3->data[i] = p2 / rootR;
    p4->data[i] = p2 / rVec->data[i];

    qNS->data[i]  = coeffs->a1 * q1 + coeffs->a2 * q2 + coeffs->a3 * q3->data[i];
    pNS->data[i]  = coeffs->b1 * p1 + coeffs->b2 * p2;
    q3LM->data[i] = q3->data[i] * amplitude->data[i];
    q4LM->data[i] = q4->data[i] * amplitude->data[i];
    q5LM->data[i] = q5->data[i] * amplitude->data[i];

    qNSLM->data[i] = qNS->data[i] * amplitude->data[i];
  }

  /* Allocate all the memory we need */
  //XLAL_CALLGSL(
    /* a stuff */
    qMatrix = gsl_matrix_alloc( 3, 3 );
    aCoeff  = gsl_vector_alloc( 3 );
    amps    = gsl_vector_alloc( 3 );
    perm1   = gsl_permutation_alloc( 3 );

    /* b stuff */
    pMatrix  = gsl_matrix_alloc( 2, 2 );
    bCoeff   = gsl_vector_alloc( 2 );
    omegaVec = gsl_vector_alloc( 2 );
    perm2    = gsl_permutation_alloc( 2 );
  //);

  if ( !qMatrix || !aCoeff || !amps || !pMatrix || !bCoeff || !omegaVec )
  {
    XLALDestroyREAL8Vector( timeVec );
    XLALDestroyREAL8Vector( q3 );
    XLALDestroyREAL8Vector( q4 );
    XLALDestroyREAL8Vector( q5 );
    XLALDestroyREAL8Vector( p3 );
    XLALDestroyREAL8Vector( p4 );
    XLALDestroyREAL8Vector( qNS );
    XLALDestroyREAL8Vector( pNS );
    XLALDestroyREAL8Vector( q3LM );
    XLALDestroyREAL8Vector( q4LM );
    XLALDestroyREAL8Vector( q5LM );
    XLALDestroyREAL8Vector( qNSLM );
    fprintf(stderr,"Error(%d)",XLAL_EFUNC );
  }

  /* The time we want to take as the peak time depends on l and m */
  /* Calculate the adjustment we need to make here */
  nrDeltaT   = XLALSimIMREOBGetNRSpinPeakDeltaT( l, m, eta, a );
#if debugOutput
    fprintf(stderr, "DEBUG:nrDeltaT=%f\n", nrDeltaT);
#endif
  if ( XLAL_IS_REAL8_FAIL_NAN( nrDeltaT ) )
  {
    XLALDestroyREAL8Vector( timeVec );
    XLALDestroyREAL8Vector( q3 );
    XLALDestroyREAL8Vector( q4 );
    XLALDestroyREAL8Vector( q5 );
    XLALDestroyREAL8Vector( p3 );
    XLALDestroyREAL8Vector( p4 );
    XLALDestroyREAL8Vector( qNS );
    XLALDestroyREAL8Vector( pNS );
    XLALDestroyREAL8Vector( q3LM );
    XLALDestroyREAL8Vector( q4LM );
    XLALDestroyREAL8Vector( q5LM );
    XLALDestroyREAL8Vector( qNSLM );
    fprintf(stderr,"Error(%d)",XLAL_EFUNC );
  }

  /* nrDeltaT defined in XLALSimIMREOBGetNRSpinPeakDeltaT is a minus sign different from Eq. (33) of Taracchini et al.
   * Therefore, the plus sign in Eq. (21) of Taracchini et al and Eq. (18) of DCC document T1100433v2 is 
   * changed to a minus sign here.
   */
  nrTimePeak = timePeak - nrDeltaT;
#if debugOutput
    fprintf(stderr, "DEBUG:nrTimePeak=%f\n", nrTimePeak);
#endif
  /* We are now in a position to use the interp stuff to calculate the derivatives we need */
  /* We will start with the quantities used in the calculation of the a coefficients */
  spline = gsl_spline_alloc( gsl_interp_cspline, amplitude->length );
  acc    = gsl_interp_accel_alloc();

  /* Populate the Q matrix in Eq. 18 of the LIGO DCC document T1100433v2 */
  /* Q3 */
  gsl_spline_init( spline, timeVec->data, q3LM->data, q3LM->length );
  gsl_matrix_set( qMatrix, 0, 0, gsl_spline_eval( spline, nrTimePeak, acc ) );
  gsl_matrix_set( qMatrix, 1, 0, gsl_spline_eval_deriv( spline, nrTimePeak, acc ) );
  gsl_matrix_set( qMatrix, 2, 0, gsl_spline_eval_deriv2( spline, nrTimePeak, acc ) );

  /* Q4 */
  gsl_spline_init( spline, timeVec->data, q4LM->data, q4LM->length );
  gsl_interp_accel_reset( acc );
  gsl_matrix_set( qMatrix, 0, 1, gsl_spline_eval( spline, nrTimePeak, acc ) );
  gsl_matrix_set( qMatrix, 1, 1, gsl_spline_eval_deriv( spline, nrTimePeak, acc ) );
  gsl_matrix_set( qMatrix, 2, 1, gsl_spline_eval_deriv2( spline, nrTimePeak, acc ) );

  /* Q5 */
  gsl_spline_init( spline, timeVec->data, q5LM->data, q5LM->length );
  gsl_interp_accel_reset( acc );
  gsl_matrix_set( qMatrix, 0, 2, gsl_spline_eval( spline, nrTimePeak, acc ) );
  gsl_matrix_set( qMatrix, 1, 2, gsl_spline_eval_deriv( spline, nrTimePeak, acc ) );
  gsl_matrix_set( qMatrix, 2, 2, gsl_spline_eval_deriv2( spline, nrTimePeak, acc ) );

  /* Populate the r.h.s vector of Eq. 18 of the LIGO DCC document T1100433v2 */
  /* Amplitude */
  gsl_spline_init( spline, timeVec->data, amplitude->data, amplitude->length );
  gsl_interp_accel_reset( acc );
  amp   = gsl_spline_eval( spline, nrTimePeak, acc );
  aDot  = gsl_spline_eval_deriv( spline, nrTimePeak, acc );
  aDDot = gsl_spline_eval_deriv2( spline, nrTimePeak, acc );

  /* qNSLM */
  gsl_spline_init( spline, timeVec->data, qNSLM->data, qNSLM->length );
  gsl_interp_accel_reset( acc );
  qNSLMPeak = gsl_spline_eval( spline, nrTimePeak, acc );
  qNSLMDot  = gsl_spline_eval_deriv( spline, nrTimePeak, acc );
  qNSLMDDot = gsl_spline_eval_deriv2( spline, nrTimePeak, acc );

  nra = GetNRSpinPeakAmplitude( l, m, eta, a );
  nraDDot = - GetNRSpinPeakADDot( l, m, eta, a );

  if ( XLAL_IS_REAL8_FAIL_NAN( nra ) || XLAL_IS_REAL8_FAIL_NAN( nraDDot ) )
  {
    XLALDestroyREAL8Vector( timeVec );
    XLALDestroyREAL8Vector( q3 );
    XLALDestroyREAL8Vector( q4 );
    XLALDestroyREAL8Vector( q5 );
    XLALDestroyREAL8Vector( p3 );
    XLALDestroyREAL8Vector( p4 );
    XLALDestroyREAL8Vector( qNS );
    XLALDestroyREAL8Vector( pNS );
    XLALDestroyREAL8Vector( q3LM );
    XLALDestroyREAL8Vector( q4LM );
    XLALDestroyREAL8Vector( q5LM );
    XLALDestroyREAL8Vector( qNSLM );
    fprintf(stderr,"Error(%d)",XLAL_EFUNC );
  }

  gsl_vector_set( amps, 0, nra - amp - qNSLMPeak );
  gsl_vector_set( amps, 1, - aDot - qNSLMDot );
  gsl_vector_set( amps, 2, nraDDot - aDDot - qNSLMDDot );

  /* We have now set up all the stuff to calculate the a coefficients */
  /* So let us do it! */
  gsl_linalg_LU_decomp( qMatrix, perm1, &signum );
  gsl_linalg_LU_solve( qMatrix, perm1, amps, aCoeff );

  /* Now we (should) have calculated the a values. Now we can do the b values */

  /* Populate the P matrix in Eq. 18 of the LIGO DCC document T1100433v2 */
  /* P3 */
  gsl_spline_init( spline, timeVec->data, p3->data, p3->length );
  gsl_interp_accel_reset( acc );
  gsl_matrix_set( pMatrix, 0, 0, - gsl_spline_eval_deriv( spline, nrTimePeak, acc ) );
  gsl_matrix_set( pMatrix, 1, 0, - gsl_spline_eval_deriv2( spline, nrTimePeak, acc ) );

  /* P4 */
  gsl_spline_init( spline, timeVec->data, p4->data, p4->length );
  gsl_interp_accel_reset( acc );
  gsl_matrix_set( pMatrix, 0, 1, - gsl_spline_eval_deriv( spline, nrTimePeak, acc ) );
  gsl_matrix_set( pMatrix, 1, 1, - gsl_spline_eval_deriv2( spline, nrTimePeak, acc ) );

  /* Populate the r.h.s vector of Eq. 18 of the LIGO DCC document T1100433v2 */
  /* Phase */
  gsl_spline_init( spline, timeVec->data, phase->data, phase->length );
  gsl_interp_accel_reset( acc );
  omega    = gsl_spline_eval_deriv( spline, nrTimePeak, acc );
  omegaDot = gsl_spline_eval_deriv2( spline, nrTimePeak, acc );

  /* pNSLM */
  gsl_spline_init( spline, timeVec->data, pNS->data, pNS->length );
  gsl_interp_accel_reset( acc );
  pNSLMDot  = gsl_spline_eval_deriv( spline, nrTimePeak, acc );
  pNSLMDDot = gsl_spline_eval_deriv2( spline, nrTimePeak, acc );

  /* Since the phase can be decreasing, we need to take care not to have a -ve frequency */
  if ( omega * omegaDot > 0.0 )
  {
    omega    = fabs( omega );
    omegaDot = fabs( omegaDot );
  }
  else
  {
    omega    = fabs( omega );
    omegaDot = - fabs( omegaDot );
  }

  //nromega = GetNRPeakOmega( l, m, eta );
  //nromegaDot = GetNRPeakOmegaDot( l, m, eta );
  nromega = GetNRSpinPeakOmega( l, m, eta, a );
  nromegaDot = GetNRSpinPeakOmegaDot( l, m, eta, a );

  //printf("NR inputs: %.16e, %.16e, %.16e, %.16e\n",nra,nraDDot,nromega,nromegaDot);

  if ( XLAL_IS_REAL8_FAIL_NAN( nromega ) || XLAL_IS_REAL8_FAIL_NAN( nromegaDot ) )
  {
    XLALDestroyREAL8Vector( timeVec );
    XLALDestroyREAL8Vector( q3 );
    XLALDestroyREAL8Vector( q4 );
    XLALDestroyREAL8Vector( q5 );
    XLALDestroyREAL8Vector( p3 );
    XLALDestroyREAL8Vector( p4 );
    XLALDestroyREAL8Vector( qNS );
    XLALDestroyREAL8Vector( pNS );
    XLALDestroyREAL8Vector( q3LM );
    XLALDestroyREAL8Vector( q4LM );
    XLALDestroyREAL8Vector( q5LM );
    XLALDestroyREAL8Vector( qNSLM );
    fprintf(stderr,"Error(%d)",XLAL_EFUNC );
  }

  gsl_vector_set( omegaVec, 0, nromega - omega + pNSLMDot );
  gsl_vector_set( omegaVec, 1, nromegaDot - omegaDot + pNSLMDDot );

  /*printf( "P MATRIX\n" );
  for (unsigned int i = 0; i < 2; i++ )
  {
    for (unsigned int j = 0; j < 2; j++ )
    {
      printf( "%.12e\t", gsl_matrix_get( pMatrix, i, j ));
    }
    printf( "= %.12e\n", gsl_vector_get( omegaVec, i ) );
  }*/

  /* And now solve for the b coefficients */
  gsl_linalg_LU_decomp( pMatrix, perm2, &signum );
  gsl_linalg_LU_solve( pMatrix, perm2, omegaVec, bCoeff );

  /* We can now populate the coefficients structure */
/*  coeffs->a3S = gsl_vector_get( aCoeff, 0 );
  coeffs->a4  = gsl_vector_get( aCoeff, 1 );
  coeffs->a5  = gsl_vector_get( aCoeff, 2 );*/
  coeffs->b3  = gsl_vector_get( bCoeff, 0 );
  coeffs->b4  = gsl_vector_get( bCoeff, 1 );

  /*printf( "NQC coefficients:\n" );
  printf( "a1 = %.16e, a2 = %.16e, a3 = %.16e, a3s = %.16e, a4 = %.16e, a5 = %.16e\n",
    coeffs->a1, coeffs->a2, coeffs->a3, coeffs->a3S, coeffs->a4, coeffs->a5 );

  printf( "b1 = %.16e, b2 = %.16e, b3 = %.16e, b4 = %.16e\n",
    coeffs->b1, coeffs->b2, coeffs->b3, coeffs->b4 );*/

  /* Free memory and exit */
  gsl_matrix_free( qMatrix );
  gsl_vector_free( amps );
  gsl_vector_free( aCoeff );
  gsl_permutation_free( perm1 );

  gsl_matrix_free( pMatrix );
  gsl_vector_free( omegaVec );
  gsl_vector_free( bCoeff );
  gsl_permutation_free( perm2 );

  gsl_spline_free( spline );
  gsl_interp_accel_free( acc );

  XLALDestroyREAL8Vector( timeVec );
  XLALDestroyREAL8Vector( q3 );
  XLALDestroyREAL8Vector( q4 );
  XLALDestroyREAL8Vector( q5 );
  XLALDestroyREAL8Vector( p3 );
  XLALDestroyREAL8Vector( p4 );
  XLALDestroyREAL8Vector( qNS );
  XLALDestroyREAL8Vector( pNS );
  XLALDestroyREAL8Vector( q3LM );
  XLALDestroyREAL8Vector( q4LM );
  XLALDestroyREAL8Vector( q5LM );
  XLALDestroyREAL8Vector( qNSLM );

  return XLAL_SUCCESS;
}

 int  XLALSimIMREOBNonQCCorrection(
                      COMPLEX16      *nqc,    /**<< OUTPUT, The NQC correction */
                      REAL8Vector    *values, /**<< Dynamics r, phi, pr, pphi */
                      const REAL8               omega,  /**<< Angular frequency */
                      EOBNonQCCoeffs *coeffs  /**<< NQC coefficients */
                     )

{

  REAL8 rOmega, rOmegaSq;
  REAL8 r, p, sqrtR;

  REAL8 mag, phase;


  r = values->data[0];
  p = values->data[2];

  sqrtR = sqrt(r);

  rOmega = r * omega;
  rOmegaSq = rOmega*rOmega;

  /* In EOBNRv2, coeffs->a3S, coeffs->a4 and coeffs->a5 are set to zero */
  /* through XLALSimIMREOBGetCalibratedNQCCoeffs() */
  /* and XLALSimIMREOBCalculateNQCCoefficients() */
  mag = 1. + (p*p / rOmegaSq) * ( coeffs->a1
     + coeffs->a2 / r + ( coeffs->a3 + coeffs->a3S) / (r*sqrtR)
     + coeffs->a4 / (r*r) + coeffs->a5 / (r*r*sqrtR));

  phase = coeffs->b1 * p / rOmega + p*p*p/rOmega * ( coeffs->b2
     + coeffs->b3 / sqrtR + coeffs->b4 / r );

  *nqc = mag * cos(phase);
  *nqc += I * mag * sin(phase);

  return XLAL_SUCCESS;

}
/**
 * Function which calculates the value of the waveform, plus its
 * first and second derivatives, for the points which will be required
 * in the hybrid comb attachment of the ringdown.
 */
 INT4 XLALGenerateHybridWaveDerivatives (
	REAL8Vector	*rwave,      /**<< OUTPUT, values of the waveform at comb points */
	REAL8Vector	*dwave,      /**<< OUTPUT, 1st deriv of the waveform at comb points */
	REAL8Vector	*ddwave,     /**<< OUTPUT, 2nd deriv of the waveform at comb points */
        REAL8Vector	*timeVec,    /**<< Vector containing the time */
	REAL8Vector	*wave,       /**<< Last part of inspiral waveform */
	REAL8Vector	*matchrange, /**<< Times which determine the size of the comb */
        REAL8           dt,          /**<< Sample time step */
        REAL8           mass1,       /**<< First component mass (in Solar masses) */
        REAL8           mass2        /**<< Second component mass (in Solar masses) */
	)
{

  /* XLAL error handling */
  INT4 errcode = XLAL_SUCCESS;

  /* For checking GSL return codes */
  INT4 gslStatus;

  UINT4 j;
  UINT4 vecLength;
  REAL8 m;
  double *y;
  double ry, dy, dy2;
  double rt;
  double *tlist;
  gsl_interp_accel *acc;
  gsl_spline *spline;

  /* Total mass in geometric units */
  m  = (mass1 + mass2) * LAL_MTSUN_SI;

  tlist = (double *) malloc(6 * sizeof(double));
  rt = (matchrange->data[1] - matchrange->data[0]) / 5.;
  tlist[0] = matchrange->data[0];
  tlist[1] = tlist[0] + rt;
  tlist[2] = tlist[1] + rt;
  tlist[3] = tlist[2] + rt;
  tlist[4] = tlist[3] + rt;
  tlist[5] = matchrange->data[1];

  /* Set the length of the interpolation vectors */
  vecLength = (UINT4)( m * matchrange->data[2] / dt ) + 1;

  /* Getting interpolation and derivatives of the waveform using gsl spline routine */
  /* Initiate arrays and supporting variables for gsl */
  y = (double *) malloc(vecLength * sizeof(double));

  if ( !y )
  {
    fprintf(stderr,"Error!(%d)" ,XLAL_ENOMEM );
  }

  for (j = 0; j < vecLength; ++j)
  {
	y[j] = wave->data[j];
  }


  //XLAL_CALLGSL( acc = (gsl_interp_accel*) gsl_interp_accel_alloc() );
  acc = (gsl_interp_accel*) gsl_interp_accel_alloc();
  //XLAL_CALLGSL( spline = (gsl_spline*) gsl_spline_alloc(gsl_interp_cspline, vecLength) );
  spline = (gsl_spline*) gsl_spline_alloc(gsl_interp_cspline, vecLength);
  if ( !acc || !spline )
  {
    if ( acc )    gsl_interp_accel_free(acc);
    if ( spline ) gsl_spline_free(spline);
    LALFree( y );
    fprintf(stderr,"Error!(%d)" ,XLAL_ENOMEM );
  }

  /* Gall gsl spline interpolation */
  gslStatus = gsl_spline_init(spline, timeVec->data, y, vecLength);
  if ( gslStatus != GSL_SUCCESS )
  { 
    gsl_spline_free(spline);
    gsl_interp_accel_free(acc);
    LALFree( y );
	
    fprintf(stderr,"Error!(%d)" ,XLAL_EFUNC );
  }

  /* Getting first and second order time derivatives from gsl interpolations */
  for (j = 0; j < 6; ++j)
  {
    gslStatus = gsl_spline_eval_e( spline, tlist[j], acc, &ry );
    if ( gslStatus == GSL_SUCCESS )
    {
      gslStatus = gsl_spline_eval_deriv_e(spline, tlist[j], acc, &dy );
      gslStatus = gsl_spline_eval_deriv2_e(spline, tlist[j], acc, &dy2 );
    }
    if (gslStatus != GSL_SUCCESS )
    {
      gsl_spline_free(spline);
      gsl_interp_accel_free(acc);
      LALFree( y );
	  fprintf(stderr,"Error!(%d)" ,XLAL_EFUNC );
    }
    rwave->data[j]  = (REAL8)(ry);
    dwave->data[j]  = (REAL8)(dy/m);
    ddwave->data[j] = (REAL8)(dy2/m/m);

  }
  
  /* Free gsl variables */
  gsl_spline_free(spline);
  gsl_interp_accel_free(acc);
  LALFree( tlist );
  LALFree(y);

  return errcode;
}


/**
 * Computes the (s)Y(l,m) spin-weighted spherical harmonic.
 *
 * From somewhere ....
 *
 * See also:
 * Implements Equations (II.9)-(II.13) of
 * D. A. Brown, S. Fairhurst, B. Krishnan, R. A. Mercer, R. K. Kopparapu,
 * L. Santamaria, and J. T. Whelan,
 * "Data formats for numerical relativity waves",
 * arXiv:0709.0093v1 (2007).
 *
 * Currently only supports s=-2, l=2,3,4,5,6,7,8 modes.
 */
COMPLEX16 XLALSpinWeightedSphericalHarmonic(
                                   REAL8 theta,  /**< polar angle (rad) */
                                   REAL8 phi,    /**< azimuthal angle (rad) */
                                   int s,        /**< spin weight */
                                   int l,        /**< mode number l */
                                   int m         /**< mode number m */
    )
{
  REAL8 fac;
  COMPLEX16 ans;

  /* sanity checks ... */
  if ( l < abs(s) ) 
  {
    fprintf(stderr,"XLAL Error - : Invalid mode s=%d, l=%d, m=%d - require |s| <= l\n", s, l, m );
    //XLAL_ERROR_VAL(0, XLAL_EINVAL);
  }
  if ( l < abs(m) ) 
  {
    fprintf(stderr,"XLAL Error -: Invalid mode s=%d, l=%d, m=%d - require |m| <= l\n", s, l, m );
    //XLAL_ERROR_VAL(0, XLAL_EINVAL);
  }

  if ( s == -2 ) 
  {
    if ( l == 2 ) 
    {
      switch ( m ) 
      {
        case -2:
          fac = sqrt( 5.0 / ( 64.0 * LAL_PI ) ) * ( 1.0 - cos( theta ))*( 1.0 - cos( theta ));
          break;
        case -1:
          fac = sqrt( 5.0 / ( 16.0 * LAL_PI ) ) * sin( theta )*( 1.0 - cos( theta ));
          break;

        case 0:
          fac = sqrt( 15.0 / ( 32.0 * LAL_PI ) ) * sin( theta )*sin( theta );
          break;

        case 1:
          fac = sqrt( 5.0 / ( 16.0 * LAL_PI ) ) * sin( theta )*( 1.0 + cos( theta ));
          break;

        case 2:
          fac = sqrt( 5.0 / ( 64.0 * LAL_PI ) ) * ( 1.0 + cos( theta ))*( 1.0 + cos( theta ));
          break;
        default:
          fprintf(stderr,"XLAL Error -: Invalid mode s=%d, l=%d, m=%d - require |m| <= l\n", s, l, m );
          //XLAL_ERROR_VAL(0, XLAL_EINVAL);
          break;
      } /*  switch (m) */
    }  /* l==2*/
    else if ( l == 3 ) 
    {
      switch ( m ) 
      {
        case -3:
          fac = sqrt(21.0/(2.0*LAL_PI))*cos(theta/2.0)*pow(sin(theta/2.0),5.0);
          break;
        case -2:
          fac = sqrt(7.0/(4.0*LAL_PI))*(2.0 + 3.0*cos(theta))*pow(sin(theta/2.0),4.0);
          break;
        case -1:
          fac = sqrt(35.0/(2.0*LAL_PI))*(sin(theta) + 4.0*sin(2.0*theta) - 3.0*sin(3.0*theta))/32.0;
          break;
        case 0:
          fac = (sqrt(105.0/(2.0*LAL_PI))*cos(theta)*pow(sin(theta),2.0))/4.0;
          break;
        case 1:
          fac = -sqrt(35.0/(2.0*LAL_PI))*(sin(theta) - 4.0*sin(2.0*theta) - 3.0*sin(3.0*theta))/32.0;
          break;

        case 2:
          fac = sqrt(7.0/LAL_PI)*pow(cos(theta/2.0),4.0)*(-2.0 + 3.0*cos(theta))/2.0;
          break;

        case 3:
          fac = -sqrt(21.0/(2.0*LAL_PI))*pow(cos(theta/2.0),5.0)*sin(theta/2.0);
          break;

        default:
          fprintf(stderr,"XLAL Error -: Invalid mode s=%d, l=%d, m=%d - require |m| <= l\n", s, l, m );
          //XLAL_ERROR_VAL(0, XLAL_EINVAL);
          break;
      }
    }   /* l==3 */
    else if ( l == 4 ) 
    {
      switch ( m ) 
      {
        case -4:
          fac = 3.0*sqrt(7.0/LAL_PI)*pow(cos(theta/2.0),2.0)*pow(sin(theta/2.0),6.0);
          break;
        case -3:
          fac = 3.0*sqrt(7.0/(2.0*LAL_PI))*cos(theta/2.0)*(1.0 + 2.0*cos(theta))*pow(sin(theta/2.0),5.0);
          break;

        case -2:
          fac = (3.0*(9.0 + 14.0*cos(theta) + 7.0*cos(2.0*theta))*pow(sin(theta/2.0),4.0))/(4.0*sqrt(LAL_PI));
          break;
        case -1:
          fac = (3.0*(3.0*sin(theta) + 2.0*sin(2.0*theta) + 7.0*sin(3.0*theta) - 7.0*sin(4.0*theta)))/(32.0*sqrt(2.0*LAL_PI));
          break;
        case 0:
          fac = (3.0*sqrt(5.0/(2.0*LAL_PI))*(5.0 + 7.0*cos(2.0*theta))*pow(sin(theta),2.0))/16.0;
          break;
        case 1:
          fac = (3.0*(3.0*sin(theta) - 2.0*sin(2.0*theta) + 7.0*sin(3.0*theta) + 7.0*sin(4.0*theta)))/(32.0*sqrt(2.0*LAL_PI));
          break;
        case 2:
          fac = (3.0*pow(cos(theta/2.0),4.0)*(9.0 - 14.0*cos(theta) + 7.0*cos(2.0*theta)))/(4.0*sqrt(LAL_PI));
          break;
        case 3:
          fac = -3.0*sqrt(7.0/(2.0*LAL_PI))*pow(cos(theta/2.0),5.0)*(-1.0 + 2.0*cos(theta))*sin(theta/2.0);
          break;
        case 4:
          fac = 3.0*sqrt(7.0/LAL_PI)*pow(cos(theta/2.0),6.0)*pow(sin(theta/2.0),2.0);
          break;
        default:
          fprintf(stderr,"XLAL Error -: Invalid mode s=%d, l=%d, m=%d - require |m| <= l\n", s, l, m );
         // XLAL_ERROR_VAL(0, XLAL_EINVAL);
          break;
      }
    }    /* l==4 */
    else if ( l == 5 ) 
    {
      switch ( m ) 
      {
        case -5:
          fac = sqrt(330.0/LAL_PI)*pow(cos(theta/2.0),3.0)*pow(sin(theta/2.0),7.0);
          break;
        case -4:
          fac = sqrt(33.0/LAL_PI)*pow(cos(theta/2.0),2.0)*(2.0 + 5.0*cos(theta))*pow(sin(theta/2.0),6.0);
          break;
        case -3:
          fac = (sqrt(33.0/(2.0*LAL_PI))*cos(theta/2.0)*(17.0 + 24.0*cos(theta) + 15.0*cos(2.0*theta))*pow(sin(theta/2.0),5.0))/4.0;
          break;
        case -2:
          fac = (sqrt(11.0/LAL_PI)*(32.0 + 57.0*cos(theta) + 36.0*cos(2.0*theta) + 15.0*cos(3.0*theta))*pow(sin(theta/2.0),4.0))/8.0;
          break;
        case -1:
          fac = (sqrt(77.0/LAL_PI)*(2.0*sin(theta) + 8.0*sin(2.0*theta) + 3.0*sin(3.0*theta) + 12.0*sin(4.0*theta) - 15.0*sin(5.0*theta)))/256.0;
          break;
        case 0:
          fac = (sqrt(1155.0/(2.0*LAL_PI))*(5.0*cos(theta) + 3.0*cos(3.0*theta))*pow(sin(theta),2.0))/32.0;
          break;
        case 1:
          fac = sqrt(77.0/LAL_PI)*(-2.0*sin(theta) + 8.0*sin(2.0*theta) - 3.0*sin(3.0*theta) + 12.0*sin(4.0*theta) + 15.0*sin(5.0*theta))/256.0;
          break;
        case 2:
          fac = sqrt(11.0/LAL_PI)*pow(cos(theta/2.0),4.0)*(-32.0 + 57.0*cos(theta) - 36.0*cos(2.0*theta) + 15.0*cos(3.0*theta))/8.0;
          break;
        case 3:
          fac = -sqrt(33.0/(2.0*LAL_PI))*pow(cos(theta/2.0),5.0)*(17.0 - 24.0*cos(theta) + 15.0*cos(2.0*theta))*sin(theta/2.0)/4.0;
          break;
        case 4:
          fac = sqrt(33.0/LAL_PI)*pow(cos(theta/2.0),6.0)*(-2.0 + 5.0*cos(theta))*pow(sin(theta/2.0),2.0);
          break;
        case 5:
          fac = -sqrt(330.0/LAL_PI)*pow(cos(theta/2.0),7.0)*pow(sin(theta/2.0),3.0);
          break;
        default:
          fprintf(stderr,"XLAL Error - nvalid mode s=%d, l=%d, m=%d - require |m| <= l\n", s, l, m );
          //XLAL_ERROR_VAL(0, XLAL_EINVAL);
          break;
      }
    }  /* l==5 */
    else if ( l == 6 )
    {
      switch ( m )
      {
        case -6:
          fac = (3.*sqrt(715./LAL_PI)*pow(cos(theta/2.0),4)*pow(sin(theta/2.0),8))/2.0;
          break;
        case -5:
          fac = (sqrt(2145./LAL_PI)*pow(cos(theta/2.0),3)*(1. + 3.*cos(theta))*pow(sin(theta/2.0),7))/2.0;
          break;
        case -4:
          fac = (sqrt(195./(2.0*LAL_PI))*pow(cos(theta/2.0),2)*(35. + 44.*cos(theta) 
          + 33.*cos(2.*theta))*pow(sin(theta/2.0),6))/8.0;
          break;
        case -3:
          fac = (3.*sqrt(13./LAL_PI)*cos(theta/2.0)*(98. + 185.*cos(theta) + 110.*cos(2*theta) 
          + 55.*cos(3.*theta))*pow(sin(theta/2.0),5))/32.0;
          break;
        case -2:
          fac = (sqrt(13./LAL_PI)*(1709. + 3096.*cos(theta) + 2340.*cos(2.*theta) + 1320.*cos(3.*theta) 
          + 495.*cos(4.*theta))*pow(sin(theta/2.0),4))/256.0;
          break;
        case -1:
          fac = (sqrt(65./(2.0*LAL_PI))*cos(theta/2.0)*(161. + 252.*cos(theta) + 252.*cos(2.*theta) 
          + 132.*cos(3.*theta) + 99.*cos(4.*theta))*pow(sin(theta/2.0),3))/64.0;
          break;
        case 0:
          fac = (sqrt(1365./LAL_PI)*(35. + 60.*cos(2.*theta) + 33.*cos(4.*theta))*pow(sin(theta),2))/512.0;
          break;
        case 1:
          fac = (sqrt(65./(2.0*LAL_PI))*pow(cos(theta/2.0),3)*(161. - 252.*cos(theta) + 252.*cos(2.*theta) 
          - 132.*cos(3.*theta) + 99.*cos(4.*theta))*sin(theta/2.0))/64.0;
          break;
        case 2:
          fac = (sqrt(13./LAL_PI)*pow(cos(theta/2.0),4)*(1709. - 3096.*cos(theta) + 2340.*cos(2.*theta) 
          - 1320*cos(3*theta) + 495*cos(4*theta)))/256.0;
          break;
        case 3:
          fac = (-3.*sqrt(13./LAL_PI)*pow(cos(theta/2.0),5)*(-98. + 185.*cos(theta) - 110.*cos(2*theta) 
          + 55.*cos(3.*theta))*sin(theta/2.0))/32.0;
          break;
        case 4:
          fac = (sqrt(195./(2.0*LAL_PI))*pow(cos(theta/2.0),6)*(35. - 44.*cos(theta) 
          + 33.*cos(2*theta))*pow(sin(theta/2.0),2))/8.0;
          break;
        case 5:
          fac = -(sqrt(2145./LAL_PI)*pow(cos(theta/2.0),7)*(-1. + 3.*cos(theta))*pow(sin(theta/2.0),3))/2.0;
          break;
        case 6:
          fac = (3.*sqrt(715./LAL_PI)*pow(cos(theta/2.0),8)*pow(sin(theta/2.0),4))/2.0;
          break;
        default:
          fprintf(stderr,"XLAL Error : Invalid mode s=%d, l=%d, m=%d - require |m| <= l\n", s, l, m );
          //XLAL_ERROR_VAL(0, XLAL_EINVAL);
          break;
      }
    } /* l==6 */
    else if ( l == 7 )
    {
      switch ( m )
      {
        case -7:
          fac = sqrt(15015./(2.0*LAL_PI))*pow(cos(theta/2.0),5)*pow(sin(theta/2.0),9);
          break;
        case -6:
          fac = (sqrt(2145./LAL_PI)*pow(cos(theta/2.0),4)*(2. + 7.*cos(theta))*pow(sin(theta/2.0),8))/2.0;
          break;
        case -5:
          fac = (sqrt(165./(2.0*LAL_PI))*pow(cos(theta/2.0),3)*(93. + 104.*cos(theta) 
          + 91.*cos(2.*theta))*pow(sin(theta/2.0),7))/8.0;
          break;
        case -4:
          fac = (sqrt(165./(2.0*LAL_PI))*pow(cos(theta/2.0),2)*(140. + 285.*cos(theta) 
          + 156.*cos(2.*theta) + 91.*cos(3.*theta))*pow(sin(theta/2.0),6))/16.0;
          break;
        case -3:
          fac = (sqrt(15./(2.0*LAL_PI))*cos(theta/2.0)*(3115. + 5456.*cos(theta) + 4268.*cos(2.*theta) 
          + 2288.*cos(3.*theta) + 1001.*cos(4.*theta))*pow(sin(theta/2.0),5))/128.0;
          break;
        case -2:
          fac = (sqrt(15./LAL_PI)*(5220. + 9810.*cos(theta) + 7920.*cos(2.*theta) + 5445.*cos(3.*theta) 
          + 2860.*cos(4.*theta) + 1001.*cos(5.*theta))*pow(sin(theta/2.0),4))/512.0;
          break;
        case -1:
          fac = (3.*sqrt(5./(2.0*LAL_PI))*cos(theta/2.0)*(1890. + 4130.*cos(theta) + 3080.*cos(2.*theta) 
          + 2805.*cos(3.*theta) + 1430.*cos(4.*theta) + 1001.*cos(5*theta))*pow(sin(theta/2.0),3))/512.0;
          break;
        case 0:
          fac = (3.*sqrt(35./LAL_PI)*cos(theta)*(109. + 132.*cos(2.*theta) 
          + 143.*cos(4.*theta))*pow(sin(theta),2))/512.0;
          break;
        case 1:
          fac = (3.*sqrt(5./(2.0*LAL_PI))*pow(cos(theta/2.0),3)*(-1890. + 4130.*cos(theta) - 3080.*cos(2.*theta) 
          + 2805.*cos(3.*theta) - 1430.*cos(4.*theta) + 1001.*cos(5.*theta))*sin(theta/2.0))/512.0;
          break;
        case 2:
          fac = (sqrt(15./LAL_PI)*pow(cos(theta/2.0),4)*(-5220. + 9810.*cos(theta) - 7920.*cos(2.*theta) 
          + 5445.*cos(3.*theta) - 2860.*cos(4.*theta) + 1001.*cos(5.*theta)))/512.0;
          break;
        case 3:
          fac = -(sqrt(15./(2.0*LAL_PI))*pow(cos(theta/2.0),5)*(3115. - 5456.*cos(theta) + 4268.*cos(2.*theta) 
          - 2288.*cos(3.*theta) + 1001.*cos(4.*theta))*sin(theta/2.0))/128.0;
          break;  
        case 4:
          fac = (sqrt(165./(2.0*LAL_PI))*pow(cos(theta/2.0),6)*(-140. + 285.*cos(theta) - 156.*cos(2*theta) 
          + 91.*cos(3.*theta))*pow(sin(theta/2.0),2))/16.0;
          break;
        case 5:
          fac = -(sqrt(165./(2.0*LAL_PI))*pow(cos(theta/2.0),7)*(93. - 104.*cos(theta) 
          + 91.*cos(2.*theta))*pow(sin(theta/2.0),3))/8.0;
          break;
        case 6:
          fac = (sqrt(2145./LAL_PI)*pow(cos(theta/2.0),8)*(-2. + 7.*cos(theta))*pow(sin(theta/2.0),4))/2.0;
          break;
        case 7:
          fac = -(sqrt(15015./(2.0*LAL_PI))*pow(cos(theta/2.0),9)*pow(sin(theta/2.0),5));
          break;
        default:
          fprintf(stderr,"XLAL Error : Invalid mode s=%d, l=%d, m=%d - require |m| <= l\n", s, l, m );
          //XLAL_ERROR_VAL(0, XLAL_EINVAL);
          break;
      }
    } /* l==7 */
    else if ( l == 8 )
    {
      switch ( m )
      {
        case -8:
          fac = sqrt(34034./LAL_PI)*pow(cos(theta/2.0),6)*pow(sin(theta/2.0),10);
          break;
        case -7:
          fac = sqrt(17017./(2.0*LAL_PI))*pow(cos(theta/2.0),5)*(1. + 4.*cos(theta))*pow(sin(theta/2.0),9);
          break;
        case -6:
          fac = sqrt(255255./LAL_PI)*pow(cos(theta/2.0),4)*(1. + 2.*cos(theta))
          *sin(LAL_PI/4.0 - theta/2.0)*sin(LAL_PI/4.0 + theta/2.0)*pow(sin(theta/2.0),8);
          break;
        case -5:
          fac = (sqrt(12155./(2.0*LAL_PI))*pow(cos(theta/2.0),3)*(19. + 42.*cos(theta) 
          + 21.*cos(2.*theta) + 14.*cos(3.*theta))*pow(sin(theta/2.0),7))/8.0;
          break;
        case -4:
          fac = (sqrt(935./(2.0*LAL_PI))*pow(cos(theta/2.0),2)*(265. + 442.*cos(theta) + 364.*cos(2.*theta) 
          + 182.*cos(3.*theta) + 91.*cos(4.*theta))*pow(sin(theta/2.0),6))/32.0;
          break;
        case -3:
          fac = (sqrt(561./(2.0*LAL_PI))*cos(theta/2.0)*(869. + 1660.*cos(theta) + 1300.*cos(2.*theta) 
          + 910.*cos(3.*theta) + 455.*cos(4.*theta) + 182.*cos(5.*theta))*pow(sin(theta/2.0),5))/128.0;
          break;
        case -2:
          fac = (sqrt(17./LAL_PI)*(7626. + 14454.*cos(theta) + 12375.*cos(2.*theta) + 9295.*cos(3.*theta) 
          + 6006.*cos(4.*theta) + 3003.*cos(5.*theta) + 1001.*cos(6.*theta))*pow(sin(theta/2.0),4))/512.0;
          break;
        case -1:
          fac = (sqrt(595./(2.0*LAL_PI))*cos(theta/2.0)*(798. + 1386.*cos(theta) + 1386.*cos(2.*theta) 
          + 1001.*cos(3.*theta) + 858.*cos(4.*theta) + 429.*cos(5.*theta) + 286.*cos(6.*theta))*pow(sin(theta/2.0),3))/512.0;
          break;
        case 0:
          fac = (3.*sqrt(595./LAL_PI)*(210. + 385.*cos(2.*theta) + 286.*cos(4.*theta) 
          + 143.*cos(6.*theta))*pow(sin(theta),2))/4096.0;
          break;
        case 1:
          fac = (sqrt(595./(2.0*LAL_PI))*pow(cos(theta/2.0),3)*(798. - 1386.*cos(theta) + 1386.*cos(2.*theta) 
          - 1001.*cos(3.*theta) + 858.*cos(4.*theta) - 429.*cos(5.*theta) + 286.*cos(6.*theta))*sin(theta/2.0))/512.0;
          break;
        case 2:
          fac = (sqrt(17./LAL_PI)*pow(cos(theta/2.0),4)*(7626. - 14454.*cos(theta) + 12375.*cos(2.*theta) 
          - 9295.*cos(3.*theta) + 6006.*cos(4.*theta) - 3003.*cos(5.*theta) + 1001.*cos(6.*theta)))/512.0;
          break;
        case 3:
          fac = -(sqrt(561./(2.0*LAL_PI))*pow(cos(theta/2.0),5)*(-869. + 1660.*cos(theta) - 1300.*cos(2.*theta) 
          + 910.*cos(3.*theta) - 455.*cos(4.*theta) + 182.*cos(5.*theta))*sin(theta/2.0))/128.0;
          break;
        case 4:
          fac = (sqrt(935./(2.0*LAL_PI))*pow(cos(theta/2.0),6)*(265. - 442.*cos(theta) + 364.*cos(2.*theta) 
          - 182.*cos(3.*theta) + 91.*cos(4.*theta))*pow(sin(theta/2.0),2))/32.0;
          break;
        case 5:
          fac = -(sqrt(12155./(2.0*LAL_PI))*pow(cos(theta/2.0),7)*(-19. + 42.*cos(theta) - 21.*cos(2.*theta) 
          + 14.*cos(3.*theta))*pow(sin(theta/2.0),3))/8.0;
          break;
        case 6:
          fac = sqrt(255255./LAL_PI)*pow(cos(theta/2.0),8)*(-1. + 2.*cos(theta))*sin(LAL_PI/4.0 - theta/2.0)
          *sin(LAL_PI/4.0 + theta/2.0)*pow(sin(theta/2.0),4);
          break;
        case 7:
          fac = -(sqrt(17017./(2.0*LAL_PI))*pow(cos(theta/2.0),9)*(-1. + 4.*cos(theta))*pow(sin(theta/2.0),5));
          break;
        case 8:
          fac = sqrt(34034./LAL_PI)*pow(cos(theta/2.0),10)*pow(sin(theta/2.0),6);
          break;
        default:
          fprintf(stderr,"XLAL Error -: Invalid mode s=%d, l=%d, m=%d - require |m| <= l\n", s, l, m );
          //XLAL_ERROR_VAL(0, XLAL_EINVAL);
          break;
      }
    } /* l==8 */
    else 
    {
      fprintf(stderr,"XLAL Error -: Unsupported mode l=%d (only l in [2,8] implemented)\n", l);
      //XLAL_ERROR_VAL(0, XLAL_EINVAL);
    }
  }
  else 
  {
    fprintf(stderr,"XLAL Error -: Unsupported mode s=%d (only s=-2 implemented)\n", s);
    //XLAL_ERROR_VAL(0, XLAL_EINVAL);
  }
  if (m)
  	
    ans = CX16polar(1.0, m*phi) * fac;
  else
    ans = fac;
  return ans;
}
/**
 * Function to calculate the non-Keplerian coefficient for the spin-aligned EOB model.
 * radius \f$r\f$ times the cuberoot of the returned number is \f$r_\Omega\f$ defined in Eq. A2.
 * i.e. the function returns \f$(r_{\Omega} / r)^3\f$.
 */
 REAL8
XLALSimIMRSpinAlignedEOBNonKeplerCoeff(
                      const REAL8           values[],   /**<< Dynamical variables */
                      SpinEOBParams         *funcParams /**<< EOB parameters */
                      )
{

  REAL8 omegaCirc;

  REAL8 tmpValues[4];

  REAL8 r3;

  /* We need to find the values of omega assuming pr = 0 */
  memcpy( tmpValues, values, sizeof(tmpValues) );
  tmpValues[2] = 0.0;

  omegaCirc = XLALSimIMRSpinAlignedEOBCalcOmega( tmpValues, funcParams );
  if ( XLAL_IS_REAL8_FAIL_NAN( omegaCirc ) )
  {
    fprintf(stderr,"Error(%d)", XLAL_EFUNC );
  }

  r3 = values[0]*values[0]*values[0];

  return 1.0/(omegaCirc*omegaCirc*r3);
}

REAL8 GetNRSpinPeakAmplitude( INT4 l, INT4 m, REAL8 eta, REAL8 a )
{
  /* Fit for HOMs missing */
  return 1.3547468629743946*eta + 0.9187885481024214*eta*eta;
}

REAL8 GetNRSpinPeakADDot( INT4 l, INT4 m, REAL8 eta, REAL8 a )
{
  /* Fit for HOMs missing */
  return eta*(-0.0024971911410897156 + (-0.006128515435641139 + 0.01732656*a/(2.0-4.0*eta))*eta);
}

COMPLEX16 XLALCOMPLEX16Rect (REAL8 x, REAL8 y)
{
  COMPLEX16 z = x + I*y;
  //do {(&z)->re=(x); (&z)->im=(y);} while(0);
  
  return z;
}

double MYlog2(double x)
{
	return log(x)/log(2.0);
}
double MYcbrt(double x)
{
	return pow(x,1.0/3.0);
}

double carg (COMPLEX16 z)
{
  double x = creal(z);
  double y = cimag(z);

  if (x == 0.0 && y == 0.0)
    {
      return 0;
    }

  return atan2 (y, x);
}
COMPLEX16 cexp (COMPLEX16 a)
{
  double rho = exp(creal(a));
  double theta = cimag(a);

  COMPLEX16 z = rho * cos (theta)+I*rho * sin (theta);
  return z;
}

COMPLEX16 CX16polar(double r,double phi)
{
	double re,im;
	re = r*cos(phi);
	im = r*sin(phi);

	
	COMPLEX16 z=re+I*im;

	return z;
}

COMPLEX16 MYcpow(COMPLEX16 a,UINT4 n)
{
	COMPLEX16 z1 = 1;
	UINT4 i;
	for(i = 0;i < n;i++)
	{
		z1 = z1*a;
	}

	return z1;
}

double cabs(COMPLEX16 z)
{
	double a = creal(z);
	double b = cimag(z);

	return sqrt(a*a+b*b);
}
REAL8 CalculateCrossProduct( const int i, const REAL8 a[], const REAL8 b[] )
{
  return a[(i+1)%3]*b[(i+2)%3] - a[(i+2)%3]*b[(i+1)%3];
}
int
NormalizeVector( REAL8 a[] )
{
  REAL8 norm = sqrt( a[0]*a[0] + a[1]*a[1] + a[2]*a[2] );

  a[0] /= norm;
  a[1] /= norm;
  a[2] /= norm;

  return 0;
}
#if 1
REAL8 XLALSimIMREOBGetNRSpinPeakDeltaT( 
                 INT4 l,           /**<< Mode l */
                 INT4 m,           /**<< Mode m */
                 REAL8 eta, /**<< Symmetric mass ratio */
                 REAL8 a           /**<< Dimensionless spin */
                 )
{

  switch ( l )
  {
    case 2:
      switch ( m )
      {
        case 2:
          /* DeltaT22 defined here is a minus sign different from Eq. (33) of Taracchini et al. */
          if ( a <= 0.0 )
          {
            return 2.5;
          }
          else
          {
            return (2.5 + 1.77*a*a*a*a/(0.43655*0.43655*0.43655*0.43655)/(1.0-2.0*eta)/(1.0-2.0*eta)/(1.0-2.0*eta)/(1.0-2.0*eta));
          }
          break;
        default:
          fprintf(stderr,"Error(%d)", XLAL_EINVAL );
      }
      break;
    default:
		fprintf(stderr,"Error(%d)", XLAL_EINVAL );
  }

  /* We should never get here, but I expect a compiler whinge without it... */
  fprintf( stderr,"XLAL Error - We should never get here!!\n" );
  fprintf(stderr,"Error(%d)", XLAL_EINVAL );
  return 0;
}
REAL8 GetNRSpinPeakOmega( INT4 l, INT4  m, REAL8  eta, REAL8 a )
{
  /* Fit for HOMs missing */
  return 0.27581190323955274 + 0.19347381066059993*eta
       - 0.08898338208573725*log(1.0 - a/(1.0-2.0*eta))
       + eta*eta*(1.78832*(0.2690779744133912 + a/(2.0-4.0*eta))*(1.2056469070395925
       + a/(2.0-4.0*eta)) + 1.423734113371796*log(1.0 - a/(1.0-2.0*eta)));
}
#else
REAL8 XLALSimIMREOBGetNRSpinPeakDeltaT( 
                 int l,           /**<< Mode l */
                 int m,           /**<< Mode m */
                 REAL8 eta, /**<< Symmetric mass ratio */
                 REAL8 a           /**<< Dimensionless spin */
                 )
{

  switch ( l )
  {
    case 2:
      switch ( m )
      {
        case 2:
          /* DeltaT22 defined here is a minus sign different from Eq. (33) of Taracchini et al. */
          if ( a <= 0.0 )
          {
            return 2.5;
          }
          else
          {
            return (2.5 + 1.77*a*a*a*a/(0.43655*0.43655*0.43655*0.43655)/(1.0-2.0*eta)/(1.0-2.0*eta)/(1.0-2.0*eta)/(1.0-2.0*eta));
          }
          break;
        default:
          fprintf(stderr,"Error(%d)", XLAL_EINVAL );
      }
      break;
    default:
		fprintf(stderr,"Error(%d)", XLAL_EINVAL );
  }

  /* We should never get here, but I expect a compiler whinge without it... */
  fprintf( stderr,"XLAL Error - We should never get here!!\n" );
  fprintf(stderr,"Error(%d)", XLAL_EINVAL );
  return 0;
}
REAL8 GetNRSpinPeakOmega( int l, int  m, REAL8  eta, REAL8 a )
{
  /* Fit for HOMs missing */
  return 0.27581190323955274 + 0.19347381066059993*eta
       - 0.08898338208573725*log(1.0 - a/(1.0-2.0*eta))
       + eta*eta*(1.78832*(0.2690779744133912 + a/(2.0-4.0*eta))*(1.2056469070395925
       + a/(2.0-4.0*eta)) + 1.423734113371796*log(1.0 - a/(1.0-2.0*eta)));
}
#endif
REAL8 GetNRSpinPeakOmegaDot( INT4 l, INT4  m, REAL8 eta, REAL8 a )
{
  /* Fit for HOMs missing */
  return 0.006075014646800278 + 0.012040017219351778*eta
       + (0.0007353536801336875 + 0.0015592659912461832*a/(1.0-2.0*eta))*log(1.0-a/(1.0-2.0*eta))
       + eta*eta*(0.03575969677378844 + (-0.011765658882139 - 0.02494825585993893*a/(1.0-2.0*eta))
       * log(1.0 - a/(1.0-2.0*eta)));
}
