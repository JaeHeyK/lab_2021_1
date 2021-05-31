import numpy as np
import matplotlib.pyplot as plt
from sklearn import datasets
from sklearn.svm import SVC

class_num = 2
class_colors_linear = np.array(['brown', 'blue'])
class_colors_nonlinear = np.array(['red', 'yellow'])
random_state_linear = np.array([20, 30, 40])
factor_nonlinear = np.array([0.1])
misclass_cost_linear = np.array([10,1,0.1])
misclass_cost_nonlinear = np.array([10,0.1])

def create_blob_data(rand_state):
  # make_blobs()를 이용하여 Data와 그에 해당하는 class 값을 생성합니다.
  X, y = datasets.make_blobs(n_samples=100, centers=2, cluster_std=1.2, random_state=rand_state)

  return X, y

def create_circle_data(fc):
  # make_circles()를 이용하여 Data와 그에 해당하는 class 값을 생성합니다.
  X, y = datasets.make_circles(n_samples=100, factor=fc, noise=0.1)
  return X,y

def plot_2D_data(ax, X, y, colors):
  # 주어진 subplot ax에 각각의 class마다 다른 색깔로 산점도를 표시합니다.
  for k, clr in zip(range(class_num), colors):
    members = (y == k)
    ax.scatter(X[members,0], X[members,1], s=10, facecolor=clr)
  
def plot_3D_data(ax, X, y, colors):
  rbf = np.exp(-(X**2).sum(1))
  for k, clr in zip(range(class_num), colors):
    members = (y == k)
    ax.scatter(X[members,0], X[members,1], rbf[members], s=10, facecolor=clr)

def plot_2D_classifier(ax, clf, sv=1):
  # decision boundary와 margin을 표시하기 위해 meshgrid를 생성한 후 contour를 그려야합니다.
  # plot의 x,y 축의 최대, 최소값을 구합니다.
  xlim = ax.get_xlim()
  ylim = ax.get_ylim()
  # 구한 최대/최소값을 이용해 x,y축 상의 linear space 배열을 구하고,
  # 이를 이용하여 x,y 각각의 meshgrid 값을 구합니다.
  xcords = np.linspace(xlim[0],xlim[1],50)
  ycords = np.linspace(ylim[0],ylim[1],50)
  xmeshgrid, ymeshgrid = np.meshgrid(xcords, ycords)

  # x,y meshgrid 값을 1차원 배열로 나열한 뒤 합쳐 (x,y) 좌표계를 나타내는 행렬을 만듭니다.
  xymeshgrid = np.r_[[xmeshgrid.reshape(-1)],[ymeshgrid.reshape(-1)]].T
  
  # 위에서 만든 좌표 행렬을 classifier의 decision function에 대입하여, 해당 결과값을 z에 저장합니다.
  z = clf.decision_function(xymeshgrid).reshape(xmeshgrid.shape)

  # 저장한 z값을 contour 함수에 전달하여 boundary를 그리되, levels에 [-1,0,1]의 배열을 전달하여
  # negative support vector, positive support vector를 각각 지나는 직선과
  # decision boundary를 모두 그려줍니다.
  ax.contour(xmeshgrid, ymeshgrid, z, levels=[-1, 0, 1], linestyles=['--','-','--'], colors='k')

  # 또한 sv 인자값이 1일 경우, 모든 support vector에 대해서도 marker를 추가로 표시해줍니다.
  if (sv==1):
    ax.scatter(clf.support_vectors_[:,0],clf.support_vectors_[:,1], s=100, facecolors='none', edgecolors='k')
  

def linear_svm_classification(rand_states, misclassCost):
  # 1x3 크기의 subplot array ax1을 생성합니다.
  fig1, ax1= plt.subplots(1, rand_states.shape[0], figsize=(10,3))
  fig1.tight_layout(pad=3.0)

  # 주어진 random_state 값들에 대해서 각각 blob 데이터를 계산하고 표시합니다.
  for i, rs in enumerate(rand_states):
    X, y = create_blob_data(rs)
    ax1[i].set_title(f"random_state={rs}")
    plot_2D_data(ax1[i], X, y, class_colors_linear)
  plt.show()

  # 3x3 크기의 subplot array ax2를 생성합니다.
  fig2, ax2 = plt.subplots(rand_states.shape[0], misclassCost.shape[0], squeeze=False, figsize=(10,10))
  fig2.tight_layout(pad=3.0)
  for i, rs in enumerate(rand_states):
    # 각 random_state마다 3가지 misclassfication cost를 갖는 classifier를 training시킵니다.
    # 우선 데이터를 생성합니다.
    X, y = create_blob_data(rs)
    for j, mc in enumerate(misclassCost):
      # 각 misclassfication cost에 해당하는 classfier를 생성합니다.
      clf = SVC(kernel='linear', C=mc)
      clf.fit(X,y)

      # ax에 데이터를 그린 후, classfier와 support vector들도 차례로 그립니다.
      ax2[i,j].set_title(f"random_state={rs}, C={mc:.1f}")
      plot_2D_data(ax2[i,j], X, y, class_colors_linear)
      plot_2D_classifier(ax2[i,j], clf)
  plt.show()

def nonlinear_svm_classification(factors, misclassCost):
  # 1x1 크기의 subplot array ax1을 생성합니다.
  fig1, ax1 = plt.subplots(1,factors.shape[0], figsize=(5,5))

  # 주어진 factor 값에 대해서 circle 데이터를 계산하고 표시합니다.
  for i, factor in enumerate(factors):
    X, y = create_circle_data(factor)
    ax1.set_title(f"factor={factor}")
    plot_2D_data(ax1,X,y, class_colors_nonlinear)
  plt.show()
  
  # 1x1 크기의 3d subplot을 생성합니다.
  fig2, ax2 = plt.subplots(1,factors.shape[0], figsize=(5,5))
  ax2 = plt.subplot(projection='3d')
  # 계산한 circle data에 대해 gausian RBF를 적용하여 3번째 축의 값을 구하고, 이를 이용하여 3차원 산점도를 그립니다.
  for i, factor in enumerate(factors):
    X, y = create_circle_data(factor)
    ax2.set_title(f"factor={factor}")
    plot_3D_data(ax2,X,y, class_colors_nonlinear)
  plt.show()

  fig3, ax3 = plt.subplots(factors.shape[0], misclassCost.shape[0], squeeze=False, figsize=(10,5))
  fig3.tight_layout(pad=3.0)
  for i, fc in enumerate(factors):
    # 2가지 misclassfication cost를 갖는 classifier를 training시킵니다.
    # 우선 데이터를 생성합니다.
    X, y = create_circle_data(fc)
    for j, mc in enumerate(misclassCost):
      # 각 misclassfication cost에 해당하는 classfier를 생성합니다.
      clf = SVC(kernel='rbf', C=mc)
      clf.fit(X,y)
      # ax에 데이터를 그린 후, classfier와 support vector들도 차례로 그립니다.
      ax3[i,j].set_title(f"factor={fc}, C={mc:.1f}")
      plot_2D_data(ax3[i,j], X, y, class_colors_nonlinear)
      plot_2D_classifier(ax3[i,j], clf, sv=0)
  plt.show()


def main():
  linear_svm_classification(random_state_linear, misclass_cost_linear)
  nonlinear_svm_classification(factor_nonlinear, misclass_cost_nonlinear)

main()