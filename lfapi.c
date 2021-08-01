
#include "lfapi.h"
#include "../lfkmod/lfmod_public.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <unistd.h>

struct lf_device {
    int fp;
};

// device handle memory management
struct lf_device *lfapi_createHandle()
{
    struct lf_device *dev = malloc(sizeof(struct lf_device));

    dev->fp = 0;

    return dev;
}

struct lf_device *lfapi_destroyHandle(struct lf_device *dev)
{
    if (dev) {
        free(dev);
    }

    return NULL;
}

// basic device management
int lfapi_open(struct lf_device *dev, int idx)
{
    // argument sanity
    if (NULL == dev) {
        return LFAPI_ERR_NULL_ARGUMENT;
    }
    if (idx < 0) {
        return LFAPI_ERR_OUT_OF_RANGE;
    }

    if (dev->fp) {
        return LFAPI_ERR_ALREADY_OPEN;
    }

    // open file handle
    dev->fp = open("/dev/litefury", O_RDWR);
    if (dev->fp < 0) {
        return LFAPI_ERR_OPEN;
    }

    return LFAPI_SUCCESS;
}

int lfapi_close(struct lf_device *dev)
{
    // argument sanity
    if (NULL == dev) {
        return LFAPI_ERR_NULL_ARGUMENT;
    }

    if (dev->fp) {
        int rc = close(dev->fp);
        if (rc < 0) {
            return LFAPI_ERR_CLOSE;
        }
    }
    dev->fp = 0;

    return LFAPI_SUCCESS;
}

// low-level register i/o
int lfapi_registerRead(struct lf_device *dev, uint32_t address, uint32_t *data)
{
    // argument sanity
    if (NULL == dev || NULL == data) {
        return LFAPI_ERR_NULL_ARGUMENT;
    }

    struct lfmod_register_io reg = {.offset = address, .data = 0, .mask = 0};
    int rc = ioctl(dev->fp, LFMOD_REGISTER_READ, &reg);
    if (rc) {
        return LFAPI_ERR_IOCTL;
    }

    *data = reg.data;

    return LFAPI_SUCCESS;
}

int lfapi_registerWrite(struct lf_device *dev, uint32_t address, uint32_t data)
{
    // argument sanity
    if (NULL == dev) {
        return LFAPI_ERR_NULL_ARGUMENT;
    }

    struct lfmod_register_io reg = {.offset = address, .data = data, .mask = 0};
    int rc = ioctl(dev->fp, LFMOD_REGISTER_WRITE, &reg);
    if (rc) {
        return LFAPI_ERR_IOCTL;
    }

    return LFAPI_SUCCESS;
}

int lfapi_registerRMW(struct lf_device *dev, uint32_t address, uint32_t mask, uint32_t data)
{
    // argument sanity
    if (NULL == dev) {
        return LFAPI_ERR_NULL_ARGUMENT;
    }

    struct lfmod_register_io reg = {.offset = address, .data = data, .mask = mask};
    int rc = ioctl(dev->fp, LFMOD_REGISTER_RMW, &reg);
    if (rc) {
        return LFAPI_ERR_IOCTL;
    }

    return LFAPI_SUCCESS;
}
