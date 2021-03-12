#-------------------------------------------------------------------------------
# Author:      Zoh, 알고리즘 Order를 보여주기 위하여 만들어 봅니데이.
# Created:     2021-03-05
#-------------------------------------------------------------------------------

"""
time.clock( )는 더 이상 사용되지 않음. 헐... 대신에
time.perf_counter() 또는 time.process_time() 함수를 써야 한다.
"""

import time

Ln= [10000, 20000, 40000, 80000, 160000, 320000] # 64만까지 하면 디짐.

print("List.append( )의 성능")
for N in Ln :

    ml= list() #[]

    tbegin = time.process_time()            # 시작!
    for w in range(N): ml.append(N-w-1)     # 리스트 뒤로부터  집어 넣기  N개 N초
    tend = time.process_time()              # 끝 땡!

    print( f'N={ N:7} elapsed= {tend-tbegin:10.7f} sec.')

print("\n List.insert_middle( )의 성능")

for N in Ln :

    ml= list() #[]

    tbegin = time.process_time()            # 시작!
    for w in range(N): ml.insert(int(N/2),N-w-1)     # 리스트 뒤로부터  집어 넣기  N개 N초
    tend = time.process_time()              # 끝 땡!

    print( f'N={ N:7} elapsed= {tend-tbegin:10.7f} sec.')

print("\n List.insert_front( )의 성능")

for N in Ln :

    ml= list() #[]

    tbegin = time.process_time()            # 시작!
    for w in range(N): ml.insert(0,N-w-1)     # 리스트 뒤로부터  집어 넣기  N개 N초
    tend = time.process_time()              # 끝 땡!

    print( f'N={ N:7} elapsed= {tend-tbegin:10.7f} sec.')


