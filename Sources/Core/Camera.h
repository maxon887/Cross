#pragma once
#include "Cross.h"
#include "Component.h"

namespace cross {

/*	3D Camera Component. At least one camera must be set into Scene by SetCamera() in order to render a Scene.
	Camera can't be used until SetProjectionMatrix() is called.
	Camera Component can't be used with Entity without Transform Component */
class Camera : public Component {
public:
	Camera();
	/* Initializes Camera Component */
	bool Activate() override;
	/* Removes default Camera from Scene if it was out camera */
	void Deactivate() override;
	/* Updates Camera Component. Realized in order to recalculate view Matrix */
	void Update(float sec) override;
	/* Save copy of Camera Component */
	Component* Clone() const override;

	/* Returns Camera view distance. Object under that distance won't be rendered */
	float GetViewDistance() const;
	/* Returns Camera's view Matrix. Depends on Entity's Transform Component */
	const Matrix& GetViewMatrix() const;
	/* Returns Camera's projection Matrix. Must be setup first */
	const Matrix& GetProjectionMatrix() const;
	/* Recalculate projection matrix based new values */
	void UpdateProjectionMatrix();

protected:
	Property<float> view_distance	= Property<float>(this, "ViewDistance", 100.f);

	Matrix view						= Matrix::Identity;
	Matrix projection				= Matrix::Identity;

	void RecalcView();
};

}