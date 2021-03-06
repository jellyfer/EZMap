/*
 * This milestone is designed to build organized databases and implement basic
 * functionalities of a mapping program.
 */
#include <vector>
#include <unordered_map>
#include <math.h>
#include <string>
#include <algorithm>

#include <boost/geometry/index/rtree.hpp>
#include <boost/geometry.hpp>
#include <boost/geometry/core/cs.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/box.hpp>

#include "m1.h"
#include "m1_additional.h"
#include "StreetsDatabaseAPI.h"
#include "preload.h"
extern void draw_map();

using namespace std;
//namespace and defined types for r-trees
namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;
typedef bg::model::point<double, 2, bg::cs::cartesian> point;
typedef bg::model::box<point> box;
typedef std::pair<box, unsigned> value;

extern unordered_map <string, vector<unsigned>> POItype_by_POI_id;
//Organized database.
//maps from OSMID to the associated way
unordered_map <OSMID, const OSMWay*> OSMWay_ids_map;
//POI_ID
unordered_map <string,vector <unsigned>> POI_id;
// maps OSMWays to type of road (eg highway, major, minor, etc.), for road OSMWays only
unordered_map <string, vector<unsigned int>> street_segment_types_map;
//Pre-calculated street_segment_length and street_length sorted by street_ids.
vector <double> street_segment_length;
vector <double> street_length;
//Pre-loaded speed_limit of each segment for quick access.
vector <double> speed_limit;
//Associate street ids with street names.
unordered_map<string, vector<unsigned>> find_street_ids_from_name_map;
//Store all the street segments with their corresponding intersections.
vector <vector<unsigned>> id_intersection_street_segments;
//Pre-loaded adjacent intersections
vector <vector<unsigned>> id0_adjacent_intersections;
//Pre-loaded street segments for each street.
vector <vector<unsigned>> street_street_segments;
//Pre-loaded intersections for each street.
vector <vector<unsigned>> street_intersections;
//Global variables used for quick calculations in closest_point_of_interest
//and closest_intersections.
double cosLatAvgPOI;
double cosLatAvgCI;
//R-trees stored with closest_point_of_interest and closest_intersections.
bgi::rtree<value, bgi::rstar<10> > closest_point_of_interest;
bgi::rtree<value, bgi::rstar<10> > closest_intersection;
float highest_speed_limit;

//loads map given a street path such as "/cad2/ece297s/public/maps/toronto_canada.streets.bin"
bool load_map(string map_name) {
    string osm_path = map_name.substr(0,map_name.find("streets.bin")) + "osm.bin";
    if (!loadStreetsDatabaseBIN(map_name) || !loadOSMDatabaseBIN(osm_path))
        return false;
    //Pre-load necessary database.
    OSMWay_ids_preload (OSMWay_ids_map);
    street_segment_by_street_type_preload(street_segment_types_map, OSMWay_ids_map);
    street_street_segments_preload(street_street_segments, highest_speed_limit);
    street_intersections_preload(street_intersections);
    street_segment_length_preload(street_segment_length);
    street_length_preload(street_length);
    speed_limit_preload(speed_limit);
    street_ids_preload(find_street_ids_from_name_map);
    intersection_street_segments_preload(id_intersection_street_segments);
    adjacent_intersections_preload(id0_adjacent_intersections);
    cosLatAvgCI = cos_lat_avg_CI();
    cosLatAvgPOI = cos_lat_avg_POI();
    closest_point_of_interest = closest_point_of_interest_preload();
    closest_intersection = closest_intersection_preload();   
    poi_ids_preload(POI_id);
    draw_map();
    
    
    return true;
}

float fastest_speed(){
    return highest_speed_limit;
}

void close_map() {
    //Clean-up your map related data structures here
    //free any other data structures if we decide to use them
    POI_id.clear();
    street_segment_length.clear();
    street_length.clear();
    speed_limit.clear();
    id_intersection_street_segments.clear();
    id0_adjacent_intersections.clear();
    find_street_ids_from_name_map.clear();
    street_street_segments.clear();
    street_intersections.clear();
    closest_point_of_interest.clear();
    closest_intersection.clear();
    OSMWay_ids_map.clear();
    POItype_by_POI_id.clear();
    closeStreetDatabase();
    closeOSMDatabase();
}

//Returns OSMWay* given an OSMID
const OSMWay* find_way_from_OSMID (OSMID o){
    return OSMWay_ids_map[o];
}

//returns a vector of OSMWay* by the type of way it is
//categorized by freeway, major, regular, minor
vector<unsigned int> find_street_segment_street_by_type (string type){
    return street_segment_types_map [type];
}

//Returns street id(s) for the given street name
//If no street with this name exists, returns a 0-length vector.

vector<unsigned> find_street_ids_from_name(string street_name) {
    if (find_street_ids_from_name_map.find(street_name) == find_street_ids_from_name_map.end()) {
        vector <unsigned> empty;
        return empty;
    }
    return find_street_ids_from_name_map[street_name];
}

//Returns the street segments for the given intersection

vector<unsigned> find_intersection_street_segments(unsigned intersection_id) {

    return id_intersection_street_segments[intersection_id];

}

//Returns the street names at the given intersection (includes duplicate street names in returned vector)

vector<string> find_intersection_street_names(unsigned intersection_id) {
    vector<string> intersection_street_names;
    unsigned StreetSegmentCount, i;

    StreetSegmentCount = getIntersectionStreetSegmentCount(intersection_id);

    //loop through all the number of street segments and fetch street names for each of them
    for (i = 0; i < StreetSegmentCount; i++) {
        StreetSegmentIndex streetSegmentldx;
        streetSegmentldx = getIntersectionStreetSegment(intersection_id, i);
        StreetIndex streetldx = getStreetSegmentInfo(streetSegmentldx).streetID;
        string street_names = getStreetName(streetldx);
        intersection_street_names.push_back(street_names);
    }

    return intersection_street_names;
}

//Returns true if you can get from intersection1 to intersection2 using a single street segment (hint: check for 1-way streets too)
//corner case: an intersection is considered to be connected to itself

bool are_directly_connected(unsigned intersection_id1, unsigned intersection_id2) {
    unsigned StreetSegmentCount, i;
    //check when an intersection is connected to itself
    if (intersection_id1 == intersection_id2) return true;
    StreetSegmentCount = getIntersectionStreetSegmentCount(intersection_id1);

    //loop through all the street segments for the given intersections
    //check if the given two intersections are connected
    for (i = 0; i < StreetSegmentCount; i++) {

        StreetSegmentIndex streetSegmentldx;
        streetSegmentldx = getIntersectionStreetSegment(intersection_id1, i);
        StreetSegmentInfo StreetSegmentIn = getStreetSegmentInfo(streetSegmentldx);

        //check for one way and avoid duplications 
        if ((StreetSegmentIn.oneWay) && (intersection_id2 == StreetSegmentIn.to)) return true;
            //check for two-way street
        else if ((intersection_id2 == StreetSegmentIn.from) || (intersection_id2 == StreetSegmentIn.to)) return true;

    }
    return false;
}

//Returns all intersections reachable by traveling down one street segment
//from given intersection (hint: you can't travel the wrong way on a 1-way street)
//the returned vector should NOT contain duplicate intersections

vector<unsigned> find_adjacent_intersections(unsigned intersection_id) {
    return id0_adjacent_intersections[intersection_id];
}


//Returns all street segments for the given street

vector<unsigned> find_street_street_segments(unsigned street_id) {
    return street_street_segments[street_id];
}

//Returns all intersections along the a given street

vector<unsigned> find_all_street_intersections(unsigned street_id) {
    return street_intersections[street_id];
}

//Return all intersection ids for two intersecting streets
//This function will typically return one intersection id.

vector<unsigned> find_intersection_ids_from_street_names(string street_name1, string street_name2) {
    vector<unsigned> intersectionID;
    //the ids of each street name is stored
    vector<unsigned> street_name1_id = find_street_ids_from_name(street_name1);

    vector<unsigned> street_name2_id = find_street_ids_from_name(street_name2);
  
    
    //to contain all intersections of the first and second street
    vector<unsigned> street1_intersections;
    vector<unsigned> street2_intersections;

    vector<unsigned> temporary_intersections;
    //stores all the intersections of first street

    for (unsigned int x = 0; x < street_name1_id.size(); x++) {
        temporary_intersections = find_all_street_intersections(street_name1_id[x]);
        street1_intersections.insert(street1_intersections.end(), temporary_intersections.begin(), temporary_intersections.end());
    }

    //stores all the intersections of the second street
    for (unsigned int x = 0; x < street_name2_id.size(); x++) {
        temporary_intersections = find_all_street_intersections(street_name2_id[x]);
        street2_intersections.insert(street2_intersections.end(), temporary_intersections.begin(), temporary_intersections.end());

    }

    //checks if there are common intersections for both street names
    for (unsigned int x = 0; x < street1_intersections.size(); x++)
        if (find(street2_intersections.begin(), street2_intersections.end(), street1_intersections[x]) != street2_intersections.end())
            intersectionID.push_back(street1_intersections[x]);
    
    return intersectionID;
}

//Returns the distance between two coordinates in meters

double find_distance_between_two_points(LatLon point1, LatLon point2) {
    //Calculate the average latitude with given points.
    double lat_avg = DEG_TO_RAD * (point1.lat() + point2.lat()) / 2.0;

    //Convert into x,y coordinates.
    double x1 = DEG_TO_RAD * point1.lon() * cos(lat_avg);
    double x2 = DEG_TO_RAD * point2.lon() * cos(lat_avg);

    double y1 = DEG_TO_RAD * point1.lat();
    double y2 = DEG_TO_RAD * point2.lat();

    //Calculate the distance between two points with given function.
    double distance = EARTH_RADIUS_IN_METERS * sqrt((y2 - y1) * (y2 - y1) + (x2 - x1) * (x2 - x1));

    return distance;
}

//Returns the length of the given street segment in meters

double find_street_segment_length(unsigned street_segment_id) {
    return street_segment_length[street_segment_id];
}

//Returns the length of the specified street in meters

double find_street_length(unsigned street_id) {
    return street_length[street_id];
}

//Returns the travel time to drive a street segment in seconds (time = distance/speedLimit)

double find_street_segment_travel_time(unsigned street_segment_id) {
    double distance = find_street_segment_length(street_segment_id);
    double speedLimit = speed_limit[street_segment_id];

    //Calculate time with distance, speedLimit, and conversion factor(3.6).
    double time = distance / speedLimit * 3.6;
    return time;
}

//Returns the nearest point of interest to the given position

unsigned find_closest_point_of_interest(LatLon my_position) {
    vector<value> result_n;
    //Convert LatLon into Cartesian coordinates.
    double x1 = my_position.lon() * cosLatAvgPOI;
    double y1 = my_position.lat();
    //Use built in function nearest to find closest point of intersect.
    closest_point_of_interest.query(bgi::nearest(point(x1, y1), 30), back_inserter(result_n));
    
    unsigned closest_POI = result_n[0].second;
    LatLon closest_POI_position = getPointOfInterestPosition(closest_POI);
    double min_distance = find_distance_between_two_points(closest_POI_position, my_position);
    
    for (unsigned int i = 1; i < result_n.size(); i++) {
        LatLon temp_position = getPointOfInterestPosition(result_n[i].second);
        double temp_distance = find_distance_between_two_points(temp_position, my_position);
        if (temp_distance < min_distance) {
            closest_POI = result_n[i].second;
            min_distance = temp_distance;
        }
    }
    
    return closest_POI;
}
//Returns the the nearest intersection to the given position

unsigned find_closest_intersection(LatLon my_position) {
    vector<value> result_n;
    //Convert LatLon into Cartesian coordinates.
    double x1 = my_position.lon() * cosLatAvgCI;
    double y1 = my_position.lat();
    //Use built in function nearest to find closest intersection.
    closest_intersection.query(bgi::nearest(point(x1, y1), 30), back_inserter(result_n));

    unsigned closest_Intersection = result_n[0].second;
    LatLon closest_Intersection_position = getIntersectionPosition(closest_Intersection);
    double min_distance = find_distance_between_two_points(closest_Intersection_position, my_position);
    
    for (unsigned int i = 1; i < result_n.size(); i++) {
        LatLon temp_position = getIntersectionPosition(result_n[i].second);
        double temp_distance = find_distance_between_two_points(temp_position, my_position);
        if (temp_distance < min_distance) {
            closest_Intersection = result_n[i].second;
            min_distance = temp_distance;
        }
    }
    
    return closest_Intersection;
}


vector<unsigned> getPointOfInterestID(std::string pointOfInterestName) {
    return POI_id[pointOfInterestName];
}