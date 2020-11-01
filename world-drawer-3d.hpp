#pragma once

#include "headers-and-defines.hpp"
#include "support-3d.hpp"

class WorldDrawer3d {
public:
    WorldDrawer3d(int argc,
                  char **argv,
                  int windowWidth,
                  int windowHeight,
                  int windowStartX,
                  int windowStartY,
                  std::string windowName);
    ~WorldDrawer3d();
    static void run();
    static void displayCallbackFunction();
    static void reshapeCallbackFunction(int width, int height);
    static void idleCallbackFunction();
    static void keyboardCallbackFunction(unsigned char key, int x, int y);
    static void keyboardSpecialCallbackFunction(int key, int x, int y);
    static void mouseCallbackFunction(int button, int state, int x, int y);
    static void mouseMotionCallbackFunction(int x, int y);
    static void init();
    static void onIdle();
    static void onKey(unsigned char key);
    static void onClick(int button, int state, int x, int y);
    static void onMove(int x, int y);

public:
    static bool animation;
    static CoordinateSystem3d cs_basis;
    static std::vector<CoordinateSystem3d *> cs_used;
};
