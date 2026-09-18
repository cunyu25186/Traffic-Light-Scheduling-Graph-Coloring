#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

// 定义路线总数
const int NUM_ROUTES = 13;

// 路线名称映射表
const string route_names[NUM_ROUTES] = {
	"AB", "AC", "AD", // 0, 1, 2
	"BA", "BC", "BD", // 3, 4, 5
	"DA", "DB", "DC", // 6, 7, 8
	"EA", "EB", "EC", "ED" // 9, 10, 11, 12
};

// 冲突图邻接矩阵 (二维数组)
bool conflict_graph[NUM_ROUTES][NUM_ROUTES] = {false};

// 辅助函数：在图中添加双向冲突边
void add_conflict(int u, int v) {
	conflict_graph[u][v] = true;
	conflict_graph[v][u] = true;
}

// 初始化路口的冲突关系
void init_conflict_graph() {
	// 依据路口几何交叉规则录入冲突
	// AB(0) 冲突: BC, BD, DA, DB, EA
	add_conflict(0, 4); add_conflict(0, 5); add_conflict(0, 6); add_conflict(0, 7); add_conflict(0, 9);
	// AC(1) 冲突: BD, DA, DB, EA, EB
	add_conflict(1, 5); add_conflict(1, 6); add_conflict(1, 7); add_conflict(1, 9); add_conflict(1, 10);
	// AD(2) 冲突: EA, EB, EC
	add_conflict(2, 9); add_conflict(2, 10); add_conflict(2, 11);
	// BC(4) 冲突: DA, DB, EB, EC
	add_conflict(4, 6); add_conflict(4, 7); add_conflict(4, 10); add_conflict(4, 11);
	// BD(5) 冲突: EB, EC
	add_conflict(5, 10); add_conflict(5, 11);
	// DA(6) 冲突: EB, EC
	add_conflict(6, 10); add_conflict(6, 11);
	// DB(7) 冲突: EB, EC
	add_conflict(7, 10); add_conflict(7, 11);
	// 注意: 右转路线 BA(3), DC(8), ED(12) 无冲突
}

int main() {
	init_conflict_graph();
	
	// color 数组记录每条路线的红绿灯相位 (-1 表示未分配)
	vector<int> color(NUM_ROUTES, -1);
	
	// 1. 现实规则优化：提取右转常绿通道 (不受红绿灯限制)
	color[3] = 0;  // BA
	color[8] = 0;  // DC
	color[12] = 0; // ED
	
	int current_phase = 1; // 当前绿灯相位
	
	// 2. 贪心图着色算法：为剩余路线分配相位
	for (int i = 0; i < NUM_ROUTES; i++) {
		if (color[i] != -1) continue; // 如果已经分配了相位，直接跳过
		
		color[i] = current_phase; // 给当前路线分配当前绿灯相位
		
		// 尝试将当前绿灯相位尽可能多地分配给后续不冲突的路线
		for (int j = i + 1; j < NUM_ROUTES; j++) {
			if (color[j] == -1) {
				bool is_safe = true;
				// 检查 j 路线是否与当前相位内已有的路线发生冲突
				for (int k = 0; k < NUM_ROUTES; k++) {
					if (color[k] == current_phase && conflict_graph[j][k]) {
						is_safe = false;
						break;
					}
				}
				// 如果安全(不冲突)，则加入当前绿灯相位
				if (is_safe) {
					color[j] = current_phase;
				}
			}
		}
		current_phase++; // 当前相位分配满载，开启下一轮绿灯
	}
	
	// 3. 打印精美的输出结果
	cout << "========================================" << endl;
	cout << "  多岔路口交通信号灯调度方案 (C++ 版本) " << endl;
	cout << "========================================" << endl;
	
	cout << " [相位 0] (右转常绿通道): ";
	for (int i = 0; i < NUM_ROUTES; i++) {
		if (color[i] == 0) cout << route_names[i] << " ";
	}
	cout << "\n----------------------------------------" << endl;
	
	for (int p = 1; p < current_phase; p++) {
		cout << "[相位 " << p << "] (绿灯通行): ";
		for (int i = 0; i < NUM_ROUTES; i++) {
			if (color[i] == p) {
				cout << route_names[i] << " ";
			}
		}
		cout << endl;
	}
	cout << "========================================" << endl;
	
	return 0;
}
