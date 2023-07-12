import matplotlib.pyplot as plt

# 解析路径点数据
x = []
y = []

with open("FFT_points.txt", "r") as file:
    for line in file:
        if line.strip().startswith("#"):
            continue  # 跳过注释行
        values = line.split("\t")
        x.append(float(values[1].split("=")[1].strip()))
        y.append(float(values[2].split("=")[1].strip()))

# 绘制频谱图
plt.figure(figsize=(8, 6))
plt.plot(x, y)
#plt.scatter(x, y, linewidths=1)
plt.xlabel("Frequency")
plt.ylabel("Magnitude")
plt.title("FFT Spectrum")
plt.grid(True)
plt.show()