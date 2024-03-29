import sys
import numpy as np
import matplotlib.pyplot as plt
from CafeMolAnalysis import DcdFile

dcd = DcdFile("output.dcd")

i = 0
j = i+1
k = i+2

T = range(dcd.header['frames'])
A = []

for t in T:
	frame = dcd.read_frame(t)
	vij = frame[j] - frame[i]
	vkj = frame[j] - frame[k]

	if sys.argv[1] == 'bond':
		#print(np.linalg.norm(vij), np.linalg.norm(vkj))
		pass
	if sys.argv[1] == 'angle':
		plt.ylim([0,180])
		S = np.sum(vij * vkj) # dot prod
		D = np.linalg.norm(vij) * np.linalg.norm(vkj)
		A.append(np.arccos(S/D) * 180 / np.pi)

plt.plot(T, A)
plt.grid()
plt.show()

