#pragma once

#include <stdint.h>
#include "linuxlist.h"

typedef struct _sJob
{
    uint32_t id;
    uint32_t arrival;
    uint32_t duration;

    struct list_head list;
} sJob;

typedef struct _sSchedJob
{
    uint32_t id;
    uint32_t start;
    uint32_t stop;

    struct list_head list;
} sSchedJob;

// Add pJob to pJobList.
// Return -1 if there is any NULL pointer; otherwise, return 0.
int add_job(struct list_head *pJobList, sJob *pJob);

// According to pJobList, schedule all jobs and stored in pSchedJobList.
// All data stored in pSchedJobList is sSchedJob.
// Note that pSchedJobList should be sorted in the start time ascending order.
// Return -1 if there is any NULL pointer; otherwise, return 0.
// Each entry in pJobList is sJob while each entry in pSchedJobList is sSchedJob
int schedule(struct list_head *pJobList, struct list_head *pSchedJobList);

// Calculate the average turn around time.
// Turn around time is is the time interval from the time of coming of a job
// to the time of the completion of the job.
// The result is stored in *pTat.
// Return -1 if there is any NULL pointer; otherwise, return 0.
int tat(double *pTat, struct list_head *pJobList, struct list_head *pSchedJobList);

// Calculate the average waiting time.
// Waiting time is is the time spent by a job waiting in the queue for getting executed.
// Waiting time = turn around time - working duration.
// The result is stored in *pWt.
// Return -1 if there is any NULL pointer; otherwise, return 0.
int wt(double *pWt, struct list_head *pJobList, struct list_head *pSchedJobList);