#include "water_tower_attack_component.hpp"

WaterTowerAttackComponent::WaterTowerAttackComponent(vec2 _relativeFirePosition, vector<float> _rangePerLvl, vector<float> _slowPerLvl) :
  TowerAttackComponent(_relativeFirePosition, vector<int>{}, _rangePerLvl, vector<float>{}),
  slowPerLvl(_slowPerLvl)
{
}

void WaterTowerAttackComponent::setToLevel(int level)
{
  attackRange = rangePerLvl[level];
  slowDownFactor = slowPerLvl[level]; 
}

float WaterTowerAttackComponent::getSlowDownFactor()
{
  return slowDownFactor;
}
