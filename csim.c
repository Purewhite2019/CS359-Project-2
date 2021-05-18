// File Name: csim.c
// Description: cache_t simulator, takes a valgrind memory trace as input, 
//              simulates the hit/miss behavior of a cache memory on this trace, 
//              and outputs the total number of hits, misses, and evictions.
// Author: Qi Liu (519021910529, purewhite@sjtu.edu.cn)

#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include "cachelab.h"

#define ADDR_LENGTH 64
#define USE_LRU

typedef uint64_t content_t;

typedef struct {
    int valid;
    uint64_t tag;
    content_t content;
    int timer;
} block_t;
typedef block_t* set_t;
typedef set_t* cache_t;

char *HELPER_INFO[] = {
"[-hv] -s <s> -E <E> -b <b> -t <tracefile>", 
"\t•-h: Optional help flag that prints usage info", 
"\t•-v: Optional verbose flag that displays trace info", 
"\t•-s <s>: Number of set index bits (num_set = 2^s is the number of sets)", 
"\t•-E <E>: Associativity (number of lines perset)", 
"\t•-b <b>: Number of block bits (size_block = 2^b is the blocksize)", 
"\t•-t <tracefile>: Name of the valgrind trace to replay"
};

int t = -1, s = -1, b = -1, e = -1, isVerbose = 0;;
int num_set = -1, size_block = -1;
int hit_cnt = 0, miss_cnt = 0, evict_cnt = 0, cur_timer = 0;

uint64_t get_offset(uint64_t addr){
    return addr & ((1<<b)-1);
}

uint64_t get_index(uint64_t addr){
    return (addr >> b) & ((1<<s)-1);
}

uint64_t get_tag(uint64_t addr){
    return (addr >> (s+b));
}

void caccess(cache_t cache, uint64_t addr, int cnt);

int main(int argc, char *argv[])
{
    int opt = 0;
    char *tracefile = NULL;
    cache_t cache = NULL;
    FILE *fd = NULL;
    uint64_t addr;
    char *cmd = NULL;
    int cnt;
    size_t len;

    while((opt = getopt(argc, argv, "hvs:E:b:t:")) != -1){
        switch(opt){
            case 'h':
                printf("Usage: %s %s\n", argv[0], HELPER_INFO[0]);
                for(int i = 1; i < sizeof(HELPER_INFO)/sizeof(char*); ++i)
                    printf("%s\n", HELPER_INFO[i]);
                exit(EXIT_SUCCESS);
                break;
            case 'v':
                isVerbose = 1;
                break;
            case 's':
                num_set = 1 << (s = atoi(optarg));
                // printf("s = %d\n", s);
                break;
            case 'E':
                e = atoi(optarg);
                // printf("e = %d\n", e);
                break;
            case 'b':
                size_block = 1 << (b = atoi(optarg));
                // printf("b = %d\n", b);
                break;
            case 't':
                tracefile = optarg;
                break;
            default:
                fprintf(stderr, "Error: Invalid command.\n");
                fprintf(stderr, "Usage: %s %s\n", argv[0], HELPER_INFO[0]);
                for(int i = 1; i < sizeof(HELPER_INFO)/sizeof(char*); ++i)
                    fprintf(stderr, "%s\n", HELPER_INFO[i]);
                exit(EXIT_FAILURE);
        }
    }
    if(s < 0 || b < 0 || e <= 0){
        fprintf(stderr, "Error: Invalid command.\n");
        fprintf(stderr, "Usage: %s %s\n", argv[0], HELPER_INFO[0]);
        for(int i = 1; i < sizeof(HELPER_INFO)/sizeof(char*); ++i)
            fprintf(stderr, "%s\n", HELPER_INFO[i]);
        exit(EXIT_FAILURE);
    }
    t = ADDR_LENGTH - s - b;


    if((fd = fopen(tracefile, "r")) == NULL){
        fprintf(stderr, "Error: fopen() failed.\n");
        exit(EXIT_FAILURE);
    }

    cache = (set_t*)malloc(sizeof(set_t) * num_set);
    for(int i = 0; i < num_set; ++i){
        cache[i] = (block_t*)malloc(sizeof(block_t) * e);
        memset(cache[i], 0, sizeof(block_t) * e);
    }
    while(getline(&cmd, &len, fd) != -1){
        if(cmd[0] != ' ') continue;
        addr = 0;
        int i = 2;
        while(i < len && !isalnum(cmd[i])) ++i;
        while(i < len && isalnum(cmd[i])){
            addr <<= 4;
            if(isdigit(cmd[i]))
                addr |= (uint8_t)(cmd[i]-'0');
            else if(isalpha(cmd[i]))
                addr |= (uint8_t)(tolower(cmd[i])-'a' + 10);
            ++i;
        }
        while(i < len && !isalnum(cmd[i])) ++i;
        cnt = atoi(cmd+i);
        if(isVerbose){
            printf("%c %lx,%d ", cmd[1], addr, cnt);
        }
        switch(cmd[1]){
            case 'M':
                caccess(cache, addr, cnt);
            case 'L':
            case 'S':
                caccess(cache, addr, cnt);
                putchar('\n');
                break;
            default:
                fprintf(stderr, "Error: invalid command \"%c\"\n", cmd[1]);
                exit(EXIT_FAILURE);
        }
        free(cmd);
        cmd = NULL;
    }
    printSummary(hit_cnt, miss_cnt, evict_cnt);
    // printf("hit_cnt:%d miss_cnt:%d evict_cnt:%d\n", hit_cnt, miss_cnt, evict_cnt);

    fclose(fd);
    return 0;
}

void caccess(cache_t cache, uint64_t addr, int cnt){
    uint64_t tag = get_tag(addr), index = get_index(addr);
    // uint64_t offset = get_offset(addr);
    ++cur_timer;
    // printf("tag: %lx, index: %lx, offset: %lx\n", tag, index, offset);
    fflush(stdout);
    for(int i = 0; i < e; ++i){
        if(cache[index][i].tag == tag && cache[index][i].valid){
#ifdef USE_LRU
            cache[index][i].timer = cur_timer;
#endif
            ++hit_cnt;
            if(isVerbose)
                printf("hit ");
            return;
        }
    }
    ++miss_cnt;
    if(isVerbose)
        printf("miss ");
    int place = 0;
    while(place < e && cache[index][place].valid) ++place;
    if(place < e){
        cache[index][place].tag = tag;
        cache[index][place].timer = cur_timer;
        cache[index][place].content = 0;
        cache[index][place].valid = 1;
    }
    else{
        ++evict_cnt;
        if(isVerbose)
            printf("eviction ");
        place = 0;
        for(int i = 1; i < e; ++i)
            if(cache[index][i].timer < cache[index][place].timer)
                place = i;
        cache[index][place].tag = tag;
        cache[index][place].timer = cur_timer;
        cache[index][place].content = 0;
        cache[index][place].valid = 1;
    }
}