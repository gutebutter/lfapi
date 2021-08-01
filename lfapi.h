#pragma once

#include <stdint.h>

// the device handle
struct lf_device;

// return codes
enum
{
    LFAPI_SUCCESS = 0,
    LFAPI_ERR_NULL_ARGUMENT,
    LFAPI_ERR_OUT_OF_RANGE,
    LFAPI_ERR_OPEN,
    LFAPI_ERR_CLOSE,
    LFAPI_ERR_IOCTL,
    LFAPI_ERR_ALREADY_OPEN,
};

#define LFAPI_OK(x)  (x) == LFAPI_SUCCESS
#define LFAPI_ERR(x) (!LFAPI_OK(x))

// device handle memory management
struct lf_device *lfapi_createHandle(void);
struct lf_device *lfapi_destroyHandle(struct lf_device *dev);

// basic device management
int lfapi_open(struct lf_device *dev, int idx);
int lfapi_close(struct lf_device *dev);

// low-level register i/o
int lfapi_registerRead(struct lf_device *dev, uint32_t address, uint32_t *data);
int lfapi_registerWrite(struct lf_device *dev, uint32_t address, uint32_t data);
int lfapi_registerRMW(struct lf_device *dev, uint32_t address, uint32_t mask, uint32_t data);
