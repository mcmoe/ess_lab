/**
* Channels updater for Four Channel PWM driver
*/

#ifndef PWM_UPDATER_H
#define PWM_UPDATER_H

/**
* Reverse updater order of channel transition
*/
void pwm_updater_reverse(void);

/**
* Update channel starting from 0 -> 100 -> 0 rotating between them one at a time (from ch0 -> .. -> ch3)
*/
void pwm_update_channels(void);

#endif
