CXX = g++
CXXFLAGS = -Wall -Wextra -O2
LDFLAGS = -lssl -lcrypto

# Основная программа
TARGET = client
SRCS = main.cpp interface.cpp file_io.cpp network.cpp authenticator.cpp
OBJS = $(SRCS:.cpp=.o)

# Тестовая программа
TEST_TARGET = test_runner
TEST_SRCS = test_runner.cpp test_file_io.cpp test_authenticator.cpp test_interface.cpp
TEST_OBJS = $(TEST_SRCS:.cpp=.o)
# Тестам нужны объектные файлы модулей (кроме main.o)
MODULES_OBJS = interface.o file_io.o network.o authenticator.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Сборка и запуск тестов
test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJS) $(MODULES_OBJS)
	$(CXX) $(CXXFLAGS) -o $(TEST_TARGET) $(TEST_OBJS) $(MODULES_OBJS) $(LDFLAGS)

clean:
	rm -f $(OBJS) $(TEST_OBJS) $(TARGET) $(TEST_TARGET) *.o *.bin

.PHONY: all clean test
