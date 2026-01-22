
优化器对比实验项目包
生成时间: 20251022_052534

包含内容:
1. 实验图表 (13个PNG文件)
2. 实验结果数据 (5个JSON文件)

实验设置:
- 数据集: CIFAR-10
- 模型: VGG11
- 优化器: SGD, SGD+Momentum, Adam, AdamW, RMSprop
- 训练轮次: 15 epochs

实验结果摘要:
- 最佳性能: Adam (88.80%)
- 良好性能: AdamW (88.23%), SGD+Momentum (87.57%)
- 较差性能: RMSprop (70.95%)

图表说明:
- test_accuracy_comparison.png: 测试准确率对比
- training_accuracy_comparison.png: 训练准确率对比
- training_loss_comparison.png: 训练损失对比
- best_accuracy_comparison.png: 最佳准确率柱状图
- training_time_comparison.png: 训练时间对比
- final_vs_best_accuracy.png: 最终vs最佳准确率
- optimizer_overview.png: 优化器性能概览
- efficiency_analysis.png: 效率分析散点图
- performance_radar.png: 多维度性能雷达图
- overfitting_analysis.png: 过拟合分析
- performance_ranking.png: 性能排名
- convergence_speed.png: 收敛速度分析
- loss_convergence.png: 损失收敛分析

用于: 人工智能课程期末报告
