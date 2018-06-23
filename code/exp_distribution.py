import numpy as np
import matplotlib.pyplot as plt

tau = 10
sample = np.random.exponential(tau, size=10000)  # 产生10000个满足指数分布的随机数
plt.hist(sample, bins=80, alpha=0.7, normed=True) #绘制直方图
plt.margins(0.02) 

# 根据公式绘制指数分布的概率密度函数
lam = 1 / tau
x = np.arange(0,80,0.1)
y = lam * np.exp(- lam * x)
plt.plot(x,y,color='orange', lw=3)#设置标题和坐标轴
plt.title('Exponential distribution, 1/lambda=10')
plt.xlabel('time')
plt.ylabel('PDF')
plt.show()