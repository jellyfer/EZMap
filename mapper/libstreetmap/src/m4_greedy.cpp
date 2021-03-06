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
//#include <m4.h>
//#include <m3.h>
//#include <m3_additional.h>
//#include <unordered_map>
//#include <chrono>
//#define TIME_LIMIT 30
//#define GREEDY_TIME_LIMIT 25
//
//using namespace std;
////vector of street segment ids which form a legal path
////returns vector size 0 if no legal path
//std::vector<unsigned> generate_best_path_greedy(vector <unsigned> depots, unordered_map < unsigned, vector<unsigned> > pickup_dropoff_ids, unordered_set <unsigned> nodes, unsigned num_diff_starts, unsigned turn_penalty);
//
//std::vector<unsigned> traveling_courier(const std::vector<DeliveryInfo>& deliveries,
//        const std::vector<unsigned>& depots,
//        const float turn_penalty) {
//
////    auto startTime = chrono::high_resolution_clock::now();
//
////    bool debug = false;
//    vector <unsigned> complete_best_path;
//
//    //start with first depot 
////    unsigned try_depot_number = 0;
//    unordered_map < unsigned, vector<unsigned> > pickup_dropoff_ids;
////    bool timeOut = false;
//    
//    //look thru legal pickup & dropoff (O(n) max), if already picked up, only look at dropoff
//    //if not picked up, do not look at dropoff
//    unordered_set <unsigned> new_nodes_to_look_at;
//    for (unsigned int x = 0; x < deliveries.size(); x++) {
//        new_nodes_to_look_at.insert(deliveries[x].pickUp);
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
//        //if (deliveries[x].pickUp == 64489) {
//        //    cout << "im looking at the right one" << endl;
//        //       debug = true;
//        //  }
//
//    }
//
//
////    while (!timeOut && try_depot_number < depots.size()) {
////        unordered_set <unsigned> nodes_to_look_at = new_nodes_to_look_at;
////        vector <unsigned> best_path;
////        unsigned intersection_next = depots[try_depot_number];
////        //make sure starting depot is connected!
////        vector_and_intersection closest_next = modified_Dijkstra(intersection_next,
////                nodes_to_look_at,
////                turn_penalty);
////        if (closest_next.path.size() == 0) {
////            try_depot_number++;
////            continue;
//////            bool found = false;
//////            //while we still have depots to check,
//////            while (try_depot_number < depots.size()) {
//////                intersection_next = depots[try_depot_number];
//////                try_depot_number++;
//////                //changed depot, now find again
//////                closest_next = modified_Dijkstra(intersection_next,
//////                        nodes_to_look_at,
//////                        turn_penalty);
//////                if (closest_next.path.size() != 0) {
//////                    found = true;
//////                    break;
//////                }
//////            }
//////            //no more depots, return current best path
//////            //EDIT: instead of returning current best path, will break out of this loop & try depots -> second best start
//////            if (!found) {
//////                //   cout <<"is itt going here?" <<endl;
//////
//////                break;
//////                //return complete_best_path;
//////            }
////        }//first node found
////        else {
////            best_path.insert(best_path.end(), closest_next.path.begin(), closest_next.path.end());
////            intersection_next = closest_next.intersection;
////            //need to update the set of nodes to look at, if the closest_next is a
////            //pickup, we need to remove current & add the corresponding dropoff.
////            //if closest_next is a dropoff, we simply remove current
////            nodes_to_look_at.erase(intersection_next);
////
////            //   if (debug && intersection_next == 32523) {
////            //      cout << "it reaches here.. " << endl;
////            // }
////            if (pickup_dropoff_ids.find(intersection_next) != pickup_dropoff_ids.end()) {
////                //insert the corresponding dropoff
////                for (unsigned int x = 0; x < pickup_dropoff_ids[intersection_next].size(); x++)
////                    nodes_to_look_at.insert(pickup_dropoff_ids[intersection_next][x]);
////                //        if (debug && intersection_next == 32523){
////                //      cout << pickup_dropoff_ids [intersection_next] <<" should equal 92242"<<endl;
////                // }
////            }
////        }
////
////
////
////
////
////
////
////
////
////        while (nodes_to_look_at.size() > 0) {
////            //finds the next legal dropoff/delivery intersection & path to get there
////            closest_next = modified_Dijkstra(intersection_next,
////                    nodes_to_look_at,
////                    turn_penalty);
////            //absolutely no path found to any of the remaining nodes, return vector sized o
////            if (closest_next.path.size() == 0) {
////                // if (debug == true) 
////                //  cout << "why does i tenter here?" << intersection_next << endl;
////
////
////
////
////                //while looking for a path there is none
////
////                vector<unsigned> empty;
////                return empty;
////
////            }
////            //insert the path to get there into best_path
////            best_path.insert(best_path.end(), closest_next.path.begin(), closest_next.path.end());
////            intersection_next = closest_next.intersection;
////            //need to update the set of nodes to look at, if the closest_next is a
////            //pickup, we need to remove current & add the corresponding dropoff.
////            //if closest_next is a dropoff, we simply remove current
////            nodes_to_look_at.erase(intersection_next);
////
////            //   if (debug && intersection_next == 32523) {
////            //      cout << "it reaches here.. " << endl;
////            // }
////            if (pickup_dropoff_ids.find(intersection_next) != pickup_dropoff_ids.end()) {
////                //insert the corresponding dropoff
////                for (unsigned int x = 0; x < pickup_dropoff_ids[intersection_next].size(); x++)
////                    nodes_to_look_at.insert(pickup_dropoff_ids[intersection_next][x]);
////                //        if (debug && intersection_next == 32523){
////                //      cout << pickup_dropoff_ids [intersection_next] <<" should equal 92242"<<endl;
////                // }
////            }
////
////
////        }
////
////        // end with depot closest to last dropoff
////        //last visited is currently stored as intersection_next
////        unordered_set <unsigned> depot_set;
////        for (unsigned int x = 0; x < depots.size(); x++) {
////            depot_set.insert(depots[x]);
////        }
////
////        closest_next = modified_Dijkstra(intersection_next,
////                depot_set,
////                turn_penalty);
////
////        //insert path from last droppoff to depot
////        best_path.insert(best_path.end(), closest_next.path.begin(), closest_next.path.end());
////        if (complete_best_path.size() == 0 || compute_path_travel_time(complete_best_path, turn_penalty) > compute_path_travel_time(best_path, turn_penalty)) {
////            complete_best_path = best_path;
////        }
////
////        auto currentTime = chrono::high_resolution_clock::now();
////        auto wallClock = chrono::duration_cast <chrono::duration<double>> (currentTime - startTime);
////        if (wallClock.count() > 0.9 * TIME_LIMIT) {
////            timeOut = true;
////        }
////        try_depot_number++;
////
////        //     cout <<try_depot_number<<endl;
////
////    }
////
////
////    //if it doesn't timeout & breaks out of the loop
////    //meaning, all depot starts have been searched
////    //should now start trying second best from depots
////    //only works if there are more than 1 node to look at since we're looking at second node
////    if (!timeOut && new_nodes_to_look_at.size() > 1) {
////        
////   //     cout <<"enters here" <<endl;
////        try_depot_number = 0;
////        while (!timeOut && try_depot_number < depots.size()) {
////            unordered_set <unsigned> nodes_to_look_at = new_nodes_to_look_at;
////            vector <unsigned> best_path;
////            unsigned intersection_next = depots[try_depot_number];
////            //make sure starting depot is connected!
////            vector_and_intersection closest_next = modified_Dijkstra(intersection_next,
////                    nodes_to_look_at,
////                    turn_penalty);
////            if (closest_next.path.size() == 0) {
////                continue;
////            }//first node found
////                //skip first node and look at 2nd closest node
////            else {
////                //takes the found first pickup
////                unsigned closest_pickup = closest_next.intersection;
////                //removes it from nodes to look at
////                nodes_to_look_at.erase(closest_pickup);
////                //looks at next closest node to depot
////                closest_next = modified_Dijkstra(intersection_next,
////                        nodes_to_look_at,
////                        turn_penalty);
////                //makes best path
////                best_path.insert(best_path.end(), closest_next.path.begin(), closest_next.path.end());
////
////                intersection_next = closest_next.intersection;
////                //need to update the set of nodes to look at, if the closest_next is a
////                //pickup, we need to remove current & add the corresponding dropoff.
////                //if closest_next is a dropoff, we simply remove current
////                //we also reinsert the best
////                nodes_to_look_at.erase(intersection_next);
////                nodes_to_look_at.insert(closest_pickup);
////                //   if (debug && intersection_next == 32523) {
////                //      cout << "it reaches here.. " << endl;
////                // }
////                if (pickup_dropoff_ids.find(intersection_next) != pickup_dropoff_ids.end()) {
////                    //insert the corresponding dropoff
////                    for (unsigned int x = 0; x < pickup_dropoff_ids[intersection_next].size(); x++)
////                        nodes_to_look_at.insert(pickup_dropoff_ids[intersection_next][x]);
////                    //        if (debug && intersection_next == 32523){
////                    //      cout << pickup_dropoff_ids [intersection_next] <<" should equal 92242"<<endl;
////                    // }
////                }
////            }
////
////            while (nodes_to_look_at.size() > 0) {
////                //finds the next legal dropoff/delivery intersection & path to get there
////                closest_next = modified_Dijkstra(intersection_next,
////                        nodes_to_look_at,
////                        turn_penalty);
////                //absolutely no path found to any of the remaining nodes, return vector sized o
////                if (closest_next.path.size() == 0) {
////                    // if (debug == true) 
////                    //  cout << "why does i tenter here?" << intersection_next << endl;
////
////                    //while looking for a path there is none
////                    vector<unsigned> empty;
////                    return empty;
////                }
////                //insert the path to get there into best_path
////                best_path.insert(best_path.end(), closest_next.path.begin(), closest_next.path.end());
////                intersection_next = closest_next.intersection;
////                //need to update the set of nodes to look at, if the closest_next is a
////                //pickup, we need to remove current & add the corresponding dropoff.
////                //if closest_next is a dropoff, we simply remove current
////                nodes_to_look_at.erase(intersection_next);
////
////                //   if (debug && intersection_next == 32523) {
////                //      cout << "it reaches here.. " << endl;
////                // }
////                if (pickup_dropoff_ids.find(intersection_next) != pickup_dropoff_ids.end()) {
////                    //insert the corresponding dropoff
////                    for (unsigned int x = 0; x < pickup_dropoff_ids[intersection_next].size(); x++)
////                        nodes_to_look_at.insert(pickup_dropoff_ids[intersection_next][x]);
////                    //        if (debug && intersection_next == 32523){
////                    //      cout << pickup_dropoff_ids [intersection_next] <<" should equal 92242"<<endl;
////                    // }
////                }
////
////
////            }
////
////            
////            /*unordered_set <unsigned> depot_set;
////            for (unsigned int x = 0; x < depots.size(); x++) {
////                depot_set.insert(depots[x]);
////            }*/
////            
////            unordered_set <unsigned> depot_set(depots.begin(), depots.end());
////
////            //intersection_next is last dropoff
////            //find closest depot to last dropoff
////            closest_next = modified_Dijkstra(intersection_next,
////                    depot_set,
////                    turn_penalty);
////
////            //insert path from last dropoff to depot
////            best_path.insert(best_path.end(), closest_next.path.begin(), closest_next.path.end());
////            if (complete_best_path.size() == 0 || compute_path_travel_time(complete_best_path, turn_penalty) > compute_path_travel_time(best_path, turn_penalty)) {
////                //cout << "heheh does it even go here" <<endl;
////                complete_best_path = best_path;
////            }
////
////            auto currentTime = chrono::high_resolution_clock::now();
////            auto wallClock = chrono::duration_cast <chrono::duration<double>> (currentTime - startTime);
////            if (wallClock.count() > 0.9 * TIME_LIMIT) {
////                timeOut = true;
////            }
////            try_depot_number++;
////         //   cout << try_depot_number <<endl;
////
////
////
////
////
////        }
////    }
//    //complete_best_path = generate_best_path_greedy(depots, pickup_dropoff_ids, new_nodes_to_look_at, new_nodes_to_look_at.size(), turn_penalty);
//    complete_best_path = generate_best_path_greedy(depots, pickup_dropoff_ids, new_nodes_to_look_at, new_nodes_to_look_at.size(), turn_penalty);
//
//    return complete_best_path;
//}
//
//vector<unsigned> generate_best_path_greedy(vector <unsigned> depots, 
//        unordered_map < unsigned, vector<unsigned> > pickup_dropoff_ids, 
//        unordered_set <unsigned> nodes, 
//        unsigned num_diff_starts, 
//        unsigned turn_penalty) {
//    auto startTime = chrono::high_resolution_clock::now();
//    
//    unsigned depot_i;
//    unsigned start = 0;
//    unordered_set <unsigned> depot_set(depots.begin(), depots.end());
//    vector <unsigned> best_path;
//    unordered_set <unsigned> previous_starts;
//    
//    //cout << "INITIALIZED" << endl;
//    while (start < num_diff_starts && start < nodes.size()) {
//        depot_i = 0;
//        //cout << " BEGINNING: " << (start+1) << endl;
//        
//        while (depot_i < depots.size()) {
//            auto currentTime3 = chrono::high_resolution_clock::now();
//            auto wallClock3 = chrono::duration_cast <chrono::duration<double>> (currentTime3 - startTime);
//            if (wallClock3.count() > GREEDY_TIME_LIMIT) {
//                return best_path;
//            }
//            
//            unsigned node = depots[depot_i];
//            //ignore previous starts in order to find kth start node
//            unordered_set <unsigned> nodes_to_look_at(nodes);
//            for (auto it = previous_starts.begin(); it != previous_starts.end(); it++) {
//                nodes_to_look_at.erase(*it);
//            }
//            
//            //cout << " CALCULATING: " << (start+1) << endl;
//            //make sure starting depot is connected!
//            vector_and_intersection closest_node = modified_Dijkstra(node,
//                    nodes_to_look_at,
//                    turn_penalty);
//
//            if (closest_node.path.size() == 0) {
//                depot_i++;
//                continue;
//            }
//                
//            //cout << " FOUND CONN: " << (start+1) << endl;
//            //add back previous starts
//            nodes_to_look_at.insert(previous_starts.begin(), previous_starts.end());
//            
//            //initialize path with path from depot to kth start node
//            vector <unsigned> path(closest_node.path);
//            
//            //keep track of picked up nodes as to not enqueue its dropoffs if encountered again
//            unordered_set <unsigned> was_picked_up;
//            
//            //need to update the set of nodes to look at, if the closest_next is a
//            //pickup, we need to remove current & add the corresponding dropoff.
//            //if closest_next is a dropoff, we simply remove current
//            node = closest_node.intersection;
//            previous_starts.insert(node);
//            nodes_to_look_at.erase(node);
//            
//            
//            // insert dropoffs if node is pickup
//            if (pickup_dropoff_ids.find(node) != pickup_dropoff_ids.end() && was_picked_up.find(node) == was_picked_up.end()) {
//                was_picked_up.insert(node);
//                //cout << " INSERTING DROPOFFS: " << (start+1) << endl;
//                for (unsigned int x = 0; x < pickup_dropoff_ids[node].size(); x++) {
//                    nodes_to_look_at.insert(pickup_dropoff_ids[node][x]);
//                }
//            }
//
//            //cout << " CHECKING OTHERS: " << (start+1) << endl;
//            while (nodes_to_look_at.size() > 0) {
//                auto currentTime2 = chrono::high_resolution_clock::now();
//                auto wallClock2 = chrono::duration_cast <chrono::duration<double>> (currentTime2 - startTime);
//                if (wallClock2.count() > GREEDY_TIME_LIMIT) {
//                    return best_path;
//                }
//                
//                //finds the next legal dropoff/delivery intersection & path to get there
//                closest_node = modified_Dijkstra(node,
//                        nodes_to_look_at,
//                        turn_penalty);
//                //cout << " 1: " << (start+1) << endl;
//                //absolutely no path found to any of the remaining nodes, return vector sized o
//                if (closest_node.path.size() == 0) {
//                    vector<unsigned> empty;
//                    return empty;
//                }
//                //cout << " 2: " << (start+1) << endl;
//                //insert the path to get there into best_path
//                path.insert(path.end(), closest_node.path.begin(), closest_node.path.end());
//                node = closest_node.intersection;
//                
//                //cout << " 3: " << (start+1) << endl;
//                //need to update the set of nodes to look at, if the closest_next is a
//                //pickup, we need to remove current & add the corresponding dropoff.
//                //if closest_next is a dropoff, we simply remove current
//                nodes_to_look_at.erase(node);
//                //cout << " 4: " << (start+1) << endl;
//                if (pickup_dropoff_ids.find(node) != pickup_dropoff_ids.end() && was_picked_up.find(node) == was_picked_up.end()) {
//                    was_picked_up.insert(node);
//                    //insert the corresponding dropoff
//                    for (unsigned int x = 0; x < pickup_dropoff_ids[node].size(); x++) {
//                        nodes_to_look_at.insert(pickup_dropoff_ids[node][x]);
//                    }
//                }
//                //cout << " 5: " << (start+1) << endl;
//                
//            }
//
//            //node is last dropoff
//            //find closest depot to last dropoff
//            closest_node = modified_Dijkstra(node,
//                    depot_set,
//                    turn_penalty);
//            
//            //cout << " INSERTING FINAL DEPOT: " << (start+1) << endl;
//            //insert path from last dropoff to depot
//            path.insert(path.end(), closest_node.path.begin(), closest_node.path.end());
//            //cout << " INSERTING FINAL DEPOT *: " << (start+1) << endl;
//            if (best_path.size() == 0 || compute_path_travel_time(best_path, turn_penalty) > compute_path_travel_time(path, turn_penalty)) {
//                best_path = path;
//            }
//                
//            depot_i++;
//        }
//        //cout << " TIMING *: " << (start+1) << endl;
//        
//        //cout << "END: " << (start+1) << endl;
//        start++;
//    }
//    return best_path;
//}