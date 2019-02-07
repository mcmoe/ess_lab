/**
* Channels updater for Four Channel PWM driver
*/

#ifndef PWM_UPDATER_H
#define PWM_UPDATER_H


/**
* Update channel starting from 0 -> 100 -> 0 rotating between them one at a time
*/
void pwm_update_channels(void);

#endif
