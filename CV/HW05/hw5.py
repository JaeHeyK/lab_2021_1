import numpy as np
import matplotlib.pyplot as plt
from scipy.ndimage.filters import gaussian_filter, convolve
import scipy

# img1 = plt.imread('./data/warrior_a.jpg')
# img2 = plt.imread('./data/warrior_b.jpg')
# cor1 = np.load("./data/warrior_a.npy")
# cor2 = np.load("./data/warrior_b.npy")
img1 = plt.imread('./data/graffiti_a.jpg')
img2 = plt.imread('./data/graffiti_b.jpg')
cor1 = np.load("./data/graffiti_a.npy")
cor2 = np.load("./data/graffiti_b.npy")

def compute_fundamental(x1,x2):
    n = x1.shape[1]
    if x2.shape[1] != n:
        exit(1)
        
    F = None
    ### YOUR CODE BEGINS HERE
    
    # A 행렬을 만들기 위해 x1과 x2를 변형한 후 각 요소마다 곱해줍니다.
    # x1 = [[u1, u2, u3, ... un], [v1, v2, v3, ..., vn], [1, 1, 1, ..., 1]] 에서
    # x1' = [[u1, v1, 1, u1, v1, 1, u1, v1, 1],... [un, vn, 1, un, vn, 1, un, vn, 1]]를 만들고,
    # x2 = [[u1', u2', u3', ... un'], [v1', v2', v3', ..., vn'], [1, 1, 1, ..., 1]]에서
    # x2' = [[u1', u1', u1', v1', v1', v1', 1, 1, 1], ...[un', un', un', vn', vn', vn', 1, 1, 1]]을 만들어서
    # x1'과 x2'의 요소를 각각 곱하면 원하는 A행렬을 구할 수 있습니다.
    A = (np.tile(x1, (3,1)).T) * (np.repeat(x2, 3, axis=0).T)

    # A.T @ A의 고유벡터 중 최소의 고유벡터를 얻기 위해 특이값 분해를 한 후,
    # As의 최소값(최소 고유값)에 해당하는 고유벡터를 얻어냅니다.
    # 이 벡터가 Fundamental matrix의 기반이 될 것입니다.
    Au,As,Avt = np.linalg.svd(A)
    Fp = Avt[As.argmin()]
    Fp = Fp.reshape((3,3))
    
    # Fp의 rank를 2로 만들기 위해 Fp에 대해 다시 특이값 분해를 합니다.
    u, s, vt = np.linalg.svd(Fp)

    # 이 중 Sigma 행렬의 맨 마지막 값을 0으로 만들어 F의 rank를 2로 만듭니다.
    s[-1] = 0

    # 이전에 얻은 u, vt, s에서 얻은 대각행렬을 곱하여 최종적으로 F를 구합니다.
    F = u @ np.diag(s) @ vt

    ### YOUR CODE ENDS HERE
    
    return F


def compute_norm_fundamental(x1,x2):
    n = x1.shape[1]
    if x2.shape[1] != n:
        exit(1)

    # normalize image coordinates
    x1 = x1 / x1[2]
    mean_1 = np.mean(x1[:2],axis=1)
    S1 = np.sqrt(2) / np.std(x1[:2])
    T1 = np.array([[S1,0,-S1*mean_1[0]],[0,S1,-S1*mean_1[1]],[0,0,1]])
    x1 = T1 @ x1
    
    x2 = x2 / x2[2]
    mean_2 = np.mean(x2[:2],axis=1)
    S2 = np.sqrt(2) / np.std(x2[:2])
    T2 = np.array([[S2,0,-S2*mean_2[0]],[0,S2,-S2*mean_2[1]],[0,0,1]])
    x2 = T2 @ x2

    # compute F with the normalized coordinates
    F = compute_fundamental(x1,x2)

    # reverse normalization
    F = T2.T @ F @ T1
    
    return F


def compute_epipoles(F):
    e1 = None
    e2 = None
    ### YOUR CODE BEGINS HERE
    
    # F에 대한 최소 고유벡터를 구합니다.
    # 이 벡터가 주어진 F @ e1 = 0을 만족하는 최소의 고유값을 갖는 벡터입니다.
    u1, s1, vt1 = np.linalg.svd(F);
    e1 = vt1[s1.argmin()]
    
    # e1을 normalize하여 image1 상의 점으로 나타냅니다.
    e1 = e1 / e1[2]

    # F.T에 대한 최소 고유벡터를 구합니다.
    # 이 벡터가 주어진 F.T @ e1 = 0을 만족하는 최소의 고유값을 갖는 벡터입니다.
    u2, s2, vt2 = np.linalg.svd(F.T);
    e2 = vt2[s2.argmin()]

    # e2을 normalize하여 image2 상의 점으로 나타냅니다.
    e2 = e2 / e2[2]

    ### YOUR CODE ENDS HERE
    
    return e1, e2


def draw_epipolar_lines(img1, img2, cor1, cor2):
    F = compute_norm_fundamental(cor1, cor2)

    e1, e2 = compute_epipoles(F)
    ### YOUR CODE BEGINS HERE

    # plot의 크기를 설정하고,
    # img1과 img2를 표시할 subplot을 추가합니다.
    fig = plt.figure(figsize=(24,12))
    ax1 = fig.add_subplot(1, 2, 1)
    ax2 = fig.add_subplot(1, 2, 2)
    
    # 각각의 이미지에 대해서 epipolar line을 그려줍니다.
    # 이미지를 가로지르는 직선을 그리기 위해 axline()을 사용하고,
    # epipolar line마다 다른 색깔을 부여하기 위해 np.random.rand(3,)로
    # 무작위의 RGB 값을 생성합니다.
    for point1 in cor1.T:
        ax1.axline(e1[:2],point1[0:2], color=(np.random.rand(3,)), lw=2)
    for point2 in cor2.T:
        ax2.axline(e2[:2],point2[0:2], color=np.random.rand(3,), lw=2)
    
    # 위에서 생성한 plot과 이미지를 함께 보여줍니다.
    ax1.imshow(img1)
    ax2.imshow(img2)
    plt.show()


    ### YOUR CODE ENDS HERE

    return

draw_epipolar_lines(img1, img2, cor1, cor2)