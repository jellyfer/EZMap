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
//#include <m1.h>
//#include <m3_additional.h>
//#include <unordered_map>
//#include <thread>
//#include <queue>
//#include <chrono>
//#define TIME_LIMIT 30
//using namespace std;
////vector of street segment ids which form a legal path
////returns vector size 0 if no legal path
//
////std::vector<unsigned> traveling_courier(const std::vector<DeliveryInfo>& deliveries,
////        const std::vector<unsigned>& depots,
////        const float turn_penalty) {
////
////    auto startTime = chrono::high_resolution_clock::now();
////    bool timeOut = false;
////
////    //  bool debug = false;
////    vector <unsigned> complete_best_path;
////
////    //start with first depot 
////    unsigned try_depot_number = 0;
////
////
////
////    
////
////
////    unordered_map <unsigned, vector<unsigned> > pickup_dropoff_ids;
////
////
////    //look thru legal pickup & dropoff (O(n) max), if already picked up, only look at dropoff
////    //if not picked up, do not look at dropoff
////    unordered_set <unsigned> new_nodes_to_look_at;
////    for (unsigned int x = 0; x < deliveries.size(); x++) {
////        new_nodes_to_look_at.insert(deliveries[x].pickUp);
////
////        if (pickup_dropoff_ids.find(deliveries[x].pickUp) == pickup_dropoff_ids.end()) {
////            vector<unsigned> first_dropoff;
////            first_dropoff.push_back(deliveries[x].dropOff);
////            pickup_dropoff_ids.insert(make_pair(deliveries[x].pickUp, first_dropoff));
////        } else {
////            pickup_dropoff_ids[deliveries[x].pickUp].push_back(deliveries[x].dropOff);
////        }
////    
////
////
////        //if (deliveries[x].pickUp == 64489) {
////        //    cout << "im looking at the right one" << endl;
////        //       debug = true;
////        //  }
////
////        }
////
////
////    while (!timeOut && try_depot_number < depots.size()) {
////        unordered_set <unsigned> nodes_to_look_at = new_nodes_to_look_at;
////        vector <unsigned> best_path;
////        unsigned intersection_next = depots[try_depot_number];
////        //make sure starting depot is connected!
////        vector_and_intersection closest_next = modified_Dijkstra(intersection_next,
////                nodes_to_look_at,
////                turn_penalty);
////        if (closest_next.path.size() == 0) {
////            bool found = false;
////            //while we still have depots to check,
////            while (try_depot_number < depots.size()) {
////                intersection_next = depots[try_depot_number];
////                try_depot_number++;
////                //changed depot, now find again
////                closest_next = modified_Dijkstra(intersection_next,
////                        nodes_to_look_at,
////                        turn_penalty);
////                if (closest_next.path.size() != 0) {
////                    found = true;
////                    break;
////        }
////    }
////            //no more depots, return current best path
////            //EDIT: instead of returning current best path, will break out of this loop & try depots -> second best start
////            if (!found) {
////                //   cout <<"is itt going here?" <<endl;
////
////                break;
////                //return complete_best_path;
////            }
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
////                bool found = false;
////                //while we still have depots to check,
////                while (try_depot_number < depots.size()) {
////                    intersection_next = depots[try_depot_number];
////                    try_depot_number++;
////                    //changed depot, now find again
////                    closest_next = modified_Dijkstra(intersection_next,
////                            nodes_to_look_at,
////                            turn_penalty);
////                    if (closest_next.path.size() != 0) {
////                        found = true;
////                        break;
////                    }
////                }
////                //no more depots, return current best path
////                if (!found) {
////                    //   cout <<"is itt going here?" <<endl;
////
////                    return complete_best_path;
////                }
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
////
////
////
////
////
////
////
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
////
////
////
////                    //while looking for a path there is none
////
////                    vector<unsigned> empty;
////                    return empty;
////
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
////            // end with depot closest to last dropoff
////            //last visited is currently stored as intersection_next
////            unordered_set <unsigned> depot_set;
////            for (unsigned int x = 0; x < depots.size(); x++) {
////                depot_set.insert(depots[x]);
////            }
////
////            closest_next = modified_Dijkstra(intersection_next,
////                    depot_set,
////                    turn_penalty);
////
////            //insert path from last droppoff to depot
////            best_path.insert(best_path.end(), closest_next.path.begin(), closest_next.path.end());
////            if (complete_best_path.size() == 0 || compute_path_travel_time(complete_best_path, turn_penalty) > compute_path_travel_time(best_path, turn_penalty)) {
////            //    cout << "heheh does it even go here" <<endl;
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
////}
////
////
////        return complete_best_path;
////
////
////}
//
//
//
//unordered_map < unsigned, vector<unsigned> > pickups;
//unordered_map < unsigned, DropOffInfo > dropoffs;
//unordered_map < unsigned, vector<unsigned> > pickup_dropoff_ids;
//vector <unsigned> priorDepots;
//vector <unsigned> *threadPath;
//float turn_penalty_global;
//vector <unsigned> depots_global;
//
////vector<unsigned> traveling_courier_c(const vector<DeliveryInfo>& deliveries,
////        const vector<unsigned>& depots,
////        const float turn_penalty);
//
//vector<unsigned> traveling_courier(const vector<DeliveryInfo>& deliveries,
//        const vector<unsigned>& depots,
//        const float turn_penalty) {
//    turn_penalty_global = turn_penalty;
//    depots_global = depots;
//    vector <unsigned> complete_best_path;
//
//    complete_best_path.clear();
//
//    // Corner cases check.
//    if (deliveries.empty() || depots.empty()) {
//        return complete_best_path;
//    }
//
//    double minDistance = INFINITY;
//    // Loop through all depots and rank them by total distance to all deliveries.
//    for (unsigned i = 0; i < depots.size(); i++) {
//        double totalDistance = 0;
//        for (unsigned j = 0; j < deliveries.size(); j++) {
//            totalDistance += find_distance_between_two_points(getIntersectionPosition(depots[i]), getIntersectionPosition(deliveries[j].pickUp));
//            totalDistance += find_distance_between_two_points(getIntersectionPosition(depots[i]), getIntersectionPosition(deliveries[j].dropOff));
//        }
//        if (totalDistance <= minDistance) {
//            priorDepots.insert(priorDepots.begin(), depots[i]);
//            minDistance = totalDistance;
//        }
//    }
//
//    // Re-build data structure.
//    reconstructData(deliveries);
//
//    unsigned pathSize;
//    if (priorDepots.size() > 16) {
//        threadPath = new vector <unsigned>[16];
//        pathSize = 16;
//    } else {
//        threadPath = new vector <unsigned>[priorDepots.size()];
//        pathSize = priorDepots.size();
//    }
//
//    thread myThread[16];
//    for (unsigned i = 0; i < pathSize; i++) {
//        myThread[i] = thread(findPath, i);
//    }
//
//    // Synchronize threads 
//    for (unsigned i = 0; i < pathSize; i++) {
//        myThread[i].join();
//    }
//
//    double bestTime = INFINITY;
//    unsigned bestPathID = 0;
//    // Loop through all paths and find the best one.
//    for (unsigned i = 0; i < pathSize; i++) {
//        if (!threadPath[i].empty()) {
//            double time = compute_path_travel_time(threadPath[i], turn_penalty);
//            if (time < bestTime) {
//                bestPathID = i;
//                bestTime = time;
//            }
//        }
//    }
//
//    complete_best_path = threadPath[bestPathID];
//
//    delete[] threadPath;
//    pickups.clear();
//    dropoffs.clear();
//    priorDepots.clear();
//    pickup_dropoff_ids.clear();
//    depots_global.clear();
//
//
//    //    StreetSegmentInfo first = getStreetSegmentInfo(complete_best_path[0]);
//    //    StreetSegmentInfo last = getStreetSegmentInfo(complete_best_path[complete_best_path.size()-1]);
//    //    cout << first.from << endl;
//    //    cout << first.to   << endl;
//    //    cout << last.from << endl;
//    //    cout << last.to << endl;
//
//    return complete_best_path;
//}
//
//void reconstructData(const std::vector<DeliveryInfo>& deliveries) {
//
//    for (unsigned i = 0; i < deliveries.size(); i++) {
//        
//        if (pickup_dropoff_ids.find(deliveries[i].pickUp) == pickup_dropoff_ids.end()) {
//            vector<unsigned> temp_pickup;
//            temp_pickup.push_back(deliveries[i].pickUp);
//            pickup_dropoff_ids.insert(make_pair(deliveries[i].dropOff, temp_pickup));
//        } else {
//            pickup_dropoff_ids[deliveries[i].dropOff].push_back(deliveries[i].pickUp);
//        }
//        
//        // Check if pickup point in the vector
//        if (pickups.find(deliveries[i].pickUp) == pickups.end()) {
//            vector <unsigned> temp_dropoff;
//            temp_dropoff.push_back(deliveries[i].dropOff);
//            pickups.insert(make_pair(deliveries[i].pickUp, temp_dropoff));
//        } else {
//            pickups.find(deliveries[i].pickUp)->second.push_back(deliveries[i].dropOff);
//        }
//        
//        // Check if drop off point in the vector
//        if (dropoffs.find(deliveries[i].dropOff) == dropoffs.end()) {
//            DropOffInfo temp_info;
//            temp_info.currentCount = 0;
//            temp_info.totalCount = 1;
//            dropoffs.insert(make_pair(deliveries[i].dropOff, temp_info));
//        } else {
//            dropoffs.find(deliveries[i].dropOff)->second.totalCount++;
//        }
//    }
//}
//
//void findPath(unsigned threadID) {
//    // Initialize source point as one of the depot
//    unsigned sourcePoint = priorDepots[threadID];
//    // Make a duplicate for drop off vector
//    auto dropoffs_thread = dropoffs;
//    // Valid points for visiting
//    unordered_set <unsigned> validPoints;
//
//    // Insert all pickup points into valid points
//    for (auto iter = pickups.begin(); iter != pickups.end(); iter++) {
//        validPoints.insert(iter->first);
//    }
//
//    while (!validPoints.empty()) {
//        vector_and_intersection closest_next = modified_Dijkstra(sourcePoint, validPoints, turn_penalty_global);
//        // No path found
//        if (closest_next.path.empty()) {
//            threadPath[threadID].clear();
//            return;
//        }
//        // Add partial path to completed path
//        threadPath[threadID].insert(threadPath[threadID].end(), closest_next.path.begin(), closest_next.path.end());
//        // Erase corresponding intersection from valid points 
//        validPoints.erase(closest_next.intersection);
//
//        // Change source point into previous end point
//        sourcePoint = closest_next.intersection;
//        // If previous end pint is a pickup point
//        if (pickups.find(closest_next.intersection) != pickups.end()) {
//            auto temp_dropoff = pickups.at(closest_next.intersection);
//            for (unsigned i = 0; i < temp_dropoff.size(); i++) {
//                // Increase current count of this drop off point
//                dropoffs_thread.at(temp_dropoff[i]).currentCount++;
//                // Check if all deliveries of this drop off point has completed
//                if (dropoffs_thread.at(temp_dropoff[i]).totalCount == dropoffs_thread.at(temp_dropoff[i]).currentCount) {
//                    // Add this drop off point to valid points
//                    validPoints.insert(temp_dropoff[i]);
//                }
//            }
//        }
//    }
//    
//    // Add all depots into valid points to complete the path
//    for (unsigned i = 0; i < depots_global.size(); i++) {
//        validPoints.insert(depots_global[i]);
//    }
//    vector_and_intersection destinationPath = modified_Dijkstra(sourcePoint, validPoints, turn_penalty_global);
//
//    if (destinationPath.path.empty()) {
//        threadPath[threadID].clear();
//        return;
//    }
//    
//    // Add final parts of path into completed path
//    threadPath[threadID].insert(threadPath[threadID].end(), destinationPath.path.begin(), destinationPath.path.end());
//
//    return;
//}
