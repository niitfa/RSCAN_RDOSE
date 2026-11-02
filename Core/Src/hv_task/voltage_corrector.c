#include "voltage_corrector.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>

static void process_tick(voltage_corrector_t* self);
static void process_wait_tick(voltage_corrector_t* self);
static void voltage_stablility_check_tick(voltage_corrector_t* self);
static uint8_t voltage_stable(voltage_corrector_t* self);
static uint8_t target_voltage_reached(voltage_corrector_t* self);
static int max_voltage(voltage_corrector_t* self);
static int min_voltage(voltage_corrector_t* self);
static void correction_step(voltage_corrector_t* self);
static double kCorrection(voltage_corrector_t* self);

void voltage_corrector_init(
		voltage_corrector_t* self,
		void (*set_voltage_cb)(int),
		int (*get_voltage_cb)(void),
		int updatePeriod_ms
		)
{
	memset(self, 0, sizeof(*self));

	self->updatePeriod_ms = updatePeriod_ms;

	// corrections
	self->correctionTicksWaitMax = VOLTAGE_CORRECTION_DELAY_TIME_MS / self->updatePeriod_ms;
	self->numOfCorrectionsMax = VOLTAGE_CORRECTIONS_NUM;
	self->correctionTicksWaitRemained = self->correctionTicksWaitMax;
	self->numOfCorrectionsRemained = self->numOfCorrectionsMax;

	// stability
	self->stabilityCheckTicksMax = VOLTAGE_STABILITY_CHECK_PERIOD_MS / self->updatePeriod_ms;
	self->stabilityCheckTicksRemained = self->stabilityCheckTicksMax;
	self->lastVoltage_index = 0;

	// set/get callbacks
	self->set_voltage_cb = set_voltage_cb;
	self->get_voltage_cb = get_voltage_cb;

	self->state = VOLTAGE_CORRECTOR_STANDBY;
	self->targetVoltage = 0;
	self->actualVoltage = 0;
	self->k_correction = 1;
	self->set_voltage_cb(0);
}

void voltage_corrector_run(voltage_corrector_t* self, int targetVoltage)
{
	// setup variables
	self->targetVoltage = targetVoltage;
	self->numOfCorrectionsRemained = self->numOfCorrectionsMax;
	// current
	self->targetVoltageCurrentStep = targetVoltage;
	// switch state
	self->state = VOLTAGE_CORRECTOR_PROCESS;
}

void voltage_corrector_stop(voltage_corrector_t* self)
{
	self->state = VOLTAGE_CORRECTOR_STANDBY;
}

int voltage_corrector_get_actual_voltage(voltage_corrector_t* self)
{
	return self->actualVoltage;
}

void voltage_corrector_tick(voltage_corrector_t* self)
{
	voltage_stablility_check_tick(self);
	switch(self->state)
	{
		break;
	case VOLTAGE_CORRECTOR_STANDBY:
		break;
	case VOLTAGE_CORRECTOR_PROCESS:
		process_tick(self);
		break;
	case VOLTAGE_CORRECTOR_PROCESS_WAIT:
		process_wait_tick(self);
		break;
	case VOLTAGE_CORRECTOR_SUCCESS:
		break;
	case VOLTAGE_CORRECTOR_ERROR:
		break;
	}
}

voltage_corrector_state_t voltage_corrector_get_state(voltage_corrector_t* self)
{
	return self->state;
}

static void process_tick(voltage_corrector_t* self)
{
	if(target_voltage_reached(self))
	{
		self->state = VOLTAGE_CORRECTOR_SUCCESS;
	}
	else
	{
		if(!self->numOfCorrectionsRemained)
		{
			self->state = VOLTAGE_CORRECTOR_ERROR;
		}
		else
		{
			self->correctionTicksWaitRemained = self->correctionTicksWaitMax;
			self->state = VOLTAGE_CORRECTOR_PROCESS_WAIT;
			if(voltage_stable(self) || (self->numOfCorrectionsRemained == self->numOfCorrectionsMax))
			{
				correction_step(self);
				self->numOfCorrectionsRemained--;
			}
		}
	}
}

static void process_wait_tick(voltage_corrector_t* self)
{
	if(!self->correctionTicksWaitRemained--)
	{
		self->state = VOLTAGE_CORRECTOR_PROCESS;
	}
}

static uint8_t target_voltage_reached(voltage_corrector_t* self)
{
	return (abs(self->actualVoltage - self->targetVoltage) <= 1) && voltage_stable(self);
}

static int max_voltage(voltage_corrector_t* self)
{
	int max = 0;
	if(VOLTAGE_STABILITY_ARRAY_SIZE > 1)
	{
		max = self->lastVoltage[0];
		int i;
		for (i = 1; i < VOLTAGE_STABILITY_ARRAY_SIZE; i++)
		{
			(self->lastVoltage[i] > max) ? max = self->lastVoltage[i] : 0;
		}
	}
	else
	{
		max = self->lastVoltage[0];
	}
	return max;
}
static int min_voltage(voltage_corrector_t* self)
{
	int min = 0;
	if(VOLTAGE_STABILITY_ARRAY_SIZE > 1)
	{
		min = self->lastVoltage[0];
		int i;
		for (i = 1; i < VOLTAGE_STABILITY_ARRAY_SIZE; i++)
		{
			(self->lastVoltage[i] < min) ? min = self->lastVoltage[i] : 0;
		}
	}
	else
	{
		min = self->lastVoltage[0];
	}
	return min;
}

static uint8_t voltage_stable(voltage_corrector_t* self)
{
	uint8_t stable = 1;
	if(VOLTAGE_STABILITY_ARRAY_SIZE > 0)
	{
		stable = ((max_voltage(self) - min_voltage(self)) <= 1);
	}
	else
	{
		stable = 1;
	}
	return stable;
}

static void voltage_stablility_check_tick(voltage_corrector_t* self)
{
	self->actualVoltage = self->get_voltage_cb();
	if(!self->stabilityCheckTicksRemained--)
	{
		self->lastVoltage[self->lastVoltage_index] = self->actualVoltage;
		self->lastVoltage_index = (self->lastVoltage_index + 1) % VOLTAGE_STABILITY_ARRAY_SIZE;
		self->stabilityCheckTicksRemained = self->stabilityCheckTicksMax;
	}
}

static void correction_step(voltage_corrector_t* self)
{
	if(self->numOfCorrectionsRemained == self->numOfCorrectionsMax) // first iteration
	{
		self->targetVoltageCurrentStep = self->targetVoltage;
	}
	else
	{
		self->targetVoltageCurrentStep += (int)round(kCorrection(self) * (self->targetVoltage - self->actualVoltage));
	}


	// set new voltage
	self->set_voltage_cb(self->targetVoltageCurrentStep);
}

static double kCorrection(voltage_corrector_t* self)
{
	static const double kMax = 1.2;
	static const double kMin = 1 / kMax;
	double k = 1;

	if(self->actualVoltage > 0)
	{
		k = (double)(self->targetVoltageCurrentStep) / (self->actualVoltage);
	}
	else
	{
		k = kMax;
	}

	if(k < kMin)
	{
		k = kMin;
	}

	if(k > kMax)
	{
		k = kMax;
	}
	return k;
}
