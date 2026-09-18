#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#define TOTAL_PATHS 13
// 定义路线结构
typedef struct {
	char name[5];
	int group_id; // -1表示未分配通行组，0表示常绿，1,2,3...表示不同的绿灯轮次
} TrafficPath;
// 初始化13条通行路径
TrafficPath paths[TOTAL_PATHS] = {
	{"AB", -1}, {"AC", -1}, {"AD", -1}, // 0, 1, 2
	{"BA", -1}, {"BC", -1}, {"BD", -1}, // 3, 4, 5
	{"DA", -1}, {"DB", -1}, {"DC", -1}, // 6, 7, 8
	{"EA", -1}, {"EB", -1}, {"EC", -1}, {"ED", -1} // 9, 10, 11, 12
};
// 冲突矩阵 (0表示不冲突，1表示冲突)
int collision_matrix[TOTAL_PATHS][TOTAL_PATHS] = {0};
// 设置两条路径互相冲突的辅助函数
void mark_collision(int p1, int p2) {
	collision_matrix[p1][p2] = 1;
	collision_matrix[p2][p1] = 1;
}
// 构建路口冲突关系库
void build_collision_matrix() {
	// 依据路口几何交叉规则录入冲突
	// AB(0) 冲突: BC, BD, DA, DB, EA
	mark_collision(0, 4); mark_collision(0, 5); mark_collision(0, 6); mark_collision(0, 7); mark_collision(0, 9);
	// AC(1) 冲突: BD, DA, DB, EA, EB
	mark_collision(1, 5); mark_collision(1, 6); mark_collision(1, 7); mark_collision(1, 9); mark_collision(1, 10);
	// AD(2) 冲突: EA, EB, EC
	mark_collision(2, 9); mark_collision(2, 10); mark_collision(2, 11);
	// BC(4) 冲突: DA, DB, EB, EC
	mark_collision(4, 6); mark_collision(4, 7); mark_collision(4, 10); mark_collision(4, 11);
	// BD(5) 冲突: EB, EC
	mark_collision(5, 10); mark_collision(5, 11);
	// DA(6) 冲突: EB, EC
	mark_collision(6, 10); mark_collision(6, 11);
	// DB(7) 冲突: EB, EC
	mark_collision(7, 10); mark_collision(7, 11);
}
// 执行分组调度算法 (图着色原理)
void schedule_traffic() {
	// 步骤1：处理右转常绿车道 (不需要等红绿灯)
	// 根据分析，BA(3), DC(8), ED(12) 属于外侧右转，不与任何路线交叉
	paths[3].group_id = 0;
	paths[8].group_id = 0;
	paths[12].group_id = 0;
	
	int current_turn = 1; // 当前绿灯轮次
	int unassigned_count = TOTAL_PATHS - 3; // 还需要分配的路径数量
	
	// 步骤2：为剩下的路径分配绿灯轮次
	while (unassigned_count > 0) {
		for (int i = 0; i < TOTAL_PATHS; i++) {
			// 如果这条路还没有被分配
			if (paths[i].group_id == -1) {
				bool can_go = true;
				
				// 检查这条路是否跟【当前轮次】已经放行的车冲突
				for (int j = 0; j < TOTAL_PATHS; j++) {
					if (paths[j].group_id == current_turn && collision_matrix[i][j] == 1) {
						can_go = false; // 发现冲突，这轮不能走
						break;
					}
				}
				
				// 如果不冲突，加入当前通行轮次
				if (can_go) {
					paths[i].group_id = current_turn;
					unassigned_count--;
				}
			}
		}
		current_turn++; // 这一轮能走的车安排完了，准备下一轮
	}
}

int main() {
	build_collision_matrix();
	schedule_traffic();
	// 打印最终的调度方案
	printf("==== 五岔路口智能信号灯调度方案 (C语言实现) ====\n\n");
	printf("[常绿通道 - 右转专用]:\n");
	for (int i = 0; i < TOTAL_PATHS; i++) {
		if (paths[i].group_id == 0) {
			printf("%s ", paths[i].name);
		}
	}
	printf("\n\n---------------------------------\n");
	// 找出最大的轮次
	int max_turn = 0;
	for (int i = 0; i < TOTAL_PATHS; i++) {
		if (paths[i].group_id > max_turn) max_turn = paths[i].group_id;
	}
	for (int t = 1; t <= max_turn; t++) {
		printf("[第 %d 轮绿灯放行]:\n", t);
		for (int i = 0; i < TOTAL_PATHS; i++) {
			if (paths[i].group_id == t) {
				printf("%s ", paths[i].name);
			}
		}
		printf("\n\n");
	}
	
	return 0;
}
