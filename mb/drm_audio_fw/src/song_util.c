#include <bearssl.h>

uint32_t decrypt_song() {
	char *test_key = "asdasdasdasd";
	char *test_iv = "asasdasdasdasd";
	uint32_t counter = 1;
	char *data = "test_data";
	size_t len = sizeof(data);
	uint32_t check = br_chacha20_ct_run(test_key, test_iv, counter, data, len);
	return check;
}

