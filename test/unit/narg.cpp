/**
 * @copyright 2019
 * @author Brian Cairl
 */
#ifndef DOXYGEN_SKIP

// C++ Standard Library
#include <chrono>
#include <memory>
#include <thread>
#include <vector>

// GTest
#include <gtest/gtest.h>

// NArg
#include <narg/narg.h>


using namespace narg;


struct TestObject
{
  NARG_DECL_CONSTRUCTOR(TestObject,
                        arg_pack,
                        NARG_SPEC("argument_1", int),
                        NARG_SPEC("argument_2", std::string)) :
    filled_it_1{NARG_GET(arg_pack, "argument_1")},
    filled_it_2{NARG_GET(arg_pack, "argument_2")},
    filled_it_3{NARG_GET(arg_pack, "argument_1")}
  {}

  int filled_it_1;
  std::string filled_it_2;
  const int filled_it_3;
};


TEST(NArg, NamedArgsMissing)
{
  TestObject test_object{use_nargs,
                         NARG_FILL("argument_1", 1)};

  ASSERT_EQ(test_object.filled_it_1, 1);
  ASSERT_EQ(test_object.filled_it_2, "");
}


TEST(NArg, NamedArgsOrdered)
{
  TestObject test_object{use_nargs,
                         NARG_FILL("argument_1", 1),
                         NARG_FILL("argument_2", "VALUE")};

  ASSERT_EQ(test_object.filled_it_1, 1);
  ASSERT_EQ(test_object.filled_it_2, "VALUE");
}



TEST(NArg, NamedArgsUnordered)
{
  const std::string VALUE{"VALUE"};

  TestObject test_object{use_nargs,
                         NARG_FILL("argument_2", VALUE),
                         NARG_FILL("argument_1", 2)};

  ASSERT_EQ(test_object.filled_it_2, "VALUE");
  ASSERT_EQ(test_object.filled_it_1, 2);
  ASSERT_EQ(test_object.filled_it_3, 2);
}



TEST(NArg, NamedArgsAllPositional)
{
  const std::string VALUE{"VALUE"};

  TestObject test_object{use_nargs, 2, VALUE};

  ASSERT_EQ(test_object.filled_it_2, "VALUE");
  ASSERT_EQ(test_object.filled_it_1, 2);
}



TEST(NArg, NamedArgsMixedPositional)
{
  const std::string VALUE{"VALUE"};

  TestObject test_object{use_nargs,
                         2,
                         NARG_FILL("argument_2", VALUE)};

  ASSERT_EQ(test_object.filled_it_2, "VALUE");
  ASSERT_EQ(test_object.filled_it_1, 2);
}


TEST(NArg, NamedArgsMixedPositionalWithRepeats)
{
  TestObject test_object{use_nargs,
                         2,
                         NARG_FILL("argument_1", 1),
                         NARG_FILL("argument_1", 4),
                         NARG_FILL("argument_1", 10)};

  ASSERT_EQ(test_object.filled_it_1, 2);
}


TEST(NArg, NamedArgsMove)
{
  std::string VALUE{"VALUE"};

  TestObject test_object{use_nargs,
                         2,
                         NARG_FILL("argument_2", std::move(VALUE))};

  ASSERT_EQ(test_object.filled_it_2, "VALUE");
  ASSERT_EQ(test_object.filled_it_1, 2);
}

#endif  // DOXYGEN_SKIP
