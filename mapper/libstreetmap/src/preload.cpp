/*
 * Pre-load necessary database.
 *
 */

#include "preload.h"
//Global variables used for quick calculations in closest_point_of_interest
//and closest_intersections.
extern double cosLatAvgPOI;
extern double cosLatAvgCI;

// vector where key is the OSMid, and value will be the OSMWay pointer
void OSMWay_ids_preload (unordered_map <OSMID, const OSMWay*> &OSMWay_ids_map_load) {
    for (unsigned int x = 0; x< getNumberOfWays(); x++){
        const OSMWay* way = getWayByIndex(x);
        OSMWay_ids_map_load.insert(make_pair(way->id(), way));
    }
}



void street_segment_by_street_type_preload(unordered_map <string, vector<unsigned int>>  & OSMWay_street_types_map, unordered_map <OSMID, const OSMWay*> &OSMWay_ids_map_load){
    for (unsigned int x = 0; x< getNumberOfStreetSegments(); x++){
          StreetSegmentInfo currStreet = getStreetSegmentInfo(x);
          const OSMWay *e = OSMWay_ids_map_load [currStreet.wayOSMID];
          for (unsigned i = 0; i < getTagCount(e); i++) {
            string key, value;
            tie(key, value) = getTagPair(e, i);
       //     cout << key << endl;
            if (key == "highway") {
                if (value == "motorway") {
                    insert_into_OSMWay_street_types (OSMWay_street_types_map, "freeway", x);
                }
                else if (value == "trunk" || value == "primary" || value == "secondary") {
                    insert_into_OSMWay_street_types (OSMWay_street_types_map, "major", x);
                }
                else if (value == "unclassified" || value == "residential" || value == "service"){
                    insert_into_OSMWay_street_types (OSMWay_street_types_map, "minor", x);
                }
                else if (value == "motorway_link"){
                    insert_into_OSMWay_street_types (OSMWay_street_types_map, "link", x);
                }
                else{
                    insert_into_OSMWay_street_types (OSMWay_street_types_map, "regular", x);
                }
                break;
            }
        }
    }
}
// helper function for inserting into OSMWay_by_street_type_preload's map
void insert_into_OSMWay_street_types (unordered_map <string, vector<unsigned int>>  & OSMWay_street_types_map, string type, unsigned int x){
    
    if (OSMWay_street_types_map.find(type) == OSMWay_street_types_map.end()){
        vector <unsigned int> way;
        way.push_back(x);
        OSMWay_street_types_map.insert(make_pair(type,way));
    }
    else{
        OSMWay_street_types_map[type].push_back(x);
    }
}


void poi_ids_preload(unordered_map <string, vector<unsigned>> &POI_id_map){
    for (unsigned i = 0; i < getNumberOfPointsOfInterest(); i++) {
        string poiName = getPointOfInterestName(i);
        if (POI_id_map.find(poiName) == POI_id_map.end()) {
            vector<unsigned> id;
            id.push_back(i);
            POI_id_map.insert(make_pair(poiName, id));
        } else {
            POI_id_map[poiName].push_back(i);
        }
    }
}


//Associate street ids with street names
void street_ids_preload(unordered_map <string, vector<unsigned>> &street_ids_map) {
    string streetname;
    for (unsigned int x = 0; x < getNumberOfStreets(); x++) {
        streetname = getStreetName(x);
        if (street_ids_map.find(streetname) == street_ids_map.end()) {
            vector<unsigned> id;
            id.push_back(x);
            street_ids_map.insert(make_pair(streetname, id));
        } else {
            street_ids_map[streetname].push_back(x);
        }
    }
}


//Store all the street segments with their corresponding intersections
void intersection_street_segments_preload(vector <vector<unsigned> > & id0_intersection_street_segments) {
    unsigned i = 0, intersectionNum = getNumberOfIntersections();
    vector<unsigned>streetsegmentlds;

    //Loop through each given intersection
    for (i = 0; i < intersectionNum; i++) {
        unsigned intersection_street_segs = getIntersectionStreetSegmentCount(i);

        //Loop through all the street segments for its corresponding intersections
        for (unsigned j = 0; j < intersection_street_segs; j++) {
            StreetSegmentIndex streetSegmentldx;
            streetSegmentldx = getIntersectionStreetSegment(i, j);
            //store each street segments of its current intersection in a vector  
            streetsegmentlds.push_back(streetSegmentldx);
        }
        //Street segments indexed by the intersection id
        id0_intersection_street_segments.push_back(streetsegmentlds);
        //Clear the content of the vector that stores the current street segments 
        //Prepare to store information of the next intersection
        streetsegmentlds.clear();
    }
}

//Pre-load all the adjacent intersections
void adjacent_intersections_preload(vector <vector<unsigned> > & id0_adjacent_intersections) {
    unsigned i = 0, intersectionNum = getNumberOfIntersections();
    vector<unsigned> adjacent_intersections;
    IntersectionIndex intersection_idx;
    IntersectionIndex intersection_id;
    StreetSegmentIndex streetSegmentldx;

    for (i = 0; i < intersectionNum; i++) {
        //The number of street segments at a intersection
        intersection_id = i;
        unsigned intersection_street_segs = getIntersectionStreetSegmentCount(i);
        for (unsigned j = 0; j < intersection_street_segs; j++) {

            streetSegmentldx = getIntersectionStreetSegment(i, j);
            StreetSegmentInfo StreetSegmentIn = getStreetSegmentInfo(streetSegmentldx);
            //check for a one way street
            if (StreetSegmentIn.oneWay) {
                if (intersection_id == StreetSegmentIn.from)
                    intersection_idx = StreetSegmentIn.to;
                else continue;
            }//check for a two-way street
            else {
                if (intersection_id == StreetSegmentIn.from)
                    intersection_idx = StreetSegmentIn.to;
                else
                    intersection_idx = StreetSegmentIn.from;
            }
            //check for duplicate intersections
            if (std::find(adjacent_intersections.begin(), adjacent_intersections.end(), intersection_idx)
                    == adjacent_intersections.end())
                adjacent_intersections.push_back(intersection_idx);
        }
        //adjacent_intersections
        id0_adjacent_intersections.push_back(adjacent_intersections);
        adjacent_intersections.clear();
    }
} //  return id0_adjacent_intersections;



//Group all street segments belonging to each street
//also returns fastest street_segment
void street_street_segments_preload(vector<vector<unsigned>> &street_street_segments, float &highest_speed_limit) {
    highest_speed_limit = 0;
    street_street_segments.resize(getNumberOfStreets());
    for (unsigned int i = 0; i < getNumberOfStreetSegments(); i++) {
        street_street_segments[getStreetSegmentInfo(i).streetID].push_back(i);
        if (getStreetSegmentInfo(i).speedLimit > highest_speed_limit){
            highest_speed_limit = getStreetSegmentInfo(i).speedLimit;
        }
    }
}

//Pre-load all intersections on each street
void street_intersections_preload(vector<vector<unsigned>> &street_intersections) {
    street_intersections.resize(getNumberOfStreets());
    StreetIndex street_id;
    unsigned to, from;
    for (unsigned int i = 0; i < getNumberOfStreetSegments(); i++) {
        street_id = getStreetSegmentInfo(i).streetID;
        to = getStreetSegmentInfo(i).to;
        from = getStreetSegmentInfo(i).from;
        //if from is not found
        if (find(street_intersections[street_id].begin(), street_intersections[street_id].end(), from) == street_intersections[street_id].end())
            street_intersections[street_id].push_back(from);
        //if to is not found
        if (find(street_intersections[street_id].begin(), street_intersections[street_id].end(), to) == street_intersections[street_id].end())
            street_intersections[street_id].push_back(to);
    }
}

//Pre-load the street segment lengths
void street_segment_length_preload(vector <double> & street_segment_length) {
    //Pre-load the vector of curve points count.
    vector <unsigned> curve_point_count;
    curve_point_count_preload(curve_point_count);

    //Loop through all street segments and parse corresponding segment lengths into the vector.
    for (unsigned int i = 0; i < getNumberOfStreetSegments(); i++) {
        double length = 0.0;
        StreetSegmentInfo segment_info = getStreetSegmentInfo(i);
        IntersectionIndex from = segment_info.from;
        IntersectionIndex to = segment_info.to;
        unsigned int curvePointCount = curve_point_count[i];

        LatLon intersection_1 = getIntersectionPosition(from);
        LatLon intersection_2 = getIntersectionPosition(to);

        //Find length directly if there is no curve.
        if (curvePointCount == 0) {
            length = find_distance_between_two_points(intersection_1, intersection_2);
        } else {
            LatLon temp_1 = intersection_1;

            //Loop through all curved segments and add their lengths up.
            for (unsigned int x = 0; x < curvePointCount; x++) {
                LatLon temp_2 = getStreetSegmentCurvePoint(i, x);
                length += find_distance_between_two_points(temp_1, temp_2);
                temp_1 = temp_2;
            }
            //Add the last curved segment length between the last point and intersection_2.
            length += find_distance_between_two_points(temp_1, intersection_2);
        }
        street_segment_length.push_back(length);
    }
}

//Pre-load the street lengths
void street_length_preload(vector <double> & street_length) {

    //Loop through all street IDs and parse corresponding street lengths into the vector.
    for (unsigned int street_id = 0; street_id < getNumberOfStreets(); street_id++) {
        double length = 0.0;
        vector <unsigned> segment_id = find_street_street_segments(street_id);
        //Sum up the length of all segments on the given street.
        for (auto iter = segment_id.begin(); iter != segment_id.end(); iter++) {
            length += find_street_segment_length(*iter);
        }
        street_length.push_back(length);
    }
}

//Pre-load speed limit
void speed_limit_preload(vector <double> & speed_limit) {

    //Parse all speed limits into a vector sorted by segment IDs.
    for (unsigned int segment_id = 0; segment_id < getNumberOfStreetSegments(); segment_id++) {
        speed_limit.push_back(getStreetSegmentInfo(segment_id).speedLimit);
    }
}

//Pre-load the number of curve points
void curve_point_count_preload(vector <unsigned> & curve_point) {
    //Parse all numbers of curve points into a vector sorted by segment IDs.
    for (unsigned int segment_id = 0; segment_id < getNumberOfStreetSegments(); segment_id++) {
        curve_point.push_back(getStreetSegmentInfo(segment_id).curvePointCount);
    }

}

//Quickly find the cos_lat_avg of all intersections.
double cos_lat_avg_CI() {
    double sum = 0;
    for (unsigned int i = 0; i < getNumberOfIntersections(); i++) {
        sum += getIntersectionPosition(i).lat();
    }
    double lat_avg = DEG_TO_RAD * sum / getNumberOfIntersections();
    //Taylor series
    double cosLatAvg = 1 - lat_avg * lat_avg / 2 + lat_avg * lat_avg * lat_avg * lat_avg / 4 - lat_avg * lat_avg * lat_avg * lat_avg * lat_avg * lat_avg / 6;

    return cosLatAvg;
}

//Quickly find the cos_lat_avg of all points of interest.
double cos_lat_avg_POI() {
    double sum = 0;
    for (unsigned int i = 0; i < getNumberOfPointsOfInterest(); i++) {
        sum += getPointOfInterestPosition(i).lat();
    }
    double lat_avg = DEG_TO_RAD * sum / getNumberOfPointsOfInterest();
    //Taylor series
    double cosLatAvg = 1 - lat_avg * lat_avg / 2 + lat_avg * lat_avg * lat_avg * lat_avg / 4 - lat_avg * lat_avg * lat_avg * lat_avg * lat_avg * lat_avg / 6;

    return cosLatAvg;
}

//Build a r-tree to store closest_point_of_interest.
bgi::rtree<values, bgi::rstar<10> > closest_point_of_interest_preload() {
    unsigned int size = getNumberOfPointsOfInterest();

    //Build a r-tree with r* algorism.
    bgi::rtree<values, bgi::rstar<10> > rtree;
    for (unsigned i = 0; i < size; i++) {
        //Convert LatLon into Cartesian coordinates.
        double x1 = getPointOfInterestPosition(i).lon() * cosLatAvgPOI;
        double y1 = getPointOfInterestPosition(i).lat();

        double x2 = getPointOfInterestPosition(i + 0.5f).lon() * cosLatAvgPOI;
        double y2 = getPointOfInterestPosition(i + 0.5f).lat();
        // create a box
        box b(point(x1, y1), point(x2, y2));
        // insert new value
        rtree.insert(make_pair(b, i));
    }
    return rtree;
}

//Build a r-tree to store closest_intersections.
bgi::rtree<values, bgi::rstar<10> > closest_intersection_preload() {
    unsigned int size = getNumberOfIntersections();

    //Build a r-tree with r* algorism.
    bgi::rtree<values, bgi::rstar<10> > rtree;
    for (unsigned i = 0; i < size; i++) {
        //Convert LatLon into Cartesian coordinates.
        double x1 = getIntersectionPosition(i).lon() * cosLatAvgCI;
        double y1 = getIntersectionPosition(i).lat();

        double x2 = getIntersectionPosition(i + 0.5f).lon() * cosLatAvgCI;
        double y2 = getIntersectionPosition(i + 0.5f).lat();
        // create a box
        box b(point(x1, y1), point(x2, y2));
        // insert new value
        rtree.insert(make_pair(b, i));
    }
    return rtree;
}
