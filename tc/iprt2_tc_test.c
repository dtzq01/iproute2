#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>

#include "utils.h"
#include "libiprt2.h"
/*
uname -a:
Linux code 5.15.0-78-lowlatency #85-Ubuntu SMP PREEMPT Tue Jul 11 07:51:28 UTC 2023 x86_64 x86_64 x86_64 GNU/Linux
lscpu:
Architecture:            x86_64
  CPU op-mode(s):        32-bit, 64-bit
  Address sizes:         43 bits physical, 48 bits virtual
  Byte Order:            Little Endian
CPU(s):                  2
  On-line CPU(s) list:   0,1
Vendor ID:               GenuineIntel
  Model name:            13th Gen Intel(R) Core(TM) i5-13500HX

test result:
56000 times: cost 0.844619 s
28000 times: cost 0.457450 s
14000 times: cost 0.215492 s
7000 times: cost 0.116946 s
3500 times: cost 0.57812 s
*/
#define TEST_TIMES 4000
int main(int argc, char **argv)
{
	char input[][256] = {
		"tc qdisc del dev lo root",
		"tc qdisc add dev lo root handle 1: htb",
		"tc class add dev lo parent 1:0 classid 1:1 htb rate 20mbit ceil 20mbit burst 2mb cburst 2mb",
		"tc class add dev lo parent 1:1 classid 1:11 htb rate 5mbit ceil 20mbit burst 5kb cburst 2mb",
		"tc class add dev lo parent 1:11 classid 1:111 htb rate 1mbit ceil 20mbit burst 16kb cburst 2mb",
		"tc class add dev lo parent 1:111 classid 1:112 htb rate 1kbit ceil 20mbit  prio 7 burst 16kb cburst 2mb quantum 1600",
		"tc qdisc replace dev lo parent 1:112 fq_codel memory_limit 1.5mb flows 32 target 600ms interval 1s noecn",
		"tc class add dev lo parent 1:111 classid 1:113 htb rate 1kbit ceil 20mbit  prio 7 burst 16kb cburst 2mb quantum 3200",
		"tc qdisc replace dev lo parent 1:113 fq_codel memory_limit 1.5mb flows 32 target 600ms interval 1s noecn",
		"tc class add dev lo parent 1:111 classid 1:114 htb rate 1kbit ceil 20mbit  prio 6 burst 16kb cburst 2mb quantum 1600",
		"tc qdisc replace dev lo parent 1:114 fq_codel memory_limit 1.5mb flows 32 target 600ms interval 1s noecn",
		"tc filter add dev lo parent 1:0 protocol ip prio 1 u32 match ip dst 192.168.23.21/32 flowid 1:112",
		"tc filter add dev lo parent 1:0 protocol ip prio 1 u32 match ip dst 192.168.23.22/32 flowid 1:113",
		"tc filter add dev lo parent 1:0 protocol ip prio 1 u32 match ip dst 192.168.23.23/32 flowid 1:114"
	};
	struct timeval begin_tv, end_tv, diff_tv;
	gettimeofday(&begin_tv, NULL);
	for (int i = 0; i < TEST_TIMES; i++) {
		for (int j = 0; j < ARRAY_SIZE(input); j++) {
			tc_cmd_helper(input[j], strlen(input[j]));
		}
	}
	gettimeofday(&end_tv, NULL);
	timersub(&end_tv, &begin_tv, &diff_tv);
	printf("%d times: cost %ld.%ld s\n", TEST_TIMES * ARRAY_SIZE(input), diff_tv.tv_sec,
	       diff_tv.tv_usec);
	return 0;
}