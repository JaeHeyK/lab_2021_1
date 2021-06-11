import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from keras.utils.np_utils import to_categorical
from keras.models import Sequential
from keras.layers import Dense, Dropout, Flatten, Conv2D, MaxPool2D
from tensorflow.keras.datasets import cifar10

labels = ['airplane', 'automoblie', 'bird', 'cat', 'deer', 'dog', 'frog', 'horse', 'ship', 'truck']

def load_and_normalize():
  # 10x10 크기의 subplot array ax를 생성하고, (1,100) 크기로 변경합니다.
  fig, ax = plt.subplots(10,10, figsize=(15,15))
  ax =  ax.flatten()
  fig.tight_layout(pad=1.0)

  # CIFAR-10 dataset을 준비합니다.
  (X_train, y_train), (X_test, y_test) = cifar10.load_data()

  # [0, 50000) 사이의 100개의 무작위 index를 생성하고,
  # 이에 해당하는 이미지와 label을 새로운 array에 저장합니다.
  # 이 때 y_train은 (50000,1) 크기의 2차원 배열이므로
  # flatten() 함수를 이용하여 1차원 배열로 바꿉니다.
  rand_index = np.random.choice(range(50000), 100)
  rand_imgs = X_train[rand_index]
  rand_label_index = y_train[rand_index].flatten()

  # 위에서 선정한 이미지와 label을 각각의 subplot에 표시합니다.
  for i, (img, idx) in enumerate(zip(rand_imgs, rand_label_index)):
    ax[i].set_title(labels[idx], size=16)
    ax[i].imshow(img)
    ax[i].axis('off')
  plt.show()

  # train image를 255로 나누어 normalize하고, 위에서 얻은 train, test 데이터들과 함께 반환합니다.
  X_train = X_train/255.0
  
  return (X_train, y_train), (X_test, y_test)

def train_model(X_train, y_train):
  # 주어진 label index를 one-hot 방식으로 인코딩합니다.
  y_train_encoded = to_categorical(y_train, len(labels))

  # 문제에서 주어진 바와 같이 모델을 구성합니다.
  model = Sequential(
    [
      Conv2D(32, kernel_size=5, activation='relu', input_shape=(32,32,3)),
      Conv2D(64, 5, activation='relu'),
      MaxPool2D(),
      Conv2D(128, 5, activation='relu'),
      MaxPool2D(),
      Dropout(0.25),
      Flatten(),
      Dense(128, activation='relu'),
      Dropout(0.3),
      Dense(len(labels), activation='softmax')
    ]
  )

  # 모델을 컴파일하고, 그 내용을 summary 함수를 통해 나타냅니다.
  model.compile(loss='categorical_crossentropy', optimizer='adam', metrics=['accuracy'])
  model.summary()

  # 주어진 데이터 X_train을 통해 모델을 학습시키고, 그 과정을 train_history에 저장합니다.
  # 여기서 전체 X_train 중 validation_split만큼의 데이터는 validation set으로 사용됩니다.
  train_history = model.fit(x=X_train, y=y_train_encoded, batch_size=64, epochs=10, validation_split=0.2)

  return model, train_history

def evaluate_and_predict(model, hist, X_test, y_test):
  # model training의 기록을 보여줄 두 개의 ax를 생성합니다.
  # 왼쪽의 subplot은 accuracy를, 오른쪽의 subplot은 loss를 나타낼 것입니다.
  eval_fig, (acc_ax, loss_ax) = plt.subplots(1, 2, figsize=(8,4))

  # 각각의 subplot에 해당하는 training history를 hist에서 가져와 서로 다른 색깔로 표시합니다.
  # 이 중 접두어 'val_'이 붙은 그래프는 validation set에서의 모델의 정확도와 loss 값을 나타냅니다.
  acc_ax.plot(hist.history['accuracy'], 'blue', label='accuracy')
  acc_ax.plot(hist.history['val_accuracy'], 'orange', label='val_accuracy')
  acc_ax.legend(loc='upper left')

  loss_ax.plot(hist.history['loss'], 'blue', label='loss')
  loss_ax.plot(hist.history['val_loss'], 'orange', label='val_loss')
  loss_ax.legend(loc='upper right')

  plt.show();

  # X_test 이미지 데이터를 normalize한 뒤, prediction을 시작합니다.
  nX_test = X_test/255.0
  y_predict = np.argmax(model.predict(nX_test,batch_size=64), axis=-1)
  
  # predict된 이미지들을 표시할 3x4 크기의 subplot을 생성하고
  # predict_ax 배열을 1차원으로 변형합니다.
  predict_fig, predict_ax = plt.subplots(3, 4, figsize=(8,6))
  predict_ax = predict_ax.flatten()
  predict_fig.tight_layout(pad=1.0)

  # [0, 10000) 사이의 12개의 무작위 index를 생성하고, 이에 해당하는 이미지와 prediction, 실제 label을 각각 새로운 array에 저장합니다.
  # 이 때 y_test, y_predict는 2차원 배열이므로 1차원으로 변형해줍니다.
  rand_index = np.random.choice(range(10000), 12)
  rand_test_imgs = X_test[rand_index]
  rand_test_label_index = y_test[rand_index].flatten()
  rand_predict_label_index = y_predict[rand_index].flatten()

  # 위에서 선정한 이미지들에 대해 해당 prediction과 실제 label을 subplot에 함께 표시합니다. 
  for i, (img, test_idx, pred_idx) in enumerate(zip(rand_test_imgs,rand_test_label_index,rand_predict_label_index)):
    predict_ax[i].axis('off')
    predict_ax[i].imshow(img)
    predict_ax[i].text(0,1,f'Prediction: {labels[pred_idx]}', backgroundcolor='white', size=6, transform=predict_ax[i].transAxes)
    predict_ax[i].text(0,0.9,f'LABEL: {labels[test_idx]}', backgroundcolor='white', size=6,transform=predict_ax[i].transAxes)
  plt.show();

def main():
  (nX_train, y_train), (X_test, y_test) = load_and_normalize()
  model, train_history = train_model(nX_train, y_train)
  evaluate_and_predict(model, train_history, X_test, y_test)

main()
