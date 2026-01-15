#include "Light.h"
#include "Game.h"
#include "Scene.h"
#include "System.h"

#include  <algorithm>

using namespace cross;

Light::Light(Type type) :
	Component("Light"),
	type(this, "Type", type)
{ }

bool Light::Activate() {
	Scene* scene = game->GetCurrentScene();
	List<Light*>& lights = scene->GetLights();
	auto it = std::find(lights.begin(), lights.end(), this);
	CROSS_RETURN(it == lights.end(), false, "Current light already in the scene");
	lights.push_back(this);
	return true;
}

void Light::Deactivate() {
	Scene* scene = game->GetCurrentScene();
	List<Light*>& lights = scene->GetLights();
	auto it = std::find(lights.begin(), lights.end(), this);
	if(it != lights.end()) {
		lights.erase(it);
	}
}

Color Light::GetColor() const {
	return color;
}

void Light::SetColor(const Color& color) {
	this->color = color;
}

Light* Light::Clone() const {
	return CREATE Light(*this);
}

Light::Type Light::GetType() const {
	return type;
}

float Light::GetIntensity() const {
	return intensity;
}

void Light::SetIntensity(float inten) {
	intensity = inten;
}

void Light::SetCutOff(float cutOff) {
	this->cut_off = cutOff;
}

float Light::GetCutOff() const {
	return cut_off;
}

float Light::GetOuterCutOff() const {
	return cut_off + 10.f;
}