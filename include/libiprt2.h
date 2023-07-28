#pragma once

#define MAX_CMD_LEN 256
#define MAX_PARAM_NUM 32

/*                               libiprt_tc.so                              */
int tc_cmd_helper(char *cmd, unsigned int cmd_len);

/*                               libiprt_ip.so                              */
int ip_cmd_helper(char *cmd, unsigned int cmd_len);

//used for int main(int argc, char** argv) => int tc_cmd_helper(char *cmd, unsigned int cmd_len)
#define libiprt2_main_2_cmdhelper()                                            \
	if (cmd_len >= MAX_CMD_LEN)                                            \
		return E2BIG;                                                  \
	char *argv1[MAX_PARAM_NUM] = {};                                       \
	char **argv = argv1;                                                   \
	const char *delimiters = " ";                                          \
	int i = 0;                                                             \
	char tmp[MAX_CMD_LEN] = {};                                            \
	memcpy(tmp, cmd, cmd_len);                                             \
	argv1[i] = strtok(tmp, delimiters);                                    \
	for (i = 1; i < MAX_PARAM_NUM; i++) {                                  \
		argv1[i] = strtok(NULL, delimiters);                           \
		if (NULL == argv1[i])                                          \
			break;                                                 \
	}                                                                      \
	int argc = i

;