#include "m1.h"
#include "m1_additional.h"
#include "m2_M.h"
#include "m3.h"
#include "m3_additional.h"
#include "StreetsDatabaseAPI.h"
#include <algorithm>
#include <queue>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <graphics.h>
using namespace std;

// Returns the time required to travel along the path specified, in seconds. 
// The path is given as a vector of street segment ids, and this function 
// can assume the vector either forms a legal path or has size == 0.
// The travel time is the sum of the length/speed-limit of each street 
// segment, plus the given turn_penalty (in seconds) per turn implied by the path. 
// A turn occurs when two consecutive street segments have different street IDs.

double compute_path_travel_time(const std::vector<unsigned>& path,
        const double turn_penalty) {
    double time = 0;
    // If there's an empty path, return 0s.
    if (path.empty()) {
        return time;
        // If there's only one segment, return its travel time.
    } else if (path.size() == 1) {
        time = find_street_segment_travel_time(path[0]);
        return time;
    } else {
        // Calculate the first segment travel time.
        StreetSegmentInfo previousSegment = getStreetSegmentInfo(path[0]);
        time = time + find_street_segment_travel_time(path[0]);
        for (unsigned int size = 1; size < path.size(); size++) {
            StreetSegmentInfo currentSegment = getStreetSegmentInfo(path[size]);
            double travelTime = find_street_segment_travel_time(path[size]);
            // Check if there's a turn from previous segment and current segment.
            if (previousSegment.streetID != currentSegment.streetID) {
                time = time + travelTime + turn_penalty;
            } else {
                time = time + travelTime;
            }
            previousSegment = currentSegment;
        }
    }

    return time;
}

double heuristic(unsigned current, unsigned dest) {
    double dist = find_distance_between_two_points(getIntersectionPosition(current), getIntersectionPosition(dest));

    return dist / fastest_speed()* 3.6;

}

// Found shortest path from start intersection to POI's nearest intersection.
// Used for printing instructions.
vector <unsigned> shortest_path_instruction;

// Returns a path (route) between the start intersection and the end 
// intersection, if one exists. This routine should return the shortest path
// between the given intersections when the time penalty to turn (change
// street IDs) is given by turn_penalty (in seconds).
// If no path exists, this routine returns an empty (size == 0) vector. 
// If more than one path exists, the path with the shortest travel time is 
// returned. The path is returned as a vector of street segment ids; traversing 
// these street segments, in the returned order, would take one from the start 
// to the end intersection.

vector<unsigned> find_path_between_intersections(const unsigned intersect_id_start,
        const unsigned intersect_id_end,
        const double turn_penalty) {
    
    shortest_path_instruction.clear();
    vector <unsigned> path;
    if (intersect_id_start == intersect_id_end) {
        return path;
    }
    
    path = a_star(intersect_id_start, intersect_id_end, turn_penalty);
    shortest_path_instruction = path;
    return path;
    
/*
    unordered_map <unsigned, unsigned> closet_intersection_list;
    closet_intersection_list.insert({intersect_id_end, 0});
    return Dijkstra(intersect_id_start, closet_intersection_list, turn_penalty);*/

}

// Returns the shortest travel time path (vector of street segments) from 
// the start intersection to a point of interest with the specified name.
// The path will begin at the specified intersection, and end on the 
// intersection that is closest (in Euclidean distance) to the point of 
// interest.
// If no such path exists, returns an empty (size == 0) vector.

std::vector<unsigned> find_path_to_point_of_interest(const unsigned intersect_id_start,
        const std::string point_of_interest_name,
        const double turn_penalty) {
    // Shortest path vector with segment IDs in it.
    vector <unsigned> path;
    // Matched POI IDs with given name.
    vector <unsigned> POI_id = getPointOfInterestID(point_of_interest_name);
    //Clear previous path instruction.
    shortest_path_instruction.clear();
    // If there is no POI matches given name.
    if (POI_id.empty()) {
        return path;
        // If there is only one POI matches given name, find the shortest path directly.
    } else if (POI_id.size() == 1) {
        LatLon POI_position = getPointOfInterestPosition(POI_id[0]);
        // Key(.first) is the intersect_id, and value(.second) is the POI_id.
        unordered_map <unsigned, unsigned> closest_intersection;
        closest_intersection.insert({find_closest_intersection(POI_position), POI_id[0]});
        path = Dijkstra(intersect_id_start, closest_intersection, turn_penalty);
        shortest_path_instruction = path;
        return path;
        // If there are 20 POIs match given name, find the shortest path among them.
    } else if (POI_id.size() <= 20) {
        // Key(.first) is the intersect_id, and value(.second) is the POI_id.
        unordered_map <unsigned, unsigned> closet_intersection_list;
        for (auto iter = POI_id.begin(); iter != POI_id.end(); iter++) {
            LatLon POI_position = getPointOfInterestPosition(*iter);
            closet_intersection_list.insert({find_closest_intersection(POI_position), *iter});
        }
        path = Dijkstra(intersect_id_start, closet_intersection_list, turn_penalty);
        shortest_path_instruction = path;
        return path;
        // If there are too many(>20) POIs match given name, use priority_queue to find shortest path. 
    } else {
        LatLon intersection_position = getIntersectionPosition(intersect_id_start);
        // Declare a pair for distance and POI id.
        // First is the distance from POI to start intersection.
        // Second is the POI_id.
        typedef pair<double, unsigned> POI_pair;
        // Smaller distance means higher priority.
        priority_queue <POI_pair, vector<POI_pair>, greater < POI_pair>> POI_priority_queue;

        // Store all matched POIs into POI_priority_queue.
        for (auto iter = POI_id.begin(); iter != POI_id.end(); iter++) {
            LatLon POI_position = getPointOfInterestPosition(*iter);
            double distance = find_distance_between_two_points(POI_position, intersection_position);
            POI_pair temp_pair(distance, *iter);
            POI_priority_queue.push(temp_pair);
        }
        // Key(.first) is the intersect_id, and value(.second) is the POI_id.
        unordered_map <unsigned, unsigned> closet_intersection_list;
        // Only take top 20 priorities into consideration. 
        for (unsigned i = 0; i <= 20; i++) {
            unsigned cloest_POI_id = POI_priority_queue.top().second;
            LatLon POI_position = getPointOfInterestPosition(cloest_POI_id);
            unsigned closet_intersection = find_closest_intersection(POI_position);
            closet_intersection_list.insert({closet_intersection, cloest_POI_id});
            // Remove stored POI.
            POI_priority_queue.pop();
        }
        path = Dijkstra(intersect_id_start, closet_intersection_list, turn_penalty);
        shortest_path_instruction = path;
        return path;
    }
}

vector <unsigned> Dijkstra(const unsigned intersect_id_start,
        unordered_map <unsigned, unsigned> closet_intersection_list,
        const double turn_penalty) {
    // Shortest path vector with segment IDs in it.
    vector <unsigned> path;

    // Total amount of intersections.
    unsigned intersection_amount = getNumberOfIntersections();

    // Index is intersection_id, pair.first is from segment, and pair.second is from intersection.
    vector <pair<unsigned, unsigned>> cameFrom;
    cameFrom.resize(intersection_amount);

    // Travel time from start point to the intersection. Set to be infinity here.
    vector <double> score;
    score.resize(intersection_amount, INFINITY);
    score[intersect_id_start] = 0;

    // Flags for each visited intersection.
    vector <bool> closeSet;
    closeSet.resize(intersection_amount, true);

    // First is the travel time (score).
    // Second is the intersection_id.
    typedef pair<double, unsigned> node_pair;
    // All nodes waiting for comparing in a priority_queue.
    // Less travel time means higher priority.
    priority_queue <node_pair, vector<node_pair>, greater < node_pair>> openSet;
    node_pair start_node(INFINITY, intersect_id_start);
    openSet.push(start_node);

    // Corner cases check.
    //    for (auto iter = closet_intersection_list.begin(); iter != closet_intersection_list.end(); iter++) {
    //        // Corner cases check.
    //        if (intersect_id_start == iterfirst || intersect_id_start >= intersection_amount) {
    //            return path;
    //        }
    //    }

    // While there's still nodes waiting for comparing.
    while (!openSet.empty()) {
        // Highest priority intersection.
        node_pair current_node = openSet.top();
        // Remove it from waiting list.
        openSet.pop();

        // If current intersection hasn't been visited.
        if (closeSet[current_node.second]) {
            unsigned current_intersection = current_node.second;

            // Check if the destination is found.
            if (closet_intersection_list.find(current_intersection) != closet_intersection_list.end()) {
                unsigned reconstructed_current_intersection = current_intersection;

                while (reconstructed_current_intersection != intersect_id_start) {
                    unsigned reconstructed_segment = cameFrom[reconstructed_current_intersection].first;
                    // Always insert reconstructed_segment before the begin of path. 
                    auto iter = path.begin();
                    path.insert(iter, reconstructed_segment);
                    // Basically made a duplicate of shortest path for printing instructions. 
                    //auto iter_instruction = shortest_path_instruction.begin();
                    //shortest_path_instruction.insert(iter_instruction, reconstructed_current_intersection);
                    // Came back to previous intersection.
                    unsigned reconstructed_previous_intersection = cameFrom[reconstructed_current_intersection].second;
                    reconstructed_current_intersection = reconstructed_previous_intersection;
                }

                // Insert start intersection to shortest_path_instruction.
                //auto iter_instruction = shortest_path_instruction.begin();
                //shortest_path_instruction.insert(iter_instruction, intersect_id_start);
                // Break out from the while loop.
                break;
            }

            // Current intersection != destination
            // Mark its flag to false.
            closeSet[current_intersection] = false;

            unsigned segment_amount = getIntersectionStreetSegmentCount(current_intersection);

            for (unsigned i = 0; i < segment_amount; i++) {
                unsigned current_segment = getIntersectionStreetSegment(current_intersection, i);
                StreetSegmentInfo temp_segment = getStreetSegmentInfo(current_segment);

                unsigned from = temp_segment.from;
                unsigned to = temp_segment.to;

                node_pair temp_node;

                // One way segment
                if (temp_segment.oneWay) {
                    // Check the legality
                    if (current_intersection == from) {
                        if (closeSet[to]) {
                            double time = find_street_segment_travel_time(current_segment);

                            if (current_intersection != intersect_id_start) {
                                StreetSegmentInfo current_info = getStreetSegmentInfo(cameFrom[current_intersection].first);
                                // Check if there's a turn
                                if (current_info.streetID != temp_segment.streetID) {
                                    time += turn_penalty;
                                }
                            }

                            time += score[current_intersection];

                            // Better score means shorter path
                            if (time < score[to]) {
                                score[to] = time;
                                cameFrom[to].first = current_segment;
                                cameFrom[to].second = current_intersection;
                            }

                            temp_node.first = score[to];
                            temp_node.second = to;
                            // Push to waiting list.
                            openSet.push(temp_node);
                        }
                    }
                    // Not one way segment.
                } else {
                    if (current_intersection == from) {
                        if (closeSet[to]) {
                            double time = find_street_segment_travel_time(current_segment);

                            if (current_intersection != intersect_id_start) {
                                StreetSegmentInfo current_info = getStreetSegmentInfo(cameFrom[current_intersection].first);
                                // Check if there's a turn
                                if (current_info.streetID != temp_segment.streetID) {
                                    time += turn_penalty;
                                }
                            }

                            time += score[current_intersection];

                            // Better score means shorter path
                            if (time < score[to]) {
                                score[to] = time;
                                cameFrom[to].first = current_segment;
                                cameFrom[to].second = current_intersection;
                            }

                            temp_node.first = score[to];
                            temp_node.second = to;
                            // Push to waiting list.
                            openSet.push(temp_node);
                        }
                        // current_intersection == to
                    } else {
                        if (closeSet[from]) {
                            double time = find_street_segment_travel_time(current_segment);

                            if (current_intersection != intersect_id_start) {
                                StreetSegmentInfo info = getStreetSegmentInfo(cameFrom[current_intersection].first);
                                // Check if there's a turn
                                if (info.streetID != temp_segment.streetID) {
                                    time += turn_penalty;
                                }
                            }

                            time += score[current_intersection];

                            // Better score means shorter path
                            if (time < score[from]) {
                                score[from] = time;
                                cameFrom[from].first = current_segment;
                                cameFrom[from].second = current_intersection;
                            }

                            temp_node.first = score[from];
                            temp_node.second = from;
                            // Push to waiting list.
                            openSet.push(temp_node);
                        }
                    }
                }
            }
        }
    }
    return path;
}

//path is found, data is backwards so use list to reorder & copy back to vector
//delete all dynamically allocated memory
vector <unsigned> finish_clean(node* & last, vector <node*> & get_node) {
    list <unsigned> street_segment_path_list;

    while (last->street_seg_id != -1) {
        street_segment_path_list.push_front(last->street_seg_id);
        last = last -> prev_node;
    }

    vector <unsigned> street_segment_path_vector{std::begin(street_segment_path_list), std::end(street_segment_path_list)};
    for (unsigned int x = 0; x < get_node.size(); x++) {
        if (get_node[x] != NULL) {
            delete get_node[x];
        }
    }
    get_node.clear();
    return street_segment_path_vector;

}

vector <unsigned> a_star(const unsigned intersect_id_start,
        const unsigned intersect_id_end,
        const double turn_penalty) {
    // Shortest path vector with segment IDs in it.
    vector <unsigned> empty;

    // Total amount of intersections.
    unsigned intersection_amount = getNumberOfIntersections();


    //get node* from intersection id, being in this means it is either in open
    //or closed set. additional serves to delete memory cleanly.
    vector <node*> get_node;
    get_node.resize(intersection_amount, NULL);

    //an open set stores what is still needed to bee searched
    //is sorted by lowest f using compare
    set <node*, compare> openSet;
    //put in first node
    node* start_node = new node();
    start_node->intersection_id = intersect_id_start;
    openSet.insert(start_node);
    get_node[intersect_id_start] = start_node;

    // While there's values for path to be found
    while (!openSet.empty()) {
        // first elem of openSet is highest priority, "pop" it off
        node* current_node = *openSet.begin();
        openSet.erase(openSet.begin());

        unsigned current_intersection = current_node->intersection_id;
        // Check if the destination is found.
        if (current_intersection== intersect_id_end) {
            openSet.clear();
            return finish_clean(current_node, get_node);
        }
        // Current intersection != destination
        // Mark its flag to false.
        current_node ->inClosed = true;

        unsigned segment_amount = getIntersectionStreetSegmentCount(current_intersection);
        //look at each neighbor
        for (unsigned i = 0; i < segment_amount; i++) {
            unsigned current_segemt = getIntersectionStreetSegment(current_intersection, i);
            StreetSegmentInfo temp_segment = getStreetSegmentInfo(current_segemt);
            unsigned from = temp_segment.from;
            unsigned to = temp_segment.to;
            unsigned next_intersection = to;

            //break from the loop if invalid dir, otherwise set
            //next_intersection to be the other intersection of the street seg
            if (current_intersection == to) {
                if (temp_segment.oneWay) {
                    continue;
                }
                next_intersection = from;
            }



            //calculate score first
            //f = g+h where g is time traveled already and h is estimate of
            //time still needed
            double h = heuristic(next_intersection, intersect_id_end);
            double g = current_node->g + find_street_segment_travel_time(current_segemt);
            if (current_node->street_seg_id!= -1 && temp_segment.streetID != getStreetSegmentInfo(current_node ->street_seg_id).streetID) {
                g += turn_penalty;
            }
            double f = h + g;



            //if true, then it is either in closed or open set
            if (get_node[next_intersection] != NULL) {
                //if the existing g is better than current g, we ignore this temp
                if (get_node[next_intersection]->g <= g) {
                    continue;
                }
                //current node is better, must insert that instead
                else {
                    //if its not in closed, && is in the array of existing nodes, it must be open
                    //delete from the open set & reinsert new node to maintain priority
                    //update get_node catalogue
                    if (get_node[next_intersection]->inClosed == false) {
                        node * delete_node = *(openSet.find(get_node[next_intersection]));
                        openSet.erase(get_node[next_intersection]);
                        delete delete_node;
                        delete_node = NULL;
                        node* temp_node = new node();
                        temp_node->f = f;
                        temp_node->g = g;
                        temp_node->h = h;
                        temp_node->street_seg_id = current_segemt;
                        temp_node->intersection_id = next_intersection;
                        temp_node->inClosed = false;
                        temp_node -> prev_node = current_node;
                        get_node[next_intersection] = temp_node;
                        openSet.insert(temp_node);
                    }
                    //in closed set
                    //simply update the node from get_node catalogue
                    else {
                        get_node[next_intersection] -> f = f;
                        get_node[next_intersection] -> g = g;
                        get_node[next_intersection] -> street_seg_id = current_segemt;
                        get_node[next_intersection] -> inClosed = false;
                        get_node[next_intersection] -> prev_node = current_node;
                        openSet.insert(get_node[next_intersection]);
                    }

                }
            }
            //neither in open set or closed set, not catalogued yet
            //create a new node & insert to open & get_node catalogue
            else {
                node* temp_node = new node();
                temp_node->f = f;
                temp_node->g = g;
                temp_node->h = h;
                temp_node->street_seg_id = current_segemt;
                temp_node->intersection_id = next_intersection;
                temp_node->inClosed = false;
                temp_node -> prev_node = current_node;
                get_node[next_intersection] = temp_node;
                openSet.insert(temp_node);
            }
        }

    }

    for (unsigned  int x = 0; x < get_node.size(); x++) {
        if (get_node[x] != NULL) {
            delete get_node[x];
        }
    }
    get_node.clear();
    openSet.clear();
    return empty;
}


//to return both the path & the intersection it reached
vector_and_intersection  modified_Dijkstra(const unsigned intersect_id_start,
        unordered_set <unsigned> closet_intersection_list,
        const double turn_penalty) {
    // Shortest path vector with segment IDs in it.
    vector <unsigned> path;
    unsigned closest_intersection = 0;
    // Total amount of intersections.
    unsigned intersection_amount = getNumberOfIntersections();
    
    // Index is intersection_id, pair.first is from segment, and pair.second is from intersection.
    vector <pair<unsigned, unsigned>> cameFrom;
    cameFrom.resize(intersection_amount);

    // Travel time from start point to the intersection. Set to be infinity here.
    vector <double> score;
    score.resize(intersection_amount, INFINITY);
    score[intersect_id_start] = 0;

    // Flags for each visited intersection.
    vector <bool> closeSet;
    closeSet.resize(intersection_amount, true);

    // First is the travel time (score).
    // Second is the intersection_id.
    typedef pair<double, unsigned> node_pair;
    // All nodes waiting for comparing in a priority_queue.
    // Less travel time means higher priority.
    priority_queue <node_pair, vector<node_pair>, greater < node_pair>> openSet;
    node_pair start_node(INFINITY, intersect_id_start);
    openSet.push(start_node);

    // While there's still nodes waiting for comparing.
    while (!openSet.empty()) {
        // Highest priority intersection.
        node_pair current_node = openSet.top();
        // Remove it from waiting list.
        openSet.pop();

        // If current intersection hasn't been visited.
        if (closeSet[current_node.second]) {
            unsigned current_intersection = current_node.second;

            // Check if the destination is found.
            if (closet_intersection_list.find(current_intersection) != closet_intersection_list.end()) {
                unsigned reconstructed_current_intersection = current_intersection;
                closest_intersection = current_intersection;
                while (reconstructed_current_intersection != intersect_id_start) {
                    unsigned reconstructed_segment = cameFrom[reconstructed_current_intersection].first;
                    // Always insert reconstructed_segment before the begin of path. 
                    auto iter = path.begin();
                    path.insert(iter, reconstructed_segment);
                    // Basically made a duplicate of shortest path for printing instructions. 
                    //auto iter_instruction = shortest_path_instruction.begin();
                    //shortest_path_instruction.insert(iter_instruction, reconstructed_current_intersection);
                    // Came back to previous intersection.
                    unsigned reconstructed_previous_intersection = cameFrom[reconstructed_current_intersection].second;
                    reconstructed_current_intersection = reconstructed_previous_intersection;
                }

                // Insert start intersection to shortest_path_instruction.
                //auto iter_instruction = shortest_path_instruction.begin();
                //shortest_path_instruction.insert(iter_instruction, intersect_id_start);
                // Break out from the while loop.
                break;
            }

            // Current intersection != destination
            // Mark its flag to false.
            closeSet[current_intersection] = false;

            unsigned segment_amount = getIntersectionStreetSegmentCount(current_intersection);

            for (unsigned i = 0; i < segment_amount; i++) {
                unsigned current_segment = getIntersectionStreetSegment(current_intersection, i);
                StreetSegmentInfo temp_segment = getStreetSegmentInfo(current_segment);

                unsigned from = temp_segment.from;
                unsigned to = temp_segment.to;

                node_pair temp_node;

                // One way segment
                if (temp_segment.oneWay) {
                    // Check the legality
                    if (current_intersection == from) {
                        if (closeSet[to]) {
                            double time = find_street_segment_travel_time(current_segment);

                            if (current_intersection != intersect_id_start) {
                                StreetSegmentInfo current_info = getStreetSegmentInfo(cameFrom[current_intersection].first);
                                // Check if there's a turn
                                if (current_info.streetID != temp_segment.streetID) {
                                    time += turn_penalty;
                                }
                            }

                            time += score[current_intersection];

                            // Better score means shorter path
                            if (time < score[to]) {
                                score[to] = time;
                                cameFrom[to].first = current_segment;
                                cameFrom[to].second = current_intersection;
                            }

                            temp_node.first = score[to];
                            temp_node.second = to;
                            // Push to waiting list.
                            openSet.push(temp_node);
                        }
                    }
                    // Not one way segment.
                } else {
                    if (current_intersection == from) {
                        if (closeSet[to]) {
                            double time = find_street_segment_travel_time(current_segment);

                            if (current_intersection != intersect_id_start) {
                                StreetSegmentInfo current_info = getStreetSegmentInfo(cameFrom[current_intersection].first);
                                // Check if there's a turn
                                if (current_info.streetID != temp_segment.streetID) {
                                    time += turn_penalty;
                                }
                            }

                            time += score[current_intersection];

                            // Better score means shorter path
                            if (time < score[to]) {
                                score[to] = time;
                                cameFrom[to].first = current_segment;
                                cameFrom[to].second = current_intersection;
                            }

                            temp_node.first = score[to];
                            temp_node.second = to;
                            // Push to waiting list.
                            openSet.push(temp_node);
                        }
                        // current_intersection == to
                    } else {
                        if (closeSet[from]) {
                            double time = find_street_segment_travel_time(current_segment);

                            if (current_intersection != intersect_id_start) {
                                StreetSegmentInfo info = getStreetSegmentInfo(cameFrom[current_intersection].first);
                                // Check if there's a turn
                                if (info.streetID != temp_segment.streetID) {
                                    time += turn_penalty;
                                }
                            }

                            time += score[current_intersection];

                            // Better score means shorter path
                            if (time < score[from]) {
                                score[from] = time;
                                cameFrom[from].first = current_segment;
                                cameFrom[from].second = current_intersection;
                            }

                            temp_node.first = score[from];
                            temp_node.second = from;
                            // Push to waiting list.
                            openSet.push(temp_node);
                        }
                    }
                }
            }
        }
    }
    vector_and_intersection id_and_path;
    id_and_path.path = path;
    id_and_path.intersection = closest_intersection;
    return id_and_path;
}