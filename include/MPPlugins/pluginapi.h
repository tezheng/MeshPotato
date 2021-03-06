#ifndef __PLUGINAPI_H__
#define __PLUGINAPI_H__
#include "defines.h"
#include "MPMesh/InputMesh.h"
#include "MPVolume/MPVolume.h"
#include "MPMesh/OutputMesh.h"
namespace MeshPotato {
	namespace MPMesh {
		class OutputMesh;
	}
}
namespace MeshPotato {
namespace MPPlugins {
#define PLUGIN_API_VERSION 1

#define CORE_FUNC extern "C" CORE_API
#define PLUGIN_FUNC extern "C" PLUGIN_API

/// declaration of the initialization routine for a plugin
#define PLUGIN_INIT() \
	const int PluginVersion = PLUGIN_API_VERSION; \
	PLUGIN_FUNC int PluginInit()

/// declaration  of the cleanup routine for a plugin
#define PLUGIN_FREE() \
	PLUGIN_FUNC int PluginFree()

/// declaration of the display name of a plugin
#define PLUGIN_DISPLAY_NAME(name) \
	PLUGIN_API const char * PluginDisplayName = name

/// declaration of the minimum required Plugin API version for a plugin
#define PLUGIN_MIN_VERSION(version) \
	PLUGIN_API const char * PluginMinVersion = version

/// declaration of the maximum supported Plugin API version for a plugin
#define PLUGIN_MAX_VERSION(version) \
	PLUGIN_API const char * PluginMaxVersion = version

/// The function signature for a routine that writes out/reads in a mesh
typedef MeshPotato::MPMesh::OutputMesh *(*OutputMeshInitFunc)();
typedef MeshPotato::MPMesh::InputMesh *(*InputMeshInitFunc)();
typedef MeshPotato::MPVolume::VolumeFloatPtr *(*VolumeFloatInitFunc)();

/// The function signature for a routine that destroys an output mesh
typedef void (*OutputMeshFreeFunc)(MeshPotato::MPMesh::OutputMesh *);
typedef void (*InputMeshFreeFunc)(MeshPotato::MPMesh::InputMesh *);
typedef void (*VolumeFloatFreeFunc)(MeshPotato::MPVolume::VolumeFloatPtr *);

/// A routine to let a plugin register a new output mesh type
CORE_FUNC void RegisterOutputMesh(const char *type, 
				  OutputMeshInitFunc init_cb,
				  OutputMeshFreeFunc free_cb);

CORE_FUNC void RegisterInputMesh(const char *type, 
				  InputMeshInitFunc init_cb,
				  InputMeshFreeFunc free_cb);
CORE_FUNC void RegisterVolumeFloat(const char *type, 
				  VolumeFloatInitFunc init_cb,
				  VolumeFloatFreeFunc free_cb);

} // MPPlugins
} // MeshPotato
#endif // __PLUGINAPI_H__
