// TexMol Unit Tests — CVCUtility free functions
//
// Covers the pure-function helpers in inc/Utility/utility.h that were
// previously untested. These are foundational string/coersion routines
// used throughout the codebase.

#include <Utility/utility.h>
#include <gtest/gtest.h>

using namespace CVCUtility;

// -----------------------------------------------------------------------
// strcmpCaseInsensitive
// -----------------------------------------------------------------------

TEST(StrcmpCaseInsensitiveTest, EqualLowercase) {
  EXPECT_TRUE(strcmpCaseInsensitive("hello", "hello"));
}

TEST(StrcmpCaseInsensitiveTest, EqualMixedCase) {
  EXPECT_TRUE(strcmpCaseInsensitive("Hello", "hELLO"));
  EXPECT_TRUE(strcmpCaseInsensitive("ABC", "abc"));
}

TEST(StrcmpCaseInsensitiveTest, EqualEmpty) {
  EXPECT_TRUE(strcmpCaseInsensitive("", ""));
}

TEST(StrcmpCaseInsensitiveTest, DifferentLength) {
  EXPECT_FALSE(strcmpCaseInsensitive("hello", "hell"));
  EXPECT_FALSE(strcmpCaseInsensitive("hell", "hello"));
}

TEST(StrcmpCaseInsensitiveTest, DifferentContent) {
  EXPECT_FALSE(strcmpCaseInsensitive("hello", "world"));
}

// -----------------------------------------------------------------------
// beginsWith (C-string and std::string overloads)
// -----------------------------------------------------------------------

TEST(BeginsWithTest, CStringMatchingPrefix) {
  EXPECT_TRUE(beginsWith("hello world", "hello"));
}

TEST(BeginsWithTest, CStringEntireString) {
  EXPECT_TRUE(beginsWith("hello", "hello"));
}

TEST(BeginsWithTest, CStringNotPrefix) {
  EXPECT_FALSE(beginsWith("hello world", "world"));
}

TEST(BeginsWithTest, CStringEmptyPrefix) {
  EXPECT_TRUE(beginsWith("hello", ""));
}

TEST(BeginsWithTest, CStringPrefixLongerThanString) {
  EXPECT_FALSE(beginsWith("hi", "hello"));
}

TEST(BeginsWithTest, StdStringMatchingPrefix) {
  EXPECT_TRUE(beginsWith(std::string("filename.pdb"), std::string("filename")));
}

TEST(BeginsWithTest, StdStringNotPrefix) {
  EXPECT_FALSE(beginsWith(std::string("filename.pdb"), std::string(".pdb")));
}

// -----------------------------------------------------------------------
// endsWith
// -----------------------------------------------------------------------

TEST(EndsWithTest, MatchingSuffix) {
  EXPECT_TRUE(endsWith(std::string("filename.pdb"), std::string(".pdb")));
}

TEST(EndsWithTest, EntireString) {
  EXPECT_TRUE(endsWith(std::string("pdb"), std::string("pdb")));
}

TEST(EndsWithTest, NotSuffix) {
  EXPECT_FALSE(endsWith(std::string("filename.pdb"), std::string(".pqr")));
}

TEST(EndsWithTest, SuffixLongerThanString) {
  EXPECT_FALSE(endsWith(std::string("hi"), std::string("hello")));
}

TEST(EndsWithTest, EmptySuffix) {
  EXPECT_TRUE(endsWith(std::string("hello"), std::string("")));
}

// -----------------------------------------------------------------------
// substring
// -----------------------------------------------------------------------

TEST(SubstringTest, MatchInTheMiddle) {
  EXPECT_TRUE(substring(std::string("the quick brown fox"), std::string("quick")));
}

// NB: the current impl is `myString.find(mySubstring) > 0`, which returns
// false when the needle is found at position 0. Pin that quirky behavior
// so a future cleanup catches it deliberately rather than silently.
TEST(SubstringTest, MatchAtStartReturnsFalse_KnownQuirk) {
  EXPECT_FALSE(substring(std::string("hello world"), std::string("hello")));
}

TEST(SubstringTest, MatchAtEnd) {
  EXPECT_TRUE(substring(std::string("hello world"), std::string("world")));
}

TEST(SubstringTest, NoMatchReturnsTrue_KnownBug) {
  // The impl is `myString.find(mySubstring) > 0` and find() returns
  // std::string::npos (size_t max) when the needle is absent, which is
  // also > 0 — so the function reports a match for missing needles too.
  // This makes substring() effectively useless; pinning the behavior so
  // a future fix is intentional.
  EXPECT_TRUE(substring(std::string("hello"), std::string("xyz")));
}

TEST(SubstringTest, NoMatchTooShortReturnsFalse) {
  // Only the explicit length guard saves this case.
  EXPECT_FALSE(substring(std::string("hi"), std::string("xyz")));
}

TEST(SubstringTest, EmptyNeedleReturnsFalse_KnownQuirk) {
  // std::string::find("") returns 0; combined with the `> 0` check this
  // means an empty needle reports "not a substring". Same quirk as above.
  EXPECT_FALSE(substring(std::string("hello"), std::string("")));
}

// -----------------------------------------------------------------------
// stringToInt
// -----------------------------------------------------------------------

TEST(StringToIntTest, PositiveInteger) {
  EXPECT_EQ(stringToInt(std::string("42")), 42);
}

TEST(StringToIntTest, NegativeInteger) {
  EXPECT_EQ(stringToInt(std::string("-17")), -17);
}

TEST(StringToIntTest, Zero) {
  EXPECT_EQ(stringToInt(std::string("0")), 0);
}

// -----------------------------------------------------------------------
// stringToChar
// -----------------------------------------------------------------------

TEST(StringToCharTest, SingleCharacter) {
  EXPECT_EQ(stringToChar(std::string("A")), 'A');
}

TEST(StringToCharTest, FirstCharacterOfMulti) {
  // Implementation takes the first character of the string.
  EXPECT_EQ(stringToChar(std::string("xyz")), 'x');
}

// -----------------------------------------------------------------------
// minimum / maximum
// -----------------------------------------------------------------------

TEST(MinMaxTest, MinimumOrdered) {
  EXPECT_EQ(minimum(3, 7), 3);
  EXPECT_EQ(minimum(7, 3), 3);
}

TEST(MinMaxTest, MinimumEqual) {
  EXPECT_EQ(minimum(5, 5), 5);
}

TEST(MinMaxTest, MinimumNegative) {
  EXPECT_EQ(minimum(-3, 2), -3);
  EXPECT_EQ(minimum(-10, -5), -10);
}

TEST(MinMaxTest, MaximumOrdered) {
  EXPECT_EQ(maximum(3, 7), 7);
  EXPECT_EQ(maximum(7, 3), 7);
}

TEST(MinMaxTest, MaximumEqual) {
  EXPECT_EQ(maximum(5, 5), 5);
}

TEST(MinMaxTest, MaximumNegative) {
  EXPECT_EQ(maximum(-3, 2), 2);
  EXPECT_EQ(maximum(-10, -5), -5);
}
