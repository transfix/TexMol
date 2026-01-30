#ifndef __INTERSECT_H__
#define __INTERSECT_H__

#include <SecondaryStructures/util.h>
#include <SecondaryStructures/datastruct_ss.h>

bool does_intersect_ray3_seg3_in_plane(const SecondaryStructures::Ray_3& r, const SecondaryStructures::Segment& s);
SecondaryStructures::Point intersect_ray3_seg3(const SecondaryStructures::Ray_3& r, const SecondaryStructures::Segment& s, bool& is_correct_intersection);
bool does_intersect_convex_polygon_segment_3_in_3d(const vector<SecondaryStructures::Point>& conv_poly, const SecondaryStructures::Segment& s);

#endif
