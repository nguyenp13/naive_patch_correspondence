
#pragma once

#ifndef NAIVE_PATCH_CORRESPONDENCE_H
#define NAIVE_PATCH_CORRESPONDENCE_H

#include "util.h"
#include "array.h"

#define Y_COORD 0
#define X_COORD 1
#define D_COORD 2

int patch_SSD(
            const Array<byte> &A, 
            const Array<byte> &B, 
            const int &ax, 
            const int &ay, 
            const int &bx, 
            const int &by, 
            const int &patch_dim
        ) {
    double score = 0;
    int channels = A.channels();
    for(int dy=0;dy<patch_dim;dy++) {
        for(int dx=0;dx<patch_dim;dx++) {
            for(int z=0;z<channels;z++) {
                score += SQUARE(A(ay+dy,ax+dx,z)-B(by+dy,bx+dx,z));
            }
        }
    }
    return score;
}

void naive_patch_corresopndence(
            const Array<byte> &A, 
            const Array<byte> &B, 
            Array<int> &Ann, 
            const int &A_height, 
            const int &A_width, 
            const int &B_height, 
            const int &B_width, 
            const int &Ann_height, 
            const int &Ann_width, 
            const int &patch_dim, 
            long &total_patch_distance, 
            double &mean_patch_distance
        ) {
    
    #pragma omp parallel for num_threads(8)
    for(int ay=0; ay<Ann_height; ay++) {
        for(int ax=0; ax<Ann_width; ax++) {
            Ann(ay,ax,D_COORD) = patch_SSD(A, B, ax, ay, 0, 0, patch_dim);
            for(int by=0; by<B_height; by++) {
                for(int bx=0; bx<B_width; bx++) {
                    int old_patch_distance = Ann(ay,ax,D_COORD);
                    int new_patch_distance = patch_SSD(A, B, ax, ay, bx, by, patch_dim);
                    if (new_patch_distance<old_patch_distance) {
                        Ann(ay,ax,Y_COORD) = by;
                        Ann(ay,ax,X_COORD) = bx;
                        Ann(ay,ax,D_COORD) = new_patch_distance;
                    }
                }
            }
        }
    }
    
    total_patch_distance = 0;
    // Calculate total and mean patch distances
    for(int y=0; y<Ann_height; y++) { 
        for(int x=0; x<Ann_width; x++) { 
            total_patch_distance += LONG(Ann(y,x,D_COORD));
        }
    }
    mean_patch_distance = DOUBLE(total_patch_distance)/DOUBLE(Ann_height*Ann_width);
}

#endif // NAIVE_PATCH_CORRESPONDENCE_H

