#ifndef __FRUSTUM_GRID_H__
#define __FRUSTUM_GRID_H__
#include "MPUtils/Vector.h"
#include "MPVolume/VDBVolumeGrid.h"
#include <openvdb/Grid.h>
#include <openvdb/tools/Interpolation.h>
#include <openvdb/tools/RayIntersector.h>
#include "MPUtils/Camera.h"
namespace MeshPotato {
namespace MPVolume {
	/// Creates a frustum-shaped grid
class FrustumGrid : public Volume<float> {
public:
	static boost::shared_ptr<FrustumGrid > Ptr(
				boost::shared_ptr<MeshPotato::MPUtils::Camera> _cam,
				openvdb::BBoxd _bbox, openvdb::Coord::ValueType x_count=100,
				openvdb::Coord::ValueType z_count=100
				);

FrustumGrid(
				boost::shared_ptr<MeshPotato::MPUtils::Camera> _cam,
				openvdb::BBoxd _bbox,
				openvdb::Coord::ValueType x_count=100,
				openvdb::Coord::ValueType z_count=100
				);

	const float eval(const MeshPotato::MPUtils::MPVec3 &P) const;
	const void set(const openvdb::Coord &ijk, float value);
	MeshPotato::MPUtils::MPVec3 indexToWorld(const openvdb::Coord &ijk);
	virtual const MeshPotato::MPUtils::MPVec3 grad(const MeshPotato::MPUtils::MPVec3 &P) const;
	openvdb::CoordBBox getBBox();

	const void stamp(const VolumeFloatPtr density_grid);

	const void dsm(const VolumeFloatPtr density_grid, double K);

private:
openvdb::FloatGrid::Ptr grid;
boost::shared_ptr<MeshPotato::MPUtils::Camera> camera;
openvdb::BBoxd bbox;
openvdb::math::NonlinearFrustumMap frustum_map;
openvdb::math::MapBase::Ptr frustum_map_ptr;
openvdb::math::Transform frustumTransform;
openvdb::tools::GridSampler<openvdb::FloatTree, openvdb::tools::BoxSampler> interpolator;
openvdb::FloatGrid::Accessor accessor;
openvdb::FloatGrid::ConstAccessor accessor2;
openvdb::tools::GridSampler<openvdb::FloatGrid::ConstAccessor, openvdb::tools::BoxSampler> fastSampler;
};

}
}
#endif // __FRUSTUM_GRID_H__
