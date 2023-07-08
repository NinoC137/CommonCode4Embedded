import matplotlib.pyplot as plt

# 解析路径点数据
x = []
y = []

with open("points.txt", "r") as file:
    for line in file:
        if line.strip().startswith("#"):
            continue  # 跳过注释行
        values = line.split("\t")
        x.append(float(values[1].split("=")[1].strip()))
        y.append(float(values[2].split("=")[1].strip()))

# 绘制散点图
plt.scatter(x, y, linewidths=1)
plt.xlabel("X")
plt.ylabel("Y")
plt.title("Bezier Curve")
plt.show()
