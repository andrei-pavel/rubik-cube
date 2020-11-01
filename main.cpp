#include "world-drawer-3d.hpp"

// Global variables.
bool WorldDrawer3d::animation;
CoordinateSystem3d *cs;
Object3d *cube, *label;
std::vector<Object3d *> all, whiteLabel, greenLabel, redLabel, orangeLabel,
    blueLabel, yellowLabel, selectedLabels, selectedCubes, cubes, scoreCubes;
unsigned int i, j, counter, iteration, rotateX, rotateY, rotateZ;
int currentX, currentY, zoom;
float nOfMoves;
bool change, goLeft, goRight, goForward, goBackwards, canRotate, mixingState,
    solvingState, readyState;
Object3d *getRectangularParallelipepiped(float coordX1,
                                         float coordX2,
                                         float coordY1,
                                         float coordY2,
                                         float coordZ1,
                                         float coordZ2) {

    float coords[] = {coordX1, coordX2, coordY1, coordY2, coordZ1, coordZ2};
    std::vector<Point3d> points;

    for (int i = 0; i < 8; i += 1)
        points.push_back(Point3d(coords[(int)floor((float)i / 4)],
                                 coords[(int)floor((float)i / 2) % 2 + 2],
                                 coords[i % 2 + 4]));

    std::vector<int> topology;
    topology.push_back(0);  // Bottom
    topology.push_back(1);
    topology.push_back(2);
    topology.push_back(2);
    topology.push_back(3);
    topology.push_back(1);
    topology.push_back(0);  // Back
    topology.push_back(1);
    topology.push_back(4);
    topology.push_back(4);
    topology.push_back(5);
    topology.push_back(1);
    topology.push_back(0);  // Left
    topology.push_back(2);
    topology.push_back(4);
    topology.push_back(4);
    topology.push_back(6);
    topology.push_back(2);
    topology.push_back(7);  // Front
    topology.push_back(3);
    topology.push_back(1);
    topology.push_back(1);
    topology.push_back(5);
    topology.push_back(7);
    topology.push_back(7);  // Right
    topology.push_back(3);
    topology.push_back(6);
    topology.push_back(6);
    topology.push_back(4);
    topology.push_back(3);
    topology.push_back(7);  // Top
    topology.push_back(5);
    topology.push_back(6);
    topology.push_back(6);
    topology.push_back(4);
    topology.push_back(5);

    return new Object3d(points, topology);
}

void selectCubes(unsigned int c) {
    bool add;
    switch (c) {
    case ON_X:
        for (unsigned int i = 0; i < cubes.size(); i += 1) {
            add = true;
            for (unsigned int j = 0; j < 8; j += 1)
                if (!(cubes[i]->points[j].x >= -MAX_LIMIT &&
                          cubes[i]->points[j].x <= -MIN_LIMIT ||
                      cubes[i]->points[j].x >= -MIN_LIMIT &&
                          cubes[i]->points[j].x <= MAX_LIMIT))
                    add = false;
            if (add)
                selectedCubes.push_back(cubes[i]);
        }
        break;
    case ON_Y:
        for (unsigned int i = 0; i < cubes.size(); i += 1) {
            add = true;
            for (unsigned int j = 0; j < 8; j += 1)
                if (!(cubes[i]->points[j].y >= -MAX_LIMIT &&
                          cubes[i]->points[j].y <= -MIN_LIMIT ||
                      cubes[i]->points[j].y >= -MIN_LIMIT &&
                          cubes[i]->points[j].y <= MAX_LIMIT))
                    add = false;
            if (add)
                selectedCubes.push_back(cubes[i]);
        }
        break;
    case ON_Z:
        for (unsigned int i = 0; i < cubes.size(); i += 1) {
            add = true;
            for (unsigned int j = 0; j < 8; j += 1)
                if (!(cubes[i]->points[j].z >= -MAX_LIMIT &&
                          cubes[i]->points[j].z <= -MIN_LIMIT ||
                      cubes[i]->points[j].z >= -MIN_LIMIT &&
                          cubes[i]->points[j].z <= MAX_LIMIT))
                    add = false;
            if (add)
                selectedCubes.push_back(cubes[i]);
        }
        break;
    default:
        break;
    }
}

void rotateLayer(float angleRad) {
    for (unsigned int i = 0; i < selectedLabels.size(); i += 1) {
        selectedLabels[i]->rotateXRelativeToPoint(cs->axiscenter,
                                                  rotateX * angleRad);
        selectedLabels[i]->rotateYRelativeToPoint(cs->axiscenter,
                                                  rotateY * angleRad);
        selectedLabels[i]->rotateZRelativeToPoint(cs->axiscenter,
                                                  rotateZ * angleRad);
    }
    for (unsigned int i = 0; i < selectedCubes.size(); i += 1) {
        selectedCubes[i]->rotateXRelativeToPoint(cs->axiscenter,
                                                 rotateX * angleRad);
        selectedCubes[i]->rotateYRelativeToPoint(cs->axiscenter,
                                                 rotateY * angleRad);
        selectedCubes[i]->rotateZRelativeToPoint(cs->axiscenter,
                                                 rotateZ * angleRad);
    }
}

void selectLabels() {
    for (unsigned int i = 0; i < selectedCubes.size(); i += 1) {
        if (selectedCubes[i]->neighborX->isLabel)
            selectedLabels.push_back(selectedCubes[i]->neighborX);
        if (selectedCubes[i]->neighborY->isLabel)
            selectedLabels.push_back(selectedCubes[i]->neighborY);
        if (selectedCubes[i]->neighborZ->isLabel)
            selectedLabels.push_back(selectedCubes[i]->neighborZ);
        if (selectedCubes[i]->neighborMinusX->isLabel)
            selectedLabels.push_back(selectedCubes[i]->neighborMinusX);
        if (selectedCubes[i]->neighborMinusY->isLabel)
            selectedLabels.push_back(selectedCubes[i]->neighborMinusY);
        if (selectedCubes[i]->neighborMinusZ->isLabel)
            selectedLabels.push_back(selectedCubes[i]->neighborMinusZ);
    }
}

bool correct() {
    for (i = 0; i < 9; i += 1) {
        if (i >= 18 &&
                !(cubes[i]->neighborX->points[0].x >= LABEL_WIDTH1 - 1 &&
                  cubes[i]->neighborX->points[0].x <= LABEL_WIDTH2 + 1) ||
            i % 9 >= 6 &&
                !(cubes[i]->neighborY->points[0].y >= LABEL_WIDTH1 - 1 &&
                  cubes[i]->neighborY->points[7].y <= LABEL_WIDTH2 + 1) ||
            i % 3 >= 2 &&
                !(cubes[i]->neighborZ->points[0].z >= LABEL_WIDTH1 - 1 &&
                  cubes[i]->neighborZ->points[7].z <= LABEL_WIDTH2 + 1) ||
            i <= 9 &&
                !(cubes[i]->neighborMinusX->points[0].x >= -LABEL_WIDTH2 - 1 &&
                  cubes[i]->neighborMinusX->points[7].x <= -LABEL_WIDTH1 + 1) ||
            i % 9 <= 2 &&
                !(cubes[i]->neighborMinusY->points[0].y >= -LABEL_WIDTH2 - 1 &&
                  cubes[i]->neighborMinusY->points[7].y <= -LABEL_WIDTH1 + 1) ||
            i % 3 == 0 &&
                !(cubes[i]->neighborMinusZ->points[0].z >= -LABEL_WIDTH2 - 1 &&
                  cubes[i]->neighborMinusZ->points[7].z <= -LABEL_WIDTH1 + 1))
            return false;
    }
    return true;
}

void WorldDrawer3d::init() {

    // Initializations
    cs = new CoordinateSystem3d();
    cs_used.push_back(cs);
    rotateX = 0;
    rotateY = 0;
    rotateZ = 1;
    animation = true;
    change = false;
    canRotate = false;
    goLeft = false;
    goRight = false;
    goForward = false;
    goBackwards = false;
    iteration = 0;
    nOfMoves = 0;
    zoom = 0;
    mixingState = true;
    solvingState = false;
    readyState = false;

    // Colored labels
    float coords[] = {-COORD_MARGIN2, -COORD_MARGIN1, -COORD_CENTER,
                      COORD_CENTER,   COORD_MARGIN1,  COORD_MARGIN2};
    int lowRate, fastRate;
    for (i = 0; i < 9; i += 1) {

        whiteLabel.push_back(label);
        greenLabel.push_back(label);
        redLabel.push_back(label);
        orangeLabel.push_back(label);
        blueLabel.push_back(label);
        yellowLabel.push_back(label);

        lowRate = 2 * (int)floor((float)i / 3);
        fastRate = 2 * (i % 3);

        whiteLabel[i] = getRectangularParallelipepiped(
            coords[lowRate], coords[lowRate + 1], LABEL_WIDTH1, LABEL_WIDTH2,
            coords[fastRate], coords[fastRate + 1]);
        greenLabel[i] = getRectangularParallelipepiped(
            coords[lowRate], coords[lowRate + 1], coords[fastRate],
            coords[fastRate + 1], LABEL_WIDTH1, LABEL_WIDTH2);
        redLabel[i] = getRectangularParallelipepiped(
            LABEL_WIDTH1, LABEL_WIDTH2, coords[lowRate], coords[lowRate + 1],
            coords[fastRate], coords[fastRate + 1]);
        orangeLabel[i] = getRectangularParallelipepiped(
            -LABEL_WIDTH2, -LABEL_WIDTH1, coords[lowRate], coords[lowRate + 1],
            coords[fastRate], coords[fastRate + 1]);
        blueLabel[i] = getRectangularParallelipepiped(
            coords[lowRate], coords[lowRate + 1], coords[fastRate],
            coords[fastRate + 1], -LABEL_WIDTH2, -LABEL_WIDTH1);
        yellowLabel[i] = getRectangularParallelipepiped(
            coords[lowRate], coords[lowRate + 1], -LABEL_WIDTH2, -LABEL_WIDTH1,
            coords[fastRate], coords[fastRate + 1]);

        whiteLabel[i]->isLabel = true;
        greenLabel[i]->isLabel = true;
        redLabel[i]->isLabel = true;
        orangeLabel[i]->isLabel = true;
        blueLabel[i]->isLabel = true;
        yellowLabel[i]->isLabel = true;

        whiteLabel[i]->setcolor(WHITE_COLOR);
        greenLabel[i]->setcolor(GREEN_COLOR);
        redLabel[i]->setcolor(RED_COLOR);
        orangeLabel[i]->setcolor(ORANGE_COLOR);
        blueLabel[i]->setcolor(BLUE_COLOR);
        yellowLabel[i]->setcolor(YELLOW_COLOR);

        cs->objectAdd(whiteLabel[i]);
        cs->objectAdd(greenLabel[i]);
        cs->objectAdd(redLabel[i]);
        cs->objectAdd(orangeLabel[i]);
        cs->objectAdd(blueLabel[i]);
        cs->objectAdd(yellowLabel[i]);
    }

    // The black cubes
    float dX, dY, dZ;
    for (i = 0; i < 27; i += 1) {

        dX = 200 * floor((float)i / 9);
        dY = 200 * floor((float)(i % 9) / 3);
        dZ = 200 * (float)(i % 3);

        cubes.push_back(cube);
        cubes[i] = getRectangularParallelipepiped(-300.0f + dX, -100.0f + dX,
                                                  -300.0f + dY, -100.0f + dY,
                                                  -300.0f + dZ, -100.0f + dZ);
        cubes[i]->setcolor(CUBE_COLOR);
        cs->objectAdd(cubes[i]);
    }

    // Getting the cubes to know the neighbors.
    cube = getRectangularParallelipepiped(-1, 1, -1, 1, -1, 1);
    cube->neighborX = cube;
    cube->neighborY = cube;
    cube->neighborZ = cube;
    cube->neighborMinusX = cube;
    cube->neighborMinusY = cube;
    cube->neighborMinusZ = cube;

    for (i = 0; i < 27; i += 1) {
        cubes[i]->neighborX = cube;
        cubes[i]->neighborX = cube;
        cubes[i]->neighborX = cube;
        cubes[i]->neighborMinusX = cube;
        cubes[i]->neighborMinusY = cube;
        cubes[i]->neighborMinusZ = cube;
    }

    for (i = 0; i < cubes.size(); i += 1) {
        if (i < 18)
            cubes[i]->neighborX = cubes[i + 9];
        else  // i >= 18
            cubes[i]->neighborX = redLabel[i - 18];
        if (i % 9 < 6)
            cubes[i]->neighborY = cubes[i + 3];
        else  // i % 9 >= 6
            cubes[i]->neighborY =
                whiteLabel[i % 3 + 3 * (unsigned int)floor((float)i / 9)];
        if (i % 3 < 2)
            cubes[i]->neighborZ = cubes[i + 1];
        else  // i % 3 >= 2
            cubes[i]->neighborZ = greenLabel[(i - 2) / 3];
        if (i >= 9)
            cubes[i]->neighborMinusX = cubes[i - 9];
        else  // i <= 9
            cubes[i]->neighborMinusX = orangeLabel[i];
        if (i % 9 >= 3)
            cubes[i]->neighborMinusY = cubes[i - 3];
        else  // i % 9 <= 2
            cubes[i]->neighborMinusY =
                yellowLabel[i - 6 * (unsigned int)floor((float)i / 9)];
        if (i % 3 >= 1)
            cubes[i]->neighborMinusZ = cubes[i - 1];
        else  // i % 3 == 0
            cubes[i]->neighborMinusZ = blueLabel[i / 3];
    }

    // Cubes and selected labels
    selectCubes(ON_Z);
    selectLabels();
}

// Per frame.
void WorldDrawer3d::onIdle() {
    float y, decolor, cut;
    if (animation) {
        cut = (float)(iteration % GRADIENT);
        if (cut < GRADIENT / 2)
            decolor = -cut * DECOLOR_RATE / GRADIENT;
        if (cut >= GRADIENT / 2)
            decolor = (cut - GRADIENT / 2) * DECOLOR_RATE / GRADIENT;
        if (cut < GRADIENT / 4)
            y = (GRADIENT / 4 - 1 - cut) / (GRADIENT / 4 - 1);
        if (cut >= GRADIENT / 4 && cut < GRADIENT / 2)
            y = (GRADIENT / 4 - cut) / (GRADIENT / 4 - 1);
        if (cut >= GRADIENT / 2 && cut < 3 * GRADIENT / 4)
            y = (cut - 3 * GRADIENT / 4) / (GRADIENT / 4 - 1);
        if (cut >= 3 * GRADIENT / 4 && cut < GRADIENT)
            y = (cut - 3 * GRADIENT / 4) / (GRADIENT / 4 - 1);

        for (i = 0; i < selectedLabels.size(); i += 1) {
            selectedLabels[i]->setcolor(selectedLabels[i]->colorx + decolor,
                                        selectedLabels[i]->colory + decolor,
                                        selectedLabels[i]->colorz + decolor);
        }

        for (i = 0; i < selectedCubes.size(); i += 1) {
            selectedCubes[i]->setcolor(selectedCubes[i]->colorx - decolor / 10,
                                       selectedCubes[i]->colory - decolor / 10,
                                       selectedCubes[i]->colorz - decolor / 10);
        }
        if (goForward) {
            counter += 1;
            rotateLayer(FORWARD / ROTATION_RESISTANCE);
            if (counter >= ROTATION_RESISTANCE)
                goForward = false;
        }
        if (goBackwards) {
            counter += 1;
            rotateLayer(BACKWARDS / ROTATION_RESISTANCE);
            if (counter >= ROTATION_RESISTANCE)
                goBackwards = false;
        }
        if (cut == 0) {
            if (change) {
                selectedLabels.clear();
                selectedCubes.clear();
                if (rotateX == 1 && change) {
                    rotateX = 0;
                    rotateY = 1;
                    selectCubes(ON_Y);
                    selectLabels();
                    change = false;
                }
                if (rotateY == 1 && change) {
                    rotateY = 0;
                    rotateZ = 1;
                    selectCubes(ON_Z);
                    selectLabels();
                    change = false;
                }
                if (rotateZ == 1 && change) {
                    rotateZ = 0;
                    rotateX = 1;
                    selectCubes(ON_X);
                    selectLabels();
                    change = false;
                }
            }
            if (goLeft) {
                selectedLabels.clear();
                for (i = 0; i < selectedCubes.size(); i += 1) {
                    if (rotateX == 1 && !selectedCubes[i]->neighborX->isLabel)
                        selectedCubes[i] = selectedCubes[i]->neighborX;
                    if (rotateY == 1 && !selectedCubes[i]->neighborY->isLabel)
                        selectedCubes[i] = selectedCubes[i]->neighborY;
                    if (rotateZ == 1 && !selectedCubes[i]->neighborZ->isLabel)
                        selectedCubes[i] = selectedCubes[i]->neighborZ;
                }
                selectLabels();
                goLeft = false;
            }
            if (goRight) {
                selectedLabels.clear();
                for (i = 0; i < selectedCubes.size(); i += 1) {
                    if (rotateX == 1 &&
                        !selectedCubes[i]->neighborMinusX->isLabel)
                        selectedCubes[i] = selectedCubes[i]->neighborMinusX;
                    if (rotateY == 1 &&
                        !selectedCubes[i]->neighborMinusY->isLabel)
                        selectedCubes[i] = selectedCubes[i]->neighborMinusY;
                    if (rotateZ == 1 &&
                        !selectedCubes[i]->neighborMinusZ->isLabel)
                        selectedCubes[i] = selectedCubes[i]->neighborMinusZ;
                }
                selectLabels();
                goRight = false;
            }
        }
        if (mixingState && nOfMoves >= N_OF_MOVES_TO_SOLVING_STATE) {
            mixingState = false;
            solvingState = true;
            nOfMoves = 0;
            glClearColor(SOLVING_BG_COLOR, 1);
        }
        if (solvingState && correct()) {
            solvingState = false;
            readyState = true;
            glTranslatef(-4000, -4000, -4000);
            unsigned int dY, dZ,
                length = (unsigned int)ceil(sqrt((float)nOfMoves));
            for (i = 0; i < length; i += 1)
                for (j = 0; j < length; j += 1)
                    if (i * length + j < nOfMoves) {
                        dY = 100 * i;
                        dZ = 100 * j;
                        scoreCubes.push_back(cube);
                        scoreCubes[i * length + j] =
                            getRectangularParallelipepiped(-20, 20, -20, 20,
                                                           -20, 20);
                        scoreCubes[i * length + j]->setcolor(1, 1, 1);
                        scoreCubes[i * length + j]->translate(
                            4000, 4500 - (float)dY, 4500 - (float)dZ);
                        cs->objectAdd(scoreCubes[i * length + j]);
                    }
            glClearColor(DONE_BG_COLOR, 1);
        }

        if (HOP)
            cs->translate(0, y, 0);
        iteration += 1;
    }
}

void WorldDrawer3d::onKey(unsigned char key) {
    switch (key) {
    case KEY_Q:
    case KEY_Q_CAPITAL:
        std::cout << rotateX << " " << rotateY << " " << rotateZ << std::endl;
        // Shift Rotate.
        if (!readyState && !goForward && !goBackwards)
            change = true;
        break;
    case KEY_A:
    case KEY_A_CAPITAL:
        // Shift selector to left.
        if (!readyState && !goForward && !goBackwards)
            goLeft = true;
        break;
    case KEY_D:
    case KEY_D_CAPITAL:
        // Shift selector to right.
        if (!readyState && !goForward && !goBackwards)
            goRight = true;
        break;
    case KEY_W:
    case KEY_W_CAPITAL:
        // Rotate forward.
        if (!readyState && !goForward && !goBackwards) {
            goForward = true;
            nOfMoves += 1;
            counter = 0;
        }
        break;
    case KEY_S:
    case KEY_S_CAPITAL:
        // Rotate backwards.
        if (!readyState && !goForward && !goBackwards) {
            goBackwards = true;
            nOfMoves += 1;
            counter = 0;
        }
        break;
    case KEY_P:
    case KEY_P_CAPITAL:
        animation = !animation;
        break;
    case KEY_SPACE:
        if (readyState) {
            readyState = false;
            mixingState = true;
            scoreCubes.clear();
            nOfMoves = 0;
            glTranslatef(4000, 4000, 4000);
            glClearColor(MIXING_BG_COLOR, 1);
        }
    default:
        break;
    }
}

void WorldDrawer3d::onClick(int button, int state, int posx, int posy) {
    if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
        canRotate = true;
    else
        canRotate = false;
    if (button == 3) {
        // Zoom in.
        if (zoom < MAX_ZOOM_LEVEL) {
            glTranslatef(ZOOM_IN_SPEED);
            zoom += 1;
        }
    }
    if (button == 4) {
        // Zoom out.
        if (zoom > -MAX_ZOOM_LEVEL) {
            glTranslatef(ZOOM_OUT_SPEED);
            zoom -= 1;
        }
    }

    // Update the saved mouse position.
    currentX = posx;
    currentY = posy;
}

void WorldDrawer3d::onMove(int posx, int posy) {
    if (canRotate) {
        // Rotate all objects.
        glRotatef(PI, (currentX - posx - currentY + posy) * ROTATION_SPEED,
                  0.0f, (currentY - posy) * ROTATION_SPEED);
    }

    // Update the stored mouse position for later use.
    currentX = posx;
    currentY = posy;
}

int main(int argc, char **argv) {
    WorldDrawer3d wd3d(argc, argv, 0, 0, 200, 100, std::string("Rubik"));
    wd3d.init();
    wd3d.run();
    return 0;
}
