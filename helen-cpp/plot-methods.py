import matplotlib.pyplot as plt
import numpy as np

# Data
hamilton = [0.687515, 0.687515, 0.687515, 0.687515, 0.687515, 0.687515, 0.687515, 0.687515, 0.687515, 0.687515, 0.687515, 0.767671, 0.687515, 0.695132, 0.687515, 0.910801, 0.912925, 0.931409, 0.916377, 0.918788, 0.961437, 0.956774, 0.958016, 0.962036, 0.960193, 0.969951, 0.963564, 0.969594, 0.96722, 0.969989, 0.98128, 0.980159, 0.977884, 0.977021, 0.980159, 0.985213, 0.985177, 0.985881, 0.985986, 0.984616, 0.989375, 0.988727, 0.989599, 0.988434, 0.989907, 1, 1, 1, 1, 1]

jefferson = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.65075, 0.630336, 0.537226, 0.65947, 0.631373, 0.762195, 0.762195, 0.816096, 0.779629, 0.795051, 0.863274, 0.866421, 0.849295, 0.869769, 0.866451, 0.896383, 0.902804, 0.905491, 0.896383, 0.904709, 0.928461, 0.931485, 0.927176, 0.930372, 0.933468, 0.947199, 0.948523, 0.950589, 0.949844, 0.946766, 1, 1, 1, 1, 1]

webster = [0.687515, 0.687515, 0.687515, 0.687515, 0.687515, 0.687515, 0.687515, 0.687515, 0.687515, 0.687515, 0.714309, 0.687515, 0.702921, 0.695132, 0.687515, 0.922466, 0.919185, 0.888108, 0.911794, 0.926694, 0.947056, 0.947056, 0.935813, 0.953654, 0.93898, 0.965784, 0.964764, 0.967342, 0.963564, 0.965751, 0.973243, 0.975077, 0.974724, 0.974471, 0.97587, 0.982085, 0.982237, 0.980478, 0.982237, 0.980878, 0.988555, 0.988078, 0.988123, 0.988318, 0.987047, 1, 1, 1, 1, 1]

adams = [0.944745, 0.944745, 0.944745, 0.944745, 0.944745, 0.944745, 0.944745, 0.944745, 0.944745, 0.944745, 0.944745, 0.944745, 0.948812, 0.944745, 0.949219, 0.957872, 0.957919, 0.95651, 0.956702, 0.956118, 0.964704, 0.963164, 0.96546, 0.963903, 0.964245, 0.970723, 0.969458, 0.97001, 0.97068, 0.969691, 0.975291, 0.976128, 0.974986, 0.974905, 0.975764, 0.980069, 0.980435, 0.979465, 0.980511, 0.980232, 0.985613, 0.985653, 0.985099, 0.984923, 0.985629, 1, 1, 1, 1, 1]

huntington_hill = [0.702921, 0.702921, 0.702921, 0.702921, 0.702921, 0.702921, 0.702921, 0.702921, 0.702921, 0.702921, 0.743276, 0.702921, 0.702921, 0.791126, 0.722535, 0.926637, 0.918698, 0.916848, 0.938103, 0.938821, 0.959155, 0.959693, 0.958191, 0.956107, 0.956583, 0.968391, 0.971824, 0.971989, 0.970969, 0.968391, 0.978779, 0.979734, 0.978691, 0.978664, 0.979942, 0.98236, 0.98236, 0.984313, 0.98236, 0.985218, 0.989392, 0.989368, 0.988745, 0.988252, 0.989392, 1, 1, 1, 1, 1]

# Reshape data into groups of 5 and calculate mean and std
def group_stats(data, group_size=5):
    data_array = np.array(data)
    n_groups = len(data_array) // group_size
    groups = data_array[:n_groups * group_size].reshape(n_groups, group_size)
    means = groups.mean(axis=1)
    stds = groups.std(axis=1)
    return means, stds

# Calculate statistics for each method
x_values = np.arange(0.1, 1.1, 0.1)  # 0.1, 0.2, ..., 1.0

hamilton_mean, hamilton_std = group_stats(hamilton)
jefferson_mean, jefferson_std = group_stats(jefferson)
webster_mean, webster_std = group_stats(webster)
adams_mean, adams_std = group_stats(adams)
hh_mean, hh_std = group_stats(huntington_hill)

# Create beautiful plot with pretty colors
plt.figure(figsize=(12, 7))

# Pretty color palette (vibrant but professional)
colors = {
    'hamilton': '#E74C3C',      # Coral red
    'jefferson': '#3498DB',     # Bright blue
    'webster': '#2ECC71',       # Emerald green
    'adams': '#9B59B6',         # Amethyst purple
    'huntington': '#F39C12'     # Orange
}

# Plot with error bars
plt.errorbar(x_values, hamilton_mean, yerr=hamilton_std, marker='o', 
             label='Hamilton', color=colors['hamilton'], linewidth=2, 
             markersize=8, capsize=5, capthick=2, alpha=0.85)

plt.errorbar(x_values, jefferson_mean, yerr=jefferson_std, marker='s', 
             label='Jefferson', color=colors['jefferson'], linewidth=2, 
             markersize=8, capsize=5, capthick=2, alpha=0.85)

plt.errorbar(x_values, webster_mean, yerr=webster_std, marker='^', 
             label='Webster', color=colors['webster'], linewidth=2, 
             markersize=8, capsize=5, capthick=2, alpha=0.85)

plt.errorbar(x_values, adams_mean, yerr=adams_std, marker='d', 
             label='Adams', color=colors['adams'], linewidth=2, 
             markersize=8, capsize=5, capthick=2, alpha=0.85)

plt.errorbar(x_values, hh_mean, yerr=hh_std, marker='*', 
             label='Huntington-Hill', color=colors['huntington'], linewidth=2, 
             markersize=10, capsize=5, capthick=2, alpha=0.85)

# Styling
plt.xlabel('Parameter Value', fontsize=14, fontweight='bold')
plt.ylabel('Alpha Value', fontsize=14, fontweight='bold')
plt.title('Apportionment Methods: Alpha Values by Parameter', 
          fontsize=16, fontweight='bold', pad=20)
plt.legend(loc='best', fontsize=11, framealpha=0.95)
plt.grid(True, alpha=0.3, linestyle='--', linewidth=0.8)
plt.xlim(0.05, 1.05)
plt.ylim(-0.05, 1.05)

# Add subtle background color
ax = plt.gca()
ax.set_facecolor('#F8F9FA')
plt.gcf().patch.set_facecolor('white')

plt.tight_layout()
plt.show()