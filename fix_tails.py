import numpy as np
import h5py

G_file = "G0_omega_output"
G_h5 = h5py.File(G_file)

num_ks = G_h5['/G0/mesh/2/points'].value.shape[0]

for i in range(1,4):
	print("Tail ",i)
	tail = G_h5["/G0/tail/"+str(i)].value
	print("Old tail\n",tail)
	tail_rs = tail.reshape((num_ks,2,2))
	print("New tail\n",tail_rs)
	G_h5.__delitem__("/G0/tail/"+str(i))
	G_h5.__setitem__("/G0/tail/"+str(i), tail_rs)
