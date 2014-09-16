// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_CHRONO_H_
#define HANDYSTATS_CHRONO_H_

#include <stdint.h>

#include <handystats/common.h>

HANDYSTATS_EXTERN_C
int64_t handystats_now(void);

HANDYSTATS_EXTERN_C
double handystats_difftime(int64_t end, int64_t start);

#endif // HANDYSTATS_CHRONO_H_
