## sql 查询语句如何执行

```
select * from T where ID=10;
```

![mysql arch](../assets/01_arch.png)

分为Server层和存储引擎层(负责数据的存储和提取，插件式，支持InnoDB(defult now)，MylSAM，Memory等多个存储引擎)

## 连接器

```
mysql -uroot -p
```

- 连接的和权限

```
// 查看空闲连接(sleep)
show processlist;
```

- 客户端timeout

建立连接通常比较复杂，建议尽量使用长连接

但长连接有时候内存涨的快，内存是管理在连接对象里面的，只有在连接断开才会释放。

解决 ：1. 定期断开长连接 2. 5.7之后，每次执行一个较大的操作可通过mysql_reset_connection初始化连接资源

## 查询缓存

建立好连接，执行select语句

先到查询缓存看看，之前是否执行过这条语句(key-value形式直接缓存到内存中)

hints: 不要使用缓存查询

失效非常频繁，命中率很低

除非不太常更新，比如系统配置表

```
// query_cache_type 设置 DEMAND
// 8.0 将查询缓存功能删掉了
select SQL_CACHE * from T where ID = 10;
```

## 分析器

sql语句解析

词法分析 -- 语法分析

## 优化器

当多个索引，决定使用哪个索引

当多表关联(join),决定表的连接顺序

```
select * from t1 join t2 using(ID) where t1.c=10 and t2.d =20;
```

## 执行器

会先判断对表T有没有执行查询的权限，无则返回没权限的错误

有权限则打开表继续执行，打开表的时候，执行器就会根据表的引擎定义去使用引擎提供的接口



