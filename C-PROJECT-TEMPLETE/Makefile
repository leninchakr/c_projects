# ---------- Toolchain ----------
CC      := gcc
CFLAGS  := -Wall -Wextra --std=c17 -Wpedantic -MMD -MP 
LDFLAGS :=

# ---------- Layout ----------
SRC_DIR   := src
BUILD_DIR := build
TARGET    := $(BUILD_DIR)/main

# ---------- Sources / objects ----------
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))
DEPS := $(OBJS:.o=.d)

# ---------- Phony ----------
.PHONY: all run clean checkpatch test_all test_single 

# ---------- Default ----------
all: $(TARGET)

# ---------- Link ----------
$(TARGET): $(OBJS)
	$(CC) $^ -o $@ $(LDFLAGS)

# ---------- Compile ----------
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -Iinclude -c $< -o $@

# ---------- Run ---------
run: $(TARGET)
	./$(TARGET)

# ---------- Clean ----------
clean:
	rm -rf $(BUILD_DIR)

# ---------- Kernel Syntax Check ----------
checkpatch:
	@echo "============================================="
	@echo " Running Linux Kernel Style Check..."
	@echo "============================================="
	@find src include test -maxdepth 3 -not -path "test/unity*" -type f \( -name "*.c" -o -name "*.h" \) 2>/dev/null \
		-exec ./scripts/checkpatch.pl --no-tree -f --strict {} + || true	
	@echo "============================================="
	@echo " Style verification complete."
	@echo "============================================="

# ---------- Unit Testing Automation ----------

# 1. Compile Unity framework core
$(BUILD_DIR)/test/unity/unity.o: test/unity/unity.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# 2. Find all your individual test files (e.g., test/test_math.c)
TEST_SOURCES := $(wildcard test/test_*.c)
# 3. Establish their runner target paths (e.g., build/test_math)
TEST_RUNNERS := $(patsubst test/test_%.c, $(BUILD_DIR)/test_%, $(TEST_SOURCES))

# 4. Compile the individual test source file into an object file
$(BUILD_DIR)/test/%.o: test/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -Itest/unity -Iinclude -c $< -o $@

# 5. Link Rule for each individual Test Runner Binary
# It automatically bundles: The specific test + Unity engine + ALL project source files (excluding main.o)
$(BUILD_DIR)/test_%: $(BUILD_DIR)/test/test_%.o $(BUILD_DIR)/test/unity/unity.o $(filter-out $(BUILD_DIR)/main.o, $(OBJS))
	@mkdir -p $(BUILD_DIR)
	$(CC) $^ -o $@ $(LDFLAGS)

# 6. Main execution command to loop over all runners sequentially
test_all: $(TARGET) $(TEST_RUNNERS)
	@echo "============================================="
	@echo " Executing Automated Unity Test Suites..."
	@echo "============================================="
	@for runner in $(TEST_RUNNERS); do \
		echo "---------------------------------------------"; \
		echo "Running suite: $$runner"; \
		echo "---------------------------------------------"; \
		./$$runner || exit 1; \
	done
	@echo "============================================="
	@echo " All tests passed successfully!"
	@echo "============================================="

# 7. Target to test a SINGLE file only
test_single: $(TARGET)
	@if [ -z "$(SUITE)" ]; then \
		echo "ERROR: Please specify a test suite name! Example: make test_single SUITE=spi_motor"; \
		exit 1; \
	fi
	@if [ ! -f "test/test_$(SUITE).c" ]; then \
		echo "ERROR: Test file test/test_$(SUITE).c does not exist!"; \
		exit 1; \
	fi
	@$(MAKE) $(BUILD_DIR)/test_$(SUITE)
	@echo "============================================="
	@echo " Running Single Test Suite: $(SUITE)"
	@echo "============================================="
	@./$(BUILD_DIR)/test_$(SUITE)


-include $(DEPS)

