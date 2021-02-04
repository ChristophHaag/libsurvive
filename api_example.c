#include <stdio.h>
#include <string.h>
#define SURVIVE_ENABLE_FULL_API 1
#include <survive_api.h>
#include "survive.h"
#include <os_generic.h>

static volatile int keepRunning = 1;

#ifdef __linux__

#include <assert.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>

void intHandler(int dummy) {
	if (keepRunning == 0)
		exit(-1);
	keepRunning = 0;
}

#endif

static void log_fn(SurviveSimpleContext *actx, SurviveLogLevel logLevel, const char *msg) {
	fprintf(stderr, "SimpleApi: %s\n", msg);
}

int main(int argc, char **argv) {
#ifdef __linux__
	signal(SIGINT, intHandler);
	signal(SIGTERM, intHandler);
	signal(SIGKILL, intHandler);
#endif

	SurviveSimpleContext *actx = survive_simple_init_with_logger(argc, argv, log_fn);
	if (actx == 0) // implies -help or similiar
		return 0;

	double start_time = OGGetAbsoluteTime();
	survive_simple_start_thread(actx);
	struct timespec start, end;
	clock_gettime(CLOCK_MONOTONIC_RAW, &start);

	for (const SurviveSimpleObject *it = survive_simple_get_first_object(actx); it != 0;
		 it = survive_simple_get_next_object(actx, it)) {
		printf("Found '%s'\n", survive_simple_object_name(it));
	}

	float last = 0;
	while (keepRunning) {
		clock_gettime(CLOCK_MONOTONIC_RAW, &end);
		double delta_us = (end.tv_sec - start.tv_sec) * 1000000. + (end.tv_nsec - start.tv_nsec) / 1000.;
		if (delta_us / 1000. - last > 10) {
			printf("%f ms\n", delta_us / 1000.);
			last = delta_us / 1000.;
		}

		for (const SurviveSimpleObject *it = survive_simple_get_next_updated(actx); it != 0;
			 it = survive_simple_get_next_updated(actx)) {
			SurvivePose pose;
			FLT timecode = survive_simple_object_get_latest_pose(it, &pose) - start_time;

			printf("it %p\n", it);
			SurviveObject *so = survive_simple_get_survive_object(it);
			if (so && so->conf != 0) {
				printf("%p Have config %p\n", it, so->conf);
			}

			enum SurviveSimpleObject_type type = survive_simple_object_get_type(it);
				if (type == SurviveSimpleObject_HMD && so && so->conf != 0) {
					printf("%p got HMD config %p after %f ms\n", it, so->conf, delta_us / 1000.);
					return 0;
				}
			}


	}
	printf("Cleaning up\n");
	survive_simple_close(actx);
	return 0;
}
