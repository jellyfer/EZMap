/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   m4.cpp
 * Author: caiwei7
 *
 * Created on April 1, 2017, 11:08 PM
 */

#include <cstdlib>
#include <m4.h>
#include <m3.h>
#include <m1.h>
#include <m3_additional.h>
#include <m4_additional.h>
#include <unordered_map>
#include <chrono>
#define OPT_TIME_LIMIT 1
#define GREEDY_TIME_LIMIT 27

using namespace std;
//vector of street segment ids which form a legal path
//returns vector size 0 if no legal path
vector<unsigned> generate_street_segments(vector<unsigned> path, unsigned turn_penalty);
bool check_valid(vector <unsigned> sub_vector, unsigned start, unsigned last, unordered_map < unsigned, vector<unsigned> > pickup_dropoff_ids);
std::vector<unsigned> generate_best_path_greedy(vector <unsigned> depots, unordered_map < unsigned, vector<unsigned> > pickup_dropoff_ids, unordered_set <unsigned> nodes, unsigned num_diff_starts, unsigned turn_penalty);
vector<unsigned> generate_2_opt(vector<unsigned> best, unordered_map < unsigned, vector<unsigned> > pickup_dropoff_ids);
unordered_map < unsigned, unordered_map<unsigned, path_and_cost> > path_cost;
unordered_map <unsigned, unsigned> last_occurence_of_dropoff;
unordered_set <unsigned> dependency;

std::vector<unsigned> traveling_courier(const std::vector<DeliveryInfo>& deliveries,
        const std::vector<unsigned>& depots,
        const float turn_penalty) {

    vector <unsigned> complete_best_path;
    unordered_map < unsigned, vector<unsigned> > pickup_dropoff_ids;
    
    //look thru legal pickup & dropoff (O(n) max), if already picked up, only look at dropoff
    //if not picked up, do not look at dropoff
    unordered_set <unsigned> new_nodes_to_look_at;
    for (unsigned int x = 0; x < deliveries.size(); x++) {
        new_nodes_to_look_at.insert(deliveries[x].pickUp);

        if (pickup_dropoff_ids.find(deliveries[x].pickUp) == pickup_dropoff_ids.end()) {
            vector<unsigned> first_dropoff;
            first_dropoff.push_back(deliveries[x].dropOff);
            pickup_dropoff_ids.insert(make_pair(deliveries[x].pickUp, first_dropoff));
        } else {
            pickup_dropoff_ids[deliveries[x].pickUp].push_back(deliveries[x].dropOff);
        }

        dependency.insert(deliveries[x].dropOff);
    }


    complete_best_path = generate_best_path_greedy(depots, pickup_dropoff_ids, new_nodes_to_look_at, new_nodes_to_look_at.size(), turn_penalty);
    vector <unsigned> complete_best_path_2 = generate_street_segments(complete_best_path, turn_penalty);
    complete_best_path = generate_2_opt(complete_best_path, pickup_dropoff_ids);
    complete_best_path = generate_street_segments(complete_best_path, turn_penalty);
    return compute_path_travel_time(complete_best_path,turn_penalty) < compute_path_travel_time(complete_best_path_2,turn_penalty) ? complete_best_path : complete_best_path_2;
}

bool check_valid(vector <unsigned> sub_vector, unsigned start, unsigned last, unordered_map < unsigned, vector<unsigned> > pickup_dropoff_ids) {
    for (unsigned x = 0; x < sub_vector.size(); x++) {
        //if element is a pickup
        if (pickup_dropoff_ids.find(sub_vector[x]) != pickup_dropoff_ids.end()) {
            for (unsigned i = 0; i < pickup_dropoff_ids[sub_vector[x]].size(); i++) {
                //check last occurance of the dropoff id associated with our pickup
                //also must be within the range of the subpermute we are checking
                if (last_occurence_of_dropoff[pickup_dropoff_ids[sub_vector[x]][i]] <= last &&
                    last_occurence_of_dropoff[pickup_dropoff_ids[sub_vector[x]][i]] >= start) {
                    return false;
                }
            }
        }
    }
    return true;
}

vector<unsigned> generate_street_segments(vector<unsigned> path, unsigned turn_penalty) {
    vector<unsigned> segments;
    for (unsigned i = 0; i < path.size() - 1; i++) {

        if (path[i] == path[i + 1]) {
            continue;
        }

        if (path_cost.find(path[i]) != path_cost.end() &&
                path_cost[path[i]].find(path[i + 1]) != path_cost[path[i]].end()) {

            segments.insert(segments.end(), path_cost[path[i]][path[i + 1]].path.begin(), path_cost[path[i]][path[i + 1]].path.end());

        } else {
            unordered_set <unsigned> next_intersection;
            next_intersection.insert(path[i + 1]);
            vector_and_intersection v = modified_Dijkstra(path[i], next_intersection, turn_penalty);
            segments.insert(segments.end(), v.path.begin(), v.path.end());
        }
    }
    return segments;
}

vector<unsigned> generate_2_opt(vector<unsigned> best, unordered_map < unsigned, vector<unsigned> > pickup_dropoff_ids) {
    auto startTime = chrono::high_resolution_clock::now();
    for (unsigned x = 1; x < best.size() - 2; x++) {
        for (unsigned y = x + 1; y < best.size()-1; y++) {
            vector<unsigned> current_best = best;
            reverse(current_best.begin() + x, current_best.begin() + y + 1);
            vector <unsigned> permutation_region{current_best.begin() + x, current_best.begin() + y + 1};
            if (!check_valid(permutation_region, x, y, pickup_dropoff_ids)) {
                continue;
            }
            double original_permutation_cost = 0;
            double new_permutation_cost = 0;

            if (x != 0) {
                original_permutation_cost += find_distance_between_two_points(getIntersectionPosition(best[x - 1]), getIntersectionPosition(best[x]));
                new_permutation_cost += find_distance_between_two_points(getIntersectionPosition(current_best[x - 1]), getIntersectionPosition(current_best[x]));
            }

            if (y != best.size() - 1) {
                original_permutation_cost += find_distance_between_two_points(getIntersectionPosition(best[y]), getIntersectionPosition(best[y + 1]));
                new_permutation_cost += find_distance_between_two_points(getIntersectionPosition(current_best[y]), getIntersectionPosition(current_best[y + 1]));
            }

            if (new_permutation_cost < original_permutation_cost) {
                best = current_best;
                
                //update last occurences of dropoffs
                for (unsigned i = x; i <= y; i++) {
                    if (dependency.find(best[i]) != dependency.end()) { //if dropoff
                        //check if last occurence of dropoff id was inside our permutation, update last occurence if so
                        if (last_occurence_of_dropoff[best[i]] <= y &&
                            last_occurence_of_dropoff[best[i]] >= x) {
                            last_occurence_of_dropoff[best[i]] = i;
                            
                        }
                    }
                }
            }
        }

        auto currentTime3 = chrono::high_resolution_clock::now();
        auto wallClock3 = chrono::duration_cast <chrono::duration<double>> (currentTime3 - startTime);
        if (wallClock3.count() > OPT_TIME_LIMIT) {
            return best;
        }
    }
    return best;
}

vector<unsigned> generate_best_path_greedy(vector <unsigned> depots,
        unordered_map < unsigned, vector<unsigned> > pickup_dropoff_ids,
        unordered_set <unsigned> nodes,
        unsigned num_diff_starts,
        unsigned turn_penalty) {
    auto startTime = chrono::high_resolution_clock::now();

    unsigned depot_i;
    unsigned start = 0;
    unordered_set <unsigned> depot_set(depots.begin(), depots.end());
    vector <unsigned> best_path;
    vector <unsigned> best_node_path;
    
    unordered_set <unsigned> previous_starts;
    unordered_map <unsigned, unsigned> best_map;
    
    while (start < num_diff_starts && start < nodes.size()) {
        depot_i = 0;
        while (depot_i < depots.size()) {
            
            auto currentTime3 = chrono::high_resolution_clock::now();
            auto wallClock3 = chrono::duration_cast <chrono::duration<double>> (currentTime3 - startTime);
            if (wallClock3.count() > GREEDY_TIME_LIMIT) {
              
                last_occurence_of_dropoff = best_map;
               
                return best_node_path;
            }

            unsigned node = depots[depot_i];
            
            //ignore previous starts in order to find kth start node
            unordered_set <unsigned> nodes_to_look_at(nodes);
            for (auto it = previous_starts.begin(); it != previous_starts.end(); it++) {
                nodes_to_look_at.erase(*it);
            }

            //make sure starting depot is connected!
            vector_and_intersection closest_node = modified_Dijkstra(node,
                    nodes_to_look_at,
                    turn_penalty);

            if (closest_node.path.size() == 0) {
                depot_i++;
                continue;
            }

            //add back previous starts
            nodes_to_look_at.insert(previous_starts.begin(), previous_starts.end());

            //initialize path with path from depot to kth start node
            vector <unsigned> path(closest_node.path);
            //path of nodes instead of edges
            vector <unsigned> nodes_path;
            nodes_path.push_back(node); //add start depot

            unordered_map <unsigned, unsigned> map;
            
            //keep track of picked up nodes as to not enqueue its dropoffs if encountered again
            unordered_set <unsigned> was_picked_up;

            //need to update the set of nodes to look at, if the closest_next is a
            //pickup, we need to remove current & add the corresponding dropoff.
            //if closest_next is a dropoff, we simply remove current
            node = closest_node.intersection;
            previous_starts.insert(node);
            nodes_path.push_back(node);
            nodes_to_look_at.erase(node);


            // insert dropoffs if node is pickup
            if (pickup_dropoff_ids.find(node) != pickup_dropoff_ids.end() && was_picked_up.find(node) == was_picked_up.end()) {
                was_picked_up.insert(node);
                for (unsigned int x = 0; x < pickup_dropoff_ids[node].size(); x++) {
                    nodes_to_look_at.insert(pickup_dropoff_ids[node][x]);
                }
            }
            if (dependency.find(node)!= dependency.end()) {
                map[node] = nodes_path.size() - 1;
            }

            while (nodes_to_look_at.size() > 0) {
                auto currentTime2 = chrono::high_resolution_clock::now();
                auto wallClock2 = chrono::duration_cast <chrono::duration<double>> (currentTime2 - startTime);
                if (wallClock2.count() > GREEDY_TIME_LIMIT) {
                  
                    last_occurence_of_dropoff = best_map;
                    return best_node_path;
                }

                //finds the next legal dropoff/delivery intersection & path to get there
                closest_node = modified_Dijkstra(node,
                        nodes_to_look_at,
                        turn_penalty);

                //absolutely no path found to any of the remaining nodes, return vector sized o
                if (closest_node.path.size() == 0) {
                    vector<unsigned> empty;
                    return empty;
                }

                //insert the path to get there into best_path
                path.insert(path.end(), closest_node.path.begin(), closest_node.path.end());
                path_and_cost pc;
                pc.path = closest_node.path;
                path_cost[node][closest_node.intersection] = pc;

                node = closest_node.intersection;
                nodes_path.push_back(node);

                //need to update the set of nodes to look at, if the closest_next is a
                //pickup, we need to remove current & add the corresponding dropoff.
                //if closest_next is a dropoff, we simply remove current
                nodes_to_look_at.erase(node);
                if (pickup_dropoff_ids.find(node) != pickup_dropoff_ids.end() && was_picked_up.find(node) == was_picked_up.end()) {
                    was_picked_up.insert(node);
                    
                    //insert the corresponding dropoff
                    for (unsigned int x = 0; x < pickup_dropoff_ids[node].size(); x++) {
                        nodes_to_look_at.insert(pickup_dropoff_ids[node][x]);
                    }
                }
                if (dependency.find(node)!= dependency.end()) {
                    map[node] = nodes_path.size() - 1;
                }
            }

            //node is last dropoff
            //find closest depot to last dropoff
            closest_node = modified_Dijkstra(node,
                    depot_set,
                    turn_penalty);

            //insert path from last dropoff to depot
            path.insert(path.end(), closest_node.path.begin(), closest_node.path.end());
            nodes_path.push_back(closest_node.intersection);

            if (best_path.size() == 0 || compute_path_travel_time(best_path, turn_penalty) > compute_path_travel_time(path, turn_penalty)) {
                best_path = path;
                best_node_path = nodes_path;
                best_map = map;
            }

            depot_i++;
        }
        start++;
    }
    last_occurence_of_dropoff = best_map;
    
    //return best_path;
    return best_node_path;
}