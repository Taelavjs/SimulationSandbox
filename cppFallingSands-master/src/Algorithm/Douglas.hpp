#include <list>
#include <unordered_map>
#include <unordered_set>
#include "../Utility/Velocity.hpp"
class Douglas {
public:
	using Polyline = std::vector<Vector2D<float>>;
	static std::vector<Polyline> SegmentingLines(std::unordered_map<Vector2D<float>, std::vector<Vector2D<float>>> listPoints);
	static Polyline DouglasPeucker(const Polyline& original_polyline, float epsilon);

private:
	static float perpendicularDistance(const Vector2D<float>& p, const Vector2D<float>& a, const Vector2D<float>& b);
	static void simplify(const Polyline& points, size_t first, size_t last, float epsilon, Polyline& result);
	static bool points_equal(const Vector2D<float>& p1, const Vector2D<float>& p2);
};