///*
// * To change this license header, choose License Headers in Project Properties.
// * To change this template file, choose Tools | Templates
// * and open the template in the editor.
// */
//
///* 
// * File:   m4.cpp
// * Author: caiwei7
// *
// * Created on April 1, 2017, 11:08 PM
// */
//
//#include <cstdlib>
//#include <m4_additional.h>
//#include <m4.h>
//#include <m3.h>
//#include <m3_additional.h>
//#include <unordered_map>
//#include <chrono>
//#include <thread>
//#include <algorithm>
//#define TIME_LIMIT 30
//using namespace std;
////a 2d array to avoid calculating dikstras & path again
//unordered_map <unsigned, unordered_map<unsigned, path_and_cost>> path_cost;
//unordered_map <unsigned, vector<unsigned>> dependency;
//unordered_map <unsigned, vector<unsigned>> pickup_dropoff_ids;
//std::vector<DeliveryInfo> deliveries_global;
//
////stores the last occurance of a dropoff
//unordered_map <unsigned, unsigned> master_hash_table;
//
//
////need dependency tree to see what needs
//
//
////store every path except a depot to its corresponding pickup
////forget cont dj for now
///*
//void continuousDJ();
//{
//    for (int x = 0; x < deliveries_global.size(); x++){
//        for (int y = 0 ; y < deliveries_global.size(); y++){
//            //find path from pickup to every dropoff
//            
//            //find path from dropoff to every pickup not in dependency
//        }
//    }
//
//}*/
//
////vector of street segment ids which form a legal path
////returns vector size 0 if no legal path
//
//std::vector<unsigned> traveling_courier(const std::vector<DeliveryInfo>& deliveries,
//        const std::vector<unsigned>& depots,
//        const float turn_penalty) {
//
//    //SET UP
//
//    auto startTime = chrono::high_resolution_clock::now();
//    bool timeOut = false;
//
//    deliveries_global = deliveries;
//
//    //  bool debug = false;
//    vector <unsigned> complete_best_path;
//
//    //start with first depot 
//    unsigned try_depot_number = 0;
//
//    vector <unsigned> best_path_intersections;
//
//    //create a map of pickups to see if they have been visited already
//    unordered_map <unsigned, bool> pickup_has_been_visited;
//
//
//    unordered_set <unsigned> new_nodes_to_look_at;
//    for (unsigned int x = 0; x < deliveries.size(); x++) {
//        new_nodes_to_look_at.insert(deliveries[x].pickUp);
//
//        //populate has been picked up to false
//        pickup_has_been_visited[deliveries[x].pickUp] = false;
//
//
//
//        if (pickup_dropoff_ids.find(deliveries[x].pickUp) == pickup_dropoff_ids.end()) {
//            vector<unsigned> first_dropoff;
//            first_dropoff.push_back(deliveries[x].dropOff);
//            pickup_dropoff_ids.insert(make_pair(deliveries[x].pickUp, first_dropoff));
//        } else {
//            pickup_dropoff_ids[deliveries[x].pickUp].push_back(deliveries[x].dropOff);
//        }
//
//
//
//        if (dependency.find(deliveries[x].dropOff) == dependency.end()) {
//            vector<unsigned> first_pickup;
//            first_pickup.push_back(deliveries[x].pickUp);
//            dependency.insert(make_pair(deliveries[x].dropOff, first_pickup));
//        } else {
//            dependency[deliveries[x].dropOff].push_back(deliveries[x].pickUp);
//        }
//
//
//    }
//
//
//
//
//
//
//
//
//
//
//
//
//    //BEGIN GREEDY SOLN
//    //try first depot & only first depot
//
//    unordered_set <unsigned> nodes_to_look_at = new_nodes_to_look_at;
//    vector <unsigned> best_path;
//    unsigned intersection_next = depots[try_depot_number];
//    //make sure starting depot is connected!
//    vector_and_intersection closest_next = modified_Dijkstra(intersection_next,
//            nodes_to_look_at,
//            turn_penalty);
//    if (closest_next.path.size() == 0) {
//        bool found = false;
//        //while we still have depots to check,
//        while (try_depot_number < depots.size()) {
//            intersection_next = depots[try_depot_number];
//            try_depot_number++;
//            //changed depot, now find again
//            closest_next = modified_Dijkstra(intersection_next,
//                    nodes_to_look_at,
//                    turn_penalty);
//            if (closest_next.path.size() != 0) {
//                found = true;
//                break;
//            }
//        }
//        //there is no path
//        if (!found) {
//            vector<unsigned> empty;
//            return empty;
//        }
//    }//first node found
//    else {
//        best_path.insert(best_path.end(), closest_next.path.begin(), closest_next.path.end());
//
//        best_path_intersections.push_back(closest_next.intersection);
//        pickup_has_been_visited[closest_next.intersection] = true;
//
//        intersection_next = closest_next.intersection;
//        //need to update the set of nodes to look at, if the closest_next is a
//        //pickup, we need to remove current & add the corresponding dropoff.
//        //if closest_next is a dropoff, we simply remove current
//        nodes_to_look_at.erase(intersection_next);
//
//        if (pickup_dropoff_ids.find(intersection_next) != pickup_dropoff_ids.end()) {
//            //insert the corresponding dropoff
//            for (unsigned int x = 0; x < pickup_dropoff_ids[intersection_next].size(); x++)
//                nodes_to_look_at.insert(pickup_dropoff_ids[intersection_next][x]);
//
//        }
//    }
//
//
//
//
//
//
//
//    /*look thru legal pickup & dropoff (O(n) max), if already picked up, only look at dropoff
//if not picked up, do not look at dropoff
//greedy soln*/
//
//
//    while (nodes_to_look_at.size() > 0) {
//        //finds the next legal dropoff/delivery intersection & path to get there
//
//        closest_next = modified_Dijkstra(intersection_next,
//                nodes_to_look_at,
//                turn_penalty);
//
//        //absolutely no path found to any of the remaining nodes, return vector sized o
//        if (closest_next.path.size() == 0) {
//
//            //while looking for a path there is none
//
//            vector<unsigned> empty;
//            return empty;
//
//        }
//
//        //insert the path to get there into best_path
//        best_path.insert(best_path.end(), closest_next.path.begin(), closest_next.path.end());
//
//        best_path_intersections.push_back(closest_next.intersection);
//        path_and_cost curr;
//        curr.path = closest_next.path;
//        curr.cost = compute_path_travel_time(curr.path, turn_penalty);
//        path_cost [intersection_next][closest_next.intersection] = curr;
//        intersection_next = closest_next.intersection;
//        //need to update the set of nodes to look at, if the closest_next is a
//        //pickup, we need to remove current & add the corresponding dropoff.
//        //if closest_next is a dropoff, we simply remove current
//        nodes_to_look_at.erase(intersection_next);
//
//        //if its a pickup & has not been visited before, then collect the dropoffs
//        if (pickup_dropoff_ids.find(intersection_next) != pickup_dropoff_ids.end() && pickup_has_been_visited[intersection_next] == false) {
//            //insert the corresponding dropoff
//            for (unsigned int x = 0; x < pickup_dropoff_ids[intersection_next].size(); x++)
//                nodes_to_look_at.insert(pickup_dropoff_ids[intersection_next][x]);
//            pickup_has_been_visited[intersection_next] = false;
//        }//must be a dropoff, go populate yo masta hash tableru
//            //now contains the last occurance of the dropoff
//        else {
//            master_hash_table[intersection_next] = best_path_intersections.size() - 1;
//        }
//    }
//
//
//
//
//
//
//
//
//
//    // end with depot closest to last dropoff
//    // last visited is currently stored as intersection_next
//    unordered_set <unsigned> depot_set;
//    for (unsigned int x = 0; x < depots.size(); x++) {
//        depot_set.insert(depots[x]);
//    }
//
//    closest_next = modified_Dijkstra(intersection_next,
//            depot_set,
//            turn_penalty);
//
//    //insert path from last dropoff to depot
//    best_path.insert(best_path.end(), closest_next.path.begin(), closest_next.path.end());
//
//    if (complete_best_path.size() == 0 || compute_path_travel_time(complete_best_path, turn_penalty) > compute_path_travel_time(best_path, turn_penalty)) {
//        complete_best_path = best_path;
//    }
//
//
//
//    //BEST PATH IS CURRENTLY STORED IN BEST_PATH
//    //INTERSECTIONS WENT TO IN ORDER IS STORED INSIDE BEST_PATH_INTERSECTIONS
//    //BEGIN TWO OPT
//    // do 2 opt on each group of 5. why 5? probably cus it's close to each other
//
//    for (unsigned x = 0; x < best_path_intersections.size() - 1; x++) {
//        for (unsigned y = x + 1; y < best_path_intersections.size(); y++) {
//            //thing which will be reversed for 2 opt
//            vector <unsigned> new_path_intersections = best_path_intersections;
//            //will go thru all reversals
//            reverse(new_path_intersections.begin() + x, new_path_intersections.begin() + y + 1);
//
//            //GOTTA CHECK IF ITS LEGAL!!!
//            vector <unsigned> permutation_region{new_path_intersections.begin() + x, new_path_intersections.begin() + y + 1};
//            if (!check_valid(permutation_region, x, y)) {
//                continue;
//            }
//
//
//            /*
//             example
//             * where original is 1 2 3 4 5 
//             * 4 3 2 1 5 
//             * x = 0 y = 3    
//             * so permuted area is original[0,3] original [x,y]
//             */
//
//            double original_permutation_cost = 0;
//            double new_permutation_cost = 0;
//            
//            
//            for (unsigned i = x; i < y; i++) {
//                original_permutation_cost += path_cost[best_path_intersections[i]][best_path_intersections[i + 1]].cost;
//                if (path_cost.find(new_path_intersections[i]) != path_cost.end()
//                        && path_cost[new_path_intersections[i]].find(new_path_intersections [i + 1]) != path_cost[new_path_intersections[i]].end()) {
//
//                    new_permutation_cost += path_cost [new_path_intersections[i]][new_path_intersections[i + 1]].cost;
//
//                } else {
//                    unordered_set <unsigned> next_intersection;
//                    next_intersection.insert(new_path_intersections[i + 1]);
//                    vector_and_intersection v = modified_Dijkstra(new_path_intersections[i], next_intersection, turn_penalty);
//                    double travel_time = compute_path_travel_time(v.path, turn_penalty);
//                    path_and_cost pc;
//                    pc.cost = travel_time;
//                    pc.path = v.path;
//                    path_cost[new_path_intersections[i]][new_path_intersections[i + 1]] = pc;
//                    new_permutation_cost += travel_time;
//
//                }
//            }
//     //       cout <<  "the loops difference in cost is " << new_permutation_cost - original_permutation_cost<<endl;
//
//            if (x != 0) {
//                //not the first one, can add cost from first to curr
//                original_permutation_cost += path_cost[best_path_intersections[x - 1]][best_path_intersections [x]].cost;
//                if (path_cost.find(new_path_intersections[x - 1]) != path_cost.end()
//                        && path_cost[new_path_intersections[x - 1]].find(new_path_intersections[x]) != path_cost[new_path_intersections[x - 1]].end()) {
//                    new_permutation_cost += path_cost [new_path_intersections[x - 1]][new_path_intersections[x]].cost;
//                } else {
//                    unordered_set <unsigned> next_intersection;
//                    next_intersection.insert(new_path_intersections[x]);
//                    vector_and_intersection v = modified_Dijkstra(new_path_intersections[x - 1], next_intersection, turn_penalty);
//                    double travel_time = compute_path_travel_time(v.path, turn_penalty);
//                    path_and_cost pc;
//                    pc.cost = travel_time;
//                    pc.path = v.path;
//                    path_cost[new_path_intersections[x - 1]][new_path_intersections[x]] = pc;
//                    new_permutation_cost += travel_time;
//                }
//            }
//
//            if (y != best_path_intersections.size()-1) {
//                //not the last one, can add cost from curr to last
//                original_permutation_cost += path_cost[best_path_intersections[y]][best_path_intersections[y + 1]].cost;
//                if (path_cost.find(new_path_intersections[y]) != path_cost.end()
//                        && path_cost[new_path_intersections[y]].find(new_path_intersections[y + 1]) != path_cost[new_path_intersections[y]].end()) {
//                    new_permutation_cost += path_cost [new_path_intersections[y]][new_path_intersections[y + 1]].cost;
//                } else {
//                    unordered_set <unsigned> next_intersection;
//                    next_intersection.insert(new_path_intersections[y + 1]);
//                    vector_and_intersection v = modified_Dijkstra(new_path_intersections[y], next_intersection, turn_penalty);
//                    double travel_time = compute_path_travel_time(v.path, turn_penalty);
//                    path_and_cost pc;
//                    pc.cost = travel_time;
//                    pc.path = v.path;
//                    path_cost[new_path_intersections[y]][new_path_intersections[y + 1]] = pc;
//                    new_permutation_cost += travel_time;
//
//                }
//            }
//
//            //if reversed travel time is less than original DO SHIT
//
//            if (new_permutation_cost < original_permutation_cost) {
//                //  a_better_was_found = true;
//
//                //      cout << "is it reaching here? " << endl << endl << endl;
//                //  cout << "next perm cost" << next_permutation_cost << endl << "curr perm cost" << original_permutation_cost << endl << endl;
//                //pseudocode if (current permutation is better than previous best){
//                //update everything
//                //best_path_intersections stores the intersections it goes thru
//                //for (unsigned z = 0; z < permutations.size(); z++) {
//
//                //}
//                best_path_intersections = new_path_intersections;
//
//                cout << " x = " << x << " y = " << y << " new cost: " << new_permutation_cost << "original cost: " << original_permutation_cost << endl;
//                for (unsigned int itt = 0; itt < best_path_intersections.size(); itt++)
//                    cout << best_path_intersections[itt] << endl;
//
//
//
//
//
//                cout << endl << endl << endl << endl;
//
//
//
//                //update master hash table
//                for (unsigned i = x; i < y; i++) {
//                    if (dependency.find(best_path_intersections[i]) != dependency.end()) { //if dropoff
//                        //check if last occurence of dropoff id was inside our 5-permutation, update last occurence if so
//                        if (master_hash_table[best_path_intersections[i]] < y &&
//                                master_hash_table[best_path_intersections[i]] >= x) {
//
//                            master_hash_table[best_path_intersections[i]] = x + (y - i);
//
//                        }
//                    }
//                }
//
//            }
//
//
//            auto currentTime = chrono::high_resolution_clock::now();
//            auto wallClock = chrono::duration_cast <chrono::duration<double>> (currentTime - startTime);
//            if (wallClock.count() > 0.9 * TIME_LIMIT) {
//                timeOut = true;
//                break;
//            }
//
//
//
//
//        }
//        if (timeOut) {
//            break;
//        }
//
//
//    }
//
//
//
//    //LOL better fix this later
//    vector_and_intersection begin_depot = modified_Dijkstra(best_path_intersections.front(),
//            depot_set,
//            turn_penalty);
//
//    unordered_set <unsigned> first_pickup_intersection;
//    first_pickup_intersection.insert(best_path_intersections.front());
//
//    begin_depot = modified_Dijkstra(begin_depot.intersection,
//            first_pickup_intersection,
//            turn_penalty);
//
//    vector<unsigned>path_segments{begin_depot.path};
//    for (unsigned i = 0; i < best_path_intersections.size() - 1; i++) {
//
//
//        if (path_cost.find(best_path_intersections[i]) != path_cost.end()
//                && path_cost[best_path_intersections[i]].find(best_path_intersections[i + 1]) != path_cost[best_path_intersections[i]].end()) {
//
//            path_segments.insert(path_segments.end(), path_cost[best_path_intersections[i]][best_path_intersections[i + 1]].path.begin(), path_cost[best_path_intersections[i]][best_path_intersections[i + 1]].path.end());
//
//        } else {
//            unordered_set <unsigned> next_intersection;
//            next_intersection.insert(best_path_intersections[i + 1]);
//            vector_and_intersection v = modified_Dijkstra(best_path_intersections[i], next_intersection, turn_penalty);
//            path_segments.insert(path_segments.end(), v.path.begin(), v.path.end());
//
//
//        }
//
//
//
//    }
//
//    // add on start & end depot
//
//    vector_and_intersection end_depot = modified_Dijkstra(best_path_intersections.back(),
//            depot_set,
//            turn_penalty);
//
//
//    path_segments.insert(path_segments.end(), end_depot.path.begin(), end_depot.path.end());
//
//
//    complete_best_path = path_segments;
//
//
//    //  cout << endl << endl << endl << endl;
//
//    // for (unsigned int x = 0; x < best_path_intersections.size(); x++)
//    //      cout << best_path_intersections[x] << endl;
//
//
//
//
//    //  cout << endl << endl << endl << endl;
//    return complete_best_path;
//
//
//}
//
//
//
//
////masterhash table is indexing DROPOFFS to LASTEST DROPOFF INDEX
//
//bool check_valid(vector <unsigned> sub_vector, unsigned start, unsigned last) {
//    for (unsigned x = 0; x < sub_vector.size(); x++) {
//        //if our subvector is a pickup
//        if (pickup_dropoff_ids.find(sub_vector[x]) != pickup_dropoff_ids.end()) {
//            for (unsigned i = 0; i < pickup_dropoff_ids[sub_vector[x]].size(); i++) {
//                //check last occurance of the dropoff id associated with our pickup
//                //also must be within the range of the subpermute we are checking
//                if (master_hash_table[pickup_dropoff_ids[sub_vector[x]][i]] <= last && master_hash_table[pickup_dropoff_ids[sub_vector[x]][i]] >= start) {
//                    return false;
//                    // auto index_of_dropoff = sub_permute[x].find(pickup_dropoff_ids[begin][i]);
//                    /* unsigned index = sub_vector.size();
//                     for (unsigned z = 0; z < sub_vector.size(); z++) {
//                         if (sub_vector[z] == pickup_dropoff_ids[sub_vector[x]][i]) {
//                             index = z;
//                         }
//                     }
//                     if (index < x) {
//                         return false;
//                     }*/
//                }
//            }
//
//        }
//    }
//    return true;
//
//}
//
//vector<vector<unsigned>> permute(vector <unsigned> permute_this, unsigned last) {
//    vector <vector<unsigned> > permutations;
//    if (permute_this.size() == 1) {
//        permutations.push_back(permute_this);
//        return permutations;
//    }
//
//    std::vector<unsigned>::iterator begin_it = permute_this.begin();
//    unsigned begin = *begin_it;
//    permute_this.erase(begin_it);
//    vector <vector<unsigned>> sub_permute = permute(permute_this, last);
//
//    for (unsigned int x = 0; x < sub_permute.size(); x++) {
//        for (unsigned int y = 0; y < sub_permute[x].size() + 1; y++) {
//            bool break_out = false; //needed to break out of 2 for loops
//            //if begin is a pickup
//            if (pickup_dropoff_ids.find(begin) != pickup_dropoff_ids.end()) {
//                for (unsigned i = 0; i < pickup_dropoff_ids[begin].size(); i++) {
//                    //check last occurance of the dropoff id associated with our pickup
//                    //also must be within the range of the subpermute we are checking
//                    if (master_hash_table[pickup_dropoff_ids[begin][i]] < last && master_hash_table[pickup_dropoff_ids[begin][i]] >= last - sub_permute[x].size()) {
//                        // auto index_of_dropoff = sub_permute[x].find(pickup_dropoff_ids[begin][i]);
//                        auto index_of_dropoff = find(sub_permute[x].begin(), sub_permute[x].end(), pickup_dropoff_ids[begin][i]);
//                        unsigned index = index_of_dropoff - sub_permute[x].begin();
//                        if (index < y) {
//                            break_out = true;
//                            break;
//                        }
//                    }
//                }
//                if (break_out) {
//                    break;
//                }
//            }
//
//            vector <unsigned> copy = sub_permute[x];
//            copy.insert(copy.begin() + y, begin);
//            permutations.push_back(copy);
//
//        }
//    }
//    return permutations;
//}
//
