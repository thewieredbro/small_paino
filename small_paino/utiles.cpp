inline int
clamp(int min, int val, int max) {
    if (val < min) return min;
    if (val > max) return max;
    return val;
}

#define global_variable static
#define internal static