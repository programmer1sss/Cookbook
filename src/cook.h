#pragma once

#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#include "recipe.h"

class Cook {
 public:
  Cook() = default;

  Cook(std::string name, std::string specialty)
      : name_(std::move(name)), specialty_(std::move(specialty)) {
      if (name_.empty()) throw std::invalid_argument("Cook: empty name");
  }

  explicit Cook(std::string name)
      : Cook(std::move(name), "(універсал)") {}

  const std::string& getName() const { return name_; }
  const std::string& getSpecialty() const { return specialty_; }

  void addRecipe(Recipe r) { recipes_.push_back(std::move(r)); }
  std::size_t recipeCount() const { return recipes_.size(); }

 private:
  std::string name_ = "(без імені)";
  std::string specialty_ = "(універсал)";
  std::vector<Recipe> recipes_;
};