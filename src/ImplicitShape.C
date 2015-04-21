#include "MeshPotato/MPVolume/ImplicitShape.h"

namespace MeshPotato {
namespace MPVolume {
//TODO: Implement Copy Constructor and Assignment Operator
class ImplicitSphere::Impl {
public:
	float R;
	MeshPotato::MPUtils::MPVec3 C;
};

VolumeFloatPtr ImplicitSphere::Ptr(const float &_R, const MeshPotato::MPUtils::MPVec3 &_C) {
	return VolumeFloatPtr(new ImplicitSphere(_R, _C));
}

ImplicitSphere::ImplicitSphere(const float &_R,const MeshPotato::MPUtils::MPVec3 &_C) : mImpl(new ImplicitSphere::Impl()) {
mImpl->R = _R;
mImpl->C = _C;
}

ImplicitSphere::~ImplicitSphere() {}
const ImplicitSphere::volumeDataType ImplicitSphere::eval(const MeshPotato::MPUtils::MPVec3 &P) const {
	return mImpl->R - (P - mImpl->C).length();
}
const ImplicitSphere::volumeGradType ImplicitSphere::grad(const MeshPotato::MPUtils::MPVec3 &P) const {}

VolumeFloatPtr PyroclasticSphere::Ptr(const float &_R, const MeshPotato::MPUtils::MPVec3 &_C, const MeshPotato::MPNoise::Noise_t &_noiseparms) {
 return VolumeFloatPtr(new PyroclasticSphere(_R, _C, _noiseparms));
}

class PyroclasticSphere::Impl {
public:
	float R;
	MeshPotato::MPUtils::MPVec3 C;
	MeshPotato::MPNoise::Noise_t noiseparms;
        MeshPotato::MPNoise::FractalSum<MeshPotato::MPNoise::PerlinNoiseGustavson> perlin;
        MeshPotato::MPNoise::Noise* noise;
};

PyroclasticSphere::PyroclasticSphere(const float &_R, const MeshPotato::MPUtils::MPVec3 &_C, const MeshPotato::MPNoise::Noise_t &_noiseparms) : mImpl(new PyroclasticSphere::Impl()) {
	mImpl->R = _R;
	mImpl->C = _C;
	mImpl->noiseparms = _noiseparms;
        mImpl->perlin.setParameters(mImpl->noiseparms);
        mImpl->noise = &mImpl->perlin;
 }
PyroclasticSphere::~PyroclasticSphere() {
}
const PyroclasticSphere::volumeDataType PyroclasticSphere::eval(const MeshPotato::MPUtils::MPVec3 &P) const {
                                        return mImpl->R - (P - mImpl->C).length() + pow(fabs(mImpl->noise->eval((P - mImpl->C)/(P - mImpl->C).length())), mImpl->noiseparms.gamma)*1.0;
}

const PyroclasticSphere::volumeGradType PyroclasticSphere::grad(const MeshPotato::MPUtils::MPVec3& P) const {
                                        return (mImpl->C-P).unit();
}

class Cumulo::Impl {
public:
	VolumeFloatPtr f;
	MPNoise::Noise_t parms;
	VolumeVectorPtr Y;
	MPNoise::FractalSum<MPNoise::PerlinNoiseGustavson> perlin;
	boost::shared_ptr<MPNoise::Noise> noise;
};

const Cumulo::volumeDataType Cumulo::eval(const MPUtils::MPVec3 &P) const {
	if (mImpl->f->eval(P) > -1) {
		return mImpl->f->eval(P) + pow(fabs(mImpl->noise->eval(mImpl->Y->eval(P))), mImpl->parms.gamma) * mImpl->parms.amplitude;

	}
	else
		return -1;
}
const Cumulo::volumeGradType Cumulo::grad(const MPUtils::MPVec3 &P) const {
}

template <typename T>
class Clamp<T>::Impl {
public:
	Impl(const boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > &_f,
		   const float _e0,
			 const float _e1) : f(_f), e0(_e0), e1(_e1) {}
	boost::shared_ptr<MeshPotato::MPVolume::Volume<T> >f;
	float e0;
	float e1;

};
template <typename T>
boost::shared_ptr<Volume<T> > Clamp<T>::Ptr(const boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > &_f,  const float _e0, const float _e1) {
			return boost::shared_ptr<Clamp>(new Clamp<T>(_f, _e0, _e1));
}

template <typename T>
Clamp<T>::Clamp(const boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > &_f,  const float _e0, const float _e1) : mImpl(new Impl(_f, _e0, _e1)) {}

template <typename T>
const T Clamp<T>::eval(const MeshPotato::MPUtils::MPVec3 &P) const {
		if (mImpl->e0 <= mImpl->f->eval(P) && mImpl->f->eval(P) <= mImpl->e1) {
			return mImpl->f->eval(P);
		}
		else if (mImpl->f->eval(P) < mImpl->e0) {
			return mImpl->e0;
		}
		else if (mImpl->f->eval(P) > mImpl->e1) {
			return mImpl->e1;
		}
}

template class Clamp<float>;

template <typename T>
const typename Clamp<T>::volumeGradType Clamp<T>::grad(const MeshPotato::MPUtils::MPVec3 &P) const {}

template <typename T>
class ConstantVolume<T>::Impl {
public:
	Impl(const T &_value) : value(_value) {}
	T value;
};
template <typename T>
boost::shared_ptr<Volume<T> > ConstantVolume<T>::Ptr(const T _value) {
	return boost::shared_ptr<ConstantVolume>(new ConstantVolume<T>(_value));
}

template <typename T>
ConstantVolume<T>::ConstantVolume(const T _value) : mImpl(new Impl(_value)) {}

template <typename T>
const T ConstantVolume<T>::eval(const MeshPotato::MPUtils::MPVec3 &P) const {
	return mImpl->value;
}

template <typename T>
const typename ConstantVolume<T>::volumeGradType ConstantVolume<T>::grad(const MeshPotato::MPUtils::MPVec3 &P) const {}

/*
class CumuloVDB::Impl {
public:
	VolumeFloatPtr f;
	MPNoise::Noise_t wparms;
	VolumeVectorPtr Y;
	MPNoise::FractalSum<MPNoise::PerlinNoiseGustavson> perlin;
	boost::shared_ptr<MPNoise::Noise> noise;
	openvdb::FloatGrid::Ptr grid;
};

const openvdb::FloatGrid::Ptr  CumuloVDB::eval() const {
	openvdb::FloatGrid::Ptr cumuloGrid = mImpl->grid->deepCopy();
	openvdb::math::CPT_RANGE<openvdb::math::UniformScaleMap, openvdb::math::CD_2ND> range;
	openvdb::math::UniformScaleMap::Ptr mapping(mImpl->grid->transformPtr()->map<openvdb::math::UniformScaleMap>());
	openvdb::FloatGrid::Accessor cumuloAccessor = cumuloGrid->getAccessor();
	for (FloatGrid::ValueOnCIter iter = grid->cbeginValueOn(); iter.test(); ++iter) {
	openvdb::Coord ijk = iter.getCoord();
	MPUtils::MPVec3 point = range.result(*mImpl->grid->transformPtr()->map<openvdb::math::UniformScaleMap>(), mImpl->grid->getAccessor(), ijk);
	MPUtils::MPVec3 wsPoint = mImpl->grid->transformPtr()->indexToWorld(ijk);
	MPUtils::MPVec3 vect(wsPoint.x(), wsPoint.y(), wsPoint.z());
	float newVal = iter.getValue() - pow(fabs(mImpl->noise->eval(vect)),mImpl->wparms.NP0.gamma)*mImpl->wparms.NP0.amplitude;
	cumuloAccessor.setValue(ijk, newVal);
	}
	openvdb::tools::sdfToFogVolume<openvdb::FloatGrid>(cumuloGrid.operator*());
	return cumuloGrid;
}
*/
}
}
