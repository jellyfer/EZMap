/*
 * Pre-load necessary database. (Will be organized up as a class)
 *
 */

/*
 * File:   preload.h
 * Author: caojianr
 *
 * Created on January 30, 2017, 6:31 PM
 */
#ifndef PRELOAD_H
#define PRELOAD_H

#include <vector>
#include <unordered_map>
#include <algorithm>
#include "m1.h"
#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h"
#include <boost/geometry/index/rtree.hpp>
#include <boost/geometry.hpp>
#include <boost/geometry/core/cs.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/box.hpp>

using namespace std;
//namespace and defined types for r-trees
namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;
typedef bg::model::point<double, 2, bg::cs::cartesian> point;
typedef bg::model::box<point> box;
typedef std::pair<box, unsigned> values;

//Declarations for functions.
void OSMWay_ids_preload (unordered_map <OSMID, const OSMWay*> &OSMWay_ids_map);
void street_segment_by_street_type_preload(unordered_map <string, vector<unsigned int>>  & OSMWay_street_types_map, unordered_map <OSMID, const OSMWay*> &OSMWay_ids_map_load);
void insert_into_OSMWay_street_types (unordered_map <string, vector<unsigned int>>  & OSMWay_street_types_map, string type, unsigned int x);
void street_ids_preload(unordered_map <string, vector<unsigned>> &street_ids_map);
void street_street_segments_preload(vector<vector<unsigned>> &street_street_segments, float &highest_speed_limit);
void street_intersections_preload(vector<vector<unsigned>> &street_intersections);
void street_segment_length_preload(vector <double> & street_segment_length);
void street_length_preload(vector <double> & street_length);
void speed_limit_preload(vector <double> & speed_limit);
void curve_point_count_preload(vector <unsigned> & curve_point);
void street_id_preload(vector <StreetIndex> & street_id);
void intersection_street_segments_preload(vector<vector<unsigned>> &id0_intersection_street_segments);
void adjacent_intersections_preload(vector <vector<unsigned> > & id0_adjacent_intersections);
double cos_lat_avg_CI();
double cos_lat_avg_POI();
void poi_ids_preload(unordered_map <string, vector<unsigned>> &POI_id_map);
bgi::rtree<values, bgi::rstar<10> > closest_intersection_preload();
bgi::rtree<values, bgi::rstar<10> > closest_point_of_interest_preload();
#endif /* PRELOAD_H */
