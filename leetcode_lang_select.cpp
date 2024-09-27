// Copyright 2024 Rishabh Dwivedi <rishabhdwivedi17@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the “Software”), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <functional>
#include <iostream>
#include <sstream>
#include <sys/ioctl.h>
#include <thread>
#include <unistd.h>
#include <vector>

// Sample input file format:
// ---------------------------
// The first line should contain an integer indicating the number
// of names to process. Each subsequent line should contain one name.
//
// Example input:
// 3
// Person 1
// Person 2
// Person 3
//
// The tool utilizes the `echo` and `figlet` command, for stylized text
// representation. Ensure that both `figlet` and `echo` are available in your
// environment for the tool to function correctly.

int get_terminal_width() {
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  return w.ws_col;
}

std::string get_color() {
  std::vector<std::string> colors{
      "\033[31m", // Red
      "\033[32m", // Green
      "\033[33m", // Yellow
      "\033[34m", // Blue
      "\033[35m", // Magenta
      "\033[36m", // Cyan
  };
  return colors[std::rand() % colors.size()];
}

std::string const reset = "\033[0m";
std::string const clear{"\033[2J\033[H"};
std::string const bold{"\033[1m"};

std::string assign_rand_lang() {
  std::vector<std::string> allowed_langs{
      "C++",        "Java",  "C",     "Python", "C#",     "Javascript",
      "Typescript", "PHP",   "Swift", "Kotlin", "Dart",   "Go",
      "Ruby",       "Scala", "Rust",  "Racket", "Erlang", "Elixir",
  };

  return allowed_langs[std::rand() % allowed_langs.size()];
}

auto is_space = [](char c) { return c == ' '; };

std::string make_term_person_str(std::string const &person, int term_width) {
  std::string res(term_width / 2 - 2, ' ');
  std::copy(std::begin(person), std::end(person), std::begin(res));
  std::rotate(std::begin(res), std::begin(res) + person.size(), std::end(res));
  return res;
}

void shift_to(int start, std::string &p) {
  auto begin = std::begin(p) + start;
  auto end = std::end(p);
  auto mid = std::find_if(std::begin(p), std::end(p), std::not_fn(is_space));
  std::rotate(begin, mid, end);
}

std::vector<std::string> format(std::vector<std::string> people) {
  auto term_width = get_terminal_width();
  std::transform(std::begin(people), std::end(people), std::begin(people),
                 [term_width](auto const &p) {
                   return make_term_person_str(p, term_width);
                 });
  auto fst_non_space_idx = [](auto const &s) {
    return std::find_if(std::begin(s), std::end(s), std::not_fn(is_space)) -
           std::begin(s);
  };
  auto non_space_start_idx = fst_non_space_idx(*std::min_element(
      std::begin(people), std::end(people), [&](auto const &x, auto const &y) {
        return fst_non_space_idx(x) < fst_non_space_idx(y);
      }));
  for (auto &p : people)
    shift_to(non_space_start_idx, p);
  return people;
}

void render(std::string prefix, std::vector<std::string> people,
            std::vector<std::string> const &chosen_langs) {
  std::cout << prefix;
  people = format(std::move(people));
  for (int i = 0; i < people.size(); ++i) {
    std::cout << bold << get_color() << people[i] << " -> " << chosen_langs[i]
              << std::endl;
  }
}

void print_leetcode(int i) {
  std::vector<std::string> fonts{
      "standard", "big",      "block",   "bubble", "digital", "ivrit",
      "lean",     "mini",     "script",  "shadow", "slant",   "small",
      "smscript", "smshadow", "smslant", "banner", "block",
  };
  auto idx = i < 0 ? (std::rand() % fonts.size()) : i;
  auto font = fonts[idx];
  std::stringstream out;
  auto color = get_color();
  std::cout << clear << std::endl;
  out << "echo " << '"' << color << "$(figlet -k -w $(tput cols) -f " << font
      << " -c Leetcode Competition)" << reset << '"';
  std::string str = out.str();
  std::system(str.data());
}

int main() {
  std::srand(std::time(nullptr));
  int num_people;
  std::cin >> num_people;
  std::vector<std::string> people;
  std::cin.ignore(1, '\n');
  while (num_people--) {
    std::string cur;
    std::getline(std::cin, cur);
    people.push_back(std::move(cur));
  }
  int num_times = 100;
  std::vector<std::string> final_langs;
  while (num_times--) {
    std::vector<std::string> chosen_langs;
    for (auto s : people)
      chosen_langs.push_back(assign_rand_lang());
    print_leetcode(4);
    render("", people, chosen_langs);
    final_langs = std::move(chosen_langs);
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
  }
  print_leetcode(-1);
  render("", people, final_langs);
}
