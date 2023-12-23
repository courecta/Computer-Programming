#include "firewall.h"

int main()
{
    // Set an example rule
    set_rule(0, example_rule);

    // Example input packet
    uint8_t input_packet[] = {1, 2, 3, 4, 5};

    // Apply rules
    uint8_t *output_packet;
    int32_t output_size;
    int32_t result = filter(input_packet, sizeof(input_packet), &output_packet, &output_size);

    if (result == 0)
    {
        printf("Filtered Packet Size: %d\n", output_size);
        for (int i = 0; i < output_size; ++i)
        {
            printf("%d ", output_packet[i]);
        }
        printf("\n");
    } else if (result == 1)
    {
        printf("Packet Dropped by Rule.\n");
    } else
    {
        printf("Error: Invalid Input.\n");
    }

    // Unset the example rule
    unset_rule(0);

    // Free memory
    free(output_packet);

    return 0;
}