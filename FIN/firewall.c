#include "firewall.h"

int32_t set_rule(int32_t idx, RuleFunction rule) {
    if (idx < 0 || idx >= MAX_RULES || rule == NULL) {
        return -1; // Invalid input
    }

    rules[idx] = rule;
    return 0; // Success
}

int32_t unset_rule(int32_t idx) {
    if (idx < 0 || idx >= MAX_RULES) {
        return -1; // Invalid input
    }

    rules[idx] = NULL;
    return 0; // Success
}

int32_t filter(const uint8_t *p_input_packet, const int32_t input_size, uint8_t **pp_output_packets, int32_t *p_output_size) {
    if (p_input_packet == NULL || input_size <= 0 || pp_output_packets == NULL || p_output_size == NULL) {
        return -1; // Invalid input
    }

    // Initialize output variables
    *pp_output_packets = NULL;
    *p_output_size = 0;

    // Apply rules
    for (int i = 0; i < MAX_RULES; ++i) {
        if (rules[i] != NULL) {
            int32_t result = rules[i](p_input_packet, input_size, pp_output_packets, p_output_size);
            if (result == 1) {
                // Packet dropped by the rule
                free(*pp_output_packets);
                *p_output_size = 0;
                return 1;
            } else if (result == -1) {
                // Invalid input, skip this rule
                continue;
            }
        }
    }

    return 0; // Success
}

int32_t example_rule(const uint8_t *p_input_packet, const int32_t input_size, uint8_t **pp_output_packet, int32_t *p_output_size) {
    // Example rule: Drop packets with even size
    if (input_size % 2 == 0) {
        *pp_output_packet = NULL;
        *p_output_size = 0;
        return 1; // Drop the packet
    }

    // Example rule: Pass packets with odd size
    *pp_output_packet = malloc(input_size);
    if (*pp_output_packet == NULL) {
        return -1; // Memory allocation error
    }

    for (int i = 0; i < input_size; ++i) {
        (*pp_output_packet)[i] = p_input_packet[i];
    }

    *p_output_size = input_size;
    return 0; // Pass the packet
}