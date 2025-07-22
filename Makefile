CXX = clang++
CXXFLAGS = -std=c++23 -Wall -Wextra -O0 -g
CXXFLAGS += -I ./build/build-debug/dependencies/argparse/include
CXXFLAGS += -I ./build/build-debug/dependencies/tomlplusplus/include
CXXFLAGS += -I ./src

CriterionFLAGS = -lcriterion

file := src/packers.cpp \
				src/project/file.cpp
obj := $(file:.cpp=.cpp.o)
obj := $(obj:src/%=build/build-debug/%)

packers-test := tests/packers-test.cpp \
								src/project/file.cpp

check: packers $(packers-test)
	$(CXX) $(CXXFLAGS) $(CriterionFLAGS) -o ./build/build-debug/packers-test $(packers-test)
	./build/build-debug/packers-test --tap

packers: $(obj)
	$(CXX) $(CXXFLAGS) -o packers $^

build/build-debug/%.cpp.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f packers $(obj)

.PHONY: format
format:
	find src -name '*.hpp' -o -name '*.cpp' | xargs clang-format -i

fmt: format

clang-tidy:
	clang-tidy -p . -fix $(file) $(packers-test)
