import SEOBNRE
from pycbc.waveform import get_td_waveform
import numpy as np
import lal
import lalsimulation as lalsim
import pylab


longAscNodes = 0
eccentricity = 0 
meanPerAno = 0
approx=lalsim.SEOBNRv1
nonGRdict = lal.CreateDict()
m1 = 30
m2 = 30
s1 = [0,0,0]#[0.4,-0.2,0.43]
s2 = [0,0,0]#[-0.1,0.8,0]
dist = 400.
#iota = np.pi*0.4
iota = 0.
phiRef = 0
deltaT = 1./4096/4
f_ref = 0
f_low = 30


for apx in ['SEOBNRE']:
    hp, hc = get_td_waveform(approximant = apx,
      coa_phase=np.pi/3,
    delta_t=1/4096,
    mass1=100,
    mass2=100,
    spin1z=0.0,
    spin2z=0.0,
    f_lower=20, 
    eccentricity=0.0,
    distance=1.0,
    inclination=0.0,
    long_asc_nodes=0.0)

    pylab.plot(hp.sample_times, hp, label='hplus:'+apx)


'''
hpt1, hct1 = lalsim.SimInspiralChooseTDWaveform(m1 * lal.MSUN_SI, m2 * lal.MSUN_SI,\
                                                          s1[0], s1[1], s1[2],\
                                                          s2[0], s2[1], s2[2],\
                                                          dist * 1e6 * lal.PC_SI, iota, phiRef,\
                                                          longAscNodes, eccentricity, meanPerAno,
                                                          deltaT, f_low, f_ref,\
                                                          nonGRdict, approx)

t1 = np.arange(hpt1.data.length, dtype=float) * hpt1.deltaT
t1 = t1 + hpt1.epoch
pylab.plot(t1, hpt1.data.data, label='hplus:SEOBNRv4,lal')
'''
pylab.ylabel('Strain')
pylab.xlabel('Time (s)')
pylab.legend()
pylab.show()