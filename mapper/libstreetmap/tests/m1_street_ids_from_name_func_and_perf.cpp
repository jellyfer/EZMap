#include <algorithm>
#include <set>
#include <unittest++/UnitTest++.h>
#include "unit_test_util.h"
#include "m1.h"
#include "StreetsDatabaseAPI.h"

using ece297test::relative_error;

struct MapFixture {

    MapFixture() {
        //Load the map
        load_map("/cad2/ece297s/public/maps/toronto_canada.streets.bin");
        //Initialize random number generators
        rng = std::minstd_rand(3);
        rand_street = std::uniform_int_distribution<unsigned>(1, getNumberOfStreets() - 1);
    }

    ~MapFixture() {
        //Clean-up
        close_map();
    }

    std::minstd_rand rng;
    std::uniform_int_distribution<unsigned> rand_street;
};

SUITE(street_ids_from_name) {

    TEST_FIXTURE(MapFixture, street_ids_from_name_corner) {
        std::vector<unsigned> expected;
        std::vector<unsigned> actual;

        //test empty
        expected = {};
        actual = find_street_ids_from_name("not an actual street");
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        //test street with multiple ids
        expected = {1808, 6481, 16023};
        actual = find_street_ids_from_name("Main Street");
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        //test first street
        expected = {0};
        actual = find_street_ids_from_name("<unknown>");
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);

        //test last street
        expected = {21221};
        actual = find_street_ids_from_name("Louvain Avenue");
        std::sort(actual.begin(), actual.end());
        CHECK_EQUAL(expected, actual);


    }

    TEST_FIXTURE(MapFixture, street_ids_from_name_func) {
        //Generate 10000 random inputs
        std::vector<unsigned> street_ids;
        for (size_t i = 0; i < 10000; i++) {
            street_ids.push_back(rand_street(rng));
        }
        
        std::vector<unsigned> actual;
        std::vector<unsigned> expected;
        
        //test 10000 random cases
        for (size_t i = 0; i < 10000; i++) {
            //load in expected values
            expected.clear();
            for (size_t x = 0; x < getNumberOfStreets()-1; x++) {
                if (getStreetName (x) == getStreetName(street_ids[i])) {
                    expected.push_back(x);
                }
            }
            //sort expected values
            std::sort(expected.begin(), expected.end());
            
            //load actual values & sort actual values
            actual = find_street_ids_from_name(getStreetName(street_ids[i]));
            std::sort(actual.begin(), actual.end());
            
            CHECK_EQUAL(expected, actual);
        }

    }

    TEST_FIXTURE(MapFixture, street_ids_from_name_perf) {
        //Generate random inputs
        std::vector<std::string> street_names;
        for (size_t i = 0; i < 2000000; i++) {
            unsigned street_id = rand_street(rng);
            street_names.push_back(getStreetName(street_id));
        }
        {
            //Timed Test, testing 2000000 units, so max time should be 500ms
            ECE297_TIME_CONSTRAINT(500);
            std::vector<unsigned> result;
            for (size_t i = 0; i < 2000000; i++) {
                result = find_street_ids_from_name(street_names[i]);
            }
        }
    }

}

