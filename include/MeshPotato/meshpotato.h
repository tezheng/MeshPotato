#ifndef MESHPOTATO_H
#define MESHPOTATO_H
#include <iostream>
#include <string.h>
namespace MeshPotato {
	class MeshSpec {
	}; //! Define the specification for mesh data

	struct DeepData {}; //! Define strut for deep data

	class MeshInput {
		public:
			MeshInput();
			MeshInput(const MeshInput &mesh);	
			~MeshInput();
			//! Attempt to open the mesh file
			MeshInput *create(const std::string &);
			virtual bool create(const std::string &, const MeshSpec &newspec) = 0;

	}; //! Input Mesh

	class MeshOutput {}; //! Output Mesh




}
#endif // MESHPOTATO_H
