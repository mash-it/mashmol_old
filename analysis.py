import sys
import numpy as np
from CafeMolAnalysis import DcdFile

dcd = DcdFile("test.dcd")

for i in range(dcd.header['frames']):
	frame = dcd.read_frame(i)
	vij = frame[1] - frame[0]
	vkj = frame[1] - frame[2]

	if sys.argv[1] == 'bond':
		print np.linalg.norm(vij), np.linalg.norm(vkj)
	if sys.argv[1] == 'angle':
		S = np.sum(vij * vkj) # dot prod
		D = np.linalg.norm(vij) * np.linalg.norm(vkj)
		print np.arccos(S/D) * 180 / np.pi

