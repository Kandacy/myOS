
/**
 *  @author: Song XiuMing
 *  @date: 22/11/24
 */


#include "mm/mem.h"
#include "type.h"


/* MapType */
#define MAP_TYPE_IDENTICAL  0
#define MAP_TYPE_FRAMED     1

/* MapPermission */
#define MAP_PREMISSION_R  (1 << 1)
#define MAP_PREMISSION_W  (1 << 2)
#define MAP_PREMISSION_X  (1 << 3)
#define MAP_PREMISSION_U  (1 << 4)

/* 逻辑段类型 */
typedef u64 MapType;
/* 逻辑段的权限 */
typedef u64 MapPermission;


/* 连续虚拟地址的范围 */
typedef struct VpnRange{
    u64 start_va;
    u64 size;
}VpnRange;


/* 逻辑段（具有相同权限的段，虚拟地址连续） */
typedef struct MapArea{
    VpnRange vpn_range;
    MapType map_type;
    MapPermission map_premission;
}MapArea;


/* 地址空间（一个task有一个） */
typedef struct MemorySet{
    PageTable page_table;
    areas
}MemorySet;
