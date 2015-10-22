# # All Google Test headers.  Usually you shouldn't change this
# # definition.
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h

# # Flags passed to the preprocessor.
# # Set Google Test's header directory as a system directory, such that
# # the compiler doesn't generate warnings in Google Test headers.
CPPFLAGS += -isystem $(GTEST_DIR)/include
# 
# # Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -std=c++11 -pthread

CXX := clang++
SRCDIR := src
TESTDIR := test
BUILDDIR := build
EXECDIR := bin
INCLUDEDIR := include
TARGET := $(EXECDIR)/run
TEST_TARGET := $(EXECDIR)/test

SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
TESTS := $(shell find $(TESTDIR) -type f -name *_test.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
OBJECTS := $(filter-out build/main.o, $(OBJECTS))
TEST_OBJECTS := $(patsubst $(TESTDIR)/%,$(BUILDDIR)/%,$(TESTS:.$(SRCEXT)=.o))
CFLAGS := -Wall -pedantic -std=c++11 
LIB := -pthread
INC := -I $(INCLUDEDIR)
HEADERS := $(shell find $(INCLUDEDIR) -type f -name *.h)

TEST_TARGETS := $(EXECDIR)/$(basename $(notdir $(TESTS)))

# 
# sample1.o : $(USER_DIR)/sample1.cc $(USER_DIR)/sample1.h $(GTEST_HEADERS)
# 	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/sample1.cc
# 
# sample1_unittest.o : $(USER_DIR)/sample1_unittest.cc \
#                      $(USER_DIR)/sample1.h $(GTEST_HEADERS)
# 	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/sample1_unittest.cc
# 
# sample1_unittest : sample1.o sample1_unittest.o gtest_main.a
# 	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@
# 

$(TARGET):  $(OBJECTS)
	@echo "Linking..."
	$(CXX) $^ -o $@ $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT) $(GTEST_HEADERS)
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(INC) -c $< -o $@ 

test: $(TEST_TARGET)
	./bin/test

$(TEST_TARGET): $(TEST_OBJECTS) $(OBJECTS) build/gtest_main.a 
	@echo "Linking..."
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(INC) $^ -o $@ 

$(BUILDDIR)/%.o: $(TESTDIR)/%.$(SRCEXT) $(GTEST_HEADERS)
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(INC) -c $< -o $@ 

clean:
	@echo "Cleaning..."; 
	find $(BUILDDIR) $(EXECDIR) -type f -exec rm {} +

.PHONY: clean test

# # Where to find user code.
# USER_DIR = ../samples
# 
# 
# # All tests produced by this Makefile.  Remember to add new tests you
# # created to the list.
# TESTS = sample1_unittest
# 

# # House-keeping build targets.
# 
# all : $(TESTS)
# 
# clean :
# 	rm -f $(TESTS) gtest.a gtest_main.a *.o
# 
# # Builds gtest.a and gtest_main.a.
# 
# # Usually you shouldn't tweak such internal variables, indicated by a
# # trailing _.
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)
# 
# # For simplicity and to avoid depending on Google Test's
# # implementation details, the dependencies specified below are
# # conservative and not optimized.  This is fine as Google Test
# # compiles fast and for ordinary users its source rarely changes.
build/gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -o $@ -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest-all.cc 

build/gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -o $@ -I$(GTEST_DIR) $(CPPFLAGS) -c \
            $(GTEST_DIR)/src/gtest_main.cc

build/gtest.a : build/gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

build/gtest_main.a : build/gtest-all.o build/gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

# # Builds a sample test.  A test should link with either gtest.a or
# # gtest_main.a, depending on whether it defines its own main()
# # function.
# 
# sample1.o : $(USER_DIR)/sample1.cc $(USER_DIR)/sample1.h $(GTEST_HEADERS)
# 	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/sample1.cc
# 
# sample1_unittest.o : $(USER_DIR)/sample1_unittest.cc \
#                      $(USER_DIR)/sample1.h $(GTEST_HEADERS)
# 	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/sample1_unittest.cc
# 
# sample1_unittest : sample1.o sample1_unittest.o gtest_main.a
# 	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@
# 
