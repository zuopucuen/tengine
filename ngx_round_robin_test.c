#include <stdio.h>
#include <stdint.h>

typedef intptr_t        ngx_int_t;
typedef uintptr_t       ngx_uint_t;
typedef intptr_t        ngx_flag_t;

typedef struct {
    ngx_int_t                       num;
    ngx_int_t                       current_weight;
    ngx_int_t                       effective_weight;
    ngx_int_t               select_weight;
    ngx_int_t                       weight;
} peer_t;

ngx_uint_t get_peer(peer_t *peers);
ngx_uint_t number;
ngx_uint_t init_number;
uintptr_t *tried;

int main() {
    ngx_uint_t i,j,w,t;
    peer_t peers[60];
    uintptr_t data;

    t = 0;
    number = 60;
    init_number = 15;
    tried = &data;

    for(i=0;i<number;i++){
        peers[i].num = i;
        peers[i].weight = (i%2)? 8:10;
        peers[i].effective_weight = peers[i].weight;
        peers[i].current_weight = 0;
    }

    for(i=0;i<1000;i++){
        data = 0;
        j = get_peer(peers);

        if(w != peers[j].weight && t > 0) {
            printf("weight:%d total:%d\n", w, t);
            t = 0;
        }

        w = peers[j].weight;
        t++;

        //printf("seq:%d num:%d weight:%d select_weight:%d affer_weight: %d\n", i, peers[j].num, peers[j].weight, peers[j].select_weight, peers[j].current_weight);
    }

}

ngx_uint_t get_peer(peer_t *peers) {
    uintptr_t m;
    ngx_uint_t i,n;
    ngx_int_t total, flag;
    peer_t *peer, *best;

    best = NULL;
    total = 0;
    flag = 1;

    for(i = init_number; i != init_number || flag; i = (i + 1)% number){

        flag = 0;

        n = i / (8 * sizeof(uintptr_t));
        m = (uintptr_t) 1 << i % (8 * sizeof(uintptr_t));

        if (tried[n] & m) {
            continue;
        }

        peer = &peers[i];

        peer->current_weight += peer->effective_weight;
        peer->select_weight = peer->current_weight;
        total += peer->effective_weight;

        if (best == NULL || peer->current_weight > best->current_weight) {
                    best = peer;
            }
    }

    i = best - &peers[0];

    n = i / (8 * sizeof(uintptr_t));
    m = (uintptr_t) 1 << i % (8 * sizeof(uintptr_t));

    tried[n] |= m;

    best->current_weight -= total;

    return i;
}
