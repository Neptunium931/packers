CXX ?= clang++
CXXFLAGS = -std=c++23 -Wall -Wextra -Werror -pedantic -g
CXXFLAGS += -I ./vendor/argparse/include/
CXXFLAGS += -I ./vendor/tomlplusplus/include/ -isystem ./vendor/tomlplusplus/include/
CXXFLAGS += -I ./src

CriterionFLAGS = -lcriterion

file := src/packers.cpp \
				src/project/file.cpp \
				src/executor/executor.cpp \
				src/executor/service.cpp \
				src/project/buildArgs.cpp
obj := $(file:.cpp=.cpp.o)
obj := $(obj:src/%=build/build-debug/src/%)

packers-test-src := tests/packersTest.cpp \
								tests/executorTest.cpp \
								tests/serviceTest.cpp \
								src/project/file.cpp \
								src/executor/executor.cpp \
								src/executor/service.cpp \
								tests/exampleTest.cpp \
								tests/buildArgs.cpp \
								src/project/buildArgs.cpp
packers-test2 = $(packers-test-src:.cpp=.cpp.o)
packers-test1 = $(packers-test2:tests/%=build/build-debug/tests/%)
packers-test = $(packers-test1:src/%=build/build-debug/src/%)

ifdef CODECOV
	CXXFLAGS += --coverage
endif

ifdef OPTIMIZE
	CXXFLAGS += -O3
else
	CXXFLAGS += -O0
endif

ifeq ($(findstring clang++, $(CXX)), clang++)
	CXXFLAGS += -Weverything \
							-Wno-c++98-compat \
							-Wno-c++98-compat-extra-semi \
							-Wno-c++98-compat-pedantic \
							-Wno-documentation-unknown-command \
							-Wno-error=padded

else
	CXXFLAGS += -Werror \
							-Wshadow \
							-Wnon-virtual-dtor \
							-Wold-style-cast \
							-Wcast-align \
							-Wunused \
							-Woverloaded-virtual \
							-Wpedantic \
							-Wconversion \
							-Wsign-conversion \
							-Wmisleading-indentation \
							-Wduplicated-cond \
							-Wduplicated-branches \
							-Wlogical-op \
							-Wnull-dereference \
							-Wuseless-cast \
							-Wdouble-promotion \
							-Wformat=2 \
							-Wnrvo
endif

check: packers $(packers-test)
	$(CXX) $(CXXFLAGS) $(CriterionFLAGS) -o ./build/build-debug/packers-test $(packers-test)
	./build/build-debug/packers-test --tap

packers: $(obj)
	$(CXX) $(CXXFLAGS) -o packers $^

build/build-debug/%.cpp.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f packers $(obj)

.PHONY: format
format:
	find src tests -name '*.hpp' -o -name '*.cpp' | xargs clang-format -i

fmt: format

clang-tidy:
	clang-tidy --config-file=.clang-tidy -p . --fix $(packers-test-src) ./src/packers.cpp
