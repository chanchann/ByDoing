## 处理请求

请求方法 -- 动作指令

最常用 GET HEAD POST PUT 

检查一个文件存在，是否有新版本用HEAD

POST VS PUT 

POST -- create 

PUT -- update 很少用，都是用的post

## 安全与幂

安全 -- 不破坏(修改)服务器的资源

GET/HEAD安全，幂等

幂等是执行多次相同的操作，结果也相同

delete，put幂等

post不幂等

