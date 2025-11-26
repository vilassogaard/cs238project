#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <cmath>

struct StateData {

    std::string abbreviation;
    int population;
    int seats;
    float residual;
    
    StateData() : population(0), seats(0), residual(0) {}
    StateData(std::string abbr, int pop) 
        : abbreviation(abbr), population(pop), seats(0), residual(0) {}

};

std::map<std::string, StateData> read_state_data(const std::string& filename) {

    std::map<std::string, StateData> state_map;
    std::ifstream file(filename);
    
    if (!file.is_open()) {

        std::cerr << "error: could not open file " << filename << std::endl;
        return state_map;

    }
    
    std::string line;
    std::getline(file, line); // skip header
    
    while (std::getline(file, line)) { // read data

        std::stringstream ss(line);
        std::string state;
        std::string pop_str;
        
        if (std::getline(ss, state, ',') && std::getline(ss, pop_str, ',')) { // parsing

            int population = std::stoi(pop_str);
            state_map[state] = StateData(state, population);

        }
    }
    
    file.close();
    return state_map;
}

int get_total_population(const std::map<std::string, StateData>& state_map) {

    int total = 0;

    for (const auto& [state, data] : state_map) 
        total += data.population;
    
    return total;
}

// hamilton's method (largest remainder)
void hamiltons_method(std::map<std::string, StateData>& state_map, int total_seats) {
    
    int total_pop = get_total_population(state_map);
    float quota = (float)total_pop / total_seats;
    
    int seats_assigned = 0; // calculate initial seats and residuals

    for (auto& [state, data] : state_map) {

        float exact_seats = data.population / quota;
        data.seats = (int)exact_seats;
        data.residual = exact_seats - data.seats;
        seats_assigned += data.seats;

    }
    
    
    while (seats_assigned < total_seats) { // assign remaining seats to states with largest residuals

        std::string best_state;
        float max_residual = -1;
        
        for (const auto& [state, data] : state_map) {

            if (data.residual > max_residual) {

                max_residual = data.residual;
                best_state = state;

            }
        }
        
        state_map[best_state].seats++;
        state_map[best_state].residual = -1; // mark as used
        seats_assigned++;

    }
}

// jefferson's method (largest divisor, round down)
void jeffersons_method(std::map<std::string, StateData>& state_map, int total_seats) {
    int total_pop = get_total_population(state_map);
    int divisor = total_pop / total_seats;
    int seats_assigned = 0;
    
    // calculate initial assignment
    for (auto& [state, data] : state_map) {
        data.seats = data.population / divisor;
        seats_assigned += data.seats;
    }
    
    int rem = total_seats - seats_assigned;
    
    // adjust divisor until we get exact number of seats
    while (rem != 0) {
        divisor = divisor - 1;
        seats_assigned = 0;
        
        for (auto& [state, data] : state_map) {
            data.seats = data.population / divisor;
            seats_assigned += data.seats;
        }
        
        rem = total_seats - seats_assigned;
    }
}

// webster's method (round to nearest)
void websters_method(std::map<std::string, StateData>& state_map, int total_seats) {
    int total_pop = get_total_population(state_map);
    int divisor = total_pop / total_seats;
    int seats_assigned = 0;
    
    // calculate initial assignment
    for (auto& [state, data] : state_map) {
        data.seats = (int)round((float)data.population / divisor);
        seats_assigned += data.seats;
    }
    
    int rem = total_seats - seats_assigned;
    
    // adjust divisor until we get exact number of seats
    while (rem != 0) {
        int diff = (rem < 0) ? 1 : -1; // if too many seats, increase divisor; if too few, decrease
        divisor = divisor + diff;
        seats_assigned = 0;
        
        for (auto& [state, data] : state_map) {
            data.seats = (int)round((float)data.population / divisor);
            seats_assigned += data.seats;
        }
        
        rem = total_seats - seats_assigned;
    }
}

// adams' method (round up)
void adams_method(std::map<std::string, StateData>& state_map, int total_seats) {
    int total_pop = get_total_population(state_map);
    int divisor = total_pop / total_seats;
    int seats_assigned = 0;
    
    // calculate initial assignment
    for (auto& [state, data] : state_map) {
        data.seats = (int)ceil((float)data.population / divisor);
        seats_assigned += data.seats;
    }
    
    int rem = total_seats - seats_assigned;
    
    // adjust divisor until we get exact number of seats
    while (rem != 0) {
        int diff = (rem < 0) ? 1 : -1; // if too many seats, increase divisor; if too few, decrease
        divisor = divisor + diff;
        seats_assigned = 0;
        
        for (auto& [state, data] : state_map) {
            data.seats = (int)ceil((float)data.population / divisor);
            seats_assigned += data.seats;
        }
        
        rem = total_seats - seats_assigned;
    }
}

// huntington-hill method (current US method)
void huntington_hill_method(std::map<std::string, StateData>& state_map, int total_seats) {
    // give each state 1 seat first
    for (auto& [state, data] : state_map) {
        data.seats = 1;
    }
    
    int seats_assigned = state_map.size();
    int rem = total_seats - seats_assigned;
    
    if (rem < 0) {
        std::cerr << "error: more states than total seats!" << std::endl;
        return;
    }
    
    // assign remaining seats one by one using priority values
    while (rem > 0) {
        std::string best_state;
        float max_priority = -1;
        
        for (const auto& [state, data] : state_map) {
            // priority = population / sqrt(n * (n+1)) where n is current seats
            float priority = data.population / sqrt(data.seats * (data.seats + 1.0));
            
            if (priority > max_priority) {
                max_priority = priority;
                best_state = state;
            }
        }
        
        state_map[best_state].seats++;
        rem--;
    }
}

void print_results(const std::map<std::string, StateData>& state_map) {

    std::cout << "\nstate\tpopulation\tseats\n";
    std::cout << "-----\t----------\t-----\n";
    
    int total_seats = 0;

    for (const auto& [state, data] : state_map) {

        std::cout << data.abbreviation << "\t" 
                  << data.population << "\t\t" 
                  << data.seats << "\n";
        total_seats += data.seats;

    }

    std::cout << "\ntotal seats: " << total_seats << "\n";

}

// calculate alpha using random sampling - FAST approximation
float calculate_alpha_sampling(const std::map<std::string, StateData>& state_map, int total_seats) {
    
    std::vector<std::string> states;
    std::vector<int> state_pops;
    std::vector<int> state_seats;
    
    for (const auto& [state, data] : state_map) { // pre-compute arrays for faster lookup
        
        states.push_back(state);
        state_pops.push_back(data.population);
        state_seats.push_back(data.seats);
    
    }
    
    int n = states.size();
    long long num_samples = 100000000; // number of samples
    
    int total_pop = get_total_population(state_map);
    float min_alpha = 1.0;
    long long worst_mask = 0; // store mask instead of subset vector
    float worst_pop_prop = 0;
    float worst_seat_prop = 0;
    
    std::cout << "sampling " << num_samples << " random subsets out of " 
              << ((1LL << n) - 2) << " total..." << std::endl;
    
    // seed random number generator
    std::srand(std::time(nullptr));
    
    for (long long sample = 0; sample < num_samples; sample++) {
        
        // generate random subset mask
        long long mask = 0;
        for (int i = 0; i < n; i++) {
            if (std::rand() % 2 == 1) { // 50% chance to include each state
                mask |= (1LL << i);
            }
        }
        
        if (mask == 0 || mask == (1LL << n) - 1) // skip empty and full set
            continue;
        
        int subset_pop = 0;
        int subset_seats = 0;
        
        for (int i = 0; i < n; i++) { // fast lookup using arrays instead of map
            
            if (mask & (1LL << i)) {
                
                subset_pop += state_pops[i];
                subset_seats += state_seats[i];
            
            }
        }
        
        float pop_proportion = (float)subset_pop / total_pop;
        float seat_proportion = (float)subset_seats / total_seats;
        
        if (pop_proportion > 0.0001) {
            
            float alpha = seat_proportion / pop_proportion;
            
            if (alpha < min_alpha) {
                
                min_alpha = alpha;
                worst_mask = mask; // just store the mask
                worst_pop_prop = pop_proportion;
                worst_seat_prop = seat_proportion;
            
            }
        }
        
        if (sample % (num_samples / 10) == 0 && sample > 0) {
            std::cout << "checked " << sample << " / " << num_samples << " samples (" 
                      << (100.0 * sample / num_samples) << "%)..." << std::endl;
        }
    }
    
    std::vector<std::string> worst_subset; // reconstruct worst subset at the end
    
    for (int i = 0; i < n; i++) {
        
        if (worst_mask & (1LL << i)) 
            worst_subset.push_back(states[i]);
    
    }
    
    std::cout << "\n[APPROXIMATE] alpha >= " << min_alpha << " (based on " << num_samples << " samples)" << std::endl;
    std::cout << "\nworst subset found (" << worst_subset.size() << " states):" << std::endl;
    
    for (const std::string& state : worst_subset) 
        std::cout << "  " << state;
    
    std::cout << "\npopulation proportion: " << worst_pop_prop * 100 << "%" << std::endl;
    std::cout << "seat proportion: " << worst_seat_prop * 100 << "%" << std::endl;
    std::cout << "ratio (alpha): " << min_alpha << std::endl;

    return min_alpha;

}

// calculate alpha using random sampling - FAST approximation. uses (threshold) amount instead of purely random sampling
float calculate_alpha_sampling(const std::map<std::string, StateData>& state_map, int total_seats, float threshold) {
    
    std::vector<std::string> states;
    std::vector<int> state_pops;
    std::vector<int> state_seats;
    
    for (const auto& [state, data] : state_map) { // pre-compute arrays for faster lookup
        
        states.push_back(state);
        state_pops.push_back(data.population);
        state_seats.push_back(data.seats);
    
    }
    
    int n = states.size();
    long long num_samples = 100000000; // number of samples
    
    int total_pop = get_total_population(state_map);
    float min_alpha = 1.0;
    long long worst_mask = 0; // store mask instead of subset vector
    float worst_pop_prop = 0;
    float worst_seat_prop = 0;
    
    std::cout << "sampling " << num_samples << " random subsets with threshold " << threshold << " out of " 
              << ((1LL << n) - 2) << " total samples..." << std::endl;
    
    // seed random number generator
    std::srand(std::time(nullptr));
    
    for (long long sample = 0; sample < num_samples; sample++) {
        
        // generate random subset mask
        long long mask = 0;
        for (int i = 0; i < n; i++) {
            if (std::rand() > (1 - threshold) * RAND_MAX) { // we want (threshold) amount to be included
                mask |= (1LL << i);
            }
        }
        
        if (mask == 0 || mask == (1LL << n) - 1) // skip empty and full set
            continue;
        
        int subset_pop = 0;
        int subset_seats = 0;
        
        for (int i = 0; i < n; i++) { // fast lookup using arrays instead of map
            
            if (mask & (1LL << i)) {
                
                subset_pop += state_pops[i];
                subset_seats += state_seats[i];
            
            }
        }
        
        float pop_proportion = (float)subset_pop / total_pop;
        float seat_proportion = (float)subset_seats / total_seats;
        
        if (pop_proportion > 0.0001) {
            
            float alpha = seat_proportion / pop_proportion;
            
            if (alpha < min_alpha) {
                
                min_alpha = alpha;
                worst_mask = mask; // just store the mask
                worst_pop_prop = pop_proportion;
                worst_seat_prop = seat_proportion;
            
            }
        }
        
        if (sample % (num_samples / 10) == 0 && sample > 0) {
            std::cout << "checked " << sample << " / " << num_samples << " samples (" 
                      << (100.0 * sample / num_samples) << "%)..." << std::endl;
        }
    }
    
    std::vector<std::string> worst_subset; // reconstruct worst subset at the end
    
    for (int i = 0; i < n; i++) {
        
        if (worst_mask & (1LL << i)) 
            worst_subset.push_back(states[i]);
    
    }
    
    std::cout << "\n[APPROXIMATE] alpha >= " << min_alpha << " (based on " << num_samples << " samples)" << std::endl;
    std::cout << "\nworst subset found (" << worst_subset.size() << " states):" << std::endl;
    
    for (const std::string& state : worst_subset) 
        std::cout << "  " << state;
    
    std::cout << "\npopulation proportion: " << worst_pop_prop * 100 << "%" << std::endl;
    std::cout << "seat proportion: " << worst_seat_prop * 100 << "%" << std::endl;
    std::cout << "ratio (alpha): " << min_alpha << std::endl;

    return min_alpha;

}

int main() {
    
    std::map<std::string, StateData> state_map = read_state_data("state_populations.csv");
    int total_seats = 435;

    float min_hamilton_alpha = 1;
    float min_jefferson_alpha = 1;
    float min_webster_alpha = 1;
    float min_adams_alpha = 1;
    float min_hh_alpha = 1;

    float curr;

    /*

    for (int i = 0; i < 10; i++) {

        std::cout << "=== RUN " << i << " ===" << std::endl;

        std::cout << std::endl;

        std::cout << "=== hamilton's method ===" << std::endl;
        hamiltons_method(state_map, total_seats);
        curr = calculate_alpha_sampling(state_map, total_seats);

        min_hamilton_alpha = std::min(curr, min_hamilton_alpha);
        
        std::cout << "\n\n=== jefferson's method ===" << std::endl;
        jeffersons_method(state_map, total_seats);
        curr = calculate_alpha_sampling(state_map, total_seats);

        min_jefferson_alpha = std::min(curr, min_jefferson_alpha);
        
        std::cout << "\n\n=== webster's method ===" << std::endl;
        websters_method(state_map, total_seats);
        curr = calculate_alpha_sampling(state_map, total_seats);

        min_webster_alpha = std::min(curr, min_webster_alpha);
        
        std::cout << "\n\n=== adams' method ===" << std::endl;
        adams_method(state_map, total_seats);
        curr = calculate_alpha_sampling(state_map, total_seats);

        min_adams_alpha = std::min(curr, min_adams_alpha);
        
        std::cout << "\n\n=== huntington-hill method ===" << std::endl;
        huntington_hill_method(state_map, total_seats);
        curr = calculate_alpha_sampling(state_map, total_seats);

        min_hh_alpha = std::min(curr, min_hh_alpha);

        std::cout << std::endl;
        std::cout << std::endl;

    }

    std::cout << "MINIMUM hamilton alpha: " << min_hamilton_alpha << std::endl;
    std::cout << "MINIMUM jefferson alpha: " << min_jefferson_alpha << std::endl;
    std::cout << "MINIMUM webster alpha: " << min_webster_alpha << std::endl;
    std::cout << "MINIMUM adams alpha: " << min_adams_alpha << std::endl;
    std::cout << "MINIMUM huntington hill alpha: " << min_hh_alpha << std::endl;

    */

    /*

    float threshold = 0.7; // amount you want in the coalition

    for (int i = 0; i < 10; i++) {

        std::cout << "=== RUN " << i << " ===" << std::endl;

        std::cout << std::endl;

        std::cout << "=== hamilton's method ===" << std::endl;
        hamiltons_method(state_map, total_seats);
        curr = calculate_alpha_sampling(state_map, total_seats, threshold);

        min_hamilton_alpha = std::min(curr, min_hamilton_alpha);
        
        std::cout << "\n\n=== jefferson's method ===" << std::endl;
        jeffersons_method(state_map, total_seats);
        curr = calculate_alpha_sampling(state_map, total_seats, threshold);

        min_jefferson_alpha = std::min(curr, min_jefferson_alpha);
        
        std::cout << "\n\n=== webster's method ===" << std::endl;
        websters_method(state_map, total_seats);
        curr = calculate_alpha_sampling(state_map, total_seats, threshold);

        min_webster_alpha = std::min(curr, min_webster_alpha);
        
        std::cout << "\n\n=== adams' method ===" << std::endl;
        adams_method(state_map, total_seats);
        curr = calculate_alpha_sampling(state_map, total_seats, threshold);

        min_adams_alpha = std::min(curr, min_adams_alpha);
        
        std::cout << "\n\n=== huntington-hill method ===" << std::endl;
        huntington_hill_method(state_map, total_seats);
        curr = calculate_alpha_sampling(state_map, total_seats, threshold);

        min_hh_alpha = std::min(curr, min_hh_alpha);

        std::cout << std::endl;
        std::cout << std::endl;

    }

    std::cout << "MINIMUM hamilton alpha with threshold " << threshold << ": " << min_hamilton_alpha << std::endl;
    std::cout << "MINIMUM jefferson alpha with threshold " << threshold << ": " << min_jefferson_alpha << std::endl;
    std::cout << "MINIMUM webster alpha with threshold " << threshold << ": " << min_webster_alpha << std::endl;
    std::cout << "MINIMUM adams alpha with threshold " << threshold << ": " << min_adams_alpha << std::endl;
    std::cout << "MINIMUM huntington hill alpha with threshold " << threshold << ": " << min_hh_alpha << std::endl;

    */

    float threshold = 0; // amount you want in the coalition
    std::vector<float> alphas_hamilton;
    std::vector<float> alphas_jefferson;
    std::vector<float> alphas_webster;
    std::vector<float> alphas_adams;
    std::vector<float> alphas_hh;

    for (int k = 0; k < 10; k++) {

        threshold += 0.1;

        for (int i = 0; i < 5; i++) {

            // resetting minimums

            min_hamilton_alpha = 1;
            min_jefferson_alpha = 1;
            min_webster_alpha = 1;
            min_adams_alpha = 1;
            min_hh_alpha = 1;

            std::cout << "=== RUN " << i << " ===" << std::endl;

            std::cout << std::endl;

            std::cout << "=== hamilton's method ===" << std::endl;
            hamiltons_method(state_map, total_seats);
            curr = calculate_alpha_sampling(state_map, total_seats, threshold);

            min_hamilton_alpha = std::min(curr, min_hamilton_alpha);
            alphas_hamilton.push_back(min_hamilton_alpha);
            
            std::cout << "\n\n=== jefferson's method ===" << std::endl;
            jeffersons_method(state_map, total_seats);
            curr = calculate_alpha_sampling(state_map, total_seats, threshold);

            min_jefferson_alpha = std::min(curr, min_jefferson_alpha);
            alphas_jefferson.push_back(min_jefferson_alpha);
            
            std::cout << "\n\n=== webster's method ===" << std::endl;
            websters_method(state_map, total_seats);
            curr = calculate_alpha_sampling(state_map, total_seats, threshold);

            min_webster_alpha = std::min(curr, min_webster_alpha);
            alphas_webster.push_back(min_webster_alpha);
            
            std::cout << "\n\n=== adams' method ===" << std::endl;
            adams_method(state_map, total_seats);
            curr = calculate_alpha_sampling(state_map, total_seats, threshold);

            min_adams_alpha = std::min(curr, min_adams_alpha);
            alphas_adams.push_back(min_adams_alpha);
            
            std::cout << "\n\n=== huntington-hill method ===" << std::endl;
            huntington_hill_method(state_map, total_seats);
            curr = calculate_alpha_sampling(state_map, total_seats, threshold);

            min_hh_alpha = std::min(curr, min_hh_alpha);
            alphas_hh.push_back(min_hh_alpha);

            std::cout << std::endl;
            std::cout << std::endl;

        }

    }

    std::cout << "=== hamilton alphas ===" << std::endl;

    for (int i = 0; i < 50; i++) {

        std::cout << alphas_hamilton[i] << ", ";

    }

    std::cout << std::endl;
    std::cout << std::endl;

    std::cout << "=== jefferson alphas ===" << std::endl;

    for (int i = 0; i < 50; i++) {

        std::cout << alphas_jefferson[i] << ", ";

    }

    std::cout << std::endl;
    std::cout << std::endl;

    std::cout << "=== webster alphas ===" << std::endl;

    for (int i = 0; i < 50; i++) {

        std::cout << alphas_webster[i] << ", ";

    }

    std::cout << std::endl;
    std::cout << std::endl;

    std::cout << "=== adams alphas ===" << std::endl;

    for (int i = 0; i < 50; i++) {

        std::cout << alphas_adams[i] << ", ";

    }

    std::cout << std::endl;
    std::cout << std::endl;

    std::cout << "=== huntington-hill alphas ===" << std::endl;

    for (int i = 0; i < 50; i++) {

        std::cout << alphas_hh[i] << ", ";

    }

    std::cout << std::endl;
    std::cout << std::endl;
    
    return 0;
    
}