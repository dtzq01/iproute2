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
debug tool: sudo lsof -p $(ps aux | awk '$11 ~ /iprt2_ip_test/ {print $2}')

input1 test result:
800 times: cost 4.759194 s
400 times: cost 1.717103 s
200 times: cost 1.210179 s
80 times: cost 0.498144 s
input2 test result:
24000 times: cost 0.259868 s
48000 times: cost 0.508569 s
96000 times: cost 1.203290 s

*/
#define TEST_TIMES 10
//FIXME: Cannot open netlink socket: Too many open files
int main(int argc, char **argv)
{
	char input1[][MAX_CMD_LEN] = {
		"ip link del ens36.11",
		"ip link del vrf11",
		"ip link add link ens36 name ens36.11 type vlan id 11",
		"ip link add vrf11 type vrf table 11",
		"ip link set ens36.11 master vrf11",
		"ip link set vrf11 up",
		"ip link set ens36.11 up",
		"ip addr add 192.168.11.3/24 dev ens36.11"
	};

	// char input2[][MAX_CMD_LEN] = {
	// 	"ip route del 192.168.1.1/32 dev ens36",
	// 	"ip route del 192.168.1.2/32 dev ens36",
	// 	"ip route del 192.168.1.3/32 dev ens36",
	// 	"ip route del 192.168.1.4/32 dev ens36",
	// 	"ip route del 192.168.1.5/32 dev ens36",
	// 	"ip route del 192.168.1.6/32 dev ens36",
	// 	"ip route del 192.168.1.7/32 dev ens36",
	// 	"ip route del 192.168.1.8/32 dev ens36",
	// 	"ip route del 192.168.1.9/32 dev ens36",
	// 	"ip route del 192.168.1.10/32 dev ens36",
	// 	"ip route del 192.168.1.11/32 dev ens36",
	// 	"ip route del 192.168.1.12/32 dev ens36",
	// 	"ip route add 192.168.1.1/32 dev ens36",
	// 	"ip route add 192.168.1.2/32 dev ens36",
	// 	"ip route add 192.168.1.3/32 dev ens36",
	// 	"ip route add 192.168.1.4/32 dev ens36",
	// 	"ip route add 192.168.1.5/32 dev ens36",
	// 	"ip route add 192.168.1.6/32 dev ens36",
	// 	"ip route add 192.168.1.7/32 dev ens36",
	// 	"ip route add 192.168.1.8/32 dev ens36",
	// 	"ip route add 192.168.1.9/32 dev ens36",
	// 	"ip route add 192.168.1.10/32 dev ens36",
	// 	"ip route add 192.168.1.11/32 dev ens36",
	// 	"ip route add 192.168.1.12/32 dev ens36",
	// };
	int cmd_num = ARRAY_SIZE(input1);
	struct timeval begin_tv, end_tv, diff_tv;
	gettimeofday(&begin_tv, NULL);
	for (int i = 0; i < TEST_TIMES; i++) {
		for (int j = 0; j < cmd_num; j++) {
			char *input = input1[j];
			ip_cmd_helper(input, strlen(input));
		}
	}

	gettimeofday(&end_tv, NULL);
	timersub(&end_tv, &begin_tv, &diff_tv);
	printf("%d (%d * %d) times: cost %ld.%ld s\n", TEST_TIMES * cmd_num,
	       TEST_TIMES, cmd_num, diff_tv.tv_sec, diff_tv.tv_usec);
	return 0;
}