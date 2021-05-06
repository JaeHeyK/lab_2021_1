
#In[1]:
import numpy as np
import cv2
import math
import random

#In[1]:
test = np.array([[1, 2, 3, 4], [5, 6, 7, 8], [9, 6, 5, 3]])
result = np.dot(test, np.array([1000, 100, 10, 1]))
result

# %%
test[:,2]=1
test
# %%
test = np.array([[1, 2, 3], [5, 6, 7], [9, 6, 5]])
A = np.array([[1, 2, 3], [5, 6, 7], [9, 6, 5]])
result = np.kron(test,A)
print(result)

# %%
test = np.array([[1, 2, 3, 4], [5, 6, 7, 8], [9, 6, 5, 3]])
print(np.repeat(np.repeat(test,2, axis=0), 3, axis=1))

# %%
sample_src_points = np.array([[0,1],[1,0],[0,0],[1,1]])
sample_ref_points = np.array([[0,1],[1,0],[0,0],[1,1]])
# sample_ref_points = np.array([[-1,0],[0,-1],[-1,-1],[0,0]])


sample_src_modified = np.concatenate((sample_src_points, sample_src_points[:,1:]), axis=1)
sample_src_modified[:,2] = 1
sample_src_modified = np.repeat(sample_src_modified, 2, axis=0)
sample_src_modified = np.tile(sample_src_modified, 3)
print(f'1:\n{sample_src_modified}')

sample_ref_modified = np.repeat(sample_ref_points.reshape(-1,1),3, axis=1)*-1
sample_ref_modified[0::2,0] = 1
sample_ref_modified[1::2,0] = 0
sample_ref_modified[1::2,1] = 1
sample_ref_modified[0::2,1] = 0
sample_ref_modified = np.repeat(sample_ref_modified, 3, axis=1)

base = sample_ref_modified * sample_src_modified
print(f'base:\n{base}')
print(f'{np.dot(base.T, base)}')

# %%
w, v = np.linalg.eig(np.dot(base.T, base))

h_cand = v[np.argmin(w)].reshape((3,3))
h_cand = h_cand/h_cand[2,2]
print(f'h_cand:\n{h_cand}')

print(np.dot(base, v[np.argmin(w)]))

u,s,vh = np.linalg.svd(np.dot(base.T, base))

h_cand2 = vh[-1].reshape((3,3))
h_cand2 = h_cand2/h_cand2[2,2]
print(f'vh:\n{vh}')
print(f'h_cand2:\n{h_cand2}')
# %%
test = np.array([[1, 2, 3], [5, 6, 7], [9, 6, 5]])
print(test[-1])
# %%
