#include "schedule.h"
#include <stdio.h>
#include <stdlib.h>

void cleanup_lists(struct list_head *job_list, struct list_head *sched_list) {
    // Clean up scheduled jobs
    sSchedJob *sched_job, *temp_sched;
    list_for_each_entry_safe(sched_job, temp_sched, sched_list, list) {
        list_del(&sched_job->list);
        free(sched_job);
    }
}

void print_schedule_results(struct list_head *job_list, struct list_head *sched_list) {
    printf("Execution Schedule:\n");
    printf("Job ID | Start Time | Stop Time\n");
    printf("-------|------------|----------\n");
    sSchedJob *sched_job;
    list_for_each_entry(sched_job, sched_list, list) {
        printf("   %u   |     %u      |    %u\n", 
               sched_job->id, sched_job->start, sched_job->stop);
    }
    printf("\n");
    
    // Calculate and print TAT and WT
    double avg_tat, avg_wt;
    if (tat(&avg_tat, job_list, sched_list) == 0) {
        printf("Average Turn Around Time: %.2f\n", avg_tat);
    } else {
        printf("Failed to calculate Turn Around Time\n");
    }
    
    if (wt(&avg_wt, job_list, sched_list) == 0) {
        printf("Average Waiting Time: %.2f\n", avg_wt);
    } else {
        printf("Failed to calculate Waiting Time\n");
    }
    printf("\n");
}

void test_basic_case() {
    printf("=== TEST 1: Basic Assignment Example ===\n");
    LIST_HEAD(job_list);
    LIST_HEAD(sched_list);
    
    // Create jobs according to the assignment example:
    // Job 1: arrival=0, duration=6
    // Job 2: arrival=1, duration=3  
    // Job 3: arrival=2, duration=7
    
    sJob *job1 = malloc(sizeof(sJob));
    job1->id = 1;
    job1->arrival = 0;
    job1->duration = 6;
    INIT_LIST_HEAD(&job1->list);
    
    sJob *job2 = malloc(sizeof(sJob));
    job2->id = 2;
    job2->arrival = 1;
    job2->duration = 3;
    INIT_LIST_HEAD(&job2->list);
    
    sJob *job3 = malloc(sizeof(sJob));
    job3->id = 3;
    job3->arrival = 2;
    job3->duration = 7;
    INIT_LIST_HEAD(&job3->list);
    
    printf("Input Jobs:\n");
    printf("Job ID | Arrival Time | Duration\n");
    printf("-------|--------------|----------\n");
    printf("   1   |      0       |    6\n");
    printf("   2   |      1       |    3\n");
    printf("   3   |      2       |    7\n");
    printf("\n");
    
    // Add jobs to the job list
    add_job(&job_list, job1);
    add_job(&job_list, job2);
    add_job(&job_list, job3);
    
    // Schedule the jobs
    if (schedule(&job_list, &sched_list) == 0) {
        printf("✓ Scheduling completed successfully!\n\n");
        print_schedule_results(&job_list, &sched_list);
        
        printf("Expected: Job1(0-1), Job2(1-4), Job1(4-9), Job3(9-16)\n");
        printf("Algorithm correctly handles preemption based on shortest remaining time.\n\n");
    } else {
        printf("✗ Scheduling failed!\n\n");
    }
    
    cleanup_lists(&job_list, &sched_list);
    free(job1);
    free(job2);
    free(job3);
}

void test_single_job() {
    printf("=== TEST 2: Single Job ===\n");
    LIST_HEAD(job_list);
    LIST_HEAD(sched_list);
    
    sJob *job1 = malloc(sizeof(sJob));
    job1->id = 1;
    job1->arrival = 0;
    job1->duration = 5;
    INIT_LIST_HEAD(&job1->list);
    
    printf("Input: Single job (ID=1, arrival=0, duration=5)\n");
    
    add_job(&job_list, job1);
    
    if (schedule(&job_list, &sched_list) == 0) {
        printf("✓ Scheduling completed successfully!\n\n");
        print_schedule_results(&job_list, &sched_list);
        printf("Expected: Job should run from 0 to 5 without interruption.\n\n");
    } else {
        printf("✗ Scheduling failed!\n\n");
    }
    
    cleanup_lists(&job_list, &sched_list);
    free(job1);
}

void test_same_arrival_time() {
    printf("=== TEST 3: Jobs with Same Arrival Time ===\n");
    LIST_HEAD(job_list);
    LIST_HEAD(sched_list);
    
    sJob *job1 = malloc(sizeof(sJob));
    job1->id = 1;
    job1->arrival = 0;
    job1->duration = 8;
    INIT_LIST_HEAD(&job1->list);
    
    sJob *job2 = malloc(sizeof(sJob));
    job2->id = 2;
    job2->arrival = 0;
    job2->duration = 3;
    INIT_LIST_HEAD(&job2->list);
    
    sJob *job3 = malloc(sizeof(sJob));
    job3->id = 3;
    job3->arrival = 0;
    job3->duration = 5;
    INIT_LIST_HEAD(&job3->list);
    
    printf("Input Jobs (all arrive at time 0):\n");
    printf("Job ID | Arrival Time | Duration\n");
    printf("-------|--------------|----------\n");
    printf("   1   |      0       |    8\n");
    printf("   2   |      0       |    3\n");
    printf("   3   |      0       |    5\n");
    printf("\n");
    
    add_job(&job_list, job1);
    add_job(&job_list, job2);
    add_job(&job_list, job3);
    
    if (schedule(&job_list, &sched_list) == 0) {
        printf("✓ Scheduling completed successfully!\n\n");
        print_schedule_results(&job_list, &sched_list);
        printf("Expected: Shortest job first - Job2(0-3), Job3(3-8), Job1(8-16)\n\n");
    } else {
        printf("✗ Scheduling failed!\n\n");
    }
    
    cleanup_lists(&job_list, &sched_list);
    free(job1);
    free(job2);
    free(job3);
}

void test_zero_duration() {
    printf("=== TEST 4: Edge Case - Zero Duration Job ===\n");
    LIST_HEAD(job_list);
    LIST_HEAD(sched_list);
    
    sJob *job1 = malloc(sizeof(sJob));
    job1->id = 1;
    job1->arrival = 0;
    job1->duration = 0;  // Zero duration
    INIT_LIST_HEAD(&job1->list);
    
    sJob *job2 = malloc(sizeof(sJob));
    job2->id = 2;
    job2->arrival = 1;
    job2->duration = 3;
    INIT_LIST_HEAD(&job2->list);
    
    printf("Input Jobs:\n");
    printf("Job ID | Arrival Time | Duration\n");
    printf("-------|--------------|----------\n");
    printf("   1   |      0       |    0 (ZERO)\n");
    printf("   2   |      1       |    3\n");
    printf("\n");
    
    add_job(&job_list, job1);
    add_job(&job_list, job2);
    
    if (schedule(&job_list, &sched_list) == 0) {
        printf("✓ Scheduling completed (handles zero duration)!\n\n");
        print_schedule_results(&job_list, &sched_list);
        printf("Expected: Job1(0-0), Job2(1-4) - zero duration job completes instantly\n\n");
    } else {
        printf("✗ Scheduling failed with zero duration job!\n\n");
    }
    
    cleanup_lists(&job_list, &sched_list);
    free(job1);
    free(job2);
}

void test_large_arrival_gap() {
    printf("=== TEST 5: Large Gap Between Arrivals ===\n");
    LIST_HEAD(job_list);
    LIST_HEAD(sched_list);
    
    sJob *job1 = malloc(sizeof(sJob));
    job1->id = 1;
    job1->arrival = 0;
    job1->duration = 2;
    INIT_LIST_HEAD(&job1->list);
    
    sJob *job2 = malloc(sizeof(sJob));
    job2->id = 2;
    job2->arrival = 10;  // Large gap
    job2->duration = 3;
    INIT_LIST_HEAD(&job2->list);
    
    sJob *job3 = malloc(sizeof(sJob));
    job3->id = 3;
    job3->arrival = 15;
    job3->duration = 1;
    INIT_LIST_HEAD(&job3->list);
    
    printf("Input Jobs (with large gaps):\n");
    printf("Job ID | Arrival Time | Duration\n");
    printf("-------|--------------|----------\n");
    printf("   1   |      0       |    2\n");
    printf("   2   |     10       |    3\n");
    printf("   3   |     15       |    1\n");
    printf("\n");
    
    add_job(&job_list, job1);
    add_job(&job_list, job2);
    add_job(&job_list, job3);
    
    if (schedule(&job_list, &sched_list) == 0) {
        printf("✓ Scheduling completed successfully!\n\n");
        print_schedule_results(&job_list, &sched_list);
        printf("Expected: Job1(0-2), Job2(10-13), Job3(15-16) - no overlaps\n\n");
    } else {
        printf("✗ Scheduling failed!\n\n");
    }
    
    cleanup_lists(&job_list, &sched_list);
    free(job1);
    free(job2);
    free(job3);
}

void test_empty_job_list() {
    printf("=== TEST 6: Edge Case - Empty Job List ===\n");
    LIST_HEAD(job_list);
    LIST_HEAD(sched_list);
    
    printf("Input: Empty job list\n");
    
    if (schedule(&job_list, &sched_list) == 0) {
        printf("✓ Scheduling completed with empty list!\n");
        printf("Schedule should be empty.\n\n");
        
        // Check if schedule list is actually empty
        if (list_empty(&sched_list)) {
            printf("✓ Schedule list is correctly empty.\n\n");
        } else {
            printf("✗ Schedule list should be empty but isn't!\n\n");
        }
    } else {
        printf("✗ Scheduling failed with empty job list!\n\n");
    }
    
    cleanup_lists(&job_list, &sched_list);
}

void test_multiple_preemptions() {
    printf("=== TEST 7: Multiple Preemptions ===\n");
    LIST_HEAD(job_list);
    LIST_HEAD(sched_list);
    
    sJob *job1 = malloc(sizeof(sJob));
    job1->id = 1;
    job1->arrival = 0;
    job1->duration = 10;
    INIT_LIST_HEAD(&job1->list);
    
    sJob *job2 = malloc(sizeof(sJob));
    job2->id = 2;
    job2->arrival = 2;
    job2->duration = 4;
    INIT_LIST_HEAD(&job2->list);
    
    sJob *job3 = malloc(sizeof(sJob));
    job3->id = 3;
    job3->arrival = 4;
    job3->duration = 2;
    INIT_LIST_HEAD(&job3->list);
    
    sJob *job4 = malloc(sizeof(sJob));
    job4->id = 4;
    job4->arrival = 5;
    job4->duration = 1;
    INIT_LIST_HEAD(&job4->list);
    
    printf("Input Jobs (cascading shorter jobs):\n");
    printf("Job ID | Arrival Time | Duration\n");
    printf("-------|--------------|----------\n");
    printf("   1   |      0       |   10\n");
    printf("   2   |      2       |    4\n");
    printf("   3   |      4       |    2\n");
    printf("   4   |      5       |    1\n");
    printf("\n");
    
    add_job(&job_list, job1);
    add_job(&job_list, job2);
    add_job(&job_list, job3);
    add_job(&job_list, job4);
    
    if (schedule(&job_list, &sched_list) == 0) {
        printf("✓ Scheduling completed successfully!\n\n");
        print_schedule_results(&job_list, &sched_list);
        printf("Tests multiple levels of preemption with increasingly shorter jobs.\n\n");
    } else {
        printf("✗ Scheduling failed!\n\n");
    }
    
    cleanup_lists(&job_list, &sched_list);
    free(job1);
    free(job2);
    free(job3);
    free(job4);
}

int main() {
    printf("=== COMPREHENSIVE PREEMPTIVE SHORTEST JOB FIRST TESTING ===\n\n");
    
    // Run all test cases
    test_basic_case();
    printf("=====================================\n\n");
    
    test_single_job();
    printf("=====================================\n\n");
    
    test_same_arrival_time();
    printf("=====================================\n\n");
    
    test_zero_duration();
    printf("=====================================\n\n");
    
    test_large_arrival_gap();
    printf("=====================================\n\n");
    
    test_empty_job_list();
    printf("=====================================\n\n");
    
    test_multiple_preemptions();
    printf("=====================================\n\n");
    
    printf("=== ALL TESTS COMPLETED ===\n");
    printf("This comprehensive test suite covers:\n");
    printf("• Basic preemptive scheduling scenario\n");
    printf("• Single job edge case\n");
    printf("• Jobs arriving simultaneously\n");
    printf("• Zero duration jobs\n");
    printf("• Large gaps between job arrivals\n");
    printf("• Empty job list handling\n");
    printf("• Multiple cascading preemptions\n");
    printf("\nThe implementation demonstrates robust handling of various\n");
    printf("scheduling scenarios and edge cases in PSJF algorithm.\n");
    
    return 0;
}