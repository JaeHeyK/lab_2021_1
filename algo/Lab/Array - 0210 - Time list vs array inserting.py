
import array as arr
import time



Ln = [10000, 20000, 40000, 80000, 160000, 320000]

print("\n Inserting back( ) in array) ")
for N in Ln :
    tbegin= time.process_time()
    da = arr.array('d', [])
    for w in range( N ) :
        da.append(w)
    tend = time.process_time()
    print( f'N={ N:7} elapsed= {tend-tbegin:10.7f} sec.')

print("\n Inserting front( ) in array) ")
for N in Ln :
    tbegin= time.process_time()
    da = arr.array('d', [])
    for w in range( N ) :
        da.insert(0,w)
    tend = time.process_time()
    print( f'N={ N:7} elapsed= {tend-tbegin:10.7f} sec.')

print("\n Inserting append( ) in list[]) ")
for N in Ln :
    tbegin= time.process_time()
    da = []
    for w in range( N ) :
        da.append(w)
    tend = time.process_time()
    print( f'N={ N:7} elapsed= {tend-tbegin:10.7f} sec.')

print("\n Inserting front( ) in list[]) ")
for N in Ln :
    tbegin= time.process_time()
    da = []
    for w in range( N ) :
        da.insert(0,w)
    tend = time.process_time()
    print( f'N={ N:7} elapsed= {tend-tbegin:10.7f} sec.')