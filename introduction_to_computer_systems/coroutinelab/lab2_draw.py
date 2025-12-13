import numpy as np
import matplotlib.pyplot as plt

# 设置中文字体支持
plt.rcParams['font.sans-serif'] = ['SimHei']  # 用来正常显示中文标签
plt.rcParams['axes.unicode_minus'] = False  # 用来正常显示负号

# 从REPORT.md中提取的数据
l_values = [32, 31, 30, 29, 28, 27, 26, 25]
naive_times = [1916.28, 1573.48, 1243.18, 1083.21, 856.78, 700.42, 458.00, 264.83]  # ns per search
coroutine_times = [1798.87, 1148.15, 906.62, 862.71, 719.79, 646.79, 570.04, 545.80]  # ns per search

# 创建图表
plt.figure(figsize=(10, 6))

# 绘制两种方法的曲线
plt.plot(l_values, naive_times, marker='o', linestyle='-', color='blue', label='naive')
plt.plot(l_values, coroutine_times, marker='s', linestyle='-', color='red', label='coroutine batched')

# 设置图表标题和坐标轴标签
plt.title('不同数据表大小（l值）下的搜索性能对比')
plt.xlabel('数据表大小对数 l (表大小为 2^l)')
plt.ylabel('每次搜索平均时间 (ns)')

# 添加图例
plt.legend()

# 添加网格线
plt.grid(True, linestyle='--', alpha=0.7)

# 保存图表
plt.savefig('performance_comparison.png', dpi=300, bbox_inches='tight')

# 显示图表
plt.show()
import numpy as np
import matplotlib.pyplot as plt

# 设置中文字体支持
plt.rcParams['font.sans-serif'] = ['SimHei']  # 用来正常显示中文标签
plt.rcParams['axes.unicode_minus'] = False  # 用来正常显示负号

# 从REPORT.md中提取的数据（batch size与时间的关系）
batch_sizes = [2, 3, 4, 6, 8, 10, 12, 14, 20, 24, 25, 26, 28, 30]
naive_times = [1139.52, 1078.92, 1101.96, 1133.27, 1111.98, 1259.86, 1269.12, 1222.42, 1263.94, 1256.79, 1254.39,
               1257.96, 1273.15, 1272.31]
coroutine_times = [1564.78, 1280.65, 1074.24, 1066.18, 1058.17, 1075.88, 1066.82, 1055.84, 1090.26, 1085.42, 1084.75,
                   1387.68, 1562.37, 1662.57]

# 创建图表
plt.figure(figsize=(12, 6))

# 绘制两种方法的曲线
plt.plot(batch_sizes, naive_times, marker='o', linestyle='-', color='blue', label='naive')
plt.plot(batch_sizes, coroutine_times, marker='s', linestyle='-', color='red', label='coroutine batched')

# 设置图表标题和坐标轴标签
plt.title('数据表大小 l=31 时不同 Batch 大小的搜索性能对比')
plt.xlabel('Batch 大小 (b)')
plt.ylabel('每次搜索平均时间 (ns)')

# 添加图例
plt.legend()

# 添加网格线
plt.grid(True, linestyle='--', alpha=0.7)

# 保存图表
plt.savefig('batch_performance_comparison.png', dpi=300, bbox_inches='tight')

# 显示图表
plt.show()





