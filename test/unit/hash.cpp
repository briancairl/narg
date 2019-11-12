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
#include <narg/hash.h>


using namespace narg;


TEST(Hash, Baseline)
{
  ASSERT_EQ(NARG_HASH("stack-overflow"), static_cast<std::size_t>(0x335CC04A));
}


TEST(Hash, DifferentValues)
{
  ASSERT_NE(NARG_HASH("test1"), NARG_HASH("test2"));
}


#endif  // DOXYGEN_SKIP
