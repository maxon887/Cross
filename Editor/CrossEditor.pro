QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CrossEditor
TEMPLATE = app

LIBS += "$$PWD/../Binaries/MacOS/libfmod.dylib" \
		"$$PWD/../Binaries/MacOS/libassimpMAC.dylib"

message("Project Directory = $${PWD}")
message("Build Directory = $${OUT_PWD}")
# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS \
			MACOS \
			EDITOR

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += \
	../Sources/  \
	../Sources/Core/ \
	../Sources/Internals/ \
	../Sources/Math/ \
	../Sources/Shaders/ \
	../Sources/Utils/ \
	../Sources/Libs/ \
	Sources/ \
	Sources/PropertyViews \
	/usr/local/include/

SOURCES += \
    Sources/PropertyViews/EntityView.cpp \
    Sources/PropertyViews/FileHandler.cpp \
    Sources/PropertyViews/LightComponent.cpp \
    Sources/PropertyViews/MaterialView.cpp \
    Sources/PropertyViews/MeshComponent.cpp \
    Sources/PropertyViews/PropertyView.cpp \
    Sources/PropertyViews/ShaderView.cpp \
    Sources/PropertyViews/TransformComponent.cpp \
    Sources/CrossEditor.cpp \
    Sources/FileExplorer.cpp \
    Sources/GLHandler.cpp \
    Sources/History.cpp \
    Sources/main.cpp \
    Sources/PropertiesView.cpp \
    Sources/SceneExplorer.cpp \
    Sources/SceneView.cpp \
    ../Sources/Core/Camera.cpp \
    ../Sources/Core/Color.cpp \
    ../Sources/Core/Component.cpp \
    ../Sources/Core/ComponentFactory.cpp \
    ../Sources/Core/Config.cpp \
    ../Sources/Core/Entity.cpp \
    ../Sources/Core/File.cpp \
    ../Sources/Core/Game.cpp \
    ../Sources/Core/Input.cpp \
    ../Sources/Core/Light.cpp \
    ../Sources/Core/Material.cpp \
    ../Sources/Core/Mesh.cpp \
    ../Sources/Core/Model.cpp \
    ../Sources/Core/Scene.cpp \
    ../Sources/Core/Screen.cpp \
    ../Sources/Core/Sound.cpp \
    ../Sources/Core/System.cpp \
    ../Sources/Core/Texture.cpp \
    ../Sources/Core/Transform.cpp \
    ../Sources/Core/VertexBuffer.cpp \
    ../Sources/Experimental/String.cpp \
    ../Sources/Internals/Audio.cpp \
    ../Sources/Internals/GraphicsGL.cpp \
    ../Sources/Internals/MemoryManager.cpp \
    ../Sources/Libs/Assimp/color4.inl \
    ../Sources/Libs/Assimp/material.inl \
    ../Sources/Libs/Assimp/matrix3x3.inl \
    ../Sources/Libs/Assimp/matrix4x4.inl \
    ../Sources/Libs/Assimp/quaternion.inl \
    ../Sources/Libs/Assimp/vector2.inl \
	../Sources/Libs/Assimp/vector3.inl \
    ../Sources/Libs/TinyXML2/tinyxml2.cpp \
    ../Sources/Math/All.cpp \
    ../Sources/Math/Matrix.cpp \
    ../Sources/Math/Quaternion.cpp \
    ../Sources/Math/Rect.cpp \
    ../Sources/Math/Vector2D.cpp \
    ../Sources/Math/Vector3D.cpp \
    ../Sources/Math/Vector4D.cpp \
    ../Sources/Platform/MacOS/MacSystem.cpp \
    ../Sources/Shaders/LightsShader.cpp \
    ../Sources/Shaders/Shader.cpp \
    ../Sources/Shaders/SingleLightShader.cpp \
    ../Sources/Utils/Cubemap.cpp \
    ../Sources/Utils/Debugger.cpp \
    ../Sources/Utils/FreeCameraScene.cpp \
    ../Sources/Utils/PrimitiveDrawer.cpp \
    ../Sources/Utils/Skybox.cpp \
    ../Sources/Libs/SOIL/image_helper.c \
    ../Sources/Libs/SOIL/SOIL.c \
    ../Sources/Libs/SOIL/stb_image_aug.c \
    Sources/ESystem.cpp

HEADERS += \
    Sources/PropertyViews/ComponentView.h \
    Sources/PropertyViews/EntityView.h \
    Sources/PropertyViews/FileHandler.h \
    Sources/PropertyViews/LightComponent.h \
    Sources/PropertyViews/MaterialView.h \
    Sources/PropertyViews/MeshComponent.h \
    Sources/PropertyViews/PropertyView.h \
    Sources/PropertyViews/ShaderView.h \
    Sources/PropertyViews/TransformComponent.h \
    Sources/CrossEditor.h \
    Sources/ESystem.h \
    Sources/FileExplorer.h \
    Sources/GLHandler.h \
    Sources/History.h \
    Sources/PropertiesView.h \
    Sources/SceneExplorer.h \
    Sources/SceneView.h \
    ../Sources/Core/Camera.h \
    ../Sources/Core/Color.h \
    ../Sources/Core/Component.h \
    ../Sources/Core/ComponentFactory.h \
    ../Sources/Core/Config.h \
    ../Sources/Core/Cross.h \
    ../Sources/Core/Entity.h \
    ../Sources/Core/Event.h \
    ../Sources/Core/File.h \
    ../Sources/Core/Game.h \
    ../Sources/Core/Input.h \
    ../Sources/Core/Light.h \
    ../Sources/Core/Material.h \
    ../Sources/Core/Mesh.h \
    ../Sources/Core/Model.h \
    ../Sources/Core/Scene.h \
    ../Sources/Core/Screen.h \
    ../Sources/Core/Sound.h \
    ../Sources/Core/System.h \
    ../Sources/Core/Texture.h \
    ../Sources/Core/Transform.h \
    ../Sources/Core/VertexBuffer.h \
    ../Sources/Experimental/Array.h \
    ../Sources/Experimental/ArraySTD.h \
    ../Sources/Experimental/String.h \
    ../Sources/Internals/Audio.h \
    ../Sources/Internals/GraphicsGL.h \
    ../Sources/Internals/MemoryManager.h \
    ../Sources/Libs/Assimp/Compiler/poppack1.h \
    ../Sources/Libs/Assimp/Compiler/pstdint.h \
    ../Sources/Libs/Assimp/Compiler/pushpack1.h \
    ../Sources/Libs/Assimp/port/AndroidJNI/AndroidJNIIOSystem.h \
    ../Sources/Libs/Assimp/ai_assert.h \
    ../Sources/Libs/Assimp/anim.h \
    ../Sources/Libs/Assimp/camera.h \
    ../Sources/Libs/Assimp/cexport.h \
    ../Sources/Libs/Assimp/cfileio.h \
    ../Sources/Libs/Assimp/cimport.h \
    ../Sources/Libs/Assimp/color4.h \
    ../Sources/Libs/Assimp/config.h \
    ../Sources/Libs/Assimp/DefaultIOStream.h \
    ../Sources/Libs/Assimp/DefaultIOSystem.h \
    ../Sources/Libs/Assimp/DefaultLogger.hpp \
    ../Sources/Libs/Assimp/Defines.h \
    ../Sources/Libs/Assimp/defs.h \
    ../Sources/Libs/Assimp/Exporter.hpp \
    ../Sources/Libs/Assimp/Importer.hpp \
    ../Sources/Libs/Assimp/importerdesc.h \
    ../Sources/Libs/Assimp/IOStream.hpp \
    ../Sources/Libs/Assimp/IOSystem.hpp \
    ../Sources/Libs/Assimp/light.h \
    ../Sources/Libs/Assimp/Logger.hpp \
    ../Sources/Libs/Assimp/LogStream.hpp \
    ../Sources/Libs/Assimp/material.h \
    ../Sources/Libs/Assimp/matrix3x3.h \
    ../Sources/Libs/Assimp/matrix4x4.h \
    ../Sources/Libs/Assimp/mesh.h \
    ../Sources/Libs/Assimp/metadata.h \
    ../Sources/Libs/Assimp/NullLogger.hpp \
    ../Sources/Libs/Assimp/postprocess.h \
    ../Sources/Libs/Assimp/ProgressHandler.hpp \
    ../Sources/Libs/Assimp/quaternion.h \
    ../Sources/Libs/Assimp/scene.h \
    ../Sources/Libs/Assimp/SceneCombiner.h \
    ../Sources/Libs/Assimp/texture.h \
    ../Sources/Libs/Assimp/types.h \
    ../Sources/Libs/Assimp/vector2.h \
    ../Sources/Libs/Assimp/vector3.h \
    ../Sources/Libs/Assimp/version.h \
    ../Sources/Libs/FMOD/fmod.h \
    ../Sources/Libs/FMOD/fmod.hpp \
    ../Sources/Libs/FMOD/fmod_codec.h \
    ../Sources/Libs/FMOD/fmod_common.h \
    ../Sources/Libs/FMOD/fmod_dsp.h \
    ../Sources/Libs/FMOD/fmod_dsp_effects.h \
    ../Sources/Libs/FMOD/fmod_errors.h \
    ../Sources/Libs/FMOD/fmod_output.h \
    ../Sources/Libs/SOIL/image_DXT.h \
    ../Sources/Libs/SOIL/image_helper.h \
    ../Sources/Libs/SOIL/SOIL.h \
    ../Sources/Libs/SOIL/stb_image_aug.h \
    ../Sources/Libs/SOIL/stbi_DDS_aug.h \
    ../Sources/Libs/SOIL/stbi_DDS_aug_c.h \
    ../Sources/Libs/TinyXML2/tinyxml2.h \
    ../Sources/Math/All.h \
    ../Sources/Math/Matrix.h \
    ../Sources/Math/Quaternion.h \
    ../Sources/Math/Rect.h \
    ../Sources/Math/Vector2D.h \
    ../Sources/Math/Vector3D.h \
    ../Sources/Math/Vector4D.h \
    ../Sources/Shaders/LightsShader.h \
    ../Sources/Shaders/Shader.h \
    ../Sources/Shaders/SingleLightShader.h \
    ../Sources/Utils/Cubemap.h \
    ../Sources/Utils/Debugger.h \
    ../Sources/Utils/FreeCameraScene.h \
    ../Sources/Utils/PrimitiveDrawer.h \
	../Sources/Utils/Skybox.h \
    ../Sources/Platform/MacOS/MacSystem.h

FORMS += \
    Sources/PropertyViews/EntityView.ui \
    Sources/PropertyViews/LightComponent.ui \
    Sources/PropertyViews/MaterialView.ui \
    Sources/PropertyViews/MeshComponent.ui \
    Sources/PropertyViews/ShaderView.ui \
    Sources/PropertyViews/TransformComponent.ui \
    Sources/CrossEditor.ui

DISTFILES += \
    ../Sources/Libs/ImGui/extra_fonts/Cousine-Regular.ttf \
    ../Sources/Libs/ImGui/extra_fonts/DroidSans.ttf \
    ../Sources/Libs/ImGui/extra_fonts/Karla-Regular.ttf \
    ../Sources/Libs/ImGui/extra_fonts/ProggyClean.ttf \
    ../Sources/Libs/ImGui/extra_fonts/ProggyTiny.ttf \
    ../Sources/Libs/ImGui/extra_fonts/Roboto-Medium.ttf \
    ../Sources/Libs/ImGui/extra_fonts/VeraMono-Bold-Italic.ttf \
    ../Sources/Libs/ImGui/extra_fonts/VeraMono-Italic.ttf \
    ../Sources/Libs/ImGui/extra_fonts/VeraMono.ttf \
    ../Sources/Libs/ImGui/extra_fonts/VeraMonoBold.ttf \
    ../Sources/Libs/ImGui/README.md \
    ../Sources/Libs/Assimp/config.h.in \
    ../Sources/Libs/ImGui/extra_fonts/README.txt \
    ../Sources/Libs/ImGui/LICENSE.txt
