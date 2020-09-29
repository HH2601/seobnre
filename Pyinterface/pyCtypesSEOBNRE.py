import numpy as np
from ctypes import *

import matplotlib as mpl
mpl.use('Agg')
import matplotlib.pyplot as plt

fig_width_pt = 3*246.0  # Get this from LaTeX using \showthe\columnwidth
inches_per_pt = 1.0/72.27               # Convert pt to inch
golden_mean = (np.sqrt(5)-1.0)/2.0         # Aesthetic ratio
fig_width = fig_width_pt*inches_per_pt  # width in inches
fig_height = fig_width*golden_mean      # height in inches
fig_size =  [fig_width,fig_height]

params = { 'axes.labelsize': 24,
          'font.family': 'serif',
          'font.serif': 'Computer Modern Raman',
          'font.size': 24,
          'legend.fontsize': 20,
          'xtick.labelsize': 24,
          'ytick.labelsize': 24,
          'axes.grid' : True,
          'text.usetex': True,
          'savefig.dpi' : 100,
          'lines.markersize' : 14,
          'figure.figsize': fig_size}

mpl.rcParams.update(params)

lib = CDLL('./libSEOBNREv1.so')

wfgen = lib.genwaveform
wfgen.argtypes = [POINTER(c_double), POINTER(c_double),POINTER(c_double),POINTER(c_int),c_double, c_double,c_double, c_double,c_double, c_double,c_double, c_double,c_double, c_double]


'''
The waveform parameters: 
phiC: coalescence phase
deltaT: time step
m1, m2: component mass in unit M_sun
e0: eccentricity
r: source luminosity distance in unit Mpc
inc: inclination angle
spin1z, spin2z: dimensionless spin in z direction (can't exceed 0.6)
'''
phiC = 0
deltaT = 1./4096.
m1 = 10.
m2 = 10.
fMin = 20.
e0 = 0
r = 400.
inc = 0
spin1z = 0
spin2z = 0


# Initilization
arraysize = 100000

hplus = np.zeros(arraysize)
hcross = np.zeros(arraysize)
truesize = np.array([0])
t0 = np.array([1.])

# Generate the waveform
wfgen(hplus.ctypes.data_as(POINTER(c_double)),hcross.ctypes.data_as(POINTER(c_double)),t0.ctypes.data_as(POINTER(c_double)),truesize.ctypes.data_as(POINTER(c_int)),phiC,deltaT,m1,m2,spin1z,spin2z,fMin,e0,r,inc)

hplus = hplus[:truesize[0]]
hcross = hcross[:truesize[0]]
time = np.arange(t0[0],t0[0]+deltaT*truesize[0], deltaT)

# Plot the waveform
fig = plt.figure();
ax = fig.add_subplot(111);
ax.plot(time,hplus,label=r'$h_+$')
ax.plot(time,hcross,label=r'$h_\times$')
ax.legend()
ax.set_xlabel('Time [s]')
ax.set_ylabel('Strain')
fig.savefig('eccentricity.png',bbox_inches='tight')
