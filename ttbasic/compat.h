#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN32) || defined(__APPLE__)
void *memrchr(const void *s, int c_in, size_t n);
#endif
#ifdef _WIN32
void *memmem(const void *haystack, size_t hs_len, const void *needle, size_t ne_len);
int setenv(const char *name, const char *value, int overwrite);
ssize_t getline(char **lineptr, size_t *n, FILE *stream);
#endif

#ifdef __cplusplus
}
#endif
