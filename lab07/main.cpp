#include "priority_queue.h"
#include <cstring>
#include <string>
#include <unordered_map>
#include <unordered_set>

typedef struct distance {
    std::string cities[2];
    int distance;
} distance_t;

typedef struct city_cost {
    char name[32];
    int cost;
} city_cost_t;

bool greater(const void *a, const void *b) {
    int *int_a = (int *)a;
    int *int_b = (int *)b;
    return *int_a > *int_b;
}

bool less(const void *a, const void *b) {
    city_cost_t *city_a = (city_cost_t *)a;
    city_cost_t *city_b = (city_cost_t *)b;
    return city_a->cost < city_b->cost;
}

int main() {
    priority_queue_t *pq = pq_create(less, sizeof(city_cost_t));

    if (!pq) {
        fprintf(stderr, "Failed to create priority queue\n");
        return 1;
    }

    std::unordered_set<std::string> visited_cities;
    std::unordered_map<std::string, std::unordered_map<std::string, int>> distances;

    distances["Bucharest"]["Arad"] = 366;
    distances["Arad"]["Bucharest"] = 366;

    distances["Bucharest"]["Craiova"] = 160;
    distances["Craiova"]["Bucharest"] = 160;

    distances["Bucharest"]["Drobeta"] = 242;
    distances["Drobeta"]["Bucharest"] = 242;

    distances["Bucharest"]["Eforie"] = 1610;
    distances["Eforie"]["Bucharest"] = 1610;

    distances["Bucharest"]["Constanta"] = 178;
    distances["Constanta"]["Bucharest"] = 178;

    distances["Constanta"]["Eforie"] = 14;
    distances["Eforie"]["Constanta"] = 14;

    city_cost_t start_city = {.name = "Bucharest", .cost = 0};
    std::string destination = "Eforie";
    pq_push(pq, &start_city);

    while (!is_empty(pq)) {
        city_cost_t *curr = (city_cost_t*)pq_peek(pq);
        city_cost_t current_city = *curr;

        if (visited_cities.find(current_city.name) != visited_cities.end()) {
            pq_pop(pq);
            continue;
        }

        pq_pop(pq);

        printf("Visiting city %s with cost %d\n", current_city.name, current_city.cost);
        if (current_city.name == destination) {
            printf("Found path to %s with cost %d\n", destination.c_str(), current_city.cost);
            break;
        }

        for (const auto &neighbor : distances[current_city.name]) {
            city_cost_t neighbor_city;
            neighbor_city.cost = current_city.cost + neighbor.second;
            strncpy(neighbor_city.name, neighbor.first.c_str(), sizeof(neighbor_city.name) - 1);
            pq_push(pq, &neighbor_city);
        }

        visited_cities.insert(current_city.name);
    }

    pq_destroy(pq);
    return 0;
}
