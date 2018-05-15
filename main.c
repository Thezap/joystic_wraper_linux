#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>
#include <byteswap.h>
#include <stdbool.h>
#include <pthread.h>

#define ERROR_CODE 84
#define TO_200_RANGE 325

typedef struct {
	uint32_t time;     /* event timestamp in milliseconds */
	uint16_t value;    /* value */
	uint8_t type;      /* event type */
	uint8_t number;    /* axis/button number */
} js_event_t;

typedef struct joystick_s {
	bool button[12];
	int16_t joystick[8];
} joystick_t;

static joystick_t *get_joystick(void)
{
	static joystick_t joystick;

	return (&joystick);
}

void *read_data(void *param);

static int8_t on_reading(int8_t data)
{
	static int8_t val;

	if (data != 0)
		val = data;
	return (val);
}

void display_controler_data(void)
{
	uint64_t tmp;
	joystick_t *joystick = get_joystick();

	printf("\n\n\n");
	printf("Button:\n#0: %01hX #1: %01hX #2: %01hX #3: %01hX #4: %01hX #5: %01hX "
	       "#6: %01hX #7: %01hX #8: %01hX #9: %01hX #A: %01hX #B: %01hX\n",
		joystick->button[0], joystick->button[1], joystick->button[2],
		joystick->button[3], joystick->button[4], joystick->button[5],
		joystick->button[6], joystick->button[7], joystick->button[8],
		joystick->button[9], joystick->button[10],
		joystick->button[11]);
	printf("Joystick:\n");
	for (int i = 0; i < 8; i++) {
		printf("#%d:", i);
		tmp = ((joystick->joystick[i] / TO_200_RANGE) + 100);
		for (uint16_t x = 0; x < 200; x++) {
			if (x < tmp)
				printf("|");
			else
				printf(".");
		}
		printf("\n");
	}
}

void *read_data(void *param)
{
	int game_controler_fd;
	joystick_t *joystick;

	while (1) {
		while (access("/dev/input/js0", F_OK) == -1);

		while ((game_controler_fd = open("/dev/input/js0", O_RDONLY)) == -1);
		joystick = get_joystick();
		js_event_t raw_input;
		while (read(game_controler_fd, &raw_input,
			sizeof(js_event_t)) != -1) {
			if (raw_input.type == 1) {
				//			printf("button action\n");
				joystick->button[raw_input.number] = raw_input.value;
			}
			if (raw_input.type == 2) {
				//			printf("joysitck action\n");
				joystick->joystick[raw_input.number] = raw_input.value;
			}
			//		printf("%08hX %04hX %02hX %02hX\n", raw_input.time,
			//			raw_input.value, raw_input.type, raw_input.number);
			display_controler_data();
			if (on_reading(0) == -1)
				return (NULL);
		}
		close(game_controler_fd);
	}
	return (NULL);
}

int main()
{
	on_reading(1);
	pthread_t pthread;
	pthread_create(&pthread, NULL, &read_data, NULL);
	sleep(100);
	on_reading(-1);
	printf("%d\n", on_reading(0));
	pthread_join(pthread, NULL);
	return 0;
}