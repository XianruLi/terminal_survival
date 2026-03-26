# 1. 基础配置
CXX = x86_64-w64-mingw32-g++
CFLAGS = -static -static-libgcc -static-libstdc++ -Wall -fexec-charset=GBK

# 2. 定义文件夹（保持工程目录干净）
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

# 3. 核心魔法：自动扫描雷达
# 扫描 src 文件夹下所有的 .cpp 文件
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
# 把所有 .cpp 的名字替换成 .o，并告诉电脑这些 .o 应该放在 build 文件夹里
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

# 4. 最终产品
TARGET = $(BIN_DIR)/main.exe

# --- 下面是生产流水线规则 ---

# 默认指令：当你在终端敲 make 时，它会寻找 TARGET
all: $(TARGET)

# 规则 A：总装配线（把所有 .o 零件组装成 .exe）
$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJS) -o $(TARGET) $(CFLAGS)
	@echo "✅ 整车组装成功：$(TARGET)"

# 规则 B：零件加工机床（如何把单独的 .cpp 变成 .o）
# $< 代表当前的原材料 .cpp， $@ 代表当前要生成的零件 .o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) -c $< -o $@ $(CFLAGS)
	@echo "🔧 零件加工完毕：$@"

# 垃圾清理：删掉所有生成的零件和最终程序
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
	@echo "🧹 车间清理完毕！"
