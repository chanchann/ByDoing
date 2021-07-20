## protobuf实例

结构化的数据叫Message

## protoc 编译

## Writer 和 Reader

需要处理的结构化数据由.proto文件描述

protoc生成的xx.pb.h, xx.pb.cc 提供了一系列get/set函数来修改和读取结构化数据中的数据成员(also call field)

提供相应的方法将复杂的数据变成一个字节序列，可以将这个字节序列写入磁盘(or 网络发送)

也提供了相应的方法将这个字节序列 反序列化 成 结构化数据




