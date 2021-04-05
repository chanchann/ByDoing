## mysql 语句小抄

## select 

```
// 字符串应该在单引号里
SELECT population FROM world WHERE name = 'France'

// 还可以用公式
SELECT name, gdp/population FROM world WHERE area > 5000000

// 使用in list来筛选 
SELECT name, population FROM world WHERE name IN ('Ireland', 'Iceland', 'Denmark');

// between表示范围，包含性的
SELECT name, area FROM world WHERE area BETWEEN 200000 AND 250000;

/// LIKE : Pattern Matching Strings

// 找出Y开头的国家
SELECT name FROM world WHERE name LIKE 'Y%'

// 找出Y结尾的国家
SELECT name FROM world WHERE name LIKE '%Y'

// 找出名字包含x的国家
SELECT name FROM world WHERE name LIKE '%x%';

// 三个或以上a
SELECT name FROM world WHERE name LIKE '%a%a%a%'

// 找出所有国家,其名字以t作第二個字母。
SELECT name FROM world WHERE name LIKE '_t%' ORDER BY name

//找出所有国家,其名字都有两个字母 o,被另外两个字母相隔着。
SELECT name FROM world WHERE name LIKE '%o__o%'

// 找出所有国家，字母四个字母
SELECT name FROM world WHERE name LIKE '____'

///  = 
// 显示所有国家，他的国家和首都名字相同
SELECT name FROM world WHERE name = capital

/// concat 
SELECT name FROM world WHERE capital = concat(name, ' City')

// 首都要在国家名字中出现
select capital, name from world where capital like concat('%', name, '%'); 

// 找出首都是国家名的延伸
select name, capital from world where capital like concat(name, '_%');

// REPLACE
// REPLACE('vessel','e','a') -> 'vassal'
select name,replace(capital, name, '') from world where capital Like concat(name,'%_')
```



