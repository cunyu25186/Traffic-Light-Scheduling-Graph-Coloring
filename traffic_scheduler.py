#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# 定义 13 条可行路线
ROUTE_NAMES = [
    "AB", "AC", "AD",  # 0, 1, 2
    "BA", "BC", "BD",  # 3, 4, 5
    "DA", "DB", "DC",  # 6, 7, 8
    "EA", "EB", "EC", "ED"  # 9, 10, 11, 12
]

NUM_ROUTES = len(ROUTE_NAMES)

# 初始化全为 False 的二维冲突矩阵
conflict_graph = [[False] * NUM_ROUTES for _ in range(NUM_ROUTES)]

def add_conflict(u, v):
    """在图中添加双向冲突边"""
    conflict_graph[u][v] = True
    conflict_graph[v][u] = True

def build_conflict_model():
    """依据路口几何交叉规则构建冲突模型"""
    add_conflict(0, 4); add_conflict(0, 5); add_conflict(0, 6); add_conflict(0, 7); add_conflict(0, 9)   # AB
    add_conflict(1, 5); add_conflict(1, 6); add_conflict(1, 7); add_conflict(1, 9); add_conflict(1, 10)  # AC
    add_conflict(2, 9); add_conflict(2, 10); add_conflict(2, 11)                                         # AD
    add_conflict(4, 6); add_conflict(4, 7); add_conflict(4, 10); add_conflict(4, 11)                     # BC
    add_conflict(5, 10); add_conflict(5, 11)                                                             # BD
    add_conflict(6, 10); add_conflict(6, 11)                                                             # DA
    add_conflict(7, 10); add_conflict(7, 11)                                                             # DB

def schedule_traffic():
    """执行贪心着色算法进行路权分配"""
    build_conflict_model()
    
    # colors 列表记录每条路线的相位，None 表示未分配
    colors = [None] * NUM_ROUTES
    
    # 优先处理常绿右转通道 (BA:3, DC:8, ED:12)
    colors[3] = colors[8] = colors[12] = 0
    
    current_phase = 1
    
    for i in range(NUM_ROUTES):
        if colors[i] is not None:
            continue
            
        colors[i] = current_phase
        
        # 尝试将后续不冲突的路线拼入当前相位
        for j in range(i + 1, NUM_ROUTES):
            if colors[j] is None:
                # 检查路线 j 是否与当前相位内的任何路线冲突
                is_safe = all(not (colors[k] == current_phase and conflict_graph[j][k]) for k in range(NUM_ROUTES))
                if is_safe:
                    colors[j] = current_phase
                    
        current_phase += 1
        
    return colors

if __name__ == "__main__":
    result_colors = schedule_traffic()
    
    print("=" * 40)
    print("  多岔路口交通信号灯调度方案 (Python)")
    print("=" * 40)
    
    # 打印常绿相位
    phase_0_routes = [ROUTE_NAMES[i] for i, phase in enumerate(result_colors) if phase == 0]
    print(f" [相位 0] (右转常绿通道): {' '.join(phase_0_routes)}")
    print("-" * 40)
    
    # 打印正常的绿灯相位
    max_phase = max(result_colors)
    for p in range(1, max_phase + 1):
        phase_routes = [ROUTE_NAMES[i] for i, phase in enumerate(result_colors) if phase == p]
        if phase_routes:
            print(f" [相位 {p}] (绿灯通行): {' '.join(phase_routes)}")
            
    print("=" * 40)
