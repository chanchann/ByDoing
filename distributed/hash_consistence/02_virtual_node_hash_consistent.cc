#include <string>
#include <vector>
#include <map>


/*
实验说明 : 带虚拟结点的哈希一致性

实验结果:
[192.168.0.1:8888&&VN0] put int set, hash is : 3710116013498174853
[192.168.0.1:8888&&VN1] put int set, hash is : 12890603858251286624
[192.168.0.1:8888&&VN2] put int set, hash is : 7603173508233385645
[192.168.0.1:8888&&VN3] put int set, hash is : 5756236719200809828
[192.168.0.1:8888&&VN4] put int set, hash is : 8236328850903603002
[192.168.0.2:8888&&VN0] put int set, hash is : 6191903703918278014
[192.168.0.2:8888&&VN1] put int set, hash is : 774295184670679217
[192.168.0.2:8888&&VN2] put int set, hash is : 2164697851290214540
[192.168.0.2:8888&&VN3] put int set, hash is : 14324957485418151374
[192.168.0.2:8888&&VN4] put int set, hash is : 9537256200606601758
[192.168.0.3:8888&&VN0] put int set, hash is : 10882359686582158051
[192.168.0.3:8888&&VN1] put int set, hash is : 17303120967576961008
[192.168.0.3:8888&&VN2] put int set, hash is : 3849842255190010168
[192.168.0.3:8888&&VN3] put int set, hash is : 3114459201378877743
[192.168.0.3:8888&&VN4] put int set, hash is : 3123397310438073368
[tencent]'s hash is 13506440387206333248 and route to the node : [192.168.0.2:8888]
[baidu]'s hash is 16358760792147397559 and route to the node : [192.168.0.3:8888]
[google]'s hash is 17002762361111958536 and route to the node : [192.168.0.3:8888]
*/
int main()
{
    // 待添加入Hash环的服务器列表
    std::vector<std::string> server_list = {"192.168.0.1:8888", "192.168.0.2:8888", "192.168.0.3:8888"};
    
    std::map<size_t, std::string> hash_virtual_node_map;
    std::hash<std::string> get_hash;
    
     // 虚拟节点的数目，这里写死，为了演示需要，一个真实结点对应5个虚拟节点
    const int VIRTUAL_NODE_NUM = 5;

    // 1. build map
    for(int i = 0; i < server_list.size(); i++)
    {
        for(int j = 0; j < VIRTUAL_NODE_NUM; j++)
        {
            std::string virtual_node_name = server_list[i] + "&&VN" + std::to_string(j);
            size_t hash = get_hash(virtual_node_name);
            hash_virtual_node_map[hash] = virtual_node_name;
            fprintf(stderr, "[%s] put int set, hash is : %zu\n", virtual_node_name.c_str(), hash);
        }
    }

    auto get_server = [&get_hash, &hash_virtual_node_map](const std::string& key) -> std::string
    {
        size_t hash = get_hash(key);
        auto it = hash_virtual_node_map.upper_bound(hash);
        std::string server_name;
        if(it != hash_virtual_node_map.end())  // 找到大于当前key的iter
        {
            server_name = it->second;
        } else   
        {
            server_name = hash_virtual_node_map.begin()->second;    // 没有比他大的就第一个节点
        }
        auto pos = server_name.find("&");
        return server_name.substr(0, pos);
    };

    std::vector<std::string> key_list = {"tencent", "baidu", "google"};
    for (int i = 0; i < key_list.size(); i++)
    {
        fprintf(stderr, "[%s]'s hash is %zu and route to the node : [%s]\n", 
                        key_list[i].c_str(), get_hash(key_list[i]), get_server(key_list[i]).c_str());
    }
    return 0;
}