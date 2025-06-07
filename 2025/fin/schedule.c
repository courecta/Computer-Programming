#include "schedule.h"
#include <stdlib.h>
#include <string.h>

// Add pJob to pJobList.
// Return -1 if there is any NULL pointer; otherwise, return 0.
int add_job(struct list_head *pJobList, sJob *pJob)
{
    if (pJobList == NULL || pJob == NULL) {
        return -1;
    }
    
    list_add_tail(&pJob->list, pJobList);
    return 0;
}

// Helper structure to track jobs during scheduling
typedef struct {
    uint32_t id;
    uint32_t arrival;
    uint32_t remaining_duration;
    uint32_t original_duration;
} JobTracker;

// According to pJobList, schedule all jobs and stored in pSchedJobList.
// All data stored in pSchedJobList is sSchedJob.
// Note that pSchedJobList should be sorted in the start time ascending order.
// Return -1 if there is any NULL pointer; otherwise, return 0.
int schedule(struct list_head *pJobList, struct list_head *pSchedJobList)
{
    if (pJobList == NULL || pSchedJobList == NULL) {
        return -1;
    }
    
    // Count jobs
    int job_count = 0;
    struct list_head *pos;
    list_for_each(pos, pJobList) {
        job_count++;
    }
    
    if (job_count == 0) {
        return 0;
    }
    
    // Create job trackers array
    JobTracker *jobs = malloc(job_count * sizeof(JobTracker));
    if (jobs == NULL) {
        return -1;
    }
    
    // Initialize job trackers
    int i = 0;
    sJob *job;
    list_for_each_entry(job, pJobList, list) {
        jobs[i].id = job->id;
        jobs[i].arrival = job->arrival;
        jobs[i].remaining_duration = job->duration;
        jobs[i].original_duration = job->duration;
        i++;
    }
    
    uint32_t current_time = 0;
    int running_job = -1;
    uint32_t job_start_time = 0;
    int completed_jobs = 0;
    
    while (completed_jobs < job_count) {
        // First, handle any zero-duration jobs that have arrived at current time
        for (int j = 0; j < job_count; j++) {
            if (jobs[j].arrival <= current_time && jobs[j].original_duration == 0 && jobs[j].remaining_duration == 0) {
                // Handle zero-duration job immediately
                sSchedJob *sched_job = malloc(sizeof(sSchedJob));
                if (sched_job == NULL) {
                    free(jobs);
                    return -1;
                }
                sched_job->id = jobs[j].id;
                sched_job->start = current_time;
                sched_job->stop = current_time;
                INIT_LIST_HEAD(&sched_job->list);
                list_add_tail(&sched_job->list, pSchedJobList);
                
                jobs[j].remaining_duration = UINT32_MAX; // Mark as completed (use max value to avoid confusion)
                completed_jobs++;
            }
        }
        
        // Find all jobs that have arrived and are not completed
        int best_job = -1;
        uint32_t shortest_duration = UINT32_MAX;
        
        for (int j = 0; j < job_count; j++) {
            if (jobs[j].arrival <= current_time && jobs[j].remaining_duration > 0 && jobs[j].remaining_duration != UINT32_MAX) {
                if (jobs[j].remaining_duration < shortest_duration) {
                    shortest_duration = jobs[j].remaining_duration;
                    best_job = j;
                }
            }
        }
        
        // If no job is available, advance time to the next job arrival
        if (best_job == -1) {
            uint32_t next_arrival = UINT32_MAX;
            for (int j = 0; j < job_count; j++) {
                if (jobs[j].remaining_duration > 0 && jobs[j].remaining_duration != UINT32_MAX && jobs[j].arrival > current_time) {
                    if (jobs[j].arrival < next_arrival) {
                        next_arrival = jobs[j].arrival;
                    }
                }
            }
            if (next_arrival != UINT32_MAX) {
                current_time = next_arrival;
            } else {
                // No more jobs to process
                break;
            }
            continue;
        }
        
        // Check if we need to preempt the current running job
        if (running_job != -1 && running_job != best_job) {
            // Complete the current job segment
            sSchedJob *sched_job = malloc(sizeof(sSchedJob));
            if (sched_job == NULL) {
                free(jobs);
                return -1;
            }
            
            sched_job->id = jobs[running_job].id;
            sched_job->start = job_start_time;
            sched_job->stop = current_time;
            
            list_add_tail(&sched_job->list, pSchedJobList);
        }
        
        // Start the new job or continue with the same job
        if (running_job != best_job) {
            running_job = best_job;
            job_start_time = current_time;
        }
        
        // Find the next event time (either job completion or new job arrival that can preempt)
        uint32_t next_event_time = current_time + jobs[best_job].remaining_duration;
        
        // Check for job arrivals that might preempt this job
        for (int j = 0; j < job_count; j++) {
            if (j != best_job && jobs[j].remaining_duration > 0 && jobs[j].remaining_duration != UINT32_MAX) {
                if (jobs[j].arrival > current_time && jobs[j].arrival < next_event_time) {
                    // Check if this new job would preempt the current one
                    if (jobs[j].remaining_duration < jobs[best_job].remaining_duration) {
                        next_event_time = jobs[j].arrival;
                        break;
                    }
                }
            }
        }
        
        // Execute the job until next event
        uint32_t execution_time = next_event_time - current_time;
        jobs[best_job].remaining_duration -= execution_time;
        current_time = next_event_time;
        
        // If job is completed, create final schedule entry
        if (jobs[best_job].remaining_duration == 0) {
            sSchedJob *sched_job = malloc(sizeof(sSchedJob));
            if (sched_job == NULL) {
                free(jobs);
                return -1;
            }
            
            sched_job->id = jobs[best_job].id;
            sched_job->start = job_start_time;
            sched_job->stop = current_time;
            
            list_add_tail(&sched_job->list, pSchedJobList);
            
            completed_jobs++;
            running_job = -1;
        }
    }
    
    free(jobs);
    return 0;
}

// Calculate the average turn around time.
int tat(double *pTat, struct list_head *pJobList, struct list_head *pSchedJobList)
{
    if (pTat == NULL || pJobList == NULL || pSchedJobList == NULL) {
        return -1;
    }
    
    // Count jobs
    int job_count = 0;
    struct list_head *pos;
    list_for_each(pos, pJobList) {
        job_count++;
    }
    
    if (job_count == 0) {
        *pTat = 0.0;
        return 0;
    }
    
    double total_tat = 0.0;
    sJob *job;
    
    list_for_each_entry(job, pJobList, list) {
        // Find the completion time of this job
        uint32_t completion_time = 0;
        sSchedJob *sched_job;
        
        list_for_each_entry(sched_job, pSchedJobList, list) {
            if (sched_job->id == job->id && sched_job->stop > completion_time) {
                completion_time = sched_job->stop;
            }
        }
        
        // Turn around time = completion time - arrival time
        double job_tat = (double)(completion_time - job->arrival);
        total_tat += job_tat;
    }
    
    *pTat = total_tat / job_count;
    return 0;
}

// Calculate the average waiting time.
int wt(double *pWt, struct list_head *pJobList, struct list_head *pSchedJobList)
{
    if (pWt == NULL || pJobList == NULL || pSchedJobList == NULL) {
        return -1;
    }
    
    // Count jobs
    int job_count = 0;
    struct list_head *pos;
    list_for_each(pos, pJobList) {
        job_count++;
    }
    
    if (job_count == 0) {
        *pWt = 0.0;
        return 0;
    }
    
    double total_wt = 0.0;
    sJob *job;
    
    list_for_each_entry(job, pJobList, list) {
        // Find the completion time of this job
        uint32_t completion_time = 0;
        sSchedJob *sched_job;
        
        list_for_each_entry(sched_job, pSchedJobList, list) {
            if (sched_job->id == job->id && sched_job->stop > completion_time) {
                completion_time = sched_job->stop;
            }
        }
        
        // Turn around time = completion time - arrival time
        double job_tat = (double)(completion_time - job->arrival);
        // Waiting time = turn around time - working duration
        double job_wt = job_tat - (double)job->duration;
        total_wt += job_wt;
    }
    
    *pWt = total_wt / job_count;
    return 0;
}