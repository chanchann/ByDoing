#include <string>
#include <vector>
#include <map>


// 各种字符串hash算法
// https://byvoid.com/zhs/blog/string-hash-compare

/*
实验说明 : 不带虚拟结点的哈希一致性

实验结果:

[192.168.0.1:8888] put int set, hash is : 10707207574622922825
[192.168.0.2:8888] put int set, hash is : 11140657322441365472
[192.168.0.3:8888] put int set, hash is : 15770806514270490681
[tencent]'s hash is 13506440387206333248 and route to the node : [192.168.0.3:8888]
[baidu]'s hash is 16358760792147397559 and route to the node : [192.168.0.1:8888]
[google]'s hash is 17002762361111958536 and route to the node : [192.168.0.1:8888]
*/
int main()
{
    std::vector<std::string> server_list = {"192.168.0.1:8888", "192.168.0.2:8888", "192.168.0.3:8888"};
    std::map<size_t, std::string> hash_server_map;
    std::hash<std::string> get_hash;

    // 1. build map
    for(int i = 0; i < server_list.size(); i++)
    {
        size_t hash = get_hash(server_list[i]);
        hash_server_map[hash] = server_list[i];
        fprintf(stderr, "[%s] put int set, hash is : %zu\n", server_list[i].c_str(), hash);
    }

    auto get_server = [&get_hash, &hash_server_map](const std::string& key) -> std::string
    {
        size_t hash = get_hash(key);
        auto it = hash_server_map.upper_bound(hash);
        if(it != hash_server_map.end())  // 找到大于当前key的iter
        {
            return it->second;
        } else   
        {
            return hash_server_map.begin()->second;    // 没有比他大的就第一个节点
        }
    };

    std::vector<std::string> key_list = {"tencent", "baidu", "google"};
    for (int i = 0; i < key_list.size(); i++)
    {
        fprintf(stderr, "[%s]'s hash is %zu and route to the node : [%s]\n", 
                        key_list[i].c_str(), get_hash(key_list[i]), get_server(key_list[i]).c_str());
    }
    return 0;
}