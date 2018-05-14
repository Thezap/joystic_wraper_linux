#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>
#include <byteswap.h>

#define ERROR_CODE 84

typedef struct {
	uint32_t time;     /* event timestamp in milliseconds */
	uint16_t value;    /* value */
	uint8_t type;      /* event type */
	uint8_t number;    /* axis/button number */
} js_event_t;

int main()
{
	int game_cotroler_fd = open("/dev/input/js0", O_RDONLY);

	if (game_cotroler_fd == -1) {
		fprintf(stderr, "Error: Controller not connected\n");
		return (ERROR_CODE);
	}
	js_event_t raw_input;
	while (1) {
		read(game_cotroler_fd, &raw_input, sizeof(js_event_t));
		printf("%08hX %04hX %02hX %02hX\n", raw_input.time, raw_input.value, raw_input.type, raw_input.number);
	}
	return 0;
}