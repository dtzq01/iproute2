#pragma once

#define MAX_CMD_LEN 256
#define MAX_PARAM_NUM 32

/*                               libiprt_tc.so                              */
int tc_cmd_helper(char *cmd, unsigned int cmd_len);

/*                               libiprt_ip.so                              */