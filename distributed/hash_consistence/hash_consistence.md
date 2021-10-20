## hash一致性

1. 什么是hash算法

2. 普通hash算法发不足，如何解决

引出

3. 一致性hash算法

在动态变化的Cache环境中，哈希算法应该满足的4个适应条件

- 均衡性(Balance)

- 单调性(Monotonicity)

- 分散性(Spread)

- 负载(Load)

4. 环形hash空间

5. 平衡性

引出虚拟结点

即对每一个服务节点计算多个哈希，每个计算结果位置都放置一个此服务节点，称为虚拟节点

# ref

https://zhuanlan.zhihu.com/p/98030096

https://segmentfault.com/a/1190000021199728#:~:text=%E4%B8%80%E8%87%B4%E6%80%A7%E5%93%88%E5%B8%8C%E7%AE%97%E6%B3%95%E5%9C%A81997%20%E5%B9%B4%E7%94%B1%E9%BA%BB,%E7%9A%84%E5%8A%A8%E6%80%81%E4%BC%B8%E7%BC%A9%E7%AD%89%E9%97%AE%E9%A2%98%E3%80%82

