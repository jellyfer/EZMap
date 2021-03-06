/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   m4_additional.h
 * Author: caiwei7
 *
 * Created on April 3, 2017, 2:50 PM
 */

#ifndef M4_ADDITIONAL_H
#define M4_ADDITIONAL_H
#include <vector>

using namespace std;

struct path_and_cost {
    vector <unsigned> path;
    double cost = -1;
};

struct intersections_and_streetsegments {
    vector <unsigned> intersections;
    unordered_map< unsigned, vector <unsigned> > streetsegments;
};

vector<vector<unsigned>> permute (vector <unsigned> permute_this, unsigned last);
bool check_valid(vector <unsigned> sub_vector, unsigned start, unsigned last);

#endif /* M4_ADDITIONAL_H */

