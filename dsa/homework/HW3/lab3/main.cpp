#include <cstdio>
#include <cstdlib>
#include "hashtable.h"

int main(int argc, char* argv[]){
    int type;
    char buffer[1000];int data;
    // 1:坏哈希,2:好哈希
    // 1：线性探测，2：平方探测，3：溢出区
    int m = atoi(argv[1]);
    int n = atoi(argv[2]);
    hashing_strategy* hashing;
    collision_strategy* collision;
    if(m == 1) {
        hashing = new bad_hashing();
    }
    else {
        hashing = new good_hashing();
    }
    if(n == 1) {
        collision = new linear_probe();
    }
    else if(n == 2) {
        collision = new Bid_square_probe();
    }
    else {
        collision = new Overflow_probe();
    }
    hashtable table(TABLE_SIZE,hashing, collision);
    while(true){
        scanf("%d", &type);
        if(type == 0){
            scanf("%s", buffer);scanf("%d",&data);
            table.insert(hash_entry(buffer, data));
        }else if(type == 1){
            scanf("%s",buffer);
            printf("%d\n", table.query(buffer));
        }else break;
    }
    return 0;
}