#include <ctre-unicode.hpp>
#include <flux.hpp>
#include <print>
#include <string_view>

constexpr auto run_program_p1(const std::string_view sv) -> int;
constexpr auto run_program_p2(const std::string_view sv) -> int;
constexpr inline const std::string_view input = R"###(
]select(23,564)/$!where()>%mul(747,16)*don't()why()mul(354,748)do()how
-- <Input> ---
4)}~mul(668,671)mul(787,282)</{[@+mul(669,479)&+who(324,639)when()mul(
)###";

auto main() -> int {
  // full compile-time solution
  // TODO: Add full CMake build instructions
  std::println("P1 - sum of `mult` instructions: {}", run_program_p1(input));
  std::println("P2 - w/o disabled sections: {}", run_program_p2(input));
}

constexpr auto run_program_p1(const std::string_view sv) -> int {
  static constexpr auto mult_pattern =
      ctll::fixed_string{R"((?<=mul\()(\d+),(\d+)(?=\)))"};

  auto mult_match_handler = [](auto match) -> int {
    const auto &[_, a, b] = match;
    return a.to_number() * b.to_number();
  };

  return flux::from_range(sv | ctre::search_all<mult_pattern>)
      .map(mult_match_handler)
      .sum();
}

constexpr auto run_program_p2(const std::string_view sv) -> int {
  static constexpr auto disabled_section_pattern =
      ctll::fixed_string{R"(don't\(\).*?(?:do\(\)|$))"};

  return flux::from_range(sv | ctre::split<disabled_section_pattern>)
      .map(run_program_p1)
      .sum();
}
