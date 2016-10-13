import numpy as np
import matplotlib.pyplot as plt

a = np.loadtxt('./IMU-data.dat')
data_label = ['ax','ay','az','gx','gy','gz']
ax = []
for i in range(6):
    position = '23' + str(i+1)
    ax.append(plt.subplot(position))
    ax[i].set_title(data_label[i])
    plt.plot(a[:,i])
plt.show()
