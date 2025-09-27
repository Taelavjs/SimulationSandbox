#include "Douglas.hpp"
using Polyline = std::vector<Vector2D<float>>;

std::vector<Polyline> Douglas::SegmentingLines(std::unordered_map<Vector2D<float>, std::vector<Vector2D<float>>> listPoints) {
	std::vector<Polyline> polyLines;
	std::unordered_set<Vector2D<float>> visitedLines;
	auto remove_neighbor = [](std::unordered_map<Vector2D<float>, std::vector<Vector2D<float>>>& map, const Vector2D<float>& p1, const Vector2D<float>& p2) {
		auto it_list = map.find(p1);
		if (it_list != map.end()) {
			it_list->second.erase(
				std::remove_if(it_list->second.begin(), it_list->second.end(),
					[&](const Vector2D<float>& p) { return p == p2; }),
				it_list->second.end()
			);
		}
		};

	std::vector<Vector2D<float>> keys;
	for (const auto& pair : listPoints) {
		keys.push_back(pair.first);
	}
	for (const auto& start_point : keys) {
		if (listPoints.count(start_point) == 0 || listPoints[start_point].empty()) {
			continue;
		}
		Polyline current_polyline{};
		Vector2D current_point = start_point;
		current_polyline.push_back(current_point);
		while (!listPoints[current_point].empty()) {
			Vector2D<float> next_point = listPoints[current_point].back();
			remove_neighbor(listPoints, current_point, next_point);
			remove_neighbor(listPoints, next_point, current_point);
			current_polyline.push_back(next_point);
			current_point = next_point;
			if (current_point == start_point) {
				break;
			}
		}
		if (current_polyline.size() > 1) {
			polyLines.push_back(std::move(current_polyline));
		}
	}


	return polyLines;
}

Polyline Douglas::DouglasPeucker(const Polyline& original_polyline, float epsilon) {
	if (original_polyline.size() < 2) {
		return original_polyline;
	}

	Polyline simplified_polyline;
	simplified_polyline.push_back(original_polyline.front());

	simplify(original_polyline, 0, original_polyline.size() - 1, epsilon, simplified_polyline);

	return simplified_polyline;
}

float perpendicularDistance(const Vector2D<float>& p, const Vector2D<float>& a, const Vector2D<float>& b) {
	float dx = b.x - a.x;
	float dy = b.y - a.y;
	if (std::abs(dx) < std::numeric_limits<float>::epsilon() && std::abs(dy) < std::numeric_limits<float>::epsilon()) {
		return std::sqrt(std::pow(p.x - a.x, 2) + std::pow(p.y - a.y, 2));
	}
	float t = ((p.x - a.x) * dx + (p.y - a.y) * dy) / (dx * dx + dy * dy);

	float nearestX = a.x + t * dx;
	float nearestY = a.y + t * dy;

	return std::sqrt(std::pow(p.x - nearestX, 2) + std::pow(p.y - nearestY, 2));
}

float Douglas::perpendicularDistance(const Vector2D<float>& p, const Vector2D<float>& a, const Vector2D<float>& b) {
	float dx = b.x - a.x;
	float dy = b.y - a.y;

	// If segment a-b is a point, return the distance to that point
	if (std::abs(dx) < std::numeric_limits<float>::epsilon() && std::abs(dy) < std::numeric_limits<float>::epsilon()) {
		return std::sqrt(std::pow(p.x - a.x, 2) + std::pow(p.y - a.y, 2));
	}
	float t = ((p.x - a.x) * dx + (p.y - a.y) * dy) / (dx * dx + dy * dy);
	float nearestX = a.x + t * dx;
	float nearestY = a.y + t * dy;

	return std::sqrt(std::pow(p.x - nearestX, 2) + std::pow(p.y - nearestY, 2));
}

void Douglas::simplify(const Polyline& points, size_t first, size_t last, float epsilon, Polyline& result) {
	float maxDist = 0;
	size_t index = 0;

	// Find the point with the maximum distance
	Vector2D<float> a = points[first];
	Vector2D<float> b = points[last];

	for (size_t i = first + 1; i < last; ++i) {
		float dist = perpendicularDistance(points[i], a, b);
		if (dist > maxDist) {
			maxDist = dist;
			index = i;
		}
	}
	if (maxDist > epsilon) {
		simplify(points, first, index, epsilon, result);
		simplify(points, index, last, epsilon, result);
	}
	else {
		if (result.back() != points[last]) {
			result.push_back(points[last]);
		}
	}
}
