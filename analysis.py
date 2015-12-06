import sys
import numpy as np
from CafeMolAnalysis import DcdFile

dcd = DcdFile("test.dcd")

i = 0
j = i+1
k = i+2

for t in range(dcd.header['frames']):
	frame = dcd.read_frame(t)
	vij = frame[j] - frame[i]
	vkj = frame[j] - frame[k]

	if sys.argv[1] == 'bond':
		print(np.linalg.norm(vij), np.linalg.norm(vkj))
	if sys.argv[1] == 'angle':
		S = np.sum(vij * vkj) # dot prod
		D = np.linalg.norm(vij) * np.linalg.norm(vkj)
		print(np.arccos(S/D) * 180 / np.pi)

