## git

### SVN 集中式版本控制系统

```
(代码仓库)
SVN Server : 维护了代码的所有的版本
|                  |                   |
|                  |                   |
client 1           client 2          client 3
(SVN client)     (SVN client)     (SVN client)

通过网络通信连接，所有的增删改都是在SVN server中发生，SVN只是做请求

一般在局域网上进行共同开发
```
### git分布式版本控制系统

```

(代码版本托管系统 e.g. github) 
远程仓库


(本地仓库)  (本地仓库)     (本地仓库)
git1         git2         git3

// 每一个都是一个独立的版本控制系统，工作在一个结点，结点和结点之间不需要构成一个网络
// 单独就可以工作
// 然后提交到远程仓库，进行合并
```

## git常用命令

```
git log

git remote

git branch

git branch -r
(分支就是一个代码版本)

git status

```


```
                    git clone ssh远程仓库地址
远程Hello world项目仓库    ------->   git bash 
                          
代码版本: 
master主干分支

// git clone先把代码仓库拉到本地
// 然后git bash中两个东西，自动创建
// 1. 远程的仓库名称 origin(这个名字可以更改)
// 2. 本地仓库 生成一个默认的主干分支master --> 对应origin/master

```

```
// git工作区 : 本地仓库的代码目录
// git add : 把工作区的代码改动，提交到暂存区当中
// git commit 
// git push
          git add    git commit 
git 工作区 ----》 暂存区 -------》提交到本地的分支上

// git push origin master:master
// 将本地的master分支push到远程的master
// 由于名称都是master，所以可以用简略的写法
// git push origin master

git push
-------》更新了远程origin仓库的master分支上的代码

在修改前，git pull是一个好习惯

还有多看git status

git diff HEAD -- xx.cc
```

### HEAD 指针

## git各阶段版本回退命令

都可以用git status看提示

### git add 之前

```
git checkout -- <file>
```
本地仓库/分支将 当前工作区 修改的内容/文件 覆盖掉

### git add后放入暂存区

```
git reset HEAD  

git reset HEAD <file>
```

取消相应暂存区修改

### git commit 后

```
git reset --hard <commit id>
```

注意这里只是把本地仓库分支版本上的HEAD指针进行了移动，实际上没有删除任何内容

```
git reflog
```

查看之前的commit id， reset --hard再继续回去

### 远程仓库的代码修改不想要了，有两种方法：

1. git pull，在本地分支最新的代码版本上删除之前修改的，然后重新push到远程代码仓库上

2. 在本地分支上通过git reset --hard xxx回退到之前的版本，然后通过git push -f推送覆盖远程代码仓
库

## 解决冲突

```
origin/master 
v1

MITSK git bash
git pull

v1 --> fix --> v2 推送

Ligh git bash
git pull
v1 --> fix --> v3 推送
(这个时候，远程仓库已经是V2版本了，但是v3是基于V1版本， 提交代码发生冲突问题)
```

```
1. 先更新远程的代码 git pull
2. 再git push origin master

// 如果改同一个代码，遇到冲突
// 那么手动修改
// 然后重新添加git add commit push
```

## git 分支版本控制

```
git branch 查看本地分支
git branch -r 查看远程分支
git branch -a 本地远程分支都查看

git branch xxx 创建分支，如果有则失败
git checkout xxx 切换分支

git branch -d xxx 这个没合并不能删除
git branch -D xxx 不管合没合并，都可暴力删除
```

```
git checkout -b ysy  创建并切换

                (master)  --》 origin/master
v1 -- v2 -- v3 -- v4
                 ysy  
                  |
                HEAD

就是将HEAD指针指向了ysy

```

```
切换到ysy后，git pull，发现不知道从远程哪个分支pull，没有对应的远程分支

用git branch -vv 可对比master查看
```

```
// 在ysy分支开发出v5
                (master)
v1 -- v2 -- v3 -- v4  -- v5
                        ysy  
                         |
                        HEAD

git checkout master
git merge ysy
                        HEAD
                      (master)
v1 -- v2 -- v3 -- v4  -- v5
                        ysy  

// 这里就是将master分支指向新开发的v5

git push origin master
```

```
// 或者直接push到远端的分支
// 如果不指定，就不知道怎么推了
git push origin ysy:master
```

### 分支合并时，master已经发生了改变的情况


```     
                1111
                (master)
v1 -- v2 -- v3 -- v4  -- v5
                        ysy  
                         |
                        HEAD
                        11112222

git checkout master 
git merge ysy --> 合并到本地master分支代码上
git push origin master
```

```
当ysy分支git add .
git commit 后

master已经有其他人git add .
git commit 
git push 
                (master)
                11113333
                  v6  
                  |
v1 -- v2 -- v3 -- v4  -- v5
                        ysy  
                         |
                        HEAD
                        11112222

切换回去 : git checkout master
git pull (这里如果发生改变，说明master已经被修改)
git merge ysy(如果改一个地方则合并失败)
然后手动解决
git add .
git commit 
git push 

                        HEAD(11112222333) 
                  v6  -- v7
                  |      |
v1 -- v2 -- v3 -- v4  -- v5
                        ysy  

```

## 远程分支管理

```
github网页上创建一个分支

然后git pull， 就会提醒你已经有了新的分支dev

本地创建一个分支ysy, 并且要追踪远程的dev分支

git checkout -b ysy origin/dev
```












