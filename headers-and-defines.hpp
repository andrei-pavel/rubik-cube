#pragma once

#include <GL/freeglut.h>
#include <cstdlib>
#include <iostream>

#include "support-3d.hpp"

// Changeable definitions
#define ROTATION_SPEED 0.01f
#define ZOOM_SPEED 10
#define GRADIENT 64  // used for hopping and decoloration, must be GE 8
#define DECOLOR_RATE 0.1f
#define CUBE_COLOR 0, 0, 0
#define N_OF_MOVES_TO_SOLVING_STATE 5
#define MAX_ZOOM_LEVEL 29
#define ROTATION_RESISTANCE 20
#define MIN_LIMIT 99  // increment this in definition if selection fails
#define MAX_LIMIT 101  // decrement this in definition if selection fails
#define FORWARD HALF_PI
#define BACKWARDS -HALF_PI
#define HOP false
// Colors:
#define MIXING_BG_COLOR 0.3f, 0.2f, 0.2f
#define SOLVING_BG_COLOR 0.2f, 0.3f, 0.2f
#define DONE_BG_COLOR 0.2f, 0.2f, 0.3f
#define WHITE_COLOR 1, 1, 1
#define GREEN_COLOR 0.27f, 0.73f, 0.43f
#define RED_COLOR 1, 0.2f, 0.2f
#define ORANGE_COLOR 0.99f, 0.51f, 0.2f
#define BLUE_COLOR 0, 0.59f, 0.91f
#define YELLOW_COLOR 1, 1, 0

// Unchangeable definitions
#define PI 3.14159265359f
#define HALF_PI 1.570796326795f
#define ZOOM_IN_SPEED ZOOM_SPEED, ZOOM_SPEED, ZOOM_SPEED
#define ZOOM_OUT_SPEED -ZOOM_SPEED, -ZOOM_SPEED, -ZOOM_SPEED
#define COORD_CENTER 80
#define COORD_MARGIN1 120
#define COORD_MARGIN2 280
#define LABEL_WIDTH1 301
#define LABEL_WIDTH2 302
#define ON_X 0
#define ON_Y 1
#define ON_Z 2
/* Old defines:
#define ON_X 1, 9, 0
#define ON_Y 1, 3, 6
#define ON_Z 3, 1, 0
*/
// Keys:
#define KEY_ESC 0x1B
#define KEY_SPACE 0x20
#define KEY_A_CAPITAL 0x41
#define KEY_D_CAPITAL 0x44
#define KEY_I_CAPITAL 0x49
#define KEY_J_CAPITAL 0x4a
#define KEY_K_CAPITAL 0x4b
#define KEY_L_CAPITAL 0x4c
#define KEY_M_CAPITAL 0x4d
#define KEY_N_CAPITAL 0x4e
#define KEY_O_CAPITAL 0x4f
#define KEY_P_CAPITAL 0x50
#define KEY_Q_CAPITAL 0x51
#define KEY_S_CAPITAL 0x53
#define KEY_U_CAPITAL 0x55
#define KEY_W_CAPITAL 0x57
#define KEY_A 0x61
#define KEY_D 0x64
#define KEY_I 0x69
#define KEY_J 0x6a
#define KEY_K 0x6b
#define KEY_L 0x6c
#define KEY_M 0x6d
#define KEY_N 0x6e
#define KEY_O 0x6f
#define KEY_P 0x70
#define KEY_Q 0x71
#define KEY_S 0x73
#define KEY_U 0x75
#define KEY_W 0x77
