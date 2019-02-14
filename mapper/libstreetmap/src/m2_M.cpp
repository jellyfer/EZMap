//if you want functions for get_visible_screen
//auto a = get_visible_screen().

//draw text function
//set to false if you want to draw from top left
//if you want to draw from middle, ignore it (or set to true)
//drawtext(get_visible_screen().right() - 40 - TEXTBOX_LENGTH + 10, 20 + 10, search_text, FLT_MAX, FLT_MAX, false);



/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "m1.h"
#include "m1_additional.h"
#include "m2_M.h"
#include "m3.h"
#include <string>
#include <cmath>
#include <X11/Xlib.h>
#include <chrono>
#include "graphics.h"
#include "graphics.cpp"
#include <unordered_map>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <iomanip>
#include <algorithm>
#include <sstream>

using namespace std;
using namespace boost;

//extern void act_on_new_button_func(void (*drawscreen_ptr) (void));
void act_on_button_press(float x, float y, t_event_buttonPressed event);
void act_on_mouse_move(float x, float y);
void act_on_key_press(char c, int keysym);
void drawscreen(void);
extern double cos_lat_avg_POI();
bool isOSMDoneLoading = false;
bool showArrow = false;
vector <t_point> clean_names;
bool hit_search;
string typed_text = "";
int zoom_level = 0;



std::vector<unsigned> drawPoifromname(string poiname);
t_bound_box initial_coords;

unordered_map <string, vector<unsigned>> POItype_by_POI_id;

Surface demo = load_png_from_file("libstreetmap/resources/arrow_left.png");
Surface bank = load_png_from_file("libstreetmap/resources/small_image.png");
Surface hospital = load_png_from_file("libstreetmap/resources/arrow_inout.png");
Surface  pleft = load_png_from_file("libstreetmap/resources/arrow_left.png");
Surface  pright = load_png_from_file("libstreetmap/resources/arrow_right.png");
Surface  pup = load_png_from_file("libstreetmap/resources/arrow_up.png");
//Surface 


std::vector<std::vector<LatLon>> OSMBuildings;
std::vector<unsigned>POIIDS;
vector<unsigned> found_POI_id;

double greatestLattitude, smallestLattitude;
double greatestLongitude, smallestLongitude;



vector <unsigned> intersections;
bool show_highlighted_intersections = false;
bool show_highlighted_POI = false;
bool toggle = false;

//true if storing first intersection for path with mouse clicking
//false if storing second intersection
bool first_inter = true;
unsigned first_intersection;
unsigned second_intersection;



//global vars stored for drawing & directions
vector<unsigned> path;
unsigned begin_intersection;
unsigned end_intersection;
bool POI_flag;
string POI_name_global;

bool show_path = false;

#define ZOOM_LEVEL 0.0001
#define TURN_PENALTY 15

//for help button
bool HELPSHOWED = false;

std::vector<std::string> pathScript;

//Calculate the window size based on latitude and longitude values

LatLon getAvgLatLon() {
    LatLon current;
    double latSum = 0;
    double lonSum = 0;
    double lat, lon;

    //Initialize the first position point to greatest lat and Lon values
    greatestLattitude = smallestLattitude = getIntersectionPosition(0).lat();
    greatestLongitude = smallestLongitude = getIntersectionPosition(0).lon();

    //Loop through all the interested points and make comparison between the current points and greatest Lat&Lon values
    for (unsigned i = 0; i < getNumberOfIntersections(); i++) {
        current = getIntersectionPosition(i);
        latSum += current.lat();
        lonSum += current.lon();
        if (current.lat() > greatestLattitude) greatestLattitude = current.lat();
        if (current.lon() > greatestLongitude) greatestLongitude = current.lon();
        if (current.lat() < smallestLattitude) smallestLattitude = current.lat();
        if (current.lon() < smallestLongitude) smallestLongitude = current.lon();
    }

    //Take the average of all the interest points of the map display 
    lat = latSum / getNumberOfIntersections();
    lon = -lonSum / getNumberOfIntersections();
    LatLon avgLatLon(lat, lon);

    //Decide for the actual size of the map display
    LatLon positionRightTop(greatestLattitude, greatestLongitude);
    LatLon positionLeftBottom(smallestLattitude, smallestLongitude);

    //Set the initial coordinates of the map
    initial_coords = t_bound_box(getAbsoluteXYPos(positionLeftBottom).x,
            getAbsoluteXYPos(positionLeftBottom).y,
            getAbsoluteXYPos(positionRightTop).x,
            getAbsoluteXYPos(positionRightTop).y);
    return avgLatLon;

}

void draw_map() {

    /**************** initialize display **********************/

    // Set-up coordinates. The coordinates passed in define what coordinate
    // limits you want to use; this coordinate system is mapped to fill 
    // the screen.
    getAvgLatLon();
    init_graphics("Maps Graphics", t_color(234, 234, 234));

    set_visible_world(initial_coords);
    set_drawing_buffer(OFF_SCREEN);
    POItype_by_POI_id_preload(POItype_by_POI_id);

    update_message("");

    // create_button("Window", "0 Clicks", act_on_new_button_func); // name is UTF-8 
    create_button("Window", "HELP", act_on_help_button_func);
    create_button("Window", "POI", act_on_POI);
    // Enable mouse movement (not just button presses) and key board input.
    // The appropriate callbacks will be called by event_loop.

    //Additional buttons

    create_button("Window", "Directions", act_on_path_instructions);
    
    
    //Uncomment followings will have the "path" button shown up and display the pop-up window for testing a specific path 
    create_button("Window", "Path", act_on_terminal_out_button_func);


    set_mouse_move_input(true);
    drawscreen();
    //  set_coordinate_system(GL_SCREEN);
    event_loop(act_on_button_press, NULL, act_on_key_press, drawscreen);
    // event_loop(act_on_mousebutton, NULL, act_on_keypress, drawscreen);            
    close_graphics();
    std::cout << "Graphics closed down.\n";
}

//Draws the features included in the map

void drawscreen(void) {
    set_drawing_buffer(OFF_SCREEN);
    set_coordinate_system(GL_WORLD);
    set_draw_mode(DRAW_NORMAL);
    clearscreen();
    
    drawShoreline();
    drawPark() ;         
    drawLake();
    drawIslands();
    drawBeach();
    drawGreenery();
    drawGolfcourse();
    drawRiver();
    drawStream();
    showArrow = false;


    //closest
    if (get_visible_world().area() < 5.1e-8) {
        zoom_level = 7;
        drawBuildingsAndUnknown(); // Draws buildings and unknown features
        showArrow = true;
        
        setlinewidth(3);
        drawMinorStreetSegments();
        setfontsize(6);
        drawMinorNames();
        
        setlinewidth(4);
        drawRegularStreetSegments();
        setfontsize(7);
        drawRegularNames();
        setlinewidth(4);
        drawMajorStreetSegments();
        setfontsize(7);
        drawMajorNames();
        
        setlinewidth(5);
        drawFreeWayStreetSegments();
        setfontsize(9);
        drawFreeWayNames();

        drawPOIbyType("bank");
    } else if (get_visible_world().area() < 2e-7) {
        zoom_level = 6;
        drawBuildingsAndUnknown(); // Draws buildings and unknown features
        setlinewidth(3);
        drawMinorStreetSegments();
        setlinewidth(4);
        drawRegularStreetSegments();
        setfontsize(7);
        drawRegularNames();
        setlinewidth(4);
        drawMajorStreetSegments();
        setfontsize(7);
        drawMajorNames();
        setlinewidth(5);
        drawFreeWayStreetSegments();
        setfontsize(9);
        drawFreeWayNames();

        drawPOIbyType("bank");
        drawPOIbyType("hospital");
        drawPOIbyType("college");
        drawPOIbyType("university");
        //drawPOIbyType("restaurant");

    } else if (get_visible_world().area() < 5.2e-7) {
        zoom_level = 5;
        drawBuildingsAndUnknown(); // Draws buildings and unknown features
        setlinewidth(3);
        drawMinorStreetSegments();
        setlinewidth(4);
        drawRegularStreetSegments();
        setlinewidth(4);
        drawMajorStreetSegments();
        setfontsize(7);
        drawMajorNames();

        drawPOIbyType("hospital");
        drawPOIbyType("university");
        setlinewidth(5);
        drawFreeWayStreetSegments();
        setfontsize(8);
        drawFreeWayNames();


    } else if (get_visible_world().area() < 1.4e-6) {
        zoom_level = 4;
        drawBuildingsAndUnknown(); // Draws buildings and unknown features
        
        setlinewidth(4);
        drawRegularStreetSegments();
        
        setlinewidth(4);
        drawMajorStreetSegments();
        setfontsize(7);
        drawMajorNames();

        drawPOIbyType("hospital");
       drawPOIbyType("university");
        
        setlinewidth(5);
        drawFreeWayStreetSegments();
        setfontsize(7);
        drawFreeWayNames();

    } else if (get_visible_world().area() < 4.1e-6) {
        zoom_level = 3;
        drawBuildingsAndUnknown(); // Draws buildings and unknown features
        
        setlinewidth(3);
        drawRegularStreetSegments();
        
        setlinewidth(3);
        drawMajorStreetSegments();
        setfontsize(7);
        drawMajorNames();
        
        setlinewidth(5);
        drawFreeWayStreetSegments();
        setfontsize(8);
        drawFreeWayNames();


    } else if (get_visible_world().area() < 1.1e-5) {
        zoom_level = 2;

        drawBuildingsAndUnknown(); // Draws buildings and unknown features
        
        setlinewidth(2);
        drawMajorStreetSegments();
        setfontsize(7);
        drawMajorNames();
        
        setlinewidth(4);
        drawFreeWayStreetSegments();
        setfontsize(7);
        drawFreeWayNames();


    } else if (get_visible_world().area() < 3.2e-5) {
        zoom_level = 1;
        drawBuildingsAndUnknown(); // Draws buildings and unknown features
        setlinewidth(2);
        drawMajorStreetSegments();
        setlinewidth(4);
        drawFreeWayStreetSegments();
        setfontsize(7);
        drawFreeWayNames();

    }//furthest
    else {
        zoom_level = 0;
        setlinewidth(4);
        drawFreeWayStreetSegments();

    }
    draw_highlighted_POI();
    draw_path();
    draw_highlighted_intersections();
    drawSearchBox();
    drawToggleIntersectionClick();
    copy_off_screen_buffer_to_screen();
    set_coordinate_system(GL_SCREEN);
}

void act_on_button_press(float x, float y, t_event_buttonPressed) {
    POI_flag = false;
    intersections.clear();
    show_path = false;
    show_highlighted_intersections = false;
    drawscreen();
    t_point world;
    world.x = x;
    world.y = y;
    t_point screen = visibleWorldToVisibleScreen(world);
    //hit the search bar
    if (screen.x > get_visible_screen().right() - 40 - TEXTBOX_LENGTH && screen.x < get_visible_screen().right() - 40 && screen.y > 20 && screen.y < 20 + TEXTBOX_WIDTH) {
        hit_search = true;
        set_keypress_input(true);
        set_drawing_buffer(ON_SCREEN);
        if (toggle){
            toggle = false;
            update_message("No longer finding path by clicking.");
        }
        drawSearchBox();
        drawToggleIntersectionClick();
    }//check if toggle mouse input for intersection path finding
    else if (screen.x > get_visible_screen().right() - 40 - TEXTBOX_LENGTH && screen.x < get_visible_screen().right() - 40 && screen.y > 60 && screen.y < 60 + TEXTBOX_WIDTH) {
        hit_search = false;
        set_keypress_input(false);
        typed_text = "";
        if (toggle) {
            update_message("No longer finding path by clicking.");
            toggle = false;
        } else {
            update_message("Finding path by clicking. Click on first intersection.");
            toggle = true;
        }
        set_drawing_buffer(ON_SCREEN);
        drawSearchBox();
        drawToggleIntersectionClick();
    }//check if hit intersection
    else {
        hit_search = false;
        set_keypress_input(false);
        typed_text = "";
        set_drawing_buffer(ON_SCREEN);
        drawSearchBox();
        t_point clicked;
        clicked.x = x;
        clicked.y = y;
        unsigned closest_intersection = find_closest_intersection(getLatLon(clicked));
        float length = getAbsoluteXYLength((getAbsoluteXYPos(getIntersectionPosition(closest_intersection))), clicked);

        //if distance from intersection to clicked is close enough
        if (length < 5e-06) {
            //if in find path by clicking mode
            if (toggle) {
                //if clicked the first intersection (from)
                if (first_inter) {
                    first_inter = false;
                    first_intersection = closest_intersection;
                    string msg = "First intersection: " + getIntersectionName(closest_intersection) + ". Click on second intersection.";
                    update_message(msg);
                }//if clicked the second intersection (to)
                else {
                    first_inter = true;
                    second_intersection = closest_intersection;
                    search_intersection_to_intersection_exists(first_intersection, second_intersection);
                }
            } else {
                intersections.push_back(closest_intersection);
                search_found();
            }
        } else {
            //intersection not found & is not in toggle mode, should update message to ""
            if (!toggle) {
                update_message("");
            }
        }
    }
    set_drawing_buffer(OFF_SCREEN);
}

//pans & zooms to searched intersection, including all intersections

void search_found() {
    show_highlighted_intersections = true;
    set_drawing_buffer(ON_SCREEN);
    draw_highlighted_intersections();
    set_drawing_buffer(OFF_SCREEN);
    t_point center;
    center.x = 0;
    center.y = 0;
    t_point furthest;
    for (unsigned x = 0; x < intersections.size(); x++) {
        center += getAbsoluteXYPos(getIntersectionPosition(intersections[x]));
    }

    center.x = center.x / intersections.size();
    center.y = center.y / intersections.size();
    if (zoom_level < 7) {
        if (intersections.size() == 1) {

            set_visible_world(center.x - ZOOM_LEVEL, center.y - ZOOM_LEVEL, center.x + ZOOM_LEVEL, center.y + ZOOM_LEVEL, false);
        } else {
            zoom_more_than_one_intersection(center);
        }
    }
    drawscreen();

}

void zoom_more_than_one_intersection(t_point center) {
    t_point furthest = center;
    for (unsigned x = 0; x < intersections.size(); x++) {
        if (getAbsoluteXYLength(center, furthest) < getAbsoluteXYLength(center, getAbsoluteXYPos(getIntersectionPosition(intersections[x])))) {
            furthest = getAbsoluteXYPos(getIntersectionPosition(intersections[x]));
        }
    }

    furthest.x = abs(center.x - furthest.x);
    furthest.y = abs(center.y - furthest.y);
    if (furthest.x > ZOOM_LEVEL || furthest.y > ZOOM_LEVEL)
        set_visible_world(center.x - furthest.x - ZOOM_LEVEL / 10, center.y - furthest.y - ZOOM_LEVEL / 10, center.x + furthest.x + ZOOM_LEVEL / 10, center.y + furthest.y + ZOOM_LEVEL / 10, false);
    else
        set_visible_world(center.x - ZOOM_LEVEL, center.y - ZOOM_LEVEL, center.x + ZOOM_LEVEL, center.y + ZOOM_LEVEL, false);
}


//parses user input & sees if intersection exists

void search_intersection() {
    intersections.clear(); //global variable which i am storing the every intersection matching the name of the intersection
    //it is being cleared because this function is triggered if the user presses enter
    //it wants to flush the past intersection which was searched for
    //this global variable is later used to figure out where to draw the red circle
    settextrotation(0); //make sure we are writing horizontally
    auto ampersand = typed_text.find("&"); //we know each intersection has the &
    if (ampersand == string::npos) { //if it isn't found then it is invalid format, string::npos is a constant which is returned when not found

        update_message("Invalid format, please include & between street names, ex. Eglinton Avenue East & Yonge Street");
    } else {
        string street1 = typed_text.substr(0, ampersand); //substring works like [) including first, and excluding last
        string street2 = typed_text.substr(ampersand + 2);
        trim(street1); //trim gets rid of spaces
        trim(street2);

        intersections = find_intersection_ids_from_street_names(street1, street2);
        if (intersections.size() == 0) {

            update_message("Intersection does not exist."); //message on bottom of screen
        } else {

            search_found();

        }
    }
}

void search_intersection_to_intersection() {
    settextrotation(0);
    auto to = typed_text.find(" to ");
    if (to == string::npos) {
        update_message("Invalid format, please include keyword ' to ' in your query, ex. Eglinton Avenue East & Yonge Street to Queen's Park & College Street");
    } else {
        string intersection1 = typed_text.substr(0, to);
        string intersection2 = typed_text.substr(to + 4);
        auto ampersand1 = intersection1.find("&");
        auto ampersand2 = intersection2.find("&");
        if (ampersand1 == string::npos || ampersand2 == string::npos) {
            update_message("Invalid format, please include & between street names, ex. Eglinton Avenue East & Yonge Street");
        } else {
            //substr of the ampersand
            string intersection1_street1 = intersection1.substr(0, ampersand1);
            string intersection1_street2 = intersection1.substr(ampersand1 + 2);
            trim(intersection1_street1);
            trim(intersection1_street2);
            //see if intersection ids exist
            vector <unsigned> intersections1 = find_intersection_ids_from_street_names(intersection1_street1, intersection1_street2);



            string intersection2_street1 = intersection2.substr(0, ampersand2);
            string intersection2_street2 = intersection2.substr(ampersand2 + 2);
            trim(intersection2_street1);
            trim(intersection2_street2);
            vector <unsigned> intersections2 = find_intersection_ids_from_street_names(intersection2_street1, intersection2_street2);
            if (intersections1.size() == 0) {
                string msg = intersection1_street1 + " & " + intersection1_street2 + " does not exist.";
                update_message(msg);
            } else if (intersections2.size() == 0) {
                string msg = intersection2_street1 + " & " + intersection2_street2 + " does not exist.";
                update_message(msg);
            } else {
                search_intersection_to_intersection_exists(intersections1[0], intersections2[0]);
            }

        }
    }

}

void search_intersection_to_POI() {
    settextrotation(0);
    auto to = typed_text.find(" to ");
    if (to == string::npos) {
        update_message("Invalid format, please include keyword ' to ' in your query, ex. Eglinton Avenue East & Yonge Street to CIBC");
    } else {
        string intersection1 = typed_text.substr(0, to);
        string POI_name = typed_text.substr(to + 4);
        auto ampersand = intersection1.find("&");
        trim(POI_name);
        vector <unsigned> POIs = getPointOfInterestID(POI_name);
        if (ampersand == string::npos) {
            update_message("Invalid format, please include & between street names, ex. Eglinton Avenue East & Yonge Street");
        } else if (POIs.size() == 0) {
            string msg = "The point of interest, " + POI_name + " does not exist on this map";
            update_message(msg);
        } else {
            string intersection1_street1 = intersection1.substr(0, ampersand);
            string intersection1_street2 = intersection1.substr(ampersand + 2);
            trim(intersection1_street1);
            vector <unsigned> intersections1 = find_intersection_ids_from_street_names(intersection1_street1, intersection1_street2);
            if (intersections1.size() == 0) {
                string msg = intersection1_street1 + " & " + intersection1_street2 + " does not exist.";
                update_message(msg);
            } else {
                update_message("Path Found");
                search_intersection_to_POI_exists(intersections1[0], POI_name);
            }

        }
    }


    //similar to intersection_to_intersection, but will need to check if POI name exists (jerry should have made a function for this)
    //call the search_intersection_to_POI_exists
}

void search_intersection_to_intersection_exists(unsigned intersectionId1, unsigned intersectionId2) {
    string msg = "Path found from " + getIntersectionName(intersectionId1) + " to " + getIntersectionName(intersectionId2);
    update_message(msg);
    show_path = true;
    vector <unsigned> street_segments_in_path = find_path_between_intersections(intersectionId1, intersectionId2, TURN_PENALTY);
    path = street_segments_in_path;
    begin_intersection = intersectionId1;
    end_intersection = intersectionId2;
    draw_path();
    //call find_path_between_intersections, save the vector & put it in draw path, with the start & end as well as any term penalty you want which will return a vector of street segment ids
    //call draw_path

}

void search_intersection_to_POI_exists(unsigned intersectionId1, string POI) {
    POI_flag = true;
    POI_name_global = POI;
    string msg = "Path found from " + getIntersectionName(intersectionId1) + " to nearest " + POI;
    update_message(msg);
    show_path = true;
    vector <unsigned> street_segments_in_path = find_path_to_point_of_interest(intersectionId1, POI, TURN_PENALTY);
    path = street_segments_in_path;
    draw_path();


}

void draw_path() {
    if (show_path) {
        //same as highway
        int width = 2 * sqrt(1 / (get_visible_world().get_width()*100)) + 2;
        setcolor(RED);
        setlinewidth(width);
        for (unsigned int x = 0; x < path.size(); x++) {
            drawingStreetSegments(path[x]);
        }
    }
}

//reads in user input until enter, allows backspace

void act_on_key_press(char c, int keysym) {
    if (keysym == XK_Return) { //user hit enter
        set_keypress_input(false); //no longer listens to keyboard input
        if (typed_text.substr(0, 5) == "PATH:") {
            typed_text = typed_text.substr(5);
            search_intersection_to_intersection();
        } else if (typed_text.substr(0, 4) == "POI:") {
            typed_text = typed_text.substr(4);
            search_intersection_to_POI(); //tells program to search for the intersection
        } else if (typed_text.substr(0, 7) == "SEARCH:") {
            typed_text = typed_text.substr(7);
            search_intersection();
        } else {
            update_message("Invalid Command. Available commands are SEARCH: [intersection], PATH: [intersection] to [intersection], POI: [intersection] to [name of POI], where intersection represents [street] & [street]");
        }

        typed_text = ""; //sets the typed text back to nothing
        hit_search = false; //lets program know that user clicked search bar
    } else if (keysym == XK_BackSpace && typed_text.length() > 0) { //user has hit backspace & there is already something typed
        typed_text = typed_text.substr(0, typed_text.length() - 1); //removes last character
    } else {
        if ((c >= 32 && c <= 126)) { //limits ascii
            typed_text = typed_text + char(c);
        }
    }
    set_drawing_buffer(ON_SCREEN); //changes buffer to on_screen 
    drawSearchBox(); //search box is now being drawn immediately on screen
}

//draws circles on intersections & prints intersection information on the bottom

void draw_highlighted_intersections() {
    if (show_highlighted_intersections) {

        string bottom_text = "";
        setcolor(RED);
        setlinewidth(3);
        for (unsigned x = 0; x < intersections.size(); x++) {

            drawarc(getAbsoluteXYPos(getIntersectionPosition(intersections[x])).x, getAbsoluteXYPos(getIntersectionPosition(intersections[x])).y, 0.000004, 0, 360);

            bottom_text = bottom_text + "   " + "Intersection Name: " + getIntersectionName(intersections[x]) + "   "
                    + "Lat: " + to_string(getIntersectionPosition(intersections[x]).lat()) + "   " + "Lon: " + to_string(getIntersectionPosition(intersections[x]).lon());
            update_message(bottom_text);
        }

    }
}


// Converts a given LatLon into a xy position to draw on the screen (world coords)

t_point getAbsoluteXYPos(LatLon position) {

    t_point convXY;
    float latAvg = DEG_TO_RAD * (greatestLattitude + smallestLattitude) / 2;
    convXY.x = (DEG_TO_RAD * position.lon() * cos(latAvg));
    convXY.y = DEG_TO_RAD * position.lat();
    return convXY;
}

//converts XYPos (world) back to LatLon

LatLon getLatLon(t_point XYPos) {
    float latAvg = DEG_TO_RAD * (greatestLattitude + smallestLattitude) / 2;
    LatLon pos(XYPos.y / DEG_TO_RAD, XYPos.x / (DEG_TO_RAD * cos(latAvg)));
    return pos;

}

//converts world coords to screen coords

t_point visibleWorldToVisibleScreen(t_point visible_world) {
    float y_screen_over_world = get_visible_screen().get_height() / get_visible_world().get_height();
    float x_screen_over_world = get_visible_screen().get_width() / get_visible_world().get_width();

    //find offset
    float top_world_to_screen = get_visible_world(). top() * y_screen_over_world;
    float left_world_to_screen = get_visible_world(). left() * x_screen_over_world;

    float y_offset = get_visible_screen().top() - top_world_to_screen;
    float x_offset = get_visible_screen().left() - left_world_to_screen;
    //t_point offset = get_visible_screen().top_right() - get_visible_world().top_right();
    t_point on_screen;
    on_screen.x = visible_world.x * x_screen_over_world + x_offset;
    on_screen.y = visible_world.y * y_screen_over_world + y_offset;
    return on_screen;

}

//get length between any two points

double getAbsoluteXYLength(t_point p1, t_point p2) {
    return sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
}


//draws the search box with initial text & user input

void drawSearchBox() {
    drawTextBox(get_visible_screen().right() - 40 - TEXTBOX_LENGTH, 20);
    set_coordinate_system(GL_SCREEN); //coordinate system is set to screen
    setfontsize(10);
    settextrotation(0);
    if (!hit_search) {
        setcolor(LIGHTGREY);
        string search_text = "Use SEARCH: , PATH: , or POI: commands";
        drawtext(get_visible_screen().right() - 40 - TEXTBOX_LENGTH + 10, 20 + 10, search_text, FLT_MAX, FLT_MAX, false);
    } else {
        setcolor(BLACK);
        if (typed_text.length() < 48)
            drawtext(get_visible_screen().right() - 40 - TEXTBOX_LENGTH + 10, 20 + 10, typed_text, FLT_MAX, FLT_MAX, false);
        else
            drawtext(get_visible_screen().right() - 40 - TEXTBOX_LENGTH + 10, 20 + 10, typed_text.substr(typed_text.length() - 47), FLT_MAX, FLT_MAX, false);
    }
    set_coordinate_system(GL_WORLD);
}


//draws box for toggle street intersection clicking

void drawToggleIntersectionClick() {
    set_coordinate_system(GL_SCREEN);
    string toggle_text;
    if (toggle) {
        setcolor(LIGHTGREY);
        toggle_text = "Toggle Mouse Clicking Find Path Mode: ON";
    } else {
        setcolor(DARKGREY);
        toggle_text = "Toggle Mouse Clicking Find Path Mode: OFF";
    }
    fillrect(get_visible_screen().right() - 40 - TEXTBOX_LENGTH, 60, get_visible_screen().right() - 40 - TEXTBOX_LENGTH + TEXTBOX_LENGTH, 60 + TEXTBOX_WIDTH);
    setcolor(BLACK);
    //drawtext(get_visible_screen().right() - 40 - TEXTBOX_LENGTH + 10, 60 + 10, toggle_text, FLT_MAX, FLT_MAX);
    drawtext(get_visible_screen().right() - 40 - TEXTBOX_LENGTH / 2, 60 + TEXTBOX_WIDTH / 2, toggle_text, FLT_MAX, FLT_MAX);
    set_coordinate_system(GL_WORLD);
}


//draws the background for textbox

void drawTextBox(float x_topleft, float y_topleft) {
    set_coordinate_system(GL_SCREEN);
    setcolor(WHITE);
    fillrect(x_topleft, y_topleft, x_topleft + TEXTBOX_LENGTH, y_topleft + TEXTBOX_WIDTH);
    setcolor(DARKGREY);
    setlinewidth(3);
    drawrect(x_topleft, y_topleft, x_topleft + TEXTBOX_LENGTH, y_topleft + TEXTBOX_WIDTH);
    set_coordinate_system(GL_WORLD);
}

//draws minor streets, show up only if a width is large enough

void drawMinorStreetSegments() {
        setcolor(WHITE);
        vector<unsigned int> minor = find_street_segment_street_by_type("minor");
        for (unsigned int x = 0; x < minor.size(); x++) {
            drawingStreetSegments(minor[x]);
        }
}

//draws regular streets, show up only if a width is large enough

void drawRegularStreetSegments() {
        setcolor(WHITE);
        
        vector<unsigned int> regular = find_street_segment_street_by_type("regular");
        for (unsigned int x = 0; x < regular.size(); x++) {
            drawingStreetSegments(regular[x]);
        }
        setcolor(t_color(254, 216, 157));
        vector<unsigned int> link = find_street_segment_street_by_type("link");
        for (unsigned int x = 0; x < link.size(); x++) {
            drawingStreetSegments(link[x]);
        }
}

//draws major streets, show up only if a width is large enough

void drawMajorStreetSegments() {
        setcolor(WHITE);
        vector<unsigned int> major = find_street_segment_street_by_type("major");
        for (unsigned int x = 0; x < major.size(); x++) {
            drawingStreetSegments(major[x]);
        }
}

//draws highways

void drawFreeWayStreetSegments() {
    setcolor(t_color(254, 216, 157));
    vector<unsigned int> freeways = find_street_segment_street_by_type("freeway");
    for (unsigned int x = 0; x < freeways.size(); x++) {
        drawingStreetSegments(freeways[x]);
    }
}

//helper function to draw street segments

void drawingStreetSegments(unsigned int x) {
    StreetSegmentInfo currStreetSegment = getStreetSegmentInfo(x);
    t_point from = getAbsoluteXYPos(getIntersectionPosition(currStreetSegment.from));
    t_point to = getAbsoluteXYPos(getIntersectionPosition(currStreetSegment.to));
    if (currStreetSegment.curvePointCount == 0) {
        drawline(from, to);
        if (currStreetSegment.oneWay && showArrow) {
            draw_arrow(from, to);
        }
    } else {
        drawline(getAbsoluteXYPos(getIntersectionPosition(currStreetSegment.from)), getAbsoluteXYPos(getStreetSegmentCurvePoint(x, 0)));
        for (unsigned y = 0; y < currStreetSegment.curvePointCount - 1; y++) {
            drawline(getAbsoluteXYPos(getStreetSegmentCurvePoint(x, y)), getAbsoluteXYPos(getStreetSegmentCurvePoint(x, y + 1)));
        }
        drawline(getAbsoluteXYPos(getIntersectionPosition(currStreetSegment.to)), getAbsoluteXYPos(getStreetSegmentCurvePoint(x, currStreetSegment.curvePointCount - 1)));
    }
}

//arrow for one way streets

void draw_arrow(t_point &from, t_point &to) {
    t_color temp_color = getcolor();
    t_bound_box arrow((from.x) - 0.03, (from.y) - 0.03, (from.x) + 0.03, (from.y) + 0.03);
    setcolor(LIMEGREEN);
    settextrotation(setRotation(from, to));
    drawtext_in(arrow, "->");
    setcolor(temp_color);
}

//rotation function for one way street arrows

double setRotation(t_point &from, t_point &to) {
    if (from.x == to.x) return 0;
    double angle = atan((to.y - from.y) / (to.x - from.x)) * 180 / PI;
    //if (angle < 0.0) angle += 360;
    return angle;
}


//draw minor street names

void drawMinorNames() {
    setcolor(BLACK);

    vector<unsigned int> minor = find_street_segment_street_by_type("minor");
    for (unsigned int x = 0; x < minor.size(); x = x + 3) {
        StreetSegmentInfo currStreetSegment = getStreetSegmentInfo(minor[x]);
        if (getStreetName(currStreetSegment.streetID) != "<unknown>") {
            t_point from = getAbsoluteXYPos(getIntersectionPosition(currStreetSegment.from));
            t_point to = getAbsoluteXYPos(getIntersectionPosition(currStreetSegment.to));

            if (to.x - from.x < 0) {
                settextrotation(asin((-to.y + from.y) / getAbsoluteXYLength(to, from))*180.0 / PI);
            } else {
                settextrotation(asin((to.y - from.y) / getAbsoluteXYLength(to, from))*180.0 / PI);
            }
            drawtext((to.x + from.x) / 2, (to.y + from.y) / 2, getStreetName(currStreetSegment.streetID), FLT_MAX, FLT_MAX);

        }
    }
    clean_names.clear();
}

//draw regular street names

void drawRegularNames() {
    setcolor(BLACK);

    vector<unsigned int> regular = find_street_segment_street_by_type("regular");
    for (unsigned int x = 0; x < regular.size(); x = x + 3) {
        StreetSegmentInfo currStreetSegment = getStreetSegmentInfo(regular[x]);
        if (getStreetName(currStreetSegment.streetID) != "<unknown>") {

            drawingStreetNames(currStreetSegment, get_visible_world().get_width(), "regular");
        }
    }
    clean_names.clear();
}

//draws major names

void drawMajorNames() {
    setcolor(BLACK);

    vector<unsigned int> major = find_street_segment_street_by_type("major");
    for (unsigned int x = 0; x < major.size(); x++) {
        StreetSegmentInfo currStreetSegment = getStreetSegmentInfo(major[x]);
        if (getStreetName(currStreetSegment.streetID) != "<unknown>") {

            drawingStreetNames(currStreetSegment, get_visible_world().get_width(), "major");
        }
    }
    clean_names.clear();
}

//draws freeway names

void drawFreeWayNames() {
    setcolor(BLACK);
    vector<unsigned int> freeways = find_street_segment_street_by_type("freeway");
    for (unsigned int x = 0; x < freeways.size(); x++) {
        StreetSegmentInfo currStreetSegment = getStreetSegmentInfo(freeways[x]);
        if (getStreetName(currStreetSegment.streetID) != "<unknown>") {

            drawingStreetNames(currStreetSegment, get_visible_world().get_width(), "freeway");
        }
    }
    clean_names.clear();
}

//draws street names based on rotation

void drawingStreetNames(StreetSegmentInfo currStreetSegment, double width, string type) {

    t_point from = getAbsoluteXYPos(getIntersectionPosition(currStreetSegment.from));
    t_point to = getAbsoluteXYPos(getIntersectionPosition(currStreetSegment.to));

    if (checkCleanNames((to.x + from.x) / 2, (to.y + from.y) / 2, width, type)) {
        if (to.x - from.x < 0) {
            settextrotation(asin((-to.y + from.y) / getAbsoluteXYLength(to, from))*180.0 / PI);
        } else {
            settextrotation(asin((to.y - from.y) / getAbsoluteXYLength(to, from))*180.0 / PI);
        }
        drawtext((to.x + from.x) / 2, (to.y + from.y) / 2, getStreetName(currStreetSegment.streetID), FLT_MAX, FLT_MAX);
    }
}

// prevents too much overlap of names by limiting distance

bool checkCleanNames(double x, double y, double width, string type) {
    double use_width;
    if (type == "freeway") {
        use_width = width / 8;
    } else if (type == "major") {
        use_width = width / 12;
    } else if (type == "minor") {
        use_width = width / 14;
    } else {
        use_width = width / 13;
    }
    for (unsigned i = 0; i < clean_names.size(); i++) {
        t_point temp;
        temp.x = x;
        temp.y = y;

        if (getAbsoluteXYLength(clean_names[i], temp) < use_width) {
            return false;
        }
    }
    t_point temp;
    temp.x = x;
    temp.y = y;
    clean_names.push_back(temp);
    return true;
}


    
    
//Iterates through all the features and draws parks
void drawPark() {
  unsigned featureCount = getNumberOfFeatures();
  for (unsigned i = 0; i < featureCount; i++) {
  // Check for park(1)
        if (getFeatureType(i) == 1 ) {
           // setcolor();//;LIMEGREEN
            //setcolor(TURQUOISE);
            setcolor(t_color(203, 230, 163));
            setlinewidth(5);
            drawPolygon(getFeaturePointCount(i), i);
        }
}
}
//Iterates through all the features and draws greenspaces
void drawGreenery() {
    unsigned featureCount = getNumberOfFeatures();
    for (unsigned i = 0; i < featureCount; i++) {
        // Check for greenspace(8)
        if ( getFeatureType(i) == 8) {
            setcolor(t_color(203, 230, 163));
            setlinewidth(5);
            drawPolygon(getFeaturePointCount(i), i);
        }
    }
}
//Iterates through all the features and draws Golfcourses
void drawGolfcourse() {
    unsigned featureCount = getNumberOfFeatures();
    for (unsigned i = 0; i < featureCount; i++) {
        // Check for golfcourse(9)
        if  ( getFeatureType(i) == 9) {
             setlinewidth(5);
             //setcolor(DARKGREEN);
             setcolor(t_color(214, 233, 185));
            drawPolygon(getFeaturePointCount(i), i);
        }
    }
}

//Iterates through all the features and draws rivers
void drawRiver() {
    unsigned featureCount = getNumberOfFeatures();
    for (unsigned i = 0; i < featureCount; i++) {
       
        if(getFeatureType(i) == 4) {    
            setlinewidth(1);
            setcolor(t_color(163, 204, 245));
            //setcolor(LIGHTSKYBLUE );//River,  DARKSLATEBLUE
            drawPolygon(getFeaturePointCount(i), i);
        }
        else if (getFeatureType(i) == 10) {    
            setlinewidth(1);
            setcolor(t_color(163, 204, 245));
            //setcolor(LIGHTMEDIUMBLUE);   //Stream
            drawPolygon(getFeaturePointCount(i), i);
        }      
}
}

//Iterates through all the features and draws Streams
void drawStream() {
    unsigned featureCount = getNumberOfFeatures();
    for (unsigned i = 0; i < featureCount; i++) {
        if (getFeatureType(i) == 10) {    
            setlinewidth(1);
            setcolor(t_color(163, 204, 245));
            //setcolor(LIGHTMEDIUMBLUE);   //Stream
            drawPolygon(getFeaturePointCount(i), i);
        }      
}
}

//Iterates through all the features and draws beachs
void drawBeach() {
    unsigned featureCount = getNumberOfFeatures();
    for (unsigned i = 0; i < featureCount; i++) {
      // Check for Beach(2),
        if (getFeatureType(i) == 2 ) {    
            setlinewidth(2);
            //setcolor(YELLOW);   //Beach
            setcolor(t_color(250, 242, 199));
            drawPolygon(getFeaturePointCount(i), i);
        }
}
}
//Iterates through all the features and draws  lakes
void drawLake() {
  unsigned featureCount = getNumberOfFeatures();
  for (unsigned i = 0; i < featureCount; i++) {
    // Check for Lake(3),
      if (getFeatureType(i) == 3 ) {    
            setlinewidth(2);
            setcolor(t_color(163, 204, 245));
            //setcolor(BLUE);  //Lake,
            drawPolygon(getFeaturePointCount(i), i);
        }
}
}
//Iterates through all the features and draws islands
void drawIslands() {
    unsigned featureCount = getNumberOfFeatures();
    for (unsigned i = 0; i < featureCount; i++) {
        // check for island(5)
        if (getFeatureType(i) == 5) {
            setlinewidth(3);
            //setcolor(DARKGREEN);//DARKGREY,KHAKI,
            setcolor(t_color(203, 230, 163));
            drawPolygon(getFeaturePointCount(i), i);
        }
    }
}
//Iterates through all the features and draws Shoreline
void drawShoreline() {
    unsigned featureCount = getNumberOfFeatures();
    for (unsigned i = 0; i < featureCount; i++) {
        // check for Shoreline(6)
        if (getFeatureType(i) == 6) {     // Shoreline
            setlinewidth(2);
            setcolor(SADDLEBROWN);//
            drawPolygon(getFeaturePointCount(i), i);
        }
     }
}

//draws  building or  unknown.

void drawBuildingsAndUnknown() {
    unsigned featureCount = getNumberOfFeatures();

    for (unsigned i = 0; i < featureCount; i++) {
        if (getFeatureType(i) == 7) {
            setlinewidth(2);
            //setcolor(LIGHTGREY);
            setcolor(t_color(199, 204, 207));
            drawPolygon(getFeaturePointCount(i), i);
        }// Unknown 
        else if (getFeatureType(i) == 0) {
            setlinewidth(2);
            //setcolor(BISQUE);
            setcolor(t_color(199, 204, 207));
            drawPolygon(getFeaturePointCount(i), i);
        }

    }
}




// Does the same as the below function 
// first gets the coordinates from features points and its ids

void drawPolygon(unsigned featurePoints, unsigned featureId) {
    unsigned i = 0;
    LatLon start = getFeaturePoint(featureId, i);
    LatLon end = getFeaturePoint(featureId, featurePoints - 1);
    std::vector<t_point> featurePointsXY;

    bool polygon = false;
    featurePointsXY.push_back(getAbsoluteXYPos(start));
    if ((start.lat() == end.lat()) && (start.lon() == end.lon())) {
        polygon = true;
    }

    for (i = 1; i < featurePoints; i++) {
        LatLon coordinates = getFeaturePoint(featureId, i);
        // featurePointsXY[i] = getAbsoluteXYPos(coordinates);
        featurePointsXY.push_back(getAbsoluteXYPos(coordinates));
        if (polygon == false) {
            //setlinewidth(3);
            drawline(featurePointsXY[i - 1], featurePointsXY[i]);
        }
    }
    if (polygon == true) {
        fillpoly(&featurePointsXY[0], featurePoints);
    }
    featurePointsXY.clear();
}

// Second gets the coordinates from LatLon points

void drawPolygon(std::vector<LatLon> points) {
    bool polygon = false;
    std::vector<t_point> featurePointsXY;
    //Checks here if it is a polygon or a line
    if ((points[0].lat() == points[points.size() - 1].lat()) && (points[0].lon() == points[points.size() - 1].lon())) {
        polygon = true;
    }
    unsigned i = 0;
    // change  Points_XY to Absolute_PosPointsXY [points.size()];
    featurePointsXY.push_back(getAbsoluteXYPos(points[0]));
    for (i = 1; i < points.size(); i++) {
        featurePointsXY.push_back(getAbsoluteXYPos(points[i]));
        if (polygon == false) {
            setlinewidth(3);
            drawline(featurePointsXY[i - 1], featurePointsXY[i]);
        }
    }
    //Draws that multi-sided figure
    if (polygon == true) {
        fillpoly(&featurePointsXY[0], points.size());

    }
    featurePointsXY.clear();
}



//Draw any other POIs.

void drawPointofInterest(unsigned POI_id, float rad, color_types color_1, color_types color_2) {
    LatLon position = getPointOfInterestPosition(POI_id);
    t_point POI_position = getAbsoluteXYPos(position);
    setcolor(color_1);
    fillarc(POI_position, rad, 0, 360);
    setcolor(color_2);
    fillarc(POI_position, rad / 2, 0, 360);
}

//Unordered map for associating types with POI_id.

void POItype_by_POI_id_preload(unordered_map <string, vector<unsigned>> &POItype_by_POI_ids) {
    for (unsigned i = 0; i < getNumberOfPointsOfInterest(); i++) {
        string POI_type = getPointOfInterestType(i);
        auto existing_type = POItype_by_POI_ids.find(POI_type);
        if (existing_type == POItype_by_POI_ids.end()) {
            vector<unsigned>temp_type;
            temp_type.push_back(i);
            POItype_by_POI_ids.insert(make_pair(POI_type, temp_type));
        } else {
            existing_type->second.push_back(i);
        }
    }
}

vector<unsigned> get_POI_id_by_POItype(string type) {
    vector<unsigned> POI_id;
    if (POItype_by_POI_id.count(type) > 0) {
        POI_id = POItype_by_POI_id.find(type)->second;
    }
    return POI_id;
}
//These following functions are used for drawing different types of POI in different levels
//of scales.

void drawPOIbyType(string type) {
    vector<unsigned> POI_id = get_POI_id_by_POItype(type);
    if (POI_id.size() > 0) {
        if (type == "fast_food" || type == "food_court" || type == "restaurant")
            drawSustenance(POI_id);
        else if (type == "college" || type == "school" || type == "university")
            drawEducation(POI_id);
        else if (type == "bus_station" || type == "ferry_terminal")
            drawTransportation(POI_id);
        else if (type == "bank")
            drawFinancial(POI_id);
        else if (type == "clinic" || type == "doctors" || type == "hospital")
            drawHealthcare(POI_id);
        else {
            for (auto iter = POI_id.begin(); iter != POI_id.end(); iter++) {
                drawPointofInterest(*iter, 0.00005, KHAKI, WHITE);
            }
        }
    }
}

void drawPOIName(t_point POI_position, unsigned POI_id) {
    string name = getPointOfInterestName(POI_id);
    if (name != "<noname>") {
        setcolor(BLACK);
        setfontsize(7);
        float offset = abs(xscrn_to_world(12) - xscrn_to_world(0));
        drawtext(POI_position.x, POI_position.y + offset, name, 0.001, 0.001);
    }
}

void drawSustenance(vector<unsigned> POI_id) {
    float width = 2 * sqrt(1 / (get_visible_world().get_width()*100)) + 2;
    for (auto iter = POI_id.begin(); iter != POI_id.end(); iter++) {
        LatLon position = getPointOfInterestPosition(*iter);
        t_point POI_position = getAbsoluteXYPos(position);
        setcolor(YELLOW);
        fillarc(POI_position, 0.0000035, 0, 360);
        setcolor(WHITE);
        t_bound_box text((POI_position.x) - 0.00008, (POI_position.y) - 0.00008,
                (POI_position.x) + 0.00008, (POI_position.y) + 0.00008);
        settextrotation(0);
        setfontsize(width);
        drawtext_in(text, ":)");
        drawPOIName(POI_position, *iter);
    }
}

void drawEducation(vector<unsigned> POI_id) {
    float width = 2 * sqrt(1 / (get_visible_world().get_width()*100)) + 2;
    for (auto iter = POI_id.begin(); iter != POI_id.end(); iter++) {
        LatLon position = getPointOfInterestPosition(*iter);
        t_point POI_position = getAbsoluteXYPos(position);
   //     setcolor(LIGHTSKYBLUE);
  //      fillarc(POI_position, 0.0000035, 0, 360);
        setcolor(WHITE);
        t_bound_box text((POI_position.x) - 0.00008, (POI_position.y) - 0.00008,
                (POI_position.x) + 0.00008, (POI_position.y) + 0.00008);
        settextrotation(0);
        setfontsize(width);
   ///     drawtext_in(text, "A+");
        drawPOIName(POI_position, *iter);
    }
}

void drawTransportation(vector<unsigned> POI_id) {
    float width = 2 * sqrt(1 / (get_visible_world().get_width()*100)) + 2;
    for (auto iter = POI_id.begin(); iter != POI_id.end(); iter++) {
        LatLon position = getPointOfInterestPosition(*iter);
        t_point POI_position = getAbsoluteXYPos(position);
        setcolor(LIGHTMEDIUMBLUE);
        fillarc(POI_position, 0.0000035, 0, 360);
        setcolor(WHITE);
        t_bound_box text((POI_position.x) - 0.00008, (POI_position.y) - 0.00008,
                (POI_position.x) + 0.00008, (POI_position.y) + 0.00008);
        settextrotation(0);
        setfontsize(width);
        drawtext_in(text, "<==");
        draw_surface(demo, text.get_xcenter(), text.get_ycenter());
        drawPOIName(POI_position, *iter);
    }
}

void drawFinancial(vector<unsigned> POI_id) {
    float width = 2 * sqrt(1 / (get_visible_world().get_width()*100)) + 2;
    for (auto iter = POI_id.begin(); iter != POI_id.end(); iter++) {
        LatLon position = getPointOfInterestPosition(*iter);
        t_point POI_position = getAbsoluteXYPos(position);
        setcolor(LIMEGREEN);
     //   fillarc(POI_position, 0.0000035, 0, 360);
        setcolor(WHITE);
        t_bound_box text((POI_position.x) - 0.00008, (POI_position.y) - 0.00008,
                (POI_position.x) + 0.00008, (POI_position.y) + 0.00008);
        settextrotation(0);
        setfontsize(width);
     //   drawtext_in(text, "$");
        //Is it necessary to always show POI's name?
   //     draw_surface(bank, text.get_xcenter(), text.get_ycenter());
        drawPOIName(POI_position, *iter);
    }
}

void drawHealthcare(vector<unsigned> POI_id) {
    float width = 2 * sqrt(1 / (get_visible_world().get_width()*100)) + 2;
    for (auto iter = POI_id.begin(); iter != POI_id.end(); iter++) {
        LatLon position = getPointOfInterestPosition(*iter);
        t_point POI_position = getAbsoluteXYPos(position);
        setcolor(RED);
     //   fillarc(POI_position, 0.0000035, 0, 360);
        setcolor(WHITE);
        t_bound_box text((POI_position.x) - 0.00008, (POI_position.y) - 0.00008,
                (POI_position.x) + 0.00008, (POI_position.y) + 0.00008);
        settextrotation(0);
        setfontsize(width);
    //    drawtext_in(text, "+");
    //    draw_surface(hospital, text.get_xcenter(), text.get_ycenter());
        drawPOIName(POI_position, *iter);
    }
}


//New button for searching a POI

void act_on_POI(void((*drawscreen) (void))) {
    update_message("Search for a point of interest on the terminal. ");
    string name;
    getline(cin, name);

    //vector<unsigned> POI_id = getPointOfInterestID(name);		
    found_POI_id = getPointOfInterestID(name);
    if (found_POI_id.size() > 0) {
        show_highlighted_POI = true;
        //draw_highlighted_POI();
    } else {
        cout << "Not Found." << endl;
        update_message(name + " not found.");
    }

    drawscreen();
}

// Highlights the POI when searched for. 

vector<unsigned> drawPoifromname(string poiname) {
    POIIDS = getPointOfInterestID(poiname);
    for (auto itr = POIIDS.begin(); itr != POIIDS.end(); itr++) {
        LatLon location = getPointOfInterestPosition(*itr);
        drawHighlightedIntersectionAndPOI(location);
    }
    return POIIDS;
}

void draw_highlighted_POI() {
    if (show_highlighted_POI) {
        string bottom_text = "";
        setcolor(YELLOW);
        for (auto i = found_POI_id.begin(); i != found_POI_id.end(); i++) {
            LatLon position = getPointOfInterestPosition(*i);
            t_point POI_position = getAbsoluteXYPos(position);
            fillarc(POI_position, 0.0000035, 0, 360);
            bottom_text = bottom_text + "   " + "Point Of Interest: " + getPointOfInterestName(*i) + "   "
                    + "Type: " + getPointOfInterestType(*i);
            cout << "Point Of Interest: " << getPointOfInterestName(*i) << "  Type: " << getPointOfInterestType(*i) << endl;

            update_message(bottom_text);
        }
    }
}

/* Draws a circle highlighted Point of interest with different radius 
 * for different zoom levels */ //continue..........

void drawHighlightedIntersectionAndPOI(LatLon position) {
    t_point location = getAbsoluteXYPos(position);

    fillellipticarc(location.x, location.y, 0.00007, 0.00007, 0, 360);
    flushinput();
}

/* Draws the osm buildings like airport terminals and train stations. 
 * Draws a polygon around a vector of latlons. */
void drawOSMBuildings() {
    setcolor(BLUE);
    for (auto itr = OSMBuildings.begin(); itr != OSMBuildings.end(); itr++) {
        drawPolygon(*itr);
    }
}

//Draws the gas ,hospitals,the different points of interest 
// 
//when the user clicks on the menu.

void drawHospitals() {
    for (unsigned i = 0; i < getNumberOfPointsOfInterest(); i++) {
        if (getPointOfInterestType(i) == "clinic" || getPointOfInterestType(i) == "doctor" || getPointOfInterestType(i) == "hospital") {
            LatLon location = getPointOfInterestPosition(i);
            t_point position = getAbsoluteXYPos(location);
            setcolor(DARKSLATEBLUE);
            drawHighlightedIntersectionAndPOI(location);
            setcolor(BLACK);
            setfontsize(7);
            drawtext(position, getPointOfInterestName(i), 1000, 1000);

        }
    }
}


//Clears all highlighted stuff

void clearAllVectors() {

    POIIDS.clear();

}



// button for help

void act_on_help_button_func(void((*drawscreen)(void))) {
    if (!HELPSHOWED) {
        HELPSHOWED = true;
        HelpOutput();
    } else {
        HELPSHOWED = false;
        drawscreen();
    }
   
}

double get_angle(unsigned segment_begin, unsigned segment_end) {
    StreetSegmentInfo begin = getStreetSegmentInfo(segment_begin);
    StreetSegmentInfo end = getStreetSegmentInfo(segment_end);
    unsigned intersect, from, to;

    //to == to
    if (begin.to == end.to) {
        intersect = begin.to;
        from = begin.from;
        to = end.from;
    }//to = from
    else if (begin.to == end.from) {
        intersect = begin.to;
        from = begin.from;
        to = end.to;
    }//from == to
    else if (begin.from == end.to) {
        intersect = begin.from;
        from = begin.to;
        to = end.from;
    }//from == from
    else {
        intersect = begin.from;
        from = begin.to;
        to = end.to;
    }


    LatLon intersect_latlon = getIntersectionPosition(intersect);
    LatLon to_latlon = getIntersectionPosition(to);
    LatLon from_latlon = getIntersectionPosition(from);

    t_point intersect_xy = getAbsoluteXYPos(intersect_latlon);
    t_point to_xy = getAbsoluteXYPos(to_latlon);
    t_point from_xy = getAbsoluteXYPos(from_latlon);

    double x1, x2, y1, y2;
    x1 = from_xy.x - intersect_xy.x;
    y1 = from_xy.y - intersect_xy.y;

    x2 = to_xy.x - intersect_xy.x;
    y2 = to_xy.y - intersect_xy.y;

    double dot = x1 * x2 + y1*y2;
    double det = x1 * y2 - y1*x2;
    double angle_deg = atan2(det, dot)* 180 / PI;
    return angle_deg;

}

extern vector <unsigned> shortest_path_instruction;


void act_on_terminal_out_button_func(void((*drawscreen)(void))) {
    
    if (!HELPSHOWED) {
        HELPSHOWED = true;
        
         show_path = true; 
         path = find_path_between_intersections(25454, 88919,0);
         write_path_tem(path,25454);
         PathOutput();
         update_message("See terminal for directions.");
        
      
       
    } else {
        show_path = false; 
        HELPSHOWED = false;
        drawscreen();
    }
}
void act_on_path_instructions(void((*drawscreen)(void))) {
    if (show_path) {
        update_message("See terminal for directions.");
        if (shortest_path_instruction.empty()) {
            cout << "You are at the destination." << endl;
            if (POI_flag) {
                cout << "You may now go to " << POI_name_global << "." << endl;
            }
        } else {
            unsigned prev_path = shortest_path_instruction[0];
            cout << endl << endl;
            cout << "Your optimal path will take around " << static_cast<int> (compute_path_travel_time(shortest_path_instruction, TURN_PENALTY) / 60) << " minutes." << endl;
            cout << "You are at " << getIntersectionName(begin_intersection) << ". Continue on " << getStreetName(getStreetSegmentInfo(path[0]).streetID) << "." << endl;
            for (unsigned int x = 0; x < shortest_path_instruction.size(); x++) {
//                if (shortest_path_instruction[x] == prev_path) {
//                    distance = distance + (find_street_segment_length(shortest_path_instruction[x])) / 1000;
//                }//entering into a new street, tell you the dintance for the street
//                //turn right or left, the new street name
//                else {
//                // unit for length  :kilometers(>=1km) or meters(<1km)
//                if (distance >= 1) {
//                    string distanceToString = boost::lexical_cast<std::string>(to_string_with_precision(distance, 2));
//                    cout << "After " << distanceToString << " kilometers, " << endl;
//                } else {
//                    string distanceToString = boost::lexical_cast <std::string> (to_string_with_precision(distance * 1000, 2));
//                    cout << "After "  << distanceToString << " meters, " << endl;
//                }
//                }
                //new road
                if (getStreetSegmentInfo(prev_path).streetID != getStreetSegmentInfo(path[x]).streetID) {
                    double angle = get_angle(prev_path, shortest_path_instruction[x]);
                    //0 to 170
                    if (angle > 0 && angle < 170)
                        cout << "Turn right onto " << getStreetName(getStreetSegmentInfo(shortest_path_instruction[x]).streetID) << "." << endl;
                        //0 to -170
                    else if (angle>-170) {
                        cout << "Turn left onto " << getStreetName(getStreetSegmentInfo(shortest_path_instruction[x]).streetID) << "." << endl;
                    } else {
                        cout << "Continue onto " << getStreetName(getStreetSegmentInfo(shortest_path_instruction[x]).streetID) << "." << endl;
                    }
                }
                prev_path = shortest_path_instruction[x];
            }
            cout << "You have arrived at " << getIntersectionName(end_intersection) << "." << endl;
            if (POI_flag) {
                cout << "You may now go to " << POI_name_global << "." << endl;
            }
        }
    } else {
        update_message("No path found.");
    }
    drawscreen();
}

void HelpOutput() {
    std::vector<std::string> helperScript = {
        "User Guide",          
        "The map contains multiple functions.",
        "Type in Search bar and follow the format using tags to start:",
        "E.g. SEARCH: Bay Street & College Street",
        "PATH: Bay Street & College Street to Queen's Park & College Street",
        "POI: Bay Street & College Street to CIBC",
        "Click Toggle On, select 2 intersections & 1 path will display.",
        "Click Directions after finding path to receive the directions on terminal.",
        "To close this page, click HELP button again.",
        

    };

    set_drawing_buffer(ON_SCREEN);
    //based on current_view, set values of outputArea  
    t_bound_box current_view = get_visible_screen();
    //Set the width and height for text box
    float height = current_view.get_height() * 0.50;//0.45
    float width = current_view.get_width() * 0.50;//0.50

    //Set up the coordinates systems for the BOX
    t_point bottom_left = t_point(current_view.bottom_left().x + current_view.get_width()*0.30,
            current_view.bottom_left().y + current_view.get_height()*0.30);

    t_bound_box  helperBox = t_bound_box(bottom_left, width, height);
    settextrotation(0);
    t_point   LinerBoxCente[10];
    set_coordinate_system(GL_SCREEN);
    for(unsigned i=0;i<5;i++){
      LinerBoxCente[i] =  t_point( helperBox.get_xcenter(),  helperBox.get_ycenter() + helperBox.get_height()*(4-i)* 0.100);
      LinerBoxCente[5+i] = t_point( helperBox.get_xcenter(),  helperBox.get_ycenter()- helperBox.get_height()*(1+i)*0.100);
    }

    // draw text area
    float  width1 = 5;
    setlinewidth(width1);
    setcolor(WHITE);
    fillrect(helperBox);
    setcolor(LIGHTGREY);
    drawrect(helperBox);
    // colour and font 
    setcolor(BLACK);
    setfontsize(11.5);
    for (unsigned i = 0; i<9; i++) {
         
     drawtext( LinerBoxCente[i], helperScript[i], helperBox,FLT_MAX);
     
    }
     set_coordinate_system(GL_WORLD);
}


void drawBox(t_bound_box &drawingArea) {
    //fill the rect first then draw the lines 
    int width=5;
    setlinewidth(width);
    setcolor(WHITE);
    fillrect(drawingArea);
    setcolor(LIGHTGREY);
    drawrect(drawingArea);
}

void PathOutput( /*std::vector<std::string> &script*/ ){
     set_drawing_buffer(ON_SCREEN);
    //based on current_view, set values of boxArea  
    t_bound_box current_view = get_visible_screen();
    //Set the width and height for the pop up window
    float height = current_view.get_height() * 0.90;
    float width = current_view.get_width() * 0.6;

    //Set up the coordinates systems for the BOX
    t_point bottom_left = t_point(current_view.bottom_left().x + current_view.get_width()*0.1,
            current_view.bottom_left().y + current_view.get_height()*0.10);

    t_bound_box  helperBox = t_bound_box(bottom_left, width, height);
    settextrotation(0);
    t_point   LinerBoxCente[25];
    set_coordinate_system(GL_SCREEN);
    for(unsigned i=0;i<12;i++){
      LinerBoxCente[i] =  t_point( helperBox.get_xcenter(),  helperBox.get_ycenter() + helperBox.get_height()*(11-i)* 0.040);
      LinerBoxCente[12+i] = t_point( helperBox.get_xcenter(),  helperBox.get_ycenter()- helperBox.get_height()*(1+i)*0.040);
    }
    // draw text area
    float  width1 = 4;
    setlinewidth(width1);
     setcolor(WHITE);
   // setcolor(SADDLEBROWN);
     fillrect(helperBox);
     setcolor(SADDLEBROWN);
     drawrect(helperBox);
    // printing 
     setcolor(BLUE);
     setfontsize(9);
    
    for (unsigned i = 0; i<pathScript.size(); i++) {
    drawtext( LinerBoxCente[i], pathScript[i], helperBox,FLT_MAX);
    }
//  draw_surface(demo, LinerBoxCente[1].x, LinerBoxCente[1].y);
//  draw_surface(pleft,  LinerBoxCente[2].x,  LinerBoxCente[2].y);
//  draw_surface(bank,  LinerBoxCente[3].x, LinerBoxCente[3].y);
//  draw_surface(hospital  , LinerBoxCente[0].x,  LinerBoxCente[0].y);
    
     set_coordinate_system(GL_WORLD);
  
}

 
std::vector<std::string> write_script(const std::vector<unsigned>& path_vectoc_Segment, unsigned intersect_id_Segment_startId) {
    IntersectionIndex  Segment_startId, Segment_endId, temp=0;
    StreetSegmentIndex   current_Segment; 
    StreetIndex   pre_StreetId, current_StreetId;;
    double pre_Segment_Degree, next_Segment_Degree; 
    double distance = 0., totaldistance = 0.,street_time=0,travelTime = 0.;
    unsigned curveCount=0,lastCurveCount=0;
    double compare ;
    
    std::vector<string> path_description;
    std::vector<double> outdegree;
   // curvePoint info for comparing angles
    LatLon pre_LatLon, next_LatLon;
    t_point pre_LatLon_tpoint, next_LatLon_tpoint, instersect_LatLon_tpoint;
   //  write 
    Segment_startId = intersect_id_Segment_startId;
  for (unsigned i(0); i < path_vectoc_Segment.size(); i++) {
       //  get the basic info for the segment
        current_Segment = path_vectoc_Segment[i];
        StreetSegmentInfo current_SegmentInfo = getStreetSegmentInfo(current_Segment);
        current_StreetId = current_SegmentInfo.streetID;
        curveCount = current_SegmentInfo.curvePointCount;
        if (curveCount == 0)    temp =  Segment_startId;
       // always determine the  Segment_startId point and end point id
    
        if ( Segment_startId == current_SegmentInfo.from)
                Segment_endId = current_SegmentInfo.to;
        else    Segment_endId = current_SegmentInfo.from;
    // write the Segment_startId in XXX street
        if (i == 0)  {
                  path_description.push_back("Start the journey!\n" );
                pre_StreetId = current_StreetId;         
            }
        if ( current_StreetId == pre_StreetId ){
               distance=distance + find_street_segment_length(current_Segment);
               street_time=street_time+find_street_segment_travel_time(current_Segment);
               }    
                  //  the new street dintance 
        if( current_StreetId != pre_StreetId){
              path_description.push_back("Enter: " + getStreetName(pre_StreetId) 
               +" DISTANCE: " + to_string_with_precision( distance/1000,3) + " km, " +
             "TIME: " + to_string_with_precision( street_time/60,2) + "  min\n");
             
                     //    Segment_endId to determine turn right/left
                     //   calculate pre_Segment_Degree and  next_Segment_Degree
             if (i != 0) {
                if (lastCurveCount != 0)
                                      //    last current_SegmentInfo last curvePoint, path_vectoc_Segment[i-1]
                    pre_LatLon = getStreetSegmentCurvePoint(path_vectoc_Segment[i - 1], lastCurveCount - 1);
                if (lastCurveCount == 0)
                    pre_LatLon = getIntersectionPosition(temp);
                if (curveCount != 0)
                     next_LatLon = getStreetSegmentCurvePoint(path_vectoc_Segment[i], 0);
                if (curveCount == 0)
                    next_LatLon = getIntersectionPosition( Segment_endId);
                        //    transfer into  t_point(x,y)
                pre_LatLon_tpoint = getAbsoluteXYPos(pre_LatLon);
                next_LatLon_tpoint = getAbsoluteXYPos(next_LatLon);
                instersect_LatLon_tpoint = getAbsoluteXYPos(getIntersectionPosition( Segment_startId) );
                pre_Segment_Degree= Degreefrom_point_to_point( pre_LatLon_tpoint ,instersect_LatLon_tpoint,next_LatLon_tpoint )[0];
                next_Segment_Degree= Degreefrom_point_to_point( pre_LatLon_tpoint ,instersect_LatLon_tpoint,next_LatLon_tpoint )[1]; 
                compare = next_Segment_Degree  - pre_Segment_Degree;
                 ///   need to transform them back into (0, 2*pi)
                if (compare > 360)
                    compare = compare - 360;
                if (compare < 0)
                    compare = compare + 360;
                        
            if (getStreetName( current_StreetId ) != "<unknown>" /*&& getStreetName( pre_StreetId ) != "<unknown>"*/) { //33
                             // determine the  Segment_instersect_segment   turning left  or right
                    if ((compare > 210 && compare < 360) || (compare < 0 && compare > -150)) {
                        path_description.push_back("Turn left at: " + getIntersectionName( Segment_startId )+ ",\n");
                        path_description.push_back("Into: " + getStreetName( current_StreetId ) + ",\n");
                    } else if ((compare > 0 && compare < 150) || (compare > -360 && compare < -210)) {
                        path_description.push_back("Turn right at: " + getIntersectionName( Segment_startId ) + ",\n");
                        path_description.push_back("Into: " + getStreetName(current_StreetId) + ",\n");
                    }
                 }  //33 
                          
               else if (getStreetName(current_StreetId) == "<unknown>") {
                                //now determine whether the  Segment_end segment is turning left/right
                        if ((compare > 210 && compare < 360) || (compare < 0 && compare > -150)) {
                                  path_description.push_back("Turn left,\n");
                          } else if ((compare > 0 && compare < 150) || (compare > -360 && compare < -210)) {
                                    path_description.push_back("Turn right,\n");
                          }
                         }    
                            }        
                  ////*to recount the new street  compare angle=0  
                     totaldistance= distance+totaldistance;
                     travelTime =street_time + travelTime; 
                     distance=0;
                     compare = 0;
                     street_time=0;
                                                }     
       if (i == (path_vectoc_Segment.size() - 1) && distance != 0) {
             path_description.push_back("will reach the destination after: "+to_string_with_precision( street_time/60,2)+"  min\n");
                      }
        //* move to next  Segment_endId 
        Segment_startId =  Segment_endId;
        pre_StreetId = current_StreetId;
        lastCurveCount = curveCount;
         }  // * loop end
    path_description.push_back("You have reached your destination. ");
    path_description.push_back("The total distance is around: " + to_string_with_precision(totaldistance/1000,3) + " km "+
             "   The total time is around: " + to_string_with_precision( travelTime/60,2) + "  min\n");
    return path_description;
}
 
//To calculate the degree of the intersections between the start and end street segments
 vector<double> Degreefrom_point_to_point(t_point  start_tpoint,t_point  intsec_tpoint,t_point  end_tpoint){   
       double outDegree1=0.,outDegree2=0.;
       vector<double>temp;
    if ( start_tpoint.x== intsec_tpoint.x) {
             if (intsec_tpoint.y >start_tpoint.y) outDegree1=90.;
             else outDegree1= 270.  ;
    } else if (start_tpoint.y== intsec_tpoint.y ) {
              if (intsec_tpoint.x > start_tpoint.x) outDegree1=0. ;
              else   outDegree1= 180. ;
    }else if (start_tpoint.x < intsec_tpoint.x) {
        
         outDegree1 = atan((-intsec_tpoint.y + start_tpoint.y) / (-intsec_tpoint.x + start_tpoint.x)) / DEG_TO_RAD;
     //     if(outDegree1 >=0. )180 +abs(outDegree1);else 180 - abs(outDegree1);
        if (start_tpoint.y > intsec_tpoint.y) outDegree1 = 180 - abs(outDegree1);
        else  outDegree1 = 180 +abs(outDegree1);
    } else if (start_tpoint.x > intsec_tpoint.x) {
          outDegree1 = atan((-intsec_tpoint.y +start_tpoint.y) / (-intsec_tpoint.x + start_tpoint.x)) / DEG_TO_RAD;
       
    //      if(outDegree1 >= 0.) outDegree1= abs(outDegree1);else 360.- abs(outDegree1);
         if (start_tpoint.y > intsec_tpoint.y) outDegree1= abs(outDegree1);
             else outDegree1=360.- abs(outDegree1);
    } else {  std::cout << "degree output error !!!" << std::endl; }
    if ( intsec_tpoint.x== end_tpoint.x) {
             if (end_tpoint.y >intsec_tpoint.y)outDegree2=90. ;
             else outDegree2= 270.  ;
    } else if (intsec_tpoint.y== end_tpoint.y ) {
              if (end_tpoint.x > intsec_tpoint.x) outDegree2=0. ;
              else   outDegree2= 180. ;
    }else if (intsec_tpoint.x < end_tpoint.x) {
           outDegree2 = atan((end_tpoint.y - intsec_tpoint.y) / (end_tpoint.x - intsec_tpoint.x)) / DEG_TO_RAD;
        
           if (intsec_tpoint.y < end_tpoint.y) outDegree2 = abs(outDegree2);
          else  outDegree2 = 360 - abs(outDegree2);
    
    } else if (intsec_tpoint.x > end_tpoint.x) {
               outDegree2 = atan((end_tpoint.y - intsec_tpoint.y) / (end_tpoint.x - intsec_tpoint.x)) / DEG_TO_RAD;
        if (intsec_tpoint.y > end_tpoint.y) outDegree2= 180. + abs(outDegree2);   
           else if (intsec_tpoint.y > end_tpoint.y) outDegree2= 180. -abs(outDegree2);
        } else {  std::cout << "degree ouput error !!!" << std::endl; }
       
       temp.push_back(outDegree1);
       temp.push_back(outDegree2);
            return  temp;
 }
 
std::string to_string_with_precision(double value, int n) {
    std::ostringstream out;
    out << std::fixed << std::setprecision(n) << value;
    return out.str();
}
void write_path_tem(std::vector<unsigned> &pathTodraw, unsigned intersection) {
   
    pathScript = write_script(pathTodraw, intersection);
    for( unsigned i=0 ; i < pathScript.size(); i++ ) {
        cout << pathScript[i] << endl;
    }
}
