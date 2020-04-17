# Executable names
CLI_TARGET   = ConsoleApp
SRV_TARGET   = RFAsrv
CC       = g++
LINKER   = g++

# Compile flags
# CFLAGS   = -std=c99 -Wall -I.
# Linking flags
LFLAGS   = -lstdc++fs -pthread

# Directories
SRC_DIR   = src
CLI_DIR = client
SRV_DIR = server
CMN_DIR =common
OBJ_DIR   = obj
BIN_DIR   = bin
INC_DIR   = include

CLI_SRC  := $(wildcard $(SRC_DIR)/$(CLI_DIR)/*.cpp)
SRV_SRC  := $(wildcard $(SRC_DIR)/$(SRV_DIR)/*.cpp)
CMN_SRC  := $(wildcard $(SRC_DIR)/$(CMN_DIR)/*.cpp)

CLI_OBJ  := $(CLI_SRC:$(SRC_DIR)/$(CLI_DIR)/%.cpp=$(OBJ_DIR)/$(CLI_DIR)/%.o)
SRV_OBJ  := $(SRV_SRC:$(SRC_DIR)/$(SRV_DIR)/%.cpp=$(OBJ_DIR)/$(SRV_DIR)/%.o)
CMN_OBJ  := $(CMN_SRC:$(SRC_DIR)/$(CMN_DIR)/%.cpp=$(OBJ_DIR)/$(CMN_DIR)/%.o)

CLI_BIN  := $(BIN_DIR)/$(CLI_TARGET)
SRV_BIN  := $(BIN_DIR)/$(SRV_TARGET)

rm        = rm -f
mkdir     = mkdir

all: $(CLI_BIN) $(SRV_BIN)

$(CLI_BIN): $(CLI_OBJ) $(CMN_OBJ)
	@echo "SUCCESS: Client program source files compiled."
	@$(LINKER) $(CLI_OBJ) $(CMN_OBJ) -o $@ $(LFLAGS)
	@echo "SUCCESS: Client program linked."

$(SRV_BIN): $(SRV_OBJ) $(CMN_OBJ)
	@echo "SUCCESS: Server program source files compiled."
	@$(LINKER) $(SRV_OBJ) $(CMN_OBJ) -o $@ $(LFLAGS)
	@echo "SUCCESS: Server program linked."

$(CLI_OBJ): $(OBJ_DIR)/$(CLI_DIR)/%.o : $(SRC_DIR)/$(CLI_DIR)/%.cpp
	@$(CC) -I $(INC_DIR) -c $< -o $@
	@echo "SUCCESS: "$<" compiled."

$(SRV_OBJ): $(OBJ_DIR)/$(SRV_DIR)/%.o : $(SRC_DIR)/$(SRV_DIR)/%.cpp
	@$(CC) -I $(INC_DIR) -c $< -o $@
	@echo "SUCCESS: "$<" compiled."

$(CMN_OBJ): $(OBJ_DIR)/$(CMN_DIR)/%.o : $(SRC_DIR)/$(CMN_DIR)/%.cpp
	@$(CC) -I $(INC_DIR) -c $< -o $@
	@echo "SUCCESS: "$<" compiled."

.PHONY: clean
clean:
	@$(rm) $(CLI_OBJ)
	@$(rm) $(SRV_OBJ)
	@$(rm) $(CMN_OBJ)
	@echo "SUCCESS: Cleared objects."
	@$(rm) $(BIN_DIR)/$(CLI_TARGET)
	@$(rm) $(BIN_DIR)/$(SRV_TARGET)
	@echo "SUCCESS: Cleared target executables."

