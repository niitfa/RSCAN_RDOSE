/*
 * voltage_corrector.h
 *
 *  Created on: Oct 30, 2026
 *      Author: Kirill
 */

#ifndef SRC_HV_TASK_VOLTAGE_CORRECTOR_H_
#define SRC_HV_TASK_VOLTAGE_CORRECTOR_H_

#define VOLTAGE_CORRECTION_DELAY_TIME_MS (int)2000
#define VOLTAGE_CORRECTIONS_NUM (int) 5

#define VOLTAGE_STABILITY_CHECK_PERIOD_MS (int)400
#define VOLTAGE_STABILITY_ARRAY_SIZE (int)5

#include <stdint.h>

typedef enum
{
	VOLTAGE_CORRECTOR_STANDBY,
	VOLTAGE_CORRECTOR_PROCESS,
	VOLTAGE_CORRECTOR_PROCESS_WAIT,
	VOLTAGE_CORRECTOR_SUCCESS,
	VOLTAGE_CORRECTOR_ERROR
} voltage_corrector_state_t;

typedef struct
{
	int updatePeriod_ms;
	voltage_corrector_state_t state;
	int targetVoltage;
	int actualVoltage;
	double k_correction;

	int targetVoltageCurrentStep;
	int actualVoltageCurrentStep;
	// corrections
	uint8_t correctionTicksWaitMax;
	uint8_t correctionTicksWaitRemained;
	uint8_t numOfCorrectionsMax;
	uint8_t numOfCorrectionsRemained;
	// stability
	uint8_t stabilityCheckTicksMax;
	uint8_t stabilityCheckTicksRemained;
	int lastVoltage[VOLTAGE_STABILITY_ARRAY_SIZE];
	int lastVoltage_index;
	// set/get callbacks
	void (*set_voltage_cb)(int);
	int (*get_voltage_cb)(void);
} voltage_corrector_t;


void voltage_corrector_init(
		voltage_corrector_t* self,
		void (*set_voltage_cb)(int),
		int (*get_voltage_cb)(void),
		int updatePeriod_ms
		);
void voltage_corrector_run(voltage_corrector_t* self, int targetVoltage);
void voltage_corrector_stop(voltage_corrector_t* self);
int voltage_corrector_get_actual_voltage(voltage_corrector_t* self);
void voltage_corrector_tick(voltage_corrector_t* self);

voltage_corrector_state_t voltage_corrector_get_state(voltage_corrector_t* self);

#endif /* SRC_HV_TASK_VOLTAGE_CORRECTOR_H_ */
