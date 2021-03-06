/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   m2_M.h
 * Author: zhengfa7
 *
 * Created on February 16 */

#ifndef M2_M_H
#define M2_M_H
#pragma once
#include "m1.h"
#include <boost/algorithm/string.hpp> 
#include <string>
#include <thread>
#include <chrono>
#include <cfloat>
#include <iomanip>
#include <iostream>
#include <vector>
#include "LatLon.h"
#include "math.h"
#include "StreetsDatabaseAPI.h"
#include "graphics.h"
#include "OSMDatabaseAPI.h"

#define INIT_ZOOM 10000
#define TEXTBOX_WIDTH 30
#define TEXTBOX_LENGTH 400

using namespace std;

LatLon getAvgLatLon ();
LatLon getLatLon (t_point XYPos);

void drawStreetSegments();
void drawStreetNames();
//For arrow sign of one-way segments
double setRotation(t_point &from, t_point &to);
void draw_arrow(t_point &from, t_point &to);

void drawMinorStreetSegments();
void drawRegularStreetSegments();
void drawMajorStreetSegments();
void drawFreeWayStreetSegments();

t_point visibleWorldToVisibleScreen (t_point visible_world);


void drawMinorNames(); 
void drawRegularNames();
void drawMajorNames();
void drawFreeWayNames();
void drawingStreetNames(StreetSegmentInfo currStreetSegment, double width, string type);
void drawingStreetSegments(unsigned int x);

bool checkCleanNames(double x, double y, double width, string type);

void draw_highlighted_intersections();
void search_found();
void search_intersection();
void drawSearchBox();
void drawTextBox (float x_topleft, float y_topleft);

//void drawWaterBodies ();
//void drawRiver();
void drawIslands();
void drawGreenery();
void drawHospitals();     
void drawPolygon(unsigned featurePoints, unsigned featureId);
void drawPolygon(vector<LatLon> points);

void drawRiver();
void drawShoreline();
void drawPark() ;
void drawGolfcourse();
void drawBeach();
void drawLake();
void drawStream();

void drawBuildingsAndUnknown();
void drawOSMBuildings();
void clearAllVectors();

void destroyOSMdatabase();
void drawcircle(t_point location);
void drawHighlightedIntersectionAndPOI(LatLon position);
t_point getAbsoluteXYPos(LatLon position);
double getAbsoluteXYLength (t_point p1, t_point p2);
bool loadOSMdatabase(const string & osmBin_map);
//void drawPointofInterest();
void drawPointofInterest(unsigned POI_id, float rad, color_types color_1, color_types color_2);
vector<unsigned> get_POI_id_by_POItype(string type);
void drawPOIbyType(string type);
void POItype_by_POI_id_preload (unordered_map <string, vector<unsigned>> &POItype_by_POI_ids);
void drawPOIName (t_point POI_position, unsigned POI_id);
void drawSustenance (vector<unsigned> POI_id);
void drawEducation (vector<unsigned> POI_id);
void drawTransportation (vector<unsigned> POI_id);
void drawFinancial (vector<unsigned> POI_id);
void drawHealthcare (vector<unsigned> POI_id);

void act_on_POI (void((*drawscreen) (void)));
void search_POI_found();
void draw_highlighted_POI();

// Draws the map. You can assume your load_map (string map_name)
// function is called before this function in the unit tests.
// Your main () program should do the same.
void draw_map();


void draw_path ();
void search_intersection_to_intersection_exists (unsigned intersectionId1, unsigned intersectionId2);
void search_intersection_to_POI_exists (unsigned intersectionId1, string POI);
void search_intersection_to_POI();
void search_intersection_to_intersection();

void act_on_help_button_func(void((*drawscreen) (void)));

void act_on_path_instructions(void((*drawscreen) (void)));
void drawBox(t_bound_box &drawingArea);
void HelpOutput();    
void zoom_more_than_one_intersection (t_point center);
void drawToggleIntersectionClick();
void PathOutput( /*std::vector<std::string> &script*/ );

string to_string_with_precision(double value, int n);
//vector<string> write_script(const vector<unsigned>& path_vectoc_Segment, unsigned intersect_id_Segment_start);
//void write_path_tem(vector<unsigned> &pathTodraw, unsigned intersection
vector<string> write_script(const vector<unsigned>& path_vectoc_Segment, unsigned intersect_id_Segment_start);
void write_path_tem(vector<unsigned> &pathTodraw, unsigned intersection);
vector<double> Degreefrom_point_to_point(t_point  start_tpoint,t_point  intsec_tpoint,t_point  end_tpoint);

double get_angle(unsigned segment_begin, unsigned segment_end);
void act_on_terminal_out_button_func(void((*drawscreen)(void)));
#endif /* M2_M_H */

