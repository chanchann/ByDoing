## 弹力设计

容错设计又叫弹力设计

1. 容错能力

服务隔离、异步调用、请求幂等性

2. 可伸缩性

有 / 无状态的服务

3. 一致性

补偿事务、重试

4. 应对大流量的能力

熔断、降级

### 系统可用性测量

```
// fomula : 
Availability = MTTF / (MTTF + MTTR)

可用性= 系统平均能够正常运行多长时间才发生一次故障 /（系统平均能够正常运行多长时间才发生一次故障+从出现故障到恢复正常的时间）

常说的几个9
```

### 故障原因

- 无计划

- 有计划

归类:

1. 网络问题。

2. 性能问题

3. 安全问题

4. 运维问题

5. 监控问题

6. 管理问题

7. 硬件问题。硬盘损坏

### 故障不可避免

so try to 降低 MTTR——故障的修复时间

1. 用户和内部运维来说是完全透明的，系统自动修复不需要人的干预。

2. 如果修复不了，系统能够做自我保护，而不让事态变糟糕。

## 隔离设计(Bulkheads)

### 分离方式

1. 按服务的种类来做分离

优点 / 缺点

2. 按用户的请求来做分离

### 隔离设计的重点

1. 粒度

2. trade off

3. 各种设计模式配套使用

4. 运维的复杂度的提升

5. 监控系统

### 异步通讯设计

- 同步

四个问题：

影响吞吐量、消耗系统资源、只能一对一，以及有多米诺骨牌效应

相对同步

1. 增加系统的吞吐量

2. 让服务间的解耦更为彻底，系统的调用方和被调用方可以按照自己的速率而不是步调一致

### 异步通讯的三种方式

1. 请求响应式

2. 通过订阅的方式

3. 通过 Broker 的方式

Broker 需要满足

高可用/高性能,可水平扩展/持久化不丢数据

### 事件驱动设计

事件驱动最好是使用 Broker 方式，服务间通过交换消息来完成交流和整个流程的驱动。

- 好处:

1. 减少依赖

2. 高度隔离

3. 服务间不会相互block

4. 增加Adapter容易

5. 服务间吞吐解开

- 坏处:

1. 业务流程不再那么明显和好管理。

2. 事件可能会乱序

3. 事务处理变得复杂

### 异步通讯的设计重点

1. 解耦服务间的依赖 - Broker机制(可"削峰")

2. 解耦让各个服务的隔离性更好

3. 更大的吞吐量


### 异步通讯的注意

1. Broker 高可用不丢消息

2. 最好不依赖于消息的顺序

3. Broker 上需要有相关的服务消息跟踪机制

4. 业务状态最好由一个总控方来管理

## 幂等性设计

服务调用有三种结果：成功、失败和超时

因为系统超时，而调用户方重试一下，会给我们的系统带来不一致的副作用。

### 解决办法

1. 超时后查询调用结果

2. 被调用的服务中实现幂等性

### 全局ID

Snowflake - 分布式 ID 的生成算法

### 处理流程

需要一个存储来记录收到的交易

### HTTP 的幂等性

## 服务的状态

“状态”，就是为了保留程序的一些数据或是上下文

### 无状态的服务 Stateless

现实世界是一定会有状态的

1. 程序调用的结果。

2. 服务组合下的上下文。

3. 服务的配置。

为了做出无状态的服务，我们通常需要把状态保存到一个第三方的地方(增加依赖/增加网络开销)

第三方存储服务要做成高可用高扩展的方式

要在无状态的服务中增加缓存机制(所有的机器上都创建, 浪费)

### 有状态的服务 Stateful

好处:

1. 数据本地化（Data Locality）

2. 更高的可用性和更强的一致性。也就是 CAP 原理中的 A 和 C。

因为对于有状态的服务，我们需要对于客户端传来的请求，都必需保证其落在同一个实例上，这叫 Sticky Session 或是 Sticky Connection。

无状态的服务需要我们把数据同步到不同的结点上，而有状态的服务通过 Sticky Session 做数据分片

### Sticky Session

1. simpest : 用持久化的长连接

2. hash算法

反向压力 (Back Pressure)

问题 : 结点的负载和数据并不会很均匀。  

solution : 元数据索引

### 服务状态的容错设计








## ref

https://time.geekbang.org/column/article/3912