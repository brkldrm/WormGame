// A simple variant of the game Snake
//
// Used for teaching in classes
//
// Author:
// Franz Regensburger
// Ingolstadt University of Applied Sciences
// (C) 2011
//
// The worm model

#include <curses.h>
#include "worm.h"
#include "board_model.h"
#include "worm_model.h"

// Data defining the worm
int theworm_wormpos_y[WORM_LENGTH];  // y-coordinate of the worm's head
int theworm_wormpos_x[WORM_LENGTH];  // x-coordinate of the worm's head

int theworm_maxindex;
int theworm_headindex;

// The current heading of the worm
// These are offsets from the set {-1,0,+1}
int theworm_dx;
int theworm_dy;

int theworm_wcolor; 

// Initialize the worm
extern enum ResCodes initializeWorm(int len_max, int headpos_y, int headpos_x, enum WormHeading dir, enum ColorPairs color) {
    // Initialize indices
    theworm_maxindex = len_max - 1;
    theworm_headindex = 0;

    for (int i = 0; i <= theworm_maxindex; i++) {
        theworm_wormpos_y[i] = UNUSED_POS_ELEM;
        theworm_wormpos_x[i] = UNUSED_POS_ELEM;
    }

    // Initialize position of worms head
    theworm_wormpos_y[theworm_headindex] = headpos_y;
    theworm_wormpos_x[theworm_headindex] = headpos_x;

    // Initialize the heading of the worm
    setWormHeading(dir);

    // Initialze color of the worm
    theworm_wcolor = color;

    return RES_OK;
}

// Show the worms's elements on the display
// Simple version
extern void showWorm() {
    // Due to our encoding we just need to show the head element
    // All other elements are already displayed
    placeItem(
            theworm_wormpos_y[theworm_headindex],
            theworm_wormpos_x[theworm_headindex],
            SYMBOL_WORM_INNER_ELEMENT,
            theworm_wcolor);
}

extern void cleanWormTail() {
    int tailindex = (theworm_headindex + 1) % (theworm_maxindex + 1);
    if (theworm_wormpos_y[tailindex] != UNUSED_POS_ELEM) {
        placeItem(theworm_wormpos_y[tailindex],
                  theworm_wormpos_x[tailindex],
                  SYMBOL_FREE_CELL,
                  COLP_FREE_CELL);
    }
}

extern void moveWorm(enum GameStates* agame_state) {
    // Compute and store new head position according to current heading.
    int headpos_y = theworm_wormpos_y[theworm_headindex] + theworm_dy;
    int headpos_x = theworm_wormpos_x[theworm_headindex] + theworm_dx;

    // Check if we would leave the display if we move the worm's head according
    // to worm's last direction.
    // We are not allowed to leave the display's window.
    if (headpos_x < 0) {
        *agame_state = WORM_OUT_OF_BOUNDS;
    } else if (headpos_x > getLastCol()) { 
        *agame_state = WORM_OUT_OF_BOUNDS;
    } else if (headpos_y < 0) {  
        *agame_state = WORM_OUT_OF_BOUNDS;
    } else if (headpos_y > getLastRow()) {
        *agame_state = WORM_OUT_OF_BOUNDS;
    } else {
        // We will stay within bounds.
        if (isInUseByWorm(headpos_y, headpos_x)) {
            *agame_state = WORM_CROSSING;
        }
    }

    if (*agame_state == WORM_GAME_ONGOING) {
       theworm_headindex = (theworm_headindex + 1) % (theworm_maxindex + 1);
       theworm_wormpos_y[theworm_headindex] = headpos_y;
       theworm_wormpos_x[theworm_headindex] = headpos_x;
    }
}
extern void setWormHeading(enum WormHeading dir) {
  switch(dir) {
        case WORM_UP :// User wants up
            theworm_dx=0;
            theworm_dy=-1;
            break;
        case WORM_DOWN:// User wants down
            theworm_dx=0;
            theworm_dy=1;
            break;
        case WORM_LEFT:// User wants left
            theworm_dx=-1;
            theworm_dy=0;
            break;
        case WORM_RIGHT:// User wants right
            theworm_dx=1;
            theworm_dy=0;
            break;
   }
}
 

extern bool isInUseByWorm(int pos_y, int pos_x) {
    int i;
    bool collision = false;

    i = theworm_headindex;

    do {
        if (theworm_wormpos_y[i] == pos_y &&
            theworm_wormpos_x[i] == pos_x) {
            collision = true;
            break;
        }
        i = (i + theworm_maxindex) % (theworm_maxindex + 1);
    } while (i != theworm_headindex && theworm_wormpos_y[i] != UNUSED_POS_ELEM);

    return collision;
}

