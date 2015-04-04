#ifndef CGAMERULES_H

#include <math.h>
/* calculations for various game mechanics, not currently used */
namespace GameRules {
static float DamageReductionCalculate(float armor) {
    return armor/(20+armor);
}
static float ActualArmorCalculate(float armor_skill, float armor_base, float dex, float shield_skill, float shield_base) {
    return (0.25*dex/100.0 + 1.00*armor_skill/100.0 + 1.0)*armor_base;
}
static float DodgeCalculate(float dex, float dodge) {
    return 0.65*(0.75*dodge/100.0, 0.25*dex/100.0);
}
static float ShieldBonus(float shield) {

}
static int HealthCalculate(float str, float health, float meditate, float swords, float shields) {
    return round(333.0*(0.3*health/100.0+0.3*str/100.0+0.2*shields/100.0+0.125*swords/100.0+0.075*meditate/100.0))+2.0;
}
static float ManaCalculate(float intel, float meditate, float magery) {
    return round(400.0*(0.4*intel+0.4*meditate+0.2*magery));
}
static float StamCalculate(float dex) {
    return round(100.0*(dex));
}
}
#endif
