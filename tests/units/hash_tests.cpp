#include "bayan/hash/crc32.hpp"
#include "bayan/hash/md5.hpp"

#include <gtest/gtest.h>

#include <array>
#include <cstdint>

// --- crc32 ---

TEST(Crc32, SameInputSameOutput) {
  const char data[] = "hello";
  EXPECT_EQ(bayan::crc32(data, sizeof(data) - 1), bayan::crc32(data, sizeof(data) - 1));
}

TEST(Crc32, DifferentInputDifferentOutput) {
  EXPECT_NE(bayan::crc32("hello", 5), bayan::crc32("world", 5));
}

TEST(Crc32, KnownValue) {
  // CRC32 of "123456789" is 0xCBF43926
  EXPECT_EQ(bayan::crc32("123456789", 9), 0xCBF43926u);
}

TEST(Crc32, EmptyInput) {
  EXPECT_EQ(bayan::crc32("", 0), bayan::crc32("", 0));
}

// --- md5 ---

TEST(Md5, SameInputSameOutput) {
  const char data[] = "hello";
  EXPECT_EQ(bayan::md5(data, sizeof(data) - 1), bayan::md5(data, sizeof(data) - 1));
}

TEST(Md5, DifferentInputDifferentOutput) {
  EXPECT_NE(bayan::md5("hello", 5), bayan::md5("world", 5));
}

TEST(Md5, Returns128Bits) {
  auto digest = bayan::md5("test", 4);
  EXPECT_EQ(digest.size(), 4u);
}

TEST(Md5, KnownValue) {
  // MD5("") = d41d8cd98f00b204e9800998ecf8427e
  auto digest = bayan::md5("", 0);
  EXPECT_EQ(digest[0], 0xd41d8cd9u);
  EXPECT_EQ(digest[1], 0x8f00b204u);
  EXPECT_EQ(digest[2], 0xe9800998u);
  EXPECT_EQ(digest[3], 0xecf8427eu);
}
