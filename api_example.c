#include <stdio.h>
#include <string.h>
#include <survive_api.h>
#include <os_generic.h>

static volatile int keepRunning = 1;

#ifdef __linux__

#include <assert.h>
#include <signal.h>
#include <stdlib.h>

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

	for (const SurviveSimpleObject *it = survive_simple_get_first_object(actx); it != 0;
		 it = survive_simple_get_next_object(actx, it)) {
		printf("Found '%s'\n", survive_simple_object_name(it));
	}

	FLT last_timecode_hmd = 0;
	while (survive_simple_wait_for_update(actx) && keepRunning) {
		for (const SurviveSimpleObject *it = survive_simple_get_next_updated(actx); it != 0;
			 it = survive_simple_get_next_updated(actx)) {
			SurvivePose pose;

			FLT timecode = survive_simple_object_get_latest_pose(it, &pose) - start_time;
			enum SurviveSimpleObject_type type = survive_simple_object_get_type(it);
			if (type == SurviveSimpleObject_HMD) {
				FLT timecode_hmd = survive_simple_object_get_latest_pose(it, &pose) - start_time;
				if (timecode_hmd != last_timecode_hmd) {
					printf("tc %f -> %f, diff %fms %f %f %f\n", last_timecode_hmd, timecode_hmd, (timecode_hmd - last_timecode_hmd) * 1000., pose.Pos[0], pose.Pos[1], pose.Pos[2]);
				}
				last_timecode_hmd = timecode_hmd;
			}
		}
	}
	printf("Cleaning up\n");
	survive_simple_close(actx);
	return 0;
}
