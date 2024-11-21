//Author: Andrew Kerr
//Date: 04/17/2022
//Note: There are many repeating commented function "names", this is simply because I tried to redo the same thing with multiple
//       variations as I went. I was unable to complete the project and hope you can at least see how much I struggled here :^)

//===[ INCLUDES ]==================================================================================================================
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <stdint.h>

//===[ MACROS ]====================================================================================================================
#define max(a, b) ((a) > (b) ? (a) : (b))
#define digit(chr) ((chr) - '0')

//===[ TYPEDEFS ]==================================================================================================================
typedef int64_t LargeNumber;

//===[ GLOBALS ]===================================================================================================================
//A global array for us to store our computed values for later usage.
LargeNumber arr[10][154] = {{0, }, };

//===[ FUNCTIONS ]=================================================================================================================

/**
 * 2D Array: rmax[inning][endchar#]
 * 
 * inning: 1 base number system, computers used base 0 so they start at zero.
 * 
 * score(inning, endchar#) gives the numeric value of digits.
 * max(a, b) returns greater of the two.
 *
 * endchar#: 
 *   endchar# = 0
 *   ADVANCED: for(; string[0]; endchar#++ && string++);
 *   BASIC: while(string[endchar#]) endchar#++;
 * 
 * Basecase: rmax[1][endchar#] = score(1, endchar#)
 * rmax[inning][endchar#] = max{score(cnum, endchar#) + rmax[inning - 1][cnum - 1], rmax[inning][endchar#]}
 *  
 * 
 * where 0 <= endchar - 17 <= cnum <= endchar#
 */


//Turn's a given substring interval into a numerical value.
LargeNumber score(const char* digits, int start, int end){
    LargeNumber res = 0;
    for(int i = start; i <= end; i++){
        res *= 10;
        res += digit(digits[i]);
    }
    //std::cout << "(" << end-iter << ", " << end << ")\t" << res << std::endl;
    
    return res;
}

/*LargeNumber solve_for_maximum(const char* digits, int len) {
    
    //Fill in our basecase row.
    for(int endpoint = 0; endpoint < 154 && endpoint < len; endpoint++){
        arr[0][endpoint] = score(digits, endpoint, endpoint);
    }

    //Loop through remaining row's and cascade the values as needed.
    for(int inning = 1; inning < 9; inning++){
        for(int end = inning; end < len; end++){
            int cnum = end - 17; //Backtrack 17 places.
            if(cnum < 0) cnum = 0;
            for(; cnum <= end; cnum++){
                arr[inning][end] = max(score(digits, cnum, end) + arr[inning-1][cnum-1], arr[inning][end]);
            }
        }
    }

    //Debug print function.
    for(int inning = 0; inning < 9; inning++){
        std::cout << inning << ")\n";
        for(int endpoint = 0; endpoint < 154 && endpoint < len; endpoint++){
            //if(endpoint < inning)
            //    std::cout << "ERR" << "\t\t";
            //else
            std::cout << arr[inning][endpoint] << "\t\t";
        }
        std::cout << "\n\n";
    }

    return arr[8][len-1];
}*/


LargeNumber solve_for_maximum(const char* digits, int len){
    if(len == 9){
        LargeNumber res = 0;
        for(int i = 0; i < 9; i++)
            res += digit(digits[i]);
        return res;
    }

    for(int end = 0; end < 17 && end < len; end++){
        arr[0][end] = score(digits, end, end);
    }

    /*for(int i = 1; i < 9; i++){
        for(int e = 1; e < len; e++){
            arr[i][e] = max(arr[i-1][e], score(digits, e-1, e) + arr[i-1][e-1]);
        }
    }*/

    /*for(int inning = 0; inning < 9; inning++){
        std::cout << inning << ")\n";
        for(int endpoint = 0; endpoint < 154 && endpoint < len; endpoint++){
            //if(endpoint < inning)
            //    std::cout << "ERR" << "\t\t";
            //else
            std::cout << arr[inning][endpoint] << "\t\t";
        }
        std::cout << "\n\n";
    }*/

    //Not going to lie, I knew I couldn't get this one and just wanted to put this here to at least get you to chuckle.
    std::cout << std::hex;
    return 0xDEADC0DE; //:^)
}

//Solves the given string of digits into the maximum possible subdivided scores.
/*LargeNumber solve_for_maximum(const char* digits, int len) {
    //If length is 9 then simply return the digits sum'ed together.
    if(len == 9){
        //Count the digits and return it.
        LargeNumber res = 0;
        for(int i = 0; i < 9; i++){
            res += digit(digits[i]);
        }
        return res;
    }

    //std::cout << "Len)\t" << len << std::endl;

    for(int endpoint = 1; endpoint < 154 && endpoint < len; endpoint++){
        arr[0][endpoint] = score(digits, 1, endpoint);
    }

    for(int inning = 1; inning < 9; inning++){
        for(int endpoint = inning; endpoint <= len; endpoint++) {
            int cnum = endpoint - 17;
            if(cnum < 0) cnum = 0;
            for(; cnum <= endpoint; cnum++) {
                arr[inning][endpoint] = max(score(digits, cnum, endpoint) + arr[inning-1][cnum-1], arr[inning][endpoint]);
            }
        }
    }

    for(int endpoint = 1; endpoint < 154 && endpoint < len; endpoint++){
        arr[0][endpoint-1] = score(digits, 1, endpoint);
    }

    for(int endpoint = 2; endpoint < 154 && endpoint < len; endpoint++){
        
    }
    
    //Testing for debug only!
    for(int inning = 0; inning < 9; inning++){
        std::cout << inning << ")\n";
        for(int endpoint = 0; endpoint < 154 && endpoint < len; endpoint++){
            //if(endpoint < inning)
            //    std::cout << "ERR" << "\t\t";
            //else
            std::cout << arr[inning][endpoint] << "\t\t";
        }
        std::cout << "\n\n";
    }

    //Return the bottom right value.
    return arr[8][len-1];
}*/

/*LargeNumber solve(const char* digits, int inning, int endpoint){
    if(inning == 0) {
        set[0][endpoint] = true;
        return arr[0][endpoint] = score(digits, inning, endpoint);
    }
    if(set[inning][endpoint])
        return arr[inning][endpoint];

    int cnum = endpoint - 17;
    if(cnum < 0) cnum = 0;
    for(; cnum <= endpoint; cnum++){
        arr[inning+1][endpoint+1] = max(solve(digits, cnum, endpoint) + arr[inning][cnum], arr[inning+1][endpoint+1]);
    }
    set[inning][endpoint] = true;
    return arr[inning][endpoint];
}

LargeNumber solve_for_maximum(const char* digits, int len) {
    if(len == 9){
        LargeNumber res = 0;
        for(int i = 0; i < 9; i++){
            res += digit(digits[i]);
        }
        return res;
    }
    for(int inning = 0; inning < 9; inning++){
        for(int endpoint = inning+1; endpoint < 154 && endpoint <= len; endpoint++){
            std::cout << "\t(" << inning << ", " << endpoint << "): ";
            std::cout << solve(digits, inning, endpoint) << std::endl;
        }
        std::cout << std::endl;
    }
    return 0;
}*/

//Our main function (this is where the computer will begin executing our code).
int main(int argc, const char* argv[]) {

    //const char* original_string = "100002300";
    //const char* original_string = "10391242265"; //912 is largest.
    //const char* original_string = "90107060905040308888899201098765432107654321";

    const char* digits = argv[1];

    LargeNumber solution = solve_for_maximum(digits, strlen(digits));
    std::cout << "Total:" << solution << std::endl;
    
    return 0;
}