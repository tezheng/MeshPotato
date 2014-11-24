#include "MPVolume/DenseGrid.h"

#include "MPUtils/Vector.h"
#include "MPVolume/MPVolumeGrid.h"
#include <openvdb/Grid.h>
#include <openvdb/tools/Interpolation.h>
#include <openvdb/openvdb.h>
#include <openvdb/math/Transform.h>
#include <openvdb/math/BBox.h>
#include <cmath>


using MeshPotato::MPUtils::MPVec3;
/// Partial Template Instantiation for Grid Types

namespace MeshPotato {
  namespace MPVolume {

      template <typename T>
      boost::shared_ptr<VolumeGrid<T> > DenseGrid<T>::Ptr(
          const MPUtils::BBox _box,
          const float _resolution) {
        return boost::shared_ptr<VolumeGrid<T> >(new DenseGrid<T>(_box, _resolution));
      }

      template <typename T>
      DenseGrid<T>::DenseGrid(const MPUtils::BBox _bbox,
      const float _resolution) :
      bbox(_bbox),
      voxelSize(_resolution),
      grid(MPGridT::create()),
      interpolator(
      grid->constTree(),
      grid->transform()
      ),
      accessor(grid->getAccessor())
      {

        std::cout << "test" << std::endl;
        grid->transform().preScale(voxelSize);
        grid->transform().postTranslate(bbox.min());
        std::cout << "voxelSize = " << voxelSize << std::endl;
        std::cout << "voxelSize2 = " << grid->transform().voxelSize() << std::endl;
        MPUtils::MPVec3 temp  = ((bbox.max() - bbox.min())/voxelSize);
        dim.x() = (1 + ceil(temp.x()));
        dim.y() = (1 + ceil(temp.y()));
        dim.z() = (1 + ceil(temp.z()));

      }

      /*! \brief evaluates the grid at a particular world space point using trilinear
      *           interpolation
      *
      * \param MPVec3 &P is a const vector representing a world space position
      * \return const typename Volume<T>::volumeDataType
      */
      template <typename T>
      const typename Volume<T>::volumeDataType  DenseGrid<T>::eval(const MPVec3 &P) const {
        return interpolator.wsSample(P);
      }

      template <typename T>
      const typename Volume<T>::volumeGradType  DenseGrid<T>::grad(const MPVec3 &P) const {
      }

      template <typename T>
      void DenseGrid<T>::set(const MPUtils::Coord ijk, const T value) {
        accessor.setValue(ijk, value);
      }

      template <typename T>
      const T DenseGrid<T>::get(const MPUtils::Coord ijk) const{
        return accessor.getValue(ijk);
      }

      template <typename T>
      void DenseGrid<T>::add(const MPUtils::Coord ijk, const T value) {
        accessor.setValue(ijk, accessor.getValue(ijk) + value);
      }

      template <typename T>
      void DenseGrid<T>::stamp(boost::shared_ptr<Volume<T> > volume) {
        MPUtils::Coord ijk;
        int &i = ijk[0], &j = ijk[1], &k = ijk[2];
        for (i = 0; i < dim.x(); ++i) {
          for (j = 0; j < dim.y(); ++j) {
            for (k = 0; k < dim.z(); ++k) {
              MPUtils::MPVec3 P = grid->transform().indexToWorld(ijk);
              accessor.setValue(ijk, volume->eval(P));
            }
          }
        }
      }

      template <typename T>
      const MPUtils::MPVec3 DenseGrid<T>::indexToWorld(const MPUtils::Coord ijk) const {
        return grid->transform().indexToWorld(ijk);
      }

      template <typename T>
      const MPUtils::MPVec3 DenseGrid<T>::worldToIndex(const MPUtils::MPVec3 P) const {
        return grid->transform().worldToIndex(P);
      }

      template <typename T>
      const int DenseGrid<T>::nx() const { return dim.x(); }

      template <typename T>
      const int DenseGrid<T>::ny() const { return dim.y(); }

      template <typename T>
      const int DenseGrid<T>::nz() const { return dim.z(); }


      template <typename T>
      boost::shared_ptr<VolumeGrid<T> > DenseGrid<T>::deepCopy() {
        boost::shared_ptr<VolumeGrid<T> > newGrid = DenseGrid<T>::Ptr(bbox, voxelSize);
        MPUtils::Coord ijk;
        int &i = ijk[0], &j = ijk[1], &k = ijk[2];
        for (i = bbox.min().x(); i < bbox.max().x(); ++i) {
          for (j = bbox.min().y(); j < bbox.max().y(); ++j) {
            for (k = bbox.min().z(); k < bbox.max().z(); ++k) {

              newGrid->set(ijk, accessor.getValue(ijk));
            }
          }
        }
        return newGrid;


      }

      template class DenseGrid<float>;
      template class DenseGrid<MPUtils::MPVec3>;
  }

}
