/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   m3_additional.h
 * Author: caiwei7
 *
 * Created on March 18, 2017, 3:42 AM
 */

#ifndef M3_ADDITIONAL_H
#define M3_ADDITIONAL_H
#include "StreetsDatabaseAPI.h"
#include <boost/heap/fibonacci_heap.hpp>
#include <unordered_map>
#include <unordered_set>
#include <m4.h>
using namespace std;

struct node {
    int intersection_id = -1;
    int  street_seg_id = -1;
    double f = 0;
    double g = 0;
    double h = 0; //cost
    bool inClosed = false;
    node* prev_node;
};

double heuristic (unsigned current, unsigned dest);


struct vector_and_intersection{
    unsigned intersection;
    vector <unsigned> path;
};

struct compare{
    bool operator()( node* const& n1,  node* const& n2){
        return n1->f < n2->f;
    }
};


// Dijkstra algorithm implemented for POI path search. 
std::vector <unsigned> Dijkstra(const unsigned intersect_id_start, 
                           std::unordered_map <unsigned, unsigned> closet_intersection_list, 
                           const double turn_penalty);

std::vector <unsigned> a_star (const unsigned intersect_id_start,
        const unsigned intersect_id_end,
        const double turn_penalty);

//delete all dynamically allocated memory
vector <unsigned> finish_clean(node* & last, vector <node*> & get_node);

vector_and_intersection  modified_Dijkstra(const unsigned intersect_id_start,
        unordered_set <unsigned> closet_intersection_list,
        const double turn_penalty);

void reconstructData (const vector<DeliveryInfo>& deliveries);

void findPath (unsigned threadID);

struct DropOffInfo {
    unsigned totalCount;
    unsigned currentCount;
};
#endif /* M3_ADDITIONAL_H */

