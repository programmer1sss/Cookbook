#include <gtest/gtest.h>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <stdexcept>
#include "recipe.h"
#include "cook.h"

TEST(CookCtor, DefaultAndFull) {
    Cook c;
    EXPECT_EQ(c.getName(), "(без імені)");
    Cook c2{"Іван", "українська"};
    EXPECT_EQ(c2.getName(), "Іван");
    EXPECT_EQ(c2.getSpecialty(), "українська");
}

TEST(CookCtor, ExplicitString) {
    static_assert(!std::is_convertible_v<std::string, Cook>);
    Cook c{std::string{"Петро"}};
    EXPECT_EQ(c.getName(), "Петро");
    EXPECT_EQ(c.getSpecialty(), "(універсал)");
}

TEST(CookOps, AddRecipe) {
    Cook c{"Іван", "українська"};
    c.addRecipe(Recipe{"Борщ", "українська", 120, 4});
    EXPECT_EQ(c.recipeCount(), 1u);
}

TEST(RecipeCtor, DefaultIsValid) {
    Recipe r;
    EXPECT_TRUE(r.checkInvariant());
    EXPECT_EQ(r.getName(), "(без назви)");
    EXPECT_EQ(r.getServings(), 1);
}

TEST(RecipeCtor, FullIsValid) {
    Recipe r{"Борщ", "українська", 120, 4};
    EXPECT_TRUE(r.checkInvariant());
    EXPECT_EQ(r.getName(), "Борщ");
    EXPECT_EQ(r.getCuisine(), "українська");
    EXPECT_EQ(r.getMinutes(), 120);
    EXPECT_EQ(r.getServings(), 4);
}

TEST(RecipeCtor, ShortDelegates) {
    Recipe r{"Паста"};
    EXPECT_TRUE(r.checkInvariant());
    EXPECT_EQ(r.getName(), "Паста");
    EXPECT_EQ(r.getCuisine(), "(невідома кухня)");
    EXPECT_EQ(r.getMinutes(), 30);
    EXPECT_EQ(r.getServings(), 1);
}

TEST(RecipeCtor, NotImplicitlyConvertible) {
    static_assert(!std::is_convertible_v<std::string, Recipe>);
    static_assert(std::is_constructible_v<Recipe, std::string>);
    SUCCEED();
}

TEST(RecipeCtor, EmptyNameThrows) {
    EXPECT_THROW(Recipe("", "укр", 30, 1), std::invalid_argument);
}

TEST(RecipeCtor, EmptyCuisineThrows) {
    EXPECT_THROW(Recipe("Борщ", "", 30, 1), std::invalid_argument);
}

TEST(RecipeCtor, MinutesOutOfRangeThrows) {
    EXPECT_THROW(Recipe("X", "укр", 0, 1), std::out_of_range);
    EXPECT_THROW(Recipe("X", "укр", 1441, 1), std::out_of_range);
}

TEST(RecipeCtor, ZeroServingsThrows) {
    EXPECT_THROW(Recipe("X", "укр", 30, 0), std::invalid_argument);
}

TEST(RecipeOps, AddReduceServings) {
    Recipe r{"X", "укр", 30, 2};
    EXPECT_TRUE(r.addServings(3));
    EXPECT_EQ(r.getServings(), 5);
    EXPECT_TRUE(r.reduceServings(2));
    EXPECT_EQ(r.getServings(), 3);
    EXPECT_FALSE(r.reduceServings(10));   // не можна нижче 1
    EXPECT_EQ(r.getServings(), 3);
    EXPECT_FALSE(r.addServings(0));
}

TEST(RecipeOps, QuickAndServes) {
    Recipe r{"X", "укр", 20, 2};
    EXPECT_TRUE(r.isQuick());
    EXPECT_TRUE(r.servesAtLeast(2));
    EXPECT_FALSE(r.servesAtLeast(3));
    EXPECT_EQ(r.totalMinutesFor(4), 40);
}
