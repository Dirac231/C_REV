# Binary output settings
TARGET_EXEC := binary
BUILD_DIR := ./build

# Source & Include Directories
TARGET_EXEC := binary
BUILD_DIR := ./build

# Source & Include Directories
SRC_DIRS := ./src
INCLUDE_FLAGS := -I ./include

# External Libraries (Optional + GTK)
LD_FLAGS := -L ./lib -pg #-l[lib_name] -fopenmp
#GTK_LIBS := `pkg-config --libg gtk4`

# GCC compilation flags (Optimizations / Security / GTK)
CC= gcc
STDFLAGS := -pg -Og -Wall -Wextra -Wpedantic -Wformat -Wformat-security -Werror
#OPTFLAGS := -O3 -fno-math-errno -fno-trapping-math -flto -mtune=native -fopenmp
#SECFLAGS := -Wl,-z,relro -Wl,-z,now -Wl,-z,noexecstack -Wl,-z,separate-code -fPIE -pie -fstack-protector-strong
#GTKFLAGS := `pkg-config --cflags gtk4`

# --- BUILD PROCESS --- #
SRCS := $(shell find $(SRC_DIRS) -name '*.c')
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS); $(CC) $(OBJS) -o $@ $(LD_FLAGS) $(GTK_LIBS)

$(BUILD_DIR)/%.c.o: %.c; mkdir -p $(dir $@); $(CC) $(CFLAGS) $(STDFLAGS) $(OPTFLAGS) $(SECFLAGS) $(GTKFLAGS) $(INCLUDE_FLAGS) -c $< -o $@

-include $(DEPS)
# --------------------- #
