//Author: Andrew Kerr
//Date: 04/06/2022

//===[ INCLUDES ]==================================================================================================================
#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include "Point.hpp"

//===[ MACROS ]====================================================================================================================

//A min macro that will select the smallest of the two.
#define min(a, b) ((a) < (b) ? (a) : (b))

//Define to test for the correct answer!
//#define __TEST_FOR_CORRECT__

//Used to wrap around an error message for displaying to the end user, when in this form it is deactivated.
#define ERR(msg)
// std::cerr << "ERROR:" << std::endl << "\tLine: " << __LINE__ << std::endl << "\tMessage: " << (msg) << std::endl << std::endl;

//Used to wrap around a function call for displaying a trace through the program.
#define TRACE(params)
// std::cerr << "TRACE: " << std::endl << "\tLine: " << __LINE__ << std::endl << "\tFunction: " << __FUNCTION__ << std::endl << "\tParameters:" << params << std::endl;

//Used to wrap around a function call for displaying when a trace has left a function.
#define RETURN()
// std::cerr << "RETURNED: " << std::endl << "\tLine: " << __LINE__ << std::endl << "\tFunction: " << __FUNCTION__ << std::endl << std::endl;

//===[ GLOBALS ]===================================================================================================================

//Tells merge sort by which axis to sort using.
bool bSortByX = true;

//===[ FUNCTIONS ]=================================================================================================================

//Merge sort algorithm specifically the Merge portion of it.
void Merge(Point** src, Point** dst, int begin, int middle, int end) {
    TRACE("{" << src << ", " << dst << ", " << begin << ", " << middle << ", " << end << "}");
    int i = begin; //O(1)
	int j = middle; //O(1)
	
    for(int k = begin; k < end; k++){ //O(n)
        if(i < middle && ( j >= end || (bSortByX ? (src[i]->x() < src[j]->x()) : (src[i]->y() < src[j]->y())))){
            dst[k] = src[i];
            i = i + 1;
        }
        else {
            dst[k] = src[j];
            j = j + 1;
        }
    }

    RETURN();
}

//A recursive merge sort function that recursively splits an array into smaller and smaller parts.
void RecursiveMergeSort(Point** src, Point** dst, int left, int right){ //O(n log n)
    TRACE("{" << src << ", " << dst << ", " << left << ", " << right << "}");
    //Check our bounds so that our math checks out.
    if((right - left) <= 1) return; //BASE CASE (Recursion inbound)... :^) //O(1)
	
    //Split our array into two.
    int middle = (left + right) / 2;
    RecursiveMergeSort(dst, src, left, middle);
    RecursiveMergeSort(dst, src, middle, right);

    //Merge ourselves back together.
    Merge(src, dst, left, middle, right);
    RETURN();
}

//Executes MergeSort on a given array of a given size using either the x or y axis.
void MergeSort(Point** src, int size, bool sort_using_x) { //O(n log n)
    TRACE("{" << src << ", " << size << ", " << sort_using_x << "}");
    //Create a work array to work with so we don't need multiple malloc's.
    //This was the trick I used in my first Programming Project for this class.
    Point** dst = new Point*[size];
    for(int i = 0; i < size; i++){
        dst[i] = src[i];
    }

    //Set whether or not we want to sort using the X or Y coordinate.
    bSortByX = sort_using_x;

    //Sort our array back into our source array and delete our working array.
    RecursiveMergeSort(dst, src, 0, size);
    delete dst;
    RETURN();
}

//Executes MergeSort on a given array of a given subset using either the x or y axis.
Point** MergeSort(Point** src, int left, int right, bool sort_using_x){ //O(n log n)
    TRACE("{" << src << ", " << left << ", " << right << ", " << sort_using_x << "}");

    //Obtain our array size.
    int size = right - left;

    //Construct two new arrays.
    Point** dst = new Point*[size];
    Point** tmp = new Point*[size];

    //Fill up both arrays with data for merge sort to perform.
    for(int i = 0; i < size; i++){
        tmp[i] = src[left + i];
        dst[i] = src[left + i];
    }

    //Tell mergesort if we are sorting by X (bad cause not threadsafe but we only have one thread so its reasonable).
    bSortByX = sort_using_x;

    //Call the actual MergeSort function.
    RecursiveMergeSort(tmp, dst, 0, size);

    //Delete the temporary buffer.
    delete tmp;

    //Return with the now sorted y array.
    RETURN();
    return dst;
}

//A small structure used to contain the results of our ClosestPair function (so we can return multiple data elements).
struct Results {
public:
    //The distance of the two points.
    double distance = INFINITY;
    //The first point.
    Point* p1 = NULL;
    //The second point.
    Point* p2 = NULL;

public:
    //A usefull function for printing out the results in a simple one lined format.
    friend std::ostream& operator<<(std::ostream& o, const Results& r){
        o << "(" << r.distance << ", " << (r.p1 ? *r.p1 : Point::INFINITE()) << ", " << (r.p2 ? *r.p2 : Point::INFINITE()) << ")";
        return o; 
    }
};

//An algorithm used to find the closest pair of two points given an array of a given size and places the calculated info into a structure called Results.
void ClosestPair(Point** lpPoints, int nPoints, Results& results) {
    TRACE("{" << lpPoints << ", " << nPoints << ", " << results << "}");
    //Base cases
    if(nPoints <= 3) { //If we only have two points then we have a simple distance algorithm.

        //Brute force algorithm O(1).
        results.distance = INFINITY;
        for(int i = 0; i < 3; i++) { //O(1)
            for(int j = i+1; i < 3; i++){ //O(1)
                //Check if we are within bounds.
                if(i >= nPoints) break;
                if(j >= nPoints) break;

                //Calculate the distance and take the lesser of the two.
                double dist = lpPoints[i]->distance(*lpPoints[j]);
                if(dist < results.distance){
                    results.distance = dist;
                    results.p1 = lpPoints[i];
                    results.p2 = lpPoints[j];
                }
            }
        }
        RETURN();
        return;
    }
    
    //Compute separation line L such that half the points are on
    // one side and half on the other side.
    int sep = nPoints / 2;

    //Calculate the cost (or the distance) between the left half and right half.
    double cost = INFINITY;
    {

        //Split the array into two and attempt to solve the ClosestPair
        Results left_result;
        Results right_result;
        ClosestPair(lpPoints, sep, left_result);
        ClosestPair(lpPoints + sep, nPoints - sep, right_result);
        
        //Calculate the cost now that we have actually performed the left and right halfs.
        cost = min(left_result.distance, right_result.distance);
        if(left_result.distance < right_result.distance) {
            results.p1 = left_result.p1;
            results.p2 = left_result.p2;
        }
        else {
            results.p1 = right_result.p1;
            results.p2 = right_result.p2;
        }
        results.distance = cost;
    }

    //Ignore all points further than the calculated cost.

    //Set our default left and right index to be one less or one more than our seperator.
    int left_index = sep-1;
    int right_index = sep+1;

    //Determine our upper and lower bounds based on the cost.
    int lower_bounds = lpPoints[sep]->x() - cost;
    int upper_bounds = lpPoints[sep]->x() + cost; 

    //Attempt to shift our left and right index over to collect every X within the upper and lower bounds.
    for(int i = 0; i < sep; i++){
        //Check if we have reached the end of our array and if not then lets check if point is within our lower bounds.
        if(left_index >= 0 && lpPoints[left_index]->x() >= lower_bounds)
            left_index--;
        //Check if we have reached the upper end of our array and if not then lets check if point is within our upper bounds.
        if(right_index < nPoints && lpPoints[right_index]->x() <= upper_bounds)
            right_index++;
    }

    //Increment our left index by one (done since left always extends one past the actual end).
    left_index++;
    //right_index--; //We cannot do this, because MergeSort a few lines down requires us to be one above what were asking it to sort.
    
    //Check if we actually end up in a situation where we have no points to actually check through.
    if(left_index >= right_index) { //We don't have any nodes nearby so we have nothing left to do.
        RETURN();
        return;
    }

    //Sort remaining points by y-coordinate.
    Point** y_sorted = MergeSort(lpPoints, left_index, right_index, false);
    int remaining = right_index - left_index;

    //Scan points in y-order and compare distance between each point
    // and next 11 neighbors. If any of these distances is less than
    // the calculated cost, update the calculated cost.
    for(int i = 0; i < remaining; i++){ //O(n)
        for(int j = 0; j < 11; j++){ //O(1)
            //We will exceed the bounds of our array so lets break out early.
            if(j >= remaining) break;
            //We will exceed the minimum cost, which since we are sorted by y value 
            // means our further nodes will also exceed the bounds. So break early.
            if((y_sorted[j]->y() - y_sorted[i]->y()) >= cost) break;

            //Finally calculate the possible new distance.
            double dist = y_sorted[i]->distance(*y_sorted[j]);
            if(dist < cost){
                results.p1 = y_sorted[i];
                results.p2 = y_sorted[j];
                cost = dist;
            }
        }
    }

    //Cleanup
    delete y_sorted;

    //Return the calculated cost.
    results.distance = cost;
    RETURN();
    return;
}

//Our main function (this is where the computer will begin executing our code).
int main(int argc, char* argv[]) {
    //Check our command line arguments for a filename.
    if(argc < 2){
        //No filename, print error usage message.
        std::cerr << "FATAL: Usage " << argv[0] << " filename" << std::endl << "\t'filename' must be specified!" << std::endl;
        return -1;
    }

    //Open the stream (assume it will always succeed).
    std::ifstream file(argv[1]);

    //Obtain the number of points in our file.
    int nPoints = 0;
    file >> nPoints;

    //Construct a new points array and fill it with our actual points.
    Point** lpPoints = new Point*[nPoints];
    for(int i = 0; i < nPoints; i++){
        lpPoints[i] = new Point();
        file >> *lpPoints[i];
    }

    //Sort our array by our X values.
    MergeSort(lpPoints, nPoints, true);

    //Obtain the closest pair of points.
    Results results;
    ClosestPair(lpPoints, nPoints, results);

    //Output the results to the screen.
    std::cout << "Min Distance: " << results.distance << std::endl;
    std::cout << "Between [" << *results.p1 << "] and [" << *results.p2 << "]" << std::endl;

    //The following is only needed for testing (uncomment the above __TEST_FOR_CORRECT__ define macro).
#ifdef __TEST_FOR_CORRECT__
    {
        Results r;
        for(int i = 0; i < nPoints; i++){
            for(int j = i + 1; j < nPoints; j++){
                double d = lpPoints[i]->distance(*lpPoints[j]);
                if(d < r.distance){
                    r.distance = d;
                    r.p1 = lpPoints[i];
                    r.p2 = lpPoints[j];
                }
            }
        }
        std::cout << "Answer O(n^2): " << r << std::endl;
    }
#endif

    //We have succeeded so return zero for success.
    return 0;
}