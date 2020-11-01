#include "transform-3d.hpp"
#include "support-3d.hpp"

void Transform3d::translate(Point3d *pct, float tx, float ty, float tz) {
    pct->set(pct->x + tx, pct->y + ty, pct->z + tz);
}

void Transform3d::rotateX(Point3d *pct, float angleInRadians) {
    pct->set(pct->x,
             pct->y * cos(angleInRadians) - pct->z * sin(angleInRadians),
             pct->y * sin(angleInRadians) + pct->z * cos(angleInRadians));
}
void Transform3d::rotateY(Point3d *pct, float angleInRadians) {
    pct->set(pct->x * cos(angleInRadians) - pct->z * sin(angleInRadians),
             pct->y,
             pct->x * sin(angleInRadians) + pct->z * cos(angleInRadians));
}
void Transform3d::rotateZ(Point3d *pct, float angleInRadians) {
    pct->set(pct->x * cos(angleInRadians) - pct->y * sin(angleInRadians),
             pct->x * sin(angleInRadians) + pct->y * cos(angleInRadians),
             pct->z);
}

void Transform3d::rotateXRelativeToAnotherPoint(Point3d *pct,
                                                Point3d *ref,
                                                float angleInRadians) {
    pct->translate(-(ref->x), -(ref->y), -(ref->z));
    ref->translate(-(ref->x), -(ref->y), -(ref->z));
    pct->rotateXRelativeToOrigin(angleInRadians);
    pct->translate(ref->x, ref->y, ref->z);
    ref->translate(ref->x, ref->y, ref->z);
}
void Transform3d::rotateYRelativeToAnotherPoint(Point3d *pct,
                                                Point3d *ref,
                                                float angleInRadians) {
    pct->translate(-(ref->x), -(ref->y), -(ref->z));
    ref->translate(-(ref->x), -(ref->y), -(ref->z));
    pct->rotateYRelativeToOrigin(angleInRadians);
    pct->translate(ref->x, ref->y, ref->z);
    ref->translate(ref->x, ref->y, ref->z);
}
void Transform3d::rotateZRelativeToAnotherPoint(Point3d *pct,
                                                Point3d *ref,
                                                float angleInRadians) {
    pct->translate(-(ref->x), -(ref->y), -(ref->z));
    ref->translate(-(ref->x), -(ref->y), -(ref->z));
    pct->rotateZRelativeToOrigin(angleInRadians);
    pct->translate(ref->x, ref->y, ref->z);
    ref->translate(ref->x, ref->y, ref->z);
}
void Transform3d::scale(Point3d *pct, float sx, float sy, float sz) {
    pct->set(pct->x * sx, pct->y * sy, pct->z * sz);
}
void Transform3d::scaleRelativeToAnotherPoint(
    Point3d *pct, Point3d *ref, float sx, float sy, float sz) {
    pct->translate(-(ref->x), -(ref->y), -(ref->z));
    ref->translate(-(ref->x), -(ref->y), -(ref->z));
    pct->scale(sx, sy, sz);
    pct->translate(ref->x, ref->y, ref->z);
    ref->translate(ref->x, ref->y, ref->z);
}
