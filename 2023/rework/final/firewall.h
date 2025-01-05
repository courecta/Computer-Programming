#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define MAX_RULES (100)
#define OFFSET (10)

int32_t set_rule(int32_t idx, int32_t (*rule)(const uint8_t* p_input_packet , const int32_t input_size , uint8_t** pp_output_packet , int32_t* p_output_size ));

// Set rule[idx] to NULL. If the inputs are invalid , return -1; otherwise , return 0.
int32_t unset_rule(int32_t idx);

/*
For every input packet , apply all rules on the packet and store every ouput
packet on the pp_output_packets.
If the inputs are invalid , return -1; otherwise , return 0.
*/
int32_t filter(const uint8_t *p_input_packets , const int32_t input_size , uint8_t **pp_output_packets , int32_t *p_output_size);