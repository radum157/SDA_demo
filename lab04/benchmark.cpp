#include "hashmap.h"
#include <ctime>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <time.h>

using KeyVal = std::pair<std::string, int>;


std::vector<KeyVal> get_input() {
	std::vector<KeyVal> input;
	std::ifstream fin("input.txt");

	KeyVal buf;
	while (fin >> buf.first >> buf.second) {
		input.emplace_back(buf);
	}
	fin.close();

	std::cout << "Loaded " << input.size() << " entries\n";
	return input;
}

void benchmark_hashmap(std::vector<KeyVal> &input) {
	hashmap_t map{};
	clock_t start, end;

	std::cout << "Benchmarking my hash_map:\n";
	start = clock();
	for (const auto &[k, v] : input) {
		insert(&map, k.data(), v);
	}
	end = clock();
	std::cout << "Inserted " << input.size() << " entries in " <<
			(float)(end - start) / CLOCKS_PER_SEC << " ms\n";

	start = clock();
	for (const auto &[k, _] : input) {
		search(&map, k.data());
	}
	end = clock();
	std::cout << "Queried " << input.size() << " entries in " <<
			(float)(end - start) / CLOCKS_PER_SEC << " ms\n";
}

void benchmark_unordered_map(std::vector<KeyVal> &input) {
	std::unordered_map<std::string, int> map;
	clock_t start, end;

	std::cout << "Benchmarking std::unordered_map\n";
	start = clock();
	for (const auto &[k, v] : input) {
		map.insert({k, v});
	}
	end = clock();
	std::cout << "Inserted " << input.size() << " entries in " <<
			(float)(end - start) / CLOCKS_PER_SEC << " ms\n";

	start = clock();
	for (const auto &[k, _] : input) {
		map[k];
	}
	end = clock();
	std::cout << "Queried " << input.size() << " entries in " <<
			(float)(end - start) / CLOCKS_PER_SEC << " ms\n";
}

int main() {
	std::vector<KeyVal> input{get_input()};
	benchmark_unordered_map(input);
	benchmark_hashmap(input);
}
