#ifndef __VOLUMEUTILS_H__
#define __VOLUMEUTILS_H__

#include "MPVolume.h"
#include "FrustumGrid.h"
#include <openvdb/tools/LevelSetRebuild.h>
namespace MeshPotato {
	namespace MPVolume {
		/// Helper function for reading VDB grids
		openvdb::GridBase::Ptr readVDBGrid(const std::string vdbFile);
/// Helper function for writing VDB grids
		openvdb::FloatGrid::Ptr makeVDBGrid(boost::shared_ptr<Volume<float> > &mpgrid, const MeshPotato::MPUtils::BBox& worldBB, double voxelSize);

		/// Helper function for building frustum cameras
		boost::shared_ptr<MeshPotato::MPUtils::Camera> buildFrustumCamera(MeshPotato::MPUtils::MPVec3 eye, openvdb::FloatGrid::Ptr grid);

		double computeFOVDegrees(MeshPotato::MPUtils::MPVec3 point, MeshPotato::MPUtils::MPVec3 eye, MeshPotato::MPUtils::MPVec3 view);
		double computeDistance(MeshPotato::MPUtils::MPVec3 point, MeshPotato::MPUtils::MPVec3 eye, MeshPotato::MPUtils::MPVec3 view);
		/*
		//! Start from the near plane of the frustum grid.  Calculate a ray to the far plane for each cell of the frustum
		void dsm(openvdb::tools::PerspectiveCamera &vdbcam, FrustumGrid *grid, VolumeFloatPtr f) {
		// Get dimensions of the frustum grid by getting it's axis-aligned bounding box
		openvdb::CoordBBox bbox = grid->getBoundingBox();

		int xSize = bbox.max().x() - bbox.min.x();
		int ySize = bbox.max().y() - bbox.min.y();
		int zSize = bbox.max().z() - bbox.min.z();
		for (int j = 0; j < ySize; ++j) {
		for (int i = 0; i < xSize; ++i) {
		float x = (float)i/(xSize - 1);
		float y = (float)j/(ySize - 1);
		MeshPotato::MPUtils::Ray ray = vdbcam->getRay(i,j);
		float deltaS; // (farPlane - nearPlane)/(cam->view*d))/(zSize - 1)

		for (int k = 0; k < zSize; ++k) {
		float val = f->eval(X);
		if (val > 0) {
		value += val * deltaS;
		}
		grid->set(ijk, value);
		}
		}

		}

		}
		 */
	}
}
#endif // __VOLUMEUTILS_H__
